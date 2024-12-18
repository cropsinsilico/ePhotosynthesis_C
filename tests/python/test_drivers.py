import os
import pytest
import numpy as np
import ePhotosynthesis
from ePhotosynthesis import Variables


def test_Variables():
    x = Variables()
    assert not x.useC3


def test_Modules():
    assert hasattr(ePhotosynthesis, 'modules')
    ePhotosynthesis.modules.CM.setTestSucPath(1)
    ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)


def test_drivers():
    assert hasattr(ePhotosynthesis, 'drivers')


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


@pytest.fixture
def default_vars():
    x = Variables()
    x.RUBISCOMETHOD = 2
    return x


@pytest.fixture
def C3_vars(fname_InputEvn, fname_InputATPCost,
            fname_InputEnzyme, fname_InputGRNC):
    x = Variables()
    x.useC3 = True
    x.readParam(fname_InputEvn)
    x.readParam(fname_InputATPCost)
    x.readEnzymeAct(fname_InputEnzyme)
    x.readGRN(fname_InputGRNC)
    x.RUBISCOMETHOD = 2
    x.debuglevel = 0
    ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)
    return x


def test_EPS_error(default_vars):
    startTime = 0.0
    stepSize = 1.0
    endTime = 5000.0
    maxSubsteps = 750
    atol = 1e-5
    rtol = 1e-4
    param = 1
    ratio = 1.0
    Tp = 25.0
    showWarn = False
    with pytest.raises(RuntimeError):
        ePhotosynthesis.drivers.EPSDriver(
            default_vars, startTime, stepSize, endTime, maxSubsteps,
            atol, rtol, param, ratio, Tp, showWarn)


def test_EPS(C3_vars, fname_expected_output):
    startTime = 0.0
    stepSize = 1.0
    endTime = 5000.0
    maxSubsteps = 750
    atol = 1e-5
    rtol = 1e-4
    param = 1
    ratio = 1.0
    Tp = C3_vars.Tp
    showWarn = False
    # TODO: Actually read the file?
    # expectedFile = fname_expected_output("EPS")
    expected = np.array([30.3325, 33.50430665237476, 3.1717867365048513])
    x = ePhotosynthesis.drivers.EPSDriver(
        C3_vars, startTime, stepSize, endTime, maxSubsteps,
        atol, rtol, param, ratio, Tp, showWarn)
    result = list(x.run())
    actual = np.array(result)
    np.testing.assert_allclose(actual, expected, rtol=rtol, atol=atol)
