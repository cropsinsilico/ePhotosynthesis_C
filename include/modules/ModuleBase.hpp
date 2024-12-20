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

#include "definitions.hpp"
#include "Variables.hpp"
#include "ValueSet.hpp"

namespace ePhotosynthesis {
namespace modules {

#define MODULES_MOD BF, FI, PR, PS, RROEA, RedoxReg, RuACT, SUCS, XanCycle
  

/**
  This template class provides a common set of static funtions for every Module subclass. This makes
  calling the underlying functions more straight forward.

  \tparam T The class type to work with
  \tparam U The condition class associated with this Module
  */
template<class T, class U, MODULE ID = MODULE_NONE>
class ModuleBase : public VALUE_SET_STATIC_PARENT(T, U, ID, PARAM_TYPE_MOD) {
public:
    DECLARE_VALUE_SET_STATIC_BASE(ModuleBase, VALUE_SET_STATIC_PARENT(T, U, ID, PARAM_TYPE_MOD))
    typedef T ModType;
    typedef U CondType;
  
    static bool compareState(const Variables *theVars1,
			     const Variables *theVars2,
			     const U* con1, const U* con2,
			     const std::map<int, double>& map1,
			     const std::map<int, double>& map2) {
	if ((*theVars1) != (*theVars2)) {
	    std::cerr << "theVars differs" << std::endl;
	    return false;
	}
	if ((*con1) != (*con2)) {
	    std::cerr << "con differs" << std::endl;
	    return false;
	}
	if (!BaseClass::compare_value_maps(map1, map2)) {
	    std::cerr << "maps differ" << std::endl;
	    return false;
	}
	return T::_compareState(theVars1, theVars2, con1, con2, map1, map2);
    }
    static std::string diffState(const Variables *theVars1,
				 const Variables *theVars2,
				 const U* con1, const U* con2,
				 const std::map<int, double>& map1,
				 const std::map<int, double>& map2) {
      std::string out;
      std::size_t padKeys = std::max(theVars1->max_field_width_all(),
				     theVars2->max_field_width_all());
      std::size_t padVals = std::max(theVars1->max_value_width_all(),
				     theVars2->max_value_width_all());
      padVals = std::max(padVals,
			 std::max(con1->max_value_width(),
				  con2->max_value_width()));
      padVals = std::max(padVals,
			 std::max(BaseClass::value_width_value_map(map1),
				  BaseClass::value_width_value_map(map2)));
      out += theVars1->diff(*theVars2, padKeys, padVals, true);
      out += con1->diff(*con2, padKeys, padVals, true);
      out += BaseClass::diff_value_maps(map1, map2,
					padKeys, padVals, true);
      return out;
    }

    static U* initOrig(Variables *theVars) {
	U* out = initAllValues(theVars, true);
	T::_initOrig(theVars, out);
	return out;
    }

    static U* initNew(Variables *theVars) {
        U* out = initAllValues(theVars);
        T::_initCalc(theVars, out);
        return out;
    }

    /**
      Common, public interface for the private init function.

      \param theVars The main Variables instance.
      \returns A pointer to the associated Condition class.
      */
    static U* init(Variables *theVars) {
#ifdef CHECK_NEW_MATCHES_ORIG
	bool init0 = initialized();
	std::map<int, double> map0 = getValueMap();
	std::map<int, double> preinit_map0 = getValueMap(true);
	Variables *theVars2 = theVars->deepcopy();
	if ((*theVars2) != (*theVars)) {
	  ERROR_VALUE_SET("Copied variables does not match\n",
			  theVars->diff(*theVars));
	}
	U* out = initOrig(theVars);
	std::map<int, double> map = getValueMap();
	ValueSetClass::resetValues(true);
	setValueMap(map0, init0); // TODO: Set initialized
	if (!init0)
	  setValueMap(preinit_map0, false, true);
	U* out2 = initNew(theVars2);
	std::map<int, double> map2 = getValueMap();
	if (!compareState(theVars, theVars2, out, out2, map, map2)) {
	  ERROR_VALUE_SET("Old and new methods do not match\n",
			  diffState(theVars, theVars2,
				    out, out2, map, map2));
	}
	delete theVars2;
	delete out2;
	setValueMap(map, true);
	return out;
#else // CHECK_NEW_MATCHES_ORIG
	return initNew(theVars);
#endif // CHECK_NEW_MATCHES_ORIG
    }

    /**
       Function to initialize the value set pointers and set values to
         defaults.

       \param theVars Pointer to the global variables
       \param noDefaults If true, the value pointers will be initialized,
          but the default values will not be assigned to those values.
       \return A Condition object with values set based on the input
    */
    static U* initAllValues(Variables *theVars,
			    const bool noDefaults=false) {
      U* out = T::_createCondition(theVars);
      theVars->initParam(ID, PARAM_TYPE_MOD, noDefaults, true);
      theVars->initParam(ID, PARAM_TYPE_COND, noDefaults, false,
			 static_cast<ValueSet_t*>(out));
      const std::vector<PARAM_TYPE>& param_types = get_parameter_types();
      for (typename std::vector<PARAM_TYPE>::const_iterator it = param_types.begin();
	   it != param_types.end(); it++) {
	if (*it == PARAM_TYPE_COND) continue;
	theVars->initParam(ID, *it, noDefaults);
      }
      return out;
    }
    
    /**
      Common, public interface for the private differential calculation function

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      \returns A vector of doubles containing the dy/dt values for each member of the input Condition.
      */
    static arr MB(const double t, const U* condition, Variables *theVars) {
#ifdef INCDEBUG
        DEBUG_MESSAGE(condition)
#endif
	arr out = T::_MB(t, condition, theVars);
	return out;
    }

    /**
      Common, public interface for the private differential calculation function

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      \returns A pointer to an associated Condition class containing the dy/dt values for each member of the input Condition.
      */
    static U* MB_con(const double t, const U* condition, Variables *theVars) {
#ifdef INCDEBUG
        DEBUG_MESSAGE(condition)
#endif
	U* out = T::_MB_con(t, condition, theVars);
	return out;
    }

    /** \copydoc ValueSetBase::select */
    static void _select(const bool x = true,
			const bool noChildren = false) {
	ParentClass::_select(x, noChildren);
	for (std::vector<ValueSetClassWrapper>::const_iterator it = added_classes.begin();
	     it != added_classes.end(); it++)
	  if (*it)
	    (*it)->select(x, noChildren);
    }
    /** \copydoc ValueSetBase::enableC3 */
    static void _enableC3(const bool x = true,
			  const bool noChildren = false) {
	ParentClass::_enableC3(x, noChildren);
	for (std::vector<ValueSetClassWrapper>::const_iterator it = added_classes.begin();
	     it != added_classes.end(); it++)
	  if (*it)
	    (*it)->enableC3(x, noChildren);
    }
    /** \copydoc ValueSetBase::reset */
    static void _reset(const bool noChildren = false) {
	ParentClass::_reset(noChildren);
	if (ParentClass::child_classes.size() > 0)
	  return;
	for (std::vector<ValueSetClassWrapper>::const_iterator it = added_classes.begin();
	     it != added_classes.end(); it++)
	  if (*it) {
	    (*it)->reset(noChildren);
	  }
    }

    /**
       Add an associated class to this classes set.
       \tparam C Associated class.
     */
    template<typename C>
    static void addClass() {
	added_classes.emplace_back(new ValueSetClassType<C>());
    }

    /**
       Initialize the set of added classes.

       Common, public interface for the private _initAddedClasses function.
     */
    static void initAddedClasses() {
	if (!added_classes.empty()) return;
	T::_initAddedClasses();
    }

    /** Wrappers for value set classes associated with this module */
    static std::vector<ValueSetClassWrapper> added_classes;
    
 protected:
    ModuleBase() {}

    /**
      Common interface for the private rate calculation function.

      \param t The current timestamp.
      \param condition The input Condition class.
      \param theVars The main Variables instance.
      */
    static void Rate(const double t, const U* condition, Variables *theVars) {
        T::_Rate(t, condition, theVars);
    }

 private:
    /**
      Function to set the initial state of the Condition class.

      \param theVars Pointer to the global variables
      \return A Condition object with values set based on the input
      */
    static U* _init(Variables *theVars) {
	UNUSED(theVars);
	return nullptr;
    }

    static bool _compareState(const Variables *theVars1,
			      const Variables *theVars2,
			      const U* con1, const U* con2,
			      const std::map<int, double>& map1,
			      const std::map<int, double>& map2) {
      UNUSED(theVars1);
      UNUSED(theVars2);
      UNUSED(con1);
      UNUSED(con2);
      UNUSED(map1);
      UNUSED(map2);
      return true;
    }

    /**
       Function to create a condition for this module.

       \param theVars Pointer to the global variables
       \return A Condition object with values set based on the input
    */
    static U* _createCondition(Variables *theVars) {
      UNUSED(theVars);
      return nullptr;
    }

    /**
      Function to set the initial state of the Condition class.
      This method initializes the state using the original code.

      \param theVars Pointer to the global variables
      \return A Condition object with values set based on the input
      */
    static U* _initOrig(Variables *theVars) {
	UNUSED(theVars);
	return nullptr;
    }

    /**
      Function to calculate the dy/dt values for the Condition at the
        given time stamp.

      \param t The current timestamp
      \param con Condition object giving the input parameters
      \param theVars The global variables
      \return A vector containing the dy/dt values for this time stamp.
      */
    static arr _MB(const double t, const U* con, Variables *theVars) {
	UNUSED(t);
	UNUSED(con);
	UNUSED(theVars);
	arr out;
	return out;
    }

    /**
      Function to calculate the dy/dt values for the Condition at the
        given time stamp.

      \param t The current timestamp
      \param con Condition object giving the input parameters
      \param theVars The global variables
      \return A Condition instance containing the dy/dt values for this
        time stamp.
      */
    static U* _MB_con(const double t, const U* con, Variables *theVars) {
	UNUSED(t);
	UNUSED(con);
	UNUSED(theVars);
	return nullptr;
    }

    /**
      Calculate the Rates of the module based on the input condition.

      \param t The current timestamp
      \param con Condition object giving the input parameters
      \param theVars The global variables
      */
    static void _Rate(const double t, const U* con, Variables *theVars) {
	UNUSED(t);
	UNUSED(con);
	UNUSED(theVars);
    }

 protected:

    /** Initialized the added classes */
    static void _initAddedClasses() {}

    /** \copydoc ValueSetBase::_initStaticMembers */
    static void _initStaticMembers() {
	ParentClass::_initStaticMembers();
	initAddedClasses();
    }

};

  template<class T, class U, MODULE ID>
  std::vector<ValueSetClassWrapper> ModuleBase<T, U, ID>::added_classes = {};

}  // namespace modules
}  // namespace ePhotosynthesis

#define MODULE_BASE(name)						\
  ModuleBase<name, VARS_CLASS_VAR(name, COND), MODULE_ ## name>
#ifdef TESTING
#define FORWARD_DECLARE_MODULE_BASE_TESTING(name)	\
  namespace test {					\
    class name ## ModuleTest;				\
  }
#define DECLARE_MODULE_BASE_TESTING(name)	\
  friend class test::name ## ModuleTest;
#else
#define FORWARD_DECLARE_MODULE_BASE_TESTING(name)
#define DECLARE_MODULE_BASE_TESTING(name)
#endif

  // Macros for forward declarations for module classes
/**
   Macro for boiler plate forward declaring classes required to declare a
     singular or composite module class
   \param name Name of module
 */
#define FORWARD_DECLARE_MODULE_BASE(name)				\
  FORWARD_DECLARE_MODULE_BASE_TESTING(name)
/**
   Macro for boiler plate forward declaring classes required to declare a
     singular module class
   \param name Name of module
 */
#define FORWARD_DECLARE_MODULE(name)					\
  FORWARD_DECLARE_MODULE_BASE(name)					\
  namespace modules {							\
    class PARENT_ ## name;						\
  }
/**
   Macro for boiler plate forward declaring classes required to declare a
     composite module class w/o a parent module
   \param name Name of module
 */
#define FORWARD_DECLARE_MODULE_TOP(name)				\
  FORWARD_DECLARE_MODULE_BASE(name)
/**
   Macro for boiler plate forward declaring classes required to declare a
     composite module class
   \param name Name of module
 */
#define FORWARD_DECLARE_MODULE_COMPOSITE(name)				\
  FORWARD_DECLARE_MODULE_TOP(name)					\
  namespace modules {							\
    class PARENT_ ## name;						\
  }
/**
   Macro for boiler plate forward declaring classes required to declare a
     composite module class with a driver
   \param name Name of module
 */
#define FORWARD_DECLARE_MODULE_DRIVER(name)	\
  FORWARD_DECLARE_MODULE_COMPOSITE(name)	\
  namespace drivers {				\
    class name ## Driver;			\
  }

  // Macros for module class declaration
/**
   Macro for boiler plate declaring a singular or composite module class
   \param name Name of module
 */
#define DECLARE_MODULE_BASE(name)					\
  private:								\
  friend BaseClass;							\
  friend ValueSetClassType<name>;					\
  friend ModuleBase;							\
  DECLARE_MODULE_BASE_TESTING(name)					\
  public:								\
  static const std::vector<PARAM_TYPE>& get_parameter_types() {		\
    static std::vector<PARAM_TYPE> out = {				\
      PREFIX_EACH(PARAM_TYPE_, EXPAND(PARAM_TYPES_ ## name))		\
    };									\
    return out;								\
  }									\
  static void _initAddedClasses();					\
  static VARS_CLASS_VAR(name, COND)* _createCondition(Variables *theVars); \
  static void _initOrig(Variables *theVars,				\
			VARS_CLASS_VAR(name, COND)*			\
			VARS_INST_VAR_MOD_BASE(name, COND));		\
  static void _initCalc(Variables *theVars,				\
			VARS_CLASS_VAR(name, COND)*			\
			VARS_INST_VAR_MOD_BASE(name, COND));		\
  static arr _MB(const double t,					\
		 const VARS_CLASS_VAR(name, COND)*			\
		 const VARS_INST_VAR_MOD_BASE(name, COND),		\
		 Variables *theVars);					\
  static VARS_CLASS_VAR(name, COND)*					\
    _MB_con(const double t,						\
	    const VARS_CLASS_VAR(name, COND)*				\
	    const VARS_INST_VAR_MOD_BASE(name, COND),			\
	    Variables *theVars);
/**
   Macro for boiler plate declaring a singular module class
   \param name Name of module
 */
#define DECLARE_MODULE(name)						\
  private:								\
  friend class modules::PARENT_ ## name;				\
  public:							       	\
  DECLARE_VALUE_SET_STATIC(name, ModuleBase<name,			\
			   VARS_CLASS_VAR(name, COND),		\
			   MODULE_ ## name>)				\
  DECLARE_MODULE_BASE(name);						\
  /** \copydoc ModuleBase::_Rate */					\
  static void _Rate(const double t,					\
		    const VARS_CLASS_VAR(name, COND)*			\
		    const VARS_INST_VAR_MOD_BASE(name, COND),		\
		    Variables *theVars);				\
  static double TIME; /**< The timestamp of the most recent call to _Rate */ \
  static std::size_t N; /**< The current size of the BF TimeSeries */
  
/**
   Macro for boiler plate declaring a composite module class w/o a parent
       module
   \param name Name of module
 */
#define DECLARE_MODULE_TOP(name)					\
  DECLARE_VALUE_SET_STATIC_COMPOSITE(name, (CHILDREN_ ## name),		\
				     ModuleBase<name,			\
				     VARS_CLASS_VAR(name, COND),	\
				     MODULE_ ## name>)			\
  DECLARE_MODULE_BASE(name)
/**
   Macro for boiler plate declaring a composite module class
   \param name Name of module
 */
#define DECLARE_MODULE_COMPOSITE(name)					\
  private:								\
  friend class modules::PARENT_ ## name;				\
  public:							       	\
  DECLARE_MODULE_TOP(name)						\
  /** \copydoc ModuleBase::_Rate */					\
  static void _Rate(const double t,					\
		    const VARS_CLASS_VAR(name, COND)*			\
		    const VARS_INST_VAR_MOD_BASE(name, COND),		\
		    Variables *theVars) {				\
    UNUSED(t);								\
    UNUSED(VARS_INST_VAR_MOD_BASE(name, COND));				\
    UNUSED(theVars);							\
  }
/**
   Macro for boiler plate declaring a composite module class with a driver
   \param name Name of module
 */
#define DECLARE_MODULE_DRIVER(name)		\
  DECLARE_MODULE_COMPOSITE(name)		\
  private:					\
  friend class drivers::name ## Driver;		\
  public:

  // Macros for defining module classes
/**
   Utility macro for adding additional value set classes associated with
     the module to the added_classes vector
   \param mod Module name
   \param pt Parameter type name defining the type of value set to add
 */
#define DEFINE_MODULE_COMPOSITE_ADDED_CLASS(mod, pt)			\
  addClass<VARS_CLASS_VAR(mod, pt)>()
/**
   Macro for boiler plate defining a singular or composite module class
   \param name Name of module
 */
#define DEFINE_MODULE_BASE(name)					\
  /** \copydoc ModuleBase::_initAddedClasses */				\
  void name::_initAddedClasses() {					\
      FOR_EACH_WITH_ARGS(DEFINE_MODULE_COMPOSITE_ADDED_CLASS,		\
			 (name), UNPACK_MACRO(PARAM_TYPES_ ## name));	\
      size_t exp = FOR_EACH_NARG(PARAM_TYPES_ ## name);			\
      size_t act = added_classes.size();				\
      if (exp != act) {							\
	ERROR_VALUE_SET("Failed to initialized added classes. ",	\
			exp, " expected, ", act, " added");		\
      }									\
  }
/**
   Utility macro for defining associated value set class members
   \param mod Name of the module
   \param pt Name of the associated value set parameter type
 */
#define DEFINE_MODULE_PARAM_TYPE(mod, pt)				\
  DEFINE_VALUE_SET_NS(VARS_PARAM_PT_NAMESPACE(pt, )::,			\
		      VARS_CLASS_VAR_LOCAL(mod, pt))
/**
   Utility to initialize values in an associated value set
   \param mod Name of the module
   \param pt Name of the associated value set parameter type
 */
#define DEFINE_MODULE_INIT_ADDED(mod, pt)				\
  theVars->initParam(VARS_INST_VAR_MOD(mod, pt), noDefaults)
/**
   Macro for boiler plate defining a singular module class
   \param name Name of module
 */
#define DEFINE_MODULE(name)						\
  DEFINE_VALUE_SET_STATIC(name);					\
  FOR_EACH_WITH_ARGS(DEFINE_MODULE_PARAM_TYPE, (name),			\
		     UNPACK_MACRO(PARAM_TYPES_ ## name));		\
  DEFINE_MODULE_BASE(name);						\
  VARS_CLASS_VAR_LOCAL(name, COND)* VARS_CLASS_VAR_LOCAL(name, MOD)::	\
    _createCondition(Variables *theVars) {				\
    VARS_CLASS_VAR_LOCAL(name, COND)* VARS_INST_VAR_MOD_BASE(name, COND) = new VARS_CLASS_VAR_LOCAL(name, COND)(); \
    return VARS_INST_VAR_MOD_BASE(name, COND);				\
  }
#define DEFINE_MODULE2(name)				\
  double VARS_CLASS_VAR_LOCAL(name, MOD)::TIME = 0.;	\
  std::size_t VARS_CLASS_VAR_LOCAL(name, MOD)::N = 1;	\
  DEFINE_CONDITION_INI(name)
/**
   Utility to create a child condition instance
   \param name Name of the child module
 */
#define DEFINE_MODULE_COMPOSITE_CHILD_COND(name)	\
  VARS_CLASS_VAR_LOCAL(name, COND)* VARS_INST_VAR_MOD_BASE(name, COND) = name::init(theVars)
/**
   Utility to get the local name of the created child condition instance
   \param name Name of the child module
 */
#define DEFINE_MODULE_COMPOSITE_CHILD_COND_ARG(name)	\
  VARS_INST_VAR_MOD_BASE(name, COND)
/**
   Macro for boiler plate defining a composite module class
   \param name Name of module
 */
#define DEFINE_MODULE_COMPOSITE(name)					\
  DEFINE_VALUE_SET_STATIC_COMPOSITE(name, (CHILDREN_ ## name));		\
  FOR_EACH_WITH_ARGS(DEFINE_MODULE_PARAM_TYPE, (name),			\
		     UNPACK_MACRO(PARAM_TYPES_ ## name));		\
  DEFINE_MODULE_BASE(name)						\
  VARS_CLASS_VAR_LOCAL(name, COND)* VARS_CLASS_VAR_LOCAL(name, MOD)::	\
    _createCondition(Variables *theVars) {				\
    FOR_EACH(DEFINE_MODULE_COMPOSITE_CHILD_COND, CHILDREN_ ## name);	\
    VARS_CLASS_VAR_LOCAL(name, COND)* VARS_INST_VAR_MOD_BASE(name, COND) = new VARS_CLASS_VAR_LOCAL(name, COND)( FOR_EACH_COMMA(DEFINE_MODULE_COMPOSITE_CHILD_COND_ARG, CHILDREN_ ## name) ); \
    return VARS_INST_VAR_MOD_BASE(name, COND);				\
  }
/**
   Macro for boiler plate defining a singular module class in the header
   \param name Name of module
 */
#define DEFINE_MODULE_HEADER(name)					\
  DEFINE_VALUE_SET_STATIC_HEADER(VARS_CLASS_VAR_LOCAL(name, MOD))

/**
   Macro for boiler plate defining a composite module class without a
     parent module in the header
   \param name Name of module
 */
#define DEFINE_MODULE_TOP_HEADER(name)			\
  DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE(name)
/**
   Macro for boiler plate defining a composite module class in the header
   \param name Name of module
 */
#define DEFINE_MODULE_COMPOSITE_HEADER(name)		\
  DEFINE_VALUE_SET_STATIC_HEADER_COMPOSITE(name)

// Macros for default methods (TODO: Move to base class)
#define DEFINE_DEFAULT_CHECKALT(name)
  
#define DEFINE_DEFAULT_INITALT(name)					\
  void name::_initCalc(Variables *theVars,				\
		       VARS_CLASS_VAR_LOCAL(name, COND)* VARS_INST_VAR_MOD_BASE(name, COND)) { \
    UNUSED(theVars);							\
    UNUSED(VARS_INST_VAR_MOD_BASE(name, COND));				\
  }									\
  void name::_initOrig(Variables *theVars,				\
		       VARS_CLASS_VAR_LOCAL(name, COND)* VARS_INST_VAR_MOD_BASE(name, COND)) { \
    UNUSED(theVars);							\
    UNUSED(VARS_INST_VAR_MOD_BASE(name, COND));				\
  }
