#include <cstdlib>
#include <string>
#include <stdexcept>
#include "Variables.hpp"
#include "modules/ModuleAll.hpp"
#include "globals.hpp"


#ifdef SUNDIALS_CONTEXT_REQUIRED
std::shared_ptr<SUNContext> ePhotosynthesis::global_context(NULL);

SUNContext* ePhotosynthesis::create_sundials_context() {
    SUNContext* context = new SUNContext();
    SUNErrCode error_code = SUNContext_Create(SUN_COMM_NULL, context);
    if (error_code) {
        throw std::runtime_error("SUNContext_Create failed: " +
                                 std::string(SUNGetErrMsg(error_code)));
    }
    return context;
}

void ePhotosynthesis::init_global_sundials_context() {
    if (!global_context) {
        SUNContext* context = create_sundials_context();
        global_context.reset(context);
        std::atexit(cleanup_global_sundials_context);
    }
}

void ePhotosynthesis::cleanup_global_sundials_context() {
    if (global_context) {
        SUNContext_Free(global_context.get());
        global_context.reset();
    }
}

#endif // SUNDIALS_CONTEXT_REQUIRED

using namespace ePhotosynthesis;

DEFINE_VALUE_SET(Variables);

#ifdef MAKE_EQUIVALENT_TO_MATLAB
void Variables::_initDefaults() {
    setDefault(ValueSetClass::alpha1, 1.0, true);
    setDefault(ValueSetClass::alpha2, 1.0, true);
}
#endif // MAKE_EQUIVALENT_TO_MATLAB

void Variables::_initStaticMembers() {
    select(true); // Variables always selected
    ValueSetClass::_initStaticMembers();
}

#ifdef SUNDIALS_CONTEXT_REQUIRED
Variables::Variables(SUNContext* ctx, const int flags) :
  ValueSet(), _context(), _context_flags(flags) {
    if (_context == nullptr) {
        if (flags & CONTEXT_FLAG_GLOBAL) {
            init_global_sundials_context();
            _context = global_context;
            _context_flags |= CONTEXT_FLAG_GLOBAL;
        } else {
            _context.reset(create_sundials_context());
        }
    } else {
        _context.reset(ctx);
    }
    select();
    useC3 = usesC3();
    initValues();
}
Variables::Variables() : Variables((SUNContext*)nullptr) {}
Variables::Variables(const Variables& other) :
  ValueSet(other), _context(other._context),
  _context_flags(other._context_flags) {
    copyInstance(other);
}
Variables::~Variables() {
    if (!(_context_flags & CONTEXT_FLAG_GLOBAL)) {
        if (_context.use_count() == 1)
          SUNContext_Free(_context.get());
        _context.reset();
    }
}
#else // SUNDIALS_CONTEXT_REQUIRED
Variables::Variables() :
  ValueSet() {}
Variables::Variables(const Variables& other) :
  ValueSet(other) {
    copyInstance(other);
    *this = other;
}
Variables::~Variables() {}
#endif // SUNDIALS_CONTEXT_REQUIRED

Variables::Variables(const Variables* other) :
  Variables(*other) {}

void Variables::__copyMembers(const Variables& other) {
#ifdef SUNDIALS_CONTEXT_REQUIRED
    _context = other._context;
    _context_flags = other._context_flags;
#endif // SUNDIALS_CONTEXT_REQUIRED
    record = other.record;
    GP = other.GP;
    GRNC = other.GRNC;
    GRNT = other.GRNT;
    RUBISCOMETHOD = other.RUBISCOMETHOD;
#define DO_MEMBERS(mod, pt)						\
    VARS_INST_VAR(mod, pt) = other.VARS_INST_VAR(mod, pt)
#define DO_MEMBERS_PACKED(args) DO_MEMBERS args
#define DO_MEMBERS_CORE(mod)			\
    mod ## Ratio = other.mod ## Ratio
#define DO_MEMBERS_CONNECTION(mod)		\
    mod ## _com = other.mod ## _com
    VARS_INST_APPLY_TO_MEMBERS(DO_MEMBERS);
#undef DO_MEMBERS
#undef DO_MEMBERS_PACKED
#undef DO_MEMBERS_CORE
#undef DO_MEMBERS_CONNECTION
    
    EnzymeAct = other.EnzymeAct;
    VfactorCp = other.VfactorCp;
    VfactorT = other.VfactorT;
    FluxTR = other.FluxTR;
    useC3 = other.useC3;
    ProteinTotalRatio = other.ProteinTotalRatio;
}

void Variables::finalizeInputs() {
    // Conversion from Air_CO2 ppm to intercellular CO2
#ifdef MAKE_EQUIVALENT_TO_MATLAB
    CO2_in *= 0.7;
#endif // MAKE_EQUIVALENT_TO_MATLAB
  
    // Conversion to umoles
    CO2_cond = CO2_in / (3. * pow(10., 4.));
#ifndef MAKE_EQUIVALENT_TO_MATLAB
    if (!useC3)
      CO2_cond *= 0.7;
#endif // MAKE_EQUIVALENT_TO_MATLAB
    
    if (PAR_in_Wpm2) {
      // Conversion from W m^{-2} to u moles m^{-2} s^{-1}
      TestLi_Wps = TestLi;
      TestLi = TestLi_Wps * 1.0e6 / 2.35e5;
    } else {
      TestLi_Wps = TestLi / (1.0e6 / 2.35e5);
    }
}

Variables* Variables::deepcopy() const {
  Variables* out = new Variables(*this);
  out->alfa = this->alfa;
  out->fc = this->fc;
  out->lightParam = this->lightParam;
  out->CO2A = this->CO2A;
  out->RedoxReg_MP = this->RedoxReg_MP;
#define DO_MEMBERS(mod, pt)
#define DO_MEMBERS_PACKED(args) DO_MEMBERS args
#define DO_MEMBERS_CORE(mod)						\
  out->CONCATENATE(mod, _Param) = this->CONCATENATE(mod, _Param)
#define DO_MEMBERS_CONNECTION(mod)
  VARS_INST_APPLY_TO_MEMBERS(DO_MEMBERS);
#undef DO_MEMBERS
#undef DO_MEMBERS_PACKED
#undef DO_MEMBERS_CORE
#undef DO_MEMBERS_CONNECTION
#define DO_MOD(mod)							\
  out->CONCATENATE(mod, _Vel) = this->CONCATENATE(mod, _Vel);		\
  out->CONCATENATE(mod, _VEL) = this->CONCATENATE(mod, _VEL)
  FOR_EACH(DO_MOD, EXPAND VARS_INST_MODULES);
#undef DO_MOD
  return out;
}

std::string Variables::_diff(const Variables& other,
			     std::size_t padKeys, std::size_t padVals,
			     bool includePrefixes,
			     bool noChildren) const {
    std::string out = ValueSetClass::_diff(other, padKeys, padVals,
					   includePrefixes, noChildren);
#define FITER(V, ...)							\
    if (V->_virtual_get_param_type() == PARAM_TYPE_VARS) continue;	\
    const ValueSet_t* V2 = other.GET_VALUE_SET(V->_virtual_get_module(), \
	 				       V->_virtual_get_param_type()); \
    out += V->diff(*V2, __VA_ARGS__);
    VARS_ITER_MACRO_INST(FITER, padKeys, padVals,
			 includePrefixes, noChildren);
#undef FITER
    return out;
}
bool Variables::equals(const ValueSet_t &b0,
		       const bool noChildren) const {
  if (!ValueSetClass::equals(b0)) {
    std::cerr << "ValueSetClass::equals returned false" << std::endl;
    return false;
  }
  const Variables& b = static_cast<const Variables&>(b0);
#define CHECK(name)					\
    if (name != b.name) {				\
      std::cerr << #name << " not equal" << std::endl;	\
      return false;					\
    }
#ifdef SUNDIALS_CONTEXT_REQUIRED
    CHECK(_context);
#endif // SUNDIALS_CONTEXT_REQUIRED
    FOR_EACH(CHECK, record, GP, GRNC, GRNT, RUBISCOMETHOD, useC3,
	     EnzymeAct, VfactorCp, VfactorT, FluxTR, CO2A, RedoxReg_MP,
	     FIBF_Pool);
#define CHECK_COM(name) CHECK(name ## _com)
    FOR_EACH(CHECK_COM, EXPAND(VARS_INST_CONNECTIONS));
#undef CHECK_COM
#undef CHECK
    const std::map<MODULE, const ValueSet_t*> conditions;
#define FITER(V, other, ...)						\
    const ValueSet_t* V2 = other.GET_VALUE_SET(V->_virtual_get_module(), \
					       V->_virtual_get_param_type()); \
    if (V->_virtual_selected() != V2->_virtual_selected()) {		\
	std::cerr << "isEqual(" << V->_virtual_get_module() <<		\
	  ", " << V->_virtual_get_param_type() <<			\
	  "): isSelected differs" << std::endl;				\
	return false;							\
    }									\
    if (!V->_virtual_selected()) return true;				\
    if (!V->equals(*V2, __VA_ARGS__)) return false
    VARS_ITER_MACRO_COND(FITER, b, noChildren);
#undef FITER
    return true;
}

std::ostream& ePhotosynthesis::operator<<(std::ostream &out, const Variables *in) {
    out << "record = " << in->record << std::endl;
#define DO_CONN(mod)						\
    out << #mod << "_com = " << in->mod ## _com << std::endl
    FOR_EACH(DO_CONN, VARS_INST_CONNECTIONS);
#undef DO_CONN

    out << "GP = " << in->GP << std::endl;
    out << "GRNC = " << in->GRNC << std::endl;
    out << "GRNT = " << in->GRNT << std::endl;

    out << "RUBISCOMETHOD = " << in->RUBISCOMETHOD << std::endl;

    out << "CO2_cond = " << in->CO2_cond << std::endl;

    out << "GLight = " << in->GLight << std::endl;
    out << "O2_cond = " << in->O2_cond << std::endl;
    out << "PS12ratio = " << in->PS12ratio << std::endl;
    out << "ADP = " << in->ADP << std::endl;

    out << "Pi = " << in->Pi << std::endl;

    out << "TestATPCost = " << in->TestATPCost << std::endl;
    out << "CO2_in = " << in->CO2_in << std::endl;
    out << "TestLi = " << in->TestLi << std::endl;
    out << "PS2BF_Pi = " << in->PS2BF_Pi << std::endl;
    out << "PS_PR_Param = " << in->PS_PR_Param << std::endl;

    out << "Tp = " << in->Tp << std::endl;
    out << "alfa = " << in->alfa << std::endl;
    out << "fc = " << in->fc << std::endl;
    out << "lightParam = " << in->lightParam << std::endl;

    // Parameters
#define DO_PARAM(mod)				\
    out << #mod << "_Param = ";			\
    for (auto i : in->CONCATENATE(mod, _Param))	\
      out << i << ", ";				\
    out << std::endl
    FOR_EACH(DO_PARAM, EXPAND VARS_INST_MODULES);
#undef DO_PARAM

    // Vel
#define DO_VEL(mod) out << in->CONCATENATE(mod, _Vel)
    FOR_EACH(DO_VEL, EXPAND VARS_INST_MODULES);
#undef DO_VEL

    // Ratio
#define DO_RATIO(mod)					\
    out << #mod << "Ratio = ";				\
    for (auto i : in->CONCATENATE(mod, Ratio))		\
      out << i << ", ";					\
    out << std::endl
    FOR_EACH(DO_RATIO, EXPAND VARS_INST_MODULES);
#undef DO_RATIO

    out << "VfactorCp = ";
for (auto i : in->VfactorCp)
    out << i << ", ";
out << std::endl;
    out << "VfactorT = ";
for (auto i : in->VfactorT)
    out << i << ", ";
out << std::endl;

    // misc
    out << "FluxTR = ";
for (auto i : in->FluxTR)
    out << i << ", ";
out << std::endl;

    out << "useC3 = " << in->useC3 << std::endl;

    // Conditions
#define DO_COND(mod)							\
    out << #mod "2OUT: " << std::endl << (in->mod ## 2OUT) << std::endl
    FOR_EACH(DO_COND, EXPAND VARS_INST_MODULES);
#undef DO_COND
    return out;
}

void Variables::dump(const std::string& filename,
                     const bool includeSkipped,
                     const std::vector<MODULE>& skip_modules,
                     const std::vector<PARAM_TYPE>& skip_param_types,
                     const std::vector<std::string>& skip_keys,
                     const std::map<std::string, std::string>& key_aliases,
                     const std::map<MODULE, const ValueSet_t*>& conditions,
                     const std::vector<std::string>& subset) const {
    std::ofstream fd;
    fd.open(filename);
    dump(fd, includeSkipped, skip_modules, skip_param_types,
	 skip_keys, key_aliases, conditions, subset);
    fd.close();
}
std::ostream& Variables::dump(std::ostream& out,
                              const bool includeSkipped,
                              const std::vector<MODULE>& skip_modules,
                              const std::vector<PARAM_TYPE>& skip_param_types,
                              const std::vector<std::string>& skip_keys,
                              const std::map<std::string, std::string>& key_aliases,
                              const std::map<MODULE, const ValueSet_t*>& conditions,
                              const std::vector<std::string>& subset) const {
    std::size_t pad = max_field_width_all();
    if (!subset.empty()) {
        for (typename std::vector<std::string>::const_iterator it = subset.begin();
             it != subset.end(); it++) {
            MODULE mod = MODULE_NONE;
            PARAM_TYPE pt = PARAM_TYPE_NONE;
            std::string name = parseVar(*it, mod, pt);
            const ValueSetClass_t* vs = getValueSetClass(mod, pt, false,
                                                         // conditions,
                                                         "dump");
            double val = vs->get(name);
            vs->print_value(name, val, out, 0, pad, true, includeSkipped,
                            skip_keys, key_aliases);
        }
        return out;
    }
#define FITER(V, ...)							\
    if (V->_virtual_selected() &&					\
	!__contains(skip_modules, V->_virtual_get_module()) &&		\
        !__contains(skip_param_types, V->_virtual_get_param_type())) {	\
	V->print(__VA_ARGS__);						\
    }
    VARS_ITER_MACRO_COND(FITER, out, 0, pad, true, includeSkipped,
			 skip_keys, key_aliases, true);
#undef FITER
    return out;
}
std::ostream& Variables::dump(const MODULE& module,
                              const PARAM_TYPE& param_type,
                              std::ostream& out, std::size_t pad,
                              const bool includeSkipped,
                              const std::vector<MODULE>& skip_modules,
                              const std::vector<PARAM_TYPE>& skip_param_types,
                              const std::vector<std::string>& skip_keys,
                              const std::map<std::string, std::string>& key_aliases,
                              const std::map<MODULE, const ValueSet_t*>& conditions) const {
    if (!isSelected(module, param_type)) {
	return out;
    }
    if (!skip_modules.empty()) {
      for (std::vector<MODULE>::const_iterator it = skip_modules.begin();
	   it != skip_modules.end(); it++)
	if (*it == module) return out;
    }
    if (!skip_param_types.empty()) {
      for (std::vector<PARAM_TYPE>::const_iterator it = skip_param_types.begin();
	   it != skip_param_types.end(); it++)
	if (*it == param_type) return out;
    }
    if (param_type == PARAM_TYPE_COND) {
      std::map<MODULE, const ValueSet_t*>::const_iterator it = conditions.find(module);
      if (it != conditions.end())
	return it->second->print(out, 0, pad, true, includeSkipped,
				 skip_keys, key_aliases, true);
    }
    return GET_VALUE_SET(module, param_type)->print(out, 0, pad, true, includeSkipped, skip_keys, key_aliases, true);
}

void Variables::initParam(const MODULE& mod, const PARAM_TYPE& pt,
			  const bool noDefaults, const bool force,
			  ValueSet_t* value_set) {
    GET_VALUE_SET_CLASS(mod, pt)->initDefaults(useC3);
    if (!value_set) value_set = GET_VALUE_SET(mod, pt);
    value_set->initValues(noDefaults, force, true);
}
void Variables::initAllDefaults(const bool useC3) {
    VARS_ITER_ALL_CLASS(m, pt, initDefaults, useC3, "", true);
}
void Variables::initDefaults(const MODULE& mod,
			     const PARAM_TYPE& pt,
			     const bool useC3,
			     const std::string& filename,
			     const bool force) {
    if ((!force) && !isSelected(mod, pt)) {
      if (!filename.empty()) {
	INFO_VALUE_SET(utils::enum_key2string(mod), "[",
		       utils::enum_key2string(pt),
		       "] value set not selected, but file \"",
		       filename, "\" provided containing defaults. The "
		       "values in the file will not be used.");
      } else {
	INFO_VALUE_SET(utils::enum_key2string(mod), "[",
		       utils::enum_key2string(pt),
		       "] value set not selected, the defaults will not "
		       "be initialized");
      }
      return;
    }
    GET_VALUE_SET_CLASS(mod, pt)->initDefaults(useC3, filename);
}

bool Variables::isSelected(const MODULE& mod, const PARAM_TYPE& pt) {
    return GET_VALUE_SET_CLASS(mod, pt)->selected();
}

std::size_t Variables::max_field_width_all() {
    static std::size_t pad = 0;
    if (pad == 0) {
#define MAX_WIDTH(V, dummy)					\
	std::size_t ipad = V->max_field_width(true);		\
	if (ipad > pad) pad = ipad
	VARS_ITER_MACRO_CLASS(MAX_WIDTH, )
#undef MAX_WIDTH
    }
    return pad;
}
std::size_t Variables::max_value_width_all() const {
    static std::size_t pad = 0;
    if (pad == 0) {
	std::size_t ipad = 0;
	for (std::vector<MODULE>::const_iterator m = ALL_MODULE.begin();
	     m != ALL_MODULE.end(); m++) {
	    if (*m == MODULE_ALL) continue;
	    ipad = GET_VALUE_SET_CLASS(*m, PARAM_TYPE_COND)->max_default_value_width();
	    if (ipad > pad) pad = ipad;
	}
#define MAX_WIDTH(V, dummy)					\
	ipad = V->max_value_width();				\
	if (ipad > pad) pad = ipad
	VARS_ITER_MACRO(MAX_WIDTH, );
#undef MAX_WIDTH
    }
    return pad;
}

std::string Variables::parseVar(const std::string& k,
				MODULE& mod, PARAM_TYPE& pt,
				const bool& isGlymaID,
				const bool& use_1st_match,
                                const bool& allow_no_match) {
    mod = MODULE_NONE;
    pt = PARAM_TYPE_NONE;
    std::string split="::", var1, var2, name;
    bool var1_used = false, var2_used = false;
    size_t idx1 = k.find(split);
    if (idx1 != std::string::npos) {
	var1 = k.substr(0, idx1);
	try {
	    mod = utils::enum_string2key<MODULE>(var1);
	    var1_used = true;
	} catch (...) {
	    try {
		pt = utils::enum_string2key<PARAM_TYPE>(var1);
		var1_used = true;
	    } catch (...) {
	    }
	}
	size_t idx2 = k.find(split, idx1 + split.size());
	if (idx2 == std::string::npos) {
	    name = k.substr(idx1 + split.size());
	} else {
	    var2 = k.substr(idx1 + split.size(),
			    idx2 - (idx1 + split.size()));
	    if (mod == MODULE_NONE) {
		try {
		    mod = utils::enum_string2key<MODULE>(var2);
		    var2_used = true;
		} catch (...) {
		}
	    }
	    if (pt == PARAM_TYPE_NONE && !var2_used) {
		try {
		    pt = utils::enum_string2key<PARAM_TYPE>(var2);
		    var2_used = true;
		} catch (...) {
		}
	    }
	    name = k.substr(idx2 + split.size());
	}
    } else {
        name = k;
    }
    if (mod == MODULE_NONE || pt == PARAM_TYPE_NONE) {
	std::vector<MODULE> check_modules;
	std::vector<PARAM_TYPE> check_param_types;
	if (mod != MODULE_NONE)
	  check_modules.push_back(mod);
	else
	  check_modules = ALL_MODULE;
	if (pt != PARAM_TYPE_NONE)
	  check_param_types.push_back(pt);
	else
	  check_param_types = ALL_PARAM_TYPE;
	std::vector<std::pair<MODULE, PARAM_TYPE> > matches;
	for (std::vector<MODULE>::const_iterator it_mod = check_modules.begin();
	     it_mod != check_modules.end(); it_mod++) {
	    if (use_1st_match && !matches.empty())
		break;
	    for (std::vector<PARAM_TYPE>::const_iterator it_pt = check_param_types.begin();
		 it_pt != check_param_types.end(); it_pt++) {
                if ((((*it_mod) == MODULE_ALL) || ((*it_pt) == PARAM_TYPE_VARS)) &&
                    (((*it_mod) != MODULE_ALL) || ((*it_pt) != PARAM_TYPE_VARS)))
                    continue;
		if (use_1st_match && !matches.empty())
		    break;
		if (hasVar(*it_mod, *it_pt, name, isGlymaID)) {
		    if (!isSelected(*it_mod, *it_pt)) {
			INFO_VALUE_SET("Variable \"", k, "\" matches ",
				       name, " in ",
				       utils::enum_key2string(*it_mod),
				       " ",
				       utils::enum_key2string(*it_pt),
				       " value set, but it is not selected by the current driver");
		    } else {
			matches.emplace_back(*it_mod, *it_pt);
		    }
		}
	    }
	}
	if (matches.size() == 1) {
	    mod = matches[0].first;
	    pt = matches[0].second;
	} else if (matches.size() > 1) {
	    std::stringstream ss;
	    for (std::vector<std::pair<MODULE, PARAM_TYPE> >::const_iterator it = matches.begin();
		 it != matches.end(); it++) {
		if (ss.tellp() > 0)
		  ss << ", ";
		ss << it->first << "::" << it->second << "::" << name;
	    }
	    ERROR_VALUE_SET("More than one (", matches.size(),
			    ") variable matches \"", k,
			    "\". Use a module & parameter type prefix to disambiguate [matches = ",
			    ss.str(), "]");
	} else {
            if (allow_no_match)
                return "";
	    ERROR_VALUE_SET("Could not find variable matching string \"",
			    k, "\"");
	}
    }
    DEBUG_VALUE_SET("parseVar[", k, ", ", mod, ", ", pt, ", ", name);
    if (mod != MODULE_NONE && pt != PARAM_TYPE_NONE && !name.empty())
        return GET_VALUE_SET_CLASS(mod, pt)->getAliasedName(name);
    return name;
}

bool Variables::hasVar(const MODULE& mod, const PARAM_TYPE& pt,
		       const std::string& name,
		       const bool& isGlymaID) {
    ValueSetClass_t* vs = getValueSetClass(mod, pt, true);
    if (!vs) return false;
    return vs->has(name, isGlymaID);
}
bool Variables::hasVar(const std::string& k,
		       const bool& isGlymaID) {
    std::string name;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    name = parseVar(k, mod, pt, isGlymaID, false, true);
    if (name.empty())
      return false;
    return hasVar(mod, pt, name, isGlymaID);
}
int Variables::getKey(const MODULE& mod, const PARAM_TYPE& pt,
		      const std::string& name,
		      const bool& isGlymaID) {
    return GET_VALUE_SET_CLASS(mod, pt)->fromNameWithAliases(name, isGlymaID);
}
double Variables::getDefault(const MODULE& mod, const PARAM_TYPE& pt,
                             const std::string& name,
                             const bool& isGlymaID) {
    if (mod == MODULE_ALL && pt == PARAM_TYPE_VARS)
      return ValueSetClass::getDefault(name, isGlymaID);
    return GET_VALUE_SET_CLASS(mod, pt)->getDefault(name, isGlymaID);
}
double Variables::getDefault(const MODULE& mod, const PARAM_TYPE& pt,
                             const int& key) {
    return GET_VALUE_SET_CLASS(mod, pt)->getDefault(key);
}
double Variables::getDefault(const std::string& k,
                             const bool& isGlymaID) {
    std::string name;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    name = parseVar(k, mod, pt, isGlymaID);
    return getDefault(mod, pt, name, isGlymaID);
}
void Variables::setDefault(const MODULE& mod, const PARAM_TYPE& pt,
			   const std::string& name, const double& value,
			   const bool& isGlymaID) {
    GET_VALUE_SET_CLASS(mod, pt)->setDefault(name, value, isGlymaID);
}
void Variables::setDefault(const MODULE& mod, const PARAM_TYPE& pt,
			   const int& key, const double& value) {
    GET_VALUE_SET_CLASS(mod, pt)->setDefault(key, value);
}
void Variables::setDefault(const std::string& k, const double& value,
			   const bool& isGlymaID) {
    std::string name;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    name = parseVar(k, mod, pt, isGlymaID);
    return setDefault(mod, pt, name, value, isGlymaID);
}
void Variables::setVar(const MODULE& module,
		       const PARAM_TYPE& param_type,
		       const std::string& name,
		       const double& value,
		       const bool& isGlymaID) {
    GET_VALUE_SET(module, param_type)->set(name, value, isGlymaID);
}
void Variables::setVar(const MODULE& module,
		       const PARAM_TYPE& param_type,
		       const int& key, const double& value) {
    GET_VALUE_SET(module, param_type)->set(key, value);
}
void Variables::setVar(const std::string& k, const double& value,
		       const bool& isGlymaID) {
    std::string name;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    name = parseVar(k, mod, pt, isGlymaID);
    return setVar(mod, pt, name, value, isGlymaID);
}
double Variables::getVar(const MODULE& module,
			 const PARAM_TYPE& param_type,
			 const std::string& name,
			 const bool& isGlymaID) const {
    return GET_VALUE_SET(module, param_type)->get(name, isGlymaID);
}
double Variables::getVar(const MODULE& module,
			 const PARAM_TYPE& param_type,
			 const int& key) const {
    return GET_VALUE_SET(module, param_type)->get(key);
}
double Variables::getVar(const std::string& k,
			 const bool& isGlymaID) const {
    std::string name;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    name = parseVar(k, mod, pt, isGlymaID);
    return getVar(mod, pt, name, isGlymaID);
}
std::vector<PARAM_TYPE> Variables::getParamTypes(const MODULE& mod,
						 const bool for_instance,
						 const bool include_cond,
						 const std::map<MODULE, const ValueSet_t*>& conditions) {
    std::vector<PARAM_TYPE> out;
    ValueSetClass_t* module = getModule(mod, false, "getParamTypes: ");
    const std::vector<PARAM_TYPE>& all = module->get_parameter_types();
    std::map<MODULE, const ValueSet_t*>::const_iterator itCond = conditions.find(mod);
    if (for_instance && ((!include_cond) ||
			 (include_cond &&
			  (!VARS_INST_IS_MODULE(mod)) &&
			  (itCond == conditions.end())))) {
	for (typename std::vector<PARAM_TYPE>::const_iterator it = all.begin();
	     it != all.end(); it++) {
	    if (*it != PARAM_TYPE_COND)
	      out.push_back(*it);
	}
    } else {
	out.insert(out.begin(), all.begin(), all.end());
    }
    return out;
}
ValueSetClass_t* Variables::getModule(const MODULE& mod,
				      const bool no_error_on_invalid,
				      const std::string& error_context) {
    return getValueSetClass(mod, PARAM_TYPE_MOD, no_error_on_invalid,
			    error_context);
}
ValueSetClass_t* Variables::getValueSetClass(const MODULE& mod,
					     const PARAM_TYPE& pt,
					     const bool no_error_on_invalid,
					     const std::string& error_context) {
    bool force_error = false;
    if (mod == MODULE_ALL || pt == PARAM_TYPE_VARS) {
	if (mod == MODULE_ALL && pt == PARAM_TYPE_VARS) {
	    return getValueSetClass();
	}
	force_error = true;
    } else if (pt == PARAM_TYPE_MOD) {
	SWITCH_MOD(mod, VARS_CLASS_MODULES, VARS_RETURN_CALL,
		   VARS_CLASS_CALL_STATIC,
		   getValueSetClass, ());
    } else {
	SWITCH_MOD_AND_PT(mod, VARS_CLASS_MODULES,
			  pt, MOD2PT, VARS_RETURN_CALL, VARS_CLASS_CALL,
			  getValueSetClass, ());
    }
    if ((!no_error_on_invalid) || force_error) {
      ERROR_VALUE_SET(error_context, "Invalid combination of module (",
		      utils::enum_key2string(mod),
		      ") and parameter type (",
		      utils::enum_key2string(pt), ")");
    }
    return nullptr;
}
ValueSet_t* Variables::getValueSet(const MODULE& mod,
				   const PARAM_TYPE& pt,
				   const bool no_error_on_invalid,
				   const std::map<MODULE, ValueSet_t*>& conditions,
				   const std::string& error_context) {
    std::map<MODULE, const ValueSet_t*> conditionsC;
    for (std::map<MODULE, ValueSet_t*>::const_iterator it = conditions.begin();
	 it != conditions.end(); it++)
      conditionsC[it->first] = it->second;
    return const_cast<ValueSet_t*>(const_cast<const Variables*>(this)->getValueSet(mod, pt, no_error_on_invalid, conditionsC, error_context));
}
const ValueSet_t* Variables::getValueSet(const MODULE& mod,
					 const PARAM_TYPE& pt,
					 const bool no_error_on_invalid,
					 const std::map<MODULE, const ValueSet_t*>& conditions,
					 const std::string& error_context) const {
    bool force_error = false;
    if (mod == MODULE_ALL || pt == PARAM_TYPE_VARS) {
	if (mod == MODULE_ALL && pt == PARAM_TYPE_VARS) {
	    return getValueSet();
	}
	force_error = true;
    } else if (pt == PARAM_TYPE_MOD) {
	SWITCH_MOD(mod, VARS_CLASS_MODULES, VARS_RETURN_CALL,
		   VARS_INST_CALL_STATIC,
		   getValueSet, ());
    } else if (pt == PARAM_TYPE_COND) {
	std::map<MODULE, const ValueSet_t*>::const_iterator it = conditions.find(mod);
	if (it != conditions.end())
	  return it->second;
	SWITCH_MOD_AND_PT(mod, VARS_INST_MODULES,
			  pt, MOD2PT_COND, VARS_RETURN_CALL,
			  VARS_INST_CALL,
			  getValueSet, ());
    } else {
	SWITCH_MOD_AND_PT(mod, VARS_CLASS_MODULES,
			  pt, MOD2PT_NO_COND, VARS_RETURN_CALL,
			  VARS_INST_CALL,
			  getValueSet, ());
	// SWITCH_MOD_AND_PT_PAIRS(mod, pt, (INST_MOD_PT_PAIRS),
	//        VARS_RETURN_CALL, VARS_INST_CALL,
	//        getValueSet, ());
    }
    if ((!no_error_on_invalid) || force_error)
      ERROR_VALUE_SET(error_context, "Invalid combination of module (",
		      utils::enum_key2string(mod),
		      ") and parameter type (",
		      utils::enum_key2string(pt), ")");
    return nullptr;
}
void Variables::setRecord(const ValueSet_t* x,
			  std::map<MODULE, const ValueSet_t*>& conditions) {
    MODULE m = x->_virtual_get_module();
    PARAM_TYPE pt = x->_virtual_get_param_type();
    if (pt == PARAM_TYPE_MOD) return;
    const std::vector<ValueSet_t**>& children = x->getChildren();
    if (children.size() > 0) {
	conditions[m] = x;
    }
    for (typename std::vector<ValueSet_t**>::const_iterator it = children.begin();
	 it != children.end(); it++) {
	setRecord(**it, conditions);
    }
#define DO_ASSIGN(dummy, mod, pt)					\
    VARS_INST_VAR(mod, pt) = *static_cast<const VARS_CLASS_VAR(mod, pt)*>(x)
    SWITCH_MOD_AND_PT_PAIRS(m, pt, (INST_MOD_PT_PAIRS), DO_ASSIGN, );
#undef DO_ASSIGN
}

void Variables::setRecord(const ValueSet_t* x) {
    std::map<MODULE, const ValueSet_t*> conditions;
    setRecord(x, conditions);
}
void Variables::setRecord(ValueSet_t* x,
			  std::map<MODULE, ValueSet_t*>& conditions) {
    std::map<MODULE, const ValueSet_t*> conditionsC;
    setRecord(const_cast<const ValueSet_t*>(x), conditionsC);
    for (std::map<MODULE, const ValueSet_t*>::iterator it = conditionsC.begin();
	 it != conditionsC.end(); it++) {
	conditions[it->first] = const_cast<ValueSet_t*>(it->second);
    }
}

void Variables::_readParam(const std::string& fname,
                           std::map<std::string, std::string>& inputs,
                           Variables* theVars,
                           const std::string& context) {
    if (fname.empty())
        return;
    std::cout << "PARAMETER FILE PROVIDED: " << fname << std::endl;
#define convD(x) static_cast<double>(std::stof(x, nullptr))
#define convI(x) std::stoi(x, nullptr)
#define convB(x) ((bool)(std::stoi(x, nullptr)))
#define CASE_I(mod_, pt_, src, dst)                                     \
    if (it->first == #src || it->first == (#mod_ "::" #pt_ "::" #src) || \
        it->first == #dst || it->first == (#mod_ "::" #pt_ "::" #dst)) { \
        name = #dst;                                                    \
        valueI = convI(it->second);                                     \
        mod = MODULE_ ## mod_;                                          \
        pt = PARAM_TYPE_ ## pt_;                                        \
    }
#define ASSIGN_I(mod_, pt_, src, dst)                                   \
    if (name == #dst) {                                                 \
        if (theVars) {                                                  \
            theVars-> dst = valueI;                                     \
        } else {                                                        \
            throw std::runtime_error("Cannot set a default for the parameter \"" #dst "\""); \
        }                                                               \
    }
#define ASSIGN_MODB(mod_, pt_, src, dst)                                \
    if (name == #dst) {                                                 \
      modules::mod_::set ## dst(valueI);                                \
    }
    readFile(fname, inputs);
    std::map<std::string, std::string> add_values;
    std::vector<std::string> rm_values;
    MODULE mod = MODULE_NONE;
    PARAM_TYPE pt = PARAM_TYPE_NONE;
    std::string name, name_SET, name_FULL;
    double valueD = 0.0;
    int valueI = 0;
    static std::string set_suffix = "_SETXXX";

    for (typename std::map<std::string, std::string>::const_iterator it = inputs.begin();
         it != inputs.end(); it++) {
        if (__endswith(it->first, set_suffix))
            continue;
        mod = MODULE_NONE;
        pt = PARAM_TYPE_NONE;
        CASE_I(ALL, VARS, GRNC, GRNC)
        else CASE_I(ALL, VARS, PAR_in_Wpm2, PAR_in_Wpm2)
        else CASE_I(CM, MOD, SucPath, TestSucPath)
        else {
            name = parseVar(it->first, mod, pt, false, false, true);
            if (name.empty()) {
                std::cout << context << ": String \"" << it->first <<
                  "\" does not match any known parameters." << std::endl;
                continue;
            }
            valueD = convD(it->second);
        }
        name_FULL = utils::enum_key2string(mod) + "::" +
            utils::enum_key2string(pt) + "::" + name;
        name_SET = name_FULL + set_suffix;
        if (inputs.find(name_SET) == inputs.end()) {
            std::cout << context << ": Parameter \"" << name_FULL <<
                "\" (" << it->second << ") read from \"" << it->first <<
                "\"" << std::endl;
            add_values[name_SET] = it->second;
            ASSIGN_I(ALL, VARS, GRNC, GRNC)
            else ASSIGN_I(ALL, VARS, PAR_in_Wpm2, PAR_in_Wpm2)
            else ASSIGN_MODB(CM, MOD, SucPath, TestSucPath)
            else if (theVars) {
                theVars->setVar(mod, pt, name, valueD);
            } else {
                setDefault(mod, pt, name, valueD);
            }
        } else {
            std::cout << context << ": Parameter \"" << name_FULL <<
                "\" already set. Using previous value." << std::endl <<
                "    Previous value :    " << inputs.at(name_SET) << std::endl <<
                "    Discarded value:    " << it->second << std::endl;
            rm_values.push_back(name);
        }
        rm_values.push_back(it->first);
    }
    for (typename std::vector<std::string>::iterator it = rm_values.begin();
         it != rm_values.end(); it++) {
        inputs.erase(*it);
    }
    inputs.insert(add_values.begin(), add_values.end());
#undef convD
#undef convI
#undef convB
#undef CASE_I
#undef ASSIGN_I
#undef ASSIGN_MODB
}
void Variables::readEnzymeAct(const std::string& fname) {
    if (fname.empty())
      return;
    std::cout << "ENZYME DATA PROVIDED: " << fname << std::endl;
    readFile(fname, EnzymeAct, true);
}
void Variables::readGRN(const std::string& fname) {
    if (fname.empty())
      return;
    // Read the GRN data and assign it into the correct positions
    // based on the expected order
    std::cout << "GRN DATA PROVIDED: " << fname << std::endl;
    std::map<std::string, double> glymaID_map;
    readFile(fname, glymaID_map);
    double pcfactor = 1.0 / this->ProteinTotalRatio;
    size_t i = 0;
    for (auto it = glymaID_order.begin(); it != glymaID_order.end(); it ++, i++) {
        double iVfactor = pcfactor;
        try {
            if ((i >= 33) && (this->GRNC == 0))
              iVfactor = 1.0;
            else
              iVfactor = pcfactor * glymaID_map.at(*it);
        } catch (const std::out_of_range&) {
            // Do nothing
            std::cerr << "GlymaID \"" << *it << "\" not present." << std::endl;
        }
        if (i < 33) {
            this->VfactorCp[i] = iVfactor;
        } else if (i == 33) {
            modules::BF::setcATPsyn(iVfactor);
        } else if (i == 34) {
            modules::BF::setCPSi(iVfactor);
        } else if (i == 35) {
            modules::FI::setcpsii(iVfactor);
        } else if (i == 36) {
            modules::BF::setcNADPHsyn(iVfactor);
        } else {
            throw std::runtime_error(std::string("More GlymaIDs than expected "
                                                 " (expected <= 37, provided ")
                                     + std::to_string(glymaID_order.size())
                                     + ")");
        }
    }
}

void Variables::readDefaults(const std::string& fname) {
    std::map<std::string, std::string> inputs;
    _readParam(fname, inputs, nullptr, "readDefaults");
}
void Variables::readDefaults(const std::string& fname,
                             std::map<std::string, std::string>& inputs) {
    _readParam(fname, inputs, nullptr, "readDefaults");
}
void Variables::readParam(const std::string& fname) {
    std::map<std::string, std::string> inputs;
    _readParam(fname, inputs, this, "readParam");
}
void Variables::readParam(const std::string& fname,
                          std::map<std::string, std::string>& inputs) {
    _readParam(fname, inputs, this, "readParam");
}
