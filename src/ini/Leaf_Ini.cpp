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

#include <math.h>
#include "Variables.hpp"
#include "modules/Leaf.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double Leaf::vinf = 0.;
double Leaf::TIME = 0.;
std::size_t Leaf::N = 1;

const double R = 8.3144598;// m2 kg s-2 K-1 mol-1
const double Ea_KCA = 40.9 * 1000.;
const double dS_KCA = 0.21 * 1000.;
const double Hd_KCA = 64.5 * 51000.;
const double Ea_Vpmax = 94.8 * 1000.;
const double dS_Vpmax = 0.25 * 1000.;
const double Hd_Vpmax = 73.3 * 1000.;
const double Ea_PPDK = 58.1 * 1000.;
const double Ea_Vcmax = 78. * 1000.;
const double Ea_Kc = 64.2 * 1000.;
const double Ea_Ko = 10.5 * 1000.;
// Vm_OC_25 = 0.18;// --unused
const double Ea_Vm_OC = 55.3 * 1000.;
//Ea_Jmax=43.1*1000;//grow at 25oc Bernacchi 2003
const double Ea_Jmax = 41. * 1000.;//grow at 28oc //// linear correlation between GrowTemp and Ea Bernacchi 2003
//Ea_Jmax=77900;Hd_Jmax=191929;dS_Jmax=627;//MASSAD 2007
const double Q10 = 2.;
const double Sc25 = 3. * std::pow(10., 4.);
const double So25 = 81.58;//kpa L /mmol
//Henry's law constant for CO2 and O2
const double C_CO2 = 2400.;
const double C_O2 = 1700.;


void Leaf::TempResponseEnzymes(Variables *theVars, const double Temp_leaf) {
    //WY Temp response 202001
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // //Tempreature response of enzymes

    //TempCorr_Jmax=exp(Ea_Jmax*(Temp_leaf+273.15-298.15)/(298.15*R*(Temp_leaf+273.15)))*(1+exp((298.15*dS_Jmax-Hd_Jmax)/(298.15*R)))/(1+exp(((Temp_leaf+273.15)*dS_Jmax-Hd_Jmax)/((Temp_leaf+273.15)*R)));
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    const double Sc = 1. / (1. / Sc25 * std::exp(C_CO2 * (1. / (Temp_leaf + 273.15) - 1. / 298.)));
    const double So = 1. / (1. / So25 * std::exp(C_O2 * (1. / (Temp_leaf + 273.15) - 1. / 298.)));

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////// ME and MDH and other enzymes

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    theVars->TempFactor.clear();
    theVars->TempFactor.V1 = std::exp(Ea_KCA * (Temp_leaf + 273.15 - 298.15) / (298.15 * R * (Temp_leaf + 273.15))) *
                  (1. + std::exp((298.15 * dS_KCA - Hd_KCA) / (298.15 * R))) /
                  (1. + std::exp(((Temp_leaf + 273.15) * dS_KCA - Hd_KCA) / ((Temp_leaf + 273.15) * R)));
    theVars->TempFactor.V2 = std::exp(Ea_Vpmax * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15))) *
                  (1. + std::exp((298.15 * dS_Vpmax - Hd_Vpmax) / (298.15 * R))) /
                  (1. + std::exp(((Temp_leaf + 273.15) * dS_Vpmax - Hd_Vpmax) / ((Temp_leaf + 273.15) * R)));
    theVars->TempFactor.V5 = std::exp(Ea_PPDK * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15)));
    theVars->TempFactor.V6 = std::exp(Ea_Vcmax * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15)));
    theVars->TempFactor.KmCO2_6 = std::exp(Ea_Kc * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15))) * Sc25 / Sc;
    theVars->TempFactor.KmO2_6 = std::exp(Ea_Ko * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15))) * So25 / So;
    theVars->TempFactor.Vm_OC = std::exp(Ea_Vm_OC * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15)));
    theVars->TempFactor.Jmax = std::exp(Ea_Jmax * ((Temp_leaf + 273.15) - 298.15) / (298.15 * R * (Temp_leaf + 273.15)));
    theVars->TempFactor.Vm_Enz = pow(Q10, (Temp_leaf - 25.) / 10.);

    //Chl_O2a=Chl_O2*kH_O2;//WY2018
    //Chl_O2=Chl_O2*kH_O2;
    // //other enzymes
    // Q10_1=1.93;
    // Q10_2=2;
    // Q10_3=2;
    // Q10_5=2;
    // Q10_6=2;
    // Q10_7=2;
    // Q10_8=2;
    // Q10_9=2;
    // Q10_10=2;
    // Q10_13=2;
    // Q10_23=2;
    // Q10_112=1.81;
    // Q10_113=2;
    // Q10_121=2;
    // Q10_122=2.01;
    // Q10_123=2;
    // Q10_124=2;
    // Q10_131=2;
    // Q10_51=2;
    // Q10_52=1.60;
    // Q10_55=2;
    // Q10_56=2;
    // Q10_57=2;
    // Q10_58=2;
    //
    // Ru_Act=-3E-05*Tleaf^3 + 0.0013*Tleaf^2 - 0.0106*Tleaf + 0.8839;//Rubisco activition state
    // PsV1 =PsV1_0*Ru_Act*Q10_1^((Tleaf-25)/10);
    // PsV2 =PsV2_0*Q10_2^((Tleaf-25)/10);
    // PsV3 =PsV3_0*Q10_3^((Tleaf-25)/10);
    // PsV5 =PsV5_0*Q10_5^((Tleaf-25)/10);
    // PsV6 =PsV6_0*Q10_6^((Tleaf-25)/10);
    // PsV7 =PsV7_0*Q10_7^((Tleaf-25)/10);
    // PsV8 =PsV8_0*Q10_8^((Tleaf-25)/10);
    // PsV9 =PsV9_0*Q10_9^((Tleaf-25)/10);
    // PsV10=PsV10_0*Q10_10^((Tleaf-25)/10);
    // PsV13=PsV13_0*Q10_13^((Tleaf-25)/10);
    // PsV23=PsV23_0*Q10_23^((Tleaf-25)/10);
    // PrV112=PrV112_0*Q10_112^((Tleaf-25)/10);
    // PrV113=PrV113_0*Q10_113^((Tleaf-25)/10);
    // PrV121=PrV121_0*Q10_121^((Tleaf-25)/10);
    // PrV122=PrV122_0*Q10_122^((Tleaf-25)/10);
    // PrV123=PrV123_0*Q10_123^((Tleaf-25)/10);
    // PrV124=PrV124_0*Q10_124^((Tleaf-25)/10);
    // PrV131=PrV131_0*Q10_131^((Tleaf-25)/10);
    // SUCSV51=SUCSV51_0*Q10_51^((Tleaf-25)/10);
    // SUCSV52=SUCSV52_0*Q10_52^((Tleaf-25)/10);
    // SUCSV55=SUCSV55_0*Q10_55^((Tleaf-25)/10);
    // SUCSV56=SUCSV56_0*Q10_56^((Tleaf-25)/10);
    // SUCSV57=SUCSV57_0*Q10_57^((Tleaf-25)/10);
    // SUCSV58=SUCSV58_0*Q10_58^((Tleaf-25)/10);
    //
    // PrV111= PsV1* 0.24;
}
LeafCondition* Leaf::_init(Variables* theVars) {

    LeafCondition* Leaf_con = new LeafCondition();
    Leaf_con->Tleaf = theVars->Tp;//energy balance
    Leaf_con->H2Oou = 0;
    Leaf_con->CO2in = 0;
    Leaf_con->Ci = 0.7 * theVars->CO2_in;//Initial Ci u moles/mole solublility check
    Leaf_con->Gs = theVars->BallBerryInterceptC4; // Initial stomatal conductance moles/m2 leaf area/s
    Leaf_con->Cb = 0.9 * theVars->CO2_in;
    const double ESaturation = 0.611 * std::exp(17.502 * theVars->Tp / (theVars->Tp + 240.97));//KPa
    Leaf_con->Eb = theVars->WeatherRH * ESaturation;//KPa
    return Leaf_con;
}

void Leaf::_reset() {
    vinf = 0.;
}
