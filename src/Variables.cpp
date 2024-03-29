#include "Variables.hpp"

using namespace ePhotosynthesis;

Variables::Variables(const Variables& other) : Variables() {
    *this = other;
}

Variables::Variables(const Variables* other) : Variables() {
    *this = *other;
}

Variables& Variables::operator=(const Variables &other) {
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
    RacRatio = other.RacRatio;
    SUCRatio = other.SUCRatio;
    XanRatio = other.XanRatio;
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
    out << "FI_PARAM = ";
for (auto i : in->FI_PARAM)
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
    out << "RacRatio = ";
for (auto i : in->RacRatio)
    out << i << ", ";
out << std::endl;
    out << "SUCRatio = ";
for (auto i : in->SUCRatio)
    out << i << ", ";
out << std::endl;
    out << "XanRatio = ";
for (auto i : in->XanRatio)
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
