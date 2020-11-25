cmake_minimum_required(VERSION 3.5)

project(ePhotosynthesis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ePhotosynthesis BF_Ini.cpp BF_MB.cpp BF_Rate.cpp CIMCom.cpp CM_Drive2.cpp CM_Ini.cpp CM_mb.cpp Condition.cpp DynaPS_Drive.cpp DynaPS_Ini.cpp DynaPS_mb.cpp ePhotosynthesis.cpp EPS_Ini.cpp EPS_mb.cpp FIBF_Ini.cpp FIBF_MB.cpp FI_Ini.cpp FI_MB.cpp FI_Rate.cpp GenOut.cpp globals.hpp IniModelCom.cpp ParamSet.cpp PreProcess.cpp PRinitial.cpp PRmb.cpp PRrate.cpp PSI.cpp PSInitial.cpp PSmb.cpp PS_PRIni.cpp PS_PRmb.cpp PSRate.cpp RA_Ini.cpp RA_mb.cpp RedoxReg_FPercent.cpp RedoxReg_Ini.cpp RedoxReg_mb.cpp RedoxReg_Rate.cpp RROEA_Ini.cpp RROEA_mb.cpp RROEA_Rate.cpp RuACT_Ini.cpp RuACT_MB.cpp RuACT_Rate.cpp Sim_Ephotosynthesis.cpp ssPSFun.cpp ssPSIni.cpp SUCS_Ini.cpp SUCS_mb.cpp SUCS_Rate.cpp SYSInitial.cpp TargetFunVal.cpp trDynaPS_Drive.cpp trDynaPS_Ini.cpp trDynaPS_mb.cpp XanCycle_Ini.cpp XanCycle_mb.cpp XanCycle_Rate.cpp globals.hpp BF.hpp FI.hpp FIBF.hpp EPS.hpp SUCS.hpp PS.hpp PR.hpp PS_PR.hpp CM.hpp XanCycle.hpp RuACT.hpp RA.hpp DynaPS.hpp RROEA.hpp trDynaPS.hpp CM.cpp DynaPS.cpp RedoxReg.hpp Variables.hpp definitions.hpp driver.hpp ssPS.hpp)
