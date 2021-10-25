#ifdef BUILD_PYTHON
#include "python/wrappers.hpp"
#include "Variables.hpp"
#include "modules/CM.hpp"
#include "modules/PR.hpp"
#include "modules/Enzyme.hpp"
#include "drivers/drivers.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace ePhotosynthesis;

namespace bp = boost::python;

BOOST_PYTHON_MODULE(pyPhotosynthesis) {
    bp::object package = bp::scope();
    package.attr("__path__") = "pyPhotosynthesis";

    bp::class_<Variables>("Variables")
            .def_readwrite("record", &Variables::record)
            .def_readwrite("BF_FI_com", &Variables::BF_FI_com)
            .def_readwrite("EPS_SUCS_com", &Variables::EPS_SUCS_com)
            .def_readwrite("FIBF_PSPR_com", &Variables::FIBF_PSPR_com)
            .def_readwrite("PR_PS_com", &Variables::PR_PS_com)
            .def_readwrite("PSPR_SUCS_com", &Variables::PSPR_SUCS_com)
            .def_readwrite("RROEA_EPS_com", &Variables::RROEA_EPS_com)
            .def_readwrite("RedoxReg_RA_com", &Variables::RedoxReg_RA_com)
            .def_readwrite("RuACT_EPS_com", &Variables::RuACT_EPS_com)
            .def_readwrite("XanCycle_BF_com", &Variables::XanCycle_BF_com)
            .def_readwrite("Para_mata", &Variables::Para_mata)
            .def_readwrite("GsResponse", &Variables::GsResponse)
            .def_readwrite("kdcon", &Variables::kdcon)
            .def_readwrite("EAPPDK", &Variables::EAPPDK)
            .def_readwrite("GP", &Variables::GP)
            .def_readwrite("GRNC", &Variables::GRNC)
            .def_readwrite("GRNT", &Variables::GRNT)
            .def_readwrite("RUBISCOMETHOD", &Variables::RUBISCOMETHOD)
            .def_readonly("AVR", &Variables::AVR)
            .def_readonly("HPR", &Variables::HPR)
            .def_readonly("O2", &Variables::O2)
            .def_readwrite("CO2_cond", &Variables::CO2_cond)
            .def_readwrite("GLight", &Variables::GLight)
            .def_readwrite("O2_cond", &Variables::O2_cond)
            .def_readwrite("PS12ratio", &Variables::PS12ratio)
            .def_readwrite("ADP", &Variables::ADP)
            .def_readwrite("Pi", &Variables::Pi)
            .def_readwrite("TestATPCost", &Variables::TestATPCost)
            .def_readwrite("CO2_in", &Variables::CO2_in)
            .def_readwrite("TestLi", &Variables::TestLi)
            .def_readwrite("PS2BF_Pi", &Variables::PS2BF_Pi)
            .def_readwrite("PS_PR_Param", &Variables::PS_PR_Param)
            .def_readwrite("Tp", &Variables::Tp)
            .def_readwrite("Radiation_PARo", &Variables::Radiation_PARo)
            .def_readwrite("Radiation_NIR", &Variables::Radiation_NIR)
            .def_readwrite("Radiation_LW", &Variables::Radiation_LW)
            .def_readwrite("alfa", &Variables::alfa)
            .def_readwrite("CI", &Variables::CI)
            .def_readwrite("Wind", &Variables::Wind)
            .def_readwrite("fc", &Variables::fc)
            .def_readwrite("lightParam", &Variables::lightParam)
            .def_readwrite("BallBerryInterceptC4", &Variables::BallBerryInterceptC4)
            .def_readwrite("BallBerrySlopeC4", &Variables::BallBerrySlopeC4)
            .def_readwrite("WeatherRH", &Variables::WeatherRH)
            .def_readwrite("kd", &Variables::kd)
            .def_readwrite("ki", &Variables::ki)
            .def_readwrite("taoRub", &Variables::taoRub)
            .def_readwrite("Vpmax", &Variables::Vpmax)
            .def_readwrite("Vcmax", &Variables::Vcmax)
            .def_readwrite("FactorVP", &Variables::FactorVP)
            .def_readwrite("FactorVC", &Variables::FactorVC)
            .def_readwrite("vrpd", &Variables::vrpd)
            .def_readwrite("MRd", &Variables::MRd)
            .def_readwrite("PPDKRP", &Variables::PPDKRP)
            .def_readonly("alpha1", &Variables::alpha1)
            .def_readonly("alpha2", &Variables::alpha2)
            .def_readwrite("PR_Param", &Variables::PR_Param)
            .def_readwrite("BF_Param", &Variables::BF_Param)
            .def_readwrite("FI_PARAM", &Variables::FI_PARAM)
            .def_readwrite("FI_Param", &Variables::FI_Param)
            .def_readwrite("RROEA_Param", &Variables::RROEA_Param)
            .def_readwrite("RuACT_Param", &Variables::RuACT_Param)
            .def_readwrite("SUCS_Param", &Variables::SUCS_Param)
            .def_readwrite("XanCycle_Param", &Variables::XanCycle_Param)
            .def_readwrite("BF_Vel", &Variables::BF_Vel)
            .def_readwrite("FI_Vel", &Variables::FI_Vel)
            .def_readwrite("PR_Vel", &Variables::PR_Vel)
            .def_readwrite("PS_Vel", &Variables::PS_Vel)
            .def_readwrite("RROEA_Vel", &Variables::RROEA_Vel)
            .def_readwrite("RedoxReg_Vel", &Variables::RedoxReg_Vel)
            .def_readwrite("RuACT_Vel", &Variables::RuACT_Vel)
            .def_readwrite("SUCS_Vel", &Variables::SUCS_Vel)
            .def_readwrite("XanCycle_Vel", &Variables::XanCycle_Vel)
            .def_readwrite("BFRatio", &Variables::BFRatio)
            .def_readwrite("FIRatio", &Variables::FIRatio)
            .def_readwrite("PRRatio", &Variables::PRRatio)
            .def_readwrite("PSRatio", &Variables::PSRatio)
            .def_readwrite("RacRatio", &Variables::RacRatio)
            .def_readwrite("SUCRatio", &Variables::SUCRatio)
            .def_readwrite("XanRatio", &Variables::XanRatio)
            .def_readwrite("EnzymeAct", &Variables::EnzymeAct)
            .def_readwrite("VfactorCp", &Variables::VfactorCp)
            .def_readwrite("VfactorT", &Variables::VfactorT)
            .def_readwrite("BF_Pool", &Variables::BF_Pool)
            .def_readwrite("FIBF_Pool", &Variables::FIBF_Pool)
            .def_readwrite("FI_Pool", &Variables::FI_Pool)
            .def_readwrite("RROEA_Pool", &Variables::RROEA_Pool)
            .def_readwrite("RuACT_Pool", &Variables::RuACT_Pool)
            .def_readwrite("SUCS_Pool", &Variables::SUCS_Pool)
            .def_readwrite("BF_RC", &Variables::BF_RC)
            .def_readwrite("FI_RC", &Variables::FI_RC)
            .def_readwrite("RROEA_RC", &Variables::RROEA_RC)
            .def_readwrite("RuACT_RC", &Variables::RuACT_RC)
            .def_readwrite("BF2OUT", &Variables::BF2OUT)
            .def_readwrite("PR2OUT", &Variables::PR2OUT)
            .def_readwrite("PS2OUT", &Variables::PS2OUT)
            .def_readwrite("SUCS2OUT", &Variables::SUCS2OUT)
            .def_readwrite("XanCycle2OUT", &Variables::XanCycle2OUT)
            .def_readwrite("FluxTR", &Variables::FluxTR)
            .def_readwrite("RROEA_KE", &Variables::RROEA_KE)
            .def_readwrite("BF_VEL", &Variables::BF_VEL)
            .def_readwrite("CO2A", &Variables::CO2A)
            .def_readwrite("FI_VEL", &Variables::FI_VEL)
            .def_readwrite("PR_VEL", &Variables::PR_VEL)
            .def_readwrite("PS_VEL", &Variables::PS_VEL)
            .def_readwrite("RROEA_VEL", &Variables::RROEA_VEL)
            .def_readwrite("RedoxReg_MP", &Variables::RedoxReg_MP)
            .def_readwrite("RedoxReg_VEL", &Variables::RedoxReg_VEL)
            .def_readwrite("RuACT_VEL", &Variables::RuACT_VEL)
            .def_readwrite("SUCS_VEL", &Variables::SUCS_VEL)
            .def_readwrite("XanCycle_VEL", &Variables::XanCycle_VEL)
            .def_readwrite("Leaf_Vel", &Variables::Leaf_Vel)
            .def_readwrite("AE_Vel", &Variables::AE_Vel)
            .def_readwrite("Enzyme_Vel", &Variables::Enzyme_Vel)
            .def_readwrite("TempFactor", &Variables::TempFactor)
            .def_readwrite("useC3", &Variables::useC3);
    python::exportModules();
    python::exportDrivers();
}

void python::exportModules() {
    bp::object modModule(bp::handle<>(bp::borrowed(PyImport_AddModule("pyPhotosynthesis.modules"))));
    bp::scope().attr("modules") = modModule;
    bp::scope modScope = modModule;
    bp::class_<modules::CM>("CM", bp::no_init)
            .def("setTestSucPath", &modules::CM::setTestSucPath)
            .staticmethod("setTestSucPath");
    bp::class_<modules::PR>("PR", bp::no_init)
            .def("setRUBISCOTOTAL", &modules::PR::setRUBISCOTOTAL)
            .staticmethod("setRUBISCOTOTAL");
    bp::class_<modules::Enzyme>("Enzyme", bp::no_init)
            .def("setpathway_option", & modules::Enzyme::setpathway_option)
            .staticmethod("setpathway_option");
}

void python::exportDrivers() {
    bp::object driverModule(bp::handle<>(bp::borrowed(PyImport_AddModule("pyPhotosynthesis.drivers"))));
    bp::scope().attr("drivers") = driverModule;
    bp::scope driverScope = driverModule;
    bp::class_<std::vector<double> >("stl_vector_double")
            .def(bp::vector_indexing_suite<std::vector<double> >());
    bp::class_<DriverWrap, boost::noncopyable>("Driver", bp::no_init)
            .def("run", &drivers::Driver::run)
            .def("setup", bp::pure_virtual(&drivers::Driver::setup))
            .def("getResults", bp::pure_virtual(&drivers::Driver::getResults))
            .def("MB", bp::pure_virtual(&drivers::Driver::MB));
    bp::class_<drivers::EPSDriver, bp::bases<DriverWrap>, boost::noncopyable>("EPSDriver", bp::init<Variables*, double, double, double, int, double, double, std::size_t, double, double, bool>()[bp::with_custodian_and_ward_postcall<0,2>()])
            .def("run", &drivers::EPSDriver::run)
            .def("setup", &drivers::EPSDriver::setup)
            .def("getResults", &drivers::EPSDriver::getResults);
    bp::class_<drivers::trDynaPSDriver, bp::bases<DriverWrap>, boost::noncopyable>("trDynaPSDriver", bp::init<Variables*, double, double, double, int, double, double, std::size_t, double, bool>()[bp::with_custodian_and_ward_postcall<0,2>()])
            .def("run", &drivers::trDynaPSDriver::run)
            .def("setup", &drivers::trDynaPSDriver::setup)
            .def("getResults", &drivers::trDynaPSDriver::getResults);
    bp::class_<drivers::DynaPSDriver, bp::bases<DriverWrap>, boost::noncopyable>("DynaPSDriver", bp::init<Variables*, double, double, double, int, double, double, std::size_t, double, bool>()[bp::with_custodian_and_ward_postcall<0,2>()])
            .def("run", &drivers::DynaPSDriver::run)
            .def("setup", &drivers::DynaPSDriver::setup)
            .def("getResults", &drivers::DynaPSDriver::getResults);
    bp::class_<drivers::CMDriver, bp::bases<DriverWrap>, boost::noncopyable>("CMDriver", bp::init<Variables*, double, double, double, int, double, double, bool>()[bp::with_custodian_and_ward_postcall<0,2>()])
            .def("run", &drivers::CMDriver::run)
            .def("setup", &drivers::CMDriver::setup)
            .def("getResults", &drivers::CMDriver::getResults);
    bp::class_<drivers::RAC4leafMetaDriver, bp::bases<DriverWrap>, boost::noncopyable>("RAC4leafMetaDriver", bp::init<Variables*, double, double, double, int, double, double, bool>()[bp::with_custodian_and_ward_postcall<0,2>()])
            .def("run", &drivers::RAC4leafMetaDriver::run)
            .def("setup", &drivers::RAC4leafMetaDriver::setup)
            .def("getResults", &drivers::RAC4leafMetaDriver::getResults);
}

#endif
