Structure     {#structure}
=========

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

@anchor structImg

@image html flow.svg "Fig 1: An example structure"
@image latex flow.jpg "Fig 1: An example structure"
