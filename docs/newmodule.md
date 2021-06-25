Adding A New Module     {#newparts}
===================

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
