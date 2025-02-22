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


def max_width(members, key='name', func=None, current_keys=None):
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
        width = len(create_enum_member(
            max(members, key=lambda x:
                len(create_enum_member(x, return_name=True,
                                       current_keys=current_keys))),
            return_name=True, current_keys=current_keys))
    else:
        width = len(max(members, key=lambda x:
                        len(x.get(key, ''))).get(key, ''))
    if key == 'idx':
        width += 3
    return width


def create_enum_member(member, return_name=False, add_prefix=None,
                       strip_suffix=None, replace_with_prefix=False,
                       add_kwargs=None, current_keys=None):
    if current_keys is None:
        current_keys = {
            'kprefix': [],
            'ksuffix': [],
            'kreplacement': {},
            'klower': False,
        }
    if isinstance(member, str):
        member = {'name': member}
    name = member['name']
    abbr = member.get('abbr', name)
    if replace_with_prefix and current_keys['kprefix']:
        name = ''
    name = current_keys['kreplacement'].get(
        name, name)
    if current_keys['klower']:
        name = name.lower()
    if not member.get('no_suffix', False):
        for x in current_keys['ksuffix']:
            name = name + x
    if not member.get('no_prefix', False):
        for x in current_keys['kprefix']:
            name = x + name
    if replace_with_prefix and current_keys['kprefix']:
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


def generate_enum_member(x, width=None, width_val=None,
                         name_only=False, explicit_values=False):
    assert width is not None and width_val is not None
    if isinstance(x, str):
        x = create_enum_member(x)
    val = ''
    docs = ''
    if not name_only:
        if x.get('explicit_idx', False) or explicit_values:
            val = f" = {x['idx']}"
        if x.get('doc', False):
            docs = f"  //!< {x['doc']}"
    return f"  {x['name']:{width}}{val:{width_val}},{docs}"


def generate_enum_members(members, indent='  ', members_macro=None,
                          ragged_right=False, **kwargs):
    if members_macro and not (kwargs.get('explicit_values', False)
                              or any(x.get('explicit_idx', False)
                                     for x in members)):
        if isinstance(members_macro, EnumMacroUnit):
            members_macro = members_macro.macro_name
        return generate_enum_members(members_macro, indent=indent,
                                     ragged_right=ragged_right, **kwargs)
    if isinstance(members, str):
        out = [indent + members]
    else:
        width = max_width(members)
        if ragged_right:
            width_val = 0
        else:
            width_val = max_width(members, key='idx')
        out = [
            indent + generate_enum_member(
                x, width=width, width_val=width_val, **kwargs,
            ) for x in members
        ]
    return out


class CodeContext:

    parameter_keys = [
        'in_header', 'in_source', 'inside_class', 'specialize',
        'definition', 'declaration', 'filetype', 'instantiation',
    ]

    def __init__(self, **kwargs):
        self.update(**kwargs)

    @classmethod
    def from_kwargs(cls, kwargs):
        ikws = {}
        for k in ['in_header', 'in_source']:
            if k in kwargs:
                ikws[k] = kwargs.pop(k)
        return cls(**ikws)

    @property
    def primary(self):
        return (self.declaration
                and (not self.specialize))

    @property
    def outside_class(self):
        if self.inside_class:
            return False
        return True

    @property
    def parameters(self):
        return {k: getattr(self, k) for k in self.parameter_keys}

    def update(self, in_header=None, in_source=None,
               inside_class=False, specialize=None,
               definition=None, declaration=None,
               filetype=None, instantiation=None):
        if in_header is None:
            in_header = getattr(self, 'in_header', None)
        if in_source is None:
            in_source = getattr(self, 'in_source', None)
        if inside_class is None:
            inside_class = getattr(self, 'inside_class', None)
        if inside_class:
            assert isinstance(inside_class, str)
        if specialize is None:
            specialize = getattr(self, 'specialize', None)
        if definition is None:
            definition = getattr(self, 'definition', None)
        if declaration is None:
            declaration = getattr(self, 'declaration', None)
        if filetype is None:
            filetype = getattr(self, 'filetype', None)
        if instantiation is None:
            instantiation = getattr(self, 'instantiation', None)
        if in_header is None:
            if in_source is None:
                in_source = False
            in_header = (not in_source)
        elif in_source is None:
            in_source = (not in_header)
        else:
            assert in_header == (not in_source)
        if in_source:
            inside_class = False
            definition = True
            declaration = False
        if declaration is None:
            declaration = True
        if definition is None:
            if specialize:
                definition = False
            else:
                definition = True
        if instantiation is None:
            instantiation = False
        self.in_header = in_header
        self.in_source = in_source
        self.inside_class = inside_class
        self.specialize = specialize
        self.declaration = declaration
        self.definition = definition
        self.filetype = filetype
        self.instantiation = instantiation


class CodeUnitBase:

    generate_str = False
    dont_add_template_lines = False
    is_class = False
    non_child_attributes = ['_parent', '__dict__']

    def __init__(self, name, parent=None, static=True,
                 export=False, spec_param=None, spec_var=None,
                 docs=None, namespaces=None, guards=None, chunks=None,
                 context=CodeContext(), conditions=None,
                 temp_parent=None, no_parent=False):
        if spec_param:
            if spec_var is None:
                if len(spec_param) == 1:
                    spec_var = ['T']
                else:
                    spec_var = [
                        f'T{i}' for i in range(len(spec_param))]
            assert len(spec_var) == len(spec_param)
        if isinstance(namespaces, str):
            namespaces = [namespaces]
        if isinstance(guards, (str, GuardUnit)):
            guards = [guards]
        if guards:
            assert isinstance(guards, list)
            for i in range(len(guards)):
                if not isinstance(guards[i], GuardUnit):
                    guards[i] = GuardUnit(guards[i])
        if isinstance(chunks, str):
            chunks = [chunks]
        if conditions is None:
            conditions = {}
        elif isinstance(conditions, str):
            conditions = {'generate': [[conditions]]}
        elif ((isinstance(conditions, list) and conditions
               and isinstance(conditions[0], str))):
            conditions = {'generate': [conditions]}
        elif isinstance(conditions, list):
            conditions = {'generate': conditions}
        self.name = name
        self.no_parent = no_parent
        self._parent = parent
        self._static = static
        self.export = export
        self.spec_param = spec_param
        self.spec_var = spec_var
        self.docs = docs
        self.namespaces = namespaces
        self.guards = guards
        self.chunks = chunks
        self.conditions = conditions
        self.temp_parent = temp_parent
        self.context = context

    @classmethod
    def _eval(cls, x):
        if isinstance(x, list):
            return [cls._eval(xx) for xx in x]
        if isinstance(x, str):
            return x
        elif isinstance(x, CodeUnitBase):
            return x.generate()
        raise NotImplementedError(str(type(x)))

    def set_parent(self, new_parent, temporary=False):
        if self.no_parent:
            return
        if temporary:
            self.temp_parent = new_parent
            self._parent = None
        else:
            self._parent = new_parent
            self.temp_parent = None

    @property
    def parent(self):
        if self.temp_parent:
            return self.temp_parent
        return self._parent

    @classmethod
    def iter_units(cls, k, v):
        if isinstance(v, CodeUnitBase):
            yield v
        elif isinstance(v, list):
            for vv in v:
                if isinstance(vv, CodeUnitBase):
                    yield vv
        elif isinstance(v, dict):
            for kk, vv in v.items():
                assert not isinstance(kk, CodeUnitBase)
                for vvv in cls.iter_units((k, kk), vv):
                    yield vvv

    @property
    def children(self):
        for k in dir(self):
            if ((k in self.non_child_attributes
                 or isinstance(getattr(type(self), k, None), property))):
                continue
            v = getattr(self, k)
            for vv in self.iter_units(k, v):
                yield vv

    @property
    def outside_class(self):
        return (self.parent_class and not self.inside_class)

    @property
    def primary(self):
        return self.context.primary and not (self.context.definition
                                             and self.outside_class)

    @property
    def inside_class(self):
        return (self.parent_class and self.context.inside_class
                and self.parent_class.name == self.context.inside_class)

    @property
    def parent_class(self):
        if not self.parent:
            return None
        if self.parent.is_class:
            return self.parent
        return self.parent.parent_class

    def set_context(self, context=None, **kwargs):
        if context is not None:
            kwargs = dict(context.parameters, **kwargs)
        self.context = CodeContext(**kwargs)
        for v in self.children:
            v.set_context(**kwargs)

    def update_context(self, **kwargs):
        self.context.update(**kwargs)
        for v in self.children:
            v.update_context(**kwargs)

    @property
    def specialized(self):
        if not (self.spec_var and self.context.specialize):
            return []
        param = [
            self.context.specialize.get(k, None) for k in self.spec_var
        ]
        if all(param):
            return param
        return []

    @property
    def instantiated(self):
        return self.specialized and self.context.instantiation

    @property
    def specialization(self):
        param = self.specialized
        if not param:
            return ''
        return f"<{', '.join(param)}>"

    @property
    def template(self):
        if not self.spec_var:
            return ''
        if self.specialized:
            if self.instantiated:
                return 'template '
            return 'template<> '
        param = [f"{x} {t}" for x, t in zip(self.spec_param, self.spec_var)]
        return f"template<{', '.join(param)}> "

    @property
    def template_lines(self):
        out = []
        if self.parent_class and self.outside_class:
            out += self.parent_class.template_lines
        template = self.template
        if template:
            out += [template.strip()]
        return out

    @property
    def api(self):
        if ((self.context.primary and self.export and self.inside_class
             and (not self.check_conditions('define')))):
            return f'{self.export} '
        return ''

    @property
    def static(self):
        if (self.context.primary or self.inside_class) and self._static:
            return 'static '
        return ''

    def __format__(self, spec):
        out = self.generate()
        if isinstance(out, list) and len(out) == 1:
            return out[0]
        if isinstance(out, list) and len(out) == 0:
            return ''
        assert isinstance(out, str)
        return out

    @property
    def prefix(self):
        return f'{self.static}{self.api}'

    @property
    def child_address(self):
        return f'{self.type}::'

    @property
    def parent_address(self):
        out = ''
        if self.parent_class and not self.inside_class:
            out = self.parent_class.child_address
        return out

    def get_condition(self, k):
        not_flag = "NOT:"
        eq_flag = "EQ:"
        reverse = k.startswith(not_flag)
        if reverse:
            k = k.split(not_flag)[-1]
        equals = k.startswith(eq_flag)
        if equals:
            k, equals = k.split(eq_flag)[-1].split(':')
        v = getattr(self.context, k)
        if equals:
            v = (v == equals)
        else:
            v = bool(v)
        if reverse:
            return (not v)
        return v

    def check_conditions(self, step='generate'):
        if isinstance(step, tuple):
            cond = step
        else:
            cond = self.conditions.get(step, [])
        if not cond:
            return True
        for kSet in cond:
            if all(self.get_condition(k) for k in kSet):
                return True
        return False

    def _declare(self):
        raise NotImplementedError

    def _define(self):
        raise NotImplementedError

    def _generate(self):
        out = self.declare(no_semicolon=True)
        self.add_definition(out)
        return out

    def generate(self, top=False):
        if not self.check_conditions():
            return []
        template_lines = (
            None if (self.generate_str or self.dont_add_template_lines)
            else self.template_lines
        )
        out = self._generate()
        if template_lines and out:
            if isinstance(out, str):
                out = [out]
            out = template_lines + out
        return self.finalize_generate(out)

    def define(self):
        # TODO: Check self.context.definition?
        if not (self.check_conditions('define')):
            return []
        return self._define()

    def declare(self, force=False, no_semicolon=False):
        if not (force or self.check_conditions('declare')):
            return []
        out = [self._declare()]
        if self.dont_add_template_lines:
            template = ''
            template_lines = self.template_lines
            if template_lines and '<' not in template_lines[-1]:
                template = template_lines[-1] + ' '
                template_lines = template_lines[:-1]
            out = template_lines + [f'{template}{out[0]}']
        if not no_semicolon:
            out[-1] += ';'
        return out

    def instantiate(self):
        if not (self.check_conditions('instantiate')
                and self.instantiated):
            return []
        return self.declare(force=True)

    def add_definition(self, out, no_semicolon=False,
                       add_brakets=False, indent=0,
                       prefix=None, suffix=None):
        definition = self.define()
        if add_brakets:
            indent += 1
        if indent:
            definition = ['  ' * indent + f'{x}' for x in definition]
        if prefix and definition:
            definition = prefix + definition
        if suffix and definition:
            definition = definition + suffix
        if add_brakets and definition:
            definition = [' {'] + definition + ['}']
        if definition:
            out[-1] += definition[0]
            out += definition[1:]
        elif no_semicolon == 'declaration':
            out[-1] += ';'
        if not no_semicolon:
            out[-1] += ';'
        return out

    def finalize_generate(self, out):
        return self.add_bounds(self.add_docs(out))

    def add_docs(self, out):
        if not (self.primary and self.docs):
            return out
        assert isinstance(out, list)
        if isinstance(self.docs, str):
            out = ['/** ' + self.docs + ' */'] + out
        else:
            assert isinstance(self.docs, list)
            out = ['/**'] + [
                '  ' + x for x in self.docs
            ] + ['*/'] + out
        return out

    def add_bounds(self, out):
        out = self.add_chunks(out)
        out = self.add_namespaces(out)
        out = self.add_guards(out)
        return out

    def add_chunks(self, out):
        if not (out and self.chunks):
            return out
        for x in self.chunks:
            out = EnumGeneratorBase.add_chunk(x, out)
        return out

    def add_guards(self, out):
        if not (out and self.guards):
            return out
        assert isinstance(self.guards, list)
        for x in self.guards[::-1]:  # inside out
            assert x.empty()
            x.first = out
            out = x._generate()
            x.first = None
        return out

    def add_namespaces(self, out):
        if not (out and self.namespaces):
            return out
        out = [
            ('  ' * i) + f'namespace {x} {{'
            for i, x in enumerate(self.namespaces)
        ] + [
            ('  ' * len(self.namespaces)) + x for x in out
        ] + [
            ('  ' * (len(self.namespaces) - (i + 1))) + '}'
            for i in range(len(self.namespaces))
        ]
        return out


class EmptyCodeUnit(CodeUnitBase):

    dont_add_template_lines = True

    def __init__(self):
        super(EmptyCodeUnit, self).__init__(None)

    def _generate(self):
        return []


class ContainerUnit(CodeUnitBase):

    dont_add_template_lines = True

    def __init__(self, members=None, name=None, **kwargs):
        if members is None:
            members = []
        self.members = []
        for m in members:
            self.add_member(m)
        super(ContainerUnit, self).__init__(name, **kwargs)

    def add_member(self, m, index=None):
        m.set_parent(self)
        if index is None:
            self.members.append(m)
        else:
            self.members.insert(index, m)

    def add_members(self, m):
        for x in m:
            self.add_member(x)

    def _generate(self):
        out = []
        for m in self.members:
            out += m.generate()
        return out


class ClassUnit(ContainerUnit):

    dont_add_template_lines = True
    is_class = True

    def __init__(self, name, members=None, **kwargs):
        self.class_type = kwargs.pop('class_type', 'class')
        self.specialize_class = kwargs.pop('specialize_class', False)
        self.complete_class = False
        self.body = kwargs.pop('body', [])
        kwargs.update(name=name, members=members)
        super(ClassUnit, self).__init__(**kwargs)

    @property
    def type(self):
        return TypeUnit(self.name,
                        spec_var=self.spec_var,
                        spec_param=self.spec_param,
                        parent=self.parent,
                        context=self.context)

    def update_member_context(self, inside_class=None, **kwargs):
        updated_kwargs = {}
        if inside_class is None:
            inside_class = (
                self.context.declaration
                and ((not self.context.specialize)
                     or self.specialize_class)
            )
            if inside_class:
                inside_class = self.name
            updated_kwargs['inside_class'] = inside_class
        self.complete_class = (inside_class == self.name)
        if updated_kwargs:
            for m in self.members:
                m.update_context(**updated_kwargs)

    def set_context(self, **kwargs):
        out = super(ClassUnit, self).set_context(**kwargs)
        self.update_member_context(**kwargs)
        return out

    def update_context(self, **kwargs):
        out = super(ClassUnit, self).update_context(**kwargs)
        self.update_member_context(**kwargs)
        return out

    def add_definition(self, *args, **kwargs):
        kwargs['add_brakets'] = True
        if self.class_type in ['class']:
            kwargs['prefix'] = ['public:']
        return super(ClassUnit, self).add_definition(*args, **kwargs)

    def _declare(self):
        return (f'{self.api}{self.class_type} {self.name}'
                f'{self.specialization}')

    def _define(self):
        out = []
        for m in self.members:
            out += m.generate()
        if self.context.declaration and self.complete_class:
            out += self.body
        return out

    def _generate(self):
        if not (self.context.declaration and self.complete_class):
            return self.define()
        out = super(ContainerUnit, self)._generate()
        if not self.specialized:  # or self.api:
            self.update_context(inside_class=False,
                                definition=True,
                                declaration=False)
            out += self._generate()
        return out


class TypeUnit(CodeUnitBase):

    generate_str = True

    def __init__(self, *args, **kwargs):
        self.constant = kwargs.pop('constant', False)
        self.specialize = kwargs.pop('specialize', None)
        super(TypeUnit, self).__init__(*args, **kwargs)

    @property
    def type(self):
        return self.generate()

    def _generate(self):
        const = 'const ' if self.constant else ''
        parent_address = self.parent_address
        if parent_address:
            parent_address = f'typename {parent_address}'
        if self.specialize:
            spec_param = [f'{x}' for x in self.specialize]
            specialization = f"<{', '.join(spec_param)}>"
        elif self.spec_var and not self.context.specialize:
            specialization = f"<{', '.join(self.spec_var)}>"
        else:
            specialization = self.specialization
        name = self.name
        if self.context.specialize and name in self.context.specialize:
            name = self.context.specialize[name]
        return f'{const}{parent_address}{name}{specialization}'


class TypedefUnit(CodeUnitBase):

    def __init__(self, *args, **kwargs):
        self.type = kwargs.pop('type', None)
        assert self.type
        super(TypedefUnit, self).__init__(*args, **kwargs)

    def _generate(self):
        if (((not self.context.in_header)
             or (self.parent_class and not self.inside_class))):
            return []
        if f'{self.type}' == self.name:
            return []
        return [f'typedef {self.type} {self.name};']


class MacroUnit(CodeUnitBase):

    dont_add_template_lines = True

    def __init__(self, *args, **kwargs):
        self.body = kwargs.pop('body', [])
        self._args = kwargs.pop('args', [])
        super(MacroUnit, self).__init__(*args, **kwargs)

    @property
    def args(self):
        if not self._args:
            return ''
        return '(' + ', '.join(self._args) + ')'

    @property
    def macro_name(self):
        return f'{self.name}'

    def _generate(self):
        out = []
        out += [
            f'#define {self.macro_name}{self.args}'
        ]
        if self.body:
            out[-1] += '\t\t\\'
            for i, x in enumerate(self.body):
                if i == len(self.body) - 1:
                    out.append('  ' + x.rstrip(',').rstrip())
                else:
                    out.append('  ' + x + '\t\t\\')
        return out

    def _define(self):
        return self.body


class VariableUnit(CodeUnitBase):

    def __init__(self, *args, **kwargs):
        self.value = kwargs.pop('value', None)
        self.type = kwargs.pop('type', None)
        self.constant = kwargs.pop('constant', False)
        self.reference = kwargs.pop('reference', False)
        super(VariableUnit, self).__init__(*args, **kwargs)

    def _declare(self):
        const = 'const ' if self.constant else ''
        ref = '&' if self.reference else ''
        return (f'{self.prefix}{const}{self.type}{ref} '
                f'{self.parent_address}{self.name}')

    def _define(self):
        value = self.value
        if isinstance(value, CodeUnitBase):
            value = value.generate()
        if isinstance(value, str):
            value = [value]
        out = [f' = {value[0]}']
        out += value[1:]
        return out

    def _generate(self):
        out = super(VariableUnit, self)._generate()
        if ((self.primary and self.docs
             and isinstance(self.docs, str))):
            out[-1] += f'  /**< {self.docs} */'
        return out

    def add_docs(self, out):
        if isinstance(self.docs, str):
            return out
        return super(VariableUnit, self).add_docs(out)


class ArgUnit(VariableUnit):

    def __init__(self, *args, **kwargs):
        kwargs['value'] = kwargs.pop('default', None)
        kwargs['static'] = False
        super(ArgUnit, self).__init__(*args, **kwargs)

    def add_definition(self, *args, **kwargs):
        kwargs['no_semicolon'] = True
        return super(ArgUnit, self).add_definition(*args, **kwargs)

    def _define(self):
        if (not self.context.declaration) or self.value is None:
            return []
        return super(ArgUnit, self)._define()

    def add_docs(self, out):
        return out


class ValueUnit(CodeUnitBase):

    generate_str = True
    non_child_attributes = CodeUnitBase.non_child_attributes + [
        'fstring_param'
    ]

    def __init__(self, *args, **kwargs):
        self.fstring_param = kwargs.pop('fstring_param', None)
        self.ragged_right = kwargs.pop('ragged_right', False)
        super(ValueUnit, self).__init__(*args, **kwargs)

    def _generate_value(self, x, width=None):
        if isinstance(x, CodeUnitBase):
            out = x.generate()
        elif isinstance(x, (list, tuple)):
            if width is not None:
                assert len(width) == len(x)
                out = [self._generate_value(xx, width=width[i])
                       for i, xx in enumerate(x)]
            else:
                out = [self._generate_value(xx) for xx in x]
            return f"{{{', '.join(out)}}}"
        elif isinstance(self.parent, EnumUnit):
            return f'{self.parent.value_address}{x}'
        else:
            out = f'{self.parent_address}{x}'
        if width:
            out = f"{out:{width}}"
        return out

    def _value_width(self, members, recurse=False):
        if not members:
            return None
        if isinstance(members[0], list):
            len0 = len(members[0])
            if all(len(x) == len0 for x in members[1:]):
                out = [self._value_width([x[i] for x in members],
                                         recurse=True)
                       for i in range(len(members[0]))]
                if self.ragged_right:
                    out[-1] = None
                return out
            else:
                return None
        elif self.ragged_right and not recurse:
            return None
        members = [self._generate_value(x) for x in members]
        return len(max(members, key=len))

    @property
    def value(self):
        if callable(self.name):
            return self.name(self)
        elif self.fstring_param is not None:
            assert isinstance(self.name, str)
            return self.name.format(**self.fstring_param)
        return self.name

    def _generate(self):
        value = self.value
        if isinstance(value, list):
            width = self._value_width(value)
            out = [f'  {self._generate_value(x, width)}' for x in value]
            for i in range(len(value) - 1):
                out[i] += ','
            if out:
                out = ['{'] + out + ['}']
            else:
                out = ['{}']
            return out
        return self._generate_value(value)


class ConditionalValueUnit(ValueUnit):

    @property
    def value(self):
        assert self.name and isinstance(self.name, dict)
        for k, v in self.name.items():
            if self.check_conditions(k):
                return v
        raise RuntimeError(f'Could not find a valid condition:\n'
                           f'{pprint.pformat(self.name)}')


class FunctionUnit(CodeUnitBase):

    def __init__(self, *args, **kwargs):
        self.type = kwargs.pop('type', 'void')
        self.body = kwargs.pop('body', [])
        self._args = kwargs.pop('args', [])
        self.constant = kwargs.pop('constant', False)
        self.friend = kwargs.pop('friend', False)
        assert self.type and self.body
        super(FunctionUnit, self).__init__(*args, **kwargs)

    @property
    def args(self):
        args = [f'{x}' for x in self._args]
        assert all(args)
        return ', '.join(args)

    def add_definition(self, *args, **kwargs):
        kwargs['add_brakets'] = True
        return super(FunctionUnit, self).add_definition(*args, **kwargs)

    def _declare(self):
        friend = 'friend ' if self.friend else ''
        inline = ''
        if ((self.context.definition and self.context.declaration
             and not (self.template_lines or self.static))):
            inline = 'inline '
        out = (f'{friend}{inline}{self.prefix}'
               f'{self.type} {self.parent_address}{self.name}'
               f'{self.specialization}({self.args})')
        if self.constant:
            out += ' const'
        return out

    def _define(self):
        return copy.deepcopy(self.body)


class EnumUnit(CodeUnitBase):

    is_class = True

    def __init__(self, name, members, **kwargs):
        self.enum_type = kwargs.pop('enum_type', 'int')
        self.members = members
        self.members_macro = kwargs.pop('members_macro', None)
        self.enum_name_ext = kwargs.pop('enum_name_ext', None)
        self.explicit_values = kwargs.pop('explicit_values', False)
        self.scoped_enum = kwargs.pop('scoped_enum', False)
        self.ragged_right = kwargs.pop('ragged_right', False)
        super(EnumUnit, self).__init__(name, **kwargs)

    @property
    def class_attr(self):
        if self.scoped_enum:
            return 'class '
        return ''

    @property
    def type(self):
        return TypeUnit(self.name, parent=self.parent,
                        context=self.context)

    @property
    def value_address(self):
        out = self.child_address
        if (((not self.scoped_enum) and self.parent_class
             and self.outside_class)):
            out += f'SCOPED_ENUM_TYPE({self.name})'
        return out

    @property
    def child_address(self):
        if self.scoped_enum:
            return super(EnumUnit, self).child_address
        return f'{self.parent_address}'

    def external_enum(self, **kwargs):
        if not self.enum_name_ext:
            return EmptyCodeUnit()
        default_kws = dict(
            enum_type=self.enum_type,
            members_macro=self.members_macro,
            explicit_values=self.explicit_values,
            scoped_enum=True,
        )
        for k, v in default_kws.items():
            kwargs.setdefault(k, v)
        return EnumUnit(self.enum_name_ext, self.members, **kwargs)

    def add_definition(self, *args, **kwargs):
        kwargs['add_brakets'] = True
        return super(EnumUnit, self).add_definition(*args, **kwargs)

    def _declare(self):
        return (f'enum {self.class_attr}{self.parent_address}'
                f'{self.name} : {self.enum_type}')

    def _define(self):
        if self.parent_class and self.context.primary:
            return []
        members_macro = self.members_macro
        if isinstance(members_macro, EnumUnit):
            members_macro = members_macro.members_macro
        if isinstance(members_macro, EnumMacrosUnit):
            members_macro = members_macro.macro_base
        out = generate_enum_members(
            self.members, explicit_values=self.explicit_values,
            members_macro=members_macro,
            ragged_right=self.ragged_right,
            indent=''
        )
        return out

    def _generate(self):
        out = super(EnumUnit, self)._generate()
        if ((self.enum_name_ext and self.parent_class
             and self.parent_class.specialized)):
            typedef = TypedefUnit(
                name=self.enum_name_ext,
                type=self.type,
                context=self.context,
                conditions=[['declaration',  'specialize']],
            )
            out += typedef.generate()
        return out


class EnumMacroUnit(MacroUnit):

    def __init__(self, name, members, macro_suffix=None,
                 macro_prefix='MEMBERS_', skip=[],
                 strip_member_prefix=False, width=None,
                 width_val=None, current_keys=None, **kwargs):
        macro_name = name
        self.macro_suffix = macro_suffix
        self.macro_prefix = macro_prefix
        self.disable = False
        if strip_member_prefix:
            if any(x['name'].startswith(strip_member_prefix)
                   for x in members):
                members = [
                    dict(x, name=x['name'].split(strip_member_prefix)[-1])
                    for x in members
                ]
            else:
                self.disable = True
                members = []
        members = [x for x in members if x['name'] not in skip]
        if width is None:
            width = max_width(members, current_keys=current_keys)
        if width_val is None:
            width_val = max_width(members, current_keys=current_keys,
                                  key='idx')
        body = []
        for x in members:
            body.append(
                generate_enum_member(
                    x, name_only=True,
                    width=width, width_val=width_val,
                )
            )
        super(EnumMacroUnit, self).__init__(macro_name, body=body, **kwargs)

    @property
    def macro_name(self):
        out = super(EnumMacroUnit, self).macro_name
        if self.macro_suffix is not None:
            out = out.rsplit('_', 1)[0] + self.macro_suffix
        if self.macro_prefix:
            out = self.macro_prefix + out
        return out

    def _generate(self):
        if self.disable:
            return []
        return super(EnumMacroUnit, self)._generate()


class EnumMacrosUnit(ContainerUnit):

    def __init__(self, name, members, skip=[], **kwargs):
        assert 'macro_prefix' not in kwargs
        enum_kws = {}
        for k in ['macro_suffix']:
            if k in kwargs:
                enum_kws[k] = kwargs.pop(k)
        macros = [
            EnumMacroUnit(name, members, skip=skip,
                          macro_prefix='MEMBERS_',
                          **enum_kws),
            EnumMacroUnit(name, members,
                          macro_prefix='MEMBER_NAMES_COMPLETE_',
                          skip=['NONE', 'MAX'],
                          strip_member_prefix=(name + '_'),
                          **enum_kws),
            EnumMacroUnit(name, members,
                          macro_prefix='MEMBER_NAMES_',
                          skip=(skip + ['NONE', 'MAX']),
                          strip_member_prefix=(name + '_'),
                          **enum_kws),
        ]
        kwargs.update(name=name, members=macros)
        super(EnumMacrosUnit, self).__init__(**kwargs)

    @property
    def macro_base(self):
        return self.members[0].macro_name


class HelperUnit(ClassUnit):

    non_child_attributes = ClassUnit.non_child_attributes + ['enum_unit']

    def __init__(self, *args, **kwargs):
        self.inverse = kwargs.pop('inverse', False)
        self.wrapped_type = kwargs.pop('wrapped_type', 'int')
        self.base_spec_param = kwargs.pop('spec_param', [])
        self.base_spec_var = kwargs.pop('spec_var', [])
        self.enum_unit = kwargs.pop('enum_unit', None)
        self.type_name = kwargs.pop('type_name', 'type')
        self.inv_spec_param = kwargs.pop('inv_spec_param', 'typename')
        self.inv_spec_var = kwargs.pop('inv_spec_var', 'EnumType')
        assert (self.base_spec_param and self.base_spec_var
                and self.enum_unit)
        kwargs.setdefault('class_type', 'struct')
        kwargs.setdefault('specialize_class', True)
        assert not kwargs.get('members', None)
        kwargs['members'] = []
        if self.inverse:
            kwargs['members'].append(
                TypedefUnit(self.type_name, type=self.inv_spec_var),
            )
        else:
            kwargs['members'].append(
                TypedefUnit(self.type_name, type=self.wrapped_type),
            )
        for t in self.base_spec_param:
            kwargs['members'].append(VariableUnit(
                t.lower(), type=t, value=f'{t}_NONE',
                static=True, constant=True,
                conditions={
                    'generate': [['inside_class']],
                    'define': [['declaration', 'inside_class']],
                }
            ))
        super(HelperUnit, self).__init__(*args, **kwargs)

    @property
    def spec_var(self):
        if self.inverse:
            return [self.inv_spec_var]
        else:
            return self.base_spec_var

    @property
    def spec_param(self):
        if self.inverse:
            return [self.inv_spec_param]
        else:
            return self.base_spec_param

    @spec_var.setter
    def spec_var(self, value):
        assert value is None

    @spec_param.setter
    def spec_param(self, value):
        assert value is None

    @property
    def wrapper_type(self):
        return TypeUnit(self.type_name, parent=self,
                        context=self.context)

    def update_member_context(self, **kwargs):
        super(HelperUnit, self).update_member_context(**kwargs)
        if self.specialized:
            if self.inverse:
                self.members[0].type = self.context.specialize[
                    self.inv_spec_var]
            elif self.enum_unit.enum_name_ext:
                self.members[0].type = self.enum_unit.enum_name_ext
            else:
                self.members[0].type = self.enum_unit.name
        else:
            if self.inverse:
                self.members[0].type = self.inv_spec_var
            else:
                self.members[0].type = 'int'
        for i, t in enumerate(self.base_spec_var):
            p = self.base_spec_param[i]
            if self.context.specialize and t in self.context.specialize:
                self.members[i + 1].value = self.context.specialize[t]
            else:
                self.members[i + 1].value = f'{p}_NONE'


class GuardUnit(CodeUnitBase):

    dont_add_template_lines = True

    def __init__(self, name, first=None, second=None,
                 guard_prefix=None, **kwargs):
        self.first = first
        self.second = second
        if guard_prefix is None and not name.lstrip().startswith('#'):
            guard_prefix = '#ifdef '
        self.guard_prefix = guard_prefix
        super(GuardUnit, self).__init__(name, **kwargs)
        for x in [self.first, self.second]:
            if x and isinstance(x, CodeUnitBase):
                x.set_parent(self)

    def empty(self):
        return not (self.first or self.second)

    def _generate(self):
        out = []
        out1 = []
        out2 = []
        if self.first:
            if isinstance(self.first, CodeUnitBase):
                out1 = self.first.generate()
            elif isinstance(self.first, list):
                out1 = self.first
            else:
                raise TypeError(type(self.first))
        if self.second:
            if isinstance(self.second, CodeUnitBase):
                out2 = self.second.generate()
            elif isinstance(self.second, list):
                out2 = self.second
            else:
                raise TypeError(type(self.second))
        if not (out1 or out2):
            return []
        if self.guard_prefix:
            start = f'{self.guard_prefix}{self.name}'
        else:
            start = self.name
        if (not out1) and 'ifdef' in start:
            out += [start.replace('ifdef', 'ifndef')]
        else:
            out += [start] + out1
        if out2:
            if out1:
                out += [f'#else  // {self.name}']
            out += out2
        out += [f'#endif  // {self.name}']
        return out


class BinaryOperatorUnit(CodeUnitBase):

    generate_str = True

    def __init__(self, name, members, **kwargs):
        self.members = members
        super(BinaryOperatorUnit, self).__init__(name, **kwargs)

    def _generate(self):
        if not self.members:
            return ''
        sep = f' {self.name} '
        return '(' + sep.join(self._eval(self.members)) + ')'


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
        return ['kwargs', 'existing', 'update_existing'] + [
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
                # for k in ['dont_generate'] + xcls.perfile_options_keys():
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
        if self.existing is None:
            if os.path.isfile(self.dst):
                with open(self.dst, 'r') as fd:
                    self.existing = fd.read().splitlines()
            else:
                self.existing = []
        return self.existing

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
                  markers=None, allow_empty=False, dont_buffer_end=False):
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
        if (not dont_buffer_end) and out[-1]:
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
        if not (append and self.existing_lines):
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

    def write(self, verbose=False, overwrite=False, dry_run=None,
              append=False):
        if dry_run is None:
            dry_run = self.dry_run
        self.lines = self.generate()
        if self.expect_empty:
            assert not self.lines
        if (not self.write_empty) and (not self.lines):
            if verbose:
                print(f"No lines to be written to {self.dst}")
            if os.path.isfile(self.dst) and overwrite:
                os.remove(self.dst)
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
        if self.update_existing:
            self.existing = contents.splitlines()
            return contents
        if dry_run:
            return contents
        if (not overwrite) and os.path.isfile(self.dst):
            raise AssertionError(f"{self.name} file {self.dst} "
                                 f"already exists")
        with open(self.dst, 'w') as fd:
            fd.write(contents)
        return contents

    def max_width(self, *args, **kwargs):
        kwargs.setdefault('current_keys', self._current_keys)
        return max_width(*args, **kwargs)

    def add_member(self, *args, **kwargs):
        add_enum = kwargs.pop('add_enum', False)
        if add_enum:
            self.add_enum(add_enum)
        assert self.current_key is not None
        kwargs.setdefault('current_keys', self._current_keys)
        return create_enum_member(*args, **kwargs)

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
    def include_file(cls, x, caller=None, rootdir=None,
                     no_buffer_line=False):
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
        out = [f"#include \"{header_incl}\""]
        if not no_buffer_line:
            out.append("")
        return out

    @classmethod
    def specialization(cls, lines, spec_param=[], specialize=[],
                       spec_var=None, inside_class=False):
        specialization = ''
        if spec_param:
            if specialize:
                if (not inside_class) and lines is not None:
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

    def add_include_files(self, no_buffer_line=False):
        lines = []
        if self.include_files:
            for x in self.include_files:
                lines += self.include_file(x, caller=self.dst,
                                           rootdir=self.root_include_dir,
                                           no_buffer_line=True)
            if not no_buffer_line:
                lines += ['']
        return lines

    def include_self(self, caller, rootdir=None, **kwargs):
        if rootdir is None:
            rootdir = self.root_include_dir
        return self.include_file(
            self.dst, caller=caller, rootdir=rootdir, **kwargs)

    def include_header(self, caller=None, **kwargs):
        if caller is None:
            caller = self.dst
        return self.include_file(
            self.get_child('header').dst, caller=caller,
            rootdir=self.root_include_dir, **kwargs)

    def begin_namespaces(self, namespaces=None):
        if namespaces is None:
            namespaces = self.namespaces
        if not namespaces:
            return []
        lines = []
        if self.file_extension in ['.hpp', '.h']:
            lines += [
                ('  ' * i) + f'namespace {x} {{'
                for i, x in enumerate(namespaces)
            ]
        else:
            lines += [
                f"using namespace {'::'.join(namespaces)};", ""
            ]
        return lines

    def end_namespaces(self, namespaces=None):
        if namespaces is None:
            namespaces = self.namespaces
        if not namespaces:
            return []
        lines = []
        if self.file_extension in ['.hpp', '.h']:
            lines += [
                ('  ' * (len(namespaces) - (i + 1))) + '}'
                for i in range(len(namespaces))
            ]
        return lines

    def add_namespaces(self, lines, namespaces=None):
        if namespaces is None:
            namespaces = self.namespaces
        if not (lines and namespaces):
            return lines
        if True:
            # if self.file_extension in ['.hpp', '.h']:
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

    def generate(self, indent='', no_namespaces=False, **kwargs):
        lines = []
        lines += self.add_include_files()
        new_lines = super().generate(indent=indent, **kwargs)
        if no_namespaces:
            lines += new_lines
        else:
            lines += self.add_namespaces(new_lines)
        if lines:
            lines += ['']
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
        if not self.is_parent:
            assert self.parent
            if hasattr(self.parent, 'generate_header'):
                return self.parent.generate_header(**kwargs)
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
        if not self.is_parent:
            assert self.parent
            if hasattr(self.parent, 'generate_source'):
                lines += self.parent.generate_source(
                    indent=indent, **kwargs)
                if lines:
                    includes = (
                        self.parent.include_self(
                            self.dst, no_buffer_line=True)
                        + self.add_include_files())
                    lines = includes + lines
                return lines
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
        'error_prefix',
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
    collection_functions = []
    # Functions that only need to be defined once per collection type
    universal_functions = [
        'error_prefix',
        'operator<<',
    ]
    # Methods that are only defined in specializations
    specialized_functions = [
        'error_prefix',
    ]
    _print_prefix = None
    _print_suffix = None
    _friend_value_types = ['std::string', 'double', 'int']

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
        return (self.collection_functions + self.universal_functions
                + self.friend_functions)

    def generate(self, *args, **kwargs):
        out = super(CEnumGeneratorCollectionBase, self).generate(
            *args, **kwargs)
        fglobal = self.parent.get_child('global')
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

    def collection_param_units(self, enum_name=None, **kwargs):
        return [self.parent.enum_unit(enum_name=enum_name, **kwargs).type]

    def collection_type_unit(self, *args, **kwargs):
        if self.collection_type is None:
            raise NotImplementedError
        constant = kwargs.pop('constant', None)
        if constant is None:
            constant = (not self.is_editable)
        specialize = kwargs.pop('specialize', None)
        if specialize is None:
            specialize = self.collection_param_units(*args, **kwargs)
        return TypeUnit(self.collection_type, specialize=specialize,
                        constant=constant)

    def variable_unit(self, enum_name, members, var_name=None,
                      parent=None, **kwargs):
        remainder_kwargs = {}
        enum_unit = self.parent.enum_unit(
            enum_name=enum_name, remainder_kwargs=remainder_kwargs,
            parent=parent, **kwargs)
        if var_name is None:
            var_name = self.collection_name
        remainder_kwargs.setdefault(
            'type', self.collection_type_unit(enum_unit=enum_unit))
        remainder_kwargs.setdefault(
            'value', self.value_unit(enum_name, members,
                                     parent=enum_unit))
        remainder_kwargs.setdefault(
            'docs', self.generate_docs(var_name))
        return self.parent.variable_unit(
            var_name=var_name, enum_unit=enum_unit, parent=parent,
            **remainder_kwargs
        )

    def member_units(self, x, **kwargs):
        raise NotImplementedError

    def value_unit(self, name, members, **kwargs):
        member_units = []
        for x in members:
            if x['abbr'] not in ['NONE', 'MAX']:
                member_units += self.member_units(x, **kwargs)
        return ConditionalValueUnit({(('specialize', ), ): member_units,
                                     (('NOT:specialize', ), ): []},
                                    ragged_right=self.ragged_right)

    def utility_type_units(self, *args, **kwargs):
        return []

    def generate_header_item(self, name, members):
        lines = []
        if not self.parent.as_class:
            lines += super(
                CEnumGeneratorCollectionBase, self).generate_header_item(
                    name, members)
        return lines

    def generate_item(self, name, members):
        lines = []
        if not self.parent.as_class:
            lines += super(
                CEnumGeneratorCollectionBase, self).generate_item(
                    name, members)
        return lines

    def generate_iteration(self, collection_name, iter_name):
        return (
            f"for ({iter_name} = {collection_name}.begin(); "
            f"{iter_name} != {collection_name}.end(); {iter_name}++)"
        )

    def generate_clear(self, collection_name, collection_type, **kwargs):
        return [f"{collection_name}.clear();"]

    def generate_iterator_dref(self, iter_name):
        raise NotImplementedError

    def generate_add(self, collection_name, collection_type, var_name,
                     *args, **kwargs):
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

    def generate_count(self, collection_name, collection_type, var_name,
                       **kwargs):
        fstring = ['{var_name} = {collection_name}.size();']
        fstring_param = {
            'collection_name': collection_name,
            'var_name': var_name,
        }
        return self._generate_unit(fstring, fstring_param)

    def _generate_unit(self, fstring, fstring_param, indent=''):
        if indent:
            if isinstance(fstring, list):
                fstring = [indent + x for x in fstring]
            else:
                fstring = indent + fstring
        if any(isinstance(v, CodeUnitBase) for v in fstring_param.values()):
            if isinstance(fstring, list):
                return [ValueUnit(x, fstring_param=fstring_param)
                        for x in fstring]
            else:
                return ValueUnit(fstring, fstring_param=fstring_param)
        if isinstance(fstring, list):
            return [x.format(**fstring_param) for x in fstring]
        else:
            return fstring.format(**fstring_param)

    def function_units(self, function_type=None, enum_unit=None,
                       added_functions=None, **kwargs):
        if function_type is None:
            out = []
            for function_type in self.additional_functions:
                out += self.function_units(
                    function_type, enum_unit=enum_unit,
                    added_functions=added_functions, **kwargs)
            return out
        if added_functions is not None:
            added_functions.setdefault(self.collection_type, [])
            if function_type in added_functions[self.collection_type]:
                return []
        if function_type == 'from' and (
                self.value_type == 'double'
                or self.value_key == 'ALIASES'):
            return []
        enum_name = kwargs.pop('enum_name', 'Type')
        enum_is_class = kwargs.pop('enum_is_class', self.parent.as_class)
        result = kwargs.pop('result', self.collection_name)
        utility = kwargs.pop(
            'utility', (function_type in self.utility_functions
                        or function_type in self.friend_functions))
        value_type = kwargs.pop('value_type', None)
        conditions = kwargs.pop('conditions', None)
        friend = kwargs.pop(
            'friend', (function_type in self.friend_functions))
        function_param = kwargs.pop('function_param', {})
        if enum_unit is not None:
            if enum_unit.parent_class:
                enum_is_class = enum_unit.parent_class.name
            enum_name = enum_unit.name
        assert enum_name is not None
        param_units = self.collection_param_units(
            enum_name, value_type=value_type, parent=enum_unit,
            as_class=enum_is_class)
        collection_type = self.collection_type_unit(
            enum_name, value_type=value_type, parent=enum_unit,
            as_class=enum_is_class, constant=False)
        if conditions is None:
            if friend:
                conditions = {
                    'generate': [
                        ['specialize'],
                    ],
                    'define': [
                        ['specialize', 'in_source']
                    ],
                }
            elif self.parent.get_child('global').define_in_source:
                conditions = {
                    'generate': [
                        ['primary', 'inside_class'],
                        ['NOT:specialize', 'in_source'],
                    ],
                    'define': [
                        ['NOT:specialize', 'in_source'],
                    ],
                }
            elif utility:
                conditions = {
                    'generate': [
                        ['primary', 'inside_class'],
                        ['specialize'],
                    ],
                    'define': [
                        ['specialize', 'in_source']
                    ],
                }
            else:
                conditions = {
                    'generate': [
                        ['primary', 'inside_class']
                    ],
                }
        if enum_unit.parent_class:
            if friend:
                kwargs.update(
                    no_parent=True,
                )
            else:
                kwargs.update(
                    parent=enum_unit.parent_class,
                    static=True,
                )
        ktype = param_units[0]
        vtype = None if len(param_units) == 1 else param_units[1]
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
            return_type = TypeUnit('std::vector', specialize=[ktype])
        elif function_type in ['count']:
            return_type = 'std::size_t'
        else:
            return_type = 'void'
        args = []
        if utility and function_type not in (self.universal_functions
                                             + ['operator<<']):
            args.append(
                ArgUnit('collection', type=collection_type,
                        constant=True, reference=True)
            )
        if function_type in ['add', 'remove', 'is', 'get', 'getdefault',
                             'check', 'checkNot']:
            args.append(
                ArgUnit('x', type=ktype, constant=True, reference=True)
            )
            if ((function_type == 'add' and hasattr(self, 'value_type')
                 and not getattr(self, 'current_flag', None))):
                args.append(
                    ArgUnit('y', type=vtype,
                            constant=True, reference=True)
                )
            elif function_type in ['check', 'checkNot']:
                args.append(
                    ArgUnit('context', type='std::string',
                            constant=True, reference=True,
                            default='""')
                )
            elif function_type == 'getdefault':
                args.append(
                    ArgUnit(
                        'defaultV', type=vtype,
                        constant=True, reference=True,
                    )
                )
        elif function_type in ['addMultiple', 'removeMultiple']:
            args.append(
                ArgUnit('x', type=collection_type,
                        constant=True, reference=True)
            )
        elif function_type == 'print':
            args += [
                ArgUnit('out', type='std::ostream', reference=True),
                ArgUnit('includePrefixes', type='bool', default='false'),
                ArgUnit('tab', type='unsigned int', default='0',
                        constant=True),
            ]
        elif function_type == 'operator<<':
            args.append(
                ArgUnit('out', type='std::ostream', reference=True)
            )
            if function_param.get('for_class', False):
                argT = collection_type
            else:
                argT = enum_unit.enum_name_ext
            args.append(
                ArgUnit('x', type=argT, constant=True, reference=True)
            )
        elif function_type == 'string':
            args.append(
                ArgUnit('tab', type='unsigned int', constant=True,
                        default='0')
            )
        elif function_type == 'from':
            args.append(
                ArgUnit('x', type=vtype, constant=True, reference=True)
            )
        arg_names = [x.name for x in args]
        docs = []
        body = []
        if not enum_is_class:
            body += [
                ValueUnit(
                    "{const}{collection_type} {result} = "
                    "get{function_suffix};",
                    fstring_param={
                        'collection_type': collection_type,
                        'result': result,
                        'function_suffix': function_suffix,
                        'const': ('' if self.is_editable else 'const '),
                    }
                )
            ]
        if function_type not in self.additional_functions:
            raise NotImplementedError(
                f"Invalid function_type \'{function_type}\' "
                f"(valid values = {self.additional_functions})")
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
            body += [
                ValueUnit(
                    "typename {collection_type}::const_iterator it;",
                    fstring_param={'collection_type': collection_type},
                )
            ]
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
            body += self.generate_clear(result, collection_type)
        elif function_type in ['get', 'getdefault']:
            assert hasattr(self, 'value_type')
            body += [
                ValueUnit(
                    "typename {collection_type}::const_iterator it;",
                    fstring_param={'collection_type': collection_type},
                )
            ]
            body += self.generate_find(result, "x", "it")
            dref = self.generate_iterator_dref("it")[1]
            if self.reversed:
                name_err = "x"
            else:
                name_err = "names.find(x)->second"
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
                    f'{name_err} + "\'");',
                    '}'
                ]
            docs += [
                '\\return Value'
            ]
            body += [f"return {dref};"]
        elif function_type == 'from':
            assert hasattr(self, 'value_type')
            body += [
                ValueUnit(
                    "typename {collection_type}::const_iterator it;",
                    fstring_param={'collection_type': collection_type},
                )
            ]
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
                'out += utils::get_enum_names<PARAM_TYPE>().'
                'find(param_type)->second;',
                'out += "[";',
                'out += utils::get_enum_names<MODULE>().find(module)->second;',
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
                    "utils::print_string(x, out, 4);",
                    "return out;",
                ]
            else:
                docs += [
                    '\\param[in] x Key to serialize',
                ]
                body += [
                    ValueUnit(
                        'out << {enum_unit.parent_address}getName(x);',
                        fstring_param={'enum_unit': enum_unit}
                    ),
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
                ValueUnit(
                    "typename {collection_type}::const_iterator it;",
                    fstring_param={'collection_type': collection_type},
                ),
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
                ValueUnit(
                    lambda x:
                    f"{x.parent_address}"
                    f"print{function_suffix}(collection, oss, tab);"
                ),
                "return oss.str();",
            ]
        elif function_type == 'remove':
            assert self.is_editable
            docs += [
                f'Remove an element from {self.collection_name}',
                '\\param[in] x Key to remove',
            ]
            body += [
                ValueUnit(
                    "typename {collection_type}::iterator it;",
                    fstring_param={'collection_type': collection_type},
                )
            ]
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
                body += self.generate_add(result, collection_type,
                                          *arg_names)
            else:
                body += [f"if (!is{function_suffix}(x)) {{"]
                body += self.generate_add(result, collection_type,
                                          *arg_names, indent='  ')
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
            body += [
                ValueUnit(
                    '{return_type} out;',
                    fstring_param={'return_type': return_type}
                )
            ]
            body += self.generate_count(result, collection_type, 'out')
            body += ['return out;']
        elif function_type == 'list':
            docs += [
                f'Get the set of elements in {self.collection_name} '
                f'with {self.current_flag} set.',
                '\\return Set of elements.',
            ]
            body += [
                ValueUnit(
                    '{return_type} out;',
                    fstring_param={'return_type': return_type},
                )
            ]
            body += self.generate_list(result, collection_type, 'out')
            body += ['return out;']
        elif function_type.endswith('Multiple'):
            assert self.is_editable
            short = function_type.split('Multiple')[0]
            ftype = short + function_suffix
            dref = self.generate_iterator_dref('it')
            if function_type == 'removeMultiple':
                dref = [dref[0]]
            itdref = ", ".join(dref)
            docs += [
                f'{short.title()} multiple elements to '
                f'{self.collection_name} if they are not already present',
                '\\param[in] x Elements to add',
            ]
            body += [
                ValueUnit(
                    "typename {collection_type}::const_iterator it;",
                    fstring_param={'collection_type': collection_type},
                ),
                f"{self.generate_iteration('x', 'it')} {{",
                f"  {ftype}({itdref});",
                "}"
            ]
        else:
            raise NotImplementedError(
                f"Unimplemented function_type \'{function_type}\' "
                f"(valid values = {self.additional_functions})")
        if utility and added_functions is not None:
            added_functions[self.collection_type].append(function_type)
        return [
            FunctionUnit(
                function_name, type=return_type, args=args, body=body,
                docs=docs, constant=False,
                conditions=conditions, **kwargs
            )
        ]

    @classmethod
    def make_title(cls, x):
        return ''.join([xx.title() for xx in x.split('_')])

    def generate_utility_types(self, **kwargs):
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
        ragged_right={
            'action': 'store_true',
            'help': "Generate the map with a ragged right edge.",
        },
    )
    default_value_type = None
    default_reversed = False
    default_explicit = False
    default_ragged_right = False
    collection_type = 'std::map'
    _additional_functions = (
        CEnumGeneratorCollectionBase._additional_functions + [
            'get', 'getdefault', 'from'
        ]
    )
    _print_prefix = None  # "space << \"{\" << std::endl;"
    _print_suffix = None  # "space << \"}\""

    def collection_param_units(self, *args, **kwargs):
        value_type = kwargs.pop('value_type', None)
        if value_type is None:
            value_type = self.value_type
        if not isinstance(value_type, CodeUnitBase):
            value_type = ValueUnit(value_type)
        out = super(CEnumGeneratorMapBase, self).collection_param_units(
            *args, **kwargs)
        if self.reversed:
            out = [value_type] + out
        else:
            out.append(value_type)
        return out

    def generate_docs(self, name):
        name_doc = self.collection_name.title()
        return f'{name_doc} for values'

    def member_units(self, x, value_unit=None, **kwargs):
        if value_unit is None:
            value_unit = self.generate_value(x)
        if not value_unit:
            return []
        if isinstance(value_unit, list):
            out = []
            for v in value_unit:
                out += self.member_units(x, value_unit=v, **kwargs)
            return out
        key_unit = ValueUnit(x['name'], **kwargs)
        if self.reversed:
            return [[value_unit, key_unit]]
        else:
            return [[key_unit, value_unit]]

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

    def generate_add(self, collection_name, collection_type,
                     key_name, val_name, **kwargs):
        indent = kwargs.get('indent', '')
        if self.reversed:
            return [f"{indent}{collection_name}.emplace({val_name}, "
                    f"{key_name});"]
        else:
            return [f"{indent}{collection_name}.emplace({key_name}, "
                    f"{val_name});"]

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
    default_ragged_right = True
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

    def function_units(self, function_type=None, enum_unit=None,
                       **kwargs):
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
                ikw['enum_unit'] = enum_unit
                if function_type in ['string']:
                    dummy_vect = self.get_dummy_vector_collection(k)
                    ikw['result'] = f'list{ktitle}()'
                    out += dummy_vect.function_units(
                        function_type, **ikw)
                else:
                    out += self.function_units(
                        function_type, **ikw)
                self.pop_flag()
            if function_type in methods_not_general:
                return out
        out += super(
            CEnumGeneratorFlagMapBase, self).function_units(
                function_type, enum_unit=enum_unit, **kwargs)
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

    def generate_add(self, result, collection_type, var_name,
                     *args, **kwargs):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_add(
                result, collection_type, var_name, *args, **kwargs)
        iter_name = kwargs.get('iter_name', 'it')
        indent = kwargs.get('indent', '')
        fstring_param = {
            'collection_type': collection_type,
            'iter_name': iter_name,
            'result': result,
            'var_name': var_name,
            'current_flag': self.current_flag,
        }
        out = [
            "typename {collection_type}::iterator {iter_name};",
        ]
        out += self.generate_find(result, var_name, iter_name)
        out += ['if ({iter_name} == {result}.end()) {{']
        out += super(CEnumGeneratorFlagMapBase, self).generate_add(
            result, collection_type, var_name,
            self.prefix_flags('NONE'), indent='  ')
        out += [
            '  {iter_name} = {result}.find({var_name});',
            '}}',
            '{iter_name}->second |= {current_flag};',
        ]
        return self._generate_unit(out, fstring_param, indent=indent)

    def generate_clear(self, collection_name, collection_type,
                       iter_name="it"):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_clear(
                collection_name, collection_type, iter_name=iter_name)
        dref = self.generate_iterator_dref(iter_name)[1]
        fstring_param = {
            'collection_type': collection_type,
            'iter_name': iter_name,
        }
        out = [
            "typename {collection_type}::iterator {iter_name};",
            self.generate_iteration(collection_name, iter_name) + "{{",
            f"  {dref} &= ~{self.current_flag};",
            "}}",
        ]
        return self._generate_unit(out, fstring_param)

    def generate_count(self, collection_name, collection_type, var_name,
                       iter_name="it"):
        if self.current_flag is None:
            return super(CEnumGeneratorFlagMapBase, self).generate_count(
                collection_name, collection_type, var_name,
                iter_name=iter_name)
        dref = self.generate_iterator_dref(iter_name)[1]
        fstring_param = {
            'var_name': var_name,
            'collection_type': collection_type,
            'iter_name': iter_name,
            'dref': dref,
            'current_flag': self.current_flag,
            'iteration': self.generate_iteration(
                collection_name, iter_name),
        }
        out = [
            f"{var_name} = 0;",
            "typename {collection_type}::const_iterator {iter_name};",
            "{iteration} {{",
            f"  if ({dref} & {self.current_flag}) {var_name}++;",
            "}}",
        ]
        return self._generate_unit(out, fstring_param)

    def generate_list(self, collection_name, collection_type, var_name,
                      iter_name="it"):
        assert self.current_flag is not None
        dref = self.generate_iterator_dref(iter_name)
        fstring_param = {
            'var_name': var_name,
            'collection_type': collection_type,
            'iter_name': iter_name,
            'dref': dref,
            'current_flag': self.current_flag,
            'iteration': self.generate_iteration(
                collection_name, iter_name),
        }
        out = [
            "typename {collection_type}::const_iterator {iter_name};",
            "{iteration}{{",
            "  if ({dref[1]} & {current_flag}) {{",
            "    {var_name}.push_back({dref[0]});",
            "  }}",
            "}}",
        ]
        return self._generate_unit(out, fstring_param)

    def generate_utility_types(self, **kwargs):
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
            self.flag_prefix, members, as_class=False, **kwargs)
        return out

    def member_units(self, x, **kwargs):
        value_unit = self.generate_value(x, return_list=True)
        if not value_unit:
            return []
        value_unit = BinaryOperatorUnit(
            '|', [ValueUnit(x) for x in value_unit])
        return super(CEnumGeneratorFlagMapBase, self).member_units(
            x, value_unit=value_unit, **kwargs)

    def generate_value(self, x, return_list=False):
        if self.value_key is None or not isinstance(self.value_key, list):
            raise NotImplementedError
        out = [k for k in self.value_key if x.get(k, '')]
        if self.explicit and (not out):
            return None
        if not out:
            out.append('NONE')
        out = self.prefix_flags(out)
        if return_list:
            return out
        out = ' | '.join(out)
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

    def member_units(self, x, **kwargs):
        if not self.generate_value(x):
            return []
        return [ValueUnit(x['name'], **kwargs)]

    def generate_member(self, x, width=None, enum_prefix=''):
        if not self.generate_value(x):
            return []
        assert width
        return [f"  {(enum_prefix + x['name']):{width}},"]

    def generate_iterator_dref(self, iter_name):
        return [f"(*({iter_name}))"]

    def generate_add(self, collection_name, collection_type, var_name,
                     *args, **kwargs):
        indent = kwargs.get('indent', '')
        return [f'{indent}{collection_name}.push_back({var_name});']

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


class CEnumGeneratorGlobalHeader(CEnumGeneratorBaseHeader):

    name = 'global'
    file_suffix = ''
    prefix = CEnumGeneratorBase.prefix + [
        '#include <string>',
        '#include <map>',
        '#include <vector>',
        '#include <iostream>',
        '#include <fstream>',
        '#include <sstream>',
        '#include <stdexcept>',
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
    define_in_source = True

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
                existing = self.parent.get_child('helper').existing_lines
            else:
                existing = []
        existing_enum = []
        existing_accum_enum = []
        for x in self.extract_chunk('global_enum', existing):
            if x.strip().startswith(f'{self.enum_name}_'):
                new_val = x.split(
                    f'{self.enum_name}_')[-1].split(',')[0].strip()
                if new_val not in ['NONE', 'MAX'] + existing_enum:
                    existing_enum.append(new_val)
            elif (self.accum_enum_name
                  and x.strip().startswith(f'{self.accum_enum_name}_')):
                new_val = x.split(
                    f'{self.accum_enum_name}_')[-1].split(',')[0].strip()
                if new_val not in ['NONE', 'MAX'] + existing_accum_enum:
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

    @property
    def specializations(self):
        spec_var = self.spec_var
        for x in self.enum_members:
            if x['abbr'] in ['NONE', 'MAX']:
                continue
            if self.accum_enum_name:
                for y in self.accum_enum_members:
                    if y['abbr'] in ['NONE', 'MAX']:
                        continue
                    yield {spec_var[0]: x['name'],
                           spec_var[1]: y['name']}
            else:
                yield {spec_var[0]: x['name']}

    def generate_global_enum(self, lines=None):
        if lines is None:
            lines = []
        # Modules
        lines += self.parent.generate_enum(
            self.enum_name, self.enum_members,
            as_class=False, macro_skip=self.enum_macro_skip,
            namespaces=False,
        )
        # Param types
        if self.accum_enum_name:
            lines += self.parent.generate_enum(
                self.accum_enum_name, self.accum_enum_members,
                as_class=False, macro_skip=self.accum_enum_macro_skip,
                namespaces=False,
            )
        return lines

    def includes(self):
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
            if not (v and k == 'global'):
                continue
            include_lines += ['// Global includes']
            include_lines += self.add_chunk(
                f'headers_{k}', [x for x in v if x])
        return include_lines

    def generate_source(self, namespaces=None, **kwargs):
        if not self.define_in_source:
            return []
        lines = self.generate(in_source=True, namespaces=False, **kwargs)
        if self.parent.as_class:
            enum_class = self.parent.enum_unit().parent
            for spec in self.specializations:
                enum_class.set_context(specialize=spec,
                                       in_source=True,
                                       instantiation=True,
                                       filetype='global')
                lines += enum_class.instantiate()
        lines = self.add_namespaces(lines, namespaces=namespaces)
        return lines

    def generate(self, indent='', namespaces=None, **kwargs):
        lines = []
        kwargs.update(
            spec_param=self.spec_param, spec_var=self.spec_var,
            specialize=False, namespaces=False,
        )
        # Base class
        if self.parent.as_class:
            docs = 'Unspecialized enum'
            lines += self.parent.generate_enum(
                self.parent.as_class, [], **kwargs,
            )
        else:
            docs = 'Empty enum'
            lines += self.parent.generate_enum(
                self.empty_enum_name, [], **kwargs,
            )
        if lines:
            lines = [f'// {docs}'] + lines
        lines = self.add_namespaces(lines, namespaces=namespaces)
        if not kwargs.get('in_source', False):
            lines += self.includes()
            lines = self.parent.include_helper_header() + lines
        return lines


class CEnumGeneratorHelper(CEnumGeneratorBaseHeader):

    name = 'helper'
    file_suffix = ''
    explicit_dst = True

    def extract_existing(self, append=None, **kwargs):
        out = super(CEnumGeneratorHelper, self).extract_existing(
            append=append, **kwargs)
        if append:
            new_chunk = self.parent.get_child(
                'global').generate_global_enum()
            out = self.replace_chunk('global_enum', out, new_chunk)
        return out

    def add_include_files(self, *args, **kwargs):
        if not self.append:
            return super(CEnumGeneratorHelper, self).add_include_files(
                *args, **kwargs)
        return []

    def generate(self, *args, **kwargs):
        lines = []
        fglobal = self.parent.get_child('global')
        if self.parent.as_class and not self.append:
            lines += self.add_chunk(
                'global_enum',
                fglobal.generate_global_enum(),
                allow_empty=True
            )
            lines += ['// Unspecialized enum helper']
            lines += self.parent.generate_from_unit(
                self.parent.as_class, [], specialize=False,
                filetype='helper', namespaces=False)
            lines = fglobal.add_namespaces(lines)
            assert lines
        if not self.append:
            for k in self.parent.added_collections:
                lines += self.parent.get_child(k).generate_utility_types()
        kwargs['no_namespaces'] = True
        lines += super(CEnumGeneratorHelper, self).generate(*args, **kwargs)
        return lines

    def generate_item(self, name, members, as_class=None, **kwargs):
        lines = []
        if as_class is None:
            as_class = self.parent.as_class
        if not as_class:
            return lines
        lines += self.parent.generate_from_unit(
            name, members, as_class=as_class, filetype='helper',
            **kwargs)
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
        api_macro={
            'type': str,
            'help': "Macro that should be used to export/import members",
            'default': '',
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
                'default_explicit': True,
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
                '_utility_functions': [
                    x for x in CEnumGeneratorMapBase._utility_functions
                    if x not in ['print', 'string']
                ],
            },
            'docs': {
                'default_value_type': 'std::string',
                'default_value_key': 'doc',
                'default_explicit': True,
                'default_ragged_right': True,
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
    _export_members = False

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
        if out.get('global', {}).get('namespaces', None):
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

    def generate_member(self, *args, **kwargs):
        if self.explicit_values:
            kwargs['explicit_values'] = True
        return [generate_enum_member(*args, **kwargs)]

    def include_helper_header(self):
        return self.include_file(
            self.get_child('helper').dst, caller=self.dst,
            rootdir=self.root_include_dir
        )

    def helper_unit(self, helper_name='enum_helper', inverse=False,
                    conditions=None, include_inv=False, **kwargs):
        remainder_kwargs = {}
        enum_unit = self.enum_unit(remainder_kwargs=remainder_kwargs,
                                   **kwargs)
        if not enum_unit.parent_class:
            return None
        if inverse:
            include_inv = False
        if enum_unit.parent_class:
            remainder_kwargs.update(
                spec_param=enum_unit.parent_class.spec_param,
                spec_var=enum_unit.parent_class.spec_var,
            )
        if conditions is None:
            if inverse:
                conditions = {
                    'generate': [
                        ['declaration', 'NOT:specialize',
                         'EQ:filetype:helper'],
                        ['declaration', 'specialize',
                         'NOT:EQ:filetype:helper'],
                    ]
                }
            else:
                conditions = {
                    'generate': [
                        ['declaration', 'NOT:specialize',
                         'EQ:filetype:helper'],
                        ['declaration', 'specialize',
                         'EQ:filetype:helper'],
                    ]
                }
        out = HelperUnit(
            f'{helper_name}_inv' if inverse else helper_name,
            enum_unit=enum_unit,
            conditions=conditions,
            inverse=inverse,
            **remainder_kwargs
        )
        if include_inv:
            out = ContainerUnit(members=[out])
            out.add_member(
                self.helper_unit(helper_name=helper_name, inverse=True,
                                 enum_unit=enum_unit,
                                 **remainder_kwargs)
            )
        return out

    def variable_unit(self, var_name=None, value=None, type=None,
                      static=True, conditions=None,
                      parent=None, **kwargs):
        remainder_kwargs = {}
        enum_unit = self.enum_unit(
            remainder_kwargs=remainder_kwargs, parent=parent, **kwargs)
        if not parent:
            parent = enum_unit.parent_class
        if conditions is None:
            conditions = {
                'generate': [
                    ['in_header'],
                    ['specialize', 'in_source'],
                ],
                'define': [
                    ['NOT:specialize', 'outside_class', 'in_header'],
                    ['specialize', 'in_source'],
                ]
            }
        assert (var_name and value and type)
        if self._export_members:
            remainder_kwargs.setdefault('export', self.api_macro)
        out = VariableUnit(
            var_name, type=type, value=value, static=static,
            conditions=conditions,
            parent=parent, **remainder_kwargs
        )
        return out

    def enum_unit(self, name=None, members=None,
                  enum_unit=None, enum_name=None, enum_name_ext=None,
                  enum_type='int', as_class=None,
                  spec_param=None, spec_var=None, members_macro=None,
                  parent=None, force_container=False,
                  return_type=None, remainder_kwargs=None, **kwargs):
        if remainder_kwargs is None:
            if kwargs:
                raise RuntimeError(f'ENUM_UNIT UNUSED KWARGS: '
                                   f'{pprint.pformat(kwargs)}')
        else:
            remainder_kwargs.update(**kwargs)
        if enum_unit is not None:
            if return_type:
                return enum_unit.type
            return enum_unit
        if as_class is None:
            as_class = self.as_class
        if name is None:
            assert as_class
            name = as_class
        if enum_name is None:
            if as_class:
                enum_name = 'Type'
            else:
                enum_name = name
        if enum_name_ext is None:
            enum_name_ext = (f'ENUM_{name}' if as_class else None)
        if return_type and isinstance(parent, EnumUnit):
            return parent.type
        if members is not None:
            self.complete_members(members)
        enum_unit = EnumUnit(
            enum_name, members,
            enum_type=enum_type,
            members_macro=members_macro,
            enum_name_ext=enum_name_ext,
            conditions={
                'generate': (
                    [['specialize', 'in_header'], ['declaration']]
                    if as_class else []
                )
            },
        )
        parent_created = False
        if parent is None:
            parent_created = True
            if as_class:
                if spec_param is None:
                    spec_param = self.get_child('global').spec_param
                if spec_var is None:
                    spec_var = self.get_child('global').spec_var
                parent = ClassUnit(
                    as_class, spec_param=spec_param, spec_var=spec_var,
                    conditions={
                        'generate': [
                            ['NOT:EQ:filetype:helper'],
                            # ['specialize', 'in_source', 'instantiation',
                            #  'EQ:filetype:global'],
                        ],
                        # 'instantiate': [
                        # ],
                    })
            elif force_container:
                parent = ContainerUnit()
        if parent is not None:
            temporary = (parent_created and not force_container)
            enum_unit.set_parent(parent, temporary=temporary)
        if as_class:
            assert enum_unit.parent_class
        if return_type:
            return enum_unit.type
        return enum_unit

    def generate_from_unit(self, name, members, mark=False,
                           as_class=None, spec_param=None,
                           spec_var=None, specialize=None,
                           namespaces=None, **kwargs):
        context_kws = {
            k: kwargs.pop(k) for k in CodeContext.parameter_keys
            if k in kwargs
        }
        if as_class is None:
            as_class = self.as_class
        if as_class:
            if spec_param is None:
                spec_param = self.get_child('global').spec_param
            if spec_var is None:
                spec_var = self.get_child('global').spec_var
            if as_class != name and specialize is None:
                specialize = self.get_child('global').item2specialize(name)
        if namespaces is None:
            namespaces = self.get_child('global').namespaces
        if specialize:
            assert isinstance(specialize, list)
            assert len(specialize) == len(spec_var)
            specialize = {
                k: v for k, v in zip(spec_var, specialize)
            }
            assert 'EnumType' not in specialize
            specialize['EnumType'] = f'ENUM_{name}'
        unit = self.generate_code_unit(name, members, as_class=as_class,
                                       spec_param=spec_param,
                                       spec_var=spec_var, **kwargs)
        if namespaces:
            unit.namespaces = namespaces
        unit.set_context(specialize=specialize, **context_kws)
        out = []
        if mark:
            out += ['// BEFORE GENERATED']
        out += unit.generate(top=True)
        if mark:
            out += ['// AFTER GENERATED']
        return out

    def generate_code_unit(self, name, members, macro_skip=[], **kwargs):
        enum_unit = self.enum_unit(
            name, members, force_container=True, **kwargs)
        out = enum_unit.parent
        as_class = enum_unit.parent_class
        helper = self.helper_unit(enum_unit=enum_unit)
        helper_inv = self.helper_unit(enum_unit=enum_unit, inverse=True)
        if as_class:
            out.add_member(
                GuardUnit(
                    'EPHOTO_USE_SCOPED_ENUM',
                    first=TypedefUnit(
                        name='Type',
                        type=helper.wrapper_type,
                    ),
                    second=enum_unit,
                )
            )
            assert enum_unit.parent_class
        else:
            out.add_member(enum_unit)
        members_core = [
            x for x in members if x['abbr'] not in ['NONE', 'MAX']]
        if as_class and as_class.spec_param:
            for p, t in zip(as_class.spec_param, as_class.spec_var):
                out.add_member(
                    self.variable_unit(
                        var_name=p.lower(),
                        type=f'const {p}',
                        value=t,
                        conditions={
                            'generate': [
                                ['NOT:specialize', 'outside_class',
                                 'in_header'],
                                ['NOT:specialize', 'inside_class',
                                 'in_header'],
                            ],
                            'define': [
                                ['NOT:specialize', 'outside_class',
                                 'in_header'],
                            ]
                        },
                    )
                )
        out.add_member(
            self.variable_unit(
                var_name=('all' if as_class else f'ALL_{name}'),
                type=TypeUnit('std::vector',
                              specialize=[enum_unit.type],
                              constant=True),
                docs='All enum values',
                value=ValueUnit([
                    ValueUnit(x['name'], parent=enum_unit)
                    for x in members_core
                ]),
                enum_unit=enum_unit,
            ),
        )
        if as_class:
            for k in self.added_collections:
                kchild = self.get_child(k)
                out.add_member(
                    kchild.variable_unit(
                        name, members_core, enum_unit=enum_unit,
                        parent=out,
                    )
                )
            added = {}
            for k in self.added_collections:
                out.add_members(
                    self.get_child(k).function_units(
                        enum_unit=enum_unit,
                        added_functions=added,
                    )
                )
            out = ContainerUnit([out])
        out.add_member(
            EnumMacrosUnit(
                name, members,
                macro_suffix=(self.macro_suffix if as_class else ''),
                skip=macro_skip,
                conditions={
                    'generate': (
                        [['specialize', 'EQ:filetype:helper',
                          'in_header']]
                        if as_class else []
                    )
                },
            ),
            index=0,
        )
        enum_unit.members_macro = out.members[0]
        if as_class:
            def _get_context(i):
                def _get_context_wrapped(self):
                    return self.context.specialize[
                        enum_unit.parent_class.spec_var[i]].rsplit(
                            enum_unit.parent_class.spec_param[i], 1
                        )[-1].strip('_')
                return _get_context_wrapped

            out.add_members([
                ContainerUnit([
                    enum_unit.external_enum(
                        conditions={
                            'generate': [
                                ['declaration', 'EQ:filetype:helper',
                                 'specialize']
                            ]
                        }
                    ),
                    helper
                ], guards=GuardUnit('EPHOTO_USE_SCOPED_ENUM')),
                helper_inv,
                TypedefUnit(
                    ValueUnit(_get_context(0)),
                    conditions=['outside_class', 'in_header',
                                'NOT:EQ:filetype:helper',
                                'specialize'],
                    type=ValueUnit(
                        '{enum_unit.parent_class.type}',
                        fstring_param={'enum_unit': enum_unit}),
                    namespaces=[
                        ValueUnit(_get_context(i)) for i in
                        range(1, len(enum_unit.parent_class.spec_var))
                    ]
                )
            ])
        return out

    def complete_members(self, members):
        first = "NONE"
        last = "MAX"
        skip_items = [first, last]
        if (not members) or members[0]['abbr'] != first:
            members.insert(0, {'name': first, 'abbr': first})
        if (not members) or members[-1]['abbr'] != last:
            members.append({'name': last, 'abbr': last})
        return skip_items

    def generate_source_item(self, name, members, **kwargs):
        return self.generate_from_unit(name, members, definition=True,
                                       in_source=True, namespaces=False,
                                       **kwargs)

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
        lines += self.generate_from_unit(
            name, members, as_class=as_class, **copy.deepcopy(kwargs))
        if lines:
            lines += ['']
        return lines

    def generate(self, *args, **kwargs):
        lines = super(CEnumGeneratorHeader, self).generate(*args, **kwargs)
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


def generate(args, **kws):
    if args.rename_source:
        src_cls = get_registered_class('parser', args.src_type)
        return rename_source(args.rename_source, args.src, args.dst,
                             src_cls.file_extension)
    elif args.ePhotosynthesis:
        return ePhotosynthesis(args)
    src = get_registered_class('parser', args.src_type)(
        args.src, verbose=args.verbose, is_regex=args.src_regex)
    dstcls = get_registered_class('generator', args.dst_type)
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
    return dst


def ePhotosynthesis(args):
    srcdir = args.src
    dstdir_inc = os.path.dirname(args.dst_global)
    dstdir_src = os.path.dirname(args.dst_global_source)
    args.ePhotosynthesis = False
    args.src_regex = True
    args.dst_type = 'c'
    args.namespaces_c_source = ['ePhotosynthesis']
    args.namespaces_global = ['ePhotosynthesis']
    args.enum_name_global = 'MODULE'
    args.enum_macro_skip_global = ['ALL']
    args.accum_enum_macro_skip_global = ['VARS']
    args.as_class_c = 'ValueSetEnum'
    args.dst_helper = os.path.join(dstdir_inc, 'enums_helpers.hpp')
    first = True
    macro_suffix = {
        'COND': 'Condition',
        'MOD': '',
        'POOL': 'Pool',
        'VEL': 'Vel'
    }
    added_classes = {
        'names': {},
        'values': {
            'collection_name': 'defaults',
        },
        'alternate_values': {
            'collection_name': 'defaults_C3',
            'singular_collection_name': 'default_C3',
        },
        'glymaids': {
            'explicit': True,
        },
        'aliases': {
            'reversed': True,
            'explicit': True,
            'singular_collection_name': 'alias',
        },
        'docs': {
            'explicit': True,
            'singular_collection_name': 'docs',
            'ragged_right': True,
        },
        'value_flags': {
            'is_editable': True,
            'explicit': True,
        },
        'static_value_flags': {
            'explicit': True,
        },
    }
    order = ['COND', 'POOL', 'KE', 'MOD', 'RC', 'VARS', 'VEL']
    kws = {}
    for k in ['global', 'helper']:
        kws[f'update_existing_{k}'] = True
    for suffix in order:
        last = (suffix == order[-1])
        if not first:
            args.append_global = 'preserved'
            args.append_helper = 'direct'
        if last:
            for k in ['global', 'helper']:
                kws[f'update_existing_{k}'] = False
        args.src = os.path.join(srcdir, f'*_{suffix}.txt')
        args.dst = os.path.join(dstdir_inc, f'enums_{suffix}.hpp')
        args.dst_c_source = os.path.join(dstdir_src,
                                         f'enums_{suffix}.cpp')
        args.macro_suffix_c = macro_suffix.get(suffix, suffix)
        args.strip_suffix_global = f'_{suffix}'
        for ftype, ftype_args in added_classes.items():
            setattr(args, f'expect_empty_{ftype}', True)
            setattr(args, f'expect_empty_{ftype}_header', True)
            for k, v in ftype_args.items():
                setattr(args, f'{k}_{ftype}', v)
        dst = generate(args, **kws)
        if not last:
            for k in ['global', 'helper']:
                kws[f'existing_{k}'] = dst.added_files[k].existing
        first = False
    if args.timestamp_file:
        with open(args.timestamp_file, 'w') as fd:
            fd.write("STAMP1")


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
    parser.add_argument("--ePhotosynthesis", action="store_true",
                        help="Generate the enumerators for ePhotosynthesis")
    parser.add_argument("--timestamp-file", type=str,
                        help=("File that should be created that can be "
                              "used as a time stamp"))
    for x in registered_classes('generator', return_classes=True):
        x.add_arguments(parser)
    args = parser.parse_args()
    generate(args)
