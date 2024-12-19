#include <cstdlib>
#include <string>
#include <stdexcept>
#include "Variables.hpp"
#include "modules/CM.hpp"
#include "modules/BF.hpp"
#include "modules/FI.hpp"
#include "globals.hpp"


#ifdef SUNDIALS_CONTEXT_REQUIRED
std::shared_ptr<SUNContext> ePhotosynthesis::global_context(NULL);

void ePhotosynthesis::init_global_sundials_context() {
    if (!global_context) {
        SUNContext* context = new SUNContext();
        SUNErrCode error_code = SUNContext_Create(SUN_COMM_NULL, context);
        if (error_code) {
            throw std::runtime_error("SUNContext_Create failed: " +
                                     std::string(SUNGetErrMsg(error_code)));
        }
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

#ifdef SUNDIALS_CONTEXT_REQUIRED

Variables::Variables(SUNContext* ctx) : _context(), _context_flags(0) {
    if (_context == nullptr) {
        init_global_sundials_context();
        _context = global_context;
        _context_flags |= CONTEXT_FLAG_GLOBAL;
    } else {
        _context.reset(ctx);
    }
}
Variables::Variables() : Variables((SUNContext*)nullptr) {}
Variables::Variables(const Variables& other) :
  _context(other._context), _context_flags(other._context_flags) {
    *this = other;
}
Variables::Variables(const Variables* other) :
  _context(other->_context), _context_flags(other->_context_flags) {
    *this = *other;
}
#else // SUNDIALS_CONTEXT_REQUIRED
Variables::Variables() {}
Variables::Variables(const Variables& other) : Variables() {
    *this = other;
}
Variables::Variables(const Variables* other) : Variables() {
    *this = *other;
}
#endif // SUNDIALS_CONTEXT_REQUIRED

Variables* Variables::deepcopy() const {
  Variables* out = new Variables(*this);
  out->alfa = this->alfa;
  out->fc = this->fc;
  out->Phi_max = this->Phi_max;
  out->sensitivity_sf = this->sensitivity_sf;
  out->lightParam = this->lightParam;
  out->PR_Param = this->PR_Param;
  out->BF_Param = this->BF_Param;
  out->FI_PARAM = this->FI_PARAM;
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
#ifdef SUNDIALS_CONTEXT_REQUIRED
    _context = other._context;
    _context_flags = other._context_flags;
#endif // SUNDIALS_CONTEXT_REQUIRED
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
    ProteinTotalRatio = other.ProteinTotalRatio;
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

void Variables::readParam(const std::string& fname) {
    std::map<std::string, std::string> inputs;
    readParam(fname, inputs);
}
void Variables::readParam(const std::string& fname,
                          std::map<std::string, std::string>& inputs) {
    if (fname.empty())
      return;
    std::cout << "PARAMETER FILE PROVIDED: " << fname << std::endl;
#define convD(x) static_cast<double>(std::stof(x, nullptr))
#define convI(x) std::stoi(x, nullptr)
#define convB(x) ((bool)(std::stoi(x, nullptr)))
#define setInputVar(dst, x, dummy) this-> dst = x
#define setModVar(dst, x, mod) modules::mod::set ## dst(x)
#define assign(src, dst, set, conv, ...)                                \
    {                                                                   \
      std::string ikey, ival;                                           \
      if (inputs.count(#src) > 0) {                                     \
          ikey = #src;                                                  \
      } else if (inputs.count(#dst) > 0) {                              \
          ikey = #dst;                                                  \
      }                                                                 \
      if (!ikey.empty()) {                                              \
          ival = inputs.at(ikey);                                       \
          if (inputs.count(#dst "_SET") > 0) {                          \
              if (ival != inputs.at(#dst "_SET")) {                     \
                  std::cout << "readParam: " <<                         \
                    "Parameter \"" << #dst << "\" already set." <<      \
                    " Using previous value." << std::endl <<            \
                    "    Previous value :    " <<                       \
                    inputs.at(#dst "_SET") << std::endl <<              \
                    "    Discarded value:    " << ival << std::endl;    \
              }                                                         \
              inputs.erase(ikey);                                       \
          } else {                                                      \
              set(dst, conv(inputs.at(#src)), __VA_ARGS__);             \
              std::cout << "readParam: " <<                             \
                "Parameter \"" << #dst << "\" (" <<                     \
                ival << ") read from \"" << ikey <<                     \
                "\"" << std::endl;                                      \
              inputs[#dst "_SET"] = ival;                               \
          }                                                             \
      }                                                                 \
}
#define assignInputVar(src, dst, conv)                                  \
    assign(src, dst, setInputVar, conv, )
#define assignModVar(src, mod, dst, conv)                               \
    assign(src, dst, setModVar, conv, mod)
#define assignInputVarD(src, dst) assignInputVar(src, dst, convD)
#define assignInputVarI(src, dst) assignInputVar(src, dst, convI)
#define setInputVarB(src, mod, dst) assignModVar(src, mod, dst, convB)
    readFile(fname, inputs);
    assignInputVarD(CO2, CO2_in);
    assignInputVarD(PAR, TestLi);
    assignInputVarD(ATPCost, TestATPCost);
    assignInputVarD(WeatherTemperature, Tp);
    assignInputVarD(ProteinTotalRatio, ProteinTotalRatio);
    assignInputVarI(GRNC, GRNC);
    setInputVarB(SucPath, CM, TestSucPath);
#undef setInputVarB
#undef assignInputVarD
#undef assignInputVarI
#undef assignModVar
#undef assignInputVar
#undef assign
#undef setInputVar
#undef setModVar
#undef convD
#undef convI
#undef convB
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
