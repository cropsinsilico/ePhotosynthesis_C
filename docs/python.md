@page python Calling the Code from Python


Only the Driver classes, the Variables class, and specific static module interfaces have exposed API's, as they are the only classes that need to be interacted with directly. These classes have the same calling signature in both C++ and Python. Below is an example Python session.

\code{.py}
import pyPhotosynthesis as pyPh

variables = pyPh.Variables()

variables.CO2_in = 400.
variables.TestLi = 800.
variables.TestATPCost = 0.
variables.RUBISCOMETHOD = 1
pyPh.modules.PR.setRUBISCOTOTAL(3)
begin = 0.
stepsize = 1.
stoptime = 5000.
maxSubSteps = 1500
abstol = 1e-5
reltol = 1e-4

driver = pyPh.drivers.trDynaPSDriver(variables, begin, stepsize, stoptime, maxSubSteps, abstol, reltol, 1, 1, False)

results = driver.run()

for i in results:
    print(f"{i:.3f}") 

\endcode

In this example the module is imported, the global ePhotosynthesis::Variables class is instantiated and populated with the initial values. The inputs to the driver class are defined, then an instance of the ePhotosynthesis::drivers::trDynaPSDriver class is created. `run()` is then called, placing the results of the calculations in `results`.

In addition to the Driver and Variables classes the following static member functions are also available in Python:
  - ePhotosynthesis::modules::PR::setRUBISCOTOTAL()
  - ePhotosynthesis::modules::CM::setTestSucPath()
