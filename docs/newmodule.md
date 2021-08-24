@page newparts Adding A New Module

Each module consists of a few basic parts:

- A condition class for holding data
- A Vel class for holding the rate calculation results (standalone modules only)
- Ouput functions for the condition and Vel classes
- An initializer
- A rate calculation function (standalone modules only)
- A differential calculation module (MB)
- A driver (optional)

Example files for each of these are located in the templates directory. In each of the examples block comments in the code have been removed.

- - -
- - -

@section helpers Helper Macros

The file definitions.hpp includes three macros which can help with static class members.

- - -

The \b `SET_GET` macro is used to create the given variable (as a double) and define setter and getter functions for it. The variable will be private while the setter/getter are public.

\snippet include/definitions.hpp SET_GET

Using the macro the following code snippets are identical in function.

    SET_GET(foo)

and

    public:
        static double getFoo(){ return Foo;}
        static void setFoo(const double val) {Foo = val;}
    private:
        static double Foo;

- - -

The \b `SET_GET_BOOL` macro is identical to the \b SET_GET macro, but for booleans.

\snippet include/definitions.hpp SET_GET_BOOL

- - -

The \b `SET_GET_BOOL_MODULE` macro is used to create and manage a boolean variable in a Module class, that has an identical variable at the Condition class level.

\snippet include/definitions.hpp SET_GET_BOOL_MODULE

The following snippets are identical in function.

    SET_GET_BOOL_MODULE(Foo, Bar)

and

    public:
        static bool getFoo() {return Foo;}
        static void setFoo(const bool val) {
            Foo = val;
            BarCondition::setFoo(val);
        }
    private:
        static bool Foo;

- - -
- - -

@section addsingle Adding a Standalone Module

This section describes how to add a new standalone module to the code. A standalone module is one that does not have any modules lower than it in the hierarchy. For details on adding a module which has direct dependencies see the next section.

- - -

@subsection addingscond Adding A Condition

Example files are detailed below. In each file replace `Template` with the name of your new condition. The header example gives the required elements of a condition class, other functions can be added as needed. The condition class headers are in include/conditions.

The example header file is located in `templates/include/conditions/TemplateCondition.hpp`.
\include{lineno} include/conditions/TemplateCondition.hpp

Line # | Notes
------ | -----
3      | All condition classes must inherit from the `ConditionBase` class, add any other includes as needed.
14-15  | Namspacing for the library and the conditions.
17     | Pre-declare the parent Condition (if there is one)
19     | Condition classes must inherit from the `ConditionBase` class. The `ConditionBase` class is a template that takes two arguments, the first is the name of your new class, the second is the parent condition (the condition that holds your new condition in the data hierarchy). If this is the topmost condition then just put your new class name in for the `Parent_Condition` name.
21-37  | The constructors, additional ones can be added, but these three must exist.
39     | Any condition data members should go here. They should be public to allow for ease of access from other modules. At this point they all must be `double` type or a pointer to another condition class.
41     | The `_print` function is used to print the contents of the condition during debugging.
43     | The base class must be noted as a friend so it can access the private functions.
50     | The `_fromArray` function converts the input vector into the contents of the condition. The offset parameter is used when the condition's data do not start at index 0 of the input data.
56     | The `_toArray` function is the opposite of the `_fromArray` function, converting the contents of the condition to a vector. \b Note: `_fromArray` and `_toArray` must be mirrors of each other, if vector X is sent to `_fromArray` and this output is sent to `_toArray`, the result must be equal to the input X.
61     | Function that returns the number of data members in the condition, also this is the length of the vector returned by `_toArray`.
65     | Function to clear the current data members. Not usually used in lower level Condition classes.
66     | Static function to reset all static variables to their base state.
68     | Variable to hold the number of data members.
70     | Static variable used to hold the debugging level for this class.

All functions are implemented in a cpp file located in src/conditions.

The example implementation file is located in `templates/src/conditions/TemplateCondition.cpp`.
\include{lineno} src/conditions/TemplateCondition.cpp

Line # | Notes
------ | -----
1      | Include the appropriate header.
3-4    | Set the proper namespace.
7      | Declare the number of items in the class (this is how long the input/output vectors to `_fromArray` and `_toArray` will be.
9-11   | Copy constructor, each item of the class should be copied individually.
13-15  | Constructor to create the class from an input vector, this should always call `fromArray` for consistency.
17-22  | Implementation of converting an input vector to this class.
24-29  | Implementation of converting this class to a vector.

- - -

@subsection addvel Adding a Vel Class

The Vel class is used to hold the results of the rate calculations and is a header-only implementation.

The example header file is located in `templates/include/vel/TemplateVel.hpp`.
\include{lineno} include/vel/TemplateVel.hpp

Line # | Notes
------ | -----
18-19  | The class does not inherit from anything and all functions and data members are public.
20     | Basic constructor.
26     | Copy constructor, should copy all data member values from the input class.
30     | All data members should be of \b `double` type.
32     | Add the stream operator as a friend so the data can be output for debugging.

An entry for this class should be added to the Variables class in `include/Variables.hpp`.

    TemplateVel TVel;

- - -

@subsection outsing Output Functions

Both the ConditionBase and TemplateVel classes declared output functions. These functions should be implemented in their own files.

The example Condition output can be found in `templates/src/output/Template_Con_output.cpp`.
\include{lineno} src/output/Template_Con_output.cpp

Line # | Notes
------ | -----
1      | Include the appropriate headers.
2      | Set the namespaces.
4-10   | Output the class contents to the stream.
5      | This line is required as it allows for global control of output indentation, making the hierarchical structure visible on output.
6      | The name of the class should be output.
7+     | Each data member should be output with a string giving the variable name, followed by the value.

The example Vel output can be found in `templates/src/output/Template_Vel_output.cpp`.
\include{lineno} src/output/Template_Vel_output.cpp

Line # | Notes
------ | -----
1      | Include the appropriate headers.
3-4    | Set the namespaces.
5-10   | Output the class contents to the stream.
6      | The class name should be output.
7+     | Each data member should be output with a string giving the variable name, followed by the value.
    
- - -

@subsection newsmod Adding A Module

An example header file is detailed below. In all files replace `Template` with the name of your new Module. The header example gives the required elements of a module class, other functions can be added as needed. The module class headers are in include/modules.

The example header file is located in `templates/include/modules/TemplateModule.hpp`.
\include{lineno} include/modules/TemplateModule.hpp

Line # | Notes
------ | -----
3-4    | All module classes must inherit from the `ModuleBase` class, add any other includes as needed (the associated Condition class is also included).
18     | The `ModuleBase` class is a template that takes two arguments, the first is the name of your new class, the second is the name of the condition class associated with the module. The templated base class presents all the necessary public methods
27-50  | All methods in the module class are private, `ModuleBase` is noted as a friend to give it access to the functions.
27     | The `_init` function initializes the condition associated with this class, and returns the associated class.
37     | The `_MB` is the differential calculation function, returning a vector of `dx/dt` values.
39     | The `_MB_con` is the same as the _MB function, but returns as Condition class instead. It is preferable to call the _MB_con function as it skips the steps of converting the results to an array.
49     | The `_Rate` function does the rate calculations based on the input condition class.

The implementation of these functions is broken up into three separate files in src/ini, src/MB, and src/rate.

The example Init implementation is in `templates/src/ini/Template_Ini.cpp`.
\include{lineno} src/ini/Template_Ini.cpp

Line # | Notes
------ | -----
1      | Include the appropriate header.
3-5    | Set the namespaces.
7-11   | Initialize any variables and create the initial `TemplateCondition` class for feeding into the calculations.

The example MB implementation is in `templates/src/MB/Template_MB.cpp`.
\include{lineno} src/MB/Template_MB.cpp

Line # | Notes
------ | -----
1-2    | Include the appropriate headers.
4-6    | Set the namespaces.
8-23   | Do the differential calculations.
12     | Call the rate calculator.
15     | Create the class to return the differentials.
18-20  | `ifdef` statement for any debugging output.
22     | return the differential results.
25-31  | The version of the calculations which returns a vector instead of an object. This just calls the object version under the hood and then serializes the output.

The example Rate implementation is in `templates/src/rate/Template_Rate.cpp`.
\include{lineno} /src/rate/Template_Rate.cpp

Line # | Notes
------ | -----
1-2    | Include the appropriate headers.
4-6    | Set the namespaces.
8-10   | Perform the rate calculations, the results of which go in `theVars->TVel`.

- - -
- - -

@section adddep Adding a Dependent Module

This section describes how to add a new dependent module to the code. A dependent module is one that does has one or more modules lower than it in the hierarchy. 

- - -

@subsection addingdcond Adding A Condition

Example files are detailed below. In each file replace `Dependent` with the name of your new condition. The header example gives the required elements of a condition class, other functions can be added as needed. Dependent Conditions can contain only one child Condition class, or many, the example gives details of a Condition with two child Condition classes (ACondtion and BCondition). The condition class headers are in include/conditions.

The example header file is located in `templates/include/conditions/DependentCondition.hpp`.
\include{lineno} include/conditions/DependentCondition.hpp

Line # | Notes
------ | -----
3-4    | Include appropriate headers (at least one for each sub-Condition class)
15-19  | Namspacing for the library and the conditions.
17     | Pre-declare the associated DependentModule class;
21     | Pre-declare the parent Condition (if any).
23     | Condition classes must inherit from the `ConditionBase` class. The `ConditionBase` class is a template that takes two arguments, the first is the name of your new class, the second is the parent condition (the condition that holds your new condition in the data hierarchy). If this is the topmost condition then just put your new class name in for the `Parent_Condition` name.
25-44  | The constructors, additional ones can be added, but these four must exist.
25     | The default constructor should instantiate new instances of the child classes.
36     | Unlike the standalone instance, the dependent Condition needs to have a constructor that takes any child classes as arguments.
51+    | Any condition data members should go here. They should be public to allow for ease of access from other modules. Dependent Condition classes should have a data member for each of the child classes, and can also have their own variables of type `double`.
53     | The `_print` function is used to print the contents of the condition during debugging.
55     | The base class must be noted as a friend so it can access the private functions.
56     | The associated Module class should also be added as a friend;
63     | The `_fromArray` function converts the input vector into the contents of the condition. The offset parameter is used when the condition's data do not start at index 0 of the input data.
69     | The `_toArray` function is the opposite of the `_fromArray` function, converting the contents of the condition to a vector. \b Note: `_fromArray` and `_toArray` must be mirrors of each other, if vector X is sent to `_fromArray` and this output is sent to `_toArray`, the result must be equal to the input X.
74     | Function that returns the number of data members in the condition, also this is the length of the vector returned by `_toArray`. Dependent Conditions do not hold their own size, but get it from summing the sizes of the child classes.
80     | Function to clear the current data members.
81     | Static function to reset all static variables to their base state.
85     | Variable to hold the number of data members.
87     | Static variable used to hold the debugging level for this class.

All functions are implemented in a cpp file located in src/conditions.

The example implementation file is located in `templates/src/conditions/DependentCondition.cpp`.
\include{lineno} src/conditions/DependentCondition.cpp

Line # | Notes
------ | -----
1      | Include the appropriate header.
3-4    | Set the proper namespace.
7      | Set the initial value of the size to 0. It will be calculated from the child Condition classes.
9-14   | Copy constructor. Since each child Condition class instance can have only one parent, the code should copy the contents of the input classes children to new instances.
16-29  | Constructor from the two child Condition classes. In line with the copy constructor the code checks to see if the input classes have any parent. If they do then the inputs are copied to new instances, if not then this class takes ownership of them.
31-33  | Constructor to create the class from an input vector, this should always call `fromArray` for consistency.
35-45  | Implementation of converting an input vector to this class. If either of the child instances are `nullptr` then a new instance is created and the input vector is given to each in turn, increasing the offset for each subsequent child.
47-55  | Implementation of converting this class to a vector. How this is done does not matter, as long as all data members are in the correct order.
57-66  | Function to delete the child classes if they have instances.

- - -

@subsection outding Output Functions

As with the standalone Condition case, an output function must be defined.

The example dependent Condition output can be found in `templates/src/output/Dependent_Con_output.cpp`.
\include{lineno} src/output/Dependent_Con_output.cpp

Line # | Notes
------ | -----
1      | Include the appropriate headers.
2      | Set the namespaces.
4-10   | Output the class contents to the stream.
5      | This line is required as it allows for global control of output indentation, making the hierarchical structure visible on output.
6      | The name of the class should be output.
7+     | Each child Condition class's `_print` function should be called, adding 1 to the tab parameter for each. Any other data members should be output with a string giving the variable name, followed by the value.

@subsection newdmod Adding A Module

An example header file is detailed below. In all files replace `Dependent` with the name of your new Module. The header example gives the required elements of a module class, other functions can be added as needed. The module class headers are in include/modules.

The example header file is located in `templates/include/modules/DependentModule.hpp`.
\include{lineno} include/modules/DependentModule.hpp

Line # | Notes
------ | -----
3-4    | All module classes must inherit from the `ModuleBase` class, add any other includes as needed (the associated Condition class is also included).
18     | Pre-declare the parent Module class (if any).
19     | The `ModuleBase` class is a template that takes two arguments, the first is the name of your new class, the second is the name of the condition class associated with the module. The templated base class presents all the necessary public methods
21     | The base class is declared a friend to give it access.
22     | The parent Module class is declared a friend as well.
29-56  | All methods in the module class are private.
29     | The `_init` function initializes the condition associated with this class, and returns the associated class.
39     | The `_MB` is the differential calculation function, returning a vector of `dx/dt` values.
41     | The `_MB_con` is the same as the _MB function, but returns as Condition class instead. It is preferable to call the _MB_con function as it skips the steps of converting the results to an array.
52     | The `_Rate` function does no work in the dependent case, and is just an empty function.
55     | The `_reset` function will reset any static class variables to their base values.

The implementation of these functions is broken up into two separate files in src/ini and src/MB.

The example Init implementation is in `templates/src/ini/Dependent_Ini.cpp`.
\include{lineno} src/ini/Dependent_Ini.cpp

Line # | Notes
------ | -----
1      | Include the appropriate header.
3-5    | Set the namespaces.
7-11   | Initialize the child Conditions and use them to initialize the DependentCondition.

The example MB implementation is in `templates/src/MB/Dependent_MB.cpp`.
\include{lineno} src/MB/Dependent_MB.cpp

Line # | Notes
------ | -----
1-2    | Include the appropriate headers.
4-6    | Set the namespaces.
8-23   | Do the differential calculations.
9-10   | Get a copy of the pointer to the input child conditions.
14-15  | Call the differential calculators for the child Modules.
18     | Create the class to return the differentials, based on the results of the child differentials.
21-23  | `ifdef` statement for any debugging output.
25     | return the differential results.
28-34  | The version of the calculations which returns a vector instead of an object. This just calls the object version under the hood and then serializes the output.


- - -
- - -

@section newdrive Adding A New Driver

The driver is the class the controls the path the processing takes. It defines the entry point in the hierarchy and returns the results of the calculations. A driver example is given in `templates/include/drivers/TemplateDriver.hpp`. In this example `Template` is interchangeable with `Dependent` in the class namings.

\include{lineno} include/drivers/TemplateDriver.hpp

Line # | Notes
------ | -----
3-5    | Include appropriate headers.
14-15  | Declare the current namespaces.
16     | All driver classes must inherit from the Driver base class
21-28  | Constructor for the class. It must have the given input parameters, but additional ones can be added.
25-27  | Setting for debugging.
30     | Destructor.
35     | The setup function prepares the condition classes for processing.
40     | The getResults function returns the results of the calculations, placing them in a class variable.
46     | The differential calculator function. It takes the inputs as given by the Sundials ODE framework, converts them to condition class(es) and calls the underlying _MB method for the module.
52     | The Init function prepares and calls the underlying _init function from the module, returning the prepared condition class.


The driver implementation can be found in `templates/src/drivers/Template_driver.cpp`.
\include{lineno} src/drivers/Template_Driver.cpp

Line # | Notes
------ | -----
1-4    | Include appropriate headers.
6-9    | Set the namespaces.
18-20  | The destructor should reset the module to its default state.
27-34  | Setup the constraints before processing starts.
36-43  | Set the code to do one last run
48-50  | Initialize the module.
54-65  | Convert the input `N_Vector` to a C type, create a Conditions class from this, run the differential calculator, and return the results.