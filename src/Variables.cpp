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
    files = other.files;
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
    return out;
}

double Variables::getVar(const MODULE& module,
			 const PARAM_TYPE& param_type,
			 const std::string& name) const {
#define ADD_SPEC(var, mod, pt)						\
    if (module == MODULE_ ## mod && param_type == PARAM_TYPE_ ## pt) {	\
	return var.get(name);						\
    }
#define ADD_SPEC_STATIC(var, mod, pt)					\
    if (module == MODULE_ ## mod && param_type == PARAM_TYPE_ ## pt) {	\
	return var::get(name);						\
    }
#define ADD_SPEC_VALUE_SET(mod, var_suffix, pt)	\
    ADD_SPEC(mod ## var_suffix, mod, pt)
#define ADD_SPEC_MOD(mod)						\
    ADD_SPEC_VALUE_SET(mod, _Pool, POOL)				\
    else ADD_SPEC_VALUE_SET(mod, _RC, RC)				\
    else ADD_SPEC_VALUE_SET(mod, _KE, KE)				\
    else ADD_SPEC(mod ## _VEL.getLastData(), mod, VEL)		\
    else ADD_SPEC_STATIC(modules::mod, mod, MOD)
  
    ADD_SPEC_MOD(BF)
    else ADD_SPEC_MOD(FI)
    else ADD_SPEC_MOD(PR)
    else ADD_SPEC_MOD(PS)
    else ADD_SPEC_MOD(RROEA)
    else ADD_SPEC_MOD(RedoxReg)
    else ADD_SPEC_MOD(RuACT)
    else ADD_SPEC_MOD(SUCS)
    else ADD_SPEC_MOD(XanCycle)
    // else ADD_SPEC_MOD(FIBF)
    else ADD_SPEC_VALUE_SET(FIBF, _Pool, POOL)
#undef ADD_SPEC_MOD
#undef ADD_SPEC_VALUE_SET
#undef ADD_SPEC_STATIC
#undef ADD_SPEC
    // else if (module == MODULE_NONE && PARAM_TYPE == PARAM_TYPE_NONE) {
    // 	return get(name);
    else {
        throw std::runtime_error("Invalid MODULE (" + get_enum_name(module) + ") "
				 + ", PARAM_TYPE (" + get_enum_name(param_type) + ") "
				 + "or name (" + name + ")");
    }
}

