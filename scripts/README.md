#Overview

The original Matlab code was converted to C++ using both scripts and manual coding. The [convert.py](convert.py) script was used to
handle the bulk of the conversion.

##Process

The script to do the initial conversion looks for all files in the current directory that have the suffix '.m'. It the makes multiple
passes through all of the files, gathering information and refining the code. Before the script is run, any files which do plotting
in Matlab need to be manually deleted, as currently the C++ code does not support the plotting functionality.

During the conversion process it was assumed that all variables were double precision or an array of double precision items, unless:
- The variable was only used in comparisons in `if` statements and was the only thing on its side of the comparator, then it is assumed to be a boolean
- The variable was only used for counting or as an index, then it is assumed to be a size_t or int

###Automated
During the passes through the code the script does the following:
- Replace the Matlab comment (%) with the C++ version (//)
- Remove extraneous spaces (e.g., no spaces before a terminating ';')
- Properly space around operators, parenthses, and brackets
- Locate function definitions, gathering arguments and return values
- Locate global variables (there are 450+ of them)
  - Determine if any of the variables is an array by looking at any assignment operations
- Locate calls to the ODE solver
- Locate local variables
  - Determine if any of the variables is an array
- Find function calls to determine the data types of the arguments and track where each function is called
- Track where and how often local and global variables are used
- Convert from `^` to `pow()`
- Determine which local and global variables are constants (i.e., only have a single assignment call)
- Convert from Matlab to C++ array formats
  - `()` to `[]`
  - Assignments that used `:` notation are converted to for loops over the appropriate indices
  - Indices are decremented to convert from the Matlab 1 based indexing to the C++ 0 based
- Convert uses of global variables to be members of a single `struct`
- Convert `if` statements, done recursively for nested statements
- Convert for loops, done recursively for nested statements
- Add instantiations to local variables (`const double`, `double`, etc.) to the first assigment statement
- Comment out any unused local and global variables (unused variables are those that only have assignment statements and no statementes where the variables is used otherwise)
- Run a consistency check on all function calls to make sure all function calls have the correct number of arguments
  - Any calls that do not have the correct number of arguments (based on the API of the function) are flagged
  - Any files with flagged calls are deleted
  - Check that the functions defined in the deleted files are not called by any other code
- Write out the C++ files
- Generate a header with all function signatures and the global data structure

###Manual
Once the script completes it is unlikely that the code will compile as there will be corner cases which the script did not see due to its
strict pattern matching parameters. Strict pattern matching was chosen as it would be much less likely to produce broken code when compared to
overly inclusive pattern matching. The following were done manually afterward:

- Many variable assignments were deemed to be unneeded and were simplified.
  ```
  double var1 = input1;

  double newvar = var1;
  ```
  would be converted to
  ```
  double newvar = input1;
  ```
  and
  ```
  double var1 = input1 * 3.5;

  double newvar = var2 * 8 * var1;
  ```
  would be converted to
  ```
  double newvar = var2 * 8 * input1 * 3.5;
  ```
  but
  ```
  double var1 = input1 * 3.5;

  double newvar = var2 * 8 * var1;
  double newvar2 = 18.3 * var1;
  ```
  would not change because the original assignment of `var1` included a math operation and `var1` was used more than once.
- Input data and result data from the Rate calculations were does with arrays in Matlab. However, tracing a single element
  of an array can be very tedious as the multiple output arrays are re-ordered and combined from the many Rate calculations.
  Thus in the C++ code these were converted to classes so that each variable can be traced much easier. As the ODE solver
  uses array like structures to pass the input to, and output from, the code it is evaluating, these classes have
  to be flattened into a single array for this purpose. Each class is repsonsible for converting its own data members into
  the appropriate array.
- The header was broken up into a single header for each 'module'. A module is defined as all code associated with a Rate calculation
  (e.g., BF_Ini.cpp, BF_Mb.cpp, and BF_Rate.cpp would all be part of the BF module). Each module is technically a class containing all
  the code for the module.
- All global variables were evaluated to see if they were only used in a single module. If they were they were moved to the module class,
  this greatly reduced the memory footprint of the global data structure (now only ~150 variables).
- The 2D arrays that are used to track the intermediate results were converted to TimeSeries objects to make indexing easier and reduce the
  number of lines of code
- Theuse of these tracking data structures can be triggered with a command line argument (using the tracking can increade the run time of the code by a factor of 100)
- Command line arguments were added to give the user control over the time steps that the calculations use
