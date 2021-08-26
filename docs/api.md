@page api API

This documentation is aimed at those who intend to use the ePhotosynthesis library as opposed to the stand alone binary. For the general use case code will only need to interact with a Driver class, giving it a few inputs and letting it handle the minutia. For those wanting to have finer control over the ODE functionality see the \ref apidet section below.

@section callingCode Calling the Code

For the basic use case there are only two objects which must be interacted with: a Variables instance and an instance of one of the Driver classes. The Variables class is used to hold variables on the global scope, intermediate results for each ODE iteration, and if requested a copy of the Rate calculations for each step. The Driver class is used to initialize the input variables, initialize and run the ODE solver, and return the results to the user. A few of the Variables data members may need to be changed from their default value for the computation to succeed. These include (but are not limited to):

- \b CO2_in The initial CO<sub>2</sub> concentration.
- \b TestLi The light strength?
- \b TestATPCost The ATP cost.
- \b useC3 Whether to use the C3 method of calculcations or not.
- \b RUBISCOMETHOD Which Rubisco calculation method to use.
- \b EnzymeAct Map of the input enzyme levels.

The different Driver classes take different sets of inputs (see the individual API documentations), but all take the following:

- \b start The time stamp to start calculations at.
- \b step The optimal timestep to take for each iteration (decreasing this value can help the the ODE solver is having trouble converging on a solution).
- \b endtime The timestamp to stop calculations at.
- \b maxSubSteps The maximum number of substeps to take for each step (increasing this value can help when the ODE solver is having trouble converging on a solution).
- \b atol Absolute tolerance the ODE solver will use to determine convergence.
- \b rtol Relative tolerance the ODE solver will use to determine convergence.
- \b showWarnings Whether to send convergence warning messages to stdout or not.

All drivers return a vector of doubles containing the results of the calculations. See the API documentation for the drivers for details of what each returns.

The following code snippet gives the general minimum code needed to interface with the ePhotosynthesis library (namespaces have been omited for clarity).

\code{.cpp}
    // instantiate a Variables instance
    Variables* theVars = new Variables();
    
    // set some basic parameters
    theVars->CO2_in = 800.;
    theVars->TestLi = 50.;
    theVars->TestATPCost = 1.5;
    theVars->RUBISCOMETHOD = 1;
    
    double start = 0.;
    double step = 1.;
    double end = 1000.;
    int maxSubSteps = 1500;
    double abstol = 1e-5;
    double reltol = 1e-4;
    
    // instantiate the driver
    trDynaPSDriver* driver = new trDynaPSDriver(theVars, start, step, end, maxSubSteps, abstol, reltol, 1, 1);
    
    // run the calculations and get the results
    std::vector<double> results = driver->run();
    
    // clean up
    delete driver;
    delete theVars;
\endcode

@section apidet Details
