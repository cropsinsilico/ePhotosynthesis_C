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
#include <map>
#include <memory>
#include "definitions.hpp"
#include "enums/enums_utils.hpp"
#include "ValueSet.hpp"

#include "vel/VelAll.hpp"
#include "ke/KEAll.hpp"
#include "pool/PoolAll.hpp"
#include "rc/RCAll.hpp"
#include "conditions/ConditionAll.hpp"

namespace ePhotosynthesis {

#define MEMBERS_Variables MEMBERS_ALLVARS
#define PARAM_TYPES_ALL VARS

#ifdef SUNDIALS_CONTEXT_REQUIRED
extern std::shared_ptr<SUNContext> global_context; //!< Global context

/**
   Create a new SUNContext instance.
   \returns SUNContext instance.
*/
SUNContext* create_sundials_context();

/**
   Initialize the global SUNContext.
*/
void init_global_sundials_context();

/**
   Destroy the global SUNContext.
 */
void cleanup_global_sundials_context();

#endif // SUNDIALS_CONTEXT_REQUIRED
    
/**
  Structure to hold global variables
  */
class Variables : public VALUE_SET_PARENT(Variables, Variables, MODULE_ALL, PARAM_TYPE_VARS) {
public:
    DECLARE_VALUE_SET(Variables, VALUE_SET_PARENT(Variables, Variables, MODULE_ALL, PARAM_TYPE_VARS))

#ifdef MAKE_EQUIVALENT_TO_MATLAB
    static void _initDefaults();
#endif // MAKE_EQUIVALENT_TO_MATLAB

#ifdef SUNDIALS_CONTEXT_REQUIRED

private:
    std::shared_ptr<SUNContext> _context; /**< Sundials context */
    int _context_flags = 0; /**< Flags describing the context */
    /** Flags describing context traits */
    enum CONTEXT_FLAGS : int {
        CONTEXT_FLAG_GLOBAL = 0x00000001, //< The context is global
    };
public:
    /**
       Construct a new variables instance for a given sundials context
       \param[in, out] ctx Sundials context.
       \param[in] flags Bitwise CONTEXT_FLAGS flags describing how the
         context was/should be created.
    */
    Variables(SUNContext* ctx, const int flags = 0);
    /**
       Get the shared pointer to the Sundials context.
       \return Sundials context shared pointer.
     */
    std::shared_ptr<SUNContext> context_ptr() {
      return _context;
    }
    /**
       Get the Sundials context.
       \return Sundials context.
     */
    SUNContext& context() {
      return *(_context.get());
    }
#endif // SUNDIALS_CONTEXT_REQUIRED
    /**
       Construct a new variables instance for a given sundials context
    */
    Variables();
    /** Destructor */
    ~Variables();
    /**
       Copy constructor for a pointer to another variables instance.
         Some variables are not included in the default copy (e.g. alfa, 
	 fc, lightParam, CO2A, RedoxReg_MP, module *_Param values, & 
	 rates). To include these, use deepcopy.
       \param[in] other Variables instance to copy.
     */
    Variables(const Variables* other);
    /**
       Copy constructor.
         Some variables are not included in the default copy (e.g. alfa, 
	 fc, lightParam, CO2A, RedoxReg_MP, module *_Param values, & 
	 rates). To include these, use deepcopy.
       \param[in] other Variables instance to copy.
     */
    Variables(const Variables& other);
    
 protected:
    static void _initStaticMembers();
    
    /**
       Copy additional members.
         Some variables are not included in the default copy (e.g. alfa, 
	 fc, lightParam, CO2A, RedoxReg_MP, module *_Param values, & 
	 rates). To include these, use deepcopy.
       \param[in] other Variables instance to copy.
     */
    void __copyMembers(const Variables& other) override;
 public:

    /**
       Finalize variables read from input including any unit changes.
     */
    void finalizeInputs();
    /**
       Create a deep copy of this instance including variables excluded
         from a default copy.
       \returns New instance with all parameters copied.
     */
    Variables* deepcopy() const;
    /** \copydoc ValueSet::diff */
    std::string _diff(const Variables& other,
		      std::size_t padKeys=0,
		      std::size_t padVals=0,
		      bool includePrefixes=false,
		      bool noChildren = false) const override;
    /** \copydoc ValueSet::equals */
    bool equals(const ValueSet_t& b,
		const bool noChildren = false) const override;
    /**
       Serialization operator.
       \param[in, out] out Output stream.
       \param[in] in Instance to serialize.
       \returns Updated output stream.
    */
    friend std::ostream& operator<<(std::ostream &out, const Variables *in);
    /**
       Record a value set by copying it into this set of Variables.
       \param[in] x Value set to record.
       \param[in] conditions Map that conditions for composite modules
         that are not stored on Variables instances should be added to.
     */
    void setRecord(const ValueSet_t* x,
		   std::map<MODULE, const ValueSet_t*>& conditions);
    void setRecord(const ValueSet_t* x);
    void setRecord(ValueSet_t* x,
		   std::map<MODULE, ValueSet_t*>& conditions);

    /**
       Serialize all parameters attached to this instance to a file.
       \param[in] filename Name of file where parameters should be written.
       \param[in] includeSkipped If true, skipped keys will be output.
       \param[in] skip_modules Vector of IDs for modules that should not
         be serialized.
       \param[in] skip_param_types Vector of IDs for parameter types that
         should not be serialized.
       \param[in] skip_keys Key strings to skip in output.
       \param[in] key_aliases String aliases to use for keys.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
       \param[in] subset Subset of parameters to output.
     */
    void dump(const std::string& filename,
              const bool includeSkipped = false,
              const std::vector<MODULE>& skip_modules={},
              const std::vector<PARAM_TYPE>& skip_param_types={},
              const std::vector<std::string>& skip_keys={},
              const std::map<std::string, std::string>& key_aliases={},
              const std::map<MODULE, const ValueSet_t*>& conditions={},
              const std::vector<std::string>& subset={}) const;
    /**
       Serialize all parameters attached to this instance to an output
         stream.
       \param[in, out] out Output stream.
       \param[in] includeSkipped If true, skipped keys will be output.
       \param[in] skip_modules Vector of IDs for modules that should not
         be serialized.
       \param[in] skip_param_types Vector of IDs for parameter types that
         should not be serialized.
       \param[in] skip_keys Key strings to skip in output.
       \param[in] key_aliases String aliases to use for keys.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
       \param[in] subset Subset of parameters to output.
       \returns Updated output stream.
     */
    std::ostream& dump(std::ostream& out,
                       const bool includeSkipped = false,
                       const std::vector<MODULE>& skip_modules={},
                       const std::vector<PARAM_TYPE>& skip_param_types={},
                       const std::vector<std::string>& skip_keys={},
                       const std::map<std::string, std::string>& key_aliases={},
                       const std::map<MODULE, const ValueSet_t*>& conditions={},
                       const std::vector<std::string>& subset={}) const;
    /**
       Serialize parameters for a single value set to an output stream.
       \param[in] module ID for module that should be serialized.
       \param[in] param_type ID for parameter type that should be serialized.
       \param[in, out] out Output stream.
       \param pad Number of characters that key names should be padded
         to fill.
       \param[in] includeSkipped If true, skipped keys will be output.
       \param[in] skip_modules Vector of IDs for modules that should not
         be serialized.
       \param[in] skip_param_types Vector of IDs for parameter types that
         should not be serialized.
       \param[in] skip_keys Key strings to skip in output.
       \param[in] key_aliases String aliases to use for keys.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
       \returns Updated output stream.
     */
    std::ostream& dump(const MODULE& module,
                       const PARAM_TYPE& param_type,
                       std::ostream& out, std::size_t pad,
                       const bool includeSkipped = false,
                       const std::vector<MODULE>& skip_modules={},
                       const std::vector<PARAM_TYPE>& skip_param_types={},
                       const std::vector<std::string>& skip_keys={},
                       const std::map<std::string, std::string>& key_aliases={},
                       const std::map<MODULE, const ValueSet_t*>& conditions={}) const;
  
    /**
       Initialize the parameters for a value set.
       \param mod Value set module.
       \param pt Value set parameter type.
       \param noDefaults If true, the value pointers will be initialized,
         but the default values will not be assigned to those values.
       \param force If true, initialize the values even if they have
         already been initialized.
       \param value_set Pointer to the value set that should be
         initialized if different than the default version attached to
         Variables.
     */
    void initParam(const MODULE& mod, const PARAM_TYPE& pt,
		   const bool noDefaults=false,
		   const bool force=false,
		   ValueSet_t* value_set=nullptr);

    /**
       Initialize the default values for all value sets selected by the
         current driver.
       \param useC3 If true, C3 defaults should be used.
     */
    static void initAllDefaults(const bool useC3=false);
    /**
       Initialize the default values for a value set.
       \param module Value set module.
       \param param_type Value set parameter type.
       \param useC3 If true, C3 defaults should be used.
       \param filename Full path to a file containing values that should
         be used as defaults in the value set.
       \param force If true, the defaults will be initialized reguardless
         of if the value set is selected by the current driver or not. If
	 false, a warning will be generated when the value set is not
	 selected and the defaults will not be initialized.
     */
    static void initDefaults(const MODULE& module,
			     const PARAM_TYPE& param_type,
			     const bool useC3=false,
			     const std::string& filename="",
			     const bool force=true);
    /**
       Check if a value set is selected by the current driver.
       \param[in] mod Module type of value set to check.
       \param[in] pt Parameter type of value set to check.
       \returns true if the value set is selected, false otherwise.
     */
    static bool isSelected(const MODULE& mod, const PARAM_TYPE& pt);
    /**
       Get the maximum field width of all selected value sets for use
         in aligning field names.
       \returns Maximum field width.
     */
    static std::size_t max_field_width_all();
    /**
       Get the maximum value width of all selected value sets for use
         in aligning values.
       \returns Maximum value width.
     */
    std::size_t max_value_width_all() const;
    /**
       Parse a variable string, checking if there is a prefix containing
         information on the module and/or parameter type associated with
	 the value set that the variable belongs to. If this information
	 is not provided, all value sets will be checked.
       \param[in] k String to parse.
       \param[out] mod Module that the variable was determined to belong
         to.
       \param[out] pt Parameter type that the variable was determined to
         belong to.
       \param[in] isGlymaID If true, k is assumed to contain a GlymaID
         instead of a variable name.
       \param[in] use_1st_match If true, the first variable matching the
         provided string will be returned. If false and there is more
	 than one match, an error will be thrown.
       \returns Variable name extracted from k.
     */
    static std::string parseVar(const std::string& k,
				MODULE& mod, PARAM_TYPE& pt,
				const bool& isGlymaID = false,
				const bool& use_1st_match=false);
    /**
       Check if a value set has a variable that matches the provided
         string.
       \param[in] mod Module associated with the value set that should
         be checked.
       \param[in] pt Parameter type associated with the value set that
         should be checked.
       \param[in] name String to check against variables in the value set.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
       \returns true if the variable is part of the value set, false
         otherwise.
     */
    static bool hasVar(const MODULE& mod, const PARAM_TYPE& pt,
		       const std::string& name,
		       const bool& isGlymaID = false);
    /**
       Check if there is a variable that matches the provided string.
       \param[in] name String to check against variables.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
       \returns true if the variable is part of a value set, false
         otherwise.
     */
    static bool hasVar(const std::string& name,
		       const bool& isGlymaID = false);

    /**
       Get the integer key for a value set variable.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] name String identifying the variable.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
       \returns Variable key.
     */
    static int getKey(const MODULE& mod, const PARAM_TYPE& pt,
		      const std::string& name,
		      const bool& isGlymaID = false);

    /**
       Set the default value for a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] name String identifying the variable.
       \param[in] value The new default value.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
     */
    static void setDefault(const MODULE& mod, const PARAM_TYPE& pt,
			   const std::string& name, const double& value,
			   const bool& isGlymaID = false);
    /**
       Set the default value for a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] key Key identifying the variable.
       \param[in] value The new default value.
     */
    static void setDefault(const MODULE& mod, const PARAM_TYPE& pt,
			   const int& key, const double& value);
    /**
       Set the default value for a variable in a value set.
       \param[in] k String identifying the variable.
       \param[in] value The new default value.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
     */
    static void setDefault(const std::string& k, const double& value,
			   const bool& isGlymaID = false);
    
    /**
       Set the value for a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] name String identifying the variable.
       \param[in] value The new value.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
     */
    void setVar(const MODULE& mod, const PARAM_TYPE& pt,
		const std::string& name, const double& value,
		const bool& isGlymaID = false);
    /**
       Set the value for a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] key Key identifying the variable.
       \param[in] value The new value.
     */
    void setVar(const MODULE& mod, const PARAM_TYPE& pt,
		const int& key, const double& value);
    /**
       Set the value for a variable in a value set.
       \param[in] k String identifying the variable.
       \param[in] value The new value.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
     */
    void setVar(const std::string& k, const double& value,
		const bool& isGlymaID = false);

    /**
       Get the value of a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] name String identifying the variable.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
       \returns Variable value.
     */
    double getVar(const MODULE& mod, const PARAM_TYPE& pt,
		  const std::string& name,
		  const bool& isGlymaID = false) const;
    /**
       Get the value of a variable in a value set.
       \param[in] mod Module associated with the value set that the
         variable is part of.
       \param[in] pt Parameter type associated with the value set that the
         variable is part of.
       \param[in] key Key identifying the variable.
       \returns Variable value.
     */
    double getVar(const MODULE& mod, const PARAM_TYPE& pt,
		  const int& key) const;
    /**
       Get the value of a variable in a value set.
       \param[in] k String identifying the variable.
       \param[in] isGlymaID If true, name will be treated as a GlymaID.
         If false, name will be treated as the variable name.
       \returns Variable value.
     */
    double getVar(const std::string& k,
		  const bool& isGlymaID = false) const;

    /**
       Get the set of parameters types for a module.
       \param[in] mod Module ID.
       \param[in] for_instance If true, parameter types that do not
         have instances in Variables will be excluded.
       \param[in] include_cond If true, local copies of conditions will
         be included.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
     */
    static std::vector<PARAM_TYPE> getParamTypes(const MODULE& mod,
						 const bool for_instance = false,
						 const bool include_cond = false,
						 const std::map<MODULE, const ValueSet_t*>& conditions = {});
    /**
       Get a ValueSetClass_t instance for a module that can be used to
         call virtual methods.
       \param[in] mod Module ID.
       \param[in] no_error_on_invalid If true, no error will be thrown if the
         combination of mod & pt are unrecognized.
       \param[in] error_context String providing context for errors.
       \returns Pointer to ValueSetClass_t instance.
     */
    static ValueSetClass_t* getModule(const MODULE& mod,
				      const bool no_error_on_invalid = false,
				      const std::string& error_context = "");
    /**
       Get a constant ValueSetClass_t instance for a value set that can
         be used to call static methods.
       \param[in] mod Module associated with the value set.
       \param[in] pt Parameter type associated with the value set.
       \param[in] no_error_on_invalid If true, no error will be thrown if the
         combination of mod & pt are unrecognized.
       \param[in] error_context String providing context for errors.
       \returns Pointer to ValueSetClass_t instance.
     */
    static ValueSetClass_t* getValueSetClass(const MODULE& mod,
					     const PARAM_TYPE& pt = PARAM_TYPE_MOD,
					     const bool no_error_on_invalid = false,
					     const std::string& error_context = "");
    /**
       Get a ValueSet_t* instance for a value set that can be used to
         call virtual methods.
       \param[in] mod Module associated with the value set.
       \param[in] pt Parameter type associated with the value set.
       \param[in] no_error_on_invalid If true, no error will be thrown if the
         combination of mod & pt are unrecognized.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
       \param[in] error_context String providing context for errors.
       \returns Pointer to ValueSet_t instance.
     */
    ValueSet_t* getValueSet(const MODULE& mod,
			    const PARAM_TYPE& pt = PARAM_TYPE_MOD,
			    const bool no_error_on_invalid = false,
			    const std::map<MODULE, ValueSet_t*>& conditions = {},
			    const std::string& error_context = "");
    /**
       Get a constant ValueSet_t* instance for a value set that can be
         used to call virtual methods.
       \param[in] mod Module associated with the value set.
       \param[in] pt Parameter type associated with the value set.
       \param[in] no_error_on_invalid If true, no error will be thrown if the
         combination of mod & pt are unrecognized.
       \param[in] conditions Map of conditions for composite modules that
         are not stored on Variables instances.
       \param[in] error_context String providing context for errors.
       \returns Constant pointer to ValueSet_t instance.
     */
    const ValueSet_t* getValueSet(const MODULE& mod,
				  const PARAM_TYPE& pt = PARAM_TYPE_MOD,
				  const bool no_error_on_invalid = false,
				  const std::map<MODULE, const ValueSet_t*>& conditions = {},
				  const std::string& error_context = "") const;

    /**
       Cleanup any static variables allocated for all registered value
         sets.
     */
    static void cleanupValueSets();
    /**
       Cleanup any static variables allocated for a value set.
       \param[in] mod Module associated with the value set.
       \param[in] pt Parameter type associated with the value set.
       \param[in] noChildren If true, child classes will not be cleaned
         up.
     */
    static void cleanupValueSet(const MODULE& mod, const PARAM_TYPE& pt,
                                const bool noChildren = false);

    /**
       Read parameters from a file.
       \param[in] File to read.
     */
    void readParam(const std::string& fname);
    /**
       Read parameters from a file, checking for duplicates
       \param[in] File to read.
       \param[in, out] Map that read variables should be checked
         against for duplicates and copied into.
     */
    void readParam(const std::string& fname,
                   std::map<std::string, std::string>& inputs);
    /**
       Read Enzyme activities from a file.
       \param[in] File to read.
    */
    void readEnzymeAct(const std::string& fname);
    /**
       Read genetic regulatory network expression data from a file.
       \param[in] File to read.
    */
    void readGRN(const std::string& fname);

    bool record = false;
    int GP = 0;
    int GRNC = 0; /**< Control parameter; if 1, VfactorCp values will be used to scale enzyme activities in the PS, PR, & SUCS modules when CO2 > 0 */
    int GRNT = 0; /**< Control parameter; if 1, VfactorT values will be used to scale enzyme activities in the PS, PR, & SUCS modules when T > 25 */
    int PAR_in_Wpm2 = 0; /**< Control parameter; if 1, the input TestLi will be taken to be in units of W/m**2 */

#ifdef MAKE_EQUIVALENT_TO_MATLAB
    int RUBISCOMETHOD = 2;
#else // MAKE_EQUIVALENT_TO_MATLAB
    int RUBISCOMETHOD = 1;
#endif // MAKE_EQUIVALENT_TO_MATLAB

    std::map<std::string, double> EnzymeAct; /**< Map of enzyme activity levels used when GP == 0 */

    arr VfactorCp = zeros(33);
    arr VfactorT = ones(28);

    // misc
    arr FluxTR = {};

    TimeSeries<std::vector<double> > CO2A = TimeSeries<std::vector<double> > ();
    std::vector<arr> RedoxReg_MP;

#define DO_MEMBERS(mod, pt)				\
    VARS_CLASS_VAR(mod, pt) VARS_INST_VAR(mod, pt)
#define DO_MEMBERS_PACKED(args)					\
    DO_MEMBERS args
#define DO_MEMBERS_CORE(mod)						\
    arr mod ## _Param = zeros(2);					\
    arr mod ## Ratio = ones(NRATIO_ ## mod);				\
    DO_MEMBERS(mod, VEL_SERIES) = VARS_CLASS_VAR(mod, VEL_SERIES)()
#define DO_MEMBERS_CONNECTION(mod)		\
    bool mod ## _com = false
    VARS_INST_APPLY_TO_MEMBERS(DO_MEMBERS);
#undef DO_MEMBERS_CONNECTION
#undef DO_MEMBERS_CORE
#undef DO_MEMBERS_PACKED
#undef DO_MEMBERS

    bool useC3 = false;
#ifdef INCDEBUG
    Debug::RequestedDebug debuglevel = Debug::None;
#endif
};

}  // namespace ePhotosynthesis
