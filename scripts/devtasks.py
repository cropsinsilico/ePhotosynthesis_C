import os
import shutil
import sys
import argparse
import pprint
import subprocess
import glob


_source_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


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
        if subparsers:
            for k, v in subparsers.items():
                for x in v:
                    self._subparser_classes[k].choices[x].add_argument(
                        *args, **kwargs)
        else:
            super(InstrumentedParser, self).add_argument(*args, **kwargs)


class SubTask:

    def __init__(self, args, cmds=None, output_file=None, **kwargs):
        self.adjust_args(args)
        if cmds is None:
            cmds = []
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

    @classmethod
    def adjust_args(cls, args):
        pass


class BuildSubTask(SubTask):

    def __init__(self, args, config_args=None, build_args=None,
                 build_kwargs=None, build_env=None, **kwargs):
        self.adjust_args(args)
        if build_kwargs is None:
            build_kwargs = {}
        build_kwargs.setdefault('env', build_env)
        if not args.dont_build:
            build(args, config_args=config_args, build_args=build_args,
                  **build_kwargs)
        kwargs.setdefault('cwd', args.build_dir)
        super(BuildSubTask, self).__init__(args, **kwargs)

    @classmethod
    def adjust_args(cls, args):
        if not os.path.isabs(args.build_dir):
            args.build_dir = os.path.abspath(args.build_dir)
        if not os.path.isabs(args.install_dir):
            args.install_dir = os.path.abspath(args.install_dir)


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
        if args.rebuild and os.path.isdir(args.build_dir):
            shutil.rmtree(args.build_dir)
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
        try:
            super(update_readme, self).__init__(args, cmds=cmds,
                                                output_file=helpFile)
            with open(helpFile, 'r') as fd:
                helpMsg = fd.read()
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
        if args.with_python:
            assert not args.dont_install
            # kwargs.setdefault('env', {})
            # kwargs['env'].setdefault(
            #     'PYTHONPATH', os.environ.get('PYTHONPATH', ''))
            # kwargs['env']['PYTHONPATH'] = os.pathsep.join([
            #     x for x in
            #     [args.install_dir,
            #      kwargs['env']['PYTHONPATH']]
            #     if x
            # ])
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
        super(test, self).__init__(args, cmds=cmds,
                                   config_args=config_args,
                                   build_args=build_args,
                                   **kwargs)


class ephoto(BuildSubTask):

    def __init__(self, args, config_args=None, build_args=None):
        self.adjust_args(args)
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        execFile = os.path.join(args.build_dir, 'ePhoto')
        cmds = [
            f'{execFile} -d 4 '
            f'--enzyme {args.enzyme_file} --grn {args.grn_file} '
            f'--evn {args.evn_file} --output {args.output_file}',
            f'cat {args.output_file}'
        ]
        super(ephoto, self).__init__(
            args, cmds=cmds, config_args=config_args,
            build_args=build_args, cwd=_source_dir,
        )

    @classmethod
    def adjust_args(cls, args):
        for k in ['enzyme_file', 'grn_file', 'evn_file']:
            if not os.path.isabs(getattr(args, k)):
                if k == 'output_file':
                    setattr(args, k, os.path.join(
                        _source_dir, getattr(args, k)))
                else:
                    setattr(args, k, os.path.join(
                        args.input_dir, getattr(args, k)))
            setattr(args, k, os.path.expanduser(getattr(args, k)))
        super(ephoto, cls).adjust_args(args)


class compare_matlab(BuildSubTask):

    def __init__(self, args, config_args=None, build_args=None):
        self.adjust_args(args)
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        config_args += ['-DMAKE_EQUIVALENT_TO_MATLAB']
        cmds = [
            f'python utils/compare_matlab.py {args.matlab_repo}'
            f' -d 4 --matlab {args.matlab}',
            # f'diff {args.matlab_repo}/EPS_init.txt EPS_init.txt &> '
            # f'diff_EPS_init.txt',
            # f'diff {args.matlab_repo}/EPS_final.txt EPS_final.txt &> '
            # f'diff_EPS_final.txt',
        ]
        super(compare_matlab, self).__init__(
            args, cmds=cmds, config_args=config_args,
            build_args=build_args, cwd=_source_dir,
        )


class docs(BuildSubTask):

    def __init__(self, args, config_args=None, build_args=None):
        args.with_asan = False
        args.build_type = 'Debug'
        args.dont_build = False
        if config_args is None:
            config_args = []
        if build_args is None:
            build_args = []
        config_args += ['-DBUILD_DOCS=ON', '-DDOXYGEN_CHECK_MISSING=ON']
        build_args += ['--target', 'docs']
        super(docs, self).__init__(
            args, config_args=config_args,
            build_args=build_args,
        )


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
        '--verbose', '-v', action='store_true',
        help="Turn on verbose test output")
    parser_test.add_argument(
        '--stop-on-error', '-x', action='store_true',
        help="Stop running tests after the first error")
    parser_test.add_argument(
        '--preserve-output', action='store_true',
        help="Preserve test output")
    parser_ephoto = subparsers.add_parser(
        'ephoto', help="Run ephoto executable",
        func=ephoto)
    parser_ephoto.add_argument(
        '--input-dir', type=str, default=_source_dir,
        help="Directory containing input files")
    parser_ephoto.add_argument(
        '--enzyme-file', type=str, default='InputEnzyme.txt',
        help="File containing enzyme concentrations.")
    parser_ephoto.add_argument(
        '--grn-file', type=str, default='InputGRNC.txt',
        help="File containing transcription factor levels.")
    parser_ephoto.add_argument(
        '--evn-file', type=str, default='InputEvn.txt',
        help="File containing environmental parameters.")
    parser_ephoto.add_argument(
        '--output-file', type=str, default='output.data',
        help="File where output should be saved")
    parser_matlab = subparsers.add_parser(
        'compare-matlab', help="Compare C++ & MATLAB versions",
        func=compare_matlab)
    parser_matlab.add_argument(
        '--matlab', type=str, default='matlab',
        help="Path to the MATLAB executable")
    parser_matlab.add_argument(
        '--matlab-repo', type=str,
        default=os.path.join(
            os.path.dirname(_source_dir), 'ePhotosynthesis'),
        help="Path to the MATLAB version")
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
    args = parser.parse_args()
    parser.run_subparser('task', args)
