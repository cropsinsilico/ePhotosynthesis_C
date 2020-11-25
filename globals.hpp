#pragma once

#include "definitions.hpp"

void Condition(double t, Variables *myVars);

void GenOut(double t, Variables *myVars);

void IniModelCom(Variables *myVars);

arr PSI(Variables *myVars);

void ParamSet(Variables *myVars);

void PreProcess(Variables *myVars);

void SYSInitial(Variables *myVars);

void Sim_Ephotosynthesis(bool record);

double TargetFunVal(Variables *myVars);

arr ssPSFun(double VcmaxT, double JmaxT, double temp, double CO2, double Light, Variables *myVars);

void ssPSIni(double t, Variables *myVars);
