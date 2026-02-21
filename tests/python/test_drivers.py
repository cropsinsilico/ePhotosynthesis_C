import os
import pytest
import numpy as np
import ePhotosynthesis
from ePhotosynthesis import Variables


@pytest.fixture(scope="module")
def datadir():
    r"""str: Directory containing expected test results."""
    testsdir = os.path.dirname(os.path.dirname(__file__))
    return os.path.join(testsdir, 'data')


@pytest.fixture(scope="module")
def fname_InputEnzyme(datadir):
    r"""str: Input file containing enzyme activity parameters."""
    return os.path.join(datadir, "InputEnzyme.txt")


@pytest.fixture(scope="module")
def fname_InputEvn(datadir):
    r"""str: Input file containing parameter values."""
    return os.path.join(datadir, "InputEvn.txt")


@pytest.fixture(scope="module")
def fname_InputATPCost(datadir):
    r"""str: Input file containing ATPCost value."""
    return os.path.join(datadir, "InputATPCost.txt")


@pytest.fixture(scope="module")
def fname_InputGRNC(datadir):
    r"""str: Input file containing gene expression levels"""
    return os.path.join(datadir, "InputGRNC.txt")


@pytest.fixture(scope="module")
def fname_OutputFile():
    r"""str: Name of the output file that should be created."""
    return os.path.normpath(os.path.join(os.getcwd(), "Output.txt"))


@pytest.fixture(scope="module")
def fname_expected_output(datadir):
    r"""Function to generate the name of the file containing the expected
    output for a driver test.

    Args:
        driver (str): Name of the driver to get the expected output for.

    Returns:
        str: Path to the file containing the expected result.

    """

    def wrapped_fname_expected_output(driver):
        return os.path.join(datadir, f"ePhotoOutput_{driver}.txt")

    return wrapped_fname_expected_output


@pytest.fixture
def driver_kwargs():
    r"""Driver keyword arguments."""
    return {
        'abstol': 1e-5,
        'reltol': 1e-4,
        # 'startTime': 0.0,
        # 'stepSize': 1.0,
        # 'endTime': 5000.0,
        # 'maxSubsteps': 750,
        # The following are only valid for driver
        # 'param': 1,
        # 'ratio': 1.0,
        # 'showWarn': False,
    }


@pytest.fixture
def simulation_kwargs(driver_kwargs, fname_InputEvn, fname_InputATPCost,
                      fname_InputEnzyme, fname_InputGRNC,
                      fname_OutputFile):
    r"""Keyword arguments for run_simulation."""
    return dict(
        driver_kwargs, **{
            'evnFile': fname_InputEvn,
            'atpcostFile': fname_InputATPCost,
            'enzymeFile': fname_InputEnzyme,
            'grnFile': fname_InputGRNC,
            # 'iterationsFile': fname_IterationsFile,
            'outputFile': fname_OutputFile,
        })


@pytest.fixture(scope="module")
def check_result(fname_expected_output):
    r"""Function to check that a simulation result matches the
    expectated result.

    Args:
        driver (str): Name of the driver being tested.
        result (np.ndarray, str): Result array or a file containing the
            simulation result.

    """

    def _check_result(driver, result, variables=None):
        abstol = 1e-5
        reltol = 1e-4
        expectedFile = fname_expected_output(driver)
        kws = {'skiprows': 1, 'delimiter': ','}
        if driver == "EPS":
            kws['skiprows'] = 0
        expected = np.loadtxt(expectedFile, **kws)
        if isinstance(result, str):
            assert os.path.isfile(result)
            actual = np.loadtxt(result, **kws)
        else:
            if driver == "EPS":
                result = [result[0]]
            else:
                assert variables is not None
                result.insert(0, variables.TestLi)
            actual = np.array(result)
        np.testing.assert_allclose(actual, expected,
                                   rtol=reltol, atol=abstol)

    return _check_result


@pytest.fixture(scope="module")
def driver_vars(fname_InputEvn, fname_InputATPCost,
                fname_InputEnzyme, fname_InputGRNC):
    r"""Function to generate the input variables for a driver.

    Args:
        driver (str): Name of the driver to generate input variables for.

    Returns:
        Variables: Input variables.

    """

    def wrapped_driver_vars(driver):
        x = Variables()
        ePhotosynthesis.drivers.selectDriver(driver)
        x.readParam(fname_InputEvn)
        x.readParam(fname_InputATPCost)
        if driver == "EPS":
            x.debuglevel = 0
            x.readEnzymeAct(fname_InputEnzyme)
            x.readGRN(fname_InputGRNC)
        # ePhotosynthesis.modules.PR.setRUBISCOMETHOD(2)
        # ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)
        return x

    return wrapped_driver_vars


def test_Variables():
    r"""Tests of the Variable class."""
    x = Variables()
    assert not x.useC3


def test_Modules():
    r"""Tests of methods for setting/getting module level parameters."""
    assert hasattr(ePhotosynthesis, 'modules')
    prev = ePhotosynthesis.modules.CM.getTestSucPath()
    ePhotosynthesis.modules.CM.setTestSucPath(1)
    ePhotosynthesis.modules.CM.setTestSucPath(prev)
    assert ePhotosynthesis.modules.CM.getTestSucPath() == prev
    prev = ePhotosynthesis.modules.PR.getRUBISCOTOTAL()
    ePhotosynthesis.modules.PR.setRUBISCOTOTAL(3)
    ePhotosynthesis.modules.PR.setRUBISCOTOTAL(prev)
    assert ePhotosynthesis.modules.PR.getRUBISCOTOTAL() == prev


@pytest.mark.parametrize("driver", ["trDynaPS", "DynaPS", "CM", "EPS"])
def test_run_simulation(driver, simulation_kwargs, check_result,
                        fname_OutputFile):
    r"""Test the run_simulation function for the various drivers."""
    result = ePhotosynthesis.run_simulation(
        driver, **simulation_kwargs
    )
    check_result(driver, fname_OutputFile)
    # TODO: Check result
    import pprint
    pprint.pprint(result)


# @pytest.mark.parametrize("driver", ["trDynaPS", "DynaPS", "CM", "EPS"])
# def test_drivers(driver, driver_vars, driver_kwargs, check_result):
#     r"""Test running simulation directly from the driver."""
#     assert hasattr(ePhotosynthesis, 'drivers')
#     ePhotosynthesis.drivers.selectDriver(driver)
#     try:
#         cls = getattr(ePhotosynthesis.drivers, f"{driver}Driver")
#         variables = driver_vars(driver)
#         x = cls(variables, **driver_kwargs)
#         result = x.run()
#         check_result(driver, result, variables=variables)
#         del variables
#         del driver
#     finally:
#         ePhotosynthesis.drivers.selectDriver()


def test_EPS_error(driver_vars, driver_kwargs):
    r"""Test error raised when there is missing data."""
    variables = driver_vars("CM")
    with pytest.raises(RuntimeError):
        ePhotosynthesis.drivers.EPSDriver(variables, **driver_kwargs)
