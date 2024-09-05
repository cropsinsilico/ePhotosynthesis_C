import os
import glob
import pprint
import argparse


_registry = {'parser': {}, 'generator': {}}


def registered_classes(typename, return_classes=False):
    global _registry
    if return_classes:
        return [v for k, v in _registry[typename].items() if k is not None]


def get_registered_class(typename, name):
    global _registry
    return _registry[typename][name]


class EnumMeta(type):

    def __new__(meta, name, bases, class_dict):
        cls = type.__new__(meta, name, bases, class_dict)
        global _registry
        if cls._type is not None:
            assert cls.name not in _registry[cls._type]
            _registry[cls._type][cls.name] = cls
        return cls


class EnumBase(metaclass=EnumMeta):

    _type = None
    name = None
    prefixes = {}
    suffixes = {}
    lowers = {}
    replacements = {}

    def __init__(self, src, kwargs):
        self.src = src
        self._current_keys = {}
        for k in ['prefixes', 'suffixes', 'lowers', 'replacements']:
            if k in kwargs:
                setattr(self, k, kwargs.pop(k))

    @property
    def current_key(self):
        return self._current_keys.get('k', None)

    def reset_current_keys(self):
        self._current_keys = {}

    def add_enum(self, k):
        self._current_keys = {
            'k': k,
            'kprefix': self.prefixes.get(k, []),
            'ksuffix': self.suffixes.get(k, []),
            'klower': self.lowers.get(k, False),
            'kreplacement': self.replacements.get(k, {}),
            'lastval': -1,
        }

    def add_member(self, member):
        raise NotImplementedError


class EnumParserBase(EnumBase):

    _type = 'parser'
    file_extension = None

    def __init__(self, src, is_regex=False, verbose=False, **kwargs):
        if is_regex:
            src = glob.glob(src)
            assert src
        else:
            src = [src]
        self.param = {}
        super(EnumParserBase, self).__init__(src, kwargs)
        for isrc in self.src:
            self.parse(isrc, **kwargs)
        if verbose:
            pprint.pprint(self.param)

    def add_enum(self, k):
        super(EnumParserBase, self).add_enum(k)
        self.param.setdefault(k, [])

    def add_member(self, member):
        assert self.current_key is not None
        if 'val' in member:
            member.setdefault('explicit_val', True)
        member.setdefault('val', str(self._current_keys['lastval']))
        member.setdefault('doc', '')
        member.setdefault('abbr', member['name'])
        for x in self._current_keys['kprefix']:
            member['abbr'] = member['abbr'].split(x)[-1]
        for x in self._current_keys['ksuffix']:
            member['abbr'] = member['abbr'].split(x)[0]
        if self._current_keys['klower']:
            member['abbr'] = member['abbr'].lower()
        member['abbr'] = self._current_keys['kreplacement'].get(
            member['abbr'], member['abbr'])
        if member['name']:
            self.param[self.current_key].append(member)
            self._current_keys['lastval'] = int(member['val'])

    def parse(self, src, **kwargs):
        with open(src, 'r') as fd:
            lines = fd.readlines()
        self.parse_lines(lines, **kwargs)

    def parse_lines(self, lines, **kwargs):
        raise NotImplementedError

    def prefix_by_split(self, split):
        out = {}
        for k, v in self.param.items():
            prefix = k.split(split, 1)[0]
            out[k] = [prefix]
        return out


class EnumGeneratorBase(EnumBase):

    _type = 'generator'
    prefix = []
    suffix = []
    file_prefix = ''
    file_suffix = ''
    file_extension = None
    skip = []
    skip_items = {}
    added_file_classes = {}
    explicit_dst = False
    perfile_options = {
        'dst': {
            'type': str,
            'help': "Location where generated file should be written"
        },
    }

    def __init__(self, src, dst=None, directory=None, added_files=None,
                 parent=None, child_kws={}, skip_children=False,
                 dont_generate=False, **kwargs):
        for k in self.perfile_options_keys():
            assert f'{k}_{self.name}' not in kwargs
        if dst is None:
            if self.explicit_dst:
                raise ValueError(f"Destination must be provided for "
                                 f"\'{self.name}\' {self._type}")
            assert self.file_suffix or self.file_extension
            src_parts = os.path.split(src.src[0])
            dst = directory if directory else src_parts[0]
            if self.file_prefix:
                dst = os.path.join(dst, self.file_prefix + src_parts[1])
            else:
                dst = os.path.join(dst, src_parts[1])
            if self.file_suffix:
                dst = self.file_suffix.join(os.path.splitext(dst))
            if self.file_extension:
                dst = os.path.splitext(dst)[0] + self.file_extension
        self.basename = os.path.basename(os.path.splitext(dst)[0])[-1]
        self.dst = dst
        self.parent = parent
        self.lines = []
        if added_files is None:
            added_files = {}
        self.added_files = added_files
        child_kws = self.extract_child_kws(kwargs, child_kws,
                                           directory=directory,
                                           dont_generate=dont_generate,
                                           parent=self)
        if not skip_children:
            for k, v in self.added_file_classes.items():
                kws = child_kws[v.name]
                if k == 'header' and 'dst' not in kws:
                    kws['dst'] = self.dst.replace(
                        self.file_extension, v.file_extension)
                if k not in self.added_files:
                    self.added_files[k] = v(src, child_kws=child_kws, **kws)
        super(EnumGeneratorBase, self).__init__(src, kwargs)
        if not dont_generate:
            self.lines = self.generate()
            self.write(**kwargs)

    @classmethod
    def add_arguments(cls, parser):
        for k, v in cls.perfile_options.items():
            key = f"{k}_{cls.name}"
            dest = v.get('dest', '')
            help_msg = v.get('help', '')
            if help_msg:
                v = dict(v, help=f"{cls.name}: {help_msg}")
            if dest:
                v = dict(v, dest=f"{dest}_{cls.name}")
            parser.add_argument(
                f"--{key.replace('_', '-')}", **v)

    @classmethod
    def get_arguments(cls, args, kwargs):
        for k, v in cls.perfile_options.items():
            if v.get('dest', ''):
                k = v['dest']
            key = f"{k}_{cls.name}"
            val = getattr(args, key, None)
            if val:
                kwargs[key] = val

    @classmethod
    def perfile_options_keys(cls):
        return ['kwargs'] + [
            v.get('dest', k) for k, v in cls.perfile_options.items()]

    @classmethod
    def all_children(cls, return_classes=False):
        if return_classes:
            out = list(cls.added_file_classes.values())
        else:
            out = [v.name for v in cls.added_file_classes.values()]
        for v in cls.added_file_classes.values():
            out += v.all_children(return_classes=return_classes)
        return out

    @classmethod
    def extract_child_kws(cls, kwargs, child_kws, **kws_all):
        kwargs_orig = dict(kwargs)
        out = child_kws
        kws_all.update(**child_kws.pop('all', {}))
        for xcls in cls.all_children(return_classes=True):
            x = xcls.name
            out.setdefault(x, {})
            for k in xcls.perfile_options_keys():
                key = f"{k}_{x}"
                if key in kwargs:
                    if k == 'kwargs':
                        out[x].update(**kwargs.pop(key))
                    else:
                        out[x][k] = kwargs.pop(key)
                elif key in kwargs_orig:
                    if k == 'kwargs':
                        out[x].update(**kwargs_orig[key])
                    else:
                        out[x][k] = kwargs_orig[key]
        kws_all = dict(kwargs, **kws_all)
        for x in cls.all_children():
            out[x] = dict(kws_all, **out[x])
        return out

    def write(self, verbose=False, overwrite=False, dry_run=False,
              append=False, append_unique=False):
        lines = self.prefix + self.lines + self.suffix
        if append_unique:
            append = True
        if self.name == 'global':
            assert append_unique
        if append and os.path.isfile(self.dst):
            with open(self.dst, 'r') as fd:
                existing = fd.read().splitlines()
            if append_unique:
                for x in lines:
                    if x not in existing:
                        existing.append(x)
                lines = existing
            else:
                lines = existing + lines
        contents = '\n'.join(lines) + '\n'
        if verbose:
            print(f"{self.dst}\n----------------\n{contents}")
        if dry_run:
            return
        if (not overwrite) and os.path.isfile(self.dst):
            raise AssertionError(f"{self.name} file {self.dst} "
                                 f"already exists")
        with open(self.dst, 'w') as fd:
            fd.write(contents)

    def max_width(self, members):
        width = len(self.add_member(
            max(members, key=lambda x:
                len(self.add_member(x, return_name=True))),
            return_name=True))
        return width

    def add_member(self, member, return_name=False):
        assert self.current_key is not None
        name = member['name']
        abbr = member.get('abbr', name)
        name = self._current_keys['kreplacement'].get(
            name, name)
        if self._current_keys['klower']:
            name = name.lower()
        if not member.get('no_suffix', False):
            for x in self._current_keys['ksuffix']:
                name = name + x
        if not member.get('no_prefix', False):
            for x in self._current_keys['kprefix']:
                name = x + name
        if return_name:
            return name
        return dict(member, name=name, abbr=abbr)

    def generate_member(self, x, **kwargs):
        raise NotImplementedError

    def generate_item(self, name, members, **kwargs):
        lines = []
        for x in members:
            if x['name'] in self.skip_items.get(name, []):
                continue
            x = self.add_member(x)
            lines += self.generate_member(x, **kwargs)
        return lines

    def generate(self, indent='', **kwargs):
        lines = []
        for k, v in self.src.param.items():
            if k in self.skip:
                continue
            self.add_enum(k)
            lines += [indent + x for x in
                      self.generate_item(k, v, **kwargs)]
        return lines


class CMixin:

    perfile_options = dict(
        EnumGeneratorBase.perfile_options,
        include_file={
            'action': 'append',
            'dest': 'include_files',
            'help': "File that should be included.",
        },
    )

    def __init__(self, *args, **kwargs):
        self.root_include_dir = kwargs.pop('root_include_dir', None)
        self.include_files = kwargs.pop('include_files', None)
        self.namespaces = kwargs.pop('namespaces', None)
        if self.include_files:
            assert isinstance(self.include_files, list)
        if ((self._type == 'generator'
             and (self.namespaces or self.root_include_dir))):
            kwargs.setdefault('child_kws', {})
            kwargs['child_kws'].setdefault('all', {})
            kwargs['child_kws']['all'].setdefault(
                'root_include_dir', self.root_include_dir)
            kwargs['child_kws']['all']['namespaces'] = self.namespaces
        super().__init__(*args, **kwargs)

    @classmethod
    def include_file(cls, x, caller=None, rootdir=None):
        if isinstance(x, EnumGeneratorBase):
            x = x.dst
        elif isinstance(x, EnumParserBase):
            x = x.src
        if rootdir is None and caller is not None:
            rootdir = os.path.commonprefix([x, caller])
        if rootdir is None:
            header_incl = os.path.basename(x)
        else:
            header_incl = os.path.relpath(x, rootdir)
        return [f"#include \"{header_incl}\"", ""]

    def add_include_files(self):
        lines = []
        if self.include_files:
            for x in self.include_files:
                lines += self.include_file(x, caller=self.dst,
                                           rootdir=self.root_include_dir)
            lines += ['']
        return lines

    def include_self(self, caller, rootdir=None):
        if rootdir is None:
            rootdir = self.root_include_dir
        return self.include_file(
            self.dst, caller=caller, rootdir=rootdir)

    def include_header(self, caller=None):
        if caller is None:
            caller = self.dst
        return self.include_file(
            self.added_files['header'].dst, caller=caller,
            rootdir=self.root_include_dir)

    def generate(self, indent='', **kwargs):
        lines = []
        lines += self.add_include_files()
        if self.namespaces:
            if self.file_extension in ['.hpp', '.h']:
                for x in self.namespaces:
                    lines += [f"{indent}namespace {x} {{"]
                    indent += 2 * ' '
            else:
                lines += [
                    f"using namespace {'::'.join(self.namespaces)};", ""]
        lines += super().generate(indent=indent, **kwargs)
        if self.namespaces and self.file_extension in ['.hpp', '.h']:
            for x in self.namespaces:
                indent = indent[:(len(indent) - 2)]
                lines += [f"{indent}}}"]
        return lines


class ParamFileParser(EnumParserBase):

    name = 'param'
    comment = '#'
    file_extension = '.txt'

    def parse(self, src, **kwargs):
        k = os.path.basename(os.path.splitext(src)[0])
        self.add_enum(k)
        out = super(ParamFileParser, self).parse(src, **kwargs)
        self.reset_current_keys()
        return out

    def parse_lines(self, lines):
        for line in lines:
            assert '//' not in line
            rem = line.split(self.comment, 1)
            if (not rem[0]) or rem[0].isspace():
                continue
            member = {}
            if len(rem) == 2:
                member['doc'] = rem[1].strip()
            rem = rem[0].split()
            member['name'] = rem[0].strip()
            self.add_member(member)


class CEnumParser(EnumParserBase):

    name = 'c'
    file_extension = '.cpp'

    def parse_lines(self, lines):
        i = 0
        while i < len(lines):
            if self.current_key is not None:
                if '}' in lines[i]:
                    self.reset_current_keys()
                else:
                    member = {}
                    rem = lines[i].split('//', 1)
                    if len(rem) == 2:
                        member['doc'] = rem[1].strip().rstrip('!<')
                    rem = rem[0].split(',')[0]
                    rem = rem.split("=", 1)
                    if len(rem) == 2:
                        member['val'] = eval(rem[1].strip().rstrip('LL'))
                    member['name'] = rem[0].strip()
                    self.add_member(member)
            elif lines[i].strip().startswith('enum'):
                k = lines[i].split('enum', 1)[1].split()[0].strip()
                self.add_enum(k)
                while '{' not in lines[i]:
                    i += 1
            i += 1


class CEnumGeneratorBase(CMixin, EnumGeneratorBase):

    name = 'c_base'
    file_suffix = '_enum'
    file_extension = '.hpp'
    prefix = [
        '#pragma once',
        '',
    ]


class CEnumGeneratorMapHeader(CEnumGeneratorBase):

    name = 'maps_header'
    file_suffix = '_maps'
    prefix = CEnumGeneratorBase.prefix + [
        '#include <map>',
        '#include <string>',
        '',
    ]

    def generate_item(self, name, members, tname=None, in_header=False):
        lines = []
        if tname is None:
            tname = name
        func_name = f"{name}_map"
        lines += [
            f"const std::map<const {tname}, "
            f"const std::string>& {func_name}();",
        ]
        return lines


class CEnumGeneratorMapSource(CMixin, EnumGeneratorBase):

    name = 'maps'
    file_suffix = '_maps'
    file_extension = '.cpp'
    added_file_classes = {
        'header': CEnumGeneratorMapHeader
    }

    def generate_member(self, x, width=None, width_abbr=None):
        assert width and width_abbr
        pad = (width_abbr - len(x['abbr'])) * ' '
        return [f"    {{{x['name']:{width}}, \"{x['abbr']}\"{pad}}},"]

    def generate_item(self, name, members, tname=None, in_header=False):
        lines = []
        if tname is None:
            tname = name
        func_name_decl = f"{name}_map"
        func_name = func_name_decl
        if self.namespaces and not in_header:
            func_name = f"{'::'.join(self.namespaces)}::{func_name}"
        func_decl = (
            f"const std::map<const {tname}, const std::string>& {func_name}()")
        lines += [
            f"{func_decl} {{"
            "",
            f"  static const std::map<const {tname}, const std::string> map {{"
        ]
        width = self.max_width(members)
        width_abbr = len(max(members, key=lambda x: len(x['abbr']))['abbr'])
        lines += super(CEnumGeneratorMapSource, self).generate_item(
            name, members, width=width, width_abbr=width_abbr)
        lines += [
            "  };", "  return map;", "};",
            f"template<> const std::map<const {tname}, "
            f"const std::string>& get_enum_map<enum {tname}>() {{",
            f"  return {func_name}();",
            "};",
            f"template<> MODULE get_enum_module<enum {tname}>() {{",
            f"  return MODULE_{name.split('_')[0]};",
            "}",
            ""
        ]
        return lines

    def generate(self, indent='', **kwargs):
        lines = self.include_header()
        lines += super(CEnumGeneratorMapSource, self).generate(
            indent=indent, **kwargs)
        return lines


class CEnumGeneratorGlobalHeader(CEnumGeneratorBase):

    name = 'global'
    file_suffix = ''
    explicit_dst = True
    perfile_options = dict(
        CEnumGeneratorBase.perfile_options,
        enum_name={
            'type': str,
            'help': "Name that should be used for the global enum",
        },
    )

    def __init__(self, *args, **kwargs):
        self.enum_name = kwargs.pop('enum_name', None)
        if self.enum_name is None:
            self.enum_name = self.name.upper()
        kwargs.setdefault("append_unique", True)
        super(CEnumGeneratorGlobalHeader, self).__init__(*args, **kwargs)
        assert self.parent

    def generate_item(self, name, members, width=None):
        assert width
        if self._current_keys['kprefix']:
            name = ''.join(self._current_keys['kprefix'][::-1]).rstrip('_')
        key = f"{self.enum_name}_{name}"
        return [f"{key:{width}},"]

    def generate(self, indent='', **kwargs):
        lines = []
        lines += self.parent.include_self(
            self.dst, rootdir=self.root_include_dir)

        def key_len(x):
            return len(''.join(self.prefixes.get(x, [x])))

        width = (
            key_len(max(self.src.param.keys(), key=key_len))
            + len(self.enum_name) + 1)
        first = f'{self.enum_name}_NONE'
        last = f'{self.enum_name}_MAX'
        lines += [
            f'enum {self.enum_name} {{',
            f'  {first:{width}},',
        ]
        indent += '  '
        lines += super(CEnumGeneratorGlobalHeader, self).generate(
            indent=indent, width=width, **kwargs)
        lines += [
            f'  {last:{width}},',
            "};",
            ""
        ]
        return lines


class CEnumGeneratorHeader(CEnumGeneratorBase):

    name = 'c'
    file_suffix = '_enum'
    added_file_classes = {
        'global': CEnumGeneratorGlobalHeader,
        'maps': CEnumGeneratorMapSource,
    }

    def __init__(self, *args, **kwargs):
        kwargs.setdefault('kwargs_maps_header', {})
        kwargs['kwargs_maps_header'].setdefault('include_files', [])
        kwargs['kwargs_maps_header']['include_files'].append(self)
        super(CEnumGeneratorHeader, self).__init__(*args, **kwargs)

    def generate_member(self, x, width=None, width_val=None):
        assert width is not None and width_val is not None
        val = ''
        docs = ''
        if x.get('explicit_val', False):
            val = f" = {x['val']}"
        if x.get('doc', False):
            docs = f"  //!< {x['doc']}"
        return [f"  {x['name']:{width}}{val:{width_val}},{docs}"]

    def generate_item(self, name, members):
        lines = []
        lines += [
            f"enum {name} {{",
        ]
        width = self.max_width(members)
        width_val = len(max(members, key=lambda x:
                            len(x.get('val', ''))).get('val', '')) + 3
        first = f"{name}_NONE"
        last = f"{name}_MAX"
        if members[0]['name'] != first:
            members.insert(0, {'name': first, 'abbr': first,
                               'no_prefix': True})
        if members[-1]['name'] != last:
            members.append({'name': last, 'abbr': last,
                            'no_prefix': True})
        lines += super(CEnumGeneratorHeader, self).generate_item(
            name, members, width=width, width_val=width_val)
        lines += [
            "};",
            ""
        ]
        return lines


class FortranEnumGeneratorCHeader(EnumGeneratorBase):

    name = 'fortran_wrapper_header'
    file_prefix = 'c_wrapper_'
    file_extension = '.h'
    types_cxx = {}

    def generate_member(self, x, tname=None):
        return [f"  FYGG_API extern const {tname} {x['name']}_F;"]

    def generate_item(self, name, members):
        lines = []
        tname = self.types_cxx.get(name, 'int')
        if tname == 'int':
            return lines
        lines.append('')
        lines += super(FortranEnumGeneratorCHeader, self).generate_item(
            name, members, tname=tname)
        lines.append('')
        return lines

    def generate(self, indent='', **kwargs):
        lines = [
            f'#ifndef {self.basename}_WRAPPER_H_',
            f'#define {self.basename}_WRAPPER_H_',
            '',
            '#ifdef __cplusplus',
            '#include <cstdint>',
            'extern "C" {',
            '#else',
            '#include "stdint.h"',
            '#endif',
        ]
        lines += super(FortranEnumGeneratorCHeader, self).generate(
            indent=indent, **kwargs)
        lines += [
            '',
            '#ifdef __cplusplus',
            '}',
            '#endif',
            '',
            f'#endif // {self.basename}_WRAPPER_H_',
        ]
        return lines


class FortranEnumGeneratorCSource(CMixin, EnumGeneratorBase):

    name = 'fortran_wrapper'
    file_prefix = 'c_wrapper_'
    file_extension = '.c'
    added_file_classes = {
        'header': FortranEnumGeneratorCHeader
    }

    @property
    def types_cxx(self):
        return self.added_files['header'].types_cxx

    @property
    def skip_items(self):
        return self.added_files['header'].skip_items

    def generate_member(self, x, tname=None):
        assert tname
        return [f"  const {tname} {x['name']}_F = {x['name']};"]

    def generate_item(self, name, members):
        lines = []
        tname = self.types_cxx.get(name, 'int')
        if tname == 'int':
            return lines
        lines.append('')
        lines += super(FortranEnumGeneratorCSource, self).generate_item(
            name, members, tname=tname)
        lines.append('')
        return lines

    def generate(self, indent='', **kwargs):
        lines = self.include_header()
        lines += self.src.include_header(caller=self.dst)
        lines += [
            '#ifdef __cplusplus',
            'extern "C" {',
            '#endif',
            ''
        ]
        lines += super(FortranEnumGeneratorCSource, self).generate(
            indent=indent, **kwargs)
        lines += [
            '',
            '#ifdef __cplusplus',
            '}',
            '#endif',
        ]
        return lines


# Version that binds to constants from C
class FortranEnumGeneratorBind(EnumGeneratorBase):

    name = 'fortran_bind'
    file_extension = '.F90'
    added_file_classes = {
        'c_wrapper': FortranEnumGeneratorCSource
    }
    prefix = [
        '#ifndef DOXYGEN_SHOULD_SKIP_THIS',
    ]
    suffix = [
        '#endif',
    ]

    @property
    def types_cxx(self):
        return self.added_files['c_wrapper'].types_cxx

    def generate_member(self, x, tname=None):
        assert tname
        if tname == 'int':
            return [f"        {x['name']} = {x['val']}, &"]
        else:
            return [
                f"integer(kind=c_{tname}), protected, "
                f"bind(c, name=\"{x['name']}_F\") :: {x['name']}"]

    def generate_item(self, name, members):
        lines = ['']
        tname = self.types_cxx.get(name, 'int')
        if tname == 'int':
            lines += [
                'enum, bind( C )',
                '   enumerator :: &',
            ]
        lines += super(FortranEnumGeneratorBind, self).generate_item(
            name, members, tname=tname)
        if tname == 'int':
            lines[-1] = lines[-1].split(',')[0]
            lines += [
                'end enum',
            ]
        lines += ['']
        return lines


# Version that just sets the values directly
class FortranEnumGeneratorValue(EnumGeneratorBase):

    name = 'fortran_value'
    file_extension = '.F90'
    added_file_classes = {
        'c_wrapper': FortranEnumGeneratorCSource
    }
    prefix = [
        '#ifndef DOXYGEN_SHOULD_SKIP_THIS',
    ]
    suffix = [
        '#endif',
    ]

    @property
    def types_cxx(self):
        return self.added_files['c_wrapper'].types_cxx

    def generate_member(self, x, tname=None, tsuffix=None):
        assert tname
        if tname == 'int':
            return [f"        {x['name']} = {x['val']}, &"]
        else:
            assert tsuffix
            return [
                f"integer(kind={tsuffix}), parameter :: "
                f"{x['name']} = {x['val']}_{tsuffix}"]

    def generate_item(self, name, members):
        lines = ['']
        tname = self.types_cxx.get(name, 'int')
        tsuffix = None
        if tname == 'int':
            lines += [
                '  enum, bind( C )',
                '     enumerator :: &',
            ]
        else:
            tsuffix = tname.split('_')[0]
        lines += super(FortranEnumGeneratorValue, self).generate_item(
            name, members, tname=tname, tsuffix=tsuffix)
        if tname == 'int':
            lines[-1] = lines[-1].split(',')[0]
            lines += [
                '  end enum',
            ]
        lines += ['']
        return lines


def rename_source(directory, src_suffix, dst_suffix, ext):
    import shutil
    src_regex = os.path.join(directory, f"*_{src_suffix}{ext}")
    files = sorted(glob.glob(src_regex))
    if not files:
        raise Exception(f"No files found matching {src_regex}")
    for src in files:
        dst = src.replace(src_suffix, dst_suffix)
        shutil.move(src, dst)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "Generate enum source code & header files based on "
        "the values defined in a C/C++ source file")
    parser.add_argument("src", type=str,
                        help=("Source file that enums should be "
                              "extracted from"))
    parser.add_argument("--dst", type=str,
                        help=("Location where generated file should be "
                              "written"))
    parser.add_argument("--src-type", type=str,
                        choices=registered_classes('parser'),
                        default='param',
                        help="Type of parser to use on src file")
    parser.add_argument("--dst-type", type=str,
                        choices=registered_classes('generator'),
                        default='maps',
                        help=("Type of generator that should be used "
                              "to create dst file"))
    parser.add_argument("--src-regex", action="store_true",
                        help=("Treat src as a regex to find file(s) to "
                              "parse"))
    parser.add_argument("--overwrite", action="store_true",
                        help="Overwrite any existing file(s)")
    parser.add_argument("--verbose", action="store_true",
                        help="Display the generated file content")
    parser.add_argument("--dry-run", action="store_true",
                        help="Run without writing any file(s)")
    parser.add_argument("--skip-children", action="store_true",
                        help="Don't generate the default child files")
    parser.add_argument("--root-include-dir", type=str,
                        help=("Root directory where include files are "
                              "located"))
    parser.add_argument("--prefix-with-type", action="store_true",
                        help="Prefix enums w/ their type")
    parser.add_argument("--prefix-by-split", type=str,
                        help=("Add prefixes to enum members by splitting "
                              "the name of the enum set by this string"))
    parser.add_argument("--rename-source", type=str,
                        help=("Rename source files in this directory "
                              "with the provided src suffix to use the "
                              "provided dst suffix"))
    for x in registered_classes('generator', return_classes=True):
        x.add_arguments(parser)
    args = parser.parse_args()
    if args.rename_source:
        src_cls = get_registered_class('parser', args.src_type)
        rename_source(args.rename_source, args.src, args.dst,
                      src_cls.file_extension)
    else:
        src = get_registered_class('parser', args.src_type)(
            args.src, verbose=args.verbose, is_regex=args.src_regex)
        kws = {}
        if args.prefix_by_split:
            assert not args.prefix_with_type
            kws['prefixes'] = src.prefix_by_split(args.prefix_by_split)
        elif args.prefix_with_type:
            kws['prefixes'] = {k: [f"{k}_"] for k in src.param.keys()}
        for x in registered_classes('generator', return_classes=True):
            x.get_arguments(args, kws)
        dst = get_registered_class('generator', args.dst_type)(
            src, dst=args.dst, overwrite=args.overwrite, verbose=args.verbose,
            dry_run=args.dry_run, skip_children=args.skip_children,
            root_include_dir=args.root_include_dir, **kws)
