#pragma once

/**********************************************************************************************************************************************
 *   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
 *
 * CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
 * University of Illinois at Urbana Champaign
 * Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.
 *
 * Converted from Matlab to C++ by Douglas N. Friedel, National Center for Supercomputing Applications (2020)
 *
 *   This file is part of e-photosynthesis.
 *
 *    e-photosynthesis is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation;
 *
 *    e-photosynthesis is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License (GPL)
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************************************************************/

#include <vector>

#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>
#include "globals.hpp"
#include "enums/enums_utils.hpp"

#ifndef realtype
#define realtype sunrealtype
#endif

#define MEMBERS_DRIVER trDynaPS, DynaPS, CM, EPS

namespace ePhotosynthesis {
  
enum DriverType : int {
  None, MEMBERS_DRIVER
};
  
template<>
inline const std::map<DriverType, std::string>& get_enum_names<DriverType>() {
  static const std::map<DriverType, std::string> collection = {
    {trDynaPS, "trDynaPS"},
    {DynaPS  , "DynaPS"  },
    {CM      , "CM"      },
    {EPS     , "EPS"     },
  };
  return collection;
}

class ValueSet_t;
class Variables;
  
namespace drivers {

class Driver;
class CVodeMem;

/**
  Data structure for passing parameters to the ODE solver
  */
struct UserData {
  std::vector<realtype> coeffs;
};

/**
  Create an instance of UserData

  \returns New instance of the UserData structure.
  */
inline UserData *alloc_user_data() {
  return new UserData();
}

/**
  Structure for passing a driver to the ODE routines
  */
struct CalcData {
    Driver *drv;
};

/**
  Create an instance of CalcData.

  \returns New instance of the CalcData structure.
  */
inline CalcData *alloc_calc_data() {
    return new CalcData();
}

/**
  Base class for all drivers. These classes handle taking the input data, feeding it to the ODE solver,
  iterating until the solver reaches its stopping time or fails, and returning the results.
  */
class Driver {
public:

    /**
      Generate the initial parameter Condition class for the solver to use.
      */
    virtual void setup() = 0;

    /**
      Run the ODE solver and return the results.

      \return `std::vector<double>` containg the results of the computations.
      \exception std::runtime_error General failure.
      */
    arr run();

    /**
       Dump out the current set of variables to a file.
       \param[in] filename Name of file where parameters should be written.
       \param[in] theVars Current copy of variables. If not provided,
         inputVars will be used.
       \param[in] con Current copy of conditions. If not provided,
         the result from currentCondition will be used.
       \param[in] is_init If true, this is just after initialization and
         velocities should not be included.
     */
    void dump(const std::string& filename,
	      const Variables* theVars = nullptr,
	      const ValueSet_t* con = nullptr,
	      const bool is_init = false);
  
    /**
      Runs the solver one more time on the intermediate results to get the solution at the end time
      stamp.
      */
    virtual void getResults() = 0;

    /**
       Gets the current conditions as a value set class based on the
         current conditions.
       \param[in] x Array of condition values that condition instance
         should be created from.
       \returns Conditions value set.
     */
    virtual ValueSet_t* currentConditions(realtype *x = nullptr) {
	UNUSED(x);
	return nullptr;
    }

    /**
      Wrapper for the underlying Module MB function. Takes the inputs and converts them to the
      appropriate Condition class.

      \param t The current timestamp.
      \param u A specialized data vector containgin the inputs which are converted to a Condition class.
      \returns dy/dt values for the step.
      */
    virtual arr MB(realtype t, N_Vector u) = 0;
    virtual ~Driver();
    static Variables *inputVars;  /**< the instance of Variables to use for all calculations. */
    arr constraints;   /**< serialized version of the Condition class being used. */
    std::string fname_vars_init; /**< Name of file where initial parameter values should be be output */
    std::string fname_vars_final; /**< Name of file where final parameter values should be be output */
    /**
       Turn on parameter output.
       \param[in] fname_init Name of the file that initial parameter
         values should be output to. If empty, values will not be output.
       \param[in] fname_final Name of the file that final parameter
         values should be output to. If empty, values will not be output.
     */
    void outputParam(const std::string& fname_init,
		     const std::string& fname_final);
    /**
       Turn on parameter output.
       \param[in] fname_base Base name that should be used for files that
         initial and final parameters should be output to. If not provided,
	 the name of the driver module will be used.
     */
    virtual void outputParam(const std::string& fname_base = "");

protected:
    friend CVodeMem;
    /**
      This sets up initial values for the class.

      \param[in, out] theVars Instance of the Variables class, which holds global scope variables and
             intermediate results.
      \param startTime The timestamp to start the solver at. Typically this is 0.
      \param stepSize The optimal step size for the solver to use.
      \param endTime The timestamp to end the calculations.
      \param maxSubsteps The maximum number of sub-iterations to make between steps.
      \param atol The absolute tolerance the ODE solver should use.
      \param rtol The relative tolerance the ODE solver should use.
      \param showWarn Whether or not to send solver warnings to stdout. If `true` then all
             warning messages are sent. If `false`, the default, warning messages are not sent,
             reducing clutter in production output.
      */
    Driver(Variables *theVars, const double startTime, const double stepSize, const double endTime,
           const int maxSubsteps, const double atol, const double rtol,
           const bool showWarn = false);
    /**
      Does the computations and generates the results for each step or sub-step in the solver.
      A pointer to this function is passed to the ODE solver. The API of the function cannot change
      as it is dictated by the solver.

      \param t The current timestamp.
      \param u The inputs
      \param u_dot[out] Pointer to the resulting dy/dt values.
      \param user_data Any user supplied data.
      */
    static int calculate(realtype t, N_Vector u, N_Vector u_dot, void *user_data);

    SUNContext* context;
    realtype abstol;            // absolute tolerance
    realtype reltol;            // relative tolerance
    double start, step, endtime; // time stuff
    double initialStep;         // used to store the initial step size
    int maxSubSteps;
    realtype *intermediateRes;
    arr results;
    realtype time;
    CalcData* data;
    double maxStep;
    void *cvode_mem;
    static bool showWarnings;
protected:
    void _dump(realtype t, ValueSet_t* con);
private:
    Driver() {}
    Variables* origVars;
    bool _dumpStep;
};

/**
  Base class for all drivers with an associated module. These classes
  handle taking the input data, feeding it to the ODE solver,
  iterating until the solver reaches its stopping time or fails, and
  returning the results.
  \tparam T Inheriting driver class.
  \tparam M Module ID associated with the driver.
  */
template<class T, MODULE M>
class DriverBase : public Driver {
public:
    static const MODULE module; /** Module associated with the driver */

    /**
       Initialize the driver class and associated value sets.
     */
    static void init(const bool useC3 = false) {
	T::enableC3(useC3);
	T::select();
    }
    /**
       Select value sets associated with this driver.
       \param[in] x If true, select the driver and associated value sets,
         if false, deselect them.
     */
    static void select(const bool x = true) {
	throw std::runtime_error("Cannot select the base driver.");
    }
    /**
       Enable/disable C3 defaults for value sets associated with this
         driver.
       \param[in] x If true, values sets will use C3 defaults, otherwise
         non-C3 defaults will be used.
     */
    static void enableC3(const bool x = true) {
	throw std::runtime_error("Cannot call enableC3 on the base driver.");
    }

    /**
      Get a prefix for errors describing the class
      \return Prefix
    */
    static std::string error_prefix() {
      std::string out;
      out += get_enum_names<MODULE>().find(module)->second;
      out += ": ";
      return out;
    }
    
    // virtual ~DriverBase();
    /** \copydoc Driver::outputParam */
    void outputParam(const std::string& fname_base = "") override {
	std::string fname_base_cpy = fname_base;
	if (fname_base_cpy.empty()) {
	  fname_base_cpy = get_enum_names<MODULE>().find(module)->second;
	  fname_base_cpy += "_";
	}
      Driver::outputParam(fname_base_cpy);
    }
protected:
    /** \copydoc drivers::Driver::Driver */
    DriverBase(Variables *theVars,
	       const double startTime, const double stepSize,
	       const double endTime, const int maxSubsteps,
	       const double atol, const double rtol,
	       const bool showWarn = false) :
      Driver(theVars, startTime, stepSize, endTime,
	     maxSubsteps, atol, rtol, showWarn) {}

};
  
template<class T, MODULE M>
const MODULE DriverBase<T, M>::module = M;

#ifdef TESTING
#define FORWARD_DECLARE_DRIVER(name)		\
    namespace test {				\
      class name ## DriverTest;			\
    }
#define DECLARE_DRIVER_TESTING(name)		\
    friend class test::name ## DriverTest
#else
#define FORWARD_DECLARE_DRIVER(name)
#define DECLARE_DRIVER_TESTING(name)
#endif
#define DECLARE_DRIVER(name)						\
 public:								\
  ~name ## Driver() override;						\
  /** \copydoc drivers::DriverBase::select */				\
  static void select(const bool x = true);				\
  /** \copydoc drivers::DriverBase::enableC3 */				\
  static void enableC3(const bool x = true);				\
  /** \copydoc drivers::Driver::currentConditions */			\
  ValueSet_t* currentConditions(realtype *x = nullptr) override;	\
  /** Initialize the variables */					\
private:								\
  DECLARE_DRIVER_TESTING(name);						\
  /** \returns A condition object for input into calculations */	\
  VARS_CLASS_VAR(name, COND)* name ## _Ini();				\
  /** \copydoc drivers::Driver::MB */					\
  arr MB(realtype t, N_Vector u) override;
#define DEFINE_DRIVER(name)					\
  name ## Driver::~name ## Driver() {				\
      VARS_CLASS_CALL(reset, (), name, MOD);			\
  }								\
  void name ## Driver::select(const bool x) {			\
      return VARS_CLASS_CALL(select, (x), name, MOD);		\
  }								\
  void name ## Driver::enableC3(const bool x) {			\
      return VARS_CLASS_CALL(enableC3, (x), name, MOD);		\
  }								\
  ValueSet_t* name ## Driver::currentConditions(realtype *x) {		\
      if (x) {								\
	return new VARS_CLASS_VAR(name, COND)(x);			\
      }									\
      if (intermediateRes) {						\
	return new VARS_CLASS_VAR(name, COND)(intermediateRes);		\
      }									\
      return new VARS_CLASS_VAR(name, COND)(constraints);		\
  }									\
  VARS_CLASS_VAR(name, COND)* name ## Driver::name ## _Ini() {		\
      return VARS_CLASS_VAR(name, MOD)::init(inputVars);		\
  }									\
  arr name ## Driver::MB(realtype t, N_Vector u) {			\
      realtype *x = N_VGetArrayPointer(u);				\
      VARS_CLASS_VAR(name, COND)* name ## _con = new VARS_CLASS_VAR(name, COND)(x); \
      arr dxdt = VARS_CLASS_VAR(name, MOD)::MB(t, name ## _con, inputVars); \
      _dump(t, name ## _con);						\
      delete name ## _con;						\
      return dxdt;							\
  }

}  // namespace drivers
}  // namespace ePhotosynthesis
