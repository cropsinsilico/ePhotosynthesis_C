#include "Variables.hpp"
#include "modules/ModuleAll.hpp"

using namespace ePhotosynthesis;

Variables::Variables(const Variables& other) : Variables(other.context) {
    *this = other;
}

Variables::Variables(const Variables* other) : Variables(other->context) {
    *this = *other;
}

Variables* Variables::deepcopy() const {
  Variables* out = new Variables(*this);
  out->alfa = this->alfa;
  out->fc = this->fc;
  out->lightParam = this->lightParam;
  out->PR_Param = this->PR_Param;
  out->BF_Param = this->BF_Param;
  out->FI_Param = this->FI_Param;
  out->RROEA_Param = this->RROEA_Param;
  out->RuACT_Param = this->RuACT_Param;
  out->SUCS_Param = this->SUCS_Param;
  out->XanCycle_Param = this->XanCycle_Param;
  out->BF_Vel = this->BF_Vel;
  out->FI_Vel = this->FI_Vel;
  out->PR_Vel = this->PR_Vel;
  out->PS_Vel = this->PS_Vel;
  out->RROEA_Vel = this->RROEA_Vel;
  out->RedoxReg_Vel = this->RedoxReg_Vel;
  out->RuACT_Vel = this->RuACT_Vel;
  out->SUCS_Vel = this->SUCS_Vel;
  out->XanCycle_Vel = this->XanCycle_Vel;
  out->BF_VEL = this->BF_VEL;
  out->CO2A = this->CO2A;
  out->FI_VEL = this->FI_VEL;
  out->PR_VEL = this->PR_VEL;
  out->PS_VEL = this->PS_VEL;
  out->RROEA_VEL = this->RROEA_VEL;
  out->RedoxReg_MP = this->RedoxReg_MP;
  out->RedoxReg_VEL = this->RedoxReg_VEL;
  out->RuACT_VEL = this->RuACT_VEL;
  out->SUCS_VEL = this->SUCS_VEL;
  out->XanCycle_VEL = this->XanCycle_VEL;
  return out;
}

Variables& Variables::operator=(const Variables &other) {
    context = other.context;
    record = other.record;
    BF_FI_com = other.BF_FI_com;
    EPS_SUCS_com = other.EPS_SUCS_com;
    FIBF_PSPR_com = other.FIBF_PSPR_com;
    PR_PS_com = other.PR_PS_com;
    PSPR_SUCS_com = other.PSPR_SUCS_com;
    RROEA_EPS_com = other.RROEA_EPS_com;
    RedoxReg_RA_com = other.RedoxReg_RA_com;
    RuACT_EPS_com = other.RuACT_EPS_com;
    XanCycle_BF_com = other.XanCycle_BF_com;
    GP = other.GP;
    GRNC = other.GRNC;
    GRNT = other.GRNT;
    RUBISCOMETHOD = other.RUBISCOMETHOD;
    CO2_cond = other.CO2_cond;
    GLight = other.GLight;
    O2_cond = other.O2_cond;
    PS12ratio = other.PS12ratio;
    ADP = other.ADP;
    Pi = other.Pi;
    TestATPCost = other.TestATPCost;
    CO2_in = other.CO2_in;
    TestLi = other.TestLi;
    PS2BF_Pi = other.PS2BF_Pi;
    PS_PR_Param = other.PS_PR_Param;
    Tp = other.Tp;
    BFRatio = other.BFRatio;
    FIRatio = other.FIRatio;
    PRRatio = other.PRRatio;
    PSRatio = other.PSRatio;
    RuACTRatio = other.RuACTRatio;
    SUCSRatio = other.SUCSRatio;
    XanCycleRatio = other.XanCycleRatio;
    EnzymeAct = other.EnzymeAct;
    VfactorCp = other.VfactorCp;
    VfactorT = other.VfactorT;
    BF_Pool = other.BF_Pool;
    FIBF_Pool = other.FIBF_Pool;
    FI_Pool = other.FI_Pool;
    RROEA_Pool = other.RROEA_Pool;
    RuACT_Pool = other.RuACT_Pool;
    SUCS_Pool = other.SUCS_Pool;
    BF_RC = other.BF_RC;
    FI_RC = other.FI_RC;
    RROEA_RC = other.RROEA_RC;
    RuACT_RC = other.RuACT_RC;
    BF2OUT = other.BF2OUT;
    PR2OUT = other.PR2OUT;
    PS2OUT = other.PS2OUT;
    SUCS2OUT = other.SUCS2OUT;
    XanCycle2OUT = other.XanCycle2OUT;
    FluxTR = other.FluxTR;
    RROEA_KE = other.RROEA_KE;
    useC3 = other.useC3;
    return *this;
}

std::ostream& ePhotosynthesis::operator<<(std::ostream &out, const Variables *in) {
    out << "record = " << in->record << std::endl;
    out << "BF_FI_com = " << in->BF_FI_com << std::endl;
    out << "EPS_SUCS_com = " << in->EPS_SUCS_com << std::endl;
    out << "FIBF_PSPR_com = " << in->FIBF_PSPR_com << std::endl;
    out << "PR_PS_com = " << in->PR_PS_com << std::endl;
    out << "PSPR_SUCS_com = " << in->PSPR_SUCS_com << std::endl;
    out << "RROEA_EPS_com = " << in->RROEA_EPS_com << std::endl;
    out << "RedoxReg_RA_com = " << in->RedoxReg_RA_com << std::endl;
    out << "RuACT_EPS_com = " << in->RuACT_EPS_com << std::endl;
    out << "XanCycle_BF_com = " << in->XanCycle_BF_com << std::endl;

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
    out << "PR_Param = ";
for (auto i : in->PR_Param)
    out << i << ", ";
out << std::endl;
    out << "BF_Param = ";
for (auto i : in->BF_Param)
    out << i << ", ";
out << std::endl;
    out << "FI_Param = ";
for (auto i : in->FI_Param)
    out << i << ", ";
out << std::endl;
    out << "RROEA_Param = ";
for (auto i : in->RROEA_Param)
    out << i << ", ";
out << std::endl;
    out << "RuACT_Param = ";
for (auto i : in->RuACT_Param)
    out << i << ", ";
out << std::endl;
    out << "SUCS_Param = ";
for (auto i : in->SUCS_Param)
    out << i << ", ";
out << std::endl;
    out << "XanCycle_Param = ";
for (auto i : in->XanCycle_Param)
    out << i << ", ";
out << std::endl;

    // Vel
    out << in->BF_Vel;
    out << in->FI_Vel;
    out << in->PR_Vel;
    out << in->PS_Vel;
    out << in->RROEA_Vel;
    out << in->RedoxReg_Vel;
    out << in->RuACT_Vel;
    out << in->SUCS_Vel;
    out << in->XanCycle_Vel;

    // Ratio
    out << "BFRatio = ";
for (auto i : in->BFRatio)
    out << i << ", ";
out << std::endl;
    out << "FIRatio = ";
for (auto i : in->FIRatio)
    out << i << ", ";
out << std::endl;
    out << "PRRatio = ";
for (auto i : in->PRRatio)
    out << i << ", ";
out << std::endl;
    out << "PSRatio = ";
for (auto i : in->PSRatio)
    out << i << ", ";
out << std::endl;
    out << "RuACTRatio = ";
for (auto i : in->RuACTRatio)
    out << i << ", ";
out << std::endl;
    out << "SUCSRatio = ";
for (auto i : in->SUCSRatio)
    out << i << ", ";
out << std::endl;
    out << "XanCycleRatio = ";
for (auto i : in->XanCycleRatio)
    out << i << ", ";
out << std::endl;

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
#define OUTPUT_COND(name)			\
    out << #name "2OUT: " << std::endl << (in->name ## 2OUT) << std::endl
    OUTPUT_COND(BF);
    OUTPUT_COND(FI);
    OUTPUT_COND(PR);
    OUTPUT_COND(PS);
    OUTPUT_COND(RROEA);
    OUTPUT_COND(RedoxReg);
    OUTPUT_COND(RuACT);
    OUTPUT_COND(SUCS);
    OUTPUT_COND(XanCycle);
#undef OUTPUT_COND
    return out;
}


void Variables::initDefaults(const MODULE& module,
			     const PARAM_TYPE& param_type,
			     const bool useC3,
			     const std::string& filename) {
#define INIT_DEF(c3, fname, mod, pt)			\
  VARS_CLASS_VAR(mod, pt)::initDefaults(c3, fname);	\
  return
    VARS_CLASS_SWITCH(module, param_type, INIT_DEF, useC3, filename)
#undef INIT_DEF
  /*
#define ADD_SPEC(var, mod, pt)						\
    if (module == MODULE_ ## mod && param_type == PARAM_TYPE_ ## pt) {	\
	var::initDefaults(useC3, filename);				\
	return;								\
    }
#define ADD_SPEC_MOD(mod)						\
    ADD_SPEC(conditions::mod ## Condition, mod, COND)			\
    else ADD_SPEC(pool::mod ## Pool, mod, POOL)				\
    else ADD_SPEC(RC::mod ## RC, mod, RC)				\
    else ADD_SPEC(KE::mod ## KE, mod, KE)				\
    else ADD_SPEC(vel::mod ## Vel, mod, VEL)				\
    else ADD_SPEC(modules::mod, mod, MOD)
  
    ADD_SPEC_MOD(BF)
    else ADD_SPEC_MOD(CM)
    else ADD_SPEC_MOD(DynaPS)
    else ADD_SPEC_MOD(EPS)
    else ADD_SPEC_MOD(FIBF)
    else ADD_SPEC_MOD(FI)
    else ADD_SPEC_MOD(PR)
    else ADD_SPEC_MOD(PS)
    else ADD_SPEC_MOD(PS_PR)
    else ADD_SPEC_MOD(RA)
    else ADD_SPEC_MOD(RROEA)
    else ADD_SPEC_MOD(RedoxReg)
    else ADD_SPEC_MOD(RuACT)
    else ADD_SPEC_MOD(SUCS)
    else ADD_SPEC_MOD(XanCycle)
    else ADD_SPEC_MOD(trDynaPS)
#undef ADD_SPEC_MOD
#undef ADD_SPEC
    // else if (module == MODULE_NONE && PARAM_TYPE == PARAM_TYPE_NONE) {
    // 	initDefaults(useC3, filename);
    //  return;
    else {
        throw std::runtime_error("Invalid MODULE (" + get_enum_name(module) + ") "
				 + " or PARAM_TYPE (" + get_enum_name(param_type) + ")");
    }
  */
}



#define ADD_SPEC(var, mod, pt, body)					\
    if (module == MODULE_ ## mod && param_type == PARAM_TYPE_ ## pt) {	\
	body(var.);							\
    }
#define ADD_SPEC_STATIC(var, mod, pt, body)				\
    if (module == MODULE_ ## mod && param_type == PARAM_TYPE_ ## pt) {	\
	body(var::);							\
    }
#define ADD_SPEC_VALUE_SET(mod, var_suffix, pt, body)	\
    ADD_SPEC(mod ## var_suffix, mod, pt, body)
#define ADD_SPEC_MOD(mod, body)						\
    ADD_SPEC_VALUE_SET(mod, _Pool, POOL, body)				\
    else ADD_SPEC_VALUE_SET(mod, _RC, RC, body)				\
    else ADD_SPEC_VALUE_SET(mod, _KE, KE, body)			\
    else ADD_SPEC(mod ## _VEL.getLastData(), mod, VEL, body)	\
    else ADD_SPEC_STATIC(modules::mod, mod, MOD, body)
#define ADD_SPEC_ALL(body)				\
    ADD_SPEC_MOD(BF, body)				\
    else ADD_SPEC_MOD(FI, body)				\
    else ADD_SPEC_MOD(PR, body)				\
    else ADD_SPEC_MOD(PS, body)					\
    else ADD_SPEC_MOD(RROEA, body)					\
    else ADD_SPEC_MOD(RedoxReg, body)					\
    else ADD_SPEC_MOD(RuACT, body)					\
    else ADD_SPEC_MOD(SUCS, body)					\
    else ADD_SPEC_MOD(XanCycle, body)					\
    else ADD_SPEC_VALUE_SET(FIBF, _Pool, POOL, body)			\
    else {								\
	throw std::runtime_error("Invalid MODULE (" + get_enum_name(module) + ") " \
				 + ", PARAM_TYPE (" + get_enum_name(param_type) + ") " \
				 + "or name (" + name + ")");		\
    }
    // else ADD_SPEC_MOD(FIBF, body)

std::string Variables::parseVar(const std::string& k,
				MODULE& mod, PARAM_TYPE& pt) {
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
	     it_mod != ALL_MODULE.end(); it_mod++) {
	    for (std::vector<PARAM_TYPE>::const_iterator it_pt = check_param_types.begin();
		 it_pt != ALL_PARAM_TYPE.end(); it_pt++) {
		if (hasVar(*it_mod, *it_pt, name)) {
		    matches.emplace_back(*it_mod, *it_pt);
		    // mod = *it_mod;
		    // pt = *it_pt;
		    // name = k;
		    // break;
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
	    throw std::runtime_error("More than one (" + std::to_string(matches.size()) + ") variable matches \"" + k + "\". Use a module & parameter type prefix to disambiguate [matches = " + ss.str() + "]");
	} else {
	    throw std::runtime_error("Could not find variable matching string \"" + k + "\"");
	}
    }
    std::cerr << "parseVar[" << k << ", " << mod << ", " <<
      pt << ", " << name << std::endl;
    return name;
}

bool Variables::hasVar(const MODULE& mod, const PARAM_TYPE& pt,
		       const std::string& name) {
#define HAS_VAR(nme, mod, pt)			\
    return VARS_CLASS_VAR(mod, pt)::has(name)
    VARS_CLASS_SWITCH(mod, pt, HAS_VAR, name)
#undef HAS_VAR
    return false;
}
void Variables::setVar(const MODULE& module,
		       const PARAM_TYPE& param_type,
		       const std::string& name,
		       const double& value) {
#define SET_VAR(nme, val, mod, pt)		\
    VARS_INST_VAR(mod, pt).set(nme, val);	\
    return
#define SET_VAR_STATIC(nme, val, mod)			\
    modules::mod::set(nme, val);			\
    return
    VARS_INST_SWITCH(module, param_type, SET_VAR, name, value)
#undef SET_VAR
#undef SET_VAR_STATIC
}

double Variables::getVar(const MODULE& module,
			 const PARAM_TYPE& param_type,
			 const std::string& name) const {
#define GET_VAR(nme, mod, pt)			\
    return VARS_INST_VAR(mod, pt).get(nme)
#define GET_VAR_STATIC(nme, mod)		\
    return modules::mod::get(nme)
    VARS_INST_SWITCH(module, param_type, GET_VAR, name)
#undef GET_VAR
#undef GET_VAR_STATIC
}

#undef ADD_SPEC_MOD
#undef ADD_SPEC_VALUE_SET
#undef ADD_SPEC_STATIC
#undef ADD_SPEC
