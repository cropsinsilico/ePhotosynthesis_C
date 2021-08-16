#include "Variables.hpp"

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

Variables& Variables::operator=(const Variables *other) {
    *this = *other;
    return *this;
}
