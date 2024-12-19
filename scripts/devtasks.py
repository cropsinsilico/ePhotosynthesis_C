import os
import shutil
import sys
import argparse
import pprint
import subprocess
import glob
import difflib
import numpy as np


_source_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


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

    def __init__(self, args, dont_cleanup=False, **kwargs):
        self.current_args = args
        self.dont_cleanup = dont_cleanup
        self._generated_files = []
        self.adjust_args(args)
        self.run_commands(args, **kwargs)

    def __del__(self):
        self.cleanup_files()

    @classmethod
    def adjust_args(cls, args):
        pass

    def cleanup_files(self):
        if self.dont_cleanup or self.current_args.dont_cleanup:
            return
        for x in self._generated_files:
            if os.path.isfile(x):
                os.remove(x)
            elif os.path.isdir(x) and not glob.glob(os.path.join(x, '*')):
                os.rmdir(x)

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

    def run_commands(self, args, cmds=None, output_file=None, **kwargs):
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
            if ires.returncode != 0:
                raise RuntimeError(f'Error in running \'{x}\'')
            if output_file:
                output_str += ires.stdout
        if output_file:
            with open(output_file, 'wb') as fd:
                fd.write(output_str)


class BuildSubTask(SubTask):

    def __init__(self, args, config_args=None, build_args=None,
                 install_args=None, build_kwargs=None, build_env=None,
                 **kwargs):
        self.adjust_args(args)
        if build_kwargs is None:
            build_kwargs = {}
        build_kwargs.setdefault('env', build_env)
        if not args.dont_build:
            build(args, config_args=config_args, build_args=build_args,
                  install_args=install_args, **build_kwargs)
        kwargs.setdefault('cwd', args.build_dir)
        super(BuildSubTask, self).__init__(args, **kwargs)

    @classmethod
    def adjust_args(cls, args):
        cls.prefix_path_args(args, ['build_dir', 'install_dir'])


class build(SubTask):

    def __init__(self, args, config_args=None, build_args=None,
                 install_args=None, **kwargs):
        BuildSubTask.adjust_args(args)
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        if install_args is None:
            install_args = []
        config_args += ['-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON']
        if args.make_equivalent_to_matlab:
            config_args += ['-DMAKE_EQUIVALENT_TO_MATLAB:BOOL=ON']
        if args.with_asan:
            config_args += ['-DWITH_ASAN=ON']
        if args.only_python:
            build_args += ['--target pyPhotosynthesis']
            install_args += ['--component', 'Python']
            args.with_python = True
        if args.with_python:
            config_args += ['-DBUILD_PYTHON:BOOL=ON']
        if not args.dont_install:
            config_args += [f'-DCMAKE_INSTALL_PREFIX={args.install_dir}']
        if sys.platform != 'win32':
            build_args += ['--', '-j', str(args.njobs)]
        cmds = [
            f"cmake {_source_dir} {' '.join(config_args)}",
            f"cmake --build . --config {args.build_type}"
            f" {' '.join(build_args)}",
        ]
        if args.with_python and not args.only_python:
            cmds += [
                f"cmake --build . --config {args.build_type} "
                f"--target pyPhotosynthesis"
            ]
        if not args.dont_install:
            if not os.path.isdir(args.install_dir):
                os.mkdir(args.install_dir)
            cmds += [
                f"cmake --install . --prefix {args.install_dir} "
                f"{' '.join(install_args)}"
            ]
            if args.with_python and not getattr(args, 'only_python', False):
                cmds += [
                    f"cmake --install . --prefix {args.install_dir} "
                    f"--component Python {' '.join(install_args)}"
                ]
        if ((args.rebuild and (not args.dont_install) and
             os.path.isdir(args.install_dir))):
            subdir = glob.glob(os.path.join(args.install_dir, '*'))
            if (not subdir) or subdir == [os.path.join(args.install_dir,
                                                       'ePhotosynthesis')]:
                shutil.rmtree(args.install_dir)
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

    def __init__(self, args, test_flags=None,
                 config_args=None, build_args=None, **kwargs):
        self.adjust_args(args)
        if test_flags is None:
            test_flags = []
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
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
        if args.only_python:
            args.with_python = True
        if args.with_python and args.dont_install:
            kwargs.setdefault('env', {})
            kwargs['env'].setdefault(
                'PYTHONPATH', os.environ.get('PYTHONPATH', ''))
            kwargs['env']['PYTHONPATH'] = os.pathsep.join([
                x for x in
                [args.build_dir,
                 kwargs['env']['PYTHONPATH']]
                if x
            ])
        if args.only_python:
            testdir = os.path.join(_source_dir, 'tests', 'python')
            cmds = [f"python -m pytest {' '.join(pytest_flags)} {testdir}"]
        else:
            if args.show_tests:
                cmds = [
                    'ctest -N'
                ]
            else:
                cmds = [
                    f"ctest {' '.join(test_flags)}"
                ]
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

    @classmethod
    def adjust_args(cls, args):
        if args.refresh_output:
            args.preserve_output = True
        super(test, cls).adjust_args(args)


class ephoto(BuildSubTask):

    @classmethod
    def adjust_args(cls, args):
        cls.prefix_path_args(args, ['input_dir', 'output_dir'])
        cls.prefix_path_args(args, ['enzyme_file', 'grn_file',
                                    'evn_file', 'atpcost_file'],
                             prefix=args.input_dir)
        cls.prefix_path_args(args, ['output_file', 'output_param_base'],
                             prefix=args.output_dir)
        if not os.path.isdir(args.output_dir):
            os.mkdir(args.output_dir)
        super(ephoto, cls).adjust_args(args)

    def run_commands(self, args, cmds=None, ephoto_args=None, **kwargs):
        if args.driver == 0 and cmds is None:
            self.iter_drivers(args, self.run_commands, **kwargs)
            return
        if ephoto_args is None:
            ephoto_args = []
        if cmds is None:
            execFile = os.path.join(args.build_dir, 'ePhoto')
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


class compare_matlab(BuildSubTask):

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
        if args.diff:
            args.dont_run_matlab = True
            args.dont_run_cpp = True
        if args.no_diff:
            args.dont_cleanup = True
        if args.dont_run_cpp:
            args.dont_build = True
        assert args.output_file != args.matlab_output_file
        assert args.output_param_base != args.matlab_output_param_base
        if not os.path.isdir(args.matlab_output_dir):
            os.mkdir(args.matlab_output_dir)

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
        ephoto_args += [
            '--outputParam', '2', '--stoptime', '3000',
        ]
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
        self.compare_files(finit1, finit2)
        fout1 = args.matlab_output_file
        fout2 = args.output_file
        check_output_kws = {
            'reltol': args.reltol,
            'abstol': args.abstol,
        }
        self.compare_files(fout1, fout2, check_files=self.check_output,
                           check_files_kwargs=check_output_kws,
                           ftype='output')

    @classmethod
    def compare_files(cls, f1, f2, check_files=None, ftype='parameter',
                      check_files_kwargs={}):
        with open(f1, 'r') as fd:
            lines1 = fd.readlines()
        with open(f2, 'r') as fd:
            lines2 = fd.readlines()
        line_diff = list(difflib.unified_diff(lines1, lines2,
                                              fromfile=f1, tofile=f2))
        if line_diff:
            if (((check_files is not None)
                 and check_files(f1, f2, **check_files_kwargs))):
                return
            print(line_diff)
            line_diff = '\n'.join(line_diff)
            raise RuntimeError(
                f"{ftype.title()} files differ:\n{line_diff}"
            )

    @classmethod
    def read_output_table(cls, fname):
        with open(fname, 'r') as fd:
            contents = fd.readlines()
        if len(contents) == 1:
            names = ['CO2AR']
        else:
            names = contents[0].strip().split(',')
        values = [float(x) for x in contents[-1].strip().split(',')]
        out = {k: v for k, v in zip(names, values)}
        return out

    @classmethod
    def check_output(cls, f1, f2, reltol=1.0e-05, abstol=1.0e-08):
        x1dict = cls.read_output_table(f1)
        x2dict = cls.read_output_table(f2)
        out = True
        for k in x1dict.keys():
            if k not in x2dict:
                print(f"{k} missing from C++")
        for k in x2dict.keys():
            if k not in x1dict:
                print(f"{k} missing from MATLAB")
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


class docs(BuildSubTask):

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
        build_args += ['--target', 'docs']
        install_args += ['--component', 'docs']
        super(docs, self).__init__(
            args, config_args=config_args,
            build_args=build_args,
            install_args=install_args,
        )

    @classmethod
    def adjust_args(cls, args):
        args.with_asan = False
        args.build_type = 'Debug'
        args.dont_build = False
        args.dont_install = True
        args.only_python = False
        super(docs, cls).adjust_args(args)


class coverage(BuildSubTask):

    def __init__(self, args, config_args=None, build_args=None):
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        config_args += ['-DTEST_COVERAGE=ON']
        cmds = ['make coverage']
        super(coverage, self).__init__(
            args, cmds=cmds, config_args=config_args,
            build_args=build_args
        )


class preprocess(SubTask):

    def __init__(self, args):
        cmds = [
            f'gcc -E {args.file} -I {_source_dir}/include/'
        ]
        super(preprocess, self).__init__(args, cmds=cmds,
                                         output_file=args.output_file)


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
    parser_matlab.add_argument(
        "--reltol", type=float, default=1.0e-05,
        help="Relative tolerance to use when comparing C++ & MATLAB")
    parser_matlab.add_argument(
        "--abstol", type=float, default=1.0e-08,
        help="Absolute tolerance to use when comparing C++ & MATLAB")

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
    parser_coverage = subparsers.add_parser(
        'coverage', help="Check test coverage",
        func=coverage)

    requires_build = [
        'update-readme',
        'test',
        'ephoto',
        'compare-matlab',
        'coverage',
    ]
    build_tasks = ['build'] + requires_build
    ephoto_tasks = ['ephoto', 'compare-matlab']

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
        default=os.path.join(_source_dir, '_install'),
        help="Install directory",
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
        '--only-python', action='store_true',
        help="Only run the Python tests",
        subparsers={'task': build_tasks})

    parser.add_argument(
        '--dont-build', action='store_true',
        help="Don't rebuild before performing the task",
        subparsers={'task': requires_build})

    # ePhoto arguments
    parser.add_argument(
        "--driver", '-d', type=int,
        default=0, choices=[0, 1, 2, 3, 4],
        help="Driver to run",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--input-dir', type=str, default=_source_dir,
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
        '--atpcost-file', '--atp', type=str, default='InputATPCost.txt',
        help="File containing ATP cost",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        '--output-dir', type=str, default=os.getcwd(),
        help="Directory where output should be saved",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'output_CPP'})
    parser.add_argument(
        '--output-file', '--output', type=str, default='output.data',
        help="File where driver output should be saved",
        subparsers={'task': ephoto_tasks})
    parser.add_argument(
        "--output-param-base", type=str, default="param_",
        help="File prefix for output parameter files",
        subparsers={'task': ephoto_tasks},
        subparser_defaults={'compare-matlab': 'output_param_'})

    # Universal arguments
    parser.add_argument(
        "--dont-cleanup", action='store_true',
        help="Don't clean up any files generated by the task",
        subparsers={'task': 'all'})

    args = parser.parse_args()
    parser.run_subparser('task', args)
