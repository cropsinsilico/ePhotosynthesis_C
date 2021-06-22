Documentation    {#mainpage}
=============

This page is aimed at anyone who wants to modify or create new components
of the ePhotosynthesis code. The following sections discuss the [conversion from
Matlab to C++](#conversion), [the current structure](#structure) of the code, and
how to [add new parts](#newparts).

## Conversion from Matlab to C++ ##     {#conversion}

The ePhotosynthesis code was originally written in Matlab. It was converted to
C++ to make it faster and more portable. The conversion was done using both scripts and manual coding. The [convert.py](convert.py) script was used to
handle the bulk of the conversion.

### Process ###

The script to do the initial conversion looks for all files in the current directory that have the suffix '.m'. It the makes multiple
passes through all of the files, gathering information and refining the code. Before the script is run, any files which do plotting
in Matlab need to be manually deleted, as currently the C++ code does not support the plotting functionality.

During the conversion process it was assumed that all variables were double precision or an array of double precision items, unless:
- The variable was only used in comparisons in `if` statements and was the only thing on its side of the comparator, then it is assumed to be a boolean
- The variable was only used for counting or as an index, then it is assumed to be a size_t or int

#### Automated ####

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
- Add instantiations to local variables (`const double`, `double`, etc.) to the first assignment statement
- Comment out any unused local and global variables (unused variables are those that only have assignment statements and no statements where the variables is used otherwise)
- Run a consistency check on all function calls to make sure all function calls have the correct number of arguments
  - Any calls that do not have the correct number of arguments (based on the API of the function) are flagged
  - Any files with flagged calls are deleted
  - Check that the functions defined in the deleted files are not called by any other code
- Write out the C++ files
- Generate a header with all function signatures and the global data structure

#### Manual ####
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
  to be flattened into a single array for this purpose. Each class is responsible for converting its own data members into
  the appropriate array.
- The header was broken up into a single header for each 'module'. A module is defined as all code associated with a Rate calculation
  (e.g., BF_Ini.cpp, BF_Mb.cpp, and BF_Rate.cpp would all be part of the BF module). Each module is technically a class containing all
  the code for the module.
- All global variables were evaluated to see if they were only used in a single module. If they were they were moved to the module class,
  this greatly reduced the memory footprint of the global data structure (now only ~150 variables).
- The 2D arrays that are used to track the intermediate results were converted to TimeSeries objects to make indexing easier and reduce the
  number of lines of code
- The use of these tracking data structures can be triggered with a command line argument (using the tracking can increase the run time of the code by a factor of 100)
- Command line arguments were added to give the user control over the time steps that the calculations use

The data that are passed to the ODE solver as the current values are held in arrays in both
the Matlab and C++ code. The Matlab code always kept these as arrays, access by the index number.
This can make it very cumbersome to track individual variables through the code. In the C++
version of the code these data arrays were converted to container classes with named variables
in place of the indexes, making it much easier to track individual variables.


## Structure ##     {#structure}

The input data for ePhotosynthesis are broken up and worked on in a hierarchical manner. The hierarchy is
described below, working from the lowest, most basic level, up.

### Hierarchy ###

#### Containers ####

The container classes are the basic data storage members of the system. The container classes
are mostly variables with a few common functions to convert the data to an array and back. Some
container classes also contain pointers to other container classes. For example the
FIBFContainer class has pointers to both FIContainer and BFContainer classes.

### Rate Calculators ###

The rate calculators are the part of the code that does the bulk of the calculations at each time step.
They are given a container class as input and use these data to calculate the values at the next time step.

### MB code ###

The MB classes contain the differential equation part of the calculation. They call either other MB classes
or their rate calculator. The results of these calls are then run through the differential equations
and the dx/dt values are returned to the caller.

### Driver classes ###

The driver class is the part of the code that dictates what module is called in what order. In each
execution of the ePhotosynthesis code only a single driver can be used. The driver is responsible for
initializing any variables that need it, calling the ODE solver, and processing the ODE output.

This image illustrates the call structure of ePhotosynthesis. The functions are in the boxes and ovals, with arrows
connecting the callers and callees. Red text indicates return values while blue text indicates input
values.

\image html flow.svg
\image latex flow.jpg


## Adding A New Module ##     {#newparts}

Each module consists of a few basic parts:

- A container class for holding data
- An initializer (optional)
- A rate calculation function (optional)
- A differential calculation module (MB)
- A driver (optional)

Example files for each of these are located in the templates directory. In each of the examples block comments in the code have been removed.

### Adding A New Container ##

Example files are detailed below. In each file replace ContainerTemplate with the name of your new container. The header example gives the required elements of a container class, other functions can be added as needed. The container class headers are in include/containers.

\include{lineno} ContainerTemplate.hpp

Line # | Notes
------ | -----
3      | All container classes must inherit from the ContainerBase class, add any other includes as needed.
15     | The ContainerBase class is a template that takes two arguments, the first is the name of your new class, the second is the parent container (the container that holds your new container in the data hierarchy). If this is the topmost container then just put your new class name in for the Parent_Container name.
17-33  | The constructors, additional ones can be added, but these three must exist.
35     | Any container data members should go here. They should be public to allow for ease of access from other modules. At this point they all must be `double` type or a pointer to another container class.
37     | The _print function is used to print the contents of the container during debugging.
39     | The base class must be noted as a friend so it can access the private functions.
46     | The _fromArray function converts the input vector into the contents of the container. The offset parameter is used when the container's data do not start at index 0 of the input data.
52     | The _toArray function is the opposite of the _fromArray function, converting the contents of the container to a vector. \b Note: _fromArray and _toArray must be mirrors of each other, if vector X is sent to _fromArray and this output is sent to _toArray, the result must be equal to the input X.
57     | Function that returns the number of data members in the container, also this is the length of the vector returned by _toArray.
61     | Function to clear the current data members.
63     | Variable to hold the number of data members.

All functions are implemented in a cpp file located in src/containers.


### Adding A New Module ###

An example header file is detailed below. In all files replace ModuleTemplate with the name of your new Module. The header example gives the required elements of a module class, other functions can be added as needed. The module class headers are in include/modules.

\include{lineno} ModuleTemplate.hpp

Line # | Notes
------ | -----
3      | All module classes must inherit from the ModuleBase class, add any other includes as needed.
14     | The ModuleBase class is a template that takes two arguments, the first is the name of your new class, the second is the name of the container class associated with the module.
15-16  | All methods in the module class are private, ModuleBase is noted as a friend to give it access to the functions.
23     | The _init function initializes the container associated with this class.
33     | The _MB is the differential calculation function, returning a vector of dx/dt values.
42     | The _Rate function does the rate calculations based on the input container.

The implementation of these functions is broken up into three separate files in src/ini, src/MB, and src/rate.

### Adding A New Driver ###

The driver is the class the controls the path the processing takes. It defines the entry point in the hierarchy and returns the results of the calculations. A driver example is given below.

\include{lineno} DriverTemplate.hpp

Line # | Notes
------ | -----
3      | All driver classes must inherit from the Driver base class
4      | The container class associated with the module.
19-20  | Constructor for the class. It must have the given input parameters, but additional ones can be added.
30     | The setup function prepares the containers for processing.
35     | The getResults function returns the results of the calculations, placing them in a class variable.
41     | The differential calculator function. It takes the inputs as given by the Sundials ODE framework, converts them to container class(es) and calls the underlying _MB method for the module.
47     | The Init function prepares and calls the underlying _init function from the module, returning the prepared container class.
