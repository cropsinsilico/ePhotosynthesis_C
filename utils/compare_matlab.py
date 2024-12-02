import os
import argparse
import difflib
import subprocess
import numpy as np

_drivers = ['trDynaPS', 'DynaPS', 'CM', 'EPS']
_driver_map = {(i + 1): x for i, x in enumerate(_drivers)}


_repo_dir = os.path.abspath(os.path.dirname(os.path.dirname(__file__)))


def run_matlab(args):
    cmd = [
        args.matlab, '-nodisplay', '-nosplash', '-nodesktop',
        '-nojvm', '-r',
        f'ePhotosynthesis {args.driver} {args.env} {args.grn}'
        f' {args.enzyme} {args.output} 1, exit',
    ]
    out = subprocess.run(cmd, cwd=args.matlab_dir)
    return out


def run_cpp(args):
    cmd = [
        os.path.join(args.cpp_dir, 'ePhoto'), '-d', str(args.driver),
        '--evn', args.env, '--grn', args.grn, '--enzyme', args.enzyme,
        '--output', args.output, '--outputParam', '--stoptime', '3000',
    ]
    out = subprocess.run(cmd)
    return out


def compare_files(f1, f2, check_files=None):
    with open(f1, 'r') as fd:
        lines1 = fd.readlines()
    with open(f2, 'r') as fd:
        lines2 = fd.readlines()
    line_diff = list(difflib.unified_diff(lines1, lines2,
                                          fromfile=f1, tofile=f2))
    if line_diff:
        if (check_files is not None) and check_files(f1, f2):
            return
        print(line_diff)
        line_diff = '\n'.join(line_diff)
        raise RuntimeError(
            f"Parameter files differ:\n{line_diff}"
        )


def check_output(f1, f2):
    with open(f1, 'r') as fd:
        x1 = float(fd.read().strip())
    with open(f2, 'r') as fd:
        x2 = float(fd.read().strip())
    reltol = 1.0e-05
    abstol = 1.0e-08
    out = np.isclose(x1, x2, rtol=reltol, atol=abstol)
    if not out:
        reldiff = np.abs(x1 - x2) / x2
        absdiff = np.abs(x1 - x2)
        print(f"Values differ: {x1} vs {x2}\n"
              f"    Relative diff: {reldiff} (reltol = {reltol})\n"
              f"    Absolute diff: {absdiff} (abstol = {abstol}\n")
    return out


def diff(args, out1, out2):
    finit = _driver_map[args.driver] + '_init.txt'
    finit1 = os.path.join(args.matlab_dir, finit)
    finit2 = finit
    compare_files(finit1, finit2)
    fout1 = os.path.join(args.matlab_dir, args.output)
    fout2 = args.output
    compare_files(fout1, fout2, check_output)


def compare(args):
    if args.driver == 0:
        for i in range(len(_drivers)):
            args.driver = i + 1
            compare(args)
        return
    if args.dont_run_matlab:
        out_matlab = False
    else:
        out_matlab = run_matlab(args)
    out_cpp = run_cpp(args)
    diff(args, out_matlab, out_cpp)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "Check the different between the MATLAB & C++ versions of "
        "the ePhotosynthesis model")
    parser.add_argument("matlab_dir", type=str,
                        help=("Directory containing the matlab version "
                              "of ePhotosynthesis"))
    parser.add_argument("--cpp-dir", type=str,
                        default=os.path.join(_repo_dir, 'build'),
                        help=("Directory containing the executable "
                              "for the C++ version of ePhotosynthesis"))
    parser.add_argument("--driver", '-d', type=int,
                        default=0, choices=[0, 1, 2, 3, 4],
                        help="Driver to run")
    parser.add_argument("--env", type=str,
                        default=os.path.join(_repo_dir,
                                             'InputEvn_MATLAB.txt'),
                        help=("File containing environment parameters"))
    parser.add_argument("--enzyme", type=str,
                        default=os.path.join(_repo_dir,
                                             'InputEnzyme.txt'),
                        help=("File containing enzyme activities"))
    parser.add_argument("--grn", type=str,
                        default=os.path.join(_repo_dir,
                                             'InputGRNC_MATLAB.txt'),
                        help=("File containing gene ratios"))
    parser.add_argument("--output", type=str,
                        default='output.data',
                        help=("File where output should be saved"))
    parser.add_argument("--dont-run-matlab", action='store_true',
                        help=("Don't run MATLAB, but assume it has "
                              "already been run with the required "
                              "parameters "))
    parser.add_argument("--matlab", type=str,
                        default='matlab',
                        help=("Location of the MATLAB executable"))
    args = parser.parse_args()
    # parse_known_args
    compare(args)
