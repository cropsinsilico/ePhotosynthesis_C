import os
import re
import glob
import pprint
import argparse
import copy


_registry = {'parser': {}, 'generator': {}}
_bitflags = [
    '0x00000000',
    '0x00000001',
    '0x00000002',
    '0x00000004',
    '0x00000008',
    '0x00000010',
    '0x00000020',
    '0x00000040',
    '0x00000080',
]


def registered_classes(typename, return_classes=False):
    global _registry
    if return_classes:
        return [v for k, v in _registry[typename].items() if k is not None]
    return [k for k in _registry[typename].keys() if k is not None]


def get_registered_class(typename, name):
    global _registry
    return _registry[typename][name]


def get_registered_value_keys():
    global _registry
    out = []
    for v in _registry['generator'].values():
        if getattr(v, 'default_value_flags', None):
            out += [vv.split(':')[-1] for vv in v.default_value_flags]
        elif getattr(v, 'default_value_key', None):
            out.append(v.default_value_key)
    return out


class EnumMeta(type):

    def __new__(meta, name, bases, class_dict):
        cls = type.__new__(meta, name, bases, class_dict)
        global _registry
        if ((cls._type is not None and cls.name is not None
             and not cls._dont_register)):
            if cls.name in _registry[cls._type]:
                raise AssertionError(f"{cls._type} \'{cls.name}\' "
                                     f"already registered")
            _registry[cls._type][cls.name] = cls
        cls.create_child_classes(cls)
        return cls


class EnumBase(metaclass=EnumMeta):

    _dont_register = False
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

    @staticmethod
    def create_child_classes(cls):
        pass

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
            'lastidx': -1,
        }

    def add_member(self, member):
        raise NotImplementedError

    def add_prefix(self, k, v):
        self.prefixes = dict(self.prefixes)
        self.prefixes.setdefault(k, [])
        if v not in self.prefixes[k]:
            self.prefixes[k].append(v)


class EnumParserBase(EnumBase):

    _type = 'parser'
    file_extension = None
    comment = None

    def __init__(self, src, is_regex=False, verbose=False, **kwargs):
        if is_regex:
            src = glob.glob(src)
            assert src
        else:
            src = [src]
        self.param = {}
        super(EnumParserBase, self).__init__(src, kwargs)
        for isrc in sorted(self.src):
            self.parse(isrc, **kwargs)
        if verbose:
            pprint.pprint(self.param)

    def add_enum(self, k):
        super(EnumParserBase, self).add_enum(k)
        self.param.setdefault(k, [])

    def add_member(self, member):
        assert self.current_key is not None
        if 'idx' in member:
            member.setdefault('explicit_idx', True)
        member.setdefault('idx', str(self._current_keys['lastidx']))
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
            self._current_keys['lastidx'] = int(member['idx'])

    def parse(self, src, **kwargs):
        with open(src, 'r') as fd:
            lines = fd.readlines()
        self.parse_lines(lines, src=src, **kwargs)

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
    added_section_classes = {}
    explicit_dst = False
    perfile_options = {
        'dst': {
            'type': str,
            'help': "Location where generated file should be written"
        },
        'append': {
            'type': str,
            'choices': ['nothing', 'direct', 'preserved', 'unique',
                        'discard'],
            'help': "How data should be appended to the file",
        },
        'expect_empty': {
            'action': 'store_true',
            'help': ("Assert that the generated file would not contain "
                     "any lines"),
        },
        'write_empty': {
            'action': 'store_true',
            'help': ("Create the generated file even if no lines are "
                     "generated"),
        },
    }
    disclaimer = [
        "This code is generated by generate_enum.py and should not be",
        "modified directly"
    ]
    _chunk_markers = {
        'preserved': (
            ["[BEGIN] PRESERVING CODE"],
            ["[END] PRESERVING CODE"],
        ),
        'discard': (
            ["[BEGIN] DISCARDING CODE"],
            ["[END] DISCARDING CODE"],
        ),
    }
    default_append = None

    def __init__(self, src, dst=None, directory=None, added_files=None,
                 parent=None, child_kws={}, skip_children=False,
                 dont_generate=False, added_sections=None,
                 top_level=False, **kwargs):
        for k in self.perfile_options_keys():
            key = f'{k}_{self.name}'
            if key in kwargs:
                if top_level:
                    kwargs[k] = kwargs.pop(key)
                else:
                    raise AssertionError(f'File specific keyword '
                                         f'argument present: {key}')
            if (k in kwargs) or (not hasattr(self, k)):
                setattr(self, k,
                        kwargs.pop(
                            k, getattr(self, f'default_{k}', None)))
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
        if added_sections is None:
            added_sections = {}
        self.added_sections = added_sections
        child_kws = self.extract_child_kws(kwargs, child_kws,
                                           directory=directory,
                                           dont_generate=True)
        super(EnumGeneratorBase, self).__init__(src, kwargs)
        if not skip_children:
            for k, v in self.added_file_classes.items():
                kws = child_kws[v.name]
                if k == 'header' and 'dst' not in kws:
                    kws['dst'] = self.dst.replace(
                        self.file_extension, v.file_extension)
                if k not in self.added_files:
                    self.added_files[k] = v(src, child_kws=child_kws,
                                            parent=self, **kws)
            for k, v in self.added_section_classes.items():
                kws = child_kws[v.name]
                if k not in self.added_sections:
                    self.added_sections[k] = v(src, child_kws=child_kws,
                                               parent=self, **kws)
        self.lines = []
        self.write_kwargs = dict(kwargs, append=self.append)
        if not dont_generate:
            self.do_write()

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
    def get_arguments(cls, args, kwargs, top_level=False):
        for k, v in cls.perfile_options.items():
            if v.get('dest', ''):
                k = v['dest']
            key = f"{k}_{cls.name}"
            val = getattr(args, key, None)
            if val is not None:
                if top_level:
                    kwargs[k] = val
                else:
                    kwargs[key] = val

    @classmethod
    def perfile_options_keys(cls):
        return ['kwargs'] + [
            v.get('dest', k) for k, v in cls.perfile_options.items()]

    # @classmethod
    # def add_class_defaults(cls, kwargs, keys, allow_missing=False):
    #     for k in keys:
    #         if allow_missing and not hasattr(cls, f'default_{k}'):
    #             continue
    #         kwargs.setdefault(k, getattr(cls, f'default_{k}'))

    # @classmethod
    # def add_perfile_defaults(cls, kwargs):
    #     cls.add_class_defaults(kwargs, list(cls.perfile_options.keys()),
    #                            allow_missing=True)

    @classmethod
    def all_children(cls, return_classes=False):
        if return_classes:
            out = (
                list(cls.added_file_classes.values())
                + list(cls.added_section_classes.values())
            )
        else:
            out = (
                [v.name for v in cls.added_file_classes.values()]
                + [v.name for v in cls.added_section_classes.values()]
            )
        for v in cls.added_file_classes.values():
            out += v.all_children(return_classes=return_classes)
        for v in cls.added_section_classes.values():
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

    @property
    def existing_lines(self):
        if os.path.isfile(self.dst):
            with open(self.dst, 'r') as fd:
                return fd.read().splitlines()
        return []

    def get_child(self, k):
        if k in self.added_sections:
            return self.added_sections[k]
        return self.added_files[k]

    def do_generate(self):
        for v in self.added_sections.values():
            v.do_generate()
        for v in self.added_files.values():
            v.do_generate()
        self.lines = self.generate()

    def do_write(self):
        for v in self.added_sections.values():
            v.do_generate()
        for v in self.added_files.values():
            v.do_write()
        return self.write(**self.write_kwargs)

    @classmethod
    def chunk_markers(cls, name):
        if name in cls._chunk_markers:
            return cls._chunk_markers[name]
        return ([f'[BEGIN] {name.upper()}'], [f'[END] {name.upper()}'])

    @classmethod
    def find_marker(cls, lines, marker, start=0):
        assert isinstance(marker, list)
        marker0 = cls.comment + ' ' + marker[0]
        idx = (lines.index(marker0, start)
               if marker0 in lines[start:]
               else len(lines))
        return idx

    @classmethod
    def find_chunk(cls, name, lines, markers=None, unstripped=None):
        if markers is None:
            markers = cls.chunk_markers(name)
        if unstripped is None:
            unstripped = lines
            lines = [x.strip() for x in unstripped]
        idx0 = cls.find_marker(lines, markers[0])
        idx1 = cls.find_marker(lines, markers[1], start=idx0)
        indent = 0
        if idx0 < len(lines):
            indent = len(unstripped[idx0]) - len(lines[idx0])
        return (idx0, idx1, indent)

    @classmethod
    def extract_chunk(cls, name, lines, markers=None,
                      return_outside=False):
        if markers is None:
            markers = cls.chunk_markers(name)
        outside = lines
        outside_stripped = [x.strip() for x in outside]
        inside = []
        idx0, idx1, indent = (0, 0, 0)
        while idx0 < len(outside_stripped):
            idx0, idx1, indent = cls.find_chunk(name, outside_stripped,
                                                markers=markers,
                                                unstripped=outside)
            inside += [
                x[indent:] for x in outside[(idx0 + len(markers[0])):idx1]
            ]
            outside = outside[(idx1 + len(markers[1])):]
            outside_stripped = [x.strip() for x in outside]
        if return_outside:
            return outside
        return inside

    @classmethod
    def check_chunk(cls, name, lines, **kwargs):
        idx0, idx1, indent = cls.find_chunk(name, lines, **kwargs)
        assert idx0 < len(lines) and idx1 < len(lines)

    @classmethod
    def replace_chunk(cls, name, lines, new_chunk, markers=None):
        if markers is None:
            markers = cls.chunk_markers(name)
        idx0, idx1, indent = cls.find_chunk(name, lines, markers=markers)
        assert idx0 < len(lines) and idx1 < len(lines)
        out = (
            lines[:(idx0 + len(markers[0]))]
            + [(indent * ' ') + x for x in new_chunk]
            + lines[idx1:]
        )
        cls.check_chunk(name, out, markers=markers)
        return out

    @classmethod
    def begin_marker(cls, name, extract_from=None, markers=None,
                     **kwargs):
        if markers is None:
            markers = cls.chunk_markers(name)
        out = [cls.comment + ' ' + x for x in markers[0]]
        if extract_from:
            out += cls.extract_chunk(name, extract_from, markers=markers,
                                     **kwargs)
        return out

    @classmethod
    def add_chunk(cls, name, new_chunk=None, extract_from=None,
                  markers=None, allow_empty=False):
        if new_chunk is None:
            new_chunk = []
        if markers is None:
            markers = cls.chunk_markers(name)
        if extract_from:
            new_chunk = (
                cls.extract_chunk(name, extract_from, markers=markers)
                + new_chunk
            )
        if not (new_chunk or allow_empty):
            return []
        out = (
            cls.begin_marker(name, markers=markers)
            + new_chunk
            + cls.end_marker(name, markers=markers)
        )
        cls.check_chunk(name, out, markers=markers)
        if out[-1]:
            out.append('')
        return out

    @classmethod
    def end_marker(cls, name, extract_from=None, markers=None, **kwargs):
        out = []
        if markers is None:
            markers = cls.chunk_markers(name)
        if extract_from:
            out += cls.extract_chunk(name, extract_from, markers=markers,
                                     **kwargs)
        out += [cls.comment + ' ' + x for x in markers[1]]
        return out

    def extract_existing(self, append=None):
        if append is None:
            append = self.append
        if append == 'nothing':
            append = False
        if not (append and os.path.isfile(self.dst)):
            return []
        out = copy.deepcopy(self.existing_lines)
        header = self.header
        footer = self.footer
        existing_header = all(a == b for a, b in zip(header, out))
        existing_footer = all(a == b for a, b in zip(footer[::-1],
                                                     out[::-1]))
        if header and existing_header:
            out = out[len(header):]
        if footer and existing_footer:
            out = out[:-len(footer)]
        if append == 'preserved':
            return self.add_chunk('preserved', extract_from=out)
        elif append == 'discard':
            return self.extract_chunk('discard', out,
                                      return_outside=True)
        return out

    @property
    def header(self):
        return [
            self.comment + ' ' + x for x in self.disclaimer
        ] + self.prefix

    @property
    def footer(self):
        return self.suffix

    def write(self, verbose=False, overwrite=False, dry_run=False,
              append=False):
        self.lines = self.generate()
        if self.expect_empty:
            assert not self.lines
        if (not self.write_empty) and (not self.lines):
            print(f"No lines to be written to {self.dst}")
            return
        lines = copy.deepcopy(self.lines)
        existing = self.extract_existing(append=append)
        if append and existing:
            if append in [True, 'direct', 'True', 'preserved', 'discard']:
                lines = existing + lines
            elif append == 'unique':
                for x in lines:
                    if x not in existing:
                        existing.append(x)
                lines = existing
            else:
                raise NotImplementedError(
                    f"Invalid append value: \'{append}\'")
        contents = '\n'.join(self.header + lines + self.footer) + '\n'
        if verbose:
            print(f"{self.dst}\n----------------\n{contents}")
        if dry_run:
            return
        if (not overwrite) and os.path.isfile(self.dst):
            raise AssertionError(f"{self.name} file {self.dst} "
                                 f"already exists")
        with open(self.dst, 'w') as fd:
            fd.write(contents)

    def max_width(self, members, key='name', func=None):
        if len(members) == 0:
            return 0
        if func is not None:
            def fw(x):
                y = func(x)
                if y is None:
                    return ''
                if isinstance(y, list):
                    y = max(y, key=len)
                return y
            width = len(fw(max(members, key=lambda x: len(fw(x)))))
        elif key == 'name':
            width = len(self.add_member(
                max(members, key=lambda x:
                    len(self.add_member(x, return_name=True))),
                return_name=True))
        else:
            width = len(max(members, key=lambda x:
                            len(x.get(key, ''))).get(key, ''))
        if key == 'idx':
            width += 3
        return width

    def add_member(self, member, return_name=False, add_prefix=None,
                   strip_suffix=None, replace_with_prefix=False,
                   add_enum=False, add_kwargs=None):
        if add_enum:
            self.add_enum(add_enum)
        assert self.current_key is not None
        if isinstance(member, str):
            member = {'name': member}
        name = member['name']
        abbr = member.get('abbr', name)
        if replace_with_prefix and self._current_keys['kprefix']:
            name = ''
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
        if replace_with_prefix and self._current_keys['kprefix']:
            name = name.rstrip('_')
        if add_prefix:
            name = add_prefix + name
        if strip_suffix:
            assert name.endswith(strip_suffix)
            name = name.rsplit(strip_suffix)[0]
            abbr = abbr.rsplit(strip_suffix)[0]
        if return_name:
            return name
        out = dict(member, name=name, abbr=abbr)
        if add_kwargs:
            out.update(**add_kwargs)
        return out

    def generate_member(self, x, **kwargs):
        raise NotImplementedError

    def generate_enum(self, name, members, prefix_with_type=False,
                      **kwargs):
        lines = []
        if prefix_with_type:
            self.add_prefix(name, name + '_')
        self.add_enum(name)
        lines += self.generate_item(name, members, **kwargs)
        return lines

    def generate_item(self, name, members, skip_items=[], **kwargs):
        lines = []
        for x in members:
            if x['abbr'] in self.skip_items.get(name, []) + skip_items:
                continue
            x = self.add_member(x)
            lines += self.generate_member(x, **kwargs)
        return lines

    def generate(self, indent='', skip=[], enum_keys=None, **kwargs):
        lines = []
        if enum_keys is None:
            enum_keys = list(self.src.param.keys())
        for k in enum_keys:
            if k in self.skip + skip:
                continue
            self.add_enum(k)
            lines += [indent + x for x in
                      self.generate_item(k, self.src.param[k], **kwargs)]
        return lines


class CMixin:

    comment = "//"
    perfile_options = dict(
        EnumGeneratorBase.perfile_options,
        include_file={
            'action': 'append',
            'dest': 'include_files',
            'help': "File that should be included.",
        },
        namespace={
            'action': 'append',
            'dest': 'namespaces',
            'help': "C++ namespaces that should contain the classes",
        },
    )

    def __init__(self, *args, **kwargs):
        self.root_include_dir = kwargs.pop('root_include_dir', None)
        if not hasattr(self, 'namespaces'):
            self.namespaces = kwargs.pop('namespaces', None)
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

    @classmethod
    def specialization(cls, lines, spec_param=[], specialize=[],
                       spec_var=None):
        specialization = ''
        if spec_param:
            if specialize:
                if lines is not None:
                    lines += ['template<>']
                specialization = f"<{', '.join(specialize)}>"
            else:
                if spec_var is None:
                    if len(spec_param) == 1:
                        spec_var = ['T']
                    else:
                        spec_var = [
                            f'T{i}' for i in range(len(spec_param))]
                assert len(spec_var) == len(spec_param)
                param = [f"{x} {t}" for x, t in
                         zip(spec_param, spec_var)]
                if lines is not None:
                    lines += [f"template<{', '.join(param)}>"]
        return specialization, spec_var

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
            self.get_child('header').dst, caller=caller,
            rootdir=self.root_include_dir)

    def add_namespaces(self, lines, namespaces=None):
        if namespaces is None:
            namespaces = self.namespaces
        if not (lines and namespaces):
            return lines
        if self.file_extension in ['.hpp', '.h']:
            lines = [
                ('  ' * i) + f'namespace {x} {{'
                for i, x in enumerate(namespaces)
            ] + [
                ('  ' * len(namespaces)) + x for x in lines
            ] + [
                ('  ' * (len(namespaces) - (i + 1))) + '}'
                for i in range(len(namespaces))
            ]
        else:
            lines = [
                f"using namespace {'::'.join(namespaces)};", ""
            ] + lines
        return lines

    def generate(self, indent='', **kwargs):
        lines = []
        lines += self.add_include_files()
        lines += self.add_namespaces(
            super().generate(indent=indent, **kwargs)
        )
        return lines


class ParamFileParser(EnumParserBase):

    name = 'param'
    comment = '#'
    file_extension = '.txt'
    required_keys = ['name', 'val']
    optional_keys = ['doc', 'val_alt', 'qualifiers']
    qualifier_regex = re.compile(
        r'\s*(?P<name>\w+)(?:\s*=\s*(?P<value>(?:[\w\.]+)|(?:'
        r'\{\s*\w+\s*(?:\,\w+\s*)*\})))?'
    )

    def parse(self, src, **kwargs):
        k = os.path.basename(os.path.splitext(src)[0])
        self.add_enum(k)
        out = super(ParamFileParser, self).parse(src, **kwargs)
        self.reset_current_keys()
        return out

    def parse_lines(self, lines, src=None):
        for line in lines:
            # assert '//' not in line
            rem = line.split(self.comment, 1)
            if (not rem[0]) or rem[0].isspace():
                continue
            member = {}
            if len(rem) == 2:
                member['doc'] = rem[1].strip()
                if member['doc'].startswith('['):
                    member['qualifiers'] = member['doc'].split(
                        ']')[0].lstrip('[')
                    qualifiers = {}
                    for match in self.qualifier_regex.finditer(
                            member['qualifiers']):
                        x = match.group('name').upper()
                        val = match.group('value')
                        if val is None:
                            val = True
                        elif val.startswith('{') and val.endswith('}'):
                            val = [v.strip() for v in
                                   val[1:-1].split(',')]
                        qualifiers[x] = val
                    valid_qualifiers = get_registered_value_keys()
                    for k in self.required_keys + self.optional_keys:
                        if k in valid_qualifiers:
                            valid_qualifiers.remove(k)
                    invalid_qualifiers = [
                        x for x in qualifiers.keys()
                        if x not in valid_qualifiers
                    ]
                    if invalid_qualifiers:
                        raise AssertionError(
                            f"Invalid qualifiers: {invalid_qualifiers} "
                            f"in {member['qualifiers']}"
                            f" (available options: {valid_qualifiers})")
                    member.update(qualifiers)
            rem = rem[0].split()
            member['name'] = rem[0].strip()
            member['val'] = rem[1].strip()
            if len(rem) >= 3:
                member['val_alt'] = rem[2].strip()
            if len(rem) > 3:
                raise AssertionError(f"Line in {src} contains more than "
                                     f"3 values: \"{line}\"")
            for k in self.required_keys:
                if k not in member:
                    raise AssertionError(
                        f"Required key \"{k}\" not parsed from line in "
                        f"{src}: \"{line}\"")
            self.add_member(member)


class CEnumParser(EnumParserBase):

    name = 'c'
    file_extension = '.cpp'

    def parse_lines(self, lines, src=None):
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
                        member['idx'] = eval(rem[1].strip().rstrip('LL'))
                    member['name'] = rem[0].strip()
                    self.add_member(member)
            elif lines[i].strip().startswith('enum'):
                k = lines[i].split('enum', 1)[1].split()[0].strip()
                self.add_enum(k)
                while '{' not in lines[i]:
                    i += 1
            i += 1


class CEnumGeneratorBase(CMixin, EnumGeneratorBase):

    file_suffix = '_enum'
    file_extension = '.hpp'
    prefix = [
        '#pragma once',
        '',
    ]


class CEnumGeneratorBaseHeader(CEnumGeneratorBase):

    source_attr = {}
    is_parent = True
    perfile_options = dict(
        CEnumGeneratorBase.perfile_options,
        define_in_header={
            'action': 'store_true', 'default': True,
            'help': "Write the definition in the header.",
        },
        **EnumGeneratorBase.perfile_options,
    )

    @staticmethod
    def create_child_classes(cls):
        EnumGeneratorBase.create_child_classes(cls)
        if cls.name is None or not cls.is_parent:
            return

        class SourceClass(CEnumGeneratorBaseSource):

            name = f"{cls.name}_source"
            file_suffix = cls.file_suffix
            is_parent = False

        for k, v in cls.source_attr.items():
            setattr(SourceClass, k, v)
        cls.added_file_classes = dict(
            cls.added_file_classes,
            source=SourceClass)

    def add_enum(self, *args, **kwargs):
        super(CEnumGeneratorBaseHeader, self).add_enum(*args, **kwargs)
        if not self.is_parent:
            self.parent.add_enum(*args, **kwargs)

    def generate_source_item(self, name, members):
        lines = []
        if not self.define_in_header:
            lines += [self.generate_declaration(name, members)]
            lines[-1] = lines[-1] + ' {'
            lines += [
                '  ' + x for x in self.generate_definition(name, members)]
            lines += ["};", ""]
        return lines

    def generate_source_closing(self, **kwargs):
        return []

    def generate_declaration(self, name, members, for_header=False):
        raise NotImplementedError

    def generate_definition(self, name, members, **kwargs):
        return super(CEnumGeneratorBaseHeader, self).generate_item(
            name, members, **kwargs)

    def generate_item(self, name, members, **kwargs):
        if not self.is_parent:
            assert self.parent
            return self.parent.generate_header_item(name, members, **kwargs)
        lines = []
        if self.define_in_header:
            lines += [self.generate_declaration(name, members) + ' {']
            lines += [
                '  ' + x for x in self.generate_definition(name, members)]
            lines += ["};", ""]
        return lines

    def generate(self, **kwargs):
        lines = super(CEnumGeneratorBaseHeader, self).generate(**kwargs)
        if not self.is_parent:
            assert self.parent
            lines += self.parent.generate_header_closing(**kwargs)
        return lines


class CEnumGeneratorBaseSource(CMixin, EnumGeneratorBase):

    file_extension = '.cpp'
    header_attr = {}
    is_parent = True
    perfile_options = dict(
        CMixin.perfile_options,
        define_in_header={
            'action': 'store_true', 'default': True,
            'help': "Write the definition in the header.",
        },
        docs={
            'type': str,
            'help': "Doc string for enumerator",
        },
        **EnumGeneratorBase.perfile_options,
    )

    @staticmethod
    def create_child_classes(cls):
        EnumGeneratorBase.create_child_classes(cls)
        if cls.name is None or not cls.is_parent:
            return

        class HeaderClass(CEnumGeneratorBaseHeader):

            name = f"{cls.name}_header"
            file_suffix = cls.file_suffix
            is_parent = False

        for k, v in cls.header_attr.items():
            setattr(HeaderClass, k, v)
        cls.added_file_classes = dict(
            cls.added_file_classes,
            header=HeaderClass)

    def generate_docs(self, name):
        return self.docs

    def generate_header_item(self, name, members):
        lines = [self.generate_declaration(name, members,
                                           for_header=True)]
        docs = self.generate_docs(name)
        if self.define_in_header:
            if docs:
                lines = [f'/** {docs} */'] + lines
            lines[-1] = 'inline ' + lines[-1] + ' {'
            lines += [
                '  ' + x for x in self.generate_definition(name, members)]
            lines += ["};", ""]
        else:
            lines[-1] += ';'
            if docs:
                lines[-1] += f'  /**< {docs} */'
        return lines

    def generate_header_closing(self, **kwargs):
        return []

    def generate_declaration(self, name, members, for_header=False):
        raise NotImplementedError

    def generate_definition(self, name, members, **kwargs):
        return super(CEnumGeneratorBaseSource, self).generate_item(
            name, members, **kwargs)

    def generate_item(self, name, members):
        if not self.is_parent:
            assert self.parent
            return self.parent.generate_source_item(name, members)
        lines = []
        if not self.define_in_header:
            lines += [self.generate_declaration(name, members) + ' {']
            lines += [
                '  ' + x for x in self.generate_definition(name, members)]
            lines += ["};", ""]
        return lines

    def generate(self, indent='', **kwargs):
        lines = []
        if not self.define_in_header:
            lines += self.include_header()
        lines += super(CEnumGeneratorBaseSource, self).generate(
            indent=indent, **kwargs)
        if not self.is_parent:
            assert self.parent
            lines += self.parent.generate_source_closing(
                indent=indent, **kwargs)
        return lines


class CEnumGeneratorCollectionBase(CEnumGeneratorBaseSource):

    perfile_options = dict(
        CEnumGeneratorBaseSource.perfile_options,
        collection_name={
            'type': str,
            'help': ("Name that should be used for collection. Defaults "
                     "to class name"),
        },
        singular_collection_name={
            'type': str,
            'help': ("Name that should be used for singular elements in "
                     "the collection. Defaults to collection_name with "
                     "any trailing 's' characters removed"),
        },
        function_suffix={
            'type': str,
            'help': ("Suffix that should be added the the end of the "
                     "function for returning the enum collection. "
                     "Defaults to collection_name if provided and class "
                     "name if not."),
        },
        specialize_in_source={
            'action': 'store_true',
            'help': ("Specialize the template to get the collection in "
                     "the source file instead of the header"),
        },
        dont_specialize={
            'action': 'store_true',
            'help': "Don't special the template function",
        },
        is_editable={
            'action': 'store_true',
            'help': "Allow the static collection to be editable",
        },
        value_key={
            'type': str,
            'help': ("Name of key where values controlling the "
                     "collection are stored."),
        },
        secondary_value_key={
            'type': str,
            'help': ("Name of key where values for the collection "
                     "should be taken from if value_key is not "
                     "present."),
        },
    )
    collection_type = None
    default_is_editable = False
    default_value_key = None
    default_secondary_value_key = None
    _default_function_param = {
        'return_type': 'void',
    }
    _utility_functions = [
        'error_prefix', 'print', 'string',
    ]
    _utility_function_param = {
        'error_prefix': {
        }
    }
    _friend_functions = [
        'operator<<',
    ]
    _additional_functions = []
    _editable_functions = [
        'clear', 'add', 'remove', 'addMultiple', 'removeMultiple',
    ]
    # Methods defined for collections and individual elements
    collection_functions = [
        'operator<<',
    ]
    # Functions that only need to be defined once per collection type
    universal_functions = [
        'error_prefix',
    ]
    _print_prefix = None
    _print_suffix = None

    def __init__(self, *args, **kwargs):
        kwargs.setdefault("collection_name", self.name)
        kwargs.setdefault("singular_collection_name",
                          kwargs["collection_name"].rstrip('s'))
        kwargs.setdefault("function_suffix", kwargs['collection_name'])
        if kwargs.get('define_in_header', False):
            kwargs['specialize_in_source'] = False
        super(CEnumGeneratorCollectionBase, self).__init__(*args, **kwargs)

    @classmethod
    def create_class(cls, class_name, dont_register=False, **kwargs):

        kwargs.setdefault('file_suffix', f"_{class_name}")

        class CEnumGeneratorCollectionCreated(cls):

            name = class_name
            _dont_register = dont_register

        for k, v in kwargs.items():
            setattr(CEnumGeneratorCollectionCreated, k, v)
        for k in list(CEnumGeneratorCollectionCreated.perfile_options.items()):
            if f'default_{k}' in kwargs:
                CEnumGeneratorCollectionCreated.perfile_options[k] = dict(
                    CEnumGeneratorCollectionCreated.perfile_options[k],
                    default=kwargs[f'default_{k}'])
        return CEnumGeneratorCollectionCreated

    @property
    def utility_functions(self):
        return self._utility_functions

    @property
    def friend_functions(self):
        return self._friend_functions

    @property
    def additional_functions(self):
        out = (self._utility_functions
               + self._friend_functions
               + self._additional_functions)
        if self.is_editable:
            out += self._editable_functions
        return out

    @property
    def unsuffixed_functions(self):
        return self.collection_functions + self.universal_functions

    def generate(self, *args, **kwargs):
        out = super(CEnumGeneratorCollectionBase, self).generate(
            *args, **kwargs)
        fglobal = self.parent.get_child('global')
        if fglobal._independent_headers:
            out = fglobal.add_namespaces(out)
        return out

    def generate_value(self, x):
        if self.value_key is None:
            raise NotImplementedError
        out = x.get(self.value_key, '')
        if (not out) and self.secondary_value_key is not None:
            out = x.get(self.secondary_value_key, '')
        if not isinstance(out, list):
            out = str(out)
        return out

    def generate_member(self, *args, **kwargs):
        raise NotImplementedError

    def generate_collection_param(self, name='Type', enum_prefix=''):
        if name is None:
            if self.as_class:
                name = 'Type'
            else:
                raise NotImplementedError('Should come from parent?')
        return [f'{enum_prefix}{name}']

    def generate_collection_type(self, *args, **kwargs):
        if self.collection_type is None:
            raise NotImplementedError
        param = ', '.join(
            self.generate_collection_param(*args, **kwargs))
        return f"{self.collection_type}<{param}>"

    def generate_declaration(self, name, members, for_header=False,
                             in_class=False, var_name='collection'):
        const = "" if self.is_editable else "const "
        if in_class:
            return (
                f"{const}"
                f"{self.generate_collection_type(name, enum_prefix=in_class)} "
                f"{var_name}")
        func_name = f"{name}_{self.function_suffix}"
        if (not for_header) and self.namespaces:
            func_name = f"{'::'.join(self.namespaces)}::{func_name}"
        func_decl = (
            f"{const}{self.generate_collection_type(name)}& {func_name}()")
        return func_decl

    def generate_definition(self, name, members, var_name='collection',
                            no_return=False, in_class=False,
                            enum_prefix='', function_type='get', **kwargs):
        if in_class:
            no_return = True
            enum_prefix = in_class
        if function_type != 'get':
            return self.generate_additional_method(
                function_type, enum_name=f'{enum_prefix}{name}',
                enum_is_class=in_class, result=var_name,
                no_return=no_return)
        enum_member_prefix = enum_prefix
        if in_class:
            enum_member_prefix += f'SCOPED_ENUM_TYPE({name})'
        static = '' if in_class else 'static '
        const = "" if self.is_editable else "const "
        lines = [
            f"{static}{const}"
            f"{self.generate_collection_type(name, enum_prefix=enum_prefix)} "
            f"{var_name} = {{"
        ]
        width = self.max_width(members) + len(enum_member_prefix)
        lines += super(
            CEnumGeneratorCollectionBase, self).generate_definition(
                name, members, width=width,
                enum_prefix=enum_member_prefix, **kwargs)
        lines += ["};"]
        if not no_return:
            lines += [f"return {var_name};"]
        return lines

    def generate_specialization(self, name, members, **kwargs):
        lines = []
        if self.dont_specialize:
            return lines
        if self.parent.as_class:
            specialize = [
                'typename '
                + self.parent.get_child("global").item2specialize(
                    name, return_class=True)
                + '::Type'
            ]
        else:
            specialize = [name]
        lines += self.generate_function(
            name, specialize=specialize, **kwargs)
        return lines

    def generate_header_item(self, name, members):
        lines = []
        if not self.parent.as_class:
            lines += super(
                CEnumGeneratorCollectionBase, self).generate_header_item(
                    name, members)
        if not self.specialize_in_source:
            lines += self.generate_specialization(name, members,
                                                  for_header=True)
        return lines

    def generate_item(self, name, members):
        lines = []
        if not self.parent.as_class:
            lines += super(
                CEnumGeneratorCollectionBase, self).generate_item(
                    name, members)
        if self.specialize_in_source:
            lines += self.generate_specialization(name, members)
        return lines

    def generate_iteration(self, collection_name, iter_name):
        return (
            f"for ({iter_name} = {collection_name}.begin(); "
            f"{iter_name} != {collection_name}.end(); {iter_name}++)"
        )

    def generate_clear(self, collection_name, **kwargs):
        return [f"{collection_name}.clear();"]

    def generate_iterator_dref(self, iter_name):
        raise NotImplementedError

    def generate_add(self, collection_name, var_name, *args):
        raise NotImplementedError

    def generate_remove(self, collection_name, iter_name="it"):
        out = [
            f"if ({iter_name} != {collection_name}.end()) {{",
            f"  {collection_name}.erase({iter_name});",
            "}",
        ]
        return out

    def generate_find(self, collection_name, key_name, iter_name="it"):
        raise NotImplementedError

    def generate_count(self, collection_name, var_name, **kwargs):
        return [f'{var_name} = {collection_name}.size();']

    def generate_utility_methods(self, completed_utils, methods=None,
                                 **kwargs):
        lines = []
        completed_utils.setdefault(self.collection_type, [])
        if methods is None:
            methods = self.utility_functions
        for k in methods:
            if k in completed_utils[self.collection_type]:
                continue
            ikws = copy.deepcopy(kwargs)
            ikws.setdefault('function_param', {})
            if any(k in v for v in completed_utils.values()):
                if k in self.collection_functions:
                    ikws['function_param']['for_class'] = True
                elif k in self.universal_functions:
                    continue
            lines += self.generate_additional_method(
                k, utility=True, **ikws)
            completed_utils[self.collection_type].append(k)
        return lines

    def generate_additional_methods(self, **kwargs):
        lines = []
        for k in self.additional_functions:
            lines += self.generate_additional_method(k, **kwargs)
        return lines

    def generate_additional_method(self, function_type, enum_name='Type',
                                   enum_is_class=None, result=None,
                                   no_return=False, utility=False,
                                   function_param=None):
        lines = []
        if function_type == 'from' and (
                self.value_type == 'double'
                or self.value_key == 'ALIASES'):
            return lines
        if function_param is None:
            function_param = {}
        if enum_is_class is None:
            enum_is_class = self.parent.as_class
        if result is None:
            result = self.collection_name
        ktype = enum_name
        vtype = None
        if hasattr(self, 'value_type'):
            if self.reversed:
                ktype = self.value_type
                vtype = enum_name
            else:
                ktype = enum_name
                vtype = self.value_type
        utility_suffix = function_param.get('utility_suffix', None)
        if utility_suffix is None:
            utility_suffix = '_' + self.collection_type.split('::')[-1]
        function_suffix = function_param.get('function_suffix', None)
        if function_suffix is None:
            if enum_is_class:
                if utility:
                    function_suffix = utility_suffix
                elif function_type in ['get', 'getdefault', 'from']:
                    function_suffix = self.make_title(
                        self.singular_collection_name)
                else:
                    function_suffix = self.make_title(
                        self.collection_name)
            else:
                function_suffix = f"_enum_{self.function_suffix}"
        template_value_type = None
        if ((utility and hasattr(self, 'value_type')
             and not ((function_type in self.collection_functions
                       or function_type in self.universal_functions)
                      and not function_param.get('for_class')))):
            template_value_type = 'T'
            collection_type = self.generate_collection_type(
                enum_name, value_type=template_value_type)
        else:
            collection_type = self.generate_collection_type(enum_name)
        if ((function_type in self.unsuffixed_functions
             or function_type.startswith('operator'))):
            function_name = function_type
        elif function_type in ['getdefault']:
            function_name = (
                f"{function_type.rsplit('default', 1)[0]}"
                f"{function_suffix}"
            )
        else:
            function_name = f"{function_type}{function_suffix}"
        if function_type == 'is':
            return_type = 'bool'
        elif function_type in ['get', 'getdefault']:
            return_type = vtype
        elif function_type in ['from']:
            return_type = ktype
        elif function_type in ['print', 'operator<<']:
            return_type = 'std::ostream&'
        elif function_type in ['string', 'error_prefix']:
            return_type = 'std::string'
        elif function_type in ['list']:
            return_type = f'std::vector<{ktype}>'
        elif function_type in ['count']:
            return_type = 'std::size_t'
        else:
            return_type = 'void'
        args = []
        arg_names = []
        if utility and function_type not in (self.universal_functions
                                             + ['operator<<']):
            args.append(f'const {collection_type}& collection')
            arg_names.append('collection')
        if function_type in ['add', 'remove', 'is', 'get', 'getdefault',
                             'check', 'checkNot']:
            args.append(f'const {ktype}& x')
            arg_names.append('x')
            if ((function_type == 'add' and hasattr(self, 'value_type')
                 and not getattr(self, 'current_flag', None))):
                args.append(f'const {vtype}& y')
                arg_names.append('y')
            elif function_type in ['check', 'checkNot']:
                args.append('const std::string& context = ""')
                arg_names.append('context')
            elif function_type == 'getdefault':
                args.append(f'const {vtype}& defaultV')
                arg_names.append('defaultV')
        elif function_type in ['addMultiple', 'removeMultiple']:
            args.append(f'const {collection_type}& x')
            arg_names.append('x')
        elif function_type == 'print':
            args += ['std::ostream& out', 'bool includePrefixes = false',
                     'const unsigned int tab = 0']
            arg_names += ['out', 'includePrefixes', 'tab']
        elif function_type == 'operator<<':
            args += ['std::ostream& out']
            arg_names += ['out', 'x']
            if function_param.get('for_class', False):
                args += [f'const {collection_type}& x']
            else:
                args += [f'const {enum_name}& x']
        elif function_type == 'string':
            args += ['const unsigned int tab = 0']
            arg_names += ['tab']
        elif function_type == 'from':
            args += [f'const {vtype}& x']
            arg_names += ['x']
        docs = []
        body = []
        if not enum_is_class:
            const = '' if self.is_editable else 'const '
            body += [
                f"{const}{collection_type} {result} = "
                f"get{function_suffix};"
            ]
        if function_type not in self.additional_functions:
            raise NotImplementedError(
                f"Invalid function_type \'{function_type}\' "
                f"(valid values = {self.additional_functions})")
        method_prefix = ''
        if enum_is_class and function_param.get('outside_class', False):
            method_prefix = function_param['outside_class'] + '::'
        if (not utility) and (function_type in self.utility_functions
                              or function_type in self.friend_functions):
            if ((function_type in self.universal_functions
                 or function_type.startswith('operator')
                 or function_type in self.friend_functions)):
                return []
            elif function_type == 'print':
                docs += [
                    f'Print the contents of {self.collection_name}',
                    '\\param[in,out] out Stream to print to',
                    '\\param[in] includePrefixes If true, the module & ',
                    '  parameter type prefixes will be added to the member ',
                    '  names.',
                    '\\param[in] tab Indentation to add to each line',
                    '\\return Updated stream',
                ]
            elif function_type == 'string':
                docs += [
                    f'Serialize the contents of {self.collection_name}',
                    '\\param[in] tab Indentation to add to each line',
                    '\\return Serialized collection',
                ]
            body += [
                f"return {function_type}{utility_suffix}("
                f"{result}, {', '.join(arg_names)});"
            ]
        elif function_type == 'is':
            docs += [
                f'Check if a key is in {self.collection_name}',
                '\\param[in] x Key to check',
                '\\return true if x is present, false otherwise',
            ]
            body += [f"typename {collection_type}::const_iterator it;"]
            body += self.generate_find(result, "x", "it")
            if hasattr(self, 'generate_is'):
                body += self.generate_is(result, 'it')
            else:
                body += [f"return (it != {result}.end());"]
        elif function_type in ['check', 'checkNot']:
            codeNot = ''
            strNot = ''
            if function_type == 'check':
                codeNot = '!'
                strNot = 'not '
            docs += [
                f'Throw an error if a key is {strNot}in '
                f'{self.collection_name}',
                '\\param[in] x Key to check',
                '\\param[in] context String describing context that ',
                '  should be used in the error message',
            ]
            body += [
                f'if ({codeNot}is{function_suffix}(x)) {{',
                f'  throw std::runtime_error('
                f'error_prefix() + context + ": \'" + '
                f'names.find(x)->second'
                f' + "\' is {strNot}in {self.collection_name}");',
                '}',
            ]
        elif function_type == 'clear':
            assert self.is_editable
            docs_sub = 'entries'
            if getattr(self, 'current_flag', None):
                docs_sub = f'{self.current_flag} flags'
            docs += [
                f'Remove all {docs_sub} from {self.collection_name}'
            ]
            body += self.generate_clear(result,
                                        collection_type=collection_type)
        elif function_type in ['get', 'getdefault']:
            assert hasattr(self, 'value_type')
            body += [f"typename {collection_type}::const_iterator it;"]
            body += self.generate_find(result, "x", "it")
            dref = self.generate_iterator_dref("it")[1]
            docs += [
                f'Get the {function_suffix.lower()} value corresponding '
                f'to an enum key',
                '\\param[in] x Key to get value for',
            ]
            body += [
                f'if (it == {result}.end()) {{',
            ]
            if function_type == 'getdefault':
                docs += [
                    '\\param[in] defaultV Value to return if x is not '
                    'present'
                ]
                body += ['  return defaultV;',
                         '}']
            else:
                body += [
                    f'  throw std::runtime_error("Could not locate '
                    f'{function_suffix} for \'" + '
                    f'names.find(x)->second + "\'");',
                    '}'
                ]
            docs += [
                '\\return Value'
            ]
            body += [f"return {dref};"]
        elif function_type == 'from':
            assert hasattr(self, 'value_type')
            body += [f"typename {collection_type}::const_iterator it;"]
            body += self.generate_find(result, "x", "it",
                                       from_value=True)
            dref = self.generate_iterator_dref("it")[0]
            docs += [
                f'Get the enum key corresponding '
                f'to a {function_suffix.lower()} values',
                '\\param[in] x Value to get key for',
                '\\return Key',
            ]
            conv = ('x' if self.value_type == 'std::string'
                    else 'std::to_string(x)')
            body += [
                f'if (it == {result}.end()) {{',
                f'  throw std::runtime_error("Could not locate '
                f'{function_suffix} for \'" + {conv} + "\'");',
                '}',
                f"return {dref};",
            ]
        elif function_type == 'error_prefix':
            docs += [
                'Get a prefix for errors describing the class',
                '\\return Prefix',
            ]
            body += [
                'std::string out;',
                'out += get_enum_names<PARAM_TYPE>().'
                'find(param_type)->second;',
                'out += "[";',
                'out += get_enum_names<MODULE>().find(module)->second;',
                'out += "]: ";',
                'return out;',
            ]
        elif function_type == 'operator<<':
            docs += [
                'Serialize an enum to an output stream',
                '\\param[in,out] out Output stream',
            ]
            if function_param.get('for_class', False):
                docs += [
                    '\\param[in] x Collection to serialize',
                ]
                body += [
                    f"{method_prefix}print{function_suffix}(x, out);",
                    "return out;",
                ]
            else:
                docs += [
                    '\\param[in] x Key to serialize',
                ]
                body += [
                    f'out << {method_prefix}getName(x);',
                    'return out;'
                ]
            docs += [
                '\\return Updated stream',
            ]
        elif function_type == 'print':
            docs += [
                'Print the contents of a collection',
                '\\param[in] collection Object to print',
                '\\param[in,out] out Stream to print to',
                '\\param[in] includePrefixes If true, the module & ',
                '  parameter type prefixes will be added to the member ',
                '  names.',
                '\\param[in] tab Indentation to add to each line',
                '\\return Updated stream',
            ]
            body += ["const std::string space(tab * 4, ' ');"]
            itdref = self.generate_print(
                *self.generate_iterator_dref('it'),
                space='space', out='out')
            if self._print_prefix:
                body += [f"out << {self._print_prefix};"]
            body += [
                f"typename {collection_type}::const_iterator it;",
                f"{self.generate_iteration('collection', 'it')} {{",
                f"  {itdref}",
                "}",
            ]
            if self._print_suffix:
                body += [f"out << {self._print_suffix};"]
            body += [
                "return out;",
            ]
        elif function_type == 'string':
            docs += [
                'Serialize a collection to a string',
                '\\param[in] collection Object to serialize',
                '\\param[in] tab Indentation to add to each line',
                '\\return Serialized collection',
            ]
            body += [
                "std::ostringstream oss;",
                f"{method_prefix}"
                f"print{function_suffix}(collection, oss, tab);",
                "return oss.str();",
            ]
        elif function_type == 'remove':
            assert self.is_editable
            docs += [
                f'Remove an element from {self.collection_name}',
                '\\param[in] x Key to remove',
            ]
            body += [f"typename {collection_type}::iterator it;"]
            body += self.generate_find(result, "x", "it")
            body += self.generate_remove(result, "it")
        elif function_type == 'add':
            assert self.is_editable
            docs += [
                f'Add an element to {self.collection_name} if it is not '
                f'already present',
                '\\param[in] x Key to add',
            ]
            if ((hasattr(self, 'value_type')
                 and not getattr(self, 'current_flag', None))):
                docs += ['\\param y Value to add for x']
                body += self.generate_add(result, *arg_names,
                                          collection_type=collection_type)
            else:
                body += [f"if (!is{function_suffix}(x)) {{"]
                body += [f"  {x}" for x in
                         self.generate_add(result, *arg_names,
                                           collection_type=collection_type)]
                body += ["}"]
        elif function_type == 'count':
            docs_suffix = ''
            if getattr(self, 'current_flag', None):
                docs_suffix = f' with {self.current_flag} set'
            docs += [
                f'Get the number of elements in '
                f'{self.collection_name}{docs_suffix}.',
                f'\\return Number of elements{docs_suffix}.',
            ]
            body += [f'{return_type} out;']
            body += self.generate_count(result, 'out',
                                        collection_type=collection_type)
            body += ['return out;']
        elif function_type == 'list':
            docs += [
                f'Get the set of elements in {self.collection_name} '
                f'with {self.current_flag} set.',
                '\\return Set of elements.',
            ]
            body += [f'{return_type} out;']
            body += self.generate_list(result, 'out',
                                       collection_type=collection_type)
            body += ['return out;']
        elif function_type.endswith('Multiple'):
            assert self.is_editable
            short = function_type.split('Multiple')[0]
            ftype = short + function_suffix
            itdref = ", ".join(self.generate_iterator_dref('it'))
            docs += [
                f'{short.title()} multiple elements to '
                f'{self.collection_name} if they are not already present',
                '\\param[in] x Elements to add',
            ]
            body += [
                f"typename {collection_type}::const_iterator it;",
                f"{self.generate_iteration('x', 'it')} {{",
                f"  {ftype}({itdref});",
                "}"
            ]
        else:
            raise NotImplementedError(
                f"Unimplemented function_type \'{function_type}\' "
                f"(valid values = {self.additional_functions})")
        if no_return:
            return body
        if docs:
            lines += ['/**']
            lines += ["  " + x for x in docs]
            lines += ['*/']
        static = 'static '
        if function_param.get('outside_class', False):
            if template_value_type:
                static = ''
            else:
                static = 'inline '
        elif function_type in self.friend_functions:
            if template_value_type:
                static = 'friend '
            else:
                static = 'friend inline '
        if template_value_type:
            lines += [f'template<typename {template_value_type}>']
        lines += [f"{static}{return_type} {function_name}"
                  f"({', '.join(args)}) {{"]
        lines += ["  " + x for x in body]
        lines += ["}"]
        if ((function_type in self.collection_functions
             and not function_param.get('for_class', False))):
            lines += self.generate_additional_method(
                function_type, enum_name=enum_name,
                enum_is_class=enum_is_class, result=result,
                no_return=no_return, utility=utility,
                function_param=dict(function_param, for_class=True))
        return lines

    def generate_function(self, name, for_header=False, result=None,
                          specialize_empty=False, enum_is_class=None,
                          specialize_direct=None, direct=False,
                          skip_items=[], function_type='get', **kwargs):
        lines = []
        kwargs.setdefault('spec_param', ['typename'])
        assert len(kwargs['spec_param']) == 1
        specialization, spec_var = self.specialization(lines, **kwargs)
        inline = 'inline ' if (specialization and for_header) else ''
        const = "" if self.is_editable else "const "
        key_type = (specialization.strip('<>') if specialization
                    else spec_var[0])
        class_type = None
        if enum_is_class is None:
            enum_is_class = self.parent.as_class
        if enum_is_class and specialization:
            class_type = self.parent.get_child('global').item2specialize(
                name, return_class=True)
            key_type = f'typename {class_type}::Type'
        ktype = key_type
        vtype = None
        if hasattr(self, 'value_type'):
            if self.reversed:
                vtype = key_type
                ktype = self.value_type
            else:
                vtype = self.value_type
        collection_type = self.generate_collection_type(key_type)
        args = []
        arg_names = []
        if function_type in ['add', 'remove', 'is']:
            args.append(f'const {ktype}& x')
            arg_names.append('x')
            if function_type == 'add' and hasattr(self, 'value_type'):
                args.append(f'const {vtype}& y')
                arg_names.append('y')
        elif function_type in ['addMultiple', 'removeMultiple']:
            args.append(f'const {collection_type}& x')
        if enum_is_class and specialization:
            if result is None:
                if function_type == 'get':
                    result = f'{class_type}::{self.collection_name}'
                else:
                    result = (
                        f'{class_type}::{function_type}'
                        f'{self.make_title(self.collection_name)}('
                        f'{", ".join(arg_names)})'
                    )
        if function_type == 'is':
            return_type = 'bool'
            return_value = ' = false'
            return_ref = ''
        else:
            return_type = f"{const}{collection_type}"
            return_value = ''
            return_ref = '&'
        lines += [
            f'{inline}{return_type}{return_ref} {function_type}_enum_'
            f'{self.function_suffix}{specialization}('
            f'{", ".join(args)}) {{',
        ]
        if result is None:
            if specialization:
                if specialize_empty:
                    result = 'result'
                    lines += [
                        f'  static{return_type}result{return_value};'
                    ]
                else:
                    if not direct:
                        raise AssertionError(
                            "direct must be provide names of members "
                            "for non-empty specialization")
                    self.add_enum(name)
                    result = 'collection'
                    lines += [
                        '  ' + x for x in self.generate_definition(
                            name, direct, no_return=True,
                            var_name=result, skip_items=skip_items,
                            function_type=function_type,
                        )]
                    if function_type == 'is':
                        result = None
            else:
                result = 'result'
                lines += [
                    f'  static {return_type} result{return_value};',
                    f'  throw std::runtime_error(\"No enum '
                    f'{self.collection_name} collection could be found\");'
                ]
        if result is not None:
            lines += [f'  return {result};']
        lines += ['}']
        kwargs.update(for_header=for_header, skip_items=skip_items)
        kwargs.pop('specialize', None)
        if (not specialization) and specialize_empty:
            lines += self.generate_function(
                name,
                specialize=[specialize_empty],
                specialize_empty=specialize_empty,
                **kwargs
            )
        if (not specialization) and specialize_direct:
            for k, v in specialize_direct.items():
                lines += self.generate_function(
                    k,
                    specialize=[k],
                    direct=v,
                    enum_is_class=False,
                    **kwargs
                )
        return lines

    @classmethod
    def make_title(cls, x):
        return ''.join([xx.title() for xx in x.split('_')])

    def generate_utility_types(self):
        return []


class CEnumGeneratorMapBase(CEnumGeneratorCollectionBase):

    perfile_options = dict(
        CEnumGeneratorCollectionBase.perfile_options,
        value_type={
            'type': str,
            'help': "C++ type of values in the map",
        },
        reversed={
            'action': 'store_true',
            'help': "Use value type as key and enum as value",
        },
        explicit={
            'action': 'store_true',
            'help': "Only include keys with explicit values in map",
        },
    )
    default_value_type = None
    default_reversed = False
    default_explicit = False
    collection_type = 'std::map'
    _additional_functions = (
        CEnumGeneratorCollectionBase._additional_functions + [
            'get', 'getdefault', 'from'
        ]
    )
    _print_prefix = None  # "space << \"{\" << std::endl;"
    _print_suffix = None  # "space << \"}\""

    def generate_docs(self, name):
        name_doc = self.collection_name.title()
        return f'{name_doc} for values'

    def generate_collection_param(self, *args, **kwargs):
        value_type = kwargs.pop('value_type', self.value_type)
        out = super(CEnumGeneratorMapBase, self).generate_collection_param(
            *args, **kwargs)
        if self.reversed:
            out = [value_type] + out
        else:
            out.append(value_type)
        return out

    def generate_value(self, x):
        out = super(CEnumGeneratorMapBase, self).generate_value(x)
        if self.explicit and (not out):
            return None
        if self.value_type == 'std::string':
            if isinstance(out, list):
                out = [f"\"{xx}\"" for xx in out]
            else:
                out = f"\"{out}\""
        return out

    def generate_member(self, x, width=None, width_value=None,
                        enum_prefix='', member_suffix='', value=None):
        assert width is not None and width_value is not None
        if value is None:
            value = self.generate_value(x)
        if not value:
            return []
        if isinstance(value, list):
            out = []
            for v in value:
                out += self.generate_member(x, width=width,
                                            width_value=width_value,
                                            enum_prefix=enum_prefix,
                                            member_suffix=member_suffix,
                                            value=v)
            return out
        if self.reversed:
            pad = (width - len(enum_prefix + x['name'])) * ' '
            return [
                f"  {{{value:{width_value}}, "
                f"{(enum_prefix + x['name'])}{pad}}},{member_suffix}"]
        else:
            pad = (width_value - len(value)) * ' '
            return [
                f"  {{{(enum_prefix + x['name']):{width}}, "
                f"{value}{pad}}},{member_suffix}"]

    def generate_definition(self, name, members, **kwargs):
        width_value = self.max_width(members, func=self.generate_value)
        lines = super(CEnumGeneratorMapBase, self).generate_definition(
            name, members, width_value=width_value, **kwargs)
        return lines

    def generate_iterator_dref(self, iter_name):
        return [f"{iter_name}->first", f"{iter_name}->second"]

    def generate_add(self, collection_name, key_name, val_name, **kwargs):
        if self.reversed:
            return [f"{collection_name}.emplace({val_name}, {key_name});"]
        else:
            return [f"{collection_name}.emplace({key_name}, {val_name});"]

    def generate_find(self, collection_name, key_name, iter_name="it",
                      from_value=False):
        if from_value:
            return [
                f'for ({iter_name} = {collection_name}.begin(); '
                f'{iter_name} != {collection_name}.end(); '
                f'{iter_name}++) {{',
                f'  if ({iter_name}->second == {key_name}) break;',
                '}',
            ]
        return [f"{iter_name} = {collection_name}.find({key_name});"]

    def generate_print(self, *args, space=None, out='std::cout'):
        if space:
            space = f" << {space}"
        else:
            space = ''
        assert len(args) == 2
        key = f"names.find({args[0]})->second"
        val = f"{args[1]}"
        # return (f'{out}{space} << "  " << {key} << " = " << {val} '
        #         f'<< std::endl;')
        return (f'{out}{space} << "  " << {key} << "\t" << {val} '
                f'<< std::endl;')


class CEnumGeneratorFlagMapBase(CEnumGeneratorMapBase):

    perfile_options = dict(
        {k: v for k, v in CEnumGeneratorMapBase.perfile_options.items()
         if k not in ['value_key', 'reversed']},
        value_flags={
            'action': 'append',
            'help': "Value keys that should be set to bit flags.",
        },
        flag_prefix={
            'type': str,
            'help': "Prefix that should be used for flags & flag type",
        },
    )
    _additional_functions = (
        CEnumGeneratorCollectionBase._additional_functions + [
            'get', 'is', 'check', 'checkNot', 'count', 'list',
        ]
    )
    default_value_type = 'int'
    default_value_flags = []
    default_flag_prefix = None
    reversed = False

    def __init__(self, *args, **kwargs):
        self._flag_stack = []
        self._dummy_vectors = {}
        super(CEnumGeneratorFlagMapBase, self).__init__(*args, **kwargs)

    def get_dummy_vector_collection(self, k):
        if k in self._dummy_vectors:
            return self._dummy_vectors[k]
        cls = CEnumGeneratorVectorBase.create_class(k, dont_register=True)
        self._dummy_vectors[k] = cls(
            f'invalid_{k}_src',
            dst=f'invalid_{k}_dst',
            collection_name=k,
            value_key=self.value_flag_map[k],
            is_editable=self.is_editable,
            dont_generate=True,
        )
        return self._dummy_vectors[k]

    @property
    def value_flag_map(self):
        if not getattr(self, '_value_flag_map', None):
            if ((self.value_flags is None
                 or not isinstance(self.value_flags, list))):
                raise NotImplementedError
            out = {}
            for x in self.value_flags:
                if ':' in x:
                    k, v = x.split(':')
                else:
                    k = x
                    v = x.upper()
                out[k] = v
            self._value_flag_map = out
        return self._value_flag_map

    def prefix_flags(self, x):
        if isinstance(x, list):
            return [self.prefix_flags(v) for v in x]
        if self.flag_prefix is None:
            raise NotImplementedError
        return self.flag_prefix + '_' + x

    @property
    def value_key(self):
        return list(self.value_flag_map.values())

    def push_flag(self, k):
        self._flag_stack.append(k)

    def pop_flag(self):
        self._flag_stack.pop()

    @property
    def current_flag_collection(self):
        if not self._flag_stack:
            return None
        return self._flag_stack[-1]

    @property
    def current_flag(self):
        if not self._flag_stack:
            return None
        return self.prefix_flags(
            self.value_flag_map[self.current_flag_collection])

    def make_title(self, x):
        xflag = self.current_flag_collection
        if xflag:
            x = xflag
        return CEnumGeneratorCollectionBase.make_title(x)

    def generate_utility_methods(self, *args, **kwargs):
        k = next(iter(self.value_flag_map.keys()))
        dummy_vect = self.get_dummy_vector_collection(k)
        out = []
        out += dummy_vect.generate_utility_methods(*args, **kwargs)
        out += super(
            CEnumGeneratorFlagMapBase, self).generate_utility_methods(
                *args, **kwargs)
        return out

    def generate_additional_method(self, function_type, **kwargs):
        methods_not_general = [
            'is', 'check', 'checkNot', 'list',
        ]
        methods_general = [
            'string', 'clear', 'add', 'remove', 'count',
        ]
        out = []
        if ((function_type in (methods_not_general + methods_general)
             and not self.current_flag_collection)):
            result = kwargs.get('result', self.collection_name)
            assert self.value_flag_map
            assert result
            for k in self.value_flag_map.keys():
                ktitle = CEnumGeneratorCollectionBase.make_title(k)
                self.push_flag(k)
                ikw = copy.deepcopy(kwargs)
                ikw.setdefault('function_param', {})
                if function_type in ['string']:
                    dummy_vect = self.get_dummy_vector_collection(k)
                    ikw['result'] = f'list{ktitle}()'
                    out += dummy_vect.generate_additional_method(
                        function_type, **ikw)
                else:
                    out += self.generate_additional_method(
                        function_type, **ikw)
                self.pop_flag()
            if function_type in methods_not_general:
                return out
        out += super(
            CEnumGeneratorFlagMapBase, self).generate_additional_method(
                function_type, **kwargs)
        return out

    def generate_is(self, result, value):
        assert self.current_flag is not None
        out = [
            f'if ({value} == {result}.end()) return false;',
            f'return ({value}->second & {self.current_flag});',
        ]
        return out

    def generate_remove(self, result, value, *args, **kwargs):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_remove(
                result, value, *args, **kwargs)
        out = [
            f"if ({value} != {result}.end()) {{",
            f"  {value}->second &= ~{self.current_flag};",
            "}",
        ]
        return out

    def generate_add(self, result, var_name, *args, **kwargs):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_add(
                result, var_name, *args, **kwargs)
        iter_name = kwargs.get('iter_name', 'it')
        collection_type = kwargs.get('collection_type', None)
        if collection_type is None:
            collection_type = self.generate_collection_type()
        out = [
            f"typename {collection_type}::iterator {iter_name};",
        ]
        out += self.generate_find(result, var_name, iter_name)
        out += [f'if ({iter_name} == {result}.end()) {{']
        out += [
            f'  {x}' for x in
            super(CEnumGeneratorFlagMapBase, self).generate_add(
                result, var_name, self.prefix_flags('NONE'))
        ]
        out += [
            f'  {iter_name} = {result}.find({var_name});',
            '}',
            f'{iter_name}->second |= {self.current_flag};',
        ]
        return out

    def generate_clear(self, collection_name, iter_name="it",
                       collection_type=None):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_clear(
                collection_name, iter_name=iter_name,
                collection_type=collection_type)
        dref = self.generate_iterator_dref(iter_name)[1]
        if collection_type is None:
            collection_type = self.generate_collection_type()
        out = [
            f"typename {collection_type}::iterator {iter_name};",
            self.generate_iteration(collection_name, iter_name) + "{",
            f"  {dref} &= ~{self.current_flag};",
            "}",
        ]
        return out

    def generate_count(self, collection_name, var_name, iter_name="it",
                       collection_type=None):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_count(
                collection_name, var_name, iter_name=iter_name,
                collection_type=collection_type)
        dref = self.generate_iterator_dref(iter_name)[1]
        if collection_type is None:
            collection_type = self.generate_collection_type()
        out = [
            f"{var_name} = 0;",
            f"typename {collection_type}::const_iterator {iter_name};",
            self.generate_iteration(collection_name, iter_name) + "{",
            f"  if ({dref} & {self.current_flag}) {var_name}++;",
            "}",
        ]
        return out

    def generate_list(self, collection_name, var_name, iter_name="it",
                      collection_type=None):
        assert self.current_flag is not None
        dref = self.generate_iterator_dref(iter_name)
        if collection_type is None:
            collection_type = self.generate_collection_type()
        out = [
            f"typename {collection_type}::const_iterator {iter_name};",
            self.generate_iteration(collection_name, iter_name) + "{",
            f"  if ({dref[1]} & {self.current_flag}) {{",
            f"    {var_name}.push_back({dref[0]});",
            "  }",
            "}",
        ]
        return out

    def generate_utility_types(self):
        assert self.flag_prefix
        out = [f'// {self.flag_prefix} enum']
        keys = ['NONE'] + self.value_key + ['MAX']
        members = [
            self.parent.get_child('global').add_member(
                k, add_prefix=f'{self.flag_prefix}_',
                add_enum=self.flag_prefix,
                add_kwargs={'idx': _bitflags[i],
                            'explicit_idx': True})
            for i, k in enumerate(keys)
        ]
        out += self.parent.generate_enum(
            self.flag_prefix, members, as_class=False)
        return out

    def generate_value(self, x):
        if self.value_key is None or not isinstance(self.value_key, list):
            raise NotImplementedError
        out = [k for k in self.value_key if x.get(k, '')]
        if self.explicit and (not out):
            return None
        if not out:
            out.append('NONE')
        out = ' | '.join(self.prefix_flags(out))
        return '(' + out + ')'


class CEnumGeneratorVectorBase(CEnumGeneratorCollectionBase):

    collection_type = 'std::vector'
    _additional_functions = (
        CEnumGeneratorCollectionBase._additional_functions + [
            'is', 'check', 'checkNot', 'count',
        ]
    )
    _print_prefix = "space << \"[\""
    _print_suffix = "\"]\""  # " << std::endl;"

    def generate_docs(self, name):
        name_doc = self.collection_name.rstrip('s')
        name_doc = name_doc.replace('s_C3', '_C3')
        return f'Values that are {name_doc}'

    def generate_member(self, x, width=None, enum_prefix=''):
        if not self.generate_value(x):
            return []
        assert width
        return [f"  {(enum_prefix + x['name']):{width}},"]

    def generate_iterator_dref(self, iter_name):
        return [f"(*({iter_name}))"]

    def generate_add(self, collection_name, var_name, *args, **kwargs):
        return [f'{collection_name}.push_back({var_name});']

    def generate_find(self, collection_name, key_name, iter_name="it"):
        dref = self.generate_iterator_dref(iter_name)[0]
        return [
            self.generate_iteration(collection_name, iter_name) + "{",
            f"  if ({dref} == {key_name}) break;",
            "}",
        ]

    def generate_print(self, *args, space=None, out='std::cout'):
        assert len(args) == 1
        key = f"names.find({args[0]})->second"
        return f'{out} << {key} << ",";'


class CEnumGeneratorGlobalHeader(CEnumGeneratorBase):

    name = 'global'
    file_suffix = ''
    prefix = CEnumGeneratorBase.prefix + [
        '#include <string>',
        '#include <map>',
        '#include <vector>',
        '#include <iostream>',
        '#include <fstream>',
        '#include <sstream>',
        # '#define EPHOTO_USE_SCOPED_ENUM 1',
        '#ifdef _MSC_VER',
        '// There is a bug in the MSVC compiler where it does not allow',
        '//   declaration of a specialized class member enum',
        '//   https://developercommunity.visualstudio.com/t/'
        'Explicit-specialization-of-member-enumer/10609934',
        '#define EPHOTO_USE_SCOPED_ENUM 1',
        '#endif // _MSC_VER',
        '#ifdef EPHOTO_USE_SCOPED_ENUM',
        '#define SCOPED_ENUM_TYPE(name) name::',
        '#else // EPHOTO_USE_SCOPED_ENUM',
        '#define SCOPED_ENUM_TYPE(name)',
        '#endif // EPHOTO_USE_SCOPED_ENUM',
    ]
    explicit_dst = True
    perfile_options = dict(
        {k: v for k, v in CEnumGeneratorBase.perfile_options.items()
         if k not in ['define_in_header']},
        enum_name={
            'type': str,
            'help': "Name that should be used for the global enum",
        },
        enum_macro_skip={
            'action': 'append',
            'help': ("Enum members that should be skipped in the "
                     "members macros"),
        },
        accum_enum_name={
            'type': str,
            'help': ("Name that should be used for enum accumulated "
                     "between calls"),
            'default': "PARAM_TYPE",
        },
        accum_enum_macro_skip={
            'action': 'append',
            'help': ("Accumulated enum members that should be skipped "
                     "in the members macros"),
        },
        empty_enum_name={
            'type': str,
            'help': "Name that should be used for empty enum",
            'default': "EMPTY_ENUM",
        },
        strip_suffix={
            'type': str,
            'help': ("Suffix that should be stripped from global enum "
                     "members"),
        },
    )
    _top_in_helper = True
    _independent_headers = True

    def __init__(self, *args, **kwargs):
        kwargs.setdefault('enum_name', self.name.upper())
        kwargs.setdefault('prefixes', {})
        super(CEnumGeneratorGlobalHeader, self).__init__(*args, **kwargs)
        assert self.parent

    def generate_item(self, name, members, width=None):
        assert width
        key = self.add_member(name, return_name=True,
                              replace_with_prefix=True,
                              add_prefix=f'{self.enum_name}_',
                              strip_suffix=self.strip_suffix)
        return [f"{key:{width}},"]

    def generate_type_struct(self, name, result=None, for_header=False,
                             **kwargs):
        lines = []
        func_name = f'{name}2Enum'
        if self.parent.as_class:
            lines += [
                f'#define {func_name} {self.parent.as_class}'
            ]
            return lines
        assert not self.parent.as_class
        kwargs.setdefault('spec_param', self.spec_param)
        specialization, spec_var = self.specialization(lines, **kwargs)
        lines += [
            f'struct {func_name}{specialization} {{',
            'public:',
        ]
        if result is None and (not specialization):
            result = self.empty_enum_name
        assert result is not None
        lines += [
            f'  typedef enum {result} Type;',
            '};',
        ]
        return lines

    def generate_param_function(self, dst, result=None, for_header=False,
                                **kwargs):
        lines = []
        func_name = f'get_enum_{dst.lower()}'
        kwargs.setdefault('spec_param', ['typename'])
        specialization, spec_var = self.specialization(lines, **kwargs)
        inline = 'inline ' if (specialization and for_header) else ''
        lines += [
            f'{inline}{dst} {func_name}{specialization}() {{',
        ]
        if (not specialization) and (not self.parent.as_class):
            lines += [
                f'  throw std::runtime_error(\"No {dst.lower()} '
                f'could be found\");',
            ]
        if result is None and (not specialization):
            if self.parent.as_class:
                result = f'{spec_var[0]}::{dst}'
            else:
                result = f'{dst}_NONE'
        assert result is not None
        lines += [
            f'  return {result};',
            '}',
        ]
        if (not specialization) and (not self.parent.as_class):
            kwargs.update(
                result=result,
                for_header=for_header,
                specialize=[self.empty_enum_name],
            )
            lines += self.generate_param_function(dst, **kwargs)
        return lines

    @property
    def spec_param(self):
        out = [self.enum_name]
        if self.accum_enum_name:
            out.append(self.accum_enum_name)
        return out

    @property
    def spec_var(self):
        # TODO: Add command line option for these names
        out = ['M']
        if self.accum_enum_name:
            out.append('PT')
        return out

    def item2specialize(self, name, return_class=False):
        out = [f"{self.enum_name}_{name.rsplit(self.strip_suffix)[0]}"]
        if self.accum_enum_name:
            out.append(f"{self.accum_enum_name}{self.strip_suffix}")
        if return_class:
            assert self.parent.as_class
            out = f"{self.parent.as_class}<{', '.join(out)}>"
        return out

    def extract_global_enum(self, existing=None):
        self._enum_members = []
        self._accum_enum_members = []
        if existing is None:
            if self.append:
                if self._top_in_helper:
                    existing = self.parent.get_child('helper').existing_lines
                else:
                    existing = self.existing_lines
            else:
                existing = []
        existing_enum = []
        existing_accum_enum = []
        for x in self.extract_chunk('global_enum', existing):
            if x.strip().startswith(f'{self.enum_name}_'):
                new_val = x.split(
                    f'{self.enum_name}_')[-1].split(',')[0].strip()
                if new_val not in ['NONE', 'MAX']:
                    existing_enum.append(new_val)
            elif (self.accum_enum_name
                  and x.strip().startswith(f'{self.accum_enum_name}_')):
                new_val = x.split(
                    f'{self.accum_enum_name}_')[-1].split(',')[0].strip()
                if new_val not in ['NONE', 'MAX']:
                    existing_accum_enum.append(new_val)
        # Modules
        for k in self.src.param.keys():
            key = self.add_member(k, replace_with_prefix=True,
                                  return_name=True,
                                  strip_suffix=self.strip_suffix,
                                  add_enum=self.enum_name)
            if key not in existing_enum:
                existing_enum.append(key)
        self._enum_members += [
            self.add_member(x, add_prefix=f'{self.enum_name}_',
                            add_enum=self.enum_name)
            for x in ['NONE'] + existing_enum + ['MAX']
        ]
        # Param types
        if self.accum_enum_name:
            new_accum_enum = self.strip_suffix.strip('_')
            if new_accum_enum not in existing_accum_enum:
                existing_accum_enum.append(new_accum_enum)
            self._accum_enum_members += [
                self.add_member(x, add_prefix=f'{self.accum_enum_name}_',
                                add_enum=self.accum_enum_name)
                for x in ['NONE'] + existing_accum_enum + ['MAX']
            ]

    @property
    def enum_members(self):
        if getattr(self, '_enum_members', None) is None:
            self.extract_global_enum()
        return self._enum_members

    @property
    def accum_enum_members(self):
        if getattr(self, '_accum_enum_members', None) is None:
            self.extract_global_enum()
        return self._accum_enum_members

    def generate_global_enum(self, lines=None):
        if lines is None:
            lines = []
        # Modules
        lines += self.parent.generate_enum(
            self.enum_name, self.enum_members,
            as_class=False, macro_skip=self.enum_macro_skip,
        )
        # Param types
        if self.accum_enum_name:
            lines += self.parent.generate_enum(
                self.accum_enum_name, self.accum_enum_members,
                as_class=False, macro_skip=self.accum_enum_macro_skip,
            )
        return lines

    def generate(self, indent='', **kwargs):
        lines = []
        includes = {'global': []}
        for k in self.parent.added_collections:
            includes[k] = []
        existing = [] if not self.append else self.existing_lines
        for k, v in includes.items():
            includes[k] += self.extract_chunk(f'headers_{k}', existing)
        include = self.parent.include_self(
            self.dst, rootdir=self.root_include_dir)
        for x in include:
            if x not in includes['global']:
                includes['global'].append(x)
        if not self._top_in_helper:
            lines += self.add_chunk(
                'global_enum', self.generate_global_enum())
        # Utility types for additional methods
        for k in self.parent.added_collections:
            lines += self.parent.get_child(k).generate_utility_types()
        # Utility for getting module id from enum type
        lines += [
            f'// Utility for getting {self.enum_name.lower()} '
            f'id from enum type',
        ]
        lines += self.generate_param_function(
            self.enum_name, for_header=True)
        lines += ['']
        # Utility for getting param type from enum type
        if self.accum_enum_name:
            lines += [
                f'// Utility for getting {self.accum_enum_name.lower()} '
                f'from enum type',
            ]
            lines += self.generate_param_function(
                self.accum_enum_name, for_header=True)
            lines += ['']
        # Utilities for getting names/values from enum type
        for k in self.parent.added_collections:
            lines += [
                f'// Utility for getting {k} from enum',
            ]
            specialize_direct = None
            if k == 'names':
                specialize_direct = {}
                specialize_direct[self.enum_name] = self.enum_members
                if self.accum_enum_name:
                    specialize_direct[self.accum_enum_name] = (
                        self.accum_enum_members)
            lines += self.parent.get_child(k).generate_function(
                self.enum_name, spec_param=['typename'],
                specialize_direct=specialize_direct,
                for_header=True, skip_items=['NONE', 'MAX'])
        # Base class
        if self.parent.as_class:
            lines += ['// Unspecialized enum']
            lines += self.parent.generate_enum(
                self.parent.as_class, [],
                spec_param=self.spec_param, spec_var=self.spec_var,
                specialize=False)
        else:
            # Empty enum
            lines += ['// Empty enum']
            lines += self.parent.generate_enum(
                self.empty_enum_name, [],
                spec_param=self.spec_param, spec_var=self.spec_var,
                specialize=False)
        # Include for module/param specific enums
        includes['global'] = [k for k in includes['global']
                              if k not in lines]
        # Utilities for getting names/values from enum type
        for k in self.parent.added_collections:
            if k in self.parent.added_files:
                new_include = self.parent.get_child(
                    k).get_child('header').include_self(
                        self.dst, rootdir=self.root_include_dir)
                for x in new_include:
                    if x not in includes[k]:
                        includes[k].append(x)
        include_lines = []
        for k, v in includes.items():
            if not v:
                continue
            if k == 'global':
                include_lines += ['// Global includes']
            else:
                include_lines += [f'// Specializations for get_enum_{k}']
            include_lines += self.add_chunk(
                f'headers_{k}', [x for x in v if x])
        if not self._independent_headers:
            lines += include_lines
        lines += ['']
        lines += [
            f'// Utility for getting enum type from '
            f'{self.enum_name.lower()} & '
            f'{self.accum_enum_name.lower()}',
        ]
        lines += self.generate_type_struct(
            self.enum_name, for_header=True)
        # Utility for getting name from enum
        lines += [
            '// Utility for getting name from enum',
            'template<typename T>',
            'std::string get_enum_name(const T& k) {',
            '  return get_enum_names<T>().find(k)->second;',
            '}',
        ]
        if not self.parent.as_class:
            # Begin preserved lines
            lines += self.begin_marker('preserved_utils', existing)
            for k, v in self.src.param.items():
                key = k.rsplit(self.strip_suffix)[0]
                lines += self.generate_type_struct(
                    self.enum_name, result=k, for_header=True,
                    specialize=[f'{self.enum_name}_{key}',
                                f'{self.accum_enum_name}'
                                f'{self.strip_suffix}'])
                lines += ['']
            lines += self.end_marker('preserved_utils')
        if self._independent_headers:
            lines = self.add_namespaces(lines)
            lines += include_lines
        if self._top_in_helper:
            lines = self.parent.include_helper_header() + lines
        if not self._independent_headers:
            lines = self.add_namespaces(lines)
        return lines


class CEnumGeneratorHelper(CEnumGeneratorBaseHeader):

    name = 'helper'
    file_suffix = ''
    explicit_dst = True

    def extract_existing(self, append=None, **kwargs):
        out = super(CEnumGeneratorHelper, self).extract_existing(
            append=append, **kwargs)
        if append and self.parent.get_child('global')._top_in_helper:
            new_chunk = self.parent.get_child(
                'global').generate_global_enum()
            out = self.replace_chunk('global_enum', out, new_chunk)
        return out

    def generate(self, *args, **kwargs):
        lines = []
        fglobal = self.parent.get_child('global')
        if self.parent.as_class and not self.append:
            if fglobal._top_in_helper:
                lines += self.add_chunk(
                    'global_enum',
                    fglobal.generate_global_enum(),
                    allow_empty=True
                )
            lines += ['// Unspecialized enum helper']
            lines += self.parent.generate_enum_class_helper(
                self.parent.as_class, [], specialize=False)
            if fglobal._independent_headers:
                lines = fglobal.add_namespaces(lines)
        lines += super(CEnumGeneratorHelper, self).generate(*args, **kwargs)
        return lines

    def generate_item(self, name, members, as_class=None, **kwargs):
        lines = []
        if as_class is None:
            as_class = self.parent.as_class
        if not as_class:
            return lines
        lines += self.parent.generate_enum_class_helper(
            name, members, as_class=as_class, **kwargs)
        return lines


class CEnumGeneratorHeader(CEnumGeneratorBaseHeader):

    name = 'c'
    file_suffix = '_enum'
    perfile_options = dict(
        CEnumGeneratorBaseHeader.perfile_options,
        as_class={
            'type': str,
            'help': "Create enum as a class with this name",
        },
        enum_in_source={
            'action': 'store_true',
            'help': "Define the enum in the source code."
        },
        macro_suffix={
            'type': str,
            'help': "Suffix to use for members macro",
        },
        explicit_values={
            'action': 'store_true',
            'help': "Explicitly define the value for each enum member",
        },
    )
    added_file_classes = {
        'global': CEnumGeneratorGlobalHeader,
        'helper': CEnumGeneratorHelper,
    }
    added_collection_classes = {
        'map': {
            'names': {
                'default_value_type': 'std::string',
                'default_value_key': 'abbr',
                '_utility_functions': [
                    x for x in CEnumGeneratorMapBase._utility_functions
                    if x not in ['print', 'string']
                ],
            },
            'values': {
                'default_value_type': 'double',
                'default_value_key': 'val',
            },
            'alternate_values': {
                'default_value_type': 'double',
                'default_value_key': 'val_alt',
                'default_secondary_value_key': 'val',
            },
            'glymaids': {
                'default_value_type': 'std::string',
                'default_value_key': 'GLYMAID',
                'default_explicit': True,
            },
            'aliases': {
                'default_value_type': 'std::string',
                'default_value_key': 'ALIASES',
                'default_reversed': True,
                'default_explicit': True,
            },
            'docs': {
                'default_value_type': 'std::string',
                'default_value_key': 'doc',
                'default_explicit': True,
            },
        },
        'flagmap': {
            'value_flags': {
                'default_value_flags': [
                    'skipped:SKIPPED',
                ],
                'default_is_editable': True,
                'default_flag_prefix': 'VALUE_FLAG',
                'default_explicit': True,
            },
            'static_value_flags': {
                'default_value_flags': [
                    'constant:CONST',
                    'calculated:CALC',
                    'nonvector:NON_VECTOR',
                    'resetone:RESET_ONE',
                    'initonce:INIT_ONCE',
                ],
                'default_flag_prefix': 'STATIC_VALUE_FLAG',
                'default_explicit': True,
            },
        },
    }

    @staticmethod
    def create_child_classes(cls):
        CEnumGeneratorBaseHeader.create_child_classes(cls)
        if cls.name is None or not cls.is_parent:
            return
        added = {}
        for ktype, classes in cls.added_collection_classes.items():
            if ktype == 'map':
                base = CEnumGeneratorMapBase
            elif ktype == 'flagmap':
                base = CEnumGeneratorFlagMapBase
            elif ktype == 'vector':
                base = CEnumGeneratorVectorBase
            else:
                raise NotImplementedError(
                    f"Unsupported collection class: {ktype}")
            for k, v in classes.items():
                added[k] = base.create_class(k, **v)
        cls.added_file_classes = dict(cls.added_file_classes, **added)

    @classmethod
    def extract_child_kws(cls, kwargs, child_kws, **kws_all):
        out = super(CEnumGeneratorHeader, cls).extract_child_kws(
            kwargs, child_kws, **kws_all)
        if ((CEnumGeneratorGlobalHeader._independent_headers
             and out.get('global', {}).get('namespaces', None))):
            for k, v in out.items():
                v['namespaces'] = out['global']['namespaces']
        return out

    @property
    def added_maps(self):
        return [k for k, v in self.added_file_classes.items()
                if issubclass(v, CEnumGeneratorMapBase)]

    @property
    def added_vectors(self):
        return [k for k, v in self.added_file_classes.items()
                if issubclass(v, CEnumGeneratorVectorBase)]

    @property
    def added_collections(self):
        return self.added_maps + self.added_vectors

    def generate_member(self, x, width=None, width_val=None,
                        name_only=False, explicit_values=False):
        assert width is not None and width_val is not None
        val = ''
        docs = ''
        if not name_only:
            if ((x.get('explicit_idx', False)
                 or self.explicit_values or explicit_values)):
                val = f" = {x['idx']}"
            if x.get('doc', False):
                docs = f"  //!< {x['doc']}"
        return [f"  {x['name']:{width}}{val:{width_val}},{docs}"]

    def include_helper_header(self):
        return self.include_file(
            self.get_child('helper').dst, caller=self.dst,
            rootdir=self.root_include_dir
        )

    def generate_enum_class_helper(self, name, members,
                                   enum_type='int', as_class=None,
                                   members_macro=None, **kwargs):
        lines = []
        if as_class is None:
            as_class = self.as_class
        if as_class:
            kwargs.setdefault('spec_param',
                              self.get_child('global').spec_param)
            kwargs.setdefault('spec_var',
                              self.get_child('global').spec_var)
            kwargs.setdefault('specialize',
                              self.get_child('global').item2specialize(name))
        template_lines = []
        specialization, spec_var = self.specialization(
            template_lines, **kwargs)
        self.complete_members(members)
        enum_name_ext = f'ENUM_{name}' if specialization else enum_type
        members_macro = None
        if specialization:
            members_macro = self.generate_definition_macro(
                name, members, return_macro=True)
            lines += self.generate_definition_macro(name, members)
            lines += ['']
        lines += ['#ifdef EPHOTO_USE_SCOPED_ENUM']
        if specialization:
            lines += self.generate_definition_enum(
                name, members, enum_name=enum_name_ext,
                enum_type=enum_type, as_class=as_class,
                scoped_enum=True, members_macro=members_macro)
        lines += template_lines
        lines += [
            f'struct enum_helper{specialization} {{',
            f'  typedef {enum_name_ext} type;',
            '};',
            '#endif // EPHOTO_USE_SCOPED_ENUM',
            '',
        ]
        return lines

    def generate_specialized_typedef(self, name, as_class=None, **kwargs):
        if as_class is None:
            as_class = self.as_class
        if not (as_class and kwargs.get('specialize', None)):
            return []
        specialization, spec_var = self.specialization(None, **kwargs)
        lines = []
        contexts = [
            k.rsplit(k0, 1)[-1].strip('_') for k0, k in
            zip(kwargs['spec_param'], kwargs['specialize'])
        ]
        contexts = contexts[::-1]
        lines += self.add_chunk(
            f'specialized_typedef_{name}',
            self.add_namespaces(
                [
                    f'typedef {as_class}{specialization} '
                    f'{contexts[-1]};'
                ],
                namespaces=contexts[:-1],
            )
        )
        return lines

    def generate_utilities(self, name, no_guards=False, marker=None,
                           class_name=None, specialization=''):
        if marker is None:
            marker = f'explicit_spec_{name}_utils'
        lines = []
        if not no_guards:
            lines += ['#ifdef EPHOTO_USE_SCOPED_ENUM']
        lines += [
            '// Definition of explicitly specialized enum class ',
            '//   utility methods. This causes instantiation of the ',
            '//   specialized class and must come after explicit ',
            '//   specialization of the class (and class members)',
        ]
        completed_utils = {}
        util_kws = {
            'enum_is_class': class_name,
            'enum_name': f'ENUM_{name}',
            'function_param': {
                'outside_class': f'{class_name}{specialization}',
            },
        }
        for k in self.added_collections:
            kchild = self.get_child(k)
            lines += kchild.generate_utility_methods(
                completed_utils, methods=kchild.friend_functions,
                **util_kws)
        if not no_guards:
            lines += ['#endif // EPHOTO_USE_SCOPED_ENUM']
        return self.add_chunk(marker, lines)

    def generate_declaration(self, name, members, enum_name=None,
                             enum_type='int', members_only=False,
                             as_class=None, macro_skip=[], **kwargs):
        lines = []
        if as_class is None:
            as_class = self.as_class
        if not as_class:
            lines += self.generate_definition_macro(
                name, members, macro_suffix='', skip=macro_skip)
            lines += self.generate_definition_macro(
                name, members, macro_suffix='',
                macro_prefix='MEMBER_NAMES_',
                skip=(macro_skip + ['NONE', 'MAX']),
                strip_member_prefix=(name + '_'))
            members_macro = self.generate_definition_macro(
                name, members, macro_suffix='', return_macro=True)
            lines += self.generate_definition_enum(
                name, members, enum_name=enum_name, enum_type=enum_type,
                as_class=as_class, members_macro=members_macro)
            lines += self.generate_definition_all(
                name, members, enum_name=enum_name,
                as_class=as_class)
            return lines
        if enum_name is None:
            enum_name = 'Type'
        template_lines = []
        class_name = as_class
        kwargs.setdefault('spec_param',
                          self.get_child('global').spec_param)
        kwargs.setdefault('spec_var',
                          self.get_child('global').spec_var)
        specialization, spec_var = self.specialization(
            template_lines, **kwargs)
        if specialization:
            members_only = True
        template = 'template<> ' if specialization else ''
        static = 'static ' if not specialization else ''
        enum_prefix = (
            f'{class_name}{specialization}::' if specialization else '')
        enum_name_full = f'{enum_prefix}{enum_name}'
        if specialization:
            enum_name_full = f'typename {enum_name_full}'
        if members_only:
            spec_marker = f'explicit_specialization_{name}'
            if specialization:
                lines += self.begin_marker(spec_marker)
            if specialization and not self.enum_in_source:
                members_macro = self.generate_definition_macro(
                    name, members, return_macro=True)
                lines += ['#ifndef EPHOTO_USE_SCOPED_ENUM']
                lines += template_lines
                lines += self.generate_definition_enum(
                    name, members, enum_name=enum_name,
                    enum_type=enum_type, enum_prefix=enum_prefix,
                    as_class=as_class, members_macro=members_macro)
                lines += ['#endif // EPHOTO_USE_SCOPED_ENUM']
            else:
                if as_class and spec_var:
                    lines += [
                        '#ifdef EPHOTO_USE_SCOPED_ENUM',
                        f'typedef typename '
                        f'enum_helper<{", ".join(spec_var)}>::'
                        f'type {enum_name};',
                        '#else // EPHOTO_USE_SCOPED_ENUM',
                    ]
                member_prefix = 'enum '
                lines += [
                    f'{template}{member_prefix}{enum_prefix}{enum_name} : '
                    f'{enum_type};',
                ]
                if as_class and spec_var:
                    lines += [
                        '#endif // EPHOTO_USE_SCOPED_ENUM',
                    ]
            if spec_var and not specialization:
                for p, t in zip(kwargs['spec_param'], spec_var):
                    lines += [f'{static}const {p} {p.lower()};']
            lines += self.generate_definition_all(
                name, members, enum_name=enum_name, as_class=as_class,
                declare=True, specialization=specialization)
            for k in self.added_collections:
                self.get_child(k).add_enum(enum_name)
                collection_type = self.get_child(k).generate_collection_type(
                    enum_name_full)
                const = '' if self.get_child(k).is_editable else 'const '
                lines += [
                    f'{template}{static}{const}{collection_type} '
                    f'{enum_prefix}{self.get_child(k).collection_name};'
                ]
                docs = self.get_child(k).generate_docs(k)
                if docs and not specialization:
                    lines[-1] += f'  /**< {docs} */'
            if specialization:
                lines += self.end_marker(spec_marker)
                lines += ['']
                lines += self.generate_utilities(
                    name, class_name=class_name,
                    marker=(spec_marker + '_utils'),
                    specialization=specialization)
            else:
                completed_utils = {}
                for k in self.added_collections:
                    lines += self.get_child(k).generate_utility_methods(
                        completed_utils,
                        enum_name=enum_name_full, enum_is_class=as_class)
                    lines += self.get_child(k).generate_additional_methods(
                        enum_name=enum_name_full, enum_is_class=as_class)
            return lines
        if ((spec_var and not specialization
             and not self.get_child('global')._top_in_helper)):
            lines += self.include_helper_header()
        lines += template_lines
        lines += [
            f'class {class_name}{specialization} {{',
            'public:',
        ]
        lines += [
            '  ' + x for x in self.generate_declaration(
                name, members, enum_name=enum_name, enum_type=enum_type,
                members_only=True, **kwargs)
        ]
        lines += ["};"]
        # Definition of static members for unspecialized class
        if spec_var and not specialization:
            unspecialization = f'<{", ".join(spec_var)}>'
            enum_prefix = f'{class_name}{unspecialization}::'
            enum_name_full = f'typename {enum_prefix}{enum_name}'
            for p, t in zip(kwargs['spec_param'], spec_var):
                lines += template_lines
                lines += [
                    f'const {p} {enum_prefix}{p.lower()} = {t};'
                ]
            lines += template_lines
            lines += self.generate_definition_all(
                name, members, enum_name=enum_name,
                as_class=as_class, specialization=unspecialization,
                template='')
            for k in self.added_collections:
                self.get_child(k).add_enum(enum_name)
                collection_type = self.get_child(k).generate_collection_type(
                    enum_name_full)
                const = '' if self.get_child(k).is_editable else 'const '
                lines += template_lines
                lines += [
                    f'{const}{collection_type} '
                    f'{enum_prefix}{self.get_child(k).collection_name}'
                    f' = {{}};'
                ]
        return lines

    def generate_definition_enum(self, name, members, enum_name=None,
                                 enum_type='int', as_class=None,
                                 enum_prefix='', scoped_enum=False,
                                 members_macro=None):
        lines = []
        if as_class is None:
            as_class = self.as_class
        if enum_name is None:
            if as_class:
                enum_name = 'Type'
            else:
                enum_name = name
        width = self.max_width(members)
        width_val = self.max_width(members, key='idx')
        if as_class and scoped_enum:
            member_prefix = 'enum class '
        else:
            member_prefix = 'enum '
        lines += [f'{member_prefix}{enum_prefix}{enum_name} : {enum_type} {{']
        if members_macro and not (self.explicit_values
                                  or any(x.get('explicit_idx', False)
                                         for x in members)):
            lines += ['  ' + members_macro]
        else:
            lines += [
                '  ' + x for x in
                super(CEnumGeneratorBaseHeader, self).generate_item(
                    name, members, width=width, width_val=width_val)
            ]
        lines += ['};']
        return lines

    def generate_definition_all(self, name, members, enum_name=None,
                                as_class=None, declare=False,
                                specialization=False, template=None):
        lines = []
        if as_class is None:
            as_class = self.as_class
        if template is None:
            template = 'template<> ' if specialization else ''
        static = 'static ' if not specialization else ''
        class_name = as_class if as_class else ''
        if enum_name is None:
            if as_class:
                enum_name = 'Type'
            else:
                enum_name = name
        enum_prefix = (
            f'{class_name}{specialization}::' if specialization else '')
        enum_name_full = f'{enum_prefix}{enum_name}'
        if specialization:
            enum_name_full = f'typename {enum_name_full}'
        if as_class:
            var_name = 'all'
        else:
            var_name = f'ALL_{enum_name}'
        lines += [
            f'{template}{static}'
            f'const std::vector<{enum_name_full}> '
            f'{enum_prefix}{var_name}'
        ]
        if declare:
            lines[-1] += ';'
        else:
            enum_member_prefix = enum_prefix
            if as_class:
                enum_member_prefix += f'SCOPED_ENUM_TYPE({enum_name})'
            members_core = [
                x for x in members if x['abbr'] not in ["NONE", "MAX"]
            ]
            prefixed_names = [
                enum_member_prefix + x["name"] for x in members_core
            ]
            lines[-1] += f' = {{{", ".join(prefixed_names)}}};'
        docs = '  /**< All enum values */'
        if (not as_class) or (declare and not specialization):
            lines[-1] += docs
        return lines

    def generate_definition_macro(self, name, members, macro_suffix=None,
                                  macro_prefix='MEMBERS_', skip=[],
                                  strip_member_prefix=False,
                                  return_macro=False):
        macro_name = name
        if macro_suffix is None:
            macro_suffix = self.macro_suffix
        if macro_suffix is not None:
            macro_name = name.rsplit('_', 1)[0] + macro_suffix
        if return_macro:
            return f'{macro_prefix}{macro_name}'
        lines = [
            f'#define {macro_prefix}{macro_name}'
        ]
        if strip_member_prefix:
            members = [
                dict(x, name=x['name'].split(strip_member_prefix)[-1])
                for x in members
            ]
        members = [x for x in members if x['name'] not in skip]
        if members:
            lines[-1] += '\t\t\\'
        width = self.max_width(members)
        width_val = self.max_width(members, key='idx')
        values = super(CEnumGeneratorBaseHeader, self).generate_item(
            name, members, width=width, width_val=width_val,
            name_only=True)
        for i in range(len(values)):
            if i == len(values) - 1:
                values[i] = '  ' + values[i].rstrip(',').rstrip()
            else:
                values[i] = '  ' + values[i] + '\t\t\\'
        lines += values
        return lines

    def complete_members(self, members):
        first = "NONE"
        last = "MAX"
        skip_items = [first, last]
        if (not members) or members[0]['abbr'] != first:
            members.insert(0, {'name': first, 'abbr': first})
        if (not members) or members[-1]['abbr'] != last:
            members.append({'name': last, 'abbr': last})
        return skip_items

    def generate_definition(self, name, members, enum_name=None,
                            enum_type='int', as_class=None, **kwargs):
        if as_class is None:
            as_class = self.as_class
        if enum_name is None:
            if as_class:
                enum_name = 'Type'
            else:
                enum_name = name
        skip_items = self.complete_members(members)
        lines = []
        template_lines = []
        enum_prefix = ''
        specialization = ''
        if as_class:
            class_name = as_class
            kwargs.setdefault('spec_param',
                              self.get_child('global').spec_param)
            kwargs.setdefault('spec_var',
                              self.get_child('global').spec_var)
            kwargs.setdefault('specialize',
                              self.get_child('global').item2specialize(name))
            specialization, spec_var = self.specialization(
                template_lines, **kwargs)
            enum_prefix = f'{class_name}{specialization}::'
        if self.enum_in_source:
            members_macro = self.generate_definition_macro(
                name, members, return_macro=True,
                macro_suffix=(None if as_class else ''))
            lines += template_lines
            lines += self.generate_definition_enum(
                name, members, enum_name=enum_name, enum_type=enum_type,
                enum_prefix=enum_prefix, as_class=as_class,
                members_macro=members_macro)
        if as_class:
            template = 'template<> ' if specialization else ''
            lines += self.generate_definition_all(
                name, members, enum_name=enum_name, as_class=as_class,
                specialization=specialization)
            for k in self.added_collections:
                self.get_child(k).add_enum(enum_name)
                if template:
                    lines += [template]
                lines += self.get_child(k).generate_definition(
                    enum_name, members,
                    var_name=(f'{enum_prefix}'
                              f'{self.get_child(k).collection_name}'),
                    skip_items=skip_items, in_class=enum_prefix)
        lines += ['']
        return lines

    def generate_source_item(self, name, members):
        if not self.as_class:
            return []
        specialize = self.get_child('global').item2specialize(name)
        return self.generate_definition(name, members,
                                        specialize=specialize)

    def generate_source_closing(self, **kwargs):
        lines = super(CEnumGeneratorHeader, self).generate_source_closing(
            **kwargs)
        # for k in ['NONE', 'MAX']:
        #     members = [self.add_member(x) for x in ['NONE', 'MAX']]
        #     name = f"{k}{self.get_child('global').strip_suffix}"
        #     specialize = self.get_child('global').item2specialize(name)
        #     lines += self.generate_definition(name, members,
        #                                       specialize=specialize)
        return lines

    def generate_item(self, name, members, as_class=None, **kwargs):
        if as_class is None:
            as_class = self.as_class
        if as_class:
            kwargs.setdefault('spec_param',
                              self.get_child('global').spec_param)
            kwargs.setdefault('spec_var',
                              self.get_child('global').spec_var)
            kwargs.setdefault('specialize',
                              self.get_child('global').item2specialize(name))
        lines = []
        lines += self.generate_declaration(
            name, members, as_class=as_class, **kwargs)
        lines += self.generate_specialized_typedef(
            name, as_class=as_class, **kwargs)
        lines += ['']
        return lines

    def generate(self, *args, **kwargs):
        lines = super(CEnumGeneratorHeader, self).generate(*args, **kwargs)
        # for k in ['NONE', 'MAX']:
        #     members = [self.add_member(x) for x in ['NONE', 'MAX']]
        #     lines += self.generate_enum(
        #         f"{k}{self.get_child('global').strip_suffix}", members)
        if self.get_child('global')._independent_headers:
            lines = self.get_child('global').add_namespaces(lines)
        lines = self.include_helper_header() + lines
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
            return [f"        {x['name']} = {x['idx']}, &"]
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
            return [f"        {x['name']} = {x['idx']}, &"]
        else:
            assert tsuffix
            return [
                f"integer(kind={tsuffix}), parameter :: "
                f"{x['name']} = {x['idx']}_{tsuffix}"]

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
                        default='names',
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
        dstcls = get_registered_class('generator', args.dst_type)
        kws = {}
        if args.prefix_by_split:
            assert not args.prefix_with_type
            kws['prefixes'] = src.prefix_by_split(args.prefix_by_split)
        elif args.prefix_with_type:
            kws['prefixes'] = {k: [f"{k}_"] for k in src.param.keys()}
        dst_children = dstcls.all_children()
        for x in registered_classes('generator', return_classes=True):
            if x.name not in [dstcls.name] + dst_children:
                continue
            x.get_arguments(args, kws, top_level=(x.name == args.dst_type))
        dst = dstcls(
            src, dst=args.dst, top_level=True, overwrite=args.overwrite,
            verbose=args.verbose, dry_run=args.dry_run,
            skip_children=args.skip_children,
            root_include_dir=args.root_include_dir, **kws)
