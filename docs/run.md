@page running Running the Binary


The ePhotosynthesis binary can be run from the command line with no command line options provided the default values are correct for you situation.

- - -
- - -

@section command_line_arguments Command Line Arguments

| Argument | Type | Default | Description |
| -------- | ---- | ------- | ----------- |
| <b><center>Inputs</center></b> ||||
| `-a, --atpcost` | File name | `InputATPCost.txt` | The `InputATPCost.txt` file. |
| `-e, --evn` | File name | `InputEvn.txt` | The `InputEvn.txt` file. |
| `-n, --enzyme` | File name | | The input enzyme file. Only needed if driver 4 is used. |
| <b><center>Module Control</center></b> ||||
| `-d, --driver` | Int | `1` | The driver to use. Choices are:<br>1 - trDynaPS_Drive<br>2 - DynaPS_Drive<br>3 - CM_Drive<br>4 - EPS_Drive |
| `-c, --c3` | | | Use the C3 model, automatically set to true for EPS driver. |
| `-h, --help` | | | Produce help message |
| `-T, --Tp` | Double | `0.0` | Input Temperature in degrees C. |
| <b><center>ODE Options</center></b> ||||
| `-b, --begintime` | Double | `0.0` | The starting time for the calculations. |
| `-m, --maxSubSteps` | Int | `750` | The maximum number of iterations at each time step. |
| `-z, --stepsize` | Double | `1.0` | The step size to use in the calculations. |
| `-r, --reltol` | Double | `1e-4` | Relative tolerance for calculations. |
| `-s, --stoptime` | Double | `5000.0` | The time to stop calculations. |
| `-t, --abstol` | Double | `1e-5` | Absolute tolerance for calculations. |
| <b><center>Other</center></b> ||||
| `-o, --options` | File name | | Name of a text file which specifies any of these options. Command line arguments have priority. (see @ref options_file for details) |
| `--debug` | Int | `0` | Debug level. (see @ref debugging for details) |
| `--debugDelta` | Boolean | `false` | Debug deltas. (see @ref debugging for details) |
| `--debugInternal` | Boolean | `false` | Debug internals. (see @ref debugging for details) |
| `-v, --verbose` | Boolean | `false` | Record output values for all steps (this can significantly slow the program). |

- - -
- - -

@section options_file Options File

If you have a standard set of command line options that you use you may want to consider using a options file. In an options file you specify the key word and value for any normal command line options. The format of the options file is

    keyword   value

with one keyword/value pair per line, and any type of white space between the two (tab(s)/space(s)). For example `myoptions.txt`:

    atpcost   MyATP.txt
    evn   MyEVN.txt
    driver  2
    maxSubSteps  1500
    reltol  1e-5
    stoptime  2000.0
    Tp  27.5
    
is run as 

    ePhoto -o myoptions.txt

and will produce the same result as 
    
    ePhoto -a MyATP.txt -e MyEVN.txt -d 2 -m 1500 -r 1e-5 -s 2000.0 -T 27.5

and has the added benefit of being reused multiple times.

- - -
- - -

@section debugging Debugging

If the code was compiled with debugging turned on (option `-DCMAKE_BUILD_TYPE=Debug` to `cmake`) then you can use `gdb` (or another debugger) to step through the program. In addition the current contents of different conditions can be output to `stdout`, using the `debug`, `debugDelta`, and `debugInternal` flags on the command line.

- - -

@subsection debugcl Debug Command Line Option

The `debug` command line option takes an integer as an argument to specify which conditions should be output to `stdout`. They are printed out when their respective differential calculators (MB) are called. For example the contents of PScondition will be printed when PS::MB is called. Note that when a condition is printed to `stdout` any sub-conditions are also printed out. The following table gives the full listing of possible debug values.

| Arg. | Meaning |
| ---- | ------- |
| 0    | No debugging info is printed (the default) |
| 1    | Only the top most condition is output, typically this is the condition with the same name as the driver. In the [example structure](@ref structImg) `A_Con` and its sub-condition `B_Con` and its sub-conditions `C_Con` and `D_Con` would be printed to `stdout`. |
| 2    | Only the mid-level conditions are printed. In the [example structure](@ref structImg) `B_Con` and its sub-conditions `C_Con` and `D_Con` would be printed to `stdout`.|
| 3    | The topmost condition and the mid-level conditions would be printed to `stdout`. |
| 4    | Only the lowest level conditions will be printed to stdout. In the [example structure](@ref structImg) `C_Con` and `D_Con` would be printed to `stdout`. |
| 5    | The topmost condition and lowest conditions will be printed to `stdout`. |
| 6    | The middle and lowest tier conditions will be printed to `stdout`. |
| 7    | All conditions at every level will be printed to `stdout`. This will be a huge amount of output. |

In general `debug` levels 1 and 4 are the most useful as 1 will report the contents of the conditions at the start of every ODE iteration, and 4 will report the conditions at the level the rate calculations are done. The other levels can be useful if you want to see what changes are happening to conditions during an iteration, but the output will be quite verbose.

- - -

@subsection dbdelta DebugDelta Command Line Option

The `debugDelta` command line options will report the results of the differential calculations (MB) to `stdout`. This option does not take arguments, its presence on the command line will turn it on, otherwise it is off. It operates independently from the `debug` option. Specifically this option will output the `dx/dt` vector that is returned by the differential calculator. In the [example structure](@ref structImg) this is every `dx/dt`.

- - -

@subsection dbinternal DebugInternal Command Line Option

The `debugInternal` command line option is used to output the contents of the VEL classes, typically the results of the rate calculators. This option does not take arguments, its presence on the command line will turn it on, otherwise it is off. It operates independently from the `debug` option.

