import os
import math
import pdb
import copy
import shutil
import sys
import argparse
import pprint
import subprocess
import glob
import difflib
import site
import warnings
import re
import tempfile
import scipy
from collections import OrderedDict
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from io import StringIO


_source_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
_utils_dir = os.path.join(_source_dir, 'utils')
_data_dir = os.path.join(_source_dir, 'tests', 'data')
_scripts_dir = os.path.abspath(os.path.dirname(__file__))
_param_dir = os.path.join(_source_dir, 'param')
_zhu2012_dir = os.path.join(_source_dir, 'zhu2012')

_platform = None
_library_path_var = None
if sys.platform == 'darwin':
    _platform = 'osx'
    _library_path_var = 'DYLD_LIBRARY_PATH'
elif 'linux' in sys.platform:
    _platform = 'linux'
    _library_path_var = 'LD_LIBRARY_PATH'
elif sys.platform in ['win32', 'cygwin']:
    _platform = 'win'
    _library_path_var = 'PATH'

sys.path.append(_utils_dir)

_figures = [
    'Zhu2012_Fig2', 'Zhu2012_Fig3',
    'Zaks2012_FigS3',
]
_param_sets = _figures + [
    'dilkaran', 'NPQ-explore',
    'Zhu2012_MATLAB', 'Zhu2012_REGEN',
    # True, 'Zaks_init',
]
_light_profiles = _param_sets + [
    'light-dark-light', 'dark-light-dark', 'steady-state',
]


class NoDefault(object):
    pass


def is_case_sensitive(src):
    with tempfile.NamedTemporaryFile(prefix='TmP', dir=src) as tmp_file:
        return (not os.path.exists(tmp_file.name.lower()))


def cli_param(x):
    k, v = x.split(':')
    return k, float(v)


def parse_driver(x):
    if isinstance(x, int):
        return x
    if x.isnumeric():
        return int(x)
    if x == 'all':
        return 0
    return SubTask._driver_map_reverse[x]


def search_directory(pattern, directory, ext=None, check=False,
                     ignore_prefix=None):
    if isinstance(ignore_prefix, str):
        ignore_prefix = [ignore_prefix]
    if ext is None:
        ext = r'\.*'
    cmd = ['grep', pattern, os.path.join(directory, f'*{ext}')]
    raw_output = subprocess.run(
        ' '.join(cmd), capture_output=True, shell=True, check=check,
    ).stdout.decode('utf-8')
    out = {}
    for x in raw_output.splitlines():
        fname, line = x.split(':', maxsplit=1)
        if ignore_prefix and fname.startswith(tuple(ignore_prefix)):
            continue
        fname = os.path.relpath(fname, _source_dir)
        out.setdefault(fname, [])
        out[fname].append(line.lstrip())
    return out


# TODO: This is not necessary after migration of these variables
#   to the correct names.
def get_matlab_aliases(reverse=False, strip_prefix=False):
    aliases = {
        # "BF::POOL::kA_d": "BF::POOL::Tcyt",
        # "BF::POOL::kA_f": "BF::POOL::Tcytc2",
        # "BF::POOL::kA_U": "BF::POOL::TK",
        # "BF::POOL::kU_A": "BF::POOL::TMg",
        # "BF::POOL::kU_d": "BF::POOL::TCl",
        # "BF::POOL::kU_f": "BF::POOL::TFd",
        # "BF::POOL::k1": "BF::POOL::TA",
        # "BF::POOL::k_r1": "BF::POOL::TQ",
        # "BF::POOL::kz": "BF::POOL::BFTs",
        # "BF::POOL::k12": "BF::POOL::BFTl",
        # "BF::POOL::k23": "BF::POOL::P700T",
        # "BF::POOL::k30": "BF::POOL::NADPHT",
    }
    out = aliases
    if reverse:
        out = {v: k for k, v in out.items()}
    if strip_prefix:
        if strip_prefix in ['keys', True]:
            out = {k.rsplit('::', maxsplit=1)[-1]: v
                   for k, v in out.items()}
        if strip_prefix in ['values', True]:
            out = {k: v.rsplit('::', maxsplit=1)[-1]
                   for k, v in out.items()}
    return out


def add_directory_to_path(directory, return_path=False,
                          prev_value=None, prepend=False):
    if isinstance(directory, list):
        directory = os.pathsep.join(directory)
    if prev_value is None:
        prev_value = os.environ.get(_library_path_var, '')
    out = ''
    if prev_value:
        if prepend:
            out = os.pathsep + prev_value
        else:
            out = prev_value + os.pathsep
    if prepend:
        out = directory + out
    else:
        out += directory
    if return_path:
        return out
    os.environ[_library_path_var] = out


def find_matlab(required=False):
    if shutil.which('matlab'):
        return shutil.which('matlab')
    if sys.platform == 'darwin':
        template = '/Applications/MATLAB_*.app/bin/matlab'
    else:
        if not required:
            return None
        raise Exception('No guess at location of installed MATLAB')
    locations = glob.glob(template)
    if not locations:
        if not required:
            return None
        raise Exception('MATLAB could not be located')
    return sorted(locations)[-1]  # Return newest version


def get_param_files(mod=None, pt=None):
    if mod is None:
        mod = '*'
    if pt is None:
        pt = '*'
    return sorted(glob.glob(os.path.join(_param_dir, f'{mod}_{pt}.txt')))


def paramfile2mod(fname):
    return os.path.splitext(
        os.path.basename(fname))[0].rsplit('_', maxsplit=1)[0]


def paramfile2pt(fname):
    return os.path.splitext(
        os.path.basename(fname))[0].rsplit('_', maxsplit=1)[1]


def get_module_list(pt=None, exclude_NPQ=False, include_ALL=False,
                    exclude_empty=False):
    out = ParameterSet.all_modules(pt=pt)
    if not exclude_empty:
        modules = [paramfile2mod(f) for f in get_param_files(pt="MOD")]
        out += [x for x in modules if x not in out]
    if exclude_NPQ and 'NPQ' in out:
        out.remove('NPQ')
    elif (not exclude_NPQ) and 'NPQ' not in out:
        out.append('NPQ')
    if include_ALL and 'ALL' not in out:
        out.append('ALL')
    elif (not include_ALL) and 'ALL' in out:
        out.remove('ALL')
    return sorted(out)


def get_param_type_list(mod=None):
    return ParameterSet.all_param_types(mod=mod)


def get_param_names(mod, pt, return_dict=False, **kwargs):
    param = ParameterSet.all_parameters().select(mod, pt, **kwargs)
    if return_dict:
        return param
    return list([v.name for v in param.values()])


def read_default_param(with_prefixes=False):
    defaults = OrderedDict()
    param_files = sorted(glob.glob(os.path.join(_param_dir, '*.txt')))
    for param_file in param_files:
        base = os.path.splitext(os.path.basename(param_file))[0]
        if base in ["README", "RedoxReg_MP", "VAR"]:
            continue
        mod, pt = base.rsplit('_', maxsplit=1)
        iparam = read_param(param_file, default=True)
        if with_prefixes:
            for k, v in iparam.items():
                kp = '::'.join([mod, pt, k])
                defaults[kp] = iparam
        else:
            defaults.setdefault(mod, OrderedDict())
            defaults[mod][pt] = iparam
    return defaults


def norm_param(src, dst, maxlen=None):
    if os.path.isfile(dst):
        print("DESTINATION EXISTS", dst)
    assert not os.path.isfile(dst)
    out = read_param(src)
    write_param(dst, out, sort=True, maxlen=maxlen)


def read_param(fname, default=False, include_qualifiers=None,
               exclude_qualifiers=None):
    out = OrderedDict()
    with open(fname, 'r') as fd:
        contents = fd.readlines()
    for x0 in contents:
        x = x0
        if '#' in x0:
            x, comment = x0.split('#', maxsplit=1)
        x = x.strip()
        if not x:
            continue
        if default:
            fields = x.split()
            name = fields[0]
            iout = {'value': float(fields[1]),
                    'comment': comment.strip()}
            if len(fields) == 2:
                iout['value_c3'] = iout['value']
            elif len(fields) == 3:
                iout['value_c3'] = float(fields[2])
            else:
                raise ValueError(f"More than 3 fields: \"{x}\"")
            if include_qualifiers and not any(x in iout['comment']
                                              for x in include_qualifiers):
                continue
            if exclude_qualifiers and any(x in iout['comment']
                                          for x in exclude_qualifiers):
                continue
            out[name] = iout
        else:
            name, value = x.rsplit(maxsplit=1)
            out[name] = float(value)
    return out


def read_param_table(fname, no_title=False):
    if no_title:
        return pd.read_csv(fname)
    with open(fname, 'r') as fd:
        title = fd.readlines()[0].strip()
        # df = pd.read_csv(fd)
    df = pd.read_csv(fname, header=1)
    return df, title


def write_param_table(fname, param, title=None):
    if not isinstance(param, pd.DataFrame):
        param = pd.DataFrame(param)
    if title is False:
        param.to_csv(fname, index=False)
        return
    if title is None:
        title = ''
    assert isinstance(param, pd.DataFrame)
    with open(fname, 'w') as fd:
        fd.write(title + '\n')
        param.to_csv(fd, index=False)


def write_param(fname, param, sort=False, comment_incomplete=False,
                exclude_param=None, maxlen=None, header=None):
    if exclude_param is None:
        exclude_param = []
    if maxlen is None:
        maxlen = len(max(param.keys(), key=len)) + 4
    maxlen_value = len(
        max([f'{v["value"]}' if isinstance(v, dict) else f'{v}'
             for v in param.values()], key=len)
    )
    if maxlen_value > 20:
        maxlen_value = 20
    if fname is True:
        fd = StringIO()
    else:
        fd = open(fname, 'w')
    if header:
        fd.write(header)
    order = param.keys()
    if sort in [True, 'names']:
        order = sorted(order)
    elif isinstance(sort, str):

        def get_sortkey(x):
            v = param[x]
            if isinstance(v, Parameter):
                return getattr(v, sort)
            else:
                return v[sort]

        order = sorted(order, key=get_sortkey)
    for k in order:
        v = param[k]
        pad = ' ' * (maxlen - len(k))
        comment = ''
        added_values = []
        if isinstance(v, dict):
            comment = v.get('comment', '')
            value = v['value']
        elif isinstance(v, Parameter):
            comment += f'[{v.original_name}] '
            value = v.value
            if v.comment:
                comment += v.comment
            if comment_incomplete and not v.is_complete:
                k = f'# {k}'
        elif isinstance(v, list):
            value = v[0]
            added_values = v[1:]
        else:
            value = v
        if k in exclude_param:
            k = f'# {k}'
        vstr = f'{value}'
        if comment:
            comment = (
                ' ' * (maxlen_value - len(vstr) + 2) + f'# {comment}'
            )
        fd.write(f'{k}{pad}{vstr}{comment}\n')
        for x in added_values:
            fd.write(' ' * maxlen + f'{x}\n')
    if fname is True:
        return fd.getvalue()
    fd.close()


def read_output_table(fname, sep=','):
    with open(fname, 'r') as fd:
        contents = fd.readlines()
    if len(contents) > 1:
        return read_param_table(fname, no_title=True)
    names = ['ALL::VARS::CO2AR']
    values = [
        np.array([float(x)]) for x in contents[-1].strip().split(sep)
    ]
    out = {k: v for k, v in zip(names, values)}
    return out


class ComparisonError(RuntimeError):
    pass


def check_output(f1, f2, reltol=1.0e-05, abstol=1.0e-08, sep=',',
                 label_f1='file A', label_f2='file B',
                 f1_prune=None, f2_prune=None, **kwargs):
    x1dict = read_output_table(f1, sep=sep)
    x2dict = read_output_table(f2, sep=sep)
    if f1_prune is not None:
        kwargs.setdefault('x1rem', {})
    if f2_prune is not None:
        kwargs.setdefault('x2rem', {})
    out = compare_dict(x1dict, x2dict, reltol=reltol, abstol=abstol,
                       label_f1=label_f1, label_f2=label_f2, **kwargs)
    if not out:
        if f1_prune is not None:
            write_param_table(f1_prune, kwargs['x1rem'], title=False)
        if f2_prune is not None:
            write_param_table(f2_prune, kwargs['x2rem'], title=False)
    return out


def check_param(f1, f2, reltol=1.0e-05, abstol=1.0e-08, sep='\t',
                label_f1='file A', label_f2='file B',
                f1_prune=None, f2_prune=None, **kwargs):
    x1dict = read_param(f1)
    x2dict = read_param(f2)
    if f1_prune is not None:
        kwargs.setdefault('x1rem', {})
    if f2_prune is not None:
        kwargs.setdefault('x2rem', {})
    out = compare_dict(x1dict, x2dict, reltol=reltol, abstol=abstol,
                       label_f1=label_f1, label_f2=label_f2,
                       **kwargs)
    if not out:
        if f1_prune is not None:
            write_param(f1_prune, kwargs['x1rem'], sort=True,
                        header=f'# {label_f1}\n# {f1}\n')
        if f2_prune is not None:
            write_param(f2_prune, kwargs['x2rem'], sort=True,
                        header=f'# {label_f2}\n# {f2}\n')
    return out


def compare_dict(x1dict, x2dict, reltol=1.0e-05, abstol=1.0e-08,
                 label_f1='file A', label_f2='file B', quiet=False,
                 x1rem=None, x2rem=None):
    out = True
    for k in x1dict.keys():
        if k not in x2dict:
            if not quiet:
                print(f"\"{k}\" missing from {label_f2}")
            out = False
            if x1rem is not None:
                x1rem[k] = x1dict[k]
    for k in x2dict.keys():
        if k not in x1dict:
            if not quiet:
                print(f"\"{k}\" missing from {label_f1}")
            out = False
            if x2rem is not None:
                x2rem[k] = x2dict[k]
    for k in x1dict.keys():
        if k not in x2dict:
            continue
        x1 = x1dict[k]
        x2 = x2dict[k]
        iout = np.isclose(x1, x2, rtol=reltol, atol=abstol)
        if not iout:
            out = False
            if x1rem is not None:
                x1rem[k] = x1dict[k]
            if x2rem is not None:
                x2rem[k] = x2dict[k]
            if not quiet:
                reldiff = np.abs(x1 - x2) / x2
                absdiff = np.abs(x1 - x2)
                print(
                    f"Values differ for \"{k}\": {x1} vs {x2}\n"
                    f"    Relative diff: {reldiff} (reltol = {reltol})\n"
                    f"    Absolute diff: {absdiff} (abstol = {abstol}\n"
                )
    return out


def compare_files(f1, f2, check_files=None, ftype='parameter',
                  check_files_kwargs=None, output_diff=None,
                  dont_prune_diff=False):
    if check_files_kwargs is None:
        check_files_kwargs = {}
    with open(f1, 'r') as fd:
        lines1 = fd.readlines()
    with open(f2, 'r') as fd:
        lines2 = fd.readlines()
    line_diff = list(difflib.unified_diff(lines1, lines2,
                                          fromfile=f1, tofile=f2))
    generated_files = []
    if output_diff and not dont_prune_diff:
        if 'f1_prune' not in check_files_kwargs:
            check_files_kwargs['f1_prune'] = '_TMP'.join(
                os.path.splitext(f1))
            generated_files.append(check_files_kwargs['f1_prune'])
        if 'f2_prune' not in check_files_kwargs:
            check_files_kwargs['f2_prune'] = '_TMP'.join(
                os.path.splitext(f2))
            generated_files.append(check_files_kwargs['f2_prune'])
    if line_diff:
        if check_files is None:
            if ftype == 'output':
                check_files = check_output
            elif ftype == 'parameter':
                check_files = check_param
        if (((check_files is not None)
             and check_files(f1, f2, **check_files_kwargs))):
            return
        line_diff = ''.join(line_diff)
        if output_diff:
            f1tmp = f1
            f2tmp = f2
            if (((not dont_prune_diff)
                 and os.path.isfile(check_files_kwargs['f1_prune'])
                 and os.path.isfile(check_files_kwargs['f2_prune']))):
                f1tmp = check_files_kwargs['f1_prune']
                f2tmp = check_files_kwargs['f2_prune']
            try:
                subprocess.run(
                    f'diff {f1tmp} {f2tmp} &> {output_diff}', shell=True)
            finally:
                for x in generated_files:
                    if os.path.isfile(x):
                        os.remove(x)
            # line_diff += f'\nOUTPUT_DIFF: {output_diff}\n  {f1}\n  {f2}'
            return
        raise ComparisonError(
            f"{ftype.title()} files differ:\n{line_diff}"
        )


class InstrumentedParser(argparse.ArgumentParser):

    def __init__(self, *args, **kwargs):
        self._subparser_classes = {}
        self._subparser_defaults = {}
        self._subparser_func = kwargs.pop('func', None)
        super(InstrumentedParser, self).__init__(*args, **kwargs)

    def parse_args(self, args=None, **kwargs):
        out = super(InstrumentedParser, self).parse_args(
            args=args, **kwargs)
        args_supp = []
        for k, v in self._subparser_defaults.items():
            if v is None or getattr(out, k, None):
                continue
            args_supp.append(v)
        if args_supp:
            out = super(InstrumentedParser, self).parse_args(
                args_supp + sys.argv, **kwargs)
        return out

    def add_subparsers(self, *args, **kwargs):
        name = kwargs['dest']
        self._subparser_defaults[name] = kwargs.pop('default', None)
        kwargs.setdefault('parser_class', InstrumentedParser)
        out = super(InstrumentedParser, self).add_subparsers(
            *args, **kwargs)
        self._subparser_classes[name] = out
        return out

    def run_subparser(self, name, args):
        subparser = getattr(args, name)
        if subparser is None:
            subparser = self._subparser_defaults[name]
        print(f"RUNNING {subparser}")
        self._subparser_classes[name].choices[
            subparser]._subparser_func(args)

    def add_argument(self, *args, **kwargs):
        subparsers = kwargs.pop('subparsers', None)
        subparser_defaults = kwargs.pop('subparser_defaults', {})
        if subparsers:
            for k, v in subparsers.items():
                if v == 'all':
                    unique_prog = []
                    v = []
                    for kk, vv in self._subparser_classes[k].choices.items():
                        if vv.prog not in unique_prog:
                            unique_prog.append(vv.prog)
                            v.append(kk)
                for x in v:
                    ikw = dict(**kwargs)
                    if x in subparser_defaults:
                        ikw['default'] = subparser_defaults[x]
                    self._subparser_classes[k].choices[x].add_argument(
                        *args, **ikw)
        else:
            super(InstrumentedParser, self).add_argument(*args, **kwargs)


class SubTask:

    _drivers = ['trDynaPS', 'DynaPS', 'CM', 'EPS']
    _driver_map = {(i + 1): x for i, x in enumerate(_drivers)}
    _driver_map_reverse = {x: (i + 1) for i, x in enumerate(_drivers)}
    _languages = ['cpp', 'matlab', 'python']

    @classmethod
    def adjust_args(cls, args):
        pass

    def __init__(self, args, dont_cleanup=False, **kwargs):
        self.initialize(args, dont_cleanup=dont_cleanup)
        self.run_commands(args, **kwargs)

    def __del__(self):
        self.cleanup_files()

    def initialize(self, args, dont_cleanup=False):
        if hasattr(self, 'dont_cleanup'):
            return
        self.current_args = args
        self.dont_cleanup = dont_cleanup
        self._generated_files = []
        self.adjust_args(args)

    def cleanup_files(self):
        if self.dont_cleanup or self.current_args.dont_cleanup:
            return
        for x in self._generated_files:
            if os.path.isfile(x):
                os.remove(x)
            elif os.path.isdir(x) and not glob.glob(os.path.join(x, '*')):
                os.rmdir(x)
            elif '*' in x:
                xfiles = glob.glob(x)
                for xx in xfiles:
                    os.remove(xx)

    @classmethod
    def prefix_path_args(cls, args, names, prefix=None):
        for k in names:
            v = getattr(args, k)
            if v:
                if v is True:
                    # raise ValueError(f'Cannot prefix a path for '
                    #                  f'\"{k}\" with no default')
                    continue
                v = os.path.expanduser(v)
                if not os.path.isabs(v):
                    if prefix and not os.path.dirname(v):
                        v = os.path.join(prefix, v)
                    if not os.path.isabs(v):
                        v = os.path.abspath(v)
                    v = os.path.normpath(v)
                setattr(args, k, os.path.expanduser(v))

    @classmethod
    def reset_path_args(cls, args, names, value=NoDefault,
                        set_to_base=False):
        for k in names:
            if hasattr(args, f'original_{k}'):
                if set_to_base == 'original':
                    setattr(args, k, getattr(args, f'original_{k}'))
                delattr(args, f'original_{k}')
            if value is not NoDefault:
                assert not set_to_base
                setattr(args, k, value)
            elif set_to_base and isinstance(getattr(args, k, None), str):
                setattr(args, k, os.path.basename(getattr(args, k)))

    @classmethod
    def suffix_path_args(cls, args, names, suffix):
        for k in names:
            k0 = f'original_{k}'
            if not hasattr(args, k0):
                setattr(args, k0, getattr(args, k))
            v0 = getattr(args, k0)
            if v0:
                if v0 is True:
                    # raise ValueError(f'Cannot suffix a path for '
                    #                  f'\"{k}\" with no default')
                    continue
                v0 = os.path.expanduser(v0)
                parts = os.path.splitext(v0)
                if parts[0].endswith('_') and not suffix.endswith('_'):
                    suffix = suffix + '_'
                if suffix.startswith('_') and parts[0].endswith('_'):
                    v = suffix.lstrip('_').join(os.path.splitext(v0))
                else:
                    v = suffix.join(os.path.splitext(v0))
                setattr(args, k, v)

    def run_commands(self, args, cmds=None, output_file=None,
                     allow_error=False, **kwargs):
        if cmds is None:
            cmds = []
        if not cmds:
            return
        cmdS = [x if isinstance(x, str) else ' '.join(x)
                for x in cmds]
        cmdS = '\n\t'.join(cmdS)
        print(f"Running\n{cmdS}\n"
              f"with {pprint.pformat(kwargs)}")
        if kwargs.get('env', None):
            kwargs['env'] = dict(os.environ, **kwargs['env'])
        output_str = b''
        if output_file:
            kwargs.update(capture_output=True)
        for x in cmds:
            xargs = x if isinstance(x, list) else x.split()
            ires = subprocess.run(xargs, **kwargs)
            if ires.returncode != 0 and not allow_error:
                raise RuntimeError(f'Error in running \'{x}\'')
            if output_file:
                output_str += ires.stdout
        if output_file:
            with open(output_file, 'wb') as fd:
                fd.write(output_str)


class BuildSubTask(SubTask):

    @classmethod
    def adjust_args(cls, args):
        cls.prefix_path_args(args, ['build_dir', 'install_dir'])
        if args.isolated_scikit_build:
            args.scikit_build = True
        if args.scikit_build:
            args.with_python = False
        if args.only_python:
            assert args.target in [None, 'pyPhotosynthesis']
            args.target = 'pyPhotosynthesis'
        if args.target == 'pyPhotosynthesis':
            args.component = 'Python'
            args.with_python = False
        if not args.install_dir_python:
            if args.install_dir and args.target == 'pyPhotosynthesis':
                args.install_dir_python = args.install_dir
            else:
                args.install_dir_python = site.getsitepackages()[0]
        if not args.install_dir:
            if args.target == 'pyPhotosynthesis':
                args.install_dir = args.install_dir_python
            else:
                args.install_dir = os.path.join(_source_dir, '_install')
        if args.with_python:
            args.with_python = build.copy_args(
                args, target='pyPhotosynthesis')
        super(BuildSubTask, cls).adjust_args(args)

    def __init__(self, args, **kwargs):
        self.initialize(args, kwargs.get('dont_cleanup', False))
        self.adjust_args(args)
        kwargs.setdefault('cwd', args.build_dir)
        kwargs.setdefault('env', {})
        add_dirs = [args.build_dir]
        if os.environ.get('CONDA_PREFIX'):
            conda_prefix = os.environ['CONDA_PREFIX']
            if sys.platform in ['win32', 'cygwin']:
                conda_prefix = os.path.join(
                    conda_prefix, 'Library', 'lib')
            else:
                conda_prefix = os.path.join(conda_prefix, 'lib')
            add_dirs.append(conda_prefix)
        kwargs['env'][_library_path_var] = add_directory_to_path(
            add_dirs, return_path=True, prepend=True,
            prev_value=kwargs['env'].get(_library_path_var, None),
        )
        super(BuildSubTask, self).__init__(args, **kwargs)

    def run_commands(self, args, config_args=None, build_args=None,
                     install_args=None, build_kwargs=None, build_env=None,
                     **kwargs):
        if build_kwargs is None:
            build_kwargs = {}
        build_kwargs.setdefault('env', build_env)
        if not args.dont_build:
            build(args, config_args=config_args, build_args=build_args,
                  install_args=install_args, **build_kwargs)
        super(BuildSubTask, self).run_commands(args, **kwargs)


class scikit_build(SubTask):

    @classmethod
    def adjust_args(cls, args):
        if args.target != 'pyPhotosynthesis':
            args.build_dir += '_scikit'
        args.target = None
        args.component = None
        # if args.isolated_scikit_build:
        #     warnings.warn(
        #         "Build must occur within the current environment that "
        #         "already has the build dependencies installed until "
        #         "yggdrasil wheels either install the library or the "
        #         "configuration routines are patched such that the "
        #         ".yggconfig file location for yggdrasil installed in a "
        #         "pip virtual build env is different than the .yggconfig "
        #         "file for yggdrasil installed in the target env (e.g. "
        #         "conda env)")
        #     args.isolated_scikit_build = False
        super(scikit_build, cls).adjust_args(args)

    def __init__(self, args, config_args=None, install_args=None,
                 **kwargs):
        self.adjust_args(args)
        if install_args is None:
            install_args = []
        cmds = []
        install_args += [
            '--ignore-installed', '-v',
        ]
        if not args.isolated_scikit_build:
            build_deps = [
                "scikit-build-core",
                "setuptools_scm",
                "numpy<2",
                "yggdrasil-framework",
            ]
            cmds += [
                f"pip install {' '.join(build_deps)}",
            ]
            if args.rebuild and os.path.isdir(args.build_dir):
                shutil.rmtree(args.build_dir)
            if not os.path.isdir(args.build_dir):
                os.mkdir(args.build_dir)
            install_args += [
                '--no-build-isolation',
                f'--config-settings=build-dir={args.build_dir}',
            ]
        if args.dont_install:
            install_args += [
                # '--config-settings=editable.rebuild=true',
                '-e',
            ]
        config_args = build.config_args(args, config_args=config_args,
                                        for_scikit_build=True)
        if config_args:
            kwargs.setdefault('env', {})
            kwargs['env'].setdefault(
                'CMAKE_ARGS', os.environ.get('CMAKE_ARGS', ''))
            kwargs['env']['CMAKE_ARGS'] = ' '.join([
                x for x in config_args + [kwargs['env']['CMAKE_ARGS']]
                if x
            ])
        cmds += [
            f"pip install {' '.join(install_args)} ."
        ]
        kwargs.setdefault('cwd', _source_dir)
        super(scikit_build, self).__init__(args, cmds=cmds, **kwargs)


class build(SubTask):

    @classmethod
    def adjust_args(cls, args):
        BuildSubTask.adjust_args(args)
        super(build, cls).adjust_args(args)

    @classmethod
    def copy_args(cls, args, **kwargs):
        out = copy.deepcopy(args)
        for k, v in kwargs.items():
            setattr(out, k, v)
        cls.adjust_args(out)
        return out

    @classmethod
    def config_args(cls, args, config_args=None,
                    for_scikit_build=False, **kwargs):
        if kwargs:
            args = cls.copy_args(args, **kwargs)
        config_args = copy.deepcopy(config_args)
        if config_args is None:
            config_args = []
        config_args += ['-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON']
        config_flags = {
            'BUILD_CXX': (
                args.target in [None, 'EPhotosynthesis', 'ePhoto']),
            'BUILD_PYTHON': (
                args.with_python or args.target == 'pyPhotosynthesis'),
            'BUILD_TESTS': args.build_tests,
            'BUILD_DOCS': args.build_docs,
            'TEST_COVERAGE': args.with_coverage,
            'MAKE_EQUIVALENT_TO_MATLAB': args.make_equivalent_to_matlab,
            'WITH_ASAN': args.with_asan,
            'EPHOTO_USE_SCOPED_ENUM': args.force_scoped_enum,
            'WITH_YGGDRASIL': args.with_yggdrasil,
            'BUILD_WITH_YGGINTERFACE': (args.with_yggdrasil == 'direct'),
        }
        for k, v in config_flags.items():
            config_args.append(cls.cmake_bool_flag(k, v))
        if args.with_python or args.target == 'pyPhotosynthesis':
            if not (args.dont_install or for_scikit_build):
                config_args += [
                    f'-DINSTALL_PREFIX_PYTHON={args.install_dir_python}'
                ]
        if not (args.dont_install or for_scikit_build):
            config_args += [f'-DCMAKE_INSTALL_PREFIX={args.install_dir}']
        cls.prune_duplicate_config_args(config_args)
        return config_args

    @classmethod
    def prune_duplicate_config_args(cls, config_args):
        existing = {}
        duplicates = {}
        idx_remove = []
        for idx, arg in enumerate(config_args):
            if '=' in arg:
                k, v = arg.split('=', maxsplit=1)
            else:
                k = arg
                v = None
            if k in existing:
                if existing[k] == v:
                    idx_remove.append(idx)
                else:
                    duplicates.setdefault(k, [existing[k]])
                    duplicates[k].append(v)
            else:
                existing[k] = v
        if duplicates:
            raise ValueError(f'Conflicting configuration arguments:\n'
                             f'{pprint.pformat(duplicates)}')
        for idx in idx_remove[::-1]:
            del config_args[idx]

    @classmethod
    def cmake_bool_flag(cls, name, condition):
        value = 'ON' if condition else 'OFF'
        return f'-D{name}:BOOL={value}'

    @classmethod
    def config_cmd(cls, args, **kwargs):
        config_args = cls.config_args(args, **kwargs)
        return f"cmake {_source_dir} {' '.join(config_args)}"

    @classmethod
    def build_args(cls, args, build_args=None, **kwargs):
        if kwargs:
            args = cls.copy_args(args, **kwargs)
        build_args = copy.deepcopy(build_args)
        if build_args is None:
            build_args = []
        if args.target:
            build_args += ['--target', args.target]
        if sys.platform != 'win32':
            build_args += ['--', '-j', str(args.njobs)]
        return build_args

    @classmethod
    def build_cmd(cls, args, **kwargs):
        build_args = cls.build_args(args, **kwargs)
        return (f"cmake --build . --config {args.build_type}"
                f" {' '.join(build_args)}")

    @classmethod
    def install_args(cls, args, install_args=None, **kwargs):
        if kwargs:
            args = cls.copy_args(args, **kwargs)
        install_args = copy.deepcopy(install_args)
        if install_args is None:
            install_args = []
        if args.component:
            install_args += ['--component', args.component]
        return install_args

    @classmethod
    def install_cmd(cls, args, **kwargs):
        install_args = cls.install_args(args, **kwargs)
        return (f"cmake --install . --prefix {args.install_dir} "
                f"{' '.join(install_args)}")

    def __init__(self, args, config_args=None, build_args=None,
                 install_args=None, **kwargs):
        self.adjust_args(args)
        if args.scikit_build:
            if args.target == 'pyPhotosynthesis':
                super(build, self).__init__(args, **kwargs)
            scikit_build(self.copy_args(args), config_args=config_args)
            if args.target == 'pyPhotosynthesis':
                return
        cmds = [
            self.config_cmd(args, config_args=config_args),
            self.build_cmd(args, build_args=build_args),
        ]
        if args.with_python:
            cmds += [
                self.build_cmd(args.with_python, build_args=build_args),
            ]
        if not args.dont_install:
            if not os.path.isdir(args.install_dir):
                os.mkdir(args.install_dir)
            cmds += [
                self.install_cmd(args, install_args=install_args),
            ]
            if args.with_python:
                cmds += [
                    self.install_cmd(args.with_python,
                                     install_args=install_args),
                ]
        if args.rebuild and os.path.isdir(args.build_dir):
            shutil.rmtree(args.build_dir)
        if ((args.rebuild and (not args.dont_install) and
             os.path.isdir(args.install_dir))):
            ephotodir = os.path.join(args.install_dir,
                                     'ePhotosynthesis')
            subdir = glob.glob(os.path.join(args.install_dir, '*'))
            if (not subdir) or subdir == [ephotodir]:
                shutil.rmtree(args.install_dir)
            elif os.path.isdir(ephotodir):
                shutil.rmtree(ephotodir)
        if not os.path.isdir(args.build_dir):
            os.mkdir(args.build_dir)
        kwargs.setdefault('cwd', args.build_dir)
        super(build, self).__init__(args, cmds=cmds, **kwargs)


class update_readme(BuildSubTask):

    def __init__(self, args):
        self.adjust_args(args)
        execFile = os.path.join(args.build_dir, 'ePhoto')
        cmds = [
            f'{execFile} -h'
        ]
        helpFile = os.path.join(os.getcwd(), 'help_output.txt')
        self._generated_files.append(helpFile)
        try:
            super(update_readme, self).__init__(args, cmds=cmds,
                                                output_file=helpFile)
            with open(helpFile, 'r') as fd:
                helpMsg = fd.read()
            os.remove(helpFile)
            helpMsg = helpMsg.split(':', 1)[-1]
            readme = os.path.join(_source_dir, "README.md")
            with open(readme, 'r') as fd:
                contents = fd.read()
            idx1 = contents.index("### Command line interface")
            idx2 = idx1 + contents[idx1:].index('```')
            idx3 = contents[idx2:].find('#')
            suffix = ''
            if idx3 >= 0:
                idx3 += idx2
                suffix = '\n\n' + contents[idx3:]
            prefix = contents[:idx2]
            contents = prefix + '```' + helpMsg + '```' + suffix
            with open(readme, 'w') as fd:
                fd.write(contents)
        except BaseException:
            if os.path.isfile(helpFile):
                os.remove(helpFile)
            raise


class test(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        if args.refresh_output:
            args.preserve_output = True
        super(test, cls).adjust_args(args)
        args.build_tests = (args.target != 'pyPhotosynthesis')

    def __init__(self, args, test_flags=None,
                 config_args=None, build_args=None, **kwargs):
        self.adjust_args(args)
        if test_flags is None:
            test_flags = []
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        test_flags += ['-C', args.build_type]
        pytest_flags = ['-sv']
        config_args.append(
            build.cmake_bool_flag(
                'PRESERVE_TEST_OUTPUT', args.preserve_output
            )
        )
        if args.stop_on_error:
            test_flags += ['--stop-on-failure']
            pytest_flags += ['-x']
        if args.verbose:
            test_flags += ['--output-on-failure', '-VV']
            pytest_flags += ['-v']
        if args.with_python and args.dont_install:
            python_prefix_dir = None
            if args.dont_install:
                python_prefix_dir = args.build_dir
            elif args.install_dir_python != site.getsitepackages()[0]:
                python_prefix_dir = args.install_dir
            if python_prefix_dir:
                kwargs.setdefault('env', {})
                kwargs['env'].setdefault(
                    'PYTHONPATH', os.environ.get('PYTHONPATH', ''))
                kwargs['env']['PYTHONPATH'] = os.pathsep.join([
                    x for x in
                    [python_prefix_dir,
                     kwargs['env']['PYTHONPATH']]
                    if x
                ])
        cmds = []
        if args.target != 'pyPhotosynthesis':
            if args.show_tests:
                cmds += ['ctest -N']
            else:
                cmds += [f"ctest {' '.join(test_flags)}"]
        if args.target == 'pyPhotosynthesis' or args.scikit_build:
            testdir = os.path.join(_source_dir, 'tests', 'python')
            cmds += [
                f"python -m pytest {' '.join(pytest_flags)} {testdir}"
            ]
            if args.scikit_build or not args.dont_install:
                kwargs.setdefault('cwd', _source_dir)
        try:
            super(test, self).__init__(args, cmds=cmds,
                                       config_args=config_args,
                                       build_args=build_args, **kwargs)
        finally:
            if args.refresh_output:
                for drv in BuildSubTask._drivers:
                    fsrc = os.path.join(args.build_dir,
                                        f'output_{drv}.data')
                    fdst = os.path.join(_source_dir, 'tests',
                                        'data', f'ePhotoOutput_{drv}.txt')
                    if os.path.isfile(fsrc):
                        shutil.copy2(fsrc, fdst)
                        self._generated_files.append(fsrc)
                    else:
                        print(f"MISSING OUTPUT {fsrc}")


# class ygginfo(SubTask):

#     @classmethod
#     def adjust_args(cls, args):
#         super(ygginfo, cls).adjust_args(args)

#     def __init__(self, args):
#         import yggdrasil
#         import pdb; pdb.set_trace()
#         cmds = []
#         super(ygginfo, self).__init__(args, cmds=cmds)


class ParameterError(ValueError):
    pass


class ParameterNanError(ParameterError):
    pass


class ParameterNoMatchError(ParameterError):
    pass


class ParameterConflictError(ParameterError):

    def __init__(self, first, *args):
        self.choices = [first] + list(args)
        names = [x.original_name for x in self.choices]
        names = ' vs. '.join(names)
        self.msg = (
            f'Multiple parameters resolved to \"{first.fullname}\" '
            f'({names})\n'
        )
        for i, x in enumerate(self.choices):
            self.msg += '\n' + str(x) + '\n'
        super(ParameterConflictError, self).__init__(self.msg)


class DuplicateParameterError(ParameterError):

    def __init__(self, first, *args):
        self.choices = [first] + list(args)
        names = [x.fullname for x in self.choices]
        names = ' vs. '.join(names)
        self.msg = (
            f'Multiple options for \"{first.original_name}\" '
            f'({names})\n'
        )
        for i, x in enumerate(self.choices):
            self.msg += '\n' + str(x) + '\n'
        super(DuplicateParameterError, self).__init__(self.msg)


class ParameterSet(OrderedDict):

    _all_parameters = None
    _all_modules = {}
    _all_param_types = {}

    def get_original(self, k0, default=NoDefault):
        for v in self.values():
            if v.original_name == k0:
                return v
        if default != NoDefault:
            return default
        raise KeyError(k0)

    def find(self, name, default=NoDefault, mod=None, pt=None,
             exclude_fullname=False, exclude_name=False,
             include_aliases=False, include_original=False,
             include_matlab_var=False):
        fullname = name
        if mod and pt and not exclude_fullname:
            fullname = f'{mod}::{pt}::{name}'
            if fullname in self:
                return self[fullname]

        def check(v):
            if mod and v.mod != mod:
                return False
            if pt and v.pt != pt:
                return False
            if (not exclude_fullname) and v.fullname == name:
                return True
            if (not exclude_name) and v.name == name:
                return True
            if include_aliases and name in v.aliases:
                return True
            if include_original and v.original_name == name:
                return True
            if include_matlab_var and v.matlab_var == name:
                return True
            return False

        matches = [v for v in self.values() if check(v)]
        if len(matches) > 1:
            raise ParameterNoMatchError(
                f'Multiple matches for parameter \"{fullname}\": '
                f'{[v.fullname for v in matches]}'
            )
        elif len(matches) == 0:
            if default is not NoDefault:
                return default
            raise ParameterNoMatchError(
                f'No matches for parameter \"{fullname}\"'
            )
        return matches[0]

    def get_param(self, mod, pt, name, default=NoDefault, **kwargs):
        return self.find(name, default=default, mod=mod, pt=pt, **kwargs)

    def select(self, mod=None, pt=None,
               control_values=None, qualifiers=None,
               args=None, inverse=False):
        if control_values is None:
            control_values = {}
        if qualifiers is None:
            qualifiers = {}

        def set_default(x):
            if x is None:
                return []
            elif isinstance(x, str):
                return [x]
            return x

        mod = set_default(mod)
        pt = set_default(pt)
        if args is not None:
            control_values.setdefault('ALL::VARS::useC3', args.useC3)
            control_values.setdefault(
                'ALL::VARS::UseZaksNPQ', args.use_zaks_npq)
            if not mod:
                driver_name = SubTask._driver_map[args.driver]
                mod = ['ALL'] + instrument_matlab.get_children(driver_name)

        def check(k, v):
            if isinstance(mod, dict) and not mod.get(v.mod, True):
                return False
            elif mod and v.mod not in mod:
                return False
            if isinstance(pt, dict) and not pt.get(v.pt, True):
                return False
            elif pt and v.pt not in pt:
                return False
            for kq, vq in qualifiers.items():
                if vq:
                    if not v.qualifiers.get(kq, False):
                        return False
                else:
                    if v.qualifiers.get(kq, False):
                        return False
            if not all(control_values.get(ctrl, True)
                       for ctrl in v.qualifiers.get('CTRL_ON', [])):
                return False
            if any(control_values.get(ctrl, False)
                   for ctrl in v.qualifiers.get('CTRL_OFF', [])):
                return False
            return True

        out = type(self)()
        for k, v in self.items():
            if inverse:
                if check(k, v):
                    continue
            else:
                if not check(k, v):
                    continue
            out[k] = v
        return out

    def modules(self, pt=None):
        r"""list: Set of modules represented in this set."""
        if isinstance(pt, str):
            pt = [pt]
        return sorted(
            list(set([v.mod for v in self.values()
                      if v.mod and (pt is None or v.pt in pt)]))
        )

    def param_types(self, mod=None):
        r"""list: Set of parameter types represented in this set."""
        if isinstance(mod, str):
            mod = [mod]
        return sorted(
            list(set([v.pt for v in self.values()
                      if v.pt and (mod is None or v.mod in mod)]))
        )

    def read_defaults_file(self, fname):
        from generate_enum import ParamFileParser
        base = os.path.splitext(os.path.basename(fname))[0]
        if base in ["README", "RedoxReg_MP", "VAR"]:
            return
        mod, pt = base.rsplit('_', maxsplit=1)
        with open(fname, 'r') as fd:
            lines = fd.readlines()
        for line in lines:
            member = ParamFileParser.parse_line(
                line, src=fname, dont_promote_qualifiers=True)
            if not member:
                continue
            x = Parameter.from_enum_member(mod, pt, member)
            self[x.fullname] = x

    @classmethod
    def from_defaults_files(cls):
        out = cls()
        param_files = sorted(glob.glob(os.path.join(_param_dir, '*.txt')))
        for param_file in param_files:
            out.read_defaults_file(param_file)
        return out

    @classmethod
    def all_parameters(cls):
        if cls._all_parameters is None:
            cls._all_parameters = cls.from_defaults_files()
        return cls._all_parameters

    @classmethod
    def all_modules(cls, pt=None):
        if pt not in cls._all_modules:
            cls._all_modules[pt] = cls.all_parameters().modules(pt=pt)
        return copy.deepcopy(cls._all_modules[pt])

    @classmethod
    def all_param_types(cls, mod=None):
        if mod not in cls._all_param_types:
            cls._all_param_types[mod] = cls.all_parameters().param_types(
                mod=mod)
        return copy.deepcopy(cls._all_param_types[mod])

    @classmethod
    def name2fullname(cls, name, **kwargs):
        return cls.all_parameters().find(name, **kwargs).fullname

    @classmethod
    def normalize_dict(cls, param, discard_duplicates=False):
        for k in list(param.keys()):
            kfull = cls.name2fullname(k, include_aliases=True)
            if kfull != k:
                v = param.pop(k)
                if kfull in param and discard_duplicates:
                    continue
                assert kfull not in param
                param[kfull] = v


class Parameter:

    _default_attr = [
        'name', 'mod', 'pt', 'default', 'comment',  # 'conversion',
    ]
    _table_attr = [
        'title', 'description', 'reference', 'units',  # 'value',
    ]

    def __init__(self, name, mod=None, pt=None, aliases=None,
                 value=None, value_c3=None, comment=None,
                 title=None, description=None, reference=None,
                 units=None, default=None, original_name=None,
                 original_value=None, conversion=None,
                 qualifiers=None, matlab_var=None):
        if qualifiers is None:
            qualifiers = {}
        if aliases is None:
            aliases = qualifiers.get('ALIASES', [])
        if isinstance(aliases, dict):
            aliases = list(aliases.values())
        if reference is None:
            reference = qualifiers.get('CITATION', None)
        if matlab_var is None:
            matlab_var = qualifiers.get('MATLAB_VAR', None)
        if name.count('::') == 2:
            mod, pt, name = name.split('::')
            if comment is None:
                comment = "Explicit"
        elif name.count('::') == 1:
            mod, name = name.split('::')
        if original_name is None:
            original_name = name
        if original_value is None:
            original_value = value
        self.original_name = original_name
        self.original_value = original_value
        self.name = name
        self.mod = mod
        self.pt = pt
        self.value = value
        self.value_c3 = value_c3
        self.title = title
        self.description = description
        self.reference = reference
        self.units = units
        self.comment = comment
        self.qualifiers = qualifiers
        self.matlab_var = matlab_var
        self.aliases = OrderedDict()
        self.default = default
        self.choices = OrderedDict()
        self.conversion = conversion
        self.default_diff = np.nan
        for v in aliases:
            self.add_alias(v)

    def __str__(self):
        return self._make_string()

    @classmethod
    def from_enum_member(cls, mod, pt, member):
        kwmap = {
            'val': 'value',
            'val_alt': 'value_c3',
            'doc': 'description',
        }
        for k in ['qualifiers']:
            kwmap[k] = k
        kws = {
            'mod': mod,
            'pt': pt,
        }
        for k, v in kwmap.items():
            if k not in member:
                continue
            kws[v] = member[k]
        return cls(member['name'], **kws)

    def finalize(self, defaults=None, existing=None, aliases=None,
                 **kwargs):
        if self.is_complete and existing:
            if self.fullname == 'BF::COND::PHl' and not self.conversion:
                pdb.set_trace()
            if self.conversion:
                self.apply_conversion(existing)
        if defaults:
            self.check_against_default(defaults, aliases=aliases,
                                       **kwargs)

    def apply_conversion(self, existing):
        self.original_value = self.value
        if hasattr(self, f'convert_{self.conversion}'):
            getattr(self, f'convert_{self.conversion}')(existing)
        else:
            try:
                self.value *= float(self.conversion)
            except ValueError:
                pass
        self.conversion = None

    def convert_oxidized2total(self, existing):
        base = self.original_name[:-1]
        self.value += existing.get_original(base + 'r').value

    def convert_H2pH(self, existing):
        self.value = -np.log10(self.value / 1000.)

    def check_against_default(self, defaults, aliases=None,
                              min_diff=np.inf, excluded=None):
        # TODO: include default value & data from tables in comment
        if excluded is None:
            excluded = []
        self.comment = ''
        comment_values = []
        if not self.is_complete:
            comment_values += ['No match']
            self.default_diff = np.inf
            excluded.append(self.fullname)
        else:
            mod = self.mod
            pt = self.pt
            name = self.name
            if name not in defaults[self.mod][self.pt]:
                if aliases is None:
                    aliases = self.get_aliases()
                if name in aliases:
                    mod, pt, name = aliases[name].split('::')
            if name not in defaults[mod][pt]:
                print(mod, pt, name)
                pdb.set_trace()
            default = self.from_default_entry(
                self.name, self.mod, self.pt,
                defaults[mod][pt][name],
            )
            self.default_diff = self.diff_default(default)
            if self.default_diff > 0.01:
                comment_values += [
                    f'{self.default_diff}',
                    f'{default.value}', f'{default.value_c3}'
                ]
            if self.default_diff > min_diff:
                excluded.append(self.fullname)
        self.comment += ';'.join(comment_values)

    def _make_string(self, indent=0, tab='    ', extras=None, prefix=''):
        if extras is None:
            extras = OrderedDict()
        if self.aliases:
            extras.setdefault('aliases', list(self.aliases.keys()))
        if self.choices:
            extras.setdefault('choices', list(self.choices.keys()))
        if self.default:
            extras.setdefault('default', self.default)
        out = f'{prefix}{self.fullname} ({self.original_name})'
        for k in self._table_attr + ['value']:
            if getattr(self, k) is None:
                continue
            out += f'\n{tab}{k:16}: {getattr(self, k)}'
        for k, v in extras.items():
            vstr = str(v)
            if '\n' in vstr:
                vstr = f'\n{tab}'.join(vstr.split('\n'))
            out += f'\n{tab}{k:16}: {vstr}'
        if indent > 0:
            out = (
                (indent * tab)
                + f'\n{indent * tab}'.join(out.split('\n'))
            )
        return out

    @classmethod
    def from_default_entry(cls, name, mod, pt, x):
        kwargs = dict(x)
        kwargs['description'] = kwargs.pop('comment')
        return Parameter(name, mod=mod, pt=pt, **kwargs)

    @classmethod
    def from_table_entry(cls, row, title=None,
                         ignore_existing_names=False):
        name = row['Name']
        if isinstance(name, float) and math.isnan(name):
            raise ParameterNanError
        value = float(row['Value'])
        if math.isnan(value):
            raise ParameterNanError
        aliases = None
        original_name = name
        if (((not ignore_existing_names)
             and isinstance(row['C++ Parameter'], str))):
            name = row['C++ Parameter']
            if name.startswith('IGNORED'):
                raise ParameterNanError
            if ',' in name:
                names = name.split(',')
                name = names[0]
                aliases = names[1:]
        assert isinstance(name, str)
        kwargs = {}
        for k in ['description', 'reference', 'units']:
            if k.title() in row and isinstance(row[k.title()], str):
                kwargs[k] = row[k.title()]
        if 'units' in kwargs:
            kwargs['units'] = kwargs['units'].replace('\xad', '-')
            kwargs['units'] = kwargs['units'].replace('--', '-')
        kconv = 'C++ Parameter conversion'
        if kconv in row and not ((isinstance(row[kconv], float)
                                  and np.isnan(row[kconv]))
                                 or row[kconv] == 'nan'):
            kwargs['conversion'] = row[kconv]
        name = name.replace('\xad', '-')
        return Parameter(name, aliases=aliases,
                         value=value, title=title,
                         original_name=original_name, **kwargs)

    def update_row(self, df, index):
        updated = False
        if not self.is_complete:
            return updated
        new_value = ",".join(self.all_names)
        if df.loc[index, 'C++ Parameter'] != new_value:
            df.loc[index, 'C++ Parameter'] = new_value
            updated = True
        if ((self.conversion
             and (df.loc[index, 'C++ Parameter conversion']
                  != self.conversion))):
            df.loc[index, 'C++ Parameter conversion'] = self.conversion
            updated = True
        return updated

    @classmethod
    def user_selection(cls, choices, preamble=None, matchto=None,
                       existing=None, mask=None, include_none=True,
                       include_all=False, other_options=None):
        if other_options is None:
            other_options = []
        if isinstance(choices, list):
            choices = OrderedDict([(i, x) for i, x in enumerate(choices)])
        if mask is None:
            mask = {k: True for k in choices.keys()}
        selected = OrderedDict(
            [(k, x) for k, x in choices.items() if mask[k]]
        )
        if not selected:
            return None
        if len(selected) == 1:
            return list(selected.keys())[0]
        msg = ''
        if preamble:
            msg += preamble
        if include_none:
            other_options.append(None)
            msg += (
                f'\n[{len(other_options) - 1}] '
                f'Don\'t select any parameter\n'
            )
        if include_all:
            other_options.append(True)
            msg += (
                f'\n[{len(other_options) - 1}] '
                f'Select all parameters\n'
            )
        nadded_opt = len(other_options)
        for i, x in enumerate(selected.values()):
            extras = OrderedDict()
            if existing:
                extras['exists'] = (x.fullname in existing)
            if matchto:
                extras['matches'] = matchto.value_matches_default(x)
            msg += x._make_string(prefix=f'\n[{i + nadded_opt}] ',
                                  extras=extras)
        print(msg)
        while True:
            x = input(
                'Which parameter should be selected? [0]: '
            )
            if not x:
                return None
            if not x.isnumeric():
                continue
            x = int(x)
            if x >= 0 and x < (len(selected) + nadded_opt):
                if x < nadded_opt:
                    return other_options[x]
                return list(selected.keys())[x - nadded_opt]

    def has_external_choices(self, mod=None):
        if mod is None:
            mod = self.mod
        for x in self.choices.values():
            if x.fullname == self.fullname:
                continue
            if x.mod != mod:
                return True
        return False

    def update_from_choices(self, existing=False, ask_user=False,
                            discard_current=False, ignore=None):
        if discard_current:
            self.clear()
        if self.is_complete or not self.choices:
            return
        if ignore is None:
            ignore = []
        selected = None
        discard_duplicates = False
        mask = {k: True for k in self.choices.keys()}
        if ignore:
            for k in ignore:
                mask[k] = False

        def from_mask(imask):
            if sum(imask.values()) == 1:
                for k, v in imask.items():
                    if v:
                        return k
            return None

        if selected is None and len(self.choices) == 1:
            selected = list(self.choices.keys())[0]
        if selected is None and len(self.choices) > 1:
            is_equal = {
                k: self.value_matches_default(x)
                for k, x in self.choices.items()
            }
            selected = from_mask(is_equal)
            if selected is not None:
                discard_duplicates = True
        if selected is None and len(self.choices) > 1 and existing:
            for k, v in self.choices.items():
                if v.fullname in existing:
                    mask[k] = False
            # is_missing = {
            #     k: (x.fullname not in existing)
            #     for k, x in self.choices.items()
            # ]
            # selected = from_mask(is_missing)
            # if selected is not None:
            #     discard_duplicates = True
        first = list(self.choices.values())[0]
        if selected is None and not self.has_external_choices(mod=first.mod):
            selected = first.fullname
        if selected is None and ask_user:
            selected = self.user_selection(
                self.choices, matchto=self, existing=existing, mask=mask,
                include_all=True,
                preamble=(
                    f'\n{self}\n\nSelect one of the following choices '
                    f'to direct the above parameter to: \n'
                )
            )
            if selected is True:
                selected = first.fullname
                for x in list(self.choices.values())[1:]:
                    self.add_alias(x.fullname)
            discard_duplicates = True
        if selected is not None:
            self.update_from_default(self.choices[selected])
        if selected is None or ((not discard_duplicates)
                                and self.has_external_choices(mod=first.mod)):
            raise DuplicateParameterError(*self.choices.values())

    @classmethod
    def resolve_conflict(cls, *args, ask_user=True,
                         existing=None):
        name = args[0].fullname
        idx_edit = [
            i for i, x in enumerate(args)
            if len(x.choices) > 1
        ]
        idx_preserve = [
            i for i, x in enumerate(args)
            if len(x.choices) <= 1
        ]
        if len(idx_preserve) > 1:
            raise ParameterConflictError(*args)
        if len(idx_edit) > 1 and (not idx_preserve) and ask_user:
            idx_keep = cls.user_selection(
                [args[idx] for idx in idx_edit], preamble=(
                    f'Select a parameter to assign to '
                    f'\"{name}\": \n'
                )
            )
            if idx_keep is not None:
                idx_preserve.append(idx_keep)
                del idx_edit[idx_keep]
        for idx in idx_edit:
            args[idx].update_from_choices(
                existing=existing, ask_user=ask_user,
                discard_current=True, ignore=[name],
            )

    def diff_default(self, x, only_c3=False, no_c3=False):
        if x.default:
            return self.diff_default(x.default, only_c3=only_c3,
                                     no_c3=no_c3)

        def do_diff(a, b):
            if a is None:
                return None
            if a == 0 or b == 0:
                return np.abs(a - b)
            base = a if np.abs(a) < np.abs(b) else b
            return np.abs((a - b) / base)

        diff_value = do_diff(x.value, self.value)
        diff_value_c3 = do_diff(x.value_c3, self.value)
        if only_c3:
            return diff_value_c3
        if no_c3:
            return diff_value
        if diff_value_c3 is None:
            return diff_value
        return min([diff_value, diff_value_c3])

    def value_matches_default(self, x, only_c3=False, no_c3=False):
        if x.default:
            return self.value_matches_default(x.default, only_c3=only_c3,
                                              no_c3=no_c3)
        if (not only_c3) and self.value == x.value:
            return True
        if not no_c3:
            return self.value == x.value_c3
        return False

    def add_alias(self, x):
        if isinstance(x, str):
            kws = {
                k: getattr(self, k) for k in [
                    'value', 'value_c3',
                    'original_name', 'original_value',
                ]
            }
            return self.add_alias(Parameter(x, **kws))
        if x.fullname in self.aliases:
            return
        self.aliases[x.fullname] = x

    def add_choice(self, x):
        if x.default:
            conversion = x.conversion
            x = x.default
            if conversion:
                x.conversion = conversion
        else:
            x.clear_table()
        if ((x.fullname in self.choices
             and not x.matches(self.choices[x.fullname]))):
            err = ParameterConflictError(self.choices[x.fullname], x)
            print(err.msg)
            pdb.set_trace()  # TODO
            raise err
        self.choices[x.fullname] = x

    def alternative(self, name=None):
        alternative = copy.copy(self)
        alternative.aliases = OrderedDict()
        alternative.choices = OrderedDict()
        if name is not None:
            alternative.name = name
        return alternative

    def alternative_from_default(self, mod, pt, x, **kwargs):
        alternative = self.alternative(**kwargs)
        alternative.update_from_default(mod, pt, x)
        return alternative

    def update_from_default(self, mod, pt=None, x=None):
        if isinstance(mod, Parameter):
            assert pt is None and x is None
            for k in self._default_attr + ['conversion']:
                setattr(self, k, getattr(mod, k))
            self.default = mod
        else:
            assert pt is not None and x is not None
            self.mod = mod
            self.pt = pt
            self.default = self.from_default_entry(self.name, mod, pt, x)
        if not self.value_matches_default(self):
            self.comment = 'Does not match any default values'
        elif self.value_matches_default(self, only_c3=True):
            self.comment = 'Matches default C3 value'

    def clear(self):
        for k in self._default_attr:
            setattr(self, k, None)
        self.name = self.original_name

    def clear_table(self):
        for k in self._table_attr:
            setattr(self, k, None)

    def matches(self, other):
        return (str(self) == str(other))

    @property
    def all_names(self):
        out = [self.fullname]
        for x in self.aliases.values():
            out += [xx for xx in x.all_names if xx not in out]
        for x in self.choices.values():
            if x.fullname == self.fullname:
                continue
            if x.mod == self.mod:
                out += x.all_names
        return out

    @property
    def all_param(self):
        out = [self]
        for x in self.aliases.values():
            out += x.all_param
        return out

    @property
    def fullname(self):
        if self.is_complete:
            return '::'.join([self.mod, self.pt, self.name])
        return self.name

    @property
    def is_complete(self):
        return bool(self.mod and self.pt)

    def add_to_registry(self, existing, complete=False, **kwargs):
        kwargs['existing'] = existing
        if complete and not self.is_complete:
            try:
                self.complete(**kwargs)
            except ParameterError:
                pass
        if ((self.fullname in existing
             and not self.matches(existing[self.fullname]))):
            # print("DUPLICATE", self.fullname)
            other = existing.pop(self.fullname)
            try:
                # print(f"Revising existing {other.fullname}...")
                other.complete(**kwargs)
            except ParameterError:
                pass
            # print("AFTER", other.fullname, other.is_complete)
            existing[other.fullname] = other
            try:
                # print(f"Revising new {self.fullname}...")
                self.complete(**kwargs)
            except ParameterError:
                if not other.is_complete:
                    raise
        # print(f"ADDING {self.fullname}:\n{self}")
        # if kwargs.get('defaults', None):
        #     self.check_against_default(kwargs['defaults'])
        existing[self.fullname] = self
        kwargs.pop('existing')
        for x in self.aliases.values():
            x.add_to_registry(existing, complete=complete, **kwargs)

    def gather_info(self):
        if self.name.startswith("Em_"):
            return
        pattern = f'\"{self.original_name}\"'
        ires_par = search_directory(
            pattern, _param_dir,
            ext='.txt'
        )
        ires_src = search_directory(
            pattern, os.path.join(_source_dir, 'src', '*'),
            ignore_prefix=[
                os.path.join(_source_dir, 'src', 'enum'),
            ]
        )
        if ires_par:
            print('\n=== Parameter files ' + 80 * '=')
            print(write_param(True, ires_par, sort=True))
        if ires_src:
            print('\n=== Source files    ' + 80 * '=')
            print(write_param(True, ires_src, sort=True))
        if ires_par or ires_src:
            print(f'\n{self}')
            pdb.set_trace()

    def try_replace(self, krep, ksub='', mod=None, pt=None,
                    prefix=False, suffix=False, swap_prefix_suffix=False,
                    if_no_choices=False, **kwargs):
        k = self.name
        if ((self.is_complete
             or ((not (prefix or suffix)) and krep not in k)
             or (prefix and (k == krep or not k.startswith(krep)))
             or (suffix and (k == krep or not k.endswith(krep)))
             or (if_no_choices and self.choices))):
            return
        if prefix:
            ktry = ksub + k[len(krep):]
            if swap_prefix_suffix:
                ktry += krep
        elif suffix:
            ktry = k[:(len(k) - len(krep))] + ksub
            if swap_prefix_suffix:
                ktry = krep + ktry
        else:
            ktry = k.replace(krep, ksub)
        if kwargs.get('verbose', False):
            print(f"{k}: Trying {ktry} (mod={mod}, pt={pt})")
        alternative = self.alternative(name=ktry)
        try:
            alternative.complete(mod=mod, pt=pt, **kwargs)
            assert alternative.is_complete
            self.add_choice(alternative)
        except (ParameterNoMatchError, DuplicateParameterError,
                ParameterConflictError):
            pass

    def complete(self, mod=None, pt=None,
                 defaults=None, aliases=None, existing=None,
                 ask_user=True, conversion=None, verbose=False):
        if conversion is not None:
            self.conversion = conversion
        if self.is_complete:
            return
        if defaults is None:
            defaults = read_default_param()
        if aliases is None:
            aliases = self.get_aliases()
        if existing is None:
            existing = {}
        # Aliased
        if self.name in aliases:
            imod, ipt, iname = aliases[self.name].split('::')
            self.add_choice(
                self.alternative_from_default(
                    imod, ipt,
                    defaults[imod][ipt][iname],
                    name=self.name,
                )
            )
        # Check all modules/parameter types
        modlist = list(defaults.keys())
        if self.mod and mod is None:
            modlist.remove(self.mod)
            modlist.insert(0, self.mod)
        if mod is not None:
            modlist = [mod]
        for imod in modlist:
            ptlist = list(defaults[imod].keys())
            if pt is not None:
                ptlist = [pt]
            for ipt in ptlist:
                if self.name in defaults[imod][ipt]:
                    alternative = self.alternative_from_default(
                        imod, ipt,
                        defaults[imod][ipt][self.name]
                    )
                    self.add_choice(alternative)
        # Alternative names
        kws = dict(defaults=defaults, aliases=aliases, existing=existing,
                   verbose=verbose)

        def do_try_replace(*args, **kwargs):
            if mod:
                if kwargs.get('mod', mod) != mod:
                    return
                kwargs.setdefault('mod', mod)
            if pt:
                if kwargs.get('mod', pt) != pt:
                    return
                kwargs.setdefault('pt', pt)
            if kwargs.get('conversion', None):
                if conversion:
                    return
            else:
                kwargs['conversion'] = conversion
            kwargs.update(**kws)
            self.try_replace(*args, **kwargs)

        do_try_replace('Cyt f', 'cytf1')
        do_try_replace('cyt f', 'cytf1')
        do_try_replace('-', 'n')
        do_try_replace('+', 'p')
        do_try_replace('bf', mod='BF')
        do_try_replace('BF', mod='BF', prefix=True)
        do_try_replace('ra', mod='RuACT')
        do_try_replace('GADPH', 'GAPDH')
        do_try_replace('RuACT', 'RubACT')
        do_try_replace('ADPGPP', 'ATPGPP')
        do_try_replace('K+', 'K')
        do_try_replace('Mg2+', 'Mg')
        do_try_replace('Cl-', 'Cl')
        # do_try_replace('AU', 'A_U')
        # do_try_replace('UA', 'U_A')
        do_try_replace('red', 'r', suffix=True)
        do_try_replace('r', mod='RROEA', pt='COND', suffix=True)
        do_try_replace('Activase', 'RuACT')
        # do_try_replace('Rubisco', 'RuBP')
        do_try_replace('Ke', 'KE', prefix=True)
        do_try_replace('Vm', 'V', prefix=True, mod='PS')
        do_try_replace('Vm', 'V', prefix=True, mod='PR')
        do_try_replace('Vm', 'V', prefix=True, mod='SUCS')
        do_try_replace('ox', 'o', suffix=True)
        do_try_replace('T', suffix=True, swap_prefix_suffix=True)
        # do_try_replace('o', '0', suffix=True)
        do_try_replace('k', 'K', prefix=True, if_no_choices=True)
        do_try_replace('a', '1', suffix=True, if_no_choices=True)
        do_try_replace('b', '2', suffix=True, if_no_choices=True)
        do_try_replace('Cytc1', 'Cytf')
        do_try_replace('o', 'T', suffix=True,
                       mod='RROEA', pt='POOL',
                       conversion='oxidized2total', **kws)
        do_try_replace('o', '', suffix=True,
                       mod='RROEA', pt='POOL',
                       conversion='oxidized2total', **kws)
        do_try_replace('Hf', 'PH', prefix=True,
                       conversion='H2pH', **kws)
        self.update_from_choices(existing=existing, ask_user=ask_user)
        if existing and self.fullname in existing:
            # Exclue existing values from choices in update_from_choices?
            self.resolve_conflict(
                existing[self.fullname], self, ask_user=ask_user,
            )
        if not self.is_complete:
            raise ParameterNoMatchError(
                f'Could not locate parameter \"{self.fullname}\" '
                f'(mod={mod}, pt={pt}):\n\n{self}'
            )
        if conversion:
            self.conversion = conversion

    @classmethod
    def get_aliases(cls):
        aliases = get_matlab_aliases(reverse=True,
                                     strip_prefix='keys')
        aliases['T'] = 'ALL::VARS::Tp'
        return aliases


class zhu2012(SubTask):

    @classmethod
    def adjust_args(cls, args):
        if args.split_tables:
            args.complete_param = True
        if not args.exclude_param:
            args.exclude_param = []
        args.exclude_mod = []  # 'RROEA']
        args.exclude_pt = []
        cls.prefix_path_args(args, ['tables_file', 'table_base',
                                    'param_file'],
                             prefix=_zhu2012_dir)
        super(zhu2012, cls).adjust_args(args)

    def run_commands(self, args, **kwargs):
        defaults = read_default_param()
        aliases = Parameter.get_aliases()
        if args.split_tables:
            with open(args.tables_file, 'r') as fd:
                contents = fd.read()
            split_on = 'Table II.'
            tables = [split_on + x for x in contents.split(split_on)
                      if x]
            for i in range(len(tables) - 1):
                if tables[i].endswith('"'):
                    tables[i] = tables[i][:-1]
                    tables[i + 1] = '"' + tables[i + 1]
            for i, table in enumerate(tables):
                fname = f'{args.table_base}{i}.csv'
                with open(fname, 'w') as fd:
                    fd.write(table)
                if i == 5:
                    self.convert_enzyme_table_file(fname)
            if os.path.isfile(args.param_file):
                os.remove(args.param_file)
        tables = sorted(glob.glob(f'{args.table_base}*.csv'))
        if not tables:
            args.split_tables = True
            return self.run_commands(args, **kwargs)
        if args.complete_param:
            existing = ParameterSet()
            if os.path.isfile(args.param_file):
                os.remove(args.param_file)
            for ftable in tables:
                self.read_table_file(
                    ftable, defaults=defaults, aliases=aliases,
                    existing=existing, complete=True,
                    inspect_missing=args.inspect_missing,
                    inspect_param=args.inspect_param,
                    ignore_existing_names=args.ignore_existing_names,
                    verbose=args.verbose,
                )
            args.inspect_missing = False
            args.inspect_param = []
        if args.make_param:
            existing = ParameterSet()
            for ftable in tables:
                self.read_table_file(
                    ftable, defaults=defaults, aliases=aliases,
                    existing=existing,
                    inspect_missing=args.inspect_missing,
                    inspect_param=args.inspect_param,
                    verbose=args.verbose,
                )
            for k, v in existing.items():
                v.finalize(defaults=defaults, existing=existing,
                           aliases=aliases, excluded=args.exclude_param,
                           min_diff=args.min_diff)
                if ((v.mod in args.exclude_mod
                     or v.pt in args.exclude_pt)):
                    args.exclude_param.append(v.fullname)
            include = [
                'SUCS::MOD::KE61',
                'RuACT::RC::k7',
            ]
            args.exclude_param += [
                'RuACT::COND::ECMR',
                'FIBF::RC::kdm0',
            ]
            for k in include:
                if k in args.exclude_param:
                    args.exclude_param.remove(k)
            write_param(args.param_file, existing, sort=args.sort_param,
                        comment_incomplete=(not args.include_missing),
                        exclude_param=args.exclude_param)
            print(f"WROTE {args.param_file}")

    @classmethod
    def read_table_file(cls, ftable, defaults=None, aliases=None,
                        existing=None, complete=False,
                        incremental_update=False,
                        ignore_existing_names=False,
                        inspect_missing=False, inspect_param=None,
                        verbose=False):
        if defaults is None:
            defaults = read_default_param()
        if aliases is None:
            aliases = Parameter.get_aliases()
        if existing is None:
            existing = ParameterSet()
        if inspect_param is None:
            inspect_param = []
        df, title = read_param_table(ftable)
        if 'Value' not in df:
            return
        index2param = {}
        # First just add parameters so that explicit parameters are
        # used first
        for index, row in df.iterrows():
            try:
                p = Parameter.from_table_entry(
                    row, title=title,
                    ignore_existing_names=ignore_existing_names,
                )
            except ParameterNanError:
                continue
            if p.is_complete:
                p.add_to_registry(
                    existing, defaults=defaults, aliases=aliases,
                    verbose=verbose,
                )
            index2param[index] = p
        for index, p in index2param.items():
            if not p.is_complete:
                p.add_to_registry(existing, complete=complete,
                                  defaults=defaults, aliases=aliases,
                                  verbose=verbose)
            if not p.is_complete:
                if inspect_missing or p.name in inspect_param:
                    p.gather_info()
                continue
            elif ((p.original_name in inspect_param
                   or p.name in inspect_param)):
                p.gather_info()
            updated = p.update_row(df, index)
            if updated and complete and incremental_update:
                write_param_table(ftable, df, title=title)
        if complete:
            write_param_table(ftable, df, title=title)
        return existing

    @classmethod
    def convert_enzyme_table_file(cls, ftable, df=None, title=None):
        ftable_out = '_split'.join(os.path.splitext(ftable))
        if df is None:
            df, title = read_param_table(ftable)
        assert 'Molecular Weight (D)' in df
        col2prefix = {
            'Molecular Weight (D)': 'mw_',
            'Catalytic number (s-1)': 'SA_',
        }
        col2units = {
            k: k.split('(')[-1].split(')')[0]
            for k in col2prefix.keys()
        }
        out = OrderedDict()
        for index, row in df.iterrows():
            symbol = row['Symbol']
            if not isinstance(symbol, str):
                continue
            for col, prefix in col2prefix.items():
                iout = {
                    'Name': prefix + symbol,
                    'Value': np.float64(row[col]),
                    'Units': col2units[col],
                    'C++ Parameter': np.nan,
                }
                if iout['Name'] == 'mw_GAPDH':
                    iout['C++ Parameter'] = 'RROEA::POOL::mw_GAPDH'
                for k in ['Reference', 'Description']:
                    iout[k] = row[k]
                for k, v in iout.items():
                    out.setdefault(k, [])
                    out[k].append(v)
        write_param_table(ftable_out, out, title=title)
        return ftable_out


class ephoto(BuildSubTask):

    direct_args = ['begintime', 'stoptime', 'stepsize',
                   'abstol', 'reltol']
    _output_ftypes = [
        'output_file', 'output_param_base',
        'plot_file', 'find_inflections', 'plot_trace_diffs',
        'output_diff',
    ]
    _input_ftypes = [
        'enzyme_file', 'grn_file',
        'evn_file', 'atpcost_file',
        'iterations_file',
    ]
    _file_defaults = {
        'enzyme_file': 'InputEnzyme.txt',
        'grn_file': 'InputGRNC.txt',
        'evn_file': 'InputEvn.txt',
        'atpcost_file': 'InputATPCost.txt',
    }

    @classmethod
    def adjust_args(cls, args, suffix_kws=None, add_output_ftypes=[]):
        if suffix_kws is None:
            suffix_kws = {}
        if args.match_param:
            if args.output_suffix is None:
                args.output_suffix = True
            if not args.param:
                args.param = {}
            for k in ['evn_file', 'grn_file', 'enzyme_file',
                      'atpcost_file']:
                setattr(args, k, None)
            if (((not args.light_profile)
                 and args.match_param in _light_profiles)):
                args.light_profile = args.match_param
            if (((args.match_figure in [True, None])
                 and args.match_param in _figures)):
                args.match_figure = args.match_param
            cls.match_param(args.match_param, args)
        if args.light_profile:
            create_iterations.adjust_args(args)
        if isinstance(args.language, list):
            args.all_languages = [args.language[0]]
            for x in args.language[1:]:
                if x == 'all':
                    args.all_languages += [
                        xx for xx in SubTask._languages
                        if xx not in args.all_languages
                    ]
                elif x not in args.all_languages:
                    args.all_languages.append(x)
            args.language = args.language[0]
        if args.language == 'matlab' and not args.matlab:
            args.matlab = True
        if args.matlab is True:
            args.matlab = find_matlab(required=True)
            assert args.language in [None, 'matlab']
            args.language = 'matlab'
        if not args.language:
            args.language = 'cpp'
        if not getattr(args, 'all_languages', None):
            args.all_languages = [args.language]
        if 'matlab' in args.all_languages and len(args.all_languages) > 1:
            args.make_equivalent_to_matlab = True
        if not args.driver:
            args.driver = list(SubTask._driver_map.keys())
        if isinstance(args.driver, list):
            args.all_drivers = [args.driver[0]]
            for x in args.driver:
                if x == 0:
                    args.all_drivers += [
                        xx for xx in SubTask._driver_map.keys()
                        if xx not in args.all_drivers
                    ]
                elif x not in args.all_drivers:
                    args.all_drivers.append(x)
            args.driver = args.all_drivers[0]
        if not getattr(args, 'all_drivers', None):
            args.all_drivers = [args.driver]
        if args.dont_run_language is None:
            args.dont_run_language = []
        if isinstance(args.param, list):
            args.param = OrderedDict(*args.param)
        if args.use_zaks_npq:
            if args.param is None:
                args.param = {}
            args.param.update(**{
                'ALL::VARS::UseZaksNPQ': 1,
                'XanCycle::RC::psbsQ_converRate': 4.0e-2,
                'XanCycle::RC::Fpsbs': 0.6,
                'ALL::VARS::GRNC': 0,
            })
            # TODO: Check if Vx, Ax, Zx in file
            if not (args.steady_state_start or args.evn_file):
                args.param.update(**{
                    'XanCycle::COND::Vx': 0.7,
                    'XanCycle::COND::Ax': 0.2,
                    'XanCycle::COND::Zx': 0.1,
                    # 'SUCS::MOD::KE61': 1.2e7,
                    # 'PR::MOD::PGA': 0.0,
                    # 'PS::MOD::PS_C_CN': 1.0,
                    # 'ALL::VARS::Pi': 0.967608,
                })
        if 'matlab' in args.all_languages:
            for ftype in ['grn_file', 'evn_file']:
                if getattr(args, ftype, None) is None:
                    setattr(args, ftype, '_MATLAB'.join(
                        os.path.splitext(cls._file_defaults[ftype])))
        for ftype, v in cls._file_defaults.items():
            if getattr(args, ftype, None) is None:
                setattr(args, ftype, v)
        if args.output_diff is True:
            args.output_diff = f'{args.language}.diff'
        if args.plot_file and args.output_param < 3:
            args.output_param = 3
        if args.compare and args.output_param < 2:
            args.output_param = 2
        if not isinstance(args.output_param_base, str):
            args.output_param_base = 'param_'
        if args.plot_file is True:
            args.plot_file = args.output_param_base + 'trace.png'
        cls.prefix_path_args(
            args, ['matlab_repo'], prefix=os.getcwd())
        cls.prefix_path_args(args, ['input_dir', 'output_dir',
                                    'generate_matlab_script'])
        cls.prefix_path_args(args, cls._input_ftypes,
                             prefix=args.input_dir)
        cls.prefix_path_args(args,
                             cls._output_ftypes + add_output_ftypes,
                             prefix=args.output_dir)
        if args.output_suffix is None:
            args.output_suffix = True
        if args.output_suffix:
            if args.output_suffix is True:
                args.output_suffix = cls.generate_output_suffix(
                    args, **suffix_kws)
            if isinstance(args.output_suffix, str):
                cls.suffix_path_args(
                    args, cls._output_ftypes + add_output_ftypes,
                    args.output_suffix)
        args.output_param_first = args.output_param_base + 'init.txt'
        args.output_param_final = args.output_param_base + 'last.txt'
        args.output_param_trace = args.output_param_base + 'trace.txt'
        args.output_param_steps = args.output_param_base + 'step*.txt'
        if not os.path.isdir(args.output_dir):
            os.mkdir(args.output_dir)
        if args.dont_run or ('cpp' not in args.all_languages
                             and 'python' not in args.all_languages):
            args.dont_build = True
        if 'python' in args.all_languages:
            if len(args.all_languages) == 1:
                args.only_python = True
            else:
                args.with_python = True
        if args.language == 'matlab':
            instrument_matlab.adjust_args(args)
        super(ephoto, cls).adjust_args(args)

    @classmethod
    def incorporate_evn_file(cls, args, from_output=False,
                             for_matlab=False):
        if not (args.param or args.evn_file):
            return
        exclusive_param = [
            ('ALL::VARS::O2_cond', 'ALL::VARS::O2',
             'O2_cond', 'O2'),
            ('ALL::VARS::CO2_cond', 'ALL::VARS::CO2_in',
             'CO2_cond', 'CO2_in', 'CO2'),
            ('ALL::VARS::TestLi',  'ALL::VARS::TestLi_Wps',
             'ALL::VARS::GLight',
             'TestLi', 'TestLi_Wps', 'PFD', 'GLight'),
        ]
        if args.param is None:
            args.param = {}
        else:
            ParameterSet.normalize_dict(args.param)
        if args.evn_file:
            if not hasattr(args, 'evn_file_subset'):
                args.evn_file_subset = None
            param = read_param(args.evn_file)
            ParameterSet.normalize_dict(param)
            if isinstance(args.evn_file_subset, list):
                param = {k: param[k] for k in args.evn_file_subset}
            for v in exclusive_param:
                if any(vv in args.param for vv in v):
                    for vv in v:
                        param.pop(vv, None)
                for i, vv in enumerate(v):
                    if vv in param:
                        for valt in v[(i + 1):]:
                            param.pop(valt, None)
                        break
            args.param = dict(param, **args.param)
            args.base_evn_file = args.evn_file
            args.base_evn_file_subset = args.evn_file_subset
            args.evn_file = None
            args.evn_file_subset = None
        if from_output:
            transfers = {
                'ALL::VARS::PS2BF_Pi': [
                    'ALL::VARS::Pi',
                    'BF::MOD::_Pi',
                ],
            }
            for k, v in transfers.items():
                if k in args.param:
                    for vv in v:
                        args.param[vv] = args.param[k]
        if for_matlab:
            args.param = convert_matlab_param_task.convert_to_matlab(
                args.param, use_zaks_npq=args.use_zaks_npq)
        else:
            args.param = convert_matlab_param_task.convert_from_matlab(
                args.param, use_zaks_npq=args.use_zaks_npq)

    @classmethod
    def match_param(cls, name, args):
        assert not args.evn_file
        if name.startswith('Zhu2012_'):
            args.make_equivalent_to_matlab = True
            if name == 'Zhu2012_MATLAB':
                args.evn_file = 'InputEvn_MATLAB_master.txt'
                cls.prefix_path_args(args, ['evn_file'],
                                     prefix=_data_dir)
            else:
                if name == 'Zhu2012_REGEN':
                    subprocess.run(
                        [sys.executable,
                         os.path.join(_scripts_dir, 'devtasks.py'),
                         'zhu2012', '--make-param', '--split-tables'],
                        cwd=_source_dir,
                        check=True,
                    )
                args.evn_file = 'Zhu2012_param.txt'
                cls.prefix_path_args(args, ['evn_file'],
                                     prefix=_zhu2012_dir)
            args.param.update(**{
                'ALL::VARS::O2_cond': 0.210,  # umol mol-1, 210 mmol mol-1
                'ALL::VARS::CO2_cond': 280,   # umol mol-1
                'ALL::VARS::Tp': 25,          # C
                'ALL::VARS::GP': 1,
                'PR::MOD::RUBISCOMETHOD': 2,
                # 'ALL::VARS::ProteinTotalRatio': 0.973,
                # 'ALL::VARS::GRNC': 1,
            })
        elif name.startswith('Zaks2012_'):
            args.use_zaks_npq = True
            args.param.update(**{
                'ALL::VARS::UseZaksNPQ': 1,
                'XanCycle::RC::psbsQ_converRate': 4.0e-2,
                'XanCycle::RC::Fpsbs': 1,
                'ALL::VARS::GRNC': 0,
                'XanCycle::COND::Vx': 0.7,
                'XanCycle::COND::Ax': 0.2,
                'XanCycle::COND::Zx': 0.1,
            })
        elif name == 'NPQ-explore':
            args.driver = 2
            args.use_zaks_npq = True
            # args.match_param_to_matlab = True
            args.param.update(**{
                'ALL::VARS::O2_cond': 0.210,  # umol mol-1, 210 mmol mol-1
                'ALL::VARS::CO2_cond': 400,   # umol mol-1
                'ALL::VARS::Tp': 25,          # C
                # 'ALL::VARS::GP': 1,
                # 'PR::MOD::RUBISCOMETHOD': 2,
            })
            args.evn_file = 'InitParamNPQ_MATLAB.txt'
            cls.prefix_path_args(args, ['evn_file'],
                                 prefix=_data_dir)
            args.evn_file_subset = [
                # These are unused without --useC3
                'PR::MOD::PrV112',
                'PR::MOD::PrV113',
                'PR::MOD::PrV121',
                'PR::MOD::PrV122',
                'PR::MOD::PrV123',
                'PR::MOD::PrV124',
                'PR::MOD::PrV131',
                'PR::MOD::Q10_112',
                'PR::MOD::Q10_113',
                'PR::MOD::Q10_121',
                'PR::MOD::Q10_122',
                'PR::MOD::Q10_123',
                'PR::MOD::Q10_124',
                'PR::MOD::Q10_131',
                'PS::MOD::KM71',
                'PS::MOD::KM72',
                'PS::MOD::KM73',
                'PS::MOD::KM74',
                'PS::MOD::PsV1',
                'PS::MOD::PsV2',
                'PS::MOD::PsV3',
                'PS::MOD::PsV5',
                'PS::MOD::PsV7',
                'PS::MOD::PsV8',
                'PS::MOD::PsV10',
                'PS::MOD::PsV23',
                'PS::MOD::Q10_1',
                'PS::MOD::Q10_10',
                'PS::MOD::Q10_13',
                'PS::MOD::Q10_2',
                'PS::MOD::Q10_23',
                'PS::MOD::Q10_3',
                'PS::MOD::Q10_5',
                'PS::MOD::Q10_6',
                'PS::MOD::Q10_7',
                'PS::MOD::Q10_8',
                'PS::MOD::Q10_9',
                'PS::MOD::Vf_T1',
                'PS::MOD::Vf_T13',
                'PS::MOD::Vf_T2',
                'PS::MOD::Vf_T23',
                'PS::MOD::Vf_T3',
                'PS::MOD::Vf_T5',
                'PS::MOD::Vf_T6',
                'PS::MOD::Vf_T9',
                'PS::MOD::Vfactor1',
                'PS::MOD::Vfactor13',
                'PS::MOD::Vfactor2',
                'PS::MOD::Vfactor23',
                'PS::MOD::Vfactor3',
                'PS::MOD::Vfactor5',
                'PS::MOD::Vfactor7',
                'PS::MOD::V10',
                'SUCS::MOD::Q10_51',
                'SUCS::MOD::Q10_52',
                'SUCS::MOD::Q10_55',
                'SUCS::MOD::Q10_56',
                'SUCS::MOD::Q10_57',
                'SUCS::MOD::Q10_58',
                'SUCS::MOD::SUCSV51',
                'SUCS::MOD::SUCSV52',
                'SUCS::MOD::SUCSV55',
                'SUCS::MOD::SUCSV56',
                'SUCS::MOD::SUCSV57',
                'SUCS::MOD::SUCSV58',
                'SUCS::MOD::V60',
                'SUCS::MOD::V61',
                'SUCS::MOD::Vmatpf',
                'PR::MOD::KI124',
                'PR::MOD::KI1312',
                'PR::MOD::KM1312',
                'PS::MOD::KE10',
                'PS::MOD::KM312',
                'SUCS::MOD::Km601',
                'SUCS::MOD::Km602',
                'SUCS::MOD::Km603',
                'SUCS::MOD::Km604',
                'SUCS::MOD::KE60',
                'SUCS::MOD::KI592',
                'SUCS::MOD::Km592',
                'PS::MOD::beta',
                'PS::MOD::c_c',
                'PS::MOD::c_o',
                'PS::MOD::dHa_c',
                'PS::MOD::dHa_o',
                # # These are different parameters
                # 'BF::RC::KBl',
                # 'BF::RC::KBs',
                'RuACT::RC::k7',
                # 'SUCS::MOD::ADPc',
                # 'SUCS::MOD::ATPc',
                # # These are initial conditions
                'BF::COND::Q',
            ]
        # TODO: Verify that parameters should be set
        if args.use_zaks_npq:
            args.param.update(**{
                'ALL::VARS::UseZaksNPQ': 1,
                'XanCycle::RC::psbsQ_converRate': 4.0e-2,
                'XanCycle::RC::Fpsbs': 1,
                'ALL::VARS::GRNC': 0,
            })
            if not (args.steady_state_start or args.evn_file):
                args.param.update(**{
                    'XanCycle::COND::Vx': 0.7,
                    'XanCycle::COND::Ax': 0.2,
                    'XanCycle::COND::Zx': 0.1,
                    # 'SUCS::MOD::KE61': 1.2e7,
                    'PR::MOD::PGA': 0.0,
                    'PS::MOD::PS_C_CN': 1.0,
                    # 'ALL::VARS::Pi': 0.967608,
                })

    @classmethod
    def record_last_param(cls, args):
        if not args.output_param:
            return
        param_files = sorted(glob.glob(args.output_param_steps))
        first_file = None
        final_file = None
        if os.path.isfile(args.output_param_first):
            first_file = (np.nan, args.output_param_first)
        if os.path.isfile(args.output_param_final):
            final_file = (np.nan, args.output_param_final)
        if first_file and final_file:
            return
        # print("PARAM_FILES")
        # pprint.pprint(param_files)
        if not (param_files or first_file or final_file):
            warnings.warn(f"No parameter files available matching \""
                          f"{args.output_param_steps}\"")
            return
        times = []
        for x in param_files:
            t = float(x.rsplit('_step', 1)[-1].split('.txt')[0])
            times.append(t)
            if first_file is None or t < first_file[0]:
                first_file = (t, x)
            if final_file is None or t > final_file[0]:
                final_file = (t, x)
        times = sorted(times)
        if first_file and first_file[1] != args.output_param_first:
            shutil.copy2(first_file[1], args.output_param_first)
        if final_file and final_file[1] != args.output_param_final:
            shutil.copy2(final_file[1], args.output_param_final)

    @classmethod
    def extract_param(cls, pattern, names=None, fname=None):
        param_files = sorted(glob.glob(pattern))
        if fname and not param_files:
            return read_param_table(fname, no_title=True)
        assert param_files
        variables = None
        for x in param_files:
            t = float(x.rsplit('_step', 1)[-1].split('.txt')[0])
            data = read_param(x)
            if variables is None:
                if names is None:
                    names = list(sorted(data.keys()))
                variables = OrderedDict([('time', [])])
                for v in names:
                    variables[v] = []
            variables['time'].append(t)
            if len(variables['time']) > 1:
                assert variables['time'][-1] > variables['time'][-2]
            for v in names:
                if v in data:
                    variables[v].append(data[v])
                else:
                    warnings.warn(f'Missing field \"{v}\" for t = {t}')
                    variables[v].append(np.nan)
        variables = pd.DataFrame(variables).sort_values('time')
        if fname:
            write_param_table(fname, variables, title=False)
        return variables

    @classmethod
    def read_trace(cls, args):
        if os.path.isfile(args.output_param_trace):
            return read_param_table(args.output_param_trace, no_title=True)
        if not args.output_param_steps:
            raise ValueError(f'Trace file does not exist: '
                             f'{args.output_param_trace}')
        return cls.extract_param(args.output_param_steps,
                                 fname=args.output_param_trace)

    @classmethod
    def get_command_matlab(cls, args, kwargs, ephoto_args=None):
        assert not ephoto_args
        if ephoto_args is None:
            ephoto_args = []
        ephoto_args += [
            str(args.driver),
            f'\'{args.matlab_repo}\'',
        ]
        for k in cls.direct_args:
            v = getattr(args, k, None)
            if v is None:
                continue
            if isinstance(v, str):
                v = f'\'{v}\''
            ephoto_args += [f'{k}={v}']
        if args.grn_file:
            ephoto_args += [f'GRNFile=\'{args.grn_file}\'']
        if args.atpcost_file:
            ephoto_args += [f'ATPCostFile=\'{args.atpcost_file}\'']
        if args.enzyme_file:
            ephoto_args += [f'EnzymeFile=\'{args.enzyme_file}\'']
        if args.evn_file:
            ephoto_args += [f'EnvFile=\'{args.evn_file}\'']
        if args.output_file:
            ephoto_args += [f'OutputFile=\'{args.output_file}\'']
        if args.output_param_base:
            ephoto_args += [f'OutputParamBase=\'{args.output_param_base}\'']
        if args.output_param:
            ephoto_args += [f'OutputParamLevel={args.output_param}']
        if args.iterations_file:
            ephoto_args += [f'IterationsFile=\'{args.iterations_file}\'']
        # if args.useC3:
        #     ephoto_args += ['useC3=true']
        cmd = [
            args.matlab,
            # Flags when running matlab outside batch mode
            # '-nodisplay', '-nosplash', '-nodesktop',
            # '-nojvm',
        ]
        if args.generate_matlab_script:
            cls.generate_matlab_script(args, ephoto_args)
            script_dir, script_name = os.path.split(
                args.generate_matlab_script)
            cmd += [
                '-batch', os.path.splitext(script_name)[0],
            ]
            kwargs['cwd'] = script_dir
        else:
            cmd += [
                '-batch',
                'ePhotosynthesis(' + ",".join(ephoto_args) + ');'
            ]
            kwargs['cwd'] = args.matlab_utils
        cmd = [' '.join(cmd)]
        return cmd

    @classmethod
    def get_command_cpp(cls, args, kwargs, ephoto_args=None):
        if ephoto_args is None:
            ephoto_args = []
        execFile = os.path.join(args.build_dir, 'ePhoto')
        for k in cls.direct_args:
            v = getattr(args, k, None)
            if v is None:
                continue
            ephoto_args += [f'--{k}', str(v)]
        if args.grn_file:
            ephoto_args += ['--grn', args.grn_file]
        if args.atpcost_file:
            ephoto_args += ['--atpcost', args.atpcost_file]
        if args.enzyme_file:
            ephoto_args += ['--enzyme', args.enzyme_file]
        if args.evn_file:
            ephoto_args += ['--evn', args.evn_file]
        if args.output_file:
            ephoto_args += ['--output', args.output_file]
        if args.output_param_base:
            ephoto_args += ['--outputParamBase', args.output_param_base]
        if args.output_param:
            ephoto_args += ['--outputParam', str(args.output_param)]
        if args.iterations_file:
            ephoto_args += ['--iterations', args.iterations_file]
        if args.useC3:
            ephoto_args += ['--c3']
        cmds = [
            f'{execFile} -d {args.driver} '
            f'{" ".join(ephoto_args)}',
            f'cat {args.output_file}'
        ]
        return cmds

    @classmethod
    def get_command_python(cls, args, kwargs, ephoto_args):
        assert not ephoto_args
        if ephoto_args is None:
            ephoto_args = []
        ephoto_args += [f"\"{cls._driver_map[args.driver]}\""]
        for k in cls.direct_args:
            v = getattr(args, k, None)
            if v is None:
                continue
            if isinstance(v, str):
                v = f'\"{v}\"'
            ephoto_args += [f'{k}={v}']
        if args.grn_file:
            ephoto_args += [f'grnFile=\"{args.grn_file}\"']
        if args.atpcost_file:
            ephoto_args += [f'atpcostFile=\"{args.atpcost_file}\"']
        if args.enzyme_file:
            ephoto_args += [f'enzymeFile=\"{args.enzyme_file}\"']
        if args.evn_file:
            ephoto_args += [f'evnFile=\"{args.evn_file}\"']
        if args.output_file:
            ephoto_args += [f'outputFile=\"{args.output_file}\"']
        if args.output_param_base:
            ephoto_args += [f'outputParamBase=\"{args.output_param_base}\"']
        if args.output_param:
            ephoto_args += [f'outputParam={args.output_param}']
        if args.iterations_file:
            ephoto_args += [f'iterationsFile=\"{args.iterations_file}\"']
        # if args.useC3:
        #     ephoto_args += ['useC3=true']
        cmd = [
            sys.executable, '-c',
        ]
        ephoto_args = ", ".join(ephoto_args)
        cmd += [
            f'\'from ePhotosynthesis import run_simulation; '
            f'run_simulation({ephoto_args})\''
        ]
        # kwargs['cwd'] = args.build_dir
        return [cmd]

    def run_commands(self, args, cmds=None, ephoto_args=None, **kwargs):
        args.iterations_data = None
        if args.light_profile:
            args.iterations_data = create_iterations.create_light_profile(args)
            self._generated_files += [
                args.iterations_file
            ]
            if args.steady_state_start:
                args.steady_state_start_suffix = (
                    f"steady-state_{args.iterations_data['GLight'][0]}"
                )
        if args.iterations_file and args.iterations_data is None:
            args.iterations_data = create_iterations.read_iterations(
                args.iterations_file)
        if args.iterations_data:
            args.stoptime = max(args.iterations_data['time'])
            for k, v in args.iterations_data.items():
                if k != 'time':
                    args.param[k] = v[0]
        if args.steady_state_start:
            assert args.iterations_file  # Why else would this be run
            args.evn_file = ephoto.run_steady_state(
                args,
                suffix=getattr(args, 'stead_state_start_suffix', None),
                overwrite=(args.steady_state_start == 'overwrite'),
            )
            args.param = {}
            ephoto.incorporate_evn_file(args, from_output=True)
        if cmds is None and (args.driver == 0
                             or len(args.all_drivers) > 1
                             or len(args.all_languages) > 1):
            super(ephoto, self).run_commands(args, cmds=[], **kwargs)
            update_args = {
                'dont_build': True,
                'rebuild': False,
                'output_diff': False,
            }
            self.iterate(self.run_commands, args,
                         update_args=update_args, **kwargs)
            if args.plot_file or args.compare:
                self._generated_files += self.normalize_param(
                    *args.iteration_args.values())
            if args.plot_file:
                args.result_file = [
                    x.output_param_trace
                    for x in args.iteration_args.values()
                ]
                args.plot_labels = list(args.iteration_args.keys())
                analyze_trace(args)
            if args.compare:
                arglist = list(args.iteration_args.values())
                arglist[0].output_diff = args.output_diff
                self.diff(*arglist)
            return
        if ephoto_args is None:
            ephoto_args = []
        if cmds is None and not args.dont_run:
            if args.match_param_to_matlab:
                args.evn_file = self.run_language_version(
                    "matlab", args,
                    overwrite=(
                        args.match_param_to_matlab == 'overwrite'
                    ),
                )
                args.param = {}
                self.incorporate_evn_file(args, from_output=True)
            self.incorporate_evn_file(
                args, for_matlab=(args.language == "matlab"))
            if args.param:
                assert not args.evn_file
                args.evn_file = os.path.join(
                    args.input_dir,
                    f'GeneratedEvn_{args.language.upper()}.txt'
                )
                write_param(args.evn_file, args.param)
                self._generated_files.append(args.evn_file)
            self._generated_files += [args.output_file]
            if args.output_param_base and not args.preserve_output_param:
                self._generated_files += [
                    args.output_param_first,
                    args.output_param_final,
                    args.output_param_trace,
                ]
            if args.output_param:
                if args.output_param >= 3:
                    self._generated_files += [args.output_param_steps]
            if args.language == 'matlab' and not args.dont_patch_matlab:
                instrument_matlab.instrument_matlab(args)
            cmds = getattr(self, f'get_command_{args.language}')(
                args, kwargs, ephoto_args=ephoto_args)
            if args.language == 'matlab' and args.generate_matlab_script:
                self._generated_files += [args.generate_matlab_script]
        try:
            return super(ephoto, self).run_commands(
                args, cmds=cmds, **kwargs)
        finally:
            if ((args.language == 'matlab'
                 and not (args.dont_patch_matlab
                          or args.preserve_patch_matlab))):
                args.remove_patch = True
                instrument_matlab.instrument_matlab(args)
            self.record_last_param(args)
            if ((args.plot_file or args.find_inflections
                 or args.plot_trace_diffs)):
                args.plot_labels = None
                args.result_file = [args.output_param_trace]
                analyze_trace(args)

    @classmethod
    def iterate(cls, func, args0, drivers=None, languages=None,
                update_args=None, **kwargs):
        if drivers is None:
            if args0.driver == 0:
                drivers = [i + 1 for i in range(len(cls._drivers))]
            else:
                drivers = args0.all_drivers
        if languages is None:
            languages = args0.all_languages
        if update_args is None:
            update_args = {}
        if not hasattr(args0, 'iteration_args'):
            args0.iteration_args = {}
        for driver in drivers:
            for language in languages:
                args = copy.deepcopy(args0)
                for k, v in update_args.items():
                    setattr(args, k, v)
                args.driver = driver
                args.language = language
                args.all_languages = []
                args.all_drivers = []
                args.dont_run_language = []
                args.compare = False
                if language in args0.dont_run_language:
                    args.dont_run = True
                if language == 'matlab':
                    if not args.matlab:
                        args.matlab = find_matlab(required=True)
                else:
                    args.matlab = False
                args.output_dir = os.path.join(
                    args0.output_dir, f'output_{language.upper()}')
                cls.reset_path_args(args, cls._output_ftypes,
                                    set_to_base='original')
                args.find_inflections = False
                args.plot_trace_diffs = False
                args.output_suffix = True
                cls.adjust_args(args)
                cls.assert_no_overlap(args, args0)
                args0.iteration_args[args.output_suffix.strip('_')] = args
                func(args, **kwargs)

    @classmethod
    def generate_matlab_script(cls, args, ephoto_args):
        lines = [
            f"addpath(\"{args.matlab_utils}\")"
            f"Arate = ePhotosynthesis({', '.join(ephoto_args)})"
        ]
        contents = '\n'.join(lines)
        print(f'{80*"="}\n'
              f'Writing script to {args.generate_matlab_script}:\n'
              f'{80*"-"}\n{contents}\n{80*"="}')
        with open(args.generate_matlab_script, 'w') as fd:
            fd.write(contents)

    @classmethod
    def generate_output_suffix(cls, args, extra_suffix='',
                               ignore_language=False):
        out = extra_suffix
        if args.match_param:
            out += f'_{args.match_param}'
        if args.light_profile and args.light_profile != args.match_param:
            out += create_iterations.generate_output_suffix(args)
        if len(args.all_drivers) == 1 and args.driver != 0:
            out += '_' + cls._driver_map[args.driver]
        if len(args.all_languages) == 1 and not ignore_language:
            out += '_' + args.language.upper()
        if args.compare:
            out += '_COMPARE'
            if len(args.all_drivers) > 1:
                out += '_DRIVERS'
            if len(args.all_languages) > 1:
                out += '_LANGUAGES'
        if ((args.use_zaks_npq
             and not (args.light_profile
                      and args.light_profile.startswith('Zaks')))):
            out += '_ZaksNPQ'
        if args.steady_state_start:
            assert not args.light_profile.endswith('steady-state')
            out += '_steady'
        if getattr(args, 'bfzero', False):
            out += '_BFZero'
        return out

    @classmethod
    def run_language_version(cls, language, args0, suffix=None,
                             overwrite=False):
        assert args0.language != language
        args = copy.deepcopy(args0)
        args.language = language
        if language == 'matlab':
            args.match_param_to_matlab = False
        if args.output_param < 1:
            args.output_param = 1
        cls.reset_path_args(args, cls._output_ftypes, value=True)
        if suffix is not None:
            args.output_suffix = cls.generate_output_suffix(args, suffix)
        cls.adjust_args(args)
        cls.assert_no_overlap(args, args0)
        if (not os.path.isfile(args.output_param_first)) or overwrite:
            cls(args)
        print(f"CREATED {language.upper()} STATE IN "
              f"{args.output_param_first}")
        return args.output_param_first

    @classmethod
    def run_steady_state(cls, args0, values=None, suffix=None,
                         overwrite=False):
        args = copy.deepcopy(args0)
        args.steady_state_start = False
        args.output_suffix = True
        args.preserve_output_param = True
        args.iterations_file = None
        args.light_profile = None
        if args.output_param < 2:
            args.output_param = 2
        cls.reset_path_args(args, cls._output_ftypes, value=True)
        if not args.param:
            args.param = {}
        if values:
            args.param.update(**values)
        if suffix is not None:
            args.output_suffix = cls.generate_output_suffix(args, suffix)
        cls.adjust_args(args)
        cls.assert_no_overlap(args, args0)
        if (not os.path.isfile(args.output_param_final)) or overwrite:
            cls(args)
        print(f"CREATED STEADY STATE IN {args.output_param_final}")
        return args.output_param_final

    @classmethod
    def assert_no_overlap(cls, args1, args2):
        for k in cls._output_ftypes:
            if getattr(args1, k) == getattr(args2, k):
                if getattr(args1, k) in [None, True, False]:
                    continue
                print(k)
                print(getattr(args1, k))
                print(getattr(args2, k))
            assert getattr(args1, k) != getattr(args2, k)

    @classmethod
    def normalize_param(cls, *args):
        generated_files = []
        suffix = '_DIFFNORM'
        for x in args:
            cmp_language = [
                y.language for y in args
                if y.language != x.language
            ]
            for ftype in ['output_param_first',
                          'output_param_final',
                          'output_param_trace']:
                src = getattr(x, ftype)
                if (not os.path.isfile(src)) or suffix in src:
                    continue
                dst = suffix.join(os.path.splitext(src))
                convert_matlab_param_task.normalize_param(
                    x, src, dst, src_language=x.language,
                    dst_language='cpp', cmp_language=cmp_language[0],
                    ftype=('trace' if 'trace' in ftype else None),
                )
                generated_files.append(dst)
                setattr(x, ftype, dst)
        return generated_files

    @classmethod
    def do_compare(cls, farg, *args, **kwargs):
        x1 = args[0]
        if x1.output_diff is True:
            x1.output_diff = f'{x1.language}.diff'
        if not hasattr(x1, 'diff_errors'):
            x1.diff_errors = []
        for x2 in args[1:]:
            cls.suffix_path_args(x1, ['output_diff'],
                                 f'_vs_{x2.language}_{farg}')
            f1 = getattr(x1, farg)
            f2 = getattr(x2, farg)
            kwargs.setdefault('check_files_kwargs', {})
            kwargs['check_files_kwargs'].update(
                label_f1=x1.language,
                label_f2=x2.language,
                quiet=bool(x1.output_diff),
            )
            kwargs.update(
                output_diff=x1.output_diff,
                dont_prune_diff=x1.dont_prune_diff,
            )
            compare_files(f1, f2, **kwargs)
            if x1.output_diff and os.path.isfile(x1.output_diff):
                x1.diff_errors.append(x1.output_diff)

    @classmethod
    def diff(cls, *args):
        cls.normalize_param(*args)
        if not hasattr(args[0], 'output_diff'):
            args[0].output_diff = False
        cls.do_compare('output_param_first', *args)
        cls.do_compare('output_param_final', *args)
        # cls.do_compare(
        #     'output_file', *args, ftype='output',
        #     check_files=check_output,
        #     check_files_kwargs={
        #         'reltol': args[0].reltol,
        #         'abstol': args[0].abstol,
        #         'sep': ',',
        #     }
        # )
        # cls.do_compare(
        #     'output_param_trace', *args, ftype='trace',
        #     check_files=check_output,
        #     check_files_kwargs={
        #         'reltol': args[0].reltol,
        #         'abstol': args[0].abstol,
        #         'sep': ',',
        #     },
        # )
        if args[0].diff_errors:
            raise ComparisonError('Files differ:\n\t'
                                  + '\n\t'.join(args[0].diff_errors))

    # @clasmethod
    # def diff_trace(cls, *args):
    #     t0, x0 = args[0][:]
    #     values = [x0]
    #     diffs = []
    #     tot = x0
    #     for t, x in args[1:]:
    #         y = np.interp(t0, t, x)
    #         values.append(y)
    #         tot = tot + y
    #     mean = np.abs(tot) / len(args)
    #     diffs = [np.abs(y - x0) / mean for y in values[1:]]
    #     return diffs


class analyze_trace(SubTask):

    @classmethod
    def adjust_args(cls, args):
        if not args.plot_var:
            if args.plot_ncol is None:
                args.plot_ncol = 5
            args.plot_var = [
                'ALL::VARS::TestLi',
                'BF::COND::PHl',
                # 'BF::VEL::VsATP',
                'PS::COND::ATP',
                'ALL::VARS::dissipation',
                'FI::RC::kA_d',
                # 'FI::RC::kU_d',
                'BF::RC::Kd',
                'XanCycle::COND::Vx',
                'XanCycle::COND::Ax',
                'XanCycle::COND::Zx',
                'XanCycle::COND::PsbSQ',
                'XanCycle::MOD::XanCycle2FIBF_Kd_NPQ',
                # 'FI::VEL::v_r1',
                'FI::VEL::v1',
                'FI::VEL::vP680qU',
                'ALL::VARS::expr_psbs',
                'ALL::VARS::one_minus_QH',
                'XanCycle::VEL::vpsbs_act',
                'XanCycle::VEL::vpsbs_deact',
                'ALL::VARS::Vc',
                'ALL::VARS::Vo',
                'ALL::VARS::CO2AR',
                'BF::COND::An',
                'BF::COND::Fdn',
                'BF::COND::Qi',
                'BF::VEL::Vbf5',
                'BF::VEL::Vbf7',
                'BF::VEL::Vbf15',
                'BF::VEL::Vbf16',
                'BF::VEL::vbfn2',
                'BF::VEL::vcet',
                'BF::VEL::VsNADPH',
                'BF::COND::NADPH',
                'BF::COND::cytc2',
                'FI::COND::P680pPheon',
                'FI::COND::P680Pheon',
                'BF::COND::Q',
                'FI::COND::QAQB',
                'FI::COND::QAQBn',
                'FI::COND::QAQB2n',
                'FI::COND::QAnQB',
                'FI::COND::QAnQBn',
                'FI::COND::QAnQB2n',
                'FI::RC::k2',
                # 'FIBF::MOD::FIBF2FI_PQa',
                # 'FI::POOL::QBt',
                # 'BF::COND::Qi',
                # 'BF::COND::Qn',
                # 'BF::COND::Qr',
                # 'BF::COND::ISPoQH2',
                # 'BF::COND::QHsemi',
            ]
        if args.plot_ncol is None:
            args.plot_ncol = 2
        if args.plot_file is True:
            args.plot_file = os.path.splitext(
                args.result_file[0])[0] + '.png'
        if args.find_inflections is True:
            args.find_inflections = os.path.splitext(
                args.result_file[0])[0] + '_Inflections.png'
        if args.plot_trace_diffs is True:
            args.plot_trace_diffs = os.path.splitext(
                args.result_file[0])[0] + '_TraceDiffs.png'
        cls.prefix_path_args(args, ['plot_file', 'find_inflections',
                                    'plot_trace_diffs'],
                             prefix=args.output_dir)
        cls.suffix_path_args(args, ['plot_file', 'find_inflections',
                                    'plot_trace_diffs'],
                             args.output_suffix)
        if args.plot_labels:
            assert len(args.plot_labels) == len(args.result_file)
        elif len(args.result_file) > 1:
            args.plot_labels = [
                os.path.basename(x) for x in args.result_file
            ]
        else:
            args.plot_labels = [None]
        super(analyze_trace, cls).adjust_args(args)

    def __init__(self, args, **kwargs):
        super(analyze_trace, self).__init__(args, **kwargs)
        subplots = True
        subplots_inflection = True
        subplots_trace_diff = True
        if not hasattr(args, 'plot_light_profile'):
            args.plot_light_profile = {}
        limits = {}
        data0 = None
        for i, fname in enumerate(args.result_file):
            data = read_param_table(fname, no_title=True)
            # data.set_index('time')
            if i == 0:
                data0 = data
            subplots = self.plot(
                data, names=args.plot_var, ncol=args.plot_ncol,
                tmin=args.plot_tmin, tmax=args.plot_tmax,
                limits=limits,
                vlines={'all': args.plot_tline},
                include_AvL=args.plot_AvL,
                shading=args.plot_light_profile,
                match_figure=args.match_figure,
                return_subplots=subplots,
                label=args.plot_labels[i],
            )
            if args.plot_trace_diffs and i > 0:
                assert i < 2
                data_resampled = self.resample_data(data0, data)
                trace_diffs = self.find_trace_diff(
                    data0, data_resampled, tmin=args.inflection_tmin,
                    tmax=args.inflection_tmax,
                    tol=args.reltol,
                    N=args.inflection_N,
                    dont_resample=True,
                )
                pprint.pprint(trace_diffs)
                trace_diffs = trace_diffs.sort_values('time').T
                vlines = {'all': args.plot_tline}
                for k in trace_diffs.columns:
                    vlines[k] = (
                        trace_diffs[k]['time'],
                        {'ls': ':', 'color': 'r'}
                    )
                if args.inflection_tmin:
                    vlines['all'].append((
                        args.inflection_tmin,
                        {'ls': ':', 'color': 'o'}
                    ))
                if args.inflection_tmax:
                    vlines['all'].append((
                        args.inflection_tmax,
                        {'ls': ':', 'color': 'o'}
                    ))
                subplots_trace_diff = self.plot_data(
                    data_resampled, names=trace_diffs.columns, ncol=5,
                    tmin=args.plot_tmin, tmax=args.plot_tmax,
                    vlines=vlines,
                    shading=args.plot_light_profile,
                    return_subplots=subplots_trace_diff,
                    label=args.plot_labels[i],
                    diff_data=data0,
                )
            if args.find_inflections:
                assert i < 2
                inflections = self.find_inflection(
                    data, tmin=args.inflection_tmin,
                    tmax=args.inflection_tmax,
                    tol=args.inflection_tol,
                    N=args.inflection_N,
                )
                pprint.pprint(inflections)
                inflections = inflections.sort_values('time').T
                vlines = {'all': args.plot_tline}
                for k in inflections.columns:
                    vlines[k] = (
                        inflections[k]['time'],
                        {'ls': ':', 'color': 'r'}
                    )
                if args.inflection_tmin:
                    vlines['all'].append((
                        args.inflection_tmin,
                        {'ls': ':', 'color': 'o'}
                    ))
                if args.inflection_tmax:
                    vlines['all'].append((
                        args.inflection_tmax,
                        {'ls': ':', 'color': 'o'}
                    ))
                subplots_inflection = self.plot_data(
                    data, names=inflections.columns, ncol=5,
                    tmin=args.plot_tmin, tmax=args.plot_tmax,
                    vlines=vlines,
                    shading=args.plot_light_profile,
                    return_subplots=subplots_inflection,
                    label=args.plot_labels[i],
                )
        add_legend = (len(args.result_file) > 1
                      and None not in args.plot_labels)
        self.finalize_plot(*subplots, fname=args.plot_file,
                           add_legend=add_legend)
        if args.plot_trace_diffs:
            self.finalize_plot(*subplots_trace_diff,
                               fname=args.plot_trace_diffs,
                               add_legend=add_legend)
        if args.find_inflections:
            self.finalize_plot(*subplots_inflection,
                               fname=args.find_inflections,
                               add_legend=add_legend)

    @classmethod
    def resample_data(cls, data0, data):
        interp = scipy.interpolate.CubicSpline(data['time'], data)
        data_orig = data
        data = pd.DataFrame(interp(data0['time']))
        data.columns = data_orig.columns
        return data

    @classmethod
    def find_trace_diff(cls, data0, data, name=None,
                        tmin=None, tmax=None,
                        tol=None, N=None, dont_resample=False):
        if tol is None:
            tol = 20
        if tmin is not None:
            data0 = data0[data0['time'] >= tmin]
        if tmax is not None:
            data0 = data0[data0['time'] <= tmax]
        if not dont_resample:
            data = cls.resample_data(data0, data)
        if name is None:
            values = {}
            pd.set_option('display.max_rows', None)
            for k in data0.columns:
                if k == 'time':
                    continue
                try:
                    values[k] = cls.find_trace_diff(
                        data0, data, name=k, tol=tol,
                        dont_resample=True,
                    )
                except KeyError:
                    continue
            values = pd.DataFrame(values).T.sort_values('diff')
            values = values[values['diff'] != np.inf]
            if N is not None:
                values = values.iloc[-N:, :]
            return values
        t = data['time']
        mean = (data[name] + data0[name]) / 2
        y = np.abs(data[name] - data0[name]) / np.abs(mean)
        idx = t.index[
            abs(y) > tol
        ].min()
        tidx = t[idx]
        yidx = y[idx]
        return {'idx': idx, 'time': tidx, 'diff': yidx}

    @classmethod
    def find_inflection(cls, data, name=None, tmin=None, tmax=None,
                        tol=None, N=None):
        if tol is None:
            tol = 20
        if tmin is not None:
            data = data[data['time'] >= tmin]
        if tmax is not None:
            data = data[data['time'] <= tmax]
        if name is None:
            values = {}
            pd.set_option('display.max_rows', None)
            for k in data.columns:
                if k == 'time':
                    continue
                try:
                    values[k] = cls.find_inflection(
                        data, name=k, tol=tol)
                except KeyError:
                    continue
            values = pd.DataFrame(values).T.sort_values('dydt2')
            values = values[values['dydt2'] != np.inf]
            if N is not None:
                values = values.iloc[-N:, :]
            return values
        t = data['time']
        y = data[name]
        tdiff = t.diff(periods=-1)
        ydiff = y.diff(periods=-1)
        dydt = ydiff / tdiff
        dydt2 = dydt.pct_change(periods=-1, fill_method=None)  # / tdiff
        idx = t.index[np.logical_and(
            abs(dydt2) > tol,
            abs(ydiff) > 0,
        )].min()
        tidx = t[idx]
        didx = abs(dydt2[idx])
        return {'idx': idx, 'time': tidx, 'dydt2': didx}

    @classmethod
    def plot(cls, data, match_figure=False, **kwargs):
        if not match_figure:
            return cls.plot_data(data, **kwargs)
        if match_figure.startswith('Zhu2012_'):
            return cls.plot_Zhu2012(
                data, figure=match_figure.split('Zhu2012_')[-1],
                **kwargs)
        elif match_figure.startswith('Zaks2012_'):
            return cls.plot_Zaks2012(
                data, figure=match_figure.split('Zaks2012_')[-1],
                **kwargs)
        raise NotImplementedError(match_figure)

    @classmethod
    def plot_data(cls, data, names=None, ncol=2, tmin=None, tmax=None,
                  limits=None, units={}, aliases={},
                  fname=None, include_AvL=False, vlines=None,
                  shading={}, return_subplots=False, label=None,
                  diff_data=None):
        first_pass = (not isinstance(return_subplots, tuple))
        if limits is None:
            limits = {}
        if not units:
            units = {
                'ALL::VARS::CO2AR': 'umol m**-2 s**-1',
                'ALL::VARS::dissipation': 'umol m**-2 s**-1',
                'FI::VEL::vS3_S0': 'umol m**-2 s**-1',
                'ALL::VARS::fluoresence': 'umol m**-2 s**-1',
                'ALL::VARS::MembranePotential': 'V',
                'BF::COND::Ks': 'mM',
                'BF::COND::Mgs': 'mM',
                'BF::COND::Cls': 'mM',
                'ALL::VARS::TestLi': 'umol m**-2 s**-1',
            }
        assert 'time' in data
        if names is None:
            if isinstance(data, pd.DataFrame):
                names = data.columns
            else:
                names = list(data.keys())
            names.remove('time')
        if tmin is None:
            tmin = min(data['time'])
        if tmax is None:
            tmax = max(data['time'])
        xlim = (tmin, tmax)
        nplots = len(names)
        if include_AvL:
            nplots += 1
        if nplots == 1:
            ncol = 1
        nrow = int(np.ceil(nplots / ncol))
        if isinstance(return_subplots, tuple):
            assert len(return_subplots) == 2
            fig, axs = return_subplots[:]
        else:
            fig, axs = plt.subplots(nrow, ncol,
                                    figsize=(2.5 * ncol, 1.5 * nrow),
                                    layout='constrained')
        for ax, v in zip(axs.flat, names):
            ax.set_xlabel('time (s)')
            vname = aliases.get(v, v.split('::')[-1])
            if v in units:
                vname += f' ({units[v]})'
            ax.set_ylabel(vname)
            ax.set_xlim(*xlim)
            draw_lines = first_pass
            if v not in data:
                print(f"MISSING PLOT VAR: {v}")
            else:
                if diff_data is not None:
                    vmean = np.abs(data[v] + diff_data[v]) / 2
                    vdata = np.abs(data[v] - diff_data[v]) / vmean
                else:
                    vdata = data[v]
                ax.plot(data['time'], vdata, label=label)
                if v not in limits:
                    ylim = (min(vdata), max(vdata))
                    buff = 0.1 * (ylim[1] - ylim[0])
                    ylim = (ylim[0] - buff, ylim[1] + buff)
                    limits[v] = ylim
                    draw_lines = True
            ylim = limits.get(v, (0, 0))
            if ylim[0] != ylim[1]:
                ax.set_ylim(*ylim)
            if ylim[0] != ylim[1] and draw_lines:
                ivlines = copy.deepcopy(vlines.get('all', []))
                if ivlines is None:
                    ivlines = []
                if v in vlines:
                    if isinstance(vlines[v], list):
                        ivlines += vlines[v]
                    else:
                        ivlines.append(vlines[v])
                for x in ivlines:
                    kws = {'ls': ':'}
                    if isinstance(x, tuple):
                        kws = x[1]
                        x = x[0]
                    ax.vlines(x, *ylim, **kws)
            if shading and first_pass:
                for band in args.plot_light_profile.get('dark', []):
                    ax.axvspan(*band, alpha=0.1)  # , color='blue')
                for band in args.plot_light_profile.get('light', []):
                    ax.axvspan(*band, alpha=0.1, color='yellow')
        if include_AvL:
            ax = axs.flat[-1]
            ax.plot(data['ALL::VARS::TestLi'], data['ALL::VARS::CO2AR'])
            ax.set_xlabel(f'PFD ({units.get("ALL::VARS::TestLi", "")})')
            ax.set_ylabel(f'A ({units.get("ALL::VARS::CO2AR", "")})')
        if return_subplots:
            return (fig, axs)
        cls.finalize_plot(fig, axs, fname=fname)

    @classmethod
    def finalize_plot(cls, fig, axs, fname=None, add_legend=False):
        if add_legend:
            axs.flat[0].legend()
        if fname:
            print(f'Saving plot to \"{fname}\"')
            fig.savefig(fname)
        else:
            plt.show()

    @classmethod
    def plot_Zaks2012(cls, data, figure='FigS3', **kwargs):
        if figure == 'FigS3':
            kwargs.setdefault(
                'names', [
                    'BF::COND::PHl',
                    'BF::COND::ATP',  # 'ATPStroma',
                    'ALL::VARS::dissipation',  # 'ExcitonsDissipatedBF',
                    'XanCycle::MOD::XanCycle2FIBF_Kd_NPQ',
                    'FIBF::COND::kd',
                    'FI::RC::kA_d',
                    'FI::RC::kU_d',
                    'BF::RC::Kd',
                    'XanCycle::COND::Vx',
                    'XanCycle::COND::Ax',
                    'XanCycle::COND::Zx',
                    'XanCycle::COND::PsbSQ',
                    'FI::VEL::vP680qU',
                ]
            )
            kwargs.setdefault('limits', {})
            kwargs['limits'].update(**{
                'BF::COND::PHl': (1, 8),
            })
        else:
            raise NotImplementedError(figure)
        return cls.plot_data(data, **kwargs)

    @classmethod
    def plot_Zhu2012(cls, data, figure='Fig2', **kwargs):
        ROEvar = 'FI::VEL::vS3_S0'
        PSIIvar = 'ALL::VARS::fPSII'
        kwargs.setdefault(
            'aliases', {
                ROEvar: 'O2 Evolution',
                PSIIvar: 'PhiII',
                'ALL::VARS::CO2AR': 'A',
                'ALL::VARS::TestLi': 'PFD',
                'ALL::VARS::fluoresence': 'Fluoresence',
                # 'ALL::VARS::dissipation': 'Excitons dissipated as heat',
                'ALL::VARS::dissipation': 'ExcitonsDissipatedBF',
                'BF::COND::ATP': 'ATPStroma',
            }
        )
        if figure in ['Exp1', 'Fig2']:
            kwargs.setdefault('limits', {})
            kwargs['limits'].update(**{
                'ALL::VARS::CO2AR': (0, 20),
                'ALL::VARS::dissipation': (0, 700),
                ROEvar: (0, 20),
                'ALL::VARS::fluoresence': (0, 24),
                'ALL::VARS::MembranePotential': (-0.02, 0.00),
                PSIIvar: (0, 0.6),
                'BF::COND::PHs': (7, 7.6),
                'BF::COND::PHl': (6.5, 7),
                'BF::COND::Ks': (9, 15),
                'BF::COND::Mgs': (5, 7),
                'BF::COND::Cls': (0, 1.2),
            })
            kwargs.setdefault('ncol', 2)
            kwargs.setdefault(
                'names', [
                    'ALL::VARS::CO2AR', 'ALL::VARS::dissipation',
                    ROEvar,  'ALL::VARS::fluoresence',
                    'ALL::VARS::MembranePotential', PSIIvar,
                    'BF::COND::PHs', 'BF::COND::PHl',
                    'BF::COND::Ks', 'BF::COND::Mgs',
                    'BF::COND::Cls', 'ALL::VARS::TestLi',
                ]
            )
        elif figure in ['Exp3', 'Fig3']:
            kwargs.setdefault('limits', {})
            kwargs['limits'].update(**{
                'ALL::VARS::CO2AR': (0, 32),
                'ALL::VARS::dissipation': (0, 500),
                ROEvar: (10, 20),
                'ALL::VARS::fluoresence': (0, 50),
                'ALL::VARS::MembranePotential': (-0.03, 0.03),
                PSIIvar: (0, 0.6),
                'BF::COND::PHs': (7, 8.25),
                'BF::COND::PHl': (6.5, 7),
                'ALL::VARS::TestLi': (0, 1000),
            })
            kwargs.setdefault(
                'names', [
                    'ALL::VARS::CO2AR', 'ALL::VARS::dissipation',
                    ROEvar,  'ALL::VARS::fluoresence',
                    'ALL::VARS::MembranePotential', PSIIvar,
                    'BF::COND::PHs', 'BF::COND::PHl',
                    'ALL::VARS::TestLi',
                ]
            )
        else:
            raise NotImplementedError(figure)
        return cls.plot_data(data, **kwargs)


class create_iterations(SubTask):

    @classmethod
    def adjust_args(cls, args):
        if not args.param:
            args.param = {}
        # Force output
        args.plot_file = True
        args.result_file = True  # Force output
        if args.light_profile == 'steady-state':
            args.evn_file = False
            args.preserve_output_param = True
            if args.output_param < 2:
                args.output_param = 2
        if (not args.output_suffix) or args.output_suffix is True:
            args.output_suffix = cls.generate_output_suffix(args)
        if not args.iterations_file:
            args.iterations_file = 'InputTimeIteration.txt'
        cls.suffix_path_args(args, ['iterations_file'],
                             args.output_suffix)
        super(create_iterations, cls).adjust_args(args)

    def run_commands(self, args, **kwargs):
        self.create_light_profile(args, **kwargs)

    @classmethod
    def create_light_profile(cls, args):
        if not args.light_profile:
            return None
        if (((not args.overwrite_light_profile)
             and os.path.isfile(args.iterations_file))):
            pass
        args.overwrite_light_profile = False  # Prevent creating it twice
        variables = {'time': [], 'PFD': []}
        light_profile = args.light_profile
        args.plot_light_profile = {}
        if args.light_profile == 'dilkaran':
            light_profile = 'dark-light-dark'
            args.low_light_level = 0
            args.high_light_level = 600
            args.tstart_light_change = 400
            args.duration_light_change = 400
        elif args.light_profile in ['Zhu2012', 'Zhu2012_Fig2']:
            light_profile = 'light-dark-light'
            args.low_light_level = 100
            args.high_light_level = 1000
            args.tstart_light_change = 200
            args.duration_light_change = 200
        elif args.light_profile == 'Zhu2012_Fig3':
            # 100 µmol m−2 s−1 increments
            variables['PFD'] = np.linspace(0, 1000, 11)
            variables['PFD'].append(variables['PFD'][-1])
            variables['time'] = [
                200 * x for x in range(len(variables['PFD']))
            ]
            args.plot_AvL = True
        elif args.light_profile == 'NPQ-explore':
            # light_profile = 'dark-light-dark'
            args.low_light_level = 50
            args.high_light_level = 1800
            variables['time'] = [
                0,
                40,
                180,
                250,
            ]
            variables['PFD'] = [
                args.low_light_level,
                args.high_light_level,
                args.low_light_level,
                args.low_light_level,
            ]
            args.plot_light_profile = {
                'dark': [
                    (variables['time'][0], variables['time'][1]),
                    (variables['time'][2], variables['time'][3]),
                ],
                'light': [
                    (variables['time'][1], variables['time'][2]),
                ],
            }
        elif args.light_profile in ['Zaks2012', 'Zaks2012_FigS3']:
            light_profile = 'dark-light-dark'
            args.low_light_level = 100
            args.high_light_level = 1000
            args.tstart_light_change = 2000.0 / 3
            args.duration_light_change = 2000.0 / 3
        elif args.light_profile == 'steady-state':
            variables['PFD'] = [args.low_light_level]
            variables['PFD'].append(variables['PFD'][-1])
            variables['time'] = [
                200 * x for x in range(len(variables['PFD']))
            ]
        else:
            raise ValueError(f"Unsupported light profile: "
                             f"{args.light_profile}")
        if light_profile == 'dark-light-dark':
            args.plot_light_profile = {
                'dark': [
                    (variables['time'][0], variables['time'][1]),
                    (variables['time'][2], variables['time'][3]),
                ],
                'light': [
                    (variables['time'][1], variables['time'][2]),
                ],
            }
            variables['time'] = [
                0,
                args.tstart_light_change,
                args.tstart_light_change + args.duration_light_change,
                2 * args.tstart_light_change + args.duration_light_change,
            ]
            variables['PFD'] = [
                args.low_light_level,
                args.high_light_level,
                args.low_light_level,
                args.low_light_level,
            ]
        elif light_profile == 'light-dark-light':
            args.plot_light_profile = {
                'light': [
                    (variables['time'][0], variables['time'][1]),
                    (variables['time'][2], variables['time'][3]),
                ],
                'dark': [
                    (variables['time'][1], variables['time'][2]),
                ],
            }
            variables['time'] = [
                0,
                args.tstart_light_change,
                args.tstart_light_change + args.duration_light_change,
                2 * args.tstart_light_change + args.duration_light_change,
            ]
            variables['PFD'] = [
                args.high_light_level,
                args.low_light_level,
                args.high_light_level,
                args.high_light_level,
            ]
        else:
            assert 'PFD' in variables
        variables['GLight'] = variables.pop('PFD')
        cls.write_iterations(args.iterations_file, variables)
        return variables

    @classmethod
    def write_iterations(cls, fname, data):
        df = pd.DataFrame(data)
        df.to_csv(fname, sep='\t', index=False)
        print(f'Wrote iterations to \"{fname}\"')

    @classmethod
    def read_iterations(cls, fname):
        df = pd.read_csv(fname)
        return {k: np.array(df[k]) for k in df.columns}

    @classmethod
    def generate_output_suffix(cls, args, extra_suffix=''):
        out = extra_suffix
        if args.light_profile:
            light_profile = args.light_profile
            if light_profile in ['dark-light-dark',
                                 'light-dark-light',
                                 'steady-state']:
                if args.low_light_level is None:
                    args.low_light_level = 100
                if args.high_light_level is None:
                    args.high_light_level = 1000
                if light_profile == 'steady-state':
                    light_profile = 'steady-state'
                    light_profile += f'_{args.low_light_level}'
                else:
                    light_profile += (
                        f'_{args.low_light_level}_to_'
                        f'{args.high_light_level}'
                    )
            out += f'_{light_profile}'
        return out


class IrreversiblePatchError(RuntimeError):

    def __init__(self, msg, contents):
        self.contents = contents
        super(IrreversiblePatchError, self).__init__(msg)


class PatchError(RuntimeError):
    pass


class instrument_matlab(SubTask):

    _comment = '% '
    _generated_header = (
        '% THIS FILE HAS BEEN GENERATED BY scripts/devtasks.py AND\n'
        '% SHOULD NOT BE EDITED DIRECTLY\n'
    )
    _patch_flag_header = (
        '% THIS FILE HAS BEEN PATCHED TO ALLOW FOR PARAMETER I/O \n'
        '% BY scripts/devtasks.py FROM THE C++ MODEL REPO\n'
    )
    _param_array_length = {
        'PR': {
            'COND': 13,
        },
        'PS': {
            'COND': 15,
        },
        'RROEA': {
            'POOL': 10,
            'RC': 10,
            'COND': 10,
        },
        'RuACT': {
            'RC': 10,
        },
        'NPQ': {
            'RC': 7,
        },
    }
    _param_substitutions = {
        # So that the output matches the stored parameter instead of
        #   the local variable
        'RuACT': {
            'RC': {
                'RCA': 'RCA / activase',
            },
        },
        'BF': {
            'MOD': {
                '_Pi': 'BF_con(16)',
            },
        },
    }
    _param_defaults = {
        'FIBF': {
            'kdm0': 'dmax ./ QH',
        },
        'PS': {
            'v4': '0',
            'KE1Ratio': '(1. + 1. ./ KE11 + 1. ./ KE12)',
            'KE2Ratio': '(1. + 1. ./ KE21 + KE22)',
        },
        'SUCS': {
            'v61': '0',
            'KE5Ratio': '1.0 + KE541 + 1.0 ./ KE531',
        },
        'ALL': {
            # Defaults
            'alfa': 0.85,
            'fc': 0.15,
            'Theta': 0.7,
            'beta': 0.7519,
            'PS2BF_Pi': 0.0,
            'alpha1': 1.0,
            'alpha2': 1.0,
            'F': '9.649 * 10^4',
            'R': '8.314',
            'RT': '8.314 * 298',
            # Calculated
            'Vc': 'v6_1 .* AVR',
            'Vo': 'v6_2 .* AVR',
            'VPGA': 'vpga_use .* AVR',
            'Vstarch': '(v23 - v25) .* AVR',
            'Vsucrose': 'vdhap_in .* AVR',
            'VT3P': '(v31 + v33) .* AVR',
            'Vt_glycerate': 'v1in .* AVR',
            'Vt_glycolate': 'v2out .* AVR',
            # TODO: This is 'vP680_d' in C++, but 'vU_P680' came from a
            #   MATLAB version
            'PSIIabs': 'vP680_d',
            # TODO: This is 'Vbf11' in C++, but 'Vbf10' came from a
            #   MATLAB version
            'PSIabs': 'Vbf11',
            'CarbonRate': 'v6_1 .* AVR',
            'CO2Release': 'v131 .* AVR',
            'CO2AR': 'CarbonRate - CO2Release',
            'ROE': 'vS3_S0',
            'dissipation': 'vA_d + vU_d',
            'fluoresence': 'vA_f + vU_f',
            'fPSII': (
                '1 - (fluoresence + dissipation) ./ (GLight .* 27.0 ./ 47.0)'
            ),
            'Hfs': '(10.^-PHs) .* 1000.',
            'OHs': '(10.^-14.) ./ (Hfs ./ 1000.) .* 1000.',
            'BFs': 'BFHs - Hfs',
            'BFns': 'BFTs - BFs',
            'AfC': '6.022 .* (10.^ 23.)',
            'UnitCharge': '1.6 .* (10.^-19.)',
            'NetCharge': (
                '(Hfs + Ks + 2. .* Mgs - OHs - Cls - BFns) .* '
                'RVA .* AfC .* UnitCharge ./ 1000'
            ),
            'MembranePotential': (
                '2.0 .* NetCharge ./ 6.0 .* (10.^6)'
            ),
            'expr_psbs': '10.^(hill_psbs .* (PHl - pK_psbs))',
            'QH': '1.0 ./ (1.0 + expr_psbs)',
            'one_minus_QH': 'expr_psbs ./ (1.0 + expr_psbs)',
        },
    }
    _param_cond_start = {
        'CM': 36,
        'DynaPS': 96,
        'EPS': 87,
        'FIBF': 52,
        'PS_PR': 24,
        'RA': 92,
        'trDynaPS': 120,
    }
    _param_names_extra = {
        'BF': {
            'COND': {15: 'Pi'},
        },
        'PR': {
            'COND': {
                2: 'PGA',
                11: 'CO2',
                12: 'O2',
            },
        },
        'PS': {
            'COND': {
                10: 'NADPH',
                11: 'CO2',
                12: 'O2',
            },
        },
        'SUCS': {
            'COND': {
                4: 'ATPc',
                5: 'ADPc',
                6: 'OPOPc',
                8: 'UTPc',
            },
        },
    }
    _param_names_remove = {
        'BF': {
            'RC': ['Em_IPS', 'Em_Cytf', 'Em_PG'],
        },
        'PS': {
            'VEL': ['Pi'],
            'MOD': ['c_c', 'c_o', 'dHa_c', 'dHa_o'],
            'RC': ['KM11_A', 'KM12_A'],
        },
    }
    _param_children = {
        'CM': ["PS_PR", "SUCS"],
        'DynaPS': ["RA", "XanCycle"],
        'EPS': ["FIBF", "CM"],
        'FIBF': ["BF", "FI"],
        'PS_PR': ["PS", "PR"],
        'RA': ["EPS", "RuACT"],
        'trDynaPS': ["DynaPS", "RROEA"],
    }
    _param_explicit = {
        'NPQ': {
            'MOD': ['XanCycle2FIBF_Kd_NPQ'],
            'RC': ['hill_psbs', 'pK_psbs', 'hill_vde', 'pK_vde',
                   'kvde_max', 'Fpsbs', 'psbsQ_converRate', 'k_ze'],
            'COND': ['Vx', 'Ax', 'Zx', 'PsbSQ'],
            'VEL': ['Vva', 'Vaz', 'Vza', 'Vav', 'vpsbs_act',
                    'vpsbs_deact'],
        },
        'XanCycle': {
            'MOD': ['XanCycle2FIBF_Xstate'],
            'RC': ['kva', 'kaz', 'kza', 'kav'],
            'COND': ['Vx', 'Ax', 'Zx', 'ABA'],
            'VEL': ['Vva', 'Vaz', 'Vza', 'Vav', 'Vvf',
                    'Vv2ABA', 'VABAdg'],
        },
    }
    _condition_vars = [
        'ALL::VARS::TestLi', 'ALL::VARS::TestLi_Wps', 'ALL::VARS::GLight',
        'ALL::VARS::CO2_in', 'ALL::VARS::CO2_cond',
        'ALL::VARS::O2', 'ALL::VARS::O2_cond',
        'ALL::VARS::Tp',
        'PR::MOD::RUBISCOMETHOD',
    ]

    @classmethod
    def get_children(cls, mod):
        out = [mod]
        for k in cls._param_children.get(mod, []):
            out += cls.get_children(k)
        return out

    @classmethod
    def adjust_args(cls, args):
        for k in ['remove_patch', 'force_reset', 'patch_extra',
                  'implicit_missing', 'generate_functions']:
            if not hasattr(args, k):
                setattr(args, k, False)
        if not args.matlab:
            args.matlab = find_matlab(required=True)
        cls.prefix_path_args(
            args, ['matlab_repo'], prefix=os.getcwd())
        args.matlab_repo_case_sensitive = is_case_sensitive(
            args.matlab_repo)
        if not args.remove_patch:
            print(f"MATLAB REPO: {args.matlab_repo} (case sensitive = "
                  f"{args.matlab_repo_case_sensitive})")
        args.matlab_utils = os.path.join(_scripts_dir, "matlab")
        args.all_modules = get_module_list(include_ALL=True)
        args.all_param_types = get_param_type_list()
        # print(f"MODULES = {args.all_modules}")
        # print(f"PARAM_TYPES = {args.all_param_types}")
        super(instrument_matlab, cls).adjust_args(args)

    def run_commands(self, args, **kwargs):
        self.instrument_matlab(args, **kwargs)

    @classmethod
    def instrument_matlab(cls, args, **kwargs):
        args.patched = []
        cls.apply_patch(cls.patch_condition,
                        os.path.join(args.matlab_repo, "Condition.m"),
                        args)
        cls.apply_patch(cls.patch_sysinitial,
                        os.path.join(args.matlab_repo, "SYSInitial.m"),
                        args)
        for mod in args.all_modules:
            cls.patch_module(mod, args)
        if args.patch_extra or args.remove_patch:
            extra_drivers = cls.find_file(args, "Drive")
            for x in extra_drivers:
                if x in args.patched:
                    continue
                mod = cls.fname2module(x, "Drive")
                cls.patch_module(mod, args, verbose=True)

    @classmethod
    def sysinitial_contents(cls, args):
        if not hasattr(args, 'sysinitial_contents'):
            fname_sysini = os.path.join(args.matlab_repo, 'SYSInitial.m')
            assert os.path.isfile(fname_sysini)
            with open(fname_sysini, 'r') as fd:
                args.sysinitial_contents = fd.read()
        return args.sysinitial_contents

    @classmethod
    def patch_module(cls, mod, args, verbose=False):
        if mod == 'ALL':
            if args.generate_functions:
                fnames = os.path.join(args.matlab_utils, f'{mod}_NAMES.m')
                fvalue = os.path.join(args.matlab_utils,
                                      f'{mod}_VALUES.m')
                cls.write_matlab_names(mod, fnames)
                cls.write_matlab_values(mod, fvalue,
                                        allow_calc_trace=True)
            return
        fname_driver = cls.find_file(args, "Drive", mod=mod)
        fname_rate = cls.find_file(args, "Rate", mod=mod)
        if not fname_rate:
            fname_rate = cls.find_file(args, "MB", mod=mod)
        fname_ini = cls.find_file(args, "Ini", mod=mod)
        if fname_driver:
            cls.apply_patch(
                cls.patch_drive, fname_driver[0], args, mod=mod,
                verbose=verbose,
            )
        missing = {}
        if fname_rate:
            assert len(fname_rate) == 1
            cls.apply_patch(cls.patch_rate, fname_rate[0], args, mod=mod,
                            verbose=verbose, missing=missing)
        if fname_ini:
            assert len(fname_ini) == 1
            cls.apply_patch(cls.patch_ini, fname_ini[0], args, mod=mod,
                            verbose=verbose, missing=missing)
        if args.generate_functions:
            fnames = os.path.join(args.matlab_utils, f'{mod}_NAMES.m')
            fvalue = os.path.join(args.matlab_utils, f'{mod}_VALUES.m')
            cls.write_matlab_names(mod, fnames)
            cls.write_matlab_values(mod, fvalue, allow_calc_trace=True)

    @classmethod
    def apply_patch(cls, method, fname, args, replacements=None,
                    verbose=False, **kwargs):
        if fname in args.patched:
            return
        with open(fname, 'r') as fd:
            contents = fd.read()
        contents0 = copy.deepcopy(contents)
        try:
            contents = cls.remove_patch(
                contents, force=args.force_reset)
        except IrreversiblePatchError as e:
            if args.remove_patch:
                raise e
            warnings.warn(str(e))
            return
        if args.remove_patch:
            if contents != contents0:
                with open(fname, 'w') as fd:
                    fd.write(contents)
            args.patched.append(fname)
            return
        contents_unpatched = copy.deepcopy(contents)
        if replacements:
            contents = cls.add_replacements(contents, replacements)
        contents = method(contents, fname, args, **kwargs)
        if contents != contents_unpatched:
            contents = cls._patch_flag_header + contents
            if verbose:
                print(f'PATCHED \"{fname}\"')
        if contents != contents0:
            with open(fname, 'w') as fd:
                fd.write(contents)
        args.patched.append(fname)

    @classmethod
    def remove_patch(cls, contents, force=False):
        if not (force or contents.startswith(cls._patch_flag_header)):
            return contents
        contents = contents.split(cls._patch_flag_header, maxsplit=1)[-1]
        contents = cls.remove_replacements(contents)
        contents = cls.remove_patch_blocks(contents)
        return contents

    @classmethod
    def find_file(cls, args, ftype, mod=None):
        if mod is None:
            mod = '*'
        if args.matlab_repo_case_sensitive:
            var = [ftype.title(), ftype.lower()]
            if ftype not in var:
                var.insert(0, ftype)
        else:
            var = [ftype]
        if ftype == "Ini":
            var += ["Initial"]
            if args.matlab_repo_case_sensitive:
                var += ["initial"]
        matches = []
        for v in var:
            matches += glob.glob(os.path.join(args.matlab_repo,
                                              f'{mod}{v}.m'))
            if mod != '*':
                matches += glob.glob(os.path.join(args.matlab_repo,
                                                  f'{mod}_{v}.m'))
        return sorted(list(set([os.path.realpath(x) for x in matches])))

    @classmethod
    def get_param_type_list(cls, mod, **kwargs):
        if mod == 'NPQ':
            mod = 'XanCycle'
        return get_param_type_list(mod, **kwargs)

    @classmethod
    def get_patch_guards(cls, ptype, regex=False):
        ptype_orig = ptype
        if regex:
            ptype = "XXXREPLACEXXX"
        fini = f'\n{cls._comment}DEVTASKS {ptype} START\n'
        fend = f'\n{cls._comment}DEVTASKS {ptype} END\n'
        if regex:
            fini = re.escape(fini).replace(ptype, ptype_orig)
            fend = re.escape(fend).replace(ptype, ptype_orig)
        return fini, fend

    @classmethod
    def add_replacements(cls, contents, replacements):
        for k, v in replacements.items():
            fini, fend = cls.get_patch_guards(f'REPLACE {k}->{v}')
            kregex = (
                r'(?P<newline_prev>\n)[^\n]*'
                + f'(?P<orig>{re.escape(k)})'
                r'[^\n]*(?P<newline_next>\n)'
            )
            matches = [m for m in re.finditer(kregex, contents)]
            idx = 0
            out = ''
            for m in matches:
                out += (
                    contents[idx:m.end('newline_prev')]
                    + fini
                    + contents[m.end('newline_prev'):m.start('orig')]
                    + v
                    + contents[m.end('orig'):m.end('newline_next')]
                    + fend
                )
                idx = m.end('newline_next')
            out += contents[idx:]
            contents = out
        return contents

    @classmethod
    def remove_replacements(cls, contents):
        fini = cls.get_patch_guards(
            r'REPLACE (?P<k>.+?)\-\>(?P<v>.+?)', regex=True)[0]
        matches = [m for m in re.finditer(fini, contents)]
        idx = 0
        out = ''
        for mini in matches:
            assert mini.start(0) >= idx
            k = mini.group('k')
            v = mini.group('v')
            fend = re.compile(re.escape(
                cls.get_patch_guards(f'REPLACE {k}->{v}')[1]))
            mend = fend.search(contents, mini.end(0))
            out += (
                contents[idx:mini.start(0)]
                + contents[mini.end(0):mend.start(0)].replace(v, k)
            )
            idx = mend.end(0)
        out += contents[idx:]
        return out

    @classmethod
    def add_patch_block(cls, contents, ptype, patch, regex,
                        before_regex=False, append_if_no_match=False):
        fini, fend = cls.get_patch_guards(ptype)
        if fini in contents:
            assert contents.count(fini) == 1
            assert contents.count(fend) == 1
            contents_before = contents.split(fini, maxsplit=1)[0]
            contents_after = contents.split(fend, maxsplit=1)[-1]
        else:
            match = re.search(regex, contents)
            if match:
                if before_regex:
                    contents_before = contents[:match.start(0)]
                    contents_after = contents[match.start(0):]
                else:
                    contents_before = contents[:match.end(0)]
                    contents_after = contents[match.end(0):]
            elif append_if_no_match:
                contents_before = contents
                contents_after = ''
            else:
                raise PatchError(f'Could not match regex \"{regex}\"')
        contents = (
            contents_before + fini + patch + fend + contents_after
        )
        return contents

    @classmethod
    def remove_patch_blocks(cls, contents):
        fini, fend = cls.get_patch_guards(r'\w+', regex=True)
        while re.search(fini, contents):
            contents_before, contents_after = re.split(
                fini, contents, maxsplit=1)
            assert re.search(fend, contents_after)
            contents_after = re.split(
                fend, contents_after, maxsplit=1)[-1]
            contents = contents_before + contents_after
        return contents

    @classmethod
    def find_variable(cls, contents, names, default=None):
        if isinstance(names, str):
            names = [names]
        for x in names:
            m = re.search(r'\n([^\%]*\W)?' + re.escape(x) + r'\W',
                          contents)
            if m:
                return x
        return default

    @classmethod
    def find_all_variables(cls, line):
        regex = (
            r'(?:(?:\W)|(?:^))\W*?'
            r'(?P<name>[a-zA-Z]\w*)'
            r'\W*?(?:(?:\W)|(?:$))'
        )
        return [m.group('name') for m in re.finditer(regex, line)]

    @classmethod
    def find_param(cls, contents, name, mod, pt, default=None,
                   missing=None):
        aliases = [name]
        if name in cls._param_substitutions.get(mod, {}).get(pt, {}):
            return cls._param_substitutions[mod][pt][name]
        mod0 = 'XanCycle' if mod == 'NPQ' else mod
        param = ParameterSet.all_parameters().get_param(
            mod0, pt, name, default=None)
        if param is not None and param.matlab_var:
            if contents is True:
                return param.matlab_var
            aliases.append(param.matlab_var)
        if contents is True:
            return name
        if ((default is None and pt != 'CALC'
             and name in cls._param_defaults.get(mod, {}))):
            default = cls._param_defaults[mod][name]
        missing_var = f'{mod}_{pt}_{name}'
        if isinstance(missing, dict):
            if default is None:
                default = missing_var
            aliases.append(missing_var)
        if default is None:
            default = 0
        if mod == 'XanCycle':
            aliases.append(f'XanCycle_{name}')
        out = cls.find_variable(contents, aliases, default=default)
        if isinstance(missing, dict) and out == missing_var:
            missing[(mod, pt, name)] = missing_var
        return out

    @classmethod
    def find_patch_var(cls, mod, pt, contents):
        var = [f'{mod}_{pt}', f'{mod}_{pt.title()}']
        if pt == 'COND':
            var = [f'{mod}_con', f'{mod}_Con'] + var
        elif pt == 'VEL':
            var = [f'{mod}_{pt.title()}']
        if mod in ['PS', 'PR', 'PS_PR']:
            if pt == 'COND':
                var = [f'{mod}s', f'{mod.title()}S'] + var
            elif pt == "VEL":
                var = [f'{mod}r', 'Velocity'] + var
        return cls.find_variable(contents, var)

    @classmethod
    def get_matlab_param_names(cls, mod, pt, **kwargs):
        mod0 = 'XanCycle' if mod == 'NPQ' else mod
        if pt in ['CALC', 'CTRL']:
            kwargs.setdefault('qualifiers', {})
            if pt == "CALC":
                kwargs['qualifiers']['ON_DEMAND'] = True
            else:
                kwargs['qualifiers'][pt] = True
            pt = 'VARS' if mod0 == 'ALL' else 'MOD'
            try:
                out = get_param_names(mod0, pt, **kwargs)
            except FileNotFoundError:
                out = []
        else:
            out = get_param_names(mod0, pt, **kwargs)
        if pt in cls._param_explicit.get(mod, {}):
            out = [x for x in cls._param_explicit[mod][pt] if x in out]
        for idx, name in cls._param_names_extra.get(
                mod, {}).get(pt, {}).items():
            out.insert(idx, name)
        for name in cls._param_names_remove.get(mod, {}).get(pt, []):
            if name in out:
                out.remove(name)
        return out

    @classmethod
    def matlab_code_global_vars(cls, names, defaults=None):
        out = [
            f'global {x};' for x in names
        ]
        if defaults:
            for x in names:
                out += [
                    f'if isempty({x})',
                    f'    {x} = {defaults.get(x, 0)};',
                    'end',
                ]
        return out

    @classmethod
    def name2fullname(cls, k):
        for mod, param_types in cls._param_explicit.items():
            for pt, names in param_types.items():
                if k in names:
                    return f'{mod}::{pt}::{k}'
        return ParameterSet.name2fullname(k)

    @classmethod
    def matlab_code_get_vars(cls, names, trace=False,
                             allow_missing=False):
        if trace is False:
            trace = 'false'
        elif trace is True:
            trace = 'true'
        assert isinstance(trace, str)
        if allow_missing:
            trace = trace + ', allow_missing=true'
        return [
            f'{k}\t = get_var(\"{cls.name2fullname(k)}\", {trace});'
            for k in names
        ]

    @classmethod
    def matlab_code_array(cls, name, values, strings=False, comments=None,
                          multiline=False):
        contents = [f'"{v}"' if strings else str(v)
                    for v in values]
        if multiline:
            out = [
                f'{name} = [ ...',
            ]
            contents = [f'    {x}, ...' for x in contents]
            if comments:
                contents = [f'{x} \t% {comments[i]}' for
                            i, x in enumerate(contents)]
            out += contents + ['];']
        else:
            assert not comments
            out = f'{name} = [' + ', '.join(contents) + '];'
        return out

    @classmethod
    def matlab_code_dict(cls, name, values, strings=False,
                         string_keys=True, comments=None,
                         multiline=False):
        if string_keys:
            values = {f'"{k}"': v for k, v in values.items()}
        if strings:
            values = {k: f'"{v}"' for k, v in values.items()}
        contents = [f'{k}, {v}' for k, v in values.items()]
        if multiline:
            out = [
                f'{name} = dictionary( ...'
            ]
            contents = [f'    {x}, ...' for x in contents]
            if comments:
                contents = [f'{x} \t% {comments[i]}' for
                            i, x in enumerate(contents)]
            contents[-1] = contents[-1].rsplit(',', maxsplit=1)[0] + ');'
            out += contents
        else:
            assert not comments
            out = f'{name} = dictionary(' + ', '.join(contents) + ');'
        return out

    @classmethod
    def matlab_code_param_array(cls, name, param, **kwargs):
        kwargs.setdefault('multiline', True)
        kwargs.setdefault('comments', list(param.keys()))
        return cls.matlab_code_array(name, list(param.values()), **kwargs)

    @classmethod
    def matlab_code_strings_array(cls, name, strings, **kwargs):
        kwargs['strings'] = True
        return cls.matlab_code_array(name, strings, **kwargs)

    @classmethod
    def write_matlab_code(cls, fname, contents):
        if isinstance(contents, list):
            contents = '\n'.join(contents)
        contents = cls._generated_header + contents
        with open(fname, 'w') as fd:
            fd.write(contents)

    @classmethod
    def write_matlab_values(cls, mod, fname, allow_calc_trace=False):
        function_name = os.path.splitext(os.path.basename(fname))[0]
        lines = []
        param_types = cls.get_param_type_list(mod)
        for pt in param_types + ['CTRL', 'CALC']:
            isbase = (pt in ['VARS', 'MOD'])
            if mod == 'ALL' or pt in ['CTRL', 'CALC']:
                ipatch = cls.build_parameter_array(
                    True, mod, pt, name='out',
                    qualifiers=({'ON_DEMAND': False, 'CTRL': False}
                                if isbase else {}),
                    set_global_defaults=cls._param_defaults.get(mod, {}),
                    allow_calc_trace=allow_calc_trace,
                )
                if isbase:
                    ipatch += [
                        f'out = cat(2, out, {function_name}("CTRL"));',
                    ]
            else:
                ptvar = f'{mod}_{pt}'
                if pt == "VEL":
                    ptvar = f'{mod}_Vel'
                elif pt == "POOL":
                    ptvar = f'{mod}_Pool'
                ipatch = [
                    f'global {ptvar};',
                    f'out = {ptvar};',
                ]
            if isbase and not ipatch:
                ipatch = ['out = zeros(0, 0);']
            if ipatch:
                lines += [
                    ('else' if lines else '') + f'if (pt == "{pt}")',
                ] + [
                    f'    {x}' for x in ipatch
                ]
            if isbase:
                lines += [
                    '    if options.include_calc',
                    f'        out = cat(2, out, {function_name}("CALC"));',
                    '    else',
                    f'        names = get_names(\"{mod}\", \"CALC\");',
                    '        N_CALC = length(names);',
                    '        out = cat(2, out, zeros(1, N_CALC));',
                    '    end',
                ]
        lines += [
            'else',
            '    out = zeros(0, 0);',
            'end',
        ]
        body = lines
        lines = [
            f'function out = {function_name}(pt, options)',
        ]
        if allow_calc_trace:
            lines += [
                '    arguments',
                '        pt (1,1) string',
                '        options.trace (1,1) logical = 0',
                '        options.include_calc (1,1) logical = 0',
                '    end',
            ]
        else:
            lines += [
                '    arguments',
                '        pt (1,1) string',
                '        options.include_calc (1,1) logical = 0',
                '    end',
            ]
        lines += ['    ' + x for x in body] + [
            'end',
        ]
        cls.write_matlab_code(fname, lines)

    @classmethod
    def write_matlab_names(cls, mod, fname):
        param_types = cls.get_param_type_list(mod)
        print(mod, param_types)
        start = cls._param_cond_start.get(mod, 1)
        lines = [
            f'global {mod}_COND_START;',
            f'{mod}_COND_START = {start};',
            'if (pt == "PARAM_TYPES")',
            '    ' + cls.matlab_code_strings_array(
                'out', [x for x in param_types
                        if x not in ['MOD', 'COND', 'VEL']]
                ),
        ]
        if mod == 'ALL':
            lines += [
                'elseif (pt == "MODULES")',
                '    ' + cls.matlab_code_strings_array(
                    'out', [x for x in get_module_list() if x != 'ALL']),
                'elseif (pt == "COND")',
            ] + [
                f'    {x}' for x in cls.matlab_code_strings_array(
                    'out', cls._condition_vars, multiline=True)
            ]
        if mod == 'DynaPS':
            lines += [
                'elseif (pt == "CHILDREN")',
                '    global UseZaksNPQ;',
                '    if UseZaksNPQ == 0',
                '        out = ["RA", "XanCycle"];',
                '    else',
                '        out = ["RA", "NPQ"];',
                '    end',
            ]
        else:
            lines += [
                'elseif (pt == "CHILDREN")',
                '    ' + cls.matlab_code_strings_array(
                    'out', cls._param_children.get(mod, [])),
            ]
        matlab_var = {}
        aliases = {}
        for pt in param_types + ['CTRL', 'CALC']:
            param_names = cls.get_matlab_param_names(mod, pt)
            if not param_names:
                continue
            lines += [
                f'elseif (pt == "{pt}")',
            ] + [
                f'    {x}' for x in cls.matlab_code_strings_array(
                    'out', param_names, multiline=True)
            ]
            if pt in ['CTRL', 'CALC']:
                continue
            for x in param_names:
                param = ParameterSet.all_parameters().find(
                    x, default=None, mod=mod, pt=pt)
                if param is None:
                    continue
                if param.matlab_var:
                    assert x not in matlab_var
                    matlab_var[x] = param.matlab_var
                    xx = param.matlab_var
                    if x != xx and xx not in aliases:
                        aliases[xx] = x
                if param.aliases:
                    for xx in sorted(list(param.aliases.keys())):
                        if xx == x:
                            continue
                        assert xx not in aliases
                        aliases[xx] = x
        if matlab_var:
            lines += [
                'elseif (pt == "MATLAB_VAR")',
            ] + [
                f'    {x}' for x in cls.matlab_code_dict(
                    'out', matlab_var, strings=True, multiline=True)
            ]
        if aliases:
            lines += [
                'elseif (pt == "ALIASES")',
            ] + [
                f'    {x}' for x in cls.matlab_code_dict(
                    'out', aliases, strings=True, multiline=True)
            ]
        lines += [
            'else',
            '    out = strings(0, 0);',
            'end',
        ]
        lines = [
            f'function out = {mod}_NAMES(pt)',
        ] + ['    ' + x for x in lines] + [
            'end',
        ]
        cls.write_matlab_code(fname, lines)

    @classmethod
    def fname2module(cls, fname, ftype):
        base = os.path.splitext(os.path.basename(fname))[0]
        var = [ftype.title(), ftype.lower()]
        if ftype.lower() == 'ini':
            var += ['Initial', 'initial']
        if ftype not in var:
            var.insert(0, ftype)
        for v in var:
            if base.endswith(f'_{v}'):
                return base.split(f'_{v}')[0]
            if base.endswith(v):
                return base.split(v)[0]
        raise PatchError(f"Could not determine module from {ftype} "
                         f"file \"{fname}\"")

    @classmethod
    def patch_at_function_start(cls, contents, patch):
        regex = r'\n[ \t]*function[^\n]+\n'
        return cls.add_patch_block(contents, 'FUNCTION_INI', patch, regex)

    @classmethod
    def patch_at_function_end(cls, contents, patch):
        regex = r'\n[ \t]*end\s*$'
        return cls.add_patch_block(contents, 'FUNCTION_END', patch, regex,
                                   before_regex=True,
                                   append_if_no_match=True)

    @classmethod
    def patch_ftype(cls, args, ftype, fname=None, mod=None, **kwargs):
        if isinstance(mod, list):
            assert fname is None
            for x in mod:
                cls.patch_ftype(args, ftype, mod=x, **kwargs)
            return
        if isinstance(ftype, list):
            assert fname is None
            for x in ftype:
                cls.patch_ftype(args, x, mod=mod, **kwargs)
            return
        if fname is None:
            fname = cls.find_file(args, ftype, mod=mod)
        if isinstance(fname, list):
            for x in fname:
                cls.patch_ftype(args, ftype, fname=x, mod=mod, **kwargs)
            return
        method = getattr(cls, f'patch_{ftype.lower()}')
        cls.apply_patch(method, fname, args, mod=mod, **kwargs)

    @classmethod
    def patch_condition(cls, contents, fname, args):
        patch_begin = [
            'if Condition_patch_begin(t)',
            '    fini = 1;',
            '    return;',
            'end',
        ]
        patch_end = [
            'Condition_patch_end(t);',
        ]
        contents = cls.patch_at_function_start(
            contents, '\n'.join(patch_begin)
        )
        contents = cls.patch_at_function_end(
            contents, '\n'.join(patch_end)
        )
        return contents

    @classmethod
    def patch_sysinitial(cls, contents, fname, args):
        patch_end = [
            'SYSInitial_patch_end(Begin);',
        ]
        contents = cls.patch_at_function_end(
            contents, '\n'.join(patch_end)
        )
        return contents

    @classmethod
    def patch_drive(cls, contents, fname, args, mod=None):
        replacements = {'ode15s': 'Drive'}
        contents = cls.add_replacements(contents, replacements)
        return contents

    @classmethod
    def patch_ini(cls, contents, fname, args, mod=None, missing=None):
        if mod is None:
            mod = cls.fname2module(fname, "ini")
        if missing is None:
            missing = {}
        patch_begin = [
            'global registered_modules;',
            f'registered_modules(\"{mod}\") = true;',
        ]
        patch_end = []
        for k, v in missing.items():
            patch_begin += [
                f'global {v};',
                f'{v} = 0;',
            ]
            # TODO: Use actual default if the variable can't be located?
            vvar = cls.find_variable(contents, [k[2]], None)
            if vvar is None:
                vvar = cls.find_variable(cls.sysinitial_contents(args),
                                         [k[2]], None)
                if vvar is not None:
                    patch_end += [
                        f'global {vvar};',
                    ]
            if vvar is not None:
                patch_end += [
                    f'{v} = {vvar};',
                ]
        if patch_begin:
            contents = cls.patch_at_function_start(
                contents, '\n'.join(patch_begin)
            )
        if patch_end:
            contents = cls.patch_at_function_end(
                contents, '\n'.join(patch_end)
            )
        return contents

    @classmethod
    def build_parameter_array(cls, contents, mod, pt, name=None,
                              extends=None, extends_length=0,
                              missing=None, no_global_missing=False,
                              set_global_defaults=None,
                              allow_calc_trace=False, **kwargs):
        if name is None:
            if extends is None:
                name = f'{mod}_{pt}'
            else:
                name = f'{extends}_EXTENDED'
        try:
            param_names = cls.get_matlab_param_names(mod, pt, **kwargs)
        except FileNotFoundError:
            param_names = []
        if not param_names:
            return []
        if contents is True and missing is None:
            missing = {}
        new_missing = None if missing is None else {}
        param = OrderedDict()
        patch = []
        if extends:
            for i in range(extends_length):
                param[param_names[i]] = f'{extends}({i + 1})'
        if mod == 'RedoxReg' and pt == 'RC':
            N = int(len(param_names) / 2)
            for i, x in enumerate(param_names):
                if i < extends_length:
                    continue
                if i < N:
                    param[x] = f'RedoxReg_MP({i + 1}, 3)'
                else:
                    param[x] = f'RedoxReg_MP({i + 1 - N}, 2)'
        elif mod == 'RedoxReg' and pt == 'POOL':
            for i, x in enumerate(param_names):
                if i < extends_length:
                    continue
                j = int(i / 2)
                if (i % 2) == 0:
                    param[x] = f'RedoxReg_MP({j + 1}, 2)'
                else:
                    param[x] = f'RedoxReg_MP({j + 1}, 3)'
        elif pt == 'CALC':
            for x in param_names[extends_length:]:
                param[x] = cls._param_defaults[mod][x]
        else:
            for x in param_names[extends_length:]:
                param[x] = cls.find_param(contents, x, mod, pt,
                                          missing=new_missing)
                if contents is True:
                    new_missing[(mod, pt, x)] = param[x]
        if missing is None:
            new_missing = {}
        else:
            missing.update(new_missing)
        patch = []
        if allow_calc_trace and pt != 'CALC':
            patch += [
                'if options.trace',
                f'    error(\"trace not supported for {pt}\");',
                'end',
            ]
        if pt == 'CALC':
            required_vars = []
            required_calc = []

            def add_vars(k):
                for xx in cls.find_all_variables(k):
                    if xx in cls._param_defaults[mod]:
                        if xx not in required_calc:
                            add_vars(cls._param_defaults[mod][xx])
                            if xx not in required_calc:
                                required_calc.append(xx)
                    else:
                        if xx not in required_vars:
                            required_vars.append(xx)

            for k in param.values():
                add_vars(k)
            trace = 'options.trace' if allow_calc_trace else False
            patch += cls.matlab_code_get_vars(required_vars, trace=trace,
                                              allow_missing=True)
            for xx in required_calc:
                patch += [
                    f'{xx} \t= {cls._param_defaults[mod][xx]};'
                ]
                if xx in param:
                    param[xx] = xx
        elif not no_global_missing:
            patch += cls.matlab_code_global_vars(
                list(new_missing.values()), defaults=set_global_defaults)
        patch += cls.matlab_code_param_array(name, param)
        return patch

    @classmethod
    def patch_mb(cls, contents, fname, args, mod=None, **kwargs):
        if mod is None:
            mod = cls.fname2module(fname, "MB")
        fname_rate = cls.find_file(args, "Rate", mod=mod)
        if fname_rate:
            return contents
        return cls.patch_rate(contents, fname, args, mod=mod, **kwargs)

    @classmethod
    def patch_rate(cls, contents, fname, args, mod=None, missing=None):
        if mod is None:
            mod = cls.fname2module(fname, "rate")
        param_vars = {}
        patch = []
        for k in args.all_param_types:
            v = cls.find_patch_var(mod, k, contents)
            if v:
                if k in cls._param_array_length.get(mod, {}):
                    vextends = v
                    v = f'{vextends}_EXTENDED'
                    patch += cls.build_parameter_array(
                        contents, mod, k, name=v, missing=missing,
                        extends=vextends,
                        extends_length=cls._param_array_length[mod][k])
                param_vars[k] = v
            elif not args.implicit_missing:
                v = f'{mod}_{k}'
                ipatch = cls.build_parameter_array(
                    contents, mod, k, name=v, missing=missing)
                if ipatch:
                    patch += ipatch
                    param_vars[k] = v
        assert 'COND' in param_vars
        patch += [
            f'export_mod_data(\"{mod}\", t, {param_vars["COND"]}',
        ]
        if args.implicit_missing:
            patch[-1] += ', ...'
            patch.append('                create_missing=true')
        for k, v in param_vars.items():
            if k == 'COND':
                continue
            patch[-1] += ', ...'
            patch.append(f'                {k}={v}')
        patch[-1] += ');'
        contents = cls.patch_at_function_end(contents, '\n'.join(patch))
        return contents


class docs(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        args.target = 'docs'
        args.component = 'docs'
        args.with_asan = False
        args.build_type = 'Debug'
        args.dont_build = False
        args.scikit_build = False
        args.isolated_scikit_build = False
        args.dont_install = True
        args.only_python = False
        args.force_scoped_enum = False
        args.with_coverage = False
        args.with_yggdrasil = False
        args.build_docs = True
        super(docs, cls).adjust_args(args)

    def __init__(self, args, config_args=None, build_args=None,
                 install_args=None):
        self.adjust_args(args)
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        if install_args is None:
            install_args = []
        config_args.append(
            build.cmake_bool_flag('DOXYGEN_CHECK_MISSING', True))
        super(docs, self).__init__(
            args, config_args=config_args,
            build_args=build_args,
            install_args=install_args,
        )


class coverage(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        args.with_coverage = True
        args.build_tests = True
        return super(coverage, cls).adjust_args(args)

    def __init__(self, args, config_args=None, build_args=None):
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        cmds = ['make coverage']
        super(coverage, self).__init__(
            args, cmds=cmds, config_args=config_args,
            build_args=build_args
        )


class preprocess(SubTask):

    def __init__(self, args):
        cmds = [
            f'clang -E {args.file} -I {_source_dir}/include/'
        ]
        if args.test_macros:
            if not args.defines:
                args.defines = []
            args.defines.append('DO_TEST_MACROS_PREPROCESS=1')
            if args.show_passed_macro_tests:
                args.defines.append('SHOW_PASSED_MACRO_TESTS=1')
            generate_macros = os.path.join(_utils_dir,
                                           'generate_macros.py')
            result_macros = os.path.join(_source_dir, 'include',
                                         'macros_iter.hpp')
            cmds.insert(0, (f'python {generate_macros} {result_macros}'
                            f' --overwrite'))
        if args.defines:
            for x in args.defines:
                cmds[-1] += f' -D {x}'
        if args.capture and not args.output_file:
            args.output_file = 'preprocess.txt'
        super(preprocess, self).__init__(args, cmds=cmds,
                                         output_file=args.output_file,
                                         allow_error=True)


class convert_matlab_param_task(SubTask):

    _npq_keys = [
        'XanCycle::MOD::XanCycle2FIBF_Kd_NPQ',
        'XanCycle::COND::PsbSQ', 'XanCycle::RC::k_ze',
        'XanCycle::RC::hill_psbs', 'XanCycle::RC::pK_psbs',
        'XanCycle::RC::hill_vde', 'XanCycle::RC::pK_vde',
        'XanCycle::RC::kvde_max', 'XanCycle::RC::psbsQ_converRate',
        'XanCycle::RC::Fpsbs',
        'ALL::VARS::expr_psbs', 'ALL::VARS::QH',
        'ALL::VARS::one_minus_QH',
        'XanCycle::VEL::vpsbs_act',
        'XanCycle::VEL::vpsbs_deact',
    ]
    _npq_keys_shared = [
        'XanCycle::COND::Vx', 'XanCycle::COND::Ax', 'XanCycle::COND::Zx',
        'XanCycle::VEL::Vva', 'XanCycle::VEL::Vaz',
        'XanCycle::VEL::Vza', 'XanCycle::VEL::Vav',
    ]
    _npq_keys_skip = [
        'XanCycle::RC::kav',
        'XanCycle::RC::kaz',
        'XanCycle::RC::kva',
        'XanCycle::RC::kza',
        'XanCycle::COND::ABA',
        'XanCycle::MOD::XanCycle2FIBF_Xstate',
    ]

    def __init__(self, args, **kwargs):
        super(convert_matlab_param_task, self).__init__(args, **kwargs)
        if args.dst_language == 'cpp':
            self.convert_from_matlab(
                args.src, dst=args.dst, use_zaks_npq=args.use_zaks_npq)
        elif args.dst_language == 'matlab':
            self.convert_to_matlab(
                args.src, dst=args.dst, use_zaks_npq=args.use_zaks_npq)
        else:
            raise NotImplementedError(args.dst_language)

    @classmethod
    def normalize_param(cls, args, src, dst=False, src_language=None,
                        dst_language=None, cmp_language=None,
                        skip_keys=None, transform_keys=None,
                        ftype=None, **kwargs):
        if skip_keys is None:
            skip_keys = []
        else:
            skip_keys = copy.deepcopy(skip_keys)
        if transform_keys is None:
            transform_keys = {}
        not_selected = ParameterSet.all_parameters().select(
            args=args, inverse=True)
        skip_keys += list(not_selected.keys())
        if args.use_zaks_npq:
            if src_language == 'matlab' and dst_language != 'matlab':
                for k in cls._npq_keys + cls._npq_keys_shared:
                    if 'XanCycle::' not in k:
                        continue
                    transform_keys[k.replace('XanCycle::', 'NPQ::')] = k
            elif dst_language == 'matlab' and src_language != 'matlab':
                for k in cls._npq_keys + cls._npq_keys_shared:
                    if 'XanCycle::' not in k:
                        continue
                    transform_keys[k] = k.replace('XanCycle::', 'NPQ::')
            skip_keys += cls._npq_keys_skip
        else:
            skip_keys += cls._npq_keys
        if src_language == 'matlab' and (
                dst_language != 'matlab'
                or (cmp_language and cmp_language != 'matlab')):
            for mod, added_pt in instrument_matlab._param_names_extra.items():
                for pt, added_vars in added_pt.items():
                    skip_keys += [f'{mod}::{pt}::{v}' for v in
                                  added_vars.values()]
        elif src_language != 'matlab' and (dst_language == 'matlab' or
                                           cmp_language == 'matlab'):
            for mod, rm_pt in instrument_matlab._param_names_remove.items():
                for pt, rm_vars in rm_pt.items():
                    skip_keys += [f'{mod}::{pt}::{v}' for v in rm_vars]
        kwargs.setdefault('sort', True)
        if src_language is not None and 'header' not in kwargs:
            kwargs['header'] = f'# {src_language.upper()}\n'
            if isinstance(src, str):
                kwargs['header'] += f'# {src}\n'
        if isinstance(src, dict):
            param = src
        elif ftype == 'trace':
            df = read_param_table(src, no_title=True)
            param = {k: np.array(df[k]) for k in df.columns}
        else:
            param = read_param(src)
        for k in skip_keys:
            param.pop(k, None)
        for k, v in transform_keys.items():
            if k in param:
                param[v] = param.pop(k)
        if dst:
            if ftype == 'trace':
                write_param_table(dst, param, title=False)
            else:
                write_param(dst, param, **kwargs)
        return param

    @classmethod
    def convert_from_matlab(cls, src, dst=False, use_zaks_npq=False,
                            **kwargs):
        kwargs.setdefault('sort', True)
        skip_keys = []  # TODO
        transform_keys = {}
        if use_zaks_npq:
            for k in ['::COND::Vx', '::COND::Ax', '::COND::Zx',
                      '::RC::hill_psbs', '::RC::pK_psbs',
                      '::RC::hill_vde', '::RC::pK_vde', '::RC::kvde_max',
                      '::RC::psbsQ_converRate', '::RC::Fpsbs']:
                transform_keys[f'NPQ{k}'] = f'XanCycle{k}'
        param = src if isinstance(src, dict) else read_param(src)
        for k in skip_keys:
            param.pop(k, None)
        for k, v in transform_keys.items():
            if k in param:
                param[v] = param.pop(k)
        if dst:
            write_param(dst, param, **kwargs)
        return param

    @classmethod
    def convert_to_matlab(cls, src, dst=False, use_zaks_npq=False,
                          **kwargs):
        kwargs.setdefault('sort', True)
        skip_keys = [
            # Not used by either model
            # "ALL::VARS::GLight",
            # Not named variables in Matlab
            # "PS::MOD::KE1Ratio",
            # "PS::MOD::KE2Ratio",
            # "SUCS::MOD::KE5Ratio",
            # Add for explicit definition of Zhu 2012 parameters
            # "FIBF::RC::RC0",
            # "FIBF::RC::RC",
            # "BF::MOD::F",
            # "PS::MOD::F",
            # "FIBF::RC::kdm0",
            # "RuACT::RC::factor_n7",
            # "RuACT::RC::kn7",
            # "RuACT::RC::RCA",
        ]
        for mod, param_types in instrument_matlab._param_names_remove.items():
            for pt, names in param_types.items():
                skip_keys += [f'{mod}::{pt}::{v}' for v in names]
        transform_keys = {}  # TODO
        if use_zaks_npq:
            for k in ['::COND::Vx', '::COND::Ax', '::COND::Zx',
                      '::RC::hill_psbs', '::RC::pK_psbs',
                      '::RC::hill_vde', '::RC::pK_vde', '::RC::kvde_max',
                      '::RC::psbsQ_converRate', '::RC::Fpsbs']:
                transform_keys[f'XanCycle{k}'] = f'NPQ{k}'
        param = src if isinstance(src, dict) else read_param(src)
        for k in skip_keys:
            param.pop(k, None)
        for k, v in transform_keys.items():
            if k in param:
                param[v] = param.pop(k)
        if dst:
            write_param(dst, param, **kwargs)
        return param


class compare_files_task(SubTask):

    @classmethod
    def adjust_args(cls, args):
        if not args.expected:
            if args.filetype != 'output':
                raise RuntimeError(f'Cannot infer the expected file for '
                                   f'a filetype of \'{args.filetype}\'')
            driver_name = cls._driver_map[args.driver]
            args.expected = os.path.join(_source_dir, 'tests', 'data',
                                         f'ePhotoOutput_{driver_name}.txt')
        assert os.path.isfile(args.actual)
        assert os.path.isfile(args.expected)

    def __init__(self, args, **kwargs):
        super(compare_files_task, self).__init__(args, **kwargs)
        check_files_kwargs = {
            'label_f1': 'Actual',
            'label_f2': 'Expected',
            'reltol': args.reltol,
            'abstol': args.abstol,
            'sep': args.sep,
        }
        actual = args.actual
        expected = args.expected
        if args.filetype == 'parameter':
            actual = '_tmp'.join(os.path.splitext(actual))
            expected = '_tmp'.join(os.path.splitext(expected))
            norm_param(args.actual, actual, maxlen=45)
            self._generated_files.append(actual)
            norm_param(args.expected, expected, maxlen=45)
            self._generated_files.append(expected)
        compare_files(actual, expected, ftype=args.filetype,
                      check_files_kwargs=check_files_kwargs,
                      output_diff=args.output_diff)


if __name__ == "__main__":
    parser = InstrumentedParser(
        "CLI for performing common development related tasks to maintain"
        " the ePhotosynthesis_C model")
    subparsers = parser.add_subparsers(
        dest='task', default='build',
        help="Development task that should be performed.")
    parser_build = subparsers.add_parser(
        'build', help="Build the library",
        func=build)
    parser_readme = subparsers.add_parser(
        'update-readme', help="Update README.md with output from help",
        func=update_readme)
    parser_test = subparsers.add_parser(
        'test', help="Run tests", aliases=['tests'],
        func=test)
    parser_test.add_argument(
        '--show-tests', action='store_true',
        help="Show the set of discovered tests")
    parser_test.add_argument(
        '--verbose', action='store_true',
        help="Turn on verbose test output")
    parser_test.add_argument(
        '--stop-on-error', '-x', action='store_true',
        help="Stop running tests after the first error")
    parser_test.add_argument(
        '--preserve-output', action='store_true',
        help="Preserve test output")
    parser_test.add_argument(
        '--refresh-output', action='store_true',
        help="Refresh the copies of expected test output")

    parser_zhu2012 = subparsers.add_parser(
        'zhu2012', help="Create parameters for zhu2012",
        func=zhu2012)
    parser_zhu2012.add_argument(
        '--tables-file', type=str, default='Zhu2012_tables.csv',
        help="Name of the CSV containing all of the tables")
    parser_zhu2012.add_argument(
        '--table-base', type=str, default='Zhu2012_table_',
        help="Base name for CSVs containing individual tables")
    parser_zhu2012.add_argument(
        '--param-file', type=str, default='Zhu2012_param.txt',
        help="Parameter file that should be generated from the tables")
    parser_zhu2012.add_argument(
        '--sort-param', nargs='?', const=True, default='default_diff',
        choices=[False, True, 'comment', 'name', 'default_diff'],
        help="How parameters should be sorted in the file")
    parser_zhu2012.add_argument(
        '--split-tables', action='store_true',
        help="Split the CSV into the individual tables")
    parser_zhu2012.add_argument(
        '--make-param', action='store_true',
        help='Regenerate the parameter file')
    parser_zhu2012.add_argument(
        '--complete-param', action='store_true',
        help='Filling in missing C++ parameter names in the tables')
    parser_zhu2012.add_argument(
        '--ignore-existing-names', action='store_true',
        help='Don\' use \"C++ Parameter\" values from tables')
    parser_zhu2012.add_argument(
        '--inspect-missing', action='store_true',
        help=('Step through parameters that don\'t have an assigned '
              'version for the C++ code'))
    parser_zhu2012.add_argument(
        '--include-missing', action='store_true',
        help=('Include parameters that don\'t have an assigned '
              'version for the C++ code'))
    parser_zhu2012.add_argument(
        '--exclude-param', type=str, action='extend',
        help="Names of parameters that should be excluded")
    parser_zhu2012.add_argument(
        '--inspect-param', type=str, action='append',
        help="Names of parameters that should be inspected")
    parser_zhu2012.add_argument(
        '--min-diff', type=float, default=np.inf,
        help=("Minimum difference that there should be between the "
              "Zhu 2012 parameter value and the default C++ code value "
              "to include it in the generated parameter file"))
    parser_zhu2012.add_argument(
        '--verbose', action='store_true',
        help="Turn on verbose parameter matching output")

    parser_ephoto = subparsers.add_parser(
        'ephoto', help="Run ephoto executable",
        func=ephoto)

    parser_iterations = subparsers.add_parser(
        'create-iterations', help="Create an iterations file",
        func=create_iterations)
    parser_iterations.add_argument(
        '--result-file', type=str,
        nargs='?', const='TimeIterationResult.txt',
        help=(
            "Location where the results for the output time steps "
            "should be saved"
        ),
    )
    parser_iterations.add_argument(
        '--low-light-level', '--light-level', type=float,
        help=(
            "Light level for the low light condition for a two-level "
            "light profile or the constant light condition for a "
            "steady-state light profile."
        ),
    )
    parser_iterations.add_argument(
        '--high-light-level', type=float,
        help="Light level for the high light condition.",
    )
    parser_iterations.add_argument(
        '--tstart-light-change', type=float,
        help=(
            'Time after which the change in light should be triggered.'
            'Valid for light profiles of "light-dark-light" and '
            '"dark-light-dark."'
        ),
    )
    parser_iterations.add_argument(
        '--duration-light-change', type=float,
        help=(
            'Time that change should persist before being reverted. '
            'Valid for light profiles of "light-dark-light" and '
            '"dark-light-dark."'
        ),
    )
    # parser_yggdrasil = subparsers.add_parser(
    #     'yggdrasil',
    #     help="Return information about the yggdrasil interface library",
    #     func=ygginfo)

    # Trace analysis
    parser_analyze_trace = subparsers.add_parser(
        'analyze-trace', help="Analyze an output trace",
        func=analyze_trace)
    parser_analyze_trace.add_argument(
        'result_file', type=str, nargs='+', action='extend',
        help="Path to one or more files containing ePhotosynthesis traces",
    )
    parser_analyze_trace.add_argument(
        '--plot-labels', type=str, nargs='+', action='extend',
        help="Labels for different files",
    )

    parser_docs = subparsers.add_parser(
        'docs', help="Build the docs",
        func=docs)
    parser_preprocess = subparsers.add_parser(
        'preprocess', help="Preprocess a source file",
        func=preprocess)
    parser_preprocess.add_argument(
        'file', type=str, help="File to preprocess")
    parser_preprocess.add_argument(
        '--output-file', type=str,
        help="File to direct preprocess output into")
    parser_preprocess.add_argument(
        '-D', '--define', dest='defines', nargs='*', action='extend',
        help='Definitions to pass to the preprocessor'
    )
    parser_preprocess.add_argument(
        '--test-macros', action='store_true',
        help='Enable macro testing'
    )
    parser_preprocess.add_argument(
        '--show-passed-macro-tests', action='store_true',
        help='Show results from passed macro tests'
    )
    parser_preprocess.add_argument(
        '--capture', action='store_true',
        help='Capture the output to preprocess.txt if output_file not set'
    )
    parser_coverage = subparsers.add_parser(
        'coverage', help="Check test coverage",
        func=coverage)

    # File comparison
    parser_compare_files = subparsers.add_parser(
        'compare-files', help="Compare two files",
        func=compare_files_task)
    parser_compare_files.add_argument(
        'actual', type=str, help="Actual result file for comparison")
    parser_compare_files.add_argument(
        '--expected', type=str,
        help=("File containing expected result. If not "
              "provided, the ePhoto input arguments will "
              "be used to determine which file contains "
              "the expected result."))
    parser_compare_files.add_argument(
        '--filetype', type=str, default="output",
        help="Type of file being compared")
    parser_compare_files.add_argument(
        '--output-diff', type=str,
        help='File where the diff should be saved')

    # Instrument raw MATLAB model
    parser_instrument_matlab = subparsers.add_parser(
        'instrument-matlab',
        help="Patch a version of the matlab code to allow comparison",
        func=instrument_matlab)
    parser_instrument_matlab.add_argument(
        '--remove-patch', action='store_true',
        help='Reverse the patch')
    parser_instrument_matlab.add_argument(
        '--force-reset', action='store_true',
        help=(
            'Force the removal of patches even when the flag is not '
            'present'
        ),
    )
    parser_instrument_matlab.add_argument(
        '--patch-extra', action='store_true',
        help='Patch modules not implemented by the C++ model')
    parser_instrument_matlab.add_argument(
        '--implicit-missing', action='store_true',
        help='Don\'t define missing parameters.')
    parser_instrument_matlab.add_argument(
        '--generate-functions', action='store_true',
        help=(
            'Generate MATLAB helper functions for outputing parameter '
            'names for a given module'
        ),
    )

    # Method to convert files
    parser_convert_param = subparsers.add_parser(
        'convert-param',
        help='Convert parameters files between C++ & MATLAB',
        func=convert_matlab_param_task)
    parser_convert_param.add_argument(
        'src', type=str, help="Source file")
    parser_convert_param.add_argument(
        'dst', type=str, help="Destination file")
    parser_convert_param.add_argument(
        '--dst-language', '--to',
        type=str, choices=['cpp', 'matlab'], default='matlab',
        help='Destination language')

    requires_build = [
        'update-readme',
        'test',
        'ephoto',
        'coverage',
    ]
    build_tasks = ['build'] + requires_build
    ephoto_tasks = ['ephoto']
    compare_tasks = ['compare-files']
    analysis_tasks = ephoto_tasks + ['analyze-trace']
    matlab_tasks = ['instrument-matlab']

    # Build arguments
    parser.add_argument(
        '-j', '--njobs', type=int, default=8,
        help="Number of process to use for make build",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--rebuild', action='store_true',
        help=("Remove the existing build before "
              "performing the task"),
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--build-dir', type=str,
        default=os.path.join(_source_dir, 'build'),
        help="Build directory",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--install-dir', type=str,
        help="Install directory",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--install-dir-python', type=str,
        help="Install directory for Python package",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--dont-install', action='store_true',
        help="Don't install the library",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--with-asan', action='store_true',
        help="Build with ASAN & UBSAN enabled",
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--build-type', type=str, default="Debug",
        help="Type of build",
        choices=["Debug", "Release"],
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--target', type=str,
        help="Target to build",
        # choices=[
        #     "EPhotosynthesis", "ePhoto", "pyPhotosynthesis", "docs",
        # ],
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--component', type=str,
        help="Component to install",
        # choices=[
        #     "CXX", "Python", "docs",
        # ],
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--make-equivalent-to-matlab', action='store_true',
        help=("Build with changes enabled to make the model equivalent "
              "to the MATLAB version of the model"),
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--build-tests', action='store_true',
        help="Build the tests",
        subparsers={'task': [x for x in build_tasks if x != 'test']})
    parser.add_argument(
        '--build-docs', action='store_true',
        help="Build the documentation",
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--with-python', action='store_true',
        help="Build the Python interface",
        subparsers={'task': build_tasks + ['docs']})
    parser.add_argument(
        '--with-coverage', action='store_true',
        help="Build the coverage tests",
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--only-python', action='store_true',
        help="Only run the Python tests",
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--force-scoped-enum', action='store_true',
        help=("Compile with 'EPHOTO_USE_SCOPED_ENUM' reguardless of the "
              "compiler (usually only used with MSVC)"),
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--dont-build', action='store_true',
        help="Don't rebuild before performing the task",
        subparsers={'task': requires_build})
    parser.add_argument(
        '--scikit-build', action='store_true',
        help="Build the Python wrapper package using scikit-build-core",
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--isolated-scikit-build', action='store_true',
        help=("Build the Python wrapper package using scikit-build-core "
              "in an isolated pip env"),
        subparsers={'task': build_tasks})
    parser.add_argument(
        '--with-yggdrasil', nargs='?', const=True,
        help=("Compile with WITH_YGGDRASIL. If \"direct\" is passed, "
              "the yggdrasil sources will be directly included."),
        subparsers={'task': build_tasks})

    # Comparison arguments
    parser.add_argument(
        "--sep", type=str, default=',',
        help="Separator for values in tables to compare",
        subparsers={'task': compare_tasks})

    # Explicit matlab arguments
    parser.add_argument(
        '--matlab', type=str, default=find_matlab(),
        help="Path to the MATLAB executable",
        subparsers={'task': matlab_tasks},
    )

    # ePhoto arguments
    parser.add_argument(
        "--driver", '-d', '--drivers', type=parse_driver,
        choices=[0, 1, 2, 3, 4, 'all'] + SubTask._drivers,
        action='extend', nargs='+',
        help="Driver to run",
        subparsers={'task': ephoto_tasks + ['compare-files']},
    )
    parser.add_argument(
        "--useC3", "--c3", action="store_true",
        help="Run the C3 version",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--use-zaks-npq', '--npq', action='store_true',
        help=(
            "Use the Zaks et al. 2012 model of non-photochemical "
            "quenching"
        ),
        subparsers={'task': ephoto_tasks + ['convert-param']},
    )
    parser.add_argument(
        "--language", "--languages", type=str,
        choices=['all'] + SubTask._languages,
        action='extend', nargs='+',
        help="Language(s) that the simulation should be run in",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--compare", action='store_true',
        help="Compare outputs from the provided drivers/languages",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--output-diff", nargs='?', type=str, const=True,
        help=(
            "Base name for paths where output file diffs should be "
            "stored during comparison",
        ),
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--dont-prune-diff", action='store_true',
        help=(
            "Don't prune diffs based on provided tolerances during "
            "comparison",
        ),
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--matlab", type=str, nargs='?', const=find_matlab(),
        help=(
            "Run the MATLAB version of the code. This argument can also "
            "be used to specify the location of the MATLAB executable.",
        ),
        subparsers={
            'task': [x for x in ephoto_tasks if x not in matlab_tasks]
        }
    )
    parser.add_argument(
        '--matlab-repo', type=str,
        default=os.path.join(
            os.path.dirname(_source_dir), 'npq',
            'ePhotosynthesis_Matthews_Internal'),
        # default=os.path.join(
        #     os.path.dirname(_source_dir), 'ePhotosynthesis'),
        help="Path to the MATLAB version of the model",
        subparsers={'task': ephoto_tasks + [x for x in matlab_tasks
                                            if x not in ephoto_tasks]},
    )
    parser.add_argument(
        "--generate-matlab-script", type=str,
        help=("Path where MATLAB script should be generated to run "
              "the MATLAB version of the model"),
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--dont-patch-matlab", action="store_true",
        help="Don\'t patch the matlab repository code",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--preserve-patch-matlab", action="store_true",
        help="Preserve the matlab repository code patches after the run",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--input-dir', type=str, default=_data_dir,
        help="Directory containing input files",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--enzyme-file', '--enzyme', type=str,
        help="File containing enzyme concentrations.",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--grn-file', '--grn', type=str,
        help="File containing transcription factor levels.",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--evn-file', '--evn', '--env', type=str,
        help="File containing environmental parameters.",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--atpcost-file', '--atp', type=str,
        help="File containing ATP cost",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-dir', type=str, default=os.getcwd(),
        help="Directory where output should be saved",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-file', '--output', type=str, default='output.data',
        help="File where driver output should be saved",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-suffix', type=str, const=True, nargs='?',
        help="Suffix to add to output files.",
        subparsers={'task': ephoto_tasks + ['create-iterations']})
    parser.add_argument(
        "--output-param", choices=[0, 1, 2, 3], type=int, default=0,
        help="Flag specifying when to output parameters",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        "--output-param-base", type=str, default="param_",
        help="File prefix for output parameter files",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        "--preserve-output-param", action="store_true",
        help="Don't cleanup output param files (first, final & trace)",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--dont-run', action='store_true',
        help="Don\'t run ephotosynthesis, only post-process",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        "--dont-run-language", type=str, action='extend', nargs='+',
        help=(
            "Languages that should not be run during comparison. "
            "It will be assumed that the language has already been run "
            "with the required parameters and that the required "
            "output files for comparison already exist."
        ),
    )
    parser.add_argument(
        '--begintime', type=float, default=0,
        help="Begin time for the run (in seconds)",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--stoptime', type=float, default=250,
        help="Stop time for the run (in seconds)",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--stepsize', type=float, default=1.0,
        help=(
            "The initial step size (in seconds). Also used as the "
            "interval for outputing parames if that is enabled."
        ),
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        "--abstol", type=float, default=1.0e-05,
        help="Absolute tolerance to use when comparing output files",
        subparsers={'task': ephoto_tasks + ['compare-files']})
    parser.add_argument(
        "--reltol", type=float, default=1.0e-04,
        help="Relative tolerance to use when comparing output files",
        subparsers={'task': ephoto_tasks + ['compare-files']})
    parser.add_argument(
        '--param', type=cli_param, action='extend',
        help="Parameter values that should be set",
        subparsers={'task': ephoto_tasks + ['create-iterations']},
    )
    parser.add_argument(
        '--iterations-file', '--iterations', type=str,
        subparsers={'task': ephoto_tasks + ['create-iterations']},
    )
    parser.add_argument(
        '--light-profile', choices=_light_profiles,
        help=("Create an input file that produces a desired light "
              "profile"),
        subparsers={'task': ephoto_tasks + ['create-iterations']},
        subparser_defaults={
            'create-iterations': 'Zhu2012',
        },
    )
    parser.add_argument(
        '--overwrite-light-profile', action='store_true',
        help="Overwrite any existing light_profile.",
        subparsers={'task': ephoto_tasks + ['create-iterations']},
        subparser_defaults={
            'create-iterations': True,
        },
    )
    parser.add_argument(
        '--steady-state-start', type=str,
        nargs='?', const=True,
        help=(
            "Initialize the system by running it to a steady state. "
            "Only valid if --light-profile set."
        ),
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--match-figure', type=str, choices=_figures,
        help="Recreate the named figure from a related publication",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--match-param',  type=str, choices=_param_sets,
        # nargs='?', const=True,
        help="Run with the parameters explicitly set to match the paper",
        subparsers={'task': ephoto_tasks},
    )
    parser.add_argument(
        '--match-param-to-matlab', type=str, nargs='?', const=True,
        help=(
            "Run with the parameters explicitly set to match those "
            "used by the MATLAB version of the code."
        ),
        subparsers={'task': ephoto_tasks},
    )

    # Analysis options
    # analysis_tasks
    parser.add_argument(
        '--plot-file', type=str,
        nargs='?', const=True,
        help="Location where the generated plot should be saved",
        subparsers={'task': analysis_tasks},
        subparser_defaults={'analyze-trace': True},
    )
    parser.add_argument(
        '--plot-ncol', type=int,
        help="Number of columns in figure.",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-var', '--var', type=str, action='append',
        help="Variable that should be plot against time",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-AvL', action='store_true',
        help="Include a plot of assimilation rate vs. light level.",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-tmin', type=float,
        help="Minimum time that should be plot",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-tmax', type=float,
        help="Maximum time that should be plot",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-tline', type=float, action='append', nargs='*',
        help="Time to draw a line at",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--find-inflections', type=str,
        nargs='?', const=True,
        help=(
            "Location where a plot should be saved showing the "
            "parameters with the largest changes in their second "
            "derivative"
        ),
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--plot-trace-diffs', type=str,
        nargs='?', const=True,
        help=(
            "Location where a plot should be saved showing the "
            "parameters with the largest diffs in their traces"
        ),
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--inflection-tmin', type=float,
        help="Minimum time that should be checked for inflections",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--inflection-tmax', type=float,
        help="Maximum time that should be checked for inflections",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--inflection-tol', type=float, default=20,
        help="Tolerance for finding inflection points",
        subparsers={'task': analysis_tasks},
    )
    parser.add_argument(
        '--inflection-N', '--trace-diff-N', type=float, default=25,
        help=(
            "If --find-inflections or --plot-trace-diffs is specified, "
            "this is the number "
            "of parameters that will plotted, selecting those with the "
            "largest changes in their second derivative."
        ),
        subparsers={'task': analysis_tasks},
    )

    # Universal arguments
    parser.add_argument(
        "--dont-cleanup", action='store_true',
        help="Don't clean up any files generated by the task",
        subparsers={'task': 'all'})

    args = parser.parse_args()
    parser.run_subparser('task', args)
