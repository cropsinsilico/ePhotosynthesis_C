import os
import glob
import pprint
import argparse


_registry = {'parser': {}, 'generator': {}}


def registered_classes(typename):
    global _registry
    return [k for k in _registry[typename].keys() if k is not None]


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


class CMixin:

    def __init__(self, *args, **kwargs):
        self.root_include_dir = kwargs.pop('root_include_dir', None)
        kwargs.setdefault('child_kws', {})
        kwargs['child_kws'].setdefault(
            'root_include_dir', self.root_include_dir)
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

    def include_header(self, caller=None):
        if caller is None:
            caller = self.dst
        return self.include_file(
            self.added_files['header'].dst, caller=caller,
            rootdir=self.root_include_dir)


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
        for k, v in self.param.items():
            prefix = k.split(split, 1)[0]
            for x in v:
                x['name'] = prefix + x['name']


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

    def __init__(self, src, dst=None, directory=None, added_files=None,
                 parent=None, child_kws={}, skip_children=False,
                 dont_generate=False, **kwargs):
        for k in ['dst', 'kwargs']:
            assert f'{k}_{self.name}' not in kwargs
        if dst is None:
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
        child_kws = dict(self.extract_child_kws(kwargs), **child_kws)
        if not skip_children:
            child_kws = dict(kwargs, directory=directory,
                             dont_generate=dont_generate,
                             parent=self, **child_kws)
            for k, v in self.added_file_classes.items():
                kws = dict(child_kws)
                for x in ['dst', 'kwargs']:
                    xk = f'{x}_{v.name}'
                    if xk in child_kws:
                        if x == 'kwargs':
                            kws.update(**child_kws.pop(xk))
                        else:
                            kws[x] = child_kws.pop(xk)
                        kws.pop(xk, None)
                if k == 'header' and 'dst' not in kws:
                    kws['dst'] = self.dst.replace(
                        self.file_extension, v.file_extension)
                if k not in self.added_files:
                    self.added_files[k] = v(src, **kws)
        super(EnumGeneratorBase, self).__init__(src, kwargs)
        if not dont_generate:
            self.lines = self.generate()
            self.write(**kwargs)

    @classmethod
    def extract_child_kws(cls, kwargs):
        out = {}
        for k in [f"dst_{cls.name}", f"kwargs_{cls.name}"]:
            if k in kwargs:
                out[k] = kwargs.pop(k)
        for v in cls.added_file_classes.values():
            out.update(**v.extract_child_kws(kwargs))
        return out

    def write(self, verbose=False, overwrite=False, dry_run=False):
        contents = '\n'.join(
            self.prefix + self.lines + self.suffix) + '\n'
        if verbose:
            print(f"{self.dst}\n----------------\n{contents}")
        if dry_run:
            return
        if (not overwrite) and os.path.isfile(self.dst):
            raise AssertionError(f"{self.name} file {self.dst} "
                                 f"already exists")
        with open(self.dst, 'w') as fd:
            fd.write(contents)

    def add_member(self, member):
        assert self.current_key is not None
        member.setdefault('abbr', member['name'])
        member['name'] = self._current_keys['kreplacement'].get(
            member['name'], member['name'])
        if self._current_keys['klower']:
            member['name'] = member['name'].lower()
        for x in self._current_keys['ksuffix']:
            member['name'] = member['name'] + x
        for x in self._current_keys['kprefix']:
            member['name'] = x + member['name']

    def generate_member(self, x, **kwargs):
        raise NotImplementedError

    def generate_item(self, name, members, **kwargs):
        lines = []
        for x in members:
            if x['name'] in self.skip_items.get(name, []):
                continue
            self.add_member(x)
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


class ParamFileParser(EnumParserBase):

    name = 'param'
    comment = '#'

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

    def __init__(self, src, dst=None, namespaces=None, include_files=None,
                 **kwargs):
        self.namespaces = namespaces
        self.include_files = include_files
        kwargs.setdefault('child_kws', {})
        kwargs['child_kws']['namespaces'] = self.namespaces
        super(CEnumGeneratorBase, self).__init__(
            src, dst=dst, **kwargs)

    def generate(self, indent='', **kwargs):
        lines = []
        if self.include_files:
            for x in self.include_files:
                lines += self.include_file(x, caller=self.dst,
                                           rootdir=self.root_include_dir)
            lines += ['']
        if self.namespaces:
            for x in self.namespaces:
                lines += [f"{indent}namespace {x} {{"]
                indent += 2 * ' '
        lines += super(CEnumGeneratorBase, self).generate(
            indent=indent, **kwargs)
        if self.namespaces:
            for x in self.namespaces:
                indent = indent[:(len(indent) - 2)]
                lines += [f"{indent}}}"]
        return lines


class CEnumGeneratorMapHeader(CEnumGeneratorBase):

    name = 'maps_header'
    file_suffix = '_maps'
    file_extension = '.hpp'
    prefix = [
        '#pragma once',
        '',
        '#include <map>',
        '#include <string>',
        '',
    ]

    def generate_item(self, name, members, tname=None, in_header=False):
        lines = []
        if tname is None:
            tname = name
        func_name = f"{name}_map"
        lines.append(
            f"const std::map<const {tname}, "
            f"const std::string>& {func_name}();")
        return lines


class CEnumGeneratorMapSource(CMixin, EnumGeneratorBase):

    name = 'maps'
    file_suffix = '_maps'
    file_extension = '.cpp'
    added_file_classes = {
        'header': CEnumGeneratorMapHeader
    }

    def __init__(self, *args, **kwargs):
        self.namespaces = kwargs.pop('namespaces', None)
        kwargs.setdefault('child_kws', {})
        kwargs['child_kws']['namespaces'] = self.namespaces
        super(CEnumGeneratorMapSource, self).__init__(
            *args, **kwargs)

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
        width = len(max(members, key=lambda x: len(x['name']))['name'])
        width_abbr = len(max(members, key=lambda x: len(x['abbr']))['abbr'])
        lines += super(CEnumGeneratorMapSource, self).generate_item(
            name, members, width=width, width_abbr=width_abbr)
        lines += ["  };", "  return map;", "};", ""]
        return lines

    def generate(self, indent='', **kwargs):
        lines = self.include_header()
        if self.namespaces:
            lines += [
                f"using namespace {'::'.join(self.namespaces)};", ""]
        lines += super(CEnumGeneratorMapSource, self).generate(
            indent=indent, **kwargs)
        return lines


class CEnumGeneratorHeader(CEnumGeneratorBase):

    name = 'c'
    file_suffix = '_enum'
    file_extension = '.hpp'
    prefix = [
        '#pragma once',
        '',
    ]
    added_file_classes = {
        'maps': CEnumGeneratorMapSource
    }

    def __init__(self, *args, **kwargs):
        kwargs.setdefault('child_kws', {})
        kwargs['child_kws'].setdefault('kwargs_maps_header', {})
        kwargs['child_kws']['kwargs_maps_header'].setdefault(
            'include_files', [])
        kwargs['child_kws']['kwargs_maps_header']['include_files'].append(
            self)
        super(CEnumGeneratorHeader, self).__init__(*args, **kwargs)

    def generate_member(self, x, width=None, width_val=None):
        assert width is not None and width_val is not None
        val = ''
        docs = ''
        if x.get('explicit_val', False):
            val = f" = {x['val']}"
        if x.get('docs', False):
            docs = f"  //!< {x['docs']}"
        return [f"  {x['name']:{width}}{val:{width_val}},{docs}"]

    def generate_item(self, name, members):
        lines = []
        lines += [
            f"enum {name} {{"
        ]
        width = len(max(members, key=lambda x: len(x['name']))['name'])
        width_val = len(max(members, key=lambda x:
                            len(x.get('val', ''))).get('val', '')) + 3
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
    parser.add_argument("--prefix-by-split", type=str,
                        help=("Add prefixes to enum members by splitting "
                              "the name of the enum set by this string"))
    for x in registered_classes('generator'):
        parser.add_argument(f"--dst-{x.replace('_', '-')}", type=str,
                            help=(f"Location where generated {x} file "
                                  f"should be written"))
    args = parser.parse_args()
    src = get_registered_class('parser', args.src_type)(
        args.src, verbose=args.verbose, is_regex=args.src_regex)
    if args.prefix_by_split:
        src.prefix_by_split(args.prefix_by_split)
    kws = {}
    for x in registered_classes('generator'):
        k = f"dst_{x}"
        v = getattr(args, k, None)
        if v:
            kws[k] = v
    dst = get_registered_class('generator', args.dst_type)(
        src, dst=args.dst, overwrite=args.overwrite, verbose=args.verbose,
        dry_run=args.dry_run, skip_children=args.skip_children,
        root_include_dir=args.root_include_dir, **kws)
