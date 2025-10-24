import os
import copy
import shutil
import sys
import argparse
import pprint
import subprocess
import glob
import difflib
import site
from collections import OrderedDict
import matplotlib.pyplot as plt
import numpy as np


_source_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))
_utils_dir = os.path.join(_source_dir, 'utils')
_data_dir = os.path.join(_source_dir, 'tests', 'data')

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


def read_param(fname):
    out = OrderedDict()
    with open(fname, 'r') as fd:
        contents = fd.readlines()
    for x in contents:
        name, value = x.split('#')[0].split()
        out[name] = float(value)
    return out


def write_param(fname, param):
    maxlen = len(max(param.keys(), key=len)) + 4
    with open(fname, 'w') as fd:
        for k, v in param.items():
            pad = ' ' * (maxlen - len(k))
            fd.write(f'{k}{pad}{v}\n')


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
        kwargs['env'][_library_path_var] = add_directory_to_path(
            args.build_dir, return_path=True, prepend=True,
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
        # import warnings
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
            if args.param:
                if args.evn_file:
                    param = read_param(args.evn_file)
                    args.param = dict(param, **args.param)
                args.evn_file = os.path.join(
                    args.input_dir, 'GeneratedEvn.txt'
                )
                write_param(args.evn_file, args.param)
                self._generated_files.append(args.evn_file)
            cmds = [
                f'{execFile} -d {args.driver} '
                f'--enzyme {args.enzyme_file} --grn {args.grn_file} '
                f'--evn {args.evn_file} --atpcost {args.atpcost_file} '
                f'--output {args.output_file} '
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
            cls.suffix_path_args(args, suffix_paths,
                                 '_' + cls._driver_map[args.driver])
            func(args, **kwargs)


class ephoto_iterations(ephoto):

    def __init__(self, args, **kwargs):
        super(ephoto_iterations, self).__init__(args, **kwargs)
        self.plot(self.read_param(args))

    @classmethod
    def adjust_args(cls, args):
        if not args.param:
            args.param = {}
        if args.light_profile:
            # args.evn_file = None
            args.iterations_file = (
                f'InputTimeIteration_{args.light_profile}.txt'
            )
            if args.plot_file:
                args.plot_file = (
                    f'TimeIterationResult_{args.light_profile}.png'
                )
        if args.light_profile in ['paper', 'paper_exp1', 'paper_exp3']:
            args.driver = 2
            args.plot_var = [
                'CO2AR', 'FI::VEL::vA_d',
                'ALL::VARS::O2_cond',  # fluoresence
                # membrange potential
                'PSIIabs',
                'BF::COND::PHs', 'BF::COND::PHl',
                'BF::COND::Ks', 'BF::COND::Mgs',
                'BF::COND::Cls',
                'ALL::VARS::TestLi',
            ]
            args.param['O2'] = 210  # mmol mol-1
            args.param['CO2_in'] = 280  # umol mol-1 (or CO2_cond?)
            args.param['CO2_in'] *= 3. * pow(10., 4.)  # Convert to ppm
            args.param['Tp'] = 25  # C
        cls.prefix_path_args(args, ['plot_file'], prefix=args.output_dir)
        super(ephoto_iterations, cls).adjust_args(args)

    def run_commands(self, args, cmds=None, ephoto_args=None, **kwargs):
        self.create_light_profile(args)
        return super(ephoto_iterations, self).run_commands(
            args, cmds=cmds, ephoto_args=ephoto_args, **kwargs
        )

    def create_light_profile(self, args):
        if not args.light_profile:
            return None
        variables = {'time': [], 'PAR': []}
        if args.light_profile == 'dilkaran':
            variables['time'] = [
                0,
                400,
                800,
                1200,
            ]
            variables['PAR'] = [
                0,
                600,
                0,
                0,
            ]
        elif args.light_profile in ['paper', 'paper_exp1']:
            variables['time'] = [
                0,
                200,
                400,
                600,
            ]
            variables['PAR'] = [
                1000,
                100,
                1000,
                1000,
            ]
        elif args.light_profile == 'paper_exp3':
            # 100 µmol m−2 s−1 increments
            variables['PAR'] = np.linspace(0, 1000, 11)
            variables['PAR'].append(variables['PAR'][-1])
            variables['time'] = [
                200 * x for x in range(len(variables['PAR']))
            ]
        elif args.light_profile == 'zaks':
            variables['time'] = [
                0,
                150,
                900,
                1500,
            ]
            variables['PAR'] = [
                0,
                1000,
                0,
                0,
            ]
        else:
            raise ValueError(f"Unsupported light profile: "
                             f"{args.light_profile}")
        args.param['PAR'] = variables['PAR'][0]
        args.stoptime = max(variables['time'])
        self._generated_files += [
            args.iterations_file
        ]
        self.write_iterations(args.iterations_file, variables)

    def write_iterations(self, fname, data):
        import pandas as pd
        df = pd.DataFrame(data)
        df.to_csv(fname, sep='\t', index=False)

    def read_param(self, args):
        param_files = sorted(glob.glob(
            args.output_param_base + 'step*.txt'
        ))
        assert param_files
        variables = {'time': []}
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
                variables[v].append(data[v])
        return variables

    def plot(self, data):
        assert 'time' in data
        variables = list(data.keys())
        variables.remove('time')
        nplots = len(variables)
        if args.light_profile.startswith('paper'):
            nplots += 1
        ncol = 1 if nplots == 1 else 2
        nrow = int(np.ceil(nplots / ncol))
        fig, axs = plt.subplots(nrow, ncol, figsize=(10, 3 * nrow),
                                layout='constrained')
        for ax, v in zip(axs.flat, variables):
            ax.set_xlabel('time')
            ax.set_ylabel(v.split('::')[-1])
            ax.plot(data['time'], data[v])
        if args.light_profile.startswith('paper'):
            ax = axs.flat[-1]
            ax.set_xlabel('ALL::VARS::TestLi')
            ax.set_ylabel('CO2AR')
            ax.plot(data['ALL::VARS::TestLi'], data['CO2AR'])
        if args.plot_file:
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

    parser_ephoto = subparsers.add_parser(
        'ephoto', help="Run ephoto executable",
        func=ephoto)

    parser_iterations = subparsers.add_parser(
        'ephoto-iterations', help="Run an ephoto time series",
        func=ephoto_iterations)
    parser_iterations.add_argument(
        '--plot-var', '--var', type=str, action='append',
        help="Variable that should be plot against time",
        default=[
            'ALL::VARS::TestLi', 'CO2AR',
            'ALL::VARS::O2_cond', 'ALL::VARS::CO2_cond',
            'FI::VEL::vU_d', 'FI::VEL::vA_d',
            'BF::COND::PHs', 'BF::COND::PHl',
        ])
    parser_iterations.add_argument(
        '--plot-file', type=str,
        nargs='?', const='TimeIterationResult.png',
        help="Location where the generated plot should be saved")
    parser_iterations.add_argument(
        '--tmin', type=float, default=1,
        help="Minimum time that should be plot")
    parser_iterations.add_argument(
        '--light-profile', choices=['dilkaran', 'paper'],
        default='paper',
        help=("Create an input file that produces a desired light "
              "profile"))
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
            'ephoto-iterations': 4
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
