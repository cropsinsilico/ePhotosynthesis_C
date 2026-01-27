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
from collections import OrderedDict
import matplotlib.pyplot as plt
import numpy as np
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


def cli_param(x):
    k, v = x.split(':')
    return k, float(v)


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


def get_matlab_aliases(reverse=False, strip_prefix=False):
    aliases = {
        "BF::POOL::kA_d": "BF::POOL::Tcyt",
        "BF::POOL::kA_f": "BF::POOL::Tcytc2",
        "BF::POOL::kA_U": "BF::POOL::TK",
        "BF::POOL::kU_A": "BF::POOL::TMg",
        "BF::POOL::kU_d": "BF::POOL::TCl",
        "BF::POOL::kU_f": "BF::POOL::TFd",
        "BF::POOL::k1": "BF::POOL::TA",
        "BF::POOL::k_r1": "BF::POOL::TQ",
        "BF::POOL::kz": "BF::POOL::BFTs",
        "BF::POOL::k12": "BF::POOL::BFTl",
        "BF::POOL::k23": "BF::POOL::P700T",
        "BF::POOL::k30": "BF::POOL::NADPHT",
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


def read_default_param(with_prefixes=False):
    defaults = OrderedDict()
    param_files = sorted(glob.glob(os.path.join(_param_dir, '*.txt')))
    for param_file in param_files:
        base = os.path.splitext(os.path.basename(param_file))[0]
        if base in ["README", "RedoxReg_MP", "VAR",
                    "PR_RC", "PS_RC", "SUCS_RC", "XanCycle_RC"]:
            continue
        mod, pt = base.rsplit('_', maxsplit=1)
        iparam = read_param(param_file, default=True)
        if with_prefixes:
            for k, v in iparam.items():
                kp = '::'.join([mod, pt, k])
                defaults[kp] = iparam
        else:
            defaults.setdefault(mod, OrderedDict())
            defaults[mod][pt] = read_param(param_file, default=True)
    return defaults


def read_param(fname, default=False):
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
            out[name] = {'value': float(fields[1]),
                         'comment': comment.strip()}
            if len(fields) == 2:
                out[name]['value_c3'] = out[name]['value']
            elif len(fields) == 3:
                out[name]['value_c3'] = float(fields[2])
            else:
                raise ValueError(f"More than 3 fields: \"{x}\"")
        else:
            name, value = x.rsplit(maxsplit=1)
            out[name] = float(value)
    return out


def read_param_table(fname):
    import pandas as pd
    with open(fname, 'r') as fd:
        title = fd.readlines()[0].strip()
        # df = pd.read_csv(fd)
    df = pd.read_csv(fname, header=1)
    return df, title


def write_param_table(fname, param, title=None):
    import pandas as pd
    if title is None:
        title = ''
    if not isinstance(param, pd.DataFrame):
        param = pd.DataFrame(param)
    assert isinstance(param, pd.DataFrame)
    with open(fname, 'w') as fd:
        fd.write(title + '\n')
        param.to_csv(fd, index=False)


def write_param(fname, param, sort=False, comment_incomplete=False,
                exclude_param=None):
    if exclude_param is None:
        exclude_param = []
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
    if len(contents) == 1:
        names = ['CO2AR']
    else:
        names = contents[0].strip().split(sep)
    values = [float(x) for x in contents[-1].strip().split(sep)]
    out = {k: v for k, v in zip(names, values)}
    return out


def check_output(f1, f2, reltol=1.0e-05, abstol=1.0e-08, sep=',',
                 label_f1='file A', label_f2='file B'):
    import numpy as np
    x1dict = read_output_table(f1, sep=sep)
    x2dict = read_output_table(f2, sep=sep)
    out = True
    for k in x1dict.keys():
        if k not in x2dict:
            print(f"{k} missing from {label_f2}")
    for k in x2dict.keys():
        if k not in x1dict:
            print(f"{k} missing from {label_f1}")
    for k in x1dict.keys():
        x1 = x1dict[k]
        x2 = x2dict[k]
        iout = np.isclose(x1, x2, rtol=reltol, atol=abstol)
        if not iout:
            out = False
            reldiff = np.abs(x1 - x2) / x2
            absdiff = np.abs(x1 - x2)
            print(f"Values differ: {x1} vs {x2}\n"
                  f"    Relative diff: {reldiff} (reltol = {reltol})\n"
                  f"    Absolute diff: {absdiff} (abstol = {abstol}\n")
    return out


def compare_files(f1, f2, check_files=None, ftype='parameter',
                  check_files_kwargs={}):
    with open(f1, 'r') as fd:
        lines1 = fd.readlines()
    with open(f2, 'r') as fd:
        lines2 = fd.readlines()
    line_diff = list(difflib.unified_diff(lines1, lines2,
                                          fromfile=f1, tofile=f2))
    if line_diff:
        if ftype == 'output' and check_files is None:
            check_files = check_output
        if (((check_files is not None)
             and check_files(f1, f2, **check_files_kwargs))):
            return
        print(line_diff)
        line_diff = '\n'.join(line_diff)
        raise RuntimeError(
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

    @classmethod
    def adjust_args(cls, args):
        pass

    def __init__(self, args, dont_cleanup=False, **kwargs):
        self.current_args = args
        self.dont_cleanup = dont_cleanup
        self._generated_files = []
        self.adjust_args(args)
        self.run_commands(args, **kwargs)

    def __del__(self):
        self.cleanup_files()

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
                if not os.path.isabs(v):
                    if prefix:
                        v = os.path.join(prefix, v)
                    if not os.path.isabs(v):
                        v = os.path.abspath(v)
                setattr(args, k, os.path.expanduser(v))

    @classmethod
    def suffix_path_args(cls, args, names, suffix):
        for k in names:
            k0 = f'original_{k}'
            if not hasattr(args, k0):
                setattr(args, k0, getattr(args, k))
            v0 = getattr(args, k0)
            if v0:
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
        cmdS = '\n\t'.join(cmds)
        print(f"Running\n{cmdS}\n"
              f"with {pprint.pformat(kwargs)}")
        if kwargs.get('env', None):
            kwargs['env'] = dict(os.environ, **kwargs['env'])
        output_str = b''
        if output_file:
            kwargs.update(capture_output=True)
        for x in cmds:
            ires = subprocess.run(x.split(), **kwargs)
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
        if args.make_equivalent_to_matlab:
            config_args += ['-DMAKE_EQUIVALENT_TO_MATLAB:BOOL=ON']
        else:
            config_args += ['-DMAKE_EQUIVALENT_TO_MATLAB:BOOL=OFF']
        if args.with_asan:
            config_args += ['-DWITH_ASAN:BOOL=ON']
        if args.with_coverage:
            config_args += ['-DTEST_COVERAGE:BOOL=ON']
        if args.target not in [None, 'EPhotosynthesis', 'ePhoto']:
            config_args += ['-DBUILD_CXX:BOOL=OFF']
        if args.with_python or args.target == 'pyPhotosynthesis':
            config_args += ['-DBUILD_PYTHON:BOOL=ON']
            if not (args.dont_install or for_scikit_build):
                config_args += [
                    f'-DINSTALL_PREFIX_PYTHON={args.install_dir_python}'
                ]
        if not (args.dont_install or for_scikit_build):
            config_args += [f'-DCMAKE_INSTALL_PREFIX={args.install_dir}']
        if args.force_scoped_enum:
            config_args += ['-DEPHOTO_USE_SCOPED_ENUM:BOOL=ON']
        if args.with_yggdrasil:
            config_args += ['-DWITH_YGGDRASIL:BOOL=ON']
        if args.with_yggdrasil == 'direct':
            config_args += ['-DBUILD_WITH_YGGINTERFACE:BOOL=ON']
        return config_args

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

    def __init__(self, args, test_flags=None,
                 config_args=None, build_args=None, **kwargs):
        self.adjust_args(args)
        if test_flags is None:
            test_flags = []
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        if args.target != 'pyPhotosynthesis':
            config_args += ['-DBUILD_TESTS:BOOL=ON']
        test_flags += ['-C', args.build_type]
        pytest_flags = ['-sv']
        if args.preserve_output:
            config_args += ['-DPRESERVE_TEST_OUTPUT:BOOL=ON']
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
                for drv in compare_matlab._drivers:
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

    def get_original(self, k0):
        for v in self.values():
            if v.original_name == k0:
                return v
        raise KeyError(k0)


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
                 original_value=None, conversion=None):
        if aliases is None:
            aliases = []
        if isinstance(aliases, dict):
            aliases = list(aliases.values())
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
        self.aliases = OrderedDict()
        self.default = default
        self.choices = OrderedDict()
        self.conversion = conversion
        self.default_diff = np.nan
        for v in aliases:
            self.add_alias(v)

    def __str__(self):
        return self._make_string()

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

    direct_args = ['stoptime']

    @classmethod
    def adjust_args(cls, args):
        if isinstance(args.param, list):
            args.param = OrderedDict(*args.param)
        cls.prefix_path_args(args, ['input_dir', 'output_dir'])
        cls.prefix_path_args(args, ['enzyme_file', 'grn_file',
                                    'evn_file', 'atpcost_file',
                                    'iterations_file'],
                             prefix=args.input_dir)
        cls.prefix_path_args(args, ['output_file', 'output_param_base'],
                             prefix=args.output_dir)
        if args.output_suffix:
            if args.output_suffix is True:
                args.output_suffix = '_' + cls._driver_map[args.driver]
            cls.suffix_path_args(
                args, ['output_file', 'output_param_base'],
                args.output_suffix)
        if not os.path.isdir(args.output_dir):
            os.mkdir(args.output_dir)
        if args.dont_run:
            args.dont_build = True
        super(ephoto, cls).adjust_args(args)

    def run_commands(self, args, cmds=None, ephoto_args=None, **kwargs):
        if args.driver == 0 and cmds is None:
            self.iter_drivers(self.run_commands, args, **kwargs)
            return
        if ephoto_args is None:
            ephoto_args = []
        if cmds is None and not args.dont_run:
            execFile = os.path.join(args.build_dir, 'ePhoto')
            if args.param or args.evn_file:
                exclusive_param = [
                    ('ALL::VARS::O2_cond', 'ALL::VARS::O2',
                     'O2_cond', 'O2'),
                    ('ALL::VARS::CO2_cond', 'ALL::VARS::CO2_in',
                     'CO2_cond', 'CO2_in', 'CO2'),
                    ('ALL::VARS::TestLi',  'ALL::VARS::TestLi_Wps',
                     'TestLi', 'TestLi_Wps', 'PFD'),
                ]
                if args.evn_file:
                    param = read_param(args.evn_file)
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
                args.evn_file = os.path.join(
                    args.input_dir, 'GeneratedEvn.txt'
                )
                write_param(args.evn_file, args.param)
                self._generated_files.append(args.evn_file)
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
            cmds = [
                f'{execFile} -d {args.driver} '
                f'{" ".join(ephoto_args)}',
                f'cat {args.output_file}'
            ]
            self._generated_files += [args.output_file]
            if args.output_param_base:
                cmds[0] += f' --outputParamBase {args.output_param_base}'
                self._generated_files += [
                    args.output_param_base + 'init.txt',
                    args.output_param_base + 'last.txt',
                ]
            if args.output_param:
                cmds[0] += f' --outputParam {args.output_param}'
                if args.output_param >= 3:
                    self._generated_files += [
                        args.output_param_base + 'step*.txt'
                    ]
            if args.iterations_file:
                cmds[0] += f' --iterations {args.iterations_file}'
            if args.useC3:
                cmds[0] += ' --c3'
            for k in self.direct_args:
                if getattr(args, k, None):
                    cmds[0] += f' --{k} {getattr(args, k)}'
        return super(ephoto, self).run_commands(args, cmds=cmds, **kwargs)

    @classmethod
    def iter_drivers(cls, func, args, suffix_paths=None, **kwargs):
        if suffix_paths is None:
            suffix_paths = []
        suffix_paths += ['output_file', 'output_param_base']
        for i in range(len(cls._drivers)):
            args.driver = i + 1
            args.output_suffix = True
            # cls.suffix_path_args(args, suffix_paths,
            #                      '_' + cls._driver_map[args.driver])
            # func(args, **kwargs)


class ephoto_iterations(ephoto):

    def __init__(self, args, **kwargs):
        try:
            super(ephoto_iterations, self).__init__(args, **kwargs)
        finally:
            self.record_last_param(args)
            self.plot(self.read_param(args))

    @classmethod
    def adjust_args(cls, args):
        if not args.param:
            args.param = {}
        args.plot_limits = {}
        args.plot_aliases = {}
        args.result_file = True  # Force output
        if args.light_profile in ['dark-steady-state',
                                  'light-steady-state']:
            args.evn_file = False
            args.first_param_file = True
            args.final_param_file = True
        if not args.output_suffix:
            args.output_suffix = '_' + cls._driver_map[args.driver]
            if args.light_profile:
                light_profile = args.light_profile
                if light_profile in ['dark-light-dark',
                                     'light-dark-light',
                                     'dark-steady-state',
                                     'light-steady-state']:
                    if args.low_light_level is None:
                        args.low_light_level = 100
                    if args.high_light_level is None:
                        args.high_light_level = 1000
                    if light_profile == 'dark-steady-state':
                        light_profile = 'steady-state'
                        light_profile += f'_{args.low_light_level}'
                    elif light_profile == 'light-steady-state':
                        light_profile = 'steady-state'
                        light_profile += f'_{args.high_light_level}'
                    else:
                        light_profile += (
                            f'_{args.low_light_level}_to_'
                            f'{args.high_light_level}'
                        )
                args.output_suffix = (
                    f'_{light_profile}{args.output_suffix}'
                )
            if ((args.use_zaks_npq
                 and not args.light_profile.startswith('Zaks'))):
                args.output_suffix += '_ZaksNPQ'
        ROEvar = 'FI::VEL::vS3_S0'
        PSIIvar = 'fPSII'
        args.plot_aliases = {
            ROEvar: 'O2 Evolution',
            PSIIvar: 'PhiII',
            'CO2AR': 'A',
            'ALL::VARS::TestLi': 'PFD',
            'fluoresence': 'Fluoresence',
            'dissipation': 'Excitons dissipated as heat',
        }
        args.plot_units = {
            'CO2AR': 'umol m**-2 s**-1',
            'dissipation': 'umol m**-2 s**-1',
            ROEvar: 'umol m**-2 s**-1',
            'fluoresence': 'umol m**-2 s**-1',
            'MembranePotential': 'V',
            'BF::COND::Ks': 'mM',
            'BF::COND::Mgs': 'mM',
            'BF::COND::Cls': 'mM',
            'ALL::VARS::TestLi': 'umol m**-2 s**-1',
        }
        if args.light_profile == 'NPQ-explore':
            evn_base = None
            args.use_zaks_npq = True
            if evn_base == 'Zhu2012':
                args.evn_file = 'Zhu2012_param.txt'
                cls.prefix_path_args(args, ['evn_file'],
                                     prefix=_zhu2012_dir)
                args.param.update(
                    GRNC=1.0,
                    ProteinTotalRatio=0.973,
                    RUBISCOMETHOD=2,
                )
            elif evn_base == 'steady-state':
                args.evn_file = (
                    'TimeIterationFinalParam_steady-state_50.0_'
                    'DynaPS_ZaksNPQ.txt'
                )
                cls.prefix_path_args(args, ['evn_file'],
                                     prefix=args.output_dir)
            if not args.plot_var:
                if args.plot_ncol is None:
                    args.plot_ncol = 5
                args.plot_var = [
                    'ALL::VARS::TestLi',
                    'BF::COND::PHl',
                    # 'BF::VEL::VsATP',
                    'PS::COND::ATP',
                    'dissipation',
                    'FI::RC::kA_d',
                    # 'FI::RC::kU_d',
                    'BF::RC::Kd',
                    'XanCycle::COND::Vx',
                    'XanCycle::COND::Ax',
                    'XanCycle::COND::Zx',
                    'XanCycle::COND::PsbSQ',
                    'XanCycle::MOD::XanCycle2FIBF_Kd_NPQ',
                    'FI::VEL::v_r1',
                    'FI::VEL::vP680qU',
                    'expr_psbs',
                    'QH',
                    'XanCycle::VEL::vpsbs_act',
                    'XanCycle::VEL::vpsbs_deact',
                    'Vc',
                    'Vo',
                    'CO2AR',
                ]
            args.plot_aliases.update(**{
                'CO2AR': 'Assim',
                'BF::COND::ATP': 'ATPStroma',
                'dissipation': 'ExcitonsDissipatedBF',
            })
        elif args.light_profile.startswith('Zhu2012'):
            args.make_equivalent_to_matlab = True
            args.evn_file = None
            if args.match_param == 'matlab':
                args.evn_file = 'InputEvn_MATLAB_master.txt'
                cls.prefix_path_args(args, ['evn_file'],
                                     prefix=_data_dir)
            elif args.match_param == 'Zaks_init':
                args.evn_file = 'InputEnv_Zaks.txt'
                cls.prefix_path_args(args, ['evn_file'],
                                     # _data_dir)
                                     prefix=args.output_dir)
            elif args.match_param:
                if args.match_param == 'regen':
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
            args.param.update(
                O2_cond=0.210,  # umol mol-1, 210 mmol mol-1
                CO2_cond=280,   # umol mol-1
                Tp=25,          # C
                GP=1,
                RUBISCOMETHOD=2,
                # ProteinTotalRatio=0.973,
                # GRNC=1.0,
            )
            if args.light_profile in ['Zhu2012', 'Zhu2012_exp1']:
                args.plot_limits = {
                    'CO2AR': (0, 20),
                    'dissipation': (0, 700),
                    ROEvar: (0, 20),
                    'fluoresence': (0, 24),
                    'MembranePotential': (-0.02, 0.00),
                    PSIIvar: (0, 0.6),
                    'BF::COND::PHs': (7, 7.6),
                    'BF::COND::PHl': (6.5, 7),
                    'BF::COND::Ks': (9, 15),
                    'BF::COND::Mgs': (5, 7),
                    'BF::COND::Cls': (0, 1.2),
                }
                args.plot_var = [
                    'CO2AR', 'dissipation',
                    ROEvar,  'fluoresence',
                    'MembranePotential', PSIIvar,
                    'BF::COND::PHs', 'BF::COND::PHl',
                    'BF::COND::Ks', 'BF::COND::Mgs',
                    'BF::COND::Cls', 'ALL::VARS::TestLi',
                ]
            elif args.light_profile == 'Zhu2012_exp3':
                args.plot_limits = {
                    'CO2AR': (0, 32),
                    'dissipation': (0, 500),
                    ROEvar: (10, 20),
                    'fluoresence': (0, 50),
                    'MembranePotential': (-0.03, 0.03),
                    PSIIvar: (0, 0.6),
                    'BF::COND::PHs': (7, 8.25),
                    'BF::COND::PHl': (6.5, 7),
                    'ALL::VARS::TestLi': (0, 1000),
                }
                args.plot_var = [
                    'CO2AR', 'dissipation',
                    ROEvar,  'fluoresence',
                    'MembranePotential', PSIIvar,
                    'BF::COND::PHs', 'BF::COND::PHl',
                    'ALL::VARS::TestLi',
                ]
        elif args.light_profile.startswith('Zaks2012'):
            args.use_zaks_npq = True
            args.evn_file = None
            args.param.update(
                O2_cond=0.210,  # mmol mol-1
                CO2_cond=280,   # umol mol-1
                Tp=25,          # C
                ProteinTotalRatio=0.973,
                GRNC=1.0,
                RUBISCOMETHOD=2,
                PsbSQ=1,
                ATPc=2e-10,     # molprotons∕V∕cm ∕s
            )
            if args.light_profile in ['Zaks2012', 'Zaks2012_FigS3']:
                args.plot_var = [
                    'BF::COND::PHl',
                    'BF::COND::ATP',  # 'ATPStroma',
                    'dissipation',  # 'ExcitonsDissipatedBF',
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
                args.plot_limits = {
                    'BF::COND::PHl': (1, 8),
                }
        if args.use_zaks_npq:
            args.param.update(
                UseZaksNPQ=1,
                psbsQ_converRate=4.0e-2,
                Fpsbs=1.0,
                O2_cond=0.210,  # mmol mol-1
                CO2_cond=280,   # umol mol-1
                Tp=25,          # C
                ProteinTotalRatio=0.973,
                GRNC=1.0,
                RUBISCOMETHOD=2,
                # ATPc=2e-10,     # molprotons∕V∕cm ∕s
            )
        if not args.plot_var:
            if args.plot_ncol is None:
                args.plot_ncol = 2
            args.plot_var = [
                'CO2AR', 'dissipation',
                ROEvar,  'fluoresence',
                'MembranePotential', PSIIvar,
                'BF::VEL::Vbf8',
                'BF::VEL::Vbf3',
                'BF::VEL::VgPQH2',
                'ALL::VARS::ADP',
                'PS::COND::ATP',
                'ALL::VARS::PS2BF_Pi',
                'BF::VEL::Vbf11',
                'BF::COND::PHs', 'BF::COND::PHl',
                'BF::COND::Ks', 'BF::COND::Mgs',
                'BF::COND::Cls', 'ALL::VARS::TestLi',
            ]
        if args.plot_ncol is None:
            args.plot_ncol = 2
        if args.result_file is True:
            args.result_file = 'TimeIterationResult.txt'
        if args.first_param_file is True:
            args.first_param_file = 'TimeIterationFirstParam.txt'
        if args.final_param_file is True:
            args.final_param_file = 'TimeIterationFinalParam.txt'
        cls.prefix_path_args(args, ['plot_file', 'result_file',
                                    'first_param_file',
                                    'final_param_file'],
                             prefix=args.output_dir)
        cls.suffix_path_args(args, ['plot_file', 'result_file',
                                    'first_param_file',
                                    'final_param_file',
                                    'iterations_file'],
                             args.output_suffix)
        super(ephoto_iterations, cls).adjust_args(args)

    def run_commands(self, args, cmds=None, ephoto_args=None, **kwargs):
        self.create_light_profile(args)
        return super(ephoto_iterations, self).run_commands(
            args, cmds=cmds, ephoto_args=ephoto_args, **kwargs
        )

    def create_light_profile(self, args):
        if not args.light_profile:
            return None
        variables = {'time': [], 'PFD': []}
        light_profile = args.light_profile
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
        elif args.light_profile == 'NPQ-explore':
            # light_profile = 'dark-light-dark'
            args.low_light_level = 50
            args.high_light_level = 1800
            variables['time'] = [
                0,
                200,  # 40,
                400,  # 180,
                600,  # 250,
            ]
            variables['PFD'] = [
                args.low_light_level,
                args.high_light_level,
                args.low_light_level,
                args.low_light_level,
            ]
        elif args.light_profile in ['Zaks2012', 'Zaks2012_FigS3']:
            light_profile = 'dark-light-dark'
            args.low_light_level = 100
            args.high_light_level = 1000
            args.tstart_light_change = 2000.0 / 3
            args.duration_light_change = 2000.0 / 3
        elif args.light_profile == 'light-steady-state':
            variables['PFD'] = [args.high_light_level]
            variables['PFD'].append(variables['PFD'][-1])
            variables['time'] = [
                200 * x for x in range(len(variables['PFD']))
            ]
        elif args.light_profile == 'dark-steady-state':
            variables['PFD'] = [args.low_light_level]
            variables['PFD'].append(variables['PFD'][-1])
            variables['time'] = [
                200 * x for x in range(len(variables['PFD']))
            ]
        else:
            raise ValueError(f"Unsupported light profile: "
                             f"{args.light_profile}")
        if light_profile == 'dark-light-dark':
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
        args.param['PFD'] = variables['PFD'][0]
        args.stoptime = max(variables['time'])
        self._generated_files += [
            args.iterations_file
        ]
        self.write_iterations(args.iterations_file, variables)

    def write_iterations(self, fname, data):
        import pandas as pd
        df = pd.DataFrame(data)
        df.to_csv(fname, sep='\t', index=False)

    def record_last_param(self, args):
        if not (args.first_param_file or args.final_param_file):
            return
        param_files = sorted(glob.glob(
            args.output_param_base + 'step*.txt'
        ))
        print("PARAM_FILES")
        pprint.pprint(param_files)
        if not param_files:
            warnings.warn(f"No parameter files available matching \""
                          f"{args.output_param_base}step*.txt\"")
            return
        first_file = None
        final_file = None
        times = []
        for x in param_files:
            t = float(x.rsplit('_step', 1)[-1].split('.txt')[0])
            times.append(t)
            if first_file is None or t < first_file[0]:
                first_file = (t, x)
            if final_file is None or t > final_file[0]:
                final_file = (t, x)
        times = sorted(times)
        print("TIMES")
        pprint.pprint(times)
        if args.first_param_file and first_file:
            shutil.copy2(first_file[1], args.first_param_file)
        if args.final_param_file and final_file:
            shutil.copy2(final_file[1], args.final_param_file)

    def read_param(self, args):
        param_files = sorted(glob.glob(
            args.output_param_base + 'step*.txt'
        ))
        assert param_files
        variables = OrderedDict([('time', [])])
        for v in args.plot_var:
            variables[v] = []
        for x in param_files:
            t = float(x.rsplit('_step', 1)[-1].split('.txt')[0])
            if t < args.tmin:
                continue
            variables['time'].append(t)
            if len(variables['time']) > 1:
                assert variables['time'][-1] > variables['time'][-2]
            data = read_param(x)
            for v in args.plot_var:
                if v in data:
                    variables[v].append(data[v])
                else:
                    warnings.warn(f'Missing field \"{v}\" for t = {t}')
                    variables[v].append(np.nan)
        if args.result_file:
            write_param_table(args.result_file, variables)
        return variables

    def plot(self, data):
        assert 'time' in data
        variables = list(data.keys())
        variables.remove('time')
        nplots = len(variables)
        if args.light_profile == 'Zhu2012_Fig3':
            nplots += 1
        ncol = 1 if nplots == 1 else args.plot_ncol
        nrow = int(np.ceil(nplots / ncol))
        fig, axs = plt.subplots(nrow, ncol,
                                figsize=(2.5 * ncol, 1.5 * nrow),
                                layout='constrained')
        for ax, v in zip(axs.flat, variables):
            ax.set_xlabel('time (s)')
            vname = args.plot_aliases.get(v, v.split('::')[-1])
            if v in args.plot_units:
                vname += f' ({args.plot_units[v]})'
            ax.set_ylabel(vname)
            ax.plot(data['time'], data[v])
            ax.set_xlim(min(data['time']), max(data['time']))
            if args.match_limits and v in args.plot_limits:
                ax.set_ylim(*args.plot_limits[v])
        if args.light_profile == 'Zhu2012_Fig3':
            ax = axs.flat[-1]
            ax.set_xlabel(f'PFD ({args.plot_units["ALL::VARS::TestLi"]})')
            ax.set_ylabel(f'A ({args.plot_units["CO2AR"]})')
            ax.plot(data['ALL::VARS::TestLi'], data['CO2AR'])
        if args.plot_file:
            print(f'Saving plot to \"{args.plot_file}\"')
            fig.savefig(args.plot_file)
        else:
            plt.show()


class compare_matlab(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        args.make_equivalent_to_matlab = True
        if not args.matlab:
            args.matlab = find_matlab(required=True)
        ephoto.adjust_args(args)
        cls.prefix_path_args(
            args, ['matlab_repo'], prefix=_source_dir)
        cls.prefix_path_args(
            args, ['matlab_output_dir', 'generate_matlab_script'])
        cls.prefix_path_args(
            args, ['matlab_output_file', 'matlab_output_param_base'],
            prefix=args.matlab_output_dir)
        if args.generate_matlab_script and args.dont_run_matlab:
            args.no_diff = True
        if args.diff or args.dont_run:
            args.dont_run_matlab = True
            args.dont_run_cpp = True
        if args.no_diff:
            args.dont_cleanup = True
        if args.dont_run_cpp:
            args.dont_build = True
        assert args.output_file != args.matlab_output_file
        assert args.output_param_base != args.matlab_output_param_base
        assert not args.iterations_file
        if not os.path.isdir(args.matlab_output_dir):
            os.mkdir(args.matlab_output_dir)

    def __init__(self, args, config_args=None, build_args=None):
        self.adjust_args(args)
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        cmds = []
        super(compare_matlab, self).__init__(
            args, cmds=cmds, config_args=config_args,
            build_args=build_args, cwd=_source_dir,
        )
        args.rebuild = False
        args.dont_build = True
        self.compare(args)

    def compare(self, args):
        if args.driver == 0:
            ephoto.iter_drivers(
                self.compare, args,
                suffix_paths=['matlab_output_file',
                              'matlab_output_param_base'])
            return
        if args.generate_matlab_script:
            self.generate_matlab_script(args)
        if args.dont_run_matlab:
            out_matlab = False
        else:
            out_matlab = self.run_matlab(args)
        if args.dont_run_cpp:
            out_cpp = False
        else:
            out_cpp = self.run_cpp(args)
        if not args.no_diff:
            self.diff(args, out_matlab, out_cpp)

    @classmethod
    def generate_matlab_script(cls, args):
        lines = [
            f'addpath("{args.matlab_repo}");',
            f'driver = {args.driver};',
            f'evn_file = "{args.evn_file}";',
            f'grn_file = "{args.grn_file}";',
            f'enzyme_file = "{args.enzyme_file}";',
            f'atpcost_file = "{args.atpcost_file}";',
            f'output_file = "{args.matlab_output_file}";',
            f'output_param_base = "{args.matlab_output_param_base}";',
            'Arate = ePhotosynthesis(driver, evn_file, grn_file,'
            ' enzyme_file, atpcost_file, output_file, output_param_base)'
        ]
        contents = '\n'.join(lines)
        print(f'{80*"="}\n'
              f'Writing script to {args.generate_matlab_script}:\n'
              f'{80*"-"}\n{contents}\n{80*"="}')
        with open(args.generate_matlab_script, 'w') as fd:
            fd.write(contents)

    def run_matlab(self, args, **kwargs):
        cmd = [
            args.matlab, '-nodisplay', '-nosplash', '-nodesktop',
            '-nojvm',
        ]
        if args.generate_matlab_script:
            script_dir, script_name = os.path.split(
                args.generate_matlab_script)
            cmd += [
                '-batch', os.path.splitext(script_name)[0],
            ]
            kwargs.setdefault('cwd', script_dir)
            self._generated_files += [args.generate_matlab_script]
        else:
            cmd += [
                '-r', f'ePhotosynthesis {args.driver} {args.evn_file} '
                f'{args.grn_file} {args.enzyme_file} {args.atpcost_file} '
                f'{args.matlab_output_file} {args.matlab_output_param_base}'
                f', exit'
            ]
            kwargs.setdefault('cwd', args.matlab_repo)
        cmds = [' '.join(cmd)]
        cmdS = '\n\t'.join(cmds)
        print(f"Running\n{cmdS}\n"
              f"with {pprint.pformat(kwargs)}")
        out = subprocess.run(cmd, **kwargs)
        return out

    def run_cpp(self, args, ephoto_args=None):
        if ephoto_args is None:
            ephoto_args = []
        out = ephoto(
            args, config_args=['-DMAKE_EQUIVALENT_TO_MATLAB:BOOL=ON'],
            ephoto_args=ephoto_args, dont_cleanup=True)
        return out

    def diff(self, args, out1, out2):
        self._generated_files += [
            args.output_file,
            args.output_param_base + 'init.txt',
            args.output_param_base + 'last.txt',
            args.output_dir,
            args.matlab_output_file,
            args.matlab_output_param_base + "init.txt",
            args.matlab_output_param_base + "last.txt",
            args.matlab_output_dir,
        ]
        finit1 = args.matlab_output_param_base + 'init.txt'
        finit2 = args.output_param_base + 'init.txt'
        compare_files(finit1, finit2)
        fout1 = args.matlab_output_file
        fout2 = args.output_file
        check_output_kws = {
            'reltol': args.reltol,
            'abstol': args.abstol,
            'label_f1': 'MATLAB',
            'label_f2': 'C++',
            'sep': args.sep,
        }
        compare_files(fout1, fout2, check_files=check_output,
                      check_files_kwargs=check_output_kws,
                      ftype='output')


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
        config_args += ['-DBUILD_DOCS=ON', '-DDOXYGEN_CHECK_MISSING=ON']
        super(docs, self).__init__(
            args, config_args=config_args,
            build_args=build_args,
            install_args=install_args,
        )


class coverage(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        args.with_coverage = True
        return super(coverage, cls).adjust_args(args)

    def __init__(self, args, config_args=None, build_args=None):
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        config_args += ['-DBUILD_TESTS:BOOL=ON']
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
        compare_files(args.actual, args.expected, ftype=args.filetype,
                      check_files_kwargs=check_files_kwargs)


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
        'ephoto-iterations', help="Run an ephoto time series",
        func=ephoto_iterations)
    parser_iterations.add_argument(
        '--plot-ncol', type=int,
        help="Number of columns in figure.",
    )
    parser_iterations.add_argument(
        '--plot-var', '--var', type=str, action='append',
        help="Variable that should be plot against time",
    )
    parser_iterations.add_argument(
        '--match-limits', action='store_true', default=False,
        help="Match limits to those for published plots")
    parser_iterations.add_argument(
        '--result-file', type=str,
        nargs='?', const='TimeIterationResult.txt',
        help=(
            "Location where the results for the output time steps "
            "should be saved"
        ),
    )
    parser_iterations.add_argument(
        '--first-param_file', type=str,
        nargs='?', const='TimeIterationFirstParam.txt',
        help="Location where the first parameter file should be saved.")
    parser_iterations.add_argument(
        '--final-param_file', type=str,
        nargs='?', const='TimeIterationFinalParam.txt',
        help="Location where the final parameter file should be saved.")
    parser_iterations.add_argument(
        '--plot-file', type=str,
        nargs='?', const='TimeIterationResult.png',
        help="Location where the generated plot should be saved")
    parser_iterations.add_argument(
        '--tmin', type=float, default=0,
        help="Minimum time that should be plot")
    parser_iterations.add_argument(
        '--light-profile', choices=[
            'dilkaran', 'Zhu2012',
            'Zhu2012_Fig2', 'Zhu2012_Fig3',
            'Zaks2012', 'Zaks2012_FigS3',
            'light-dark-light', 'dark-light-dark',
            'light-steady-state', 'dark-steady-state',
            'NPQ-explore',
        ],
        default='Zhu2012',
        help=("Create an input file that produces a desired light "
              "profile"))
    parser_iterations.add_argument(
        '--use-zaks-npq', action='store_true',
        help=(
            "Use the Zaks et al. 2012 model of non-photochemical "
            "quenching"
        ),
    )
    parser_iterations.add_argument(
        '--low-light-level', type=float,
        help="Light level for the low light condition.",
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
    parser_iterations.add_argument(
        '--match-param', nargs='?', const=True,
        choices=[True, 'regen', 'matlab', 'Zaks_init'],
        help="Run with the parameters explicitly set to match the paper")
    # parser_yggdrasil = subparsers.add_parser(
    #     'yggdrasil',
    #     help="Return information about the yggdrasil interface library",
    #     func=ygginfo)

    # MATLAB/C++ Comparison
    parser_matlab = subparsers.add_parser(
        'compare-matlab', help="Compare C++ & MATLAB versions",
        func=compare_matlab)
    parser_matlab.add_argument(
        '--matlab', type=str, default=find_matlab(),
        help="Path to the MATLAB executable")
    parser_matlab.add_argument(
        '--matlab-repo', type=str,
        default=os.path.join(
            os.path.dirname(_source_dir), 'ePhotosynthesis'),
        help="Path to the MATLAB version of the model")
    parser_matlab.add_argument(
        '--matlab-output-dir', type=str, default='output_MTL',
        help="Directory where MATLAB output should be saved")
    parser_matlab.add_argument(
        '--matlab-output-file', '--matlab-output',
        type=str, default='output.data',
        help="File where MATLAB driver output should be saved")
    parser_matlab.add_argument(
        "--matlab-output-param-base", type=str, default='output_param_',
        help="File prefix for MATLAB output parameter files")
    parser_matlab.add_argument(
        "--generate-matlab-script", type=str,
        help=("Path where MATLAB script should be generated to run "
              "the MATLAB version of the model"))
    parser_matlab.add_argument(
        "--dont-run-matlab", action='store_true',
        help=("Don't run MATLAB, but assume it has "
              "already been run with the required "
              "parameters "))
    parser_matlab.add_argument(
        "--dont-run-cpp", action='store_true',
        help=("Don't run C++, but assume it has "
              "already been run with the required "
              "parameters "))
    parser_matlab.add_argument(
        "--no-diff", action='store_true',
        help="Don't perform the diff on the output from the two models")
    parser_matlab.add_argument(
        "--diff", action='store_true',
        help=("Just perform the diff on the output from the two models "
              "assuming that it was already generated"))

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

    requires_build = [
        'update-readme',
        'test',
        'ephoto',
        'ephoto-iterations',
        'compare-matlab',
        'coverage',
    ]
    build_tasks = ['build'] + requires_build
    ephoto_tasks = ['ephoto', 'ephoto-iterations', 'compare-matlab']
    compare_tasks = ['compare-matlab', 'compare-files']

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
        subparsers={'task': [x for x in build_tasks + ['docs']
                             if x != 'compare-matlab']})
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
        "--reltol", type=float, default=1.0e-05,
        help="Relative tolerance to use when comparing output files",
        subparsers={'task': compare_tasks})
    parser.add_argument(
        "--abstol", type=float, default=1.0e-08,
        help="Absolute tolerance to use when comparing output files",
        subparsers={'task': compare_tasks})
    parser.add_argument(
        "--sep", type=str, default=',',
        help="Separator for values in tables to compare",
        subparsers={'task': compare_tasks})

    # ePhoto arguments
    parser.add_argument(
        "--driver", '-d', type=int,
        default=0, choices=[0, 1, 2, 3, 4],
        help="Driver to run",
        subparsers={'task': ephoto_tasks + ['compare-files']},
        subparser_defaults={
            'ephoto-iterations': 2
        })
    parser.add_argument(
        "--useC3", "--c3", action="store_true",
        help="Run the C3 version",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={
            'ephoto-iterations': False,
        })
    parser.add_argument(
        '--input-dir', type=str, default=_data_dir,
        help="Directory containing input files",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--enzyme-file', '--enzyme', type=str, default='InputEnzyme.txt',
        help="File containing enzyme concentrations.",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--grn-file', '--grn', type=str, default='InputGRNC.txt',
        help="File containing transcription factor levels.",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'InputGRNC_MATLAB.txt'})
    parser.add_argument(
        '--evn-file', '--evn', type=str, default='InputEvn.txt',
        help="File containing environmental parameters.",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'InputEvn_MATLAB.txt'})
    parser.add_argument(
        '--iterations-file', '--iterations', type=str,
        subparsers={'task': ephoto_tasks},
        subparser_defaults={
            'ephoto-iterations': 'InputTimeIteration.txt'
        })
    parser.add_argument(
        '--atpcost-file', '--atp', type=str, default='InputATPCost.txt',
        help="File containing ATP cost",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-dir', type=str, default=os.getcwd(),
        help="Directory where output should be saved",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'output_CPP',
                            'ephoto-iterations': 'output_iterations'})
    parser.add_argument(
        '--output-file', '--output', type=str, default='output.data',
        help="File where driver output should be saved",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-suffix', type=str, const=True, nargs='?',
        help="Suffix to add to output files.",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        "--output-param", choices=[0, 1, 2, 3], type=int,
        help="Flag specifying when to output parameters",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 2,
                            'ephoto-iterations': 3})
    parser.add_argument(
        "--output-param-base", type=str, default="param_",
        help="File prefix for output parameter files",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'output_param_'})
    parser.add_argument(
        '--dont-run', action='store_true',
        help="Don\'t run ephotosynthesis, only post-process",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--stoptime', type=int,
        help="Stop time for the run",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 3000})
    parser.add_argument(
        '--param', type=cli_param, action='extend',
        help="Parameter values that should be set")

    # Universal arguments
    parser.add_argument(
        "--dont-cleanup", action='store_true',
        help="Don't clean up any files generated by the task",
        subparsers={'task': 'all'})

    args = parser.parse_args()
    parser.run_subparser('task', args)
