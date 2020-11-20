#pragma once
//#include <boost/multi_array.hpp>
//#include <boost/smart_ptr/atomic_shared_ptr.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <math.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_types.h>

#define NV_Ith_S(v, i) (NV_DATA_S(v)[i])

#include "FI.hpp"
#include "BF.hpp"
#include "PS.hpp"
#include "PR.hpp"
#include "RROEA.hpp"
#include "RuACT.hpp"
#include "XanCycle.hpp"
#include "RedoxReg.hpp"
#include "SUCS.hpp"
#include "FIBF.hpp"

// static int f(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
// static int jtv(N_Vector v, N_Vector Jv, realtype t, N_Vector u, N_Vector fu,
// void *user_data, N_Vector tmp); static int check_flag(void *flagvalue, const
// char *funcname, int opt);

// typedef N_Vector myvec2;

struct UserData {
  std::vector<realtype> coeffs;
};

inline UserData *alloc_user_data() {
  UserData *data;
  data = new UserData();
  return data;
}

typedef std::vector<double> arr;
// typedef boost::multi_array<double, 2> Matrix;

inline arr ones(const size_t length) { return arr(length, 1.); }

inline arr zeros(const size_t length) { return arr(length, 0.); }

template <typename T>
class TimeSeries {
public:
  TimeSeries() {}

  void insert(size_t step, double time, T &input);

  T &operator[](size_t i) { return _data[i]; }

  double timestamp(int i = -1) {
    if (i < 0)
        return _timestamp[current];
    return _timestamp[i];
  }

  std::vector<double> getLastData() { return _data.back(); }

  double getLastTime() { return _timestamp.back(); }
  size_t size() { return _step.size(); }

  void write(std::ofstream &of);

private:
  size_t current = 0;
  std::vector<T> _data;
  std::vector<int> _step;
  std::vector<double> _timestamp;
};

template <typename T>
void TimeSeries<T>::insert(size_t step, double time, T &input) {
        std::vector<int>::iterator it = std::find(_step.begin(), _step.end(), step);
        T vec(input);
        if (it == _step.end()) {
            _step.push_back(step);
            _timestamp.push_back(time);
            _data.push_back(vec);
        } else {
            int index = std::distance(_step.begin(), it);
            _timestamp[index] = time;
            _data[index] = vec;
        }
    }

template <typename T>
void TimeSeries<T>::write(std::ofstream &of) {
        for (size_t i = 0; i < _step.size(); i++) {
            of << _timestamp[i];
            for (const auto &e : _data[i])
                of << "," << e;
            of << std::endl;
    }
}

const int CO2A_SIZE = 100;
const int FI_CON_SIZE = 6;
const int FI_VEL_SIZE = 59;
const int PR_VEL_SIZE = 11;
const int PS_VEL_SIZE = 20;
const int RROEA_CON_SIZE = 2;
const int RROEA_VEL_SIZE = 12;
const int RedoxReg_VEL_SIZE = 3;
const int RuACT_CON_SIZE = 4;
const int RuACT_VEL_SIZE = 7;
const int SUCS_CON_SIZE = 2;
const int SUCS_VEL_SIZE = 16;
const int XanCycle_CON_SIZE = 2;
const int XanCycle_VEL_SIZE = 8;

struct Variables {
    bool record = false;
    bool BF_FI_com = false;
    bool EPS_SUCS_com = false;
    bool FIBF_PSPR_com = false;
    bool PR_PS_com = false;
    bool PSPR_SUCS_com = false;
    bool RROEA_EPS_com = false;
    bool RedoxReg_RA_com = false;
    bool RuACT_EPS_com = false;
    bool StomCond_TrDynaPS_com = false;
    bool XanCycle_BF_com = false;

  int GP = 0;
  double BF_OLD_TIME = 0;
  size_t BF_TIME_N = 1;
  double FI_OLD_TIME = 0;
  size_t FI_TIME_N = 1;
  double PR_OLD_TIME = 0;
  size_t PR_TIME_N = 1;
  double PS_OLD_TIME = 0;
  size_t PS_TIME_N = 1;
  double PS_PR_OLDTIME = 0;
  size_t PS_PR_TIME_N = 1;
  double RROEA_OLD_TIME = 0;
  size_t RROEA_TIME_N = 1;
  double RedoxReg_OLD_TIME = 0;
  size_t RedoxReg_TIME_N = 1;
  double RuACT_OLD_TIME = 0;
  size_t RuACT_TIME_N = 1;
  double SUCS_OLD_TIME = 0;
  size_t SUCS_TIME_N = 1;
  double XanCycle_OLD_TIME = 0;
  size_t XanCycle_TIME_N = 1;

    double ADPc = 0.;
    double ATPActive = 0.;
    double ATPc = 0.;
    double AVR = 30;
    double BF2EPS_vbfn2 = 0.;
    double BF2FIBFMB_PHl = 0.;
    double BF2RROEA_FdT = 0.;
    double BF2RROEA_Fdn = 0.;
    double BF2RROEA_Vbf16 = 0.;
    double BF2RedoxReg_Fdt = 0.;
    double BF2TrDynaPSMB_vcet = 0.;
    double BF2XanCycle_pHl = 0.;
    double BF2trDynaPS_vbfn2 = 0.;
    double CO2_cond = 0.;
    double ChlPSI = 0.;
    double ChlT = 0.;
    double ChlT2 = 0.;
    double EPS_ATP_Rate = 0.;
    double EPS_Param = 0;
    double FIBF2FI_PQ = 0.;
    double FIBF_RA_Mg = 0.;
    double Fd_Thio_ET = 500;
    double GLUc = 0.;
    double GLight = 0.;
    double GammaStar = 0.;
    double Glight = 0.;
    double HPR = 4.66;
    double KA231 = 0.;
    double KC = 0.;
    double KE10 = 0.;
    double KE11 = 0.;
    double KE113 = 0.;
    double KE12 = 0.;
    double KE122 = 0.;
    double KE123 = 0.;
    double KE124 = 0.;
    double KE13 = 0.;
    double KE16 = 0.;
    double KE21 = 0.;
    double KE22 = 0.;
    double KE23 = 0.;
    double KE24 = 0.;
    double KE25 = 0.;
    double KE4 = 0.;
    double KE5 = 0.;
    double KE501 = 0.;
    double KE51 = 0.;
    double KE52 = 0.;
    double KE531 = 0.;
    double KE541 = 0.;
    double KE55 = 0.;
    double KE56 = 0.;
    double KE57 = 0.;
    double KE59 = 0.;
    double KE6 = 0.;
    double KE61 = 0.;
    double KE7 = 0.;
    double KE8 = 0.;
    double KE9 = 0.;
    double KGc = 0.;
    double KI1011 = 0.;
    double KI1012 = 0.;
    double KI11 = 0.;
    double KI1121 = 0.;
    double KI1122 = 0.;
    double KI113 = 0.;
    double KI12 = 0.;
    double KI1221 = 0.;
    double KI123 = 0.;
    double KI124 = 0.;
    double KI13 = 0.;
    double KI131 = 0.;
    double KI1311 = 0.;
    double KI1312 = 0.;
    double KI132 = 0.;
    double KI133 = 0.;
    double KI1331 = 0.;
    double KI1332 = 0.;
    double KI134 = 0.;
    double KI135 = 0.;
    double KI14 = 0.;
    double KI15 = 0.;
    double KI231 = 0.;
    double KI521 = 0.;
    double KI522 = 0.;
    double KI523 = 0.;
    double KI561 = 0.;
    double KI562 = 0.;
    double KI563 = 0.;
    double KI564 = 0.;
    double KI565 = 0.;
    double KI581 = 0.;
    double KI582 = 0.;
    double KI591 = 0.;
    double KI61 = 0.;
    double KI62 = 0.;
    double KI9 = 0.;
    double KM10 = 0.;
    double KM101 = 0.;
    double KM1011 = 0.;
    double KM1012 = 0.;
    double KM102 = 0.;
    double KM103 = 0.;
    double KM11 = 0.;
    double KM112 = 0.;
    double KM1131 = 0.;
    double KM1132 = 0.;
    double KM12 = 0.;
    double KM121 = 0.;
    double KM1221 = 0.;
    double KM1222 = 0.;
    double KM123 = 0.;
    double KM1241 = 0.;
    double KM1242 = 0.;
    double KM13 = 0.;
    double KM131 = 0.;
    double KM1311 = 0.;
    double KM1312 = 0.;
    double KM132 = 0.;
    double KM1321 = 0.;
    double KM1322 = 0.;
    double KM1331 = 0.;
    double KM1332 = 0.;
    double KM161 = 0.;
    double KM162 = 0.;
    double KM163 = 0.;
    double KM21 = 0.;
    double KM22 = 0.;
    double KM23 = 0.;
    double KM231 = 0.;
    double KM232 = 0.;
    double KM233 = 0.;
    double KM234 = 0.;
    double KM241 = 0.;
    double KM242 = 0.;
    double KM311 = 0.;
    double KM312 = 0.;
    double KM313 = 0.;
    double KM31a = 0.;
    double KM32 = 0.;
    double KM32b = 0.;
    double KM33 = 0.;
    double KM41 = 0.;
    double KM42 = 0.;
    double KM51 = 0.;
    double KM52 = 0.;
    double KM53 = 0.;
    double KM61 = 0.;
    double KM71 = 0.;
    double KM72 = 0.;
    double KM73 = 0.;
    double KM74 = 0.;
    double KM8 = 0.;
    double KM81 = 0.;
    double KM82 = 0.;
    double KM9 = 0.;
    double KO = 0.;
    double KR = 0.;
    double KVmo = 0.;
    double Ki572 = 0.;
    double Km511 = 0.;
    double Km512 = 0.;
    double Km513 = 0.;
    double Km521 = 0.;
    double Km551 = 0.;
    double Km552 = 0.;
    double Km553 = 0.;
    double Km554 = 0.;
    double Km561 = 0.;
    double Km562 = 0.;
    double Km571 = 0.;
    double Km581 = 0.;
    double Km591 = 0.;
    double Km593 = 0.;
    double Km621 = 0.;
    double NADHc = 0.;
    double NADc = 0.;
    double O2 = 210;
    double O2_cond = 0.;
    double PRGlu = 0.;
    double PR_ADP = 0.;
    double PR_ATP = 0.;
    double PS12ratio = 0.;
    double PS2BF_ADP = 0.;
    double PS2BF_Pi = 0.;
    double PS2CM_vdhap = 0.;
    double PS2CM_vgap = 0.;
    double PS2CM_vpga = 0.;
    double PS2EPS_V16 = 0.;
    double PS2EPS_v3 = 0.;
    double PS2PRC = 0.;
    double PS2PRO = 0.;
    double PS2PR_ADP = 0.;
    double PS2PR_ATP = 0.;
    double PS2PR_Pi = 0.;
    double PS2PR_V1 = 0.;
    double PS2RA_ATP = 0.;
    double PS2RA_RuBP_ini = 0.;
    double PS2RubACC = 0.;
    double PS2RubACO = 0.;
    double PS2SUCSV32 = 0.;
    double PSIIantennaSize = 37;
    double PSIantennaSize = 95;
    double PSPR2RA_v1 = 0.;
    double PSPR2RA_v111 = 0.;
    double PSPR2RA_v13 = 0.;
    double PSPR_RA_CA = 0.;
    double PSPR_RA_CO2 = 0.;
    double PSPR_RA_O2 = 0.;
    double PS_C_CA = 0.;
    double PS_C_CN = 0.;
    double PS_C_CP = 0.;
    double PS_PEXT = 0.;
    double PsV1 = 0.;
    double RROEA2PS_ATPGPP = 0.;
    double RROEA2PS_ATPase = 0.;
    double RROEA2PS_GAPDH = 0.;
    double RROEA2PS_PRK = 0.;
    double RROEA2PS_SBPase = 0.;
    double RROEA2RuACT_RuAC = 0.;
    double RROEA2trDynaPS_ve2Fd = 0.;
    double RROEA2trDynaPS_veFd2Calvin = 0.;
    double RUBISCOMETHOD = 1;
    double RUBISCOTOTAL = 0.;
    double Redox2PS_V13 = 0.;
    double Redox2PS_V16 = 0.;
    double Redox2PS_V6 = 0.;
    double Redox2PS_V9 = 0.;
    double RedoxReg_VMAX13 = 0.;
    double RedoxReg_VMAX16 = 0.;
    double RedoxReg_VMAX6 = 0.;
    double RedoxReg_VMAX9 = 0.;
    double RedoxReg_Param = 0;
    double RuACT2RA_v1 = 0.;
    double RuACT2RA_v61 = 0.;
    double RuACT2RA_v62 = 0.;
    double RuACT2RA_v7 = 0.;
    double RuACT2RA_vn1 = 0.;
    double RuACT2RA_vn7 = 0.;
    double SUCS2CM_vdhap = 0.;
    double SUCS2CM_vgap = 0.;
    double SUCS2CM_vpga = 0.;
    double SUCS2PS_Pic = 0.;
    double TestATPCost = 0.;
    double TestCa = 0.;
    double TestLi = 0.;
    double TestSucPath = 0.;
    double ThioT = 0.5;
    double Thio_Oxidation = 0.1;
    double V1 = 0.;
    double V10 = 0.;
    double V11 = 0.;
    double V111 = 0.;
    double V112 = 0.;
    double V113 = 0.;
    double V12 = 0.;
    double V121 = 0.;
    double V122 = 0.;
    double V123 = 0.;
    double V124 = 0.;
    double V13 = 0.;
    double V131 = 0.;
    double V132 = 0.;
    double V133 = 0.;
    double V16 = 0.;
    double V1Reg = 0.;
    double V1T = 0.;
    double V2 = 0.;
    double V21 = 0.;
    double V22 = 0.;
    double V23 = 0.;
    double V24 = 0.;
    double V2T = 0.;
    double V3 = 0.;
    double V31 = 0.;
    double V32 = 0.;
    double V33 = 0.;
    double V4 = 0.;
    double V5 = 0.;
    double V51 = 0.;
    double V52 = 0.;
    double V55 = 0.;
    double V56 = 0.;
    double V57 = 0.;
    double V58 = 0.;
    double V59 = 0.;
    double V6 = 0.;
    double V60 = 0.;
    double V61 = 10000;
    double V62 = 0.;
    double V7 = 0.;
    double V8 = 0.;
    double V9 = 0.;
    double Vdhap_in = 0.;
    double Vgap_in = 0.;
    double VolRatioStCyto = 1;
    double Vpga_in = 0.;
    double XanCycle2FIBF_Xstate = 0.;
    double XanCycle_kav = 0.;
    double XanCycle_kaz = 0.;
    double XanCycle_kva = 0.;
    double XanCycle_kza = 0.;
    double activase = 0.;
    double gp2V111 = 0.;
    double input_LHCI = 6;
    double input_LHCII = 13;
    double input_PSI = 1;
    double input_PSIIcore = 1;
    double kmCO2 = 0.;
    double kmO2 = 0.;
    double options1 = 0.;
    double tglobal = 250;
    double trDynaPS2RedReg_cal = 0.;
    double PS_PR_Param = 0;
    double SUCS_PARAM = 0;

    // Parameters
    arr PR_Param = zeros(2);
    arr BF_Param = zeros(2);
    arr FI_PARAM = zeros(2);
    arr FI_Param = zeros(2);
    arr RROEA_Param = zeros(2);
    arr RuACT_Param = zeros(2);
    arr SUCS_Param = zeros(2);
    arr XanCycle_Param = zeros(2);
    
    // Vel
    BFVEL BF_Vel; // = zeros(31);
    FI FI_Vel; // = zeros(58);
    PRVel PR_Vel; // = zeros(10);
    PSVel PS_Vel;// = zeros(18);
    RROEAVel RROEA_Vel; // = zeros(11);
    RedoxRegVel RedoxReg_Vel; // = zeros(2);
    RuACTVel RuACT_Vel;// = zeros(6);
    SUCSVel SUCS_Vel; // = zeros(15);
    XanCycleVel XanCycle_Vel; // = zeros(7);
    arr trDynaPS_VEL = zeros(0);
    
    // Ratio
    arr BFRatio = ones(49);
    arr FIRatio = ones(23);
    arr PRRatio = ones(48);
    arr PSRatio = ones(103);
    arr RacRatio = ones(16);
    arr SUCRatio = ones(66);
    arr XanRatio = ones(4);
    
    // Pool
    BFPool BF_Pool;// = zeros(12);
    FIBFPool FIBF_Pool; // = zeros(1);
    FIPool FI_Pool; // = zeros(2);
    RROEAPool RROEA_Pool; // = zeros(10);
    arr RuACT_Pool = zeros(5);
    arr SUCS_Pool = zeros(3);
 
    // RC
    BFRC BF_RC; // = zeros(34);
    FIRC FI_RC;// = zeros(21);
    RROEARC RROEA_RC;// = zeros(10);
    arr RuACT_RC = zeros(10);
    
    // Con
    arr BF_con = zeros(29);
    arr DynaPS_CON = zeros(0);
    arr FI_Con = zeros(22);
    arr RedoxReg_CON = zeros(0);
    arr trDynaPS_CON = zeros(0);

    // OUT
    arr BF2OUT = zeros(5);
    arr PR2OUT = zeros(12);
    arr PS2OUT = zeros(18);
    arr SUCS2OUT = zeros(12);
    arr XanCycle2OUT = zeros(4);

    // misc
    arr FIBF_AUX = zeros(2);
    arr FluxTR = zeros(142);
    RROEAKE RROEA_KE;// = zeros(9);
    
  arr ddd = zeros(120);
  // Matrix BF_CON = Matrix();
  TimeSeries<BFVEL> BF_VEL = TimeSeries<BFVEL>();
  TimeSeries<std::vector<double> > CO2A = TimeSeries<std::vector<double> > ();
  // Matrix FI_CON = Matrix();
  TimeSeries<FI> FI_VEL = TimeSeries<FI> ();
  TimeSeries<PRVel> PR_VEL = TimeSeries<PRVel> ();
  // Matrix PSPR = Matrix();
  TimeSeries<PSVel> PS_VEL = TimeSeries<PSVel> ();
  // Matrix RROEA_CON = Matrix();
  TimeSeries<RROEAVel> RROEA_VEL = TimeSeries<RROEAVel> ();
  std::vector<arr> RedoxReg_MP;
  TimeSeries<RedoxRegVel> RedoxReg_VEL = TimeSeries<RedoxRegVel> ();
  // Matrix RuACT_CON = Matrix();
  TimeSeries<RuACTVel> RuACT_VEL = TimeSeries<RuACTVel> ();
  // Matrix SUCS_CON = Matrix();
  TimeSeries<SUCSVel> SUCS_VEL = TimeSeries<SUCSVel> ();
  // Matrix XanCycle_CON = Matrix();
  TimeSeries<XanCycleVel> XanCycle_VEL = TimeSeries<XanCycleVel> ();
  // Matrix d = Matrix();
};
typedef Variables varptr;

void BF_Ini(varptr *myVars);

arr BF_Mb(double t, varptr *myVars);

void BF_Rate(double t, varptr *myVars);

void Condition(double t, varptr *myVars);

arr EPS_Ini(varptr *myVars);

arr EPS_mb(double t, arr &EPS_Con, varptr *myVars);

void FIBF_Ini(varptr *myVars);

arr FIBF_MB(double t, arr &FIBF_Con, varptr *myVars);

void FI_Ini(varptr *myVars);

arr FI_Mb(double t, varptr *myVars);

void FI_Rate(double t, varptr *myVars);

void GenOut(double t, varptr *myVars);

void IniModelCom(varptr *myVars);

arr PRinitial(varptr *myVars);

arr PRmb(double t, arr &PrS, varptr *myVars);

void PRrate(double t, arr &PrS, varptr *myVars);

arr PSI(varptr *myVars);

arr PSInitial(varptr *myVars);

void PSRate(double t, arr &PSs, arr &Param, varptr *myVars);

arr PS_PRIni(varptr *myVars);

arr PS_PRmb(double t, arr &PS_PRs, varptr *myVars);

arr PSmb(double t, arr &PSs, arr &Param, varptr *myVars);

void ParamSet(varptr *myVars);

void PreProcess(varptr *myVars);

arr RA_Ini(varptr *myVars);

arr RA_mb(double t, arr &RA_Con, varptr *myVars);

arr RROEA_Ini(varptr *myVars);

arr RROEA_Mb(double t, arr &RROEA_Con, varptr *myVars);

void RROEA_Rate(double t, arr &RROEA_Con, varptr *myVars);

int RedoxReg_FPercent(N_Vector u, N_Vector f_val, void *user_data);

arr RedoxReg_Ini(varptr *myVars);

void RedoxReg_Rate(double t, arr &RedoxReg_Con, varptr *myVars);

arr RedoxReg_mb(double t, arr &RedoxReg_Con, varptr *myVars);

arr RuACT_Ini(varptr *myVars);

arr RuACT_Mb(double t, arr &RuACT_Con, varptr *myVars);

void RuACT_Rate(double t, arr &RuACT_Con, varptr *myVars);

arr SUCS_Ini(varptr *myVars);

arr SUCS_Mb(double t, arr &SUCS_Con, varptr *myVars);

void SUCS_Rate(double t, arr &SUCS_Con, varptr *myVars);
void GenOut(double t, varptr *myVars);

void IniModelCom(varptr *myVars);

void ParamSet(varptr *myVars);

void PreProcess(varptr *myVars);

void SYSInitial(varptr *myVars);

void Sim_Ephotosynthesis(bool record);

double TargetFunVal(varptr *myVars);

arr XanCycle_Ini(varptr *myVars);

arr XanCycle_Mb(double t, arr &XanCycle_Con, varptr *myVars);

void XanCycle_Rate(double t, arr &XanCycle_Con, varptr *myVars);

arr ssPSFun(double VcmaxT, double JmaxT, double temp, double CO2, double Light, varptr *myVars);

void ssPSIni(double t, varptr *myVars);


class CM {
public:
    CM(varptr *myVars) {
        this->myVars = myVars;
    }
    static varptr *myVars;
    double CM_Drive2(double pop, double currentPop);
    arr CM_Ini();
    static int CM_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
};

class DynaPS {
public:
    DynaPS(varptr *myVars) {
        this->myVars = myVars;
    }
    static varptr *myVars;
    arr DynaPS_Drive(double ParaNum, double Ratio);

    static int DynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
    arr DynaPS_Ini();
};

class trDynaPS {
public:
    trDynaPS(varptr *myVars) {
        this->myVars = myVars;
    }
    static varptr *myVars;
    arr trDynaPS_Drive(double ParaNum, double Ratio);

    static int trDynaPS_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data);
    arr trDynaPS_Ini();
};
