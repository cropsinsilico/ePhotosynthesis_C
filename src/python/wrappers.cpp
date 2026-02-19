#ifdef BUILD_PYTHON
#include "python/wrappers.hpp"
#include "Variables.hpp"
#include "modules/ModuleAll.hpp"
#include "drivers/drivers.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "macros.hpp"
#include "ePhotosynthesis.hpp"
using namespace ePhotosynthesis;

namespace bp = boost::python;

#ifndef PYTHON_LIBRARY_NAME
#define PYTHON_LIBRARY_NAME pyPhotosynthesis
#endif

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Variables_setVar_overloads, setVar, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Variables_getVar_overloads, getVar, 1, 2)

BOOST_PYTHON_MODULE(PYTHON_LIBRARY_NAME) {
    // bp::object package = bp::scope();
    // package.attr("__path__") = STRINGIZE(PYTHON_LIBRARY_NAME);
    void (Variables::*readParam1)(const std::string&) = &Variables::readParam;
    void (Variables::*readParam2)(const std::string&, std::map<std::string, std::string>&) = &Variables::readParam;

    bp::class_<Variables>("Variables")
        .def("readParam", readParam1)
        .def("readParam", readParam2)
        .def("readEnzymeAct", &Variables::readEnzymeAct)
        .def("readGRN", &Variables::readGRN)
        .def("setVar", static_cast<void(Variables::*)(const std::string&, const double&,const bool&)>(&Variables::setVar), Variables_setVar_overloads())
        .def("getVar", static_cast<double(Variables::*)(const std::string&,const bool&)const>(&Variables::getVar), Variables_getVar_overloads())
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
#define ADD_MEMBER_READWRITE(name)              \
      .def_readwrite(#name, &Variables::name)
      FOR_EACH_GENERIC(ADD_MEMBER_READWRITE, CALL_WITH_EMPTY_ARGS,
                       SEP_EMPTY, (), EXPAND(MEMBERS_ALLVARS))
#undef ADD_MEMBER_READWRITE
        .def_readwrite("PR_Param", &Variables::PR_Param)
        .def_readwrite("BF_Param", &Variables::BF_Param)
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
        .def_readwrite("RuACTRatio", &Variables::RuACTRatio)
        .def_readwrite("SUCSRatio", &Variables::SUCSRatio)
        .def_readwrite("XanCycleRatio", &Variables::XanCycleRatio)
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
        .def_readwrite("useC3", &Variables::useC3);
    // bp::dict dummy;
    bp::def("run_simulation",
            python::run_simulation,
            (bp::arg("driverName"),
             bp::arg("begintime")=0.0,
             bp::arg("stoptime")=5000.0,
             bp::arg("stepsize")=1.0,
             bp::arg("maxSubSteps")=750,
             bp::arg("abstol")=1e-5,
             bp::arg("reltol")=1e-4,
             // bp::arg("inputs")=dummy,
             bp::arg("evnFile")="",
             bp::arg("atpcostFile")="",
             bp::arg("enzymeFile")="",
             bp::arg("grnFile")="",
             bp::arg("iterationsFile")="",
             bp::arg("outputFile")="",
             bp::arg("outputParam")=0,
             bp::arg("outputParamBase")=""),
            "Run a photosynthesis simulation");
    python::exportModules();
    python::exportDrivers();
}

void python::exportModules() {
    bp::object modModule(bp::handle<>(bp::borrowed(PyImport_AddModule(STRINGIZE(PYTHON_LIBRARY_NAME) ".modules"))));
    bp::scope().attr("modules") = modModule;
    bp::scope modScope = modModule;
    
#define ADD_SET_GET_MOD(mod, name)                      \
    .def("set" #name, &modules::mod::set ## name)       \
      .staticmethod("set" #name)                        \
      .def("get" #name, &modules::mod::get ## name)     \
      .staticmethod("get" #name)
#define ADD_MODULE_WRAPPERS(mod)                                        \
    bp::class_<modules::mod>(#mod, bp::no_init)                         \
      DEFER(FOR_EACH_GENERIC_DEFERED)()(PACK_MACRO(ADD_SET_GET_MOD),    \
                                        CALL_WITH_PREFIX_ARGS,          \
                                        SEP_EMPTY, (mod),               \
                                        EXPAND(MEMBER_NAMES_FOR_MODULE(mod)));

    EVAL(FOR_EACH(ADD_MODULE_WRAPPERS, MEMBER_NAMES_MODULE));
    
#undef ADD_MODULE_WRAPPERS
#undef ADD_SET_GET_MOD
    
}

template<class T>
std::shared_ptr<T>
python::Driver_Init(PYTHON_DRIVER_ARG_DEFS) {
  std::vector<std::string> outVars_vect;
  if (!outVars.is_none()) {
    for (int i = 0; i < len(outVars); i++) {
      outVars_vect.push_back(bp::extract<std::string>(outVars[i]));
    }
  }
  return std::shared_ptr<T>(new T(theVars, startTime, stepSize, endTime,
                                  maxSubsteps, atol, rtol, para, ratio,
                                  showWarn, outVars_vect));
}
template<typename T>
bp::object python::Driver_Run(T& drv) {
  std::vector<double> out_vect = drv.run();
  bp::object get_iter = bp::iterator<std::vector<double> >();
  bp::object iter = get_iter(out_vect);
  bp::list out(iter);
  return out;
}

bp::object python::run_simulation(const std::string& driverName,
                                  const double& begintime,
                                  const double& stoptime,
                                  const double& stepsize,
                                  const int& maxSubSteps,
                                  const double& abstol,
                                  const double& reltol,
                                  // bp::object inputs,
                                  const std::string& evnFile,
                                  const std::string& atpcostFile,
                                  const std::string& enzymeFile,
                                  const std::string& grnFile,
                                  const std::string& iterationsFile,
                                  const std::string& outputFile,
                                  const int& outputParam,
                                  const std::string& outputParamBase) {
  DriverType driverChoice = utils::enum_string2key<DriverType>(driverName);
  std::map<std::string, std::string> inputs_map;
  bool useC3 = false;
  std::vector<std::string> outputVars;
  std::vector<std::string> outputParamVars;
  ushort dbglvl = 0;
  bool debugDelta = false;
  bool debugInternal = false;
  bool record = false;
  bool iterationsPreserveState = false;
  Variables *theVars = nullptr;
  bp::object inputs;
  if (inputs) {
    bp::extract< bp::dict > inputs_ext(inputs);
    if (inputs_ext.check()) {
      bp::dict inputs_dict = inputs_ext();
      bp::list inputs_keys = inputs_dict.keys();
      for (int i = 0; i < len(inputs_keys); ++i) {
        bp::object curInput = inputs[inputs_keys[i]];
        if (curInput) {
          inputs_map[bp::extract<std::string>(inputs_keys[i])] = bp::extract<double>(curInput);
        }
      }
    }
  }
  std::map<std::string, double> out_map = run_simulation(
         driverChoice,
         begintime, stoptime, stepsize, maxSubSteps, abstol, reltol,
         inputs_map, useC3,
         evnFile, atpcostFile, enzymeFile, grnFile, iterationsFile,
         outputFile, outputVars,
         outputParam, outputParamBase, outputParamVars,
         dbglvl, debugDelta, debugInternal, record,
         iterationsPreserveState, theVars);
  bp::dict out;
  for (typename std::map<std::string, double>::const_iterator it = out_map.begin();
       it != out_map.end(); it++) {
    out[it->first] = it->second;
  }
  return out;
}

#define ADD_DRIVER(mod)                                                 \
  std::shared_ptr<drivers::mod ## Driver>                               \
  python::mod ## Driver_Init(PYTHON_DRIVER_ARG_DEFS) {                  \
    return Driver_Init<drivers::mod ## Driver>(PYTHON_DRIVER_ARGS);     \
  }                                                                     \
  bp::object python::mod ## Driver_Run(drivers::mod ## Driver& drv) {   \
    return Driver_Run(drv);                                             \
  }
FOR_EACH(ADD_DRIVER, EXPAND(MEMBERS_DRIVER))
#undef ADD_DRIVER

void python::exportDrivers() {
    void (*selectDriver)(const std::string&) = &drivers::select_driver;
    void (*deselectDriver)(const std::string&) = &drivers::deselect_driver;
    
    bp::object driverModule(bp::handle<>(bp::borrowed(PyImport_AddModule(STRINGIZE(PYTHON_LIBRARY_NAME) ".drivers"))));
    bp::scope().attr("drivers") = driverModule;
    bp::scope driverScope = driverModule;
    bp::def("selectDriver", selectDriver, (bp::arg("name")=""));
    bp::def("deselectDriver", deselectDriver), (bp::arg("name")="");
    bp::class_<std::vector<double> >("stl_vector_double")
            .def(bp::vector_indexing_suite<std::vector<double> >());
    
#define ADD_DRIVER_METHODS(mod)                                         \
    .def("run", &python::mod ## Driver_Run)                             \
        .def("setup", &drivers::mod ## Driver::setup)                   \
        .def("getResults", &drivers::mod ## Driver::getResults)         \
        .def_readwrite("abstol", &drivers::mod ## Driver::abstol)       \
        .def_readwrite("reltol", &drivers::mod ## Driver::reltol)       \
        .def_readwrite("start", &drivers::mod ## Driver::start)         \
        .def_readwrite("step", &drivers::mod ## Driver::step)           \
        .def_readwrite("endtime", &drivers::mod ## Driver::endtime)     \
        .def_readwrite("initialStep", &drivers::mod ## Driver::initialStep) \
        .def_readwrite("time", &drivers::mod ## Driver::time)           \
        .def_readwrite("maxStep", &drivers::mod ## Driver::maxStep)     \
        .def_readwrite("results", &drivers::mod ## Driver::results)
#define ADD_DRIVER(mod)                                                 \
    bp::class_<drivers::mod ## Driver,                                  \
               std::shared_ptr<drivers::mod ## Driver>,                 \
               boost::noncopyable>(#mod "Driver",                       \
                                   bp::no_init)                         \
      .def("__init__", bp::make_constructor(& mod ## Driver_Init,       \
      bp::default_call_policies(), (bp::arg("theVars"),                 \
      bp::arg("startTime")=0.0,                                         \
      bp::arg("stepSize")=1.0,                                          \
      bp::arg("endTime")=5000.0,                                        \
      bp::arg("maxSubsteps")=750,                                       \
      bp::arg("abstol")=1.0e-5,                                         \
      bp::arg("reltol")=1.0e-4,                                         \
      bp::arg("param")=1,                                               \
      bp::arg("ratio")=1.0,                                             \
      bp::arg("showWarn")=false,                                        \
      bp::arg("outVars")=bp::object())))                                \
      ADD_DRIVER_METHODS(mod)
    FOR_EACH(ADD_DRIVER, EXPAND(MEMBERS_DRIVER));
#undef ADD_DRIVER
#undef ADD_DRIVER_METHODS
}

#endif
