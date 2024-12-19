import os
import pytest
import numpy as np
import ePhotosynthesis
from ePhotosynthesis import Variables


@pytest.fixture(scope="module")
def datadir():
    testsdir = os.path.dirname(os.path.dirname(__file__))
    return os.path.join(testsdir, 'data')


@pytest.fixture(scope="module")
def fname_InputEnzyme(datadir):
    return os.path.join(datadir, "InputEnzyme.txt")


@pytest.fixture(scope="module")
def fname_InputEvn(datadir):
    return os.path.join(datadir, "InputEvn.txt")


@pytest.fixture(scope="module")
def fname_InputATPCost(datadir):
    return os.path.join(datadir, "InputATPCost.txt")


@pytest.fixture(scope="module")
def fname_InputGRNC(datadir):
    return os.path.join(datadir, "InputGRNC.txt")


@pytest.fixture(scope="module")
def fname_expected_output(datadir):

    def wrapped_fname_expected_output(driver):
        return os.path.join(datadir, f"ePhotoOutput_{driver}.txt")

    return wrapped_fname_expected_output


@pytest.fixture(scope="module")
def driver_vars(fname_InputEvn, fname_InputATPCost,
                fname_InputEnzyme, fname_InputGRNC):

    def wrapped_driver_vars(driver):
        x = Variables()
        x.readParam(fname_InputEvn)
        if driver == "EPS":
            x.readParam(fname_InputATPCost)
            x.readEnzymeAct(fname_InputEnzyme)
            x.readGRN(fname_InputGRNC)
            x.RUBISCOMETHOD = 2
            x.debuglevel = 0
        ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)
        return x

    return wrapped_driver_vars


@pytest.fixture(scope="module")
def driver_args(driver_vars):

    def wrapped_driver_args(driver, theVars=None):
        if theVars is None:
            theVars = driver_vars(driver)
        startTime = 0.0
        stepSize = 1.0
        endTime = 5000.0
        maxSubsteps = 750
        atol = 1e-5
        rtol = 1e-4
        param = 1
        ratio = 1.0
        Tp = theVars.Tp
        showWarn = False
        if driver == "EPS":
            return (theVars, startTime, stepSize, endTime, maxSubsteps,
                    atol, rtol, param, ratio, Tp, showWarn)
        elif driver == "CM":
            return (theVars, startTime, stepSize, endTime, maxSubsteps,
                    atol, rtol, showWarn)
        else:
            return (theVars, startTime, stepSize, endTime, maxSubsteps,
                    atol, rtol, param, ratio, showWarn)

    return wrapped_driver_args


@pytest.fixture(scope="module")
def driver_class():

    def wrapped_driver_class(driver):
        return getattr(ePhotosynthesis.drivers, f"{driver}Driver")

    return wrapped_driver_class


def test_Variables():
    x = Variables()
    assert not x.useC3


def test_Modules():
    assert hasattr(ePhotosynthesis, 'modules')
    ePhotosynthesis.modules.CM.setTestSucPath(1)
    ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)


@pytest.mark.parametrize("driver", ["trDynaPS", "DynaPS", "CM", "EPS"])
def test_drivers(driver, driver_class, driver_args, fname_expected_output):
    assert hasattr(ePhotosynthesis, 'drivers')
    cls = driver_class(driver)
    args = driver_args(driver)
    expectedFile = fname_expected_output(driver)
    kws = {'skiprows': 1, 'delimiter': ','}
    if driver == "EPS":
        kws['skiprows'] = 0
    expected = np.loadtxt(expectedFile, **kws)
    x = cls(*args)
    result = list(x.run())
    if driver == "EPS":
        result = [result[0]]
    else:
        result.insert(0, args[0].TestLi)
    actual = np.array(result)
    np.testing.assert_allclose(actual, expected,
                               rtol=x.reltol, atol=x.abstol)
    del args
    del driver


def test_EPS_error(driver_vars, driver_args):
    args = driver_args("EPS", theVars=driver_vars("CM"))
    with pytest.raises(RuntimeError):
        ePhotosynthesis.drivers.EPSDriver(*args)
