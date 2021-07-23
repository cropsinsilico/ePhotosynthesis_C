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
#include "modules/BF.hpp"

void BF::_Rate(const double t, const BFCondition* BF_con, Variables *theVars) {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Step II get the concentration of differnet component //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //  This is the initialization step for the module of the BF_con.Q cycle, and ATP synthesis steps

    double ADP = BF_con->ADP; // ADP in stroma

    double Pi = BF::_Pi; // Phosphate in stroma

    //////////////////////////////////////////////////////////////
    // Get the auxiliary variables //
    //////////////////////////////////////////////////////////////

    const double cytc1n = theVars->BF_Pool.kA_d - BF_con->cytc1; // The concentration of reduced cytc1
    const double cytbLn = theVars->BF_Pool.kA_d - BF_con->cytbL; // The concentration of reduced cytbL
    const double cytbHn = theVars->BF_Pool.kA_d - BF_con->cytbH; // The concentration of reduced cytbH
    const double cytc2n = theVars->BF_Pool.kA_f - BF_con->cytc2; // The concentration of reduced cytc2
    const double Kl = theVars->BF_Pool.kA_U - BF_con->Ks;        // The concentration of K in lumen
    const double Mgl = theVars->BF_Pool.kU_A - BF_con->Mgs;      // The concentration of Mg in lumen
    const double Cll = theVars->BF_Pool.kU_d - BF_con->Cls;      // The concentration of Cl in lumen
    const double Fd = theVars->BF_Pool.kU_f - BF_con->Fdn;       // The conncentration of oxidized Fd in stroma
    const double A = theVars->BF_Pool.k1 - BF_con->An;           // The concentration of oxidized electron acceptor in PSI
    const double QST = theVars->BF_Pool.kA_d;                    // Assuming that the total number of cytochrome is equal to the total number of quinone binding site;
    const double QSe = QST - BF_con->Qi - BF_con->Qn - BF_con->Qr;
    const double P700p = theVars->BF_Pool.k23 - BF_con->P700;    // The number of positive P700;
    const double NADP = theVars->BF_Pool.k30 - BF_con->NADPH;

    if (theVars->FIBF_PSPR_com) {
        ADP = theVars->ADP;
        Pi = theVars->Pi;
    }

    const double CoeffVol = theVars->AVR;

    const double Hfs = pow(10, (-BF_con->PHs)) * 1000;
    const double OHs = pow(10, (-14)) / (Hfs / 1000) * 1000;

    const double BFs = BF_con->BFHs - Hfs;          // The concentration of the protonated buffer speices.
                                                   // Notice here the variable BFHs represent the total concentration of both proton and the protonated buffer species in stroma
    const double BFns = theVars->BF_Pool.kz - BFs; // The total concentration of deprotonated buffer species in stroma;

    const double RegPHs = 1;

    const double Temp1 = pow(10, 5.5 - 8);
    const double a = Temp1 / (1 + Temp1);
    const double Temp2 = pow(10, 5.5 - BF_con->PHl);
    const double b = Temp2 / (1 + Temp2);
    const double RegPHl = 1 - (b - a);
    const double Iin = theVars->BF_Param[0];

    //////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the rate of different reactions //
    ////////////////////////////////////////////////////////////////////////////////////////////

    const double Vmax = theVars->BF_RC.K1 * (BF_con->ISPo + BF_con->ISPHr);                     // The maximum rate of formation of enzyme substrate complex
    theVars->BF_Vel.Vbf1 = Vmax * BF_con->ISPo / (BF_con->ISPo + BF_con->ISPHr) * BF_con->QH2 / theVars->BF_Pool.k_r1; // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf2 = theVars->BF_RC.K2 * BF_con->ISPoQH2 * RegPHl;                       // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf3 = theVars->BF_RC.K3 * BF_con->QHsemi * BF_con->cytbL / theVars->BF_Pool.kA_d * RegPHl; // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf4 = theVars->BF_RC.K4 * cytbLn * BF_con->cytbH / theVars->BF_Pool.kA_d; // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf5 = theVars->BF_RC.K5 * cytbHn * BF_con->Qi / theVars->BF_Pool.kA_d;    //;Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf6 = theVars->BF_RC.K6 * cytbHn * BF_con->Qn / theVars->BF_Pool.kA_d;    // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf7 = theVars->BF_RC.K7 * theVars->BF_Pool.kA_d * QSe / QST * BF_con->Q / theVars->BF_Pool.k_r1; // QSe ans QST represent the empty quinone binding site and the total number of quinone binding site respectively. Unit: micromole s-1 per meter squareleaf area;
    theVars->BF_Vel.Vbf8 = theVars->BF_RC.K8 * theVars->BF_Pool.kA_d * (BF_con->ISPHr / theVars->BF_Pool.kA_d * BF_con->cytc1 / theVars->BF_Pool.kA_d - BF_con->ISPo / theVars->BF_Pool.kA_d * cytc1n / theVars->BF_Pool.kA_d / theVars->BF_RC.KE8) * RegPHl; // Unit: micromole s-1 m-2 leaf area
    theVars->BF_Vel.Vbf9 = theVars->BF_RC.K9 * theVars->BF_Pool.kA_d * (cytc1n / theVars->BF_Pool.kA_d * BF_con->cytc2 / theVars->BF_Pool.kA_d - BF_con->cytc1 / theVars->BF_Pool.kA_d * cytc2n / theVars->BF_Pool.kA_d / theVars->BF_RC.KE9); // Unit: micromole s-1 m-2 leaf area
    const double KE10 = 10;
    theVars->BF_Vel.Vbf10 = theVars->BF_RC.K10 * P700p * cytc2n / theVars->BF_Pool.k23 - theVars->BF_RC.K10 * BF_con->cytc2 * BF_con->P700 / theVars->BF_Pool.k23 / KE10; // Unit: micromole s-1 m-2 leaf area

    double Ytemp;
    if (Hfs < 0) {
        Ytemp = 0;
    } else {
        Ytemp = 25.359 - 0.0425 * pow(BF_con->PHs, 3) + 1.0986 * pow(BF_con->PHs, 2) - 9.1831 * BF_con->PHs;
    }

    // Calculation of the different excition transfer reactions occurred in PSI
    theVars->BF_Vel.Vipc = BF_con->Aip * theVars->BF_RC.Kau; // The rate of exciton transfer from peripheral antenna to core antenna; unit: micromole m-2 s-1 Unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vicp = BF_con->U * theVars->BF_RC.Kua;   // The rate of exciton transfer from core antenna to peripheral antenna Unit: micromole m-2 leaf area per second

    // Iin is the total absorbed light

    theVars->BF_Vel.Vinc = Iin * theVars->ChlPSI / (theVars->ChlT2 + theVars->ChlPSI) * 95 / 184;  // PPFD absorbed by core antenna of PSI    Unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vinp = Iin * theVars->ChlPSI / (theVars->ChlT2 + theVars->ChlPSI) * 105 / 184; // PPFD absorbed by peripheral antenna of PSI  Unit: micromole m-2 leaf area per second

    theVars->BF_Vel.Vdp = BF_con->Aip * theVars->BF_RC.Kd; // The rate of heat dissipation from peripheral antenna Unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vdc = BF_con->U * theVars->BF_RC.Kd;   // The rate heat dissipation from core antenna Unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vfp = BF_con->Aip * theVars->BF_RC.Kf; // The fluorescence emission from peripheral antenna Unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vfc = BF_con->U * theVars->BF_RC.Kf;   // The fluorescence emission from core antenna Unit: micromole m-2 leaf area per second

    // The other empirical rates calculations
    theVars->BF_Vel.Vsfd = 0; // The sink for Fd utilization; unit: mmol l-1 s-1;

    const double CO2 = theVars->CO2_cond * 3 * pow(10, 4);

    const double MaxCO2Rate = 100 * CO2 / (CO2 + 460);

    theVars->BF_Vel.VsATP = MaxCO2Rate * 1.5 / CoeffVol * BF_con->ATP / 1.5;        //(ADP + ATP); // The sink for ATP utilizaiton, 20 represent the of CO2 assimilation, since 1 meter square amount to 27 ml, therefore, the sink capacity should be 20 * 1.5 * 1.5 mmol / 27 l-1 s-1. The 1.5 represents the 1.5 ATP consumption per CO2 fixation.  Unit: mmol l-2 s-1
    theVars->BF_Vel.VsNADPH = MaxCO2Rate / CoeffVol * 1 * BF_con->NADPH / theVars->BF_Pool.k30; // For 6 C6 = 5 C6 + 1C6;
    theVars->BF_Vel.VgPQH2 = BF_con->Q * 800 * RegPHs;                              // Assuming that the rate of generation of PQH2 through QB site only depend on the PQ and PQH2 exchange capacity.
    double NetCharge = Hfs + BF_con->Ks + 2 * BF_con->Mgs - OHs - BF_con->Cls - BFns; // The difference between the positive and negative charge in stroma. It was assumed that the charge is in equilibrium state in the beginning of the model, therefore, the difference in the positive and negative charges reflect the charges forming electrical potential cross the membrane. The unit is mmol l-1.
    NetCharge = NetCharge / 1000;                                                  // The unit conversion. Convert from mmol l-1 to mol l-1.
    NetCharge = NetCharge * theVars->BF_RC.RVA;                                    // Convert from mol l-1 to mol cm-2.
    const double AfC = 6.022 * pow(10, 23);
    const double UnitCharge = 1.6 * pow(10, -19);
    NetCharge = NetCharge * AfC * UnitCharge;                                      // Unit: Coulomb cm-2
    const double MPotential = 2 * NetCharge / 6 * pow(10, 6);
    const double temp = - MPotential / 0.026;                                      // 0.026 pools together the RT/F; With R as 8.314 J K-1 mol-1; T: 298; F: 9.648*10^4 C mol-1.

    double JMg;
    double JCl;
    double JK;
    if (temp == 0.) {
        JK = theVars->BF_RC.PK * (Kl - BF_con->Ks) / 10;     // The flux of K from lumen to stroma; unit: mol/dm2/s; the unit of permeability is: cm s-1; 10 represent the conversion from cm s-1 to dm s-1; Remember that 1 liter is 1 dm3.
        JCl = theVars->BF_RC.PCl * (Cll - BF_con->Cls) / 10; // The flux of Cl from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion from cm s-1 to dm s-1; remember that 1 liter amount to 1 decimeter cube
        JMg = theVars->BF_RC.PMg * (Mgl - BF_con->Mgs) / 10; // The flux of Mg from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion of cm s-1 to dm s-1. Remember that 1 liter amount to 1 decimeter cube.
    } else {
        JK = theVars->BF_RC.PK * temp * (Kl - BF_con->Ks * exp(-temp)) / (1 - exp(-temp)) / 10;                 // The flux of K from lumen to stroma; unit: mol/dm2/s; the unit of permeability is: cm s-1; 10 represent the conversion from cm s-1 to dm s-1; Remember that 1 liter is 1 dm3.
        JCl = - theVars->BF_RC.PCl * temp * (Cll - BF_con->Cls * exp(temp)) / (1 - exp(temp)) / 10;             // The flux of Cl from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion from cm s-1 to dm s-1; remember that 1 liter amount to 1 decimeter cube
        JMg = theVars->BF_RC.PMg * temp / 2 * (Mgl - BF_con->Mgs * exp(-temp / 2)) / (1 - exp(-temp / 2)) / 10; // The flux of Mg from lumen to stroma; unit: mol/dm2/s; unit of permeability is cm s-1; 10 represent the conversion of cm s-1 to dm s-1. Remember that 1 liter amount to 1 decimeter cube.
    }
    theVars->BF_Vel.Vqi  = theVars->BF_RC.Kqi * BF_con->Qr * 2 / 0.72 * Ytemp; // The rate of proton uptake at BF_con.Qi site of bc1 complex; Unit: micromole s-1 m-2 leaf area; the coefficient 2 represents that two protons were taken up at the same time.0.72 represents the calculated proton uptake rate at BF_con.Qi site when PH is 9.
    theVars->BF_Vel.JKc  = JK / theVars->BF_RC.RVA / 100;                     // 100 represent the conversion to cm-2;  theVars->BF_RC.RVA is the ratio between the lumen volume and thylakoid membrane area.
    theVars->BF_Vel.JMgc  = JMg / theVars->BF_RC.RVA / 100;                   // 100 represent the conversion to cm-2;  theVars->BF_RC.RVA is the ratio between the lumen volume and thylakoid membrane area.
    theVars->BF_Vel.JClc  = JCl / theVars->BF_RC.RVA / 100;                   // 100 represent the conversion to cm-2;  theVars->BF_RC.RVA is the ratio between the lumen volume and thylakoid membrane area.


    const double P700e = BF_con->U * (theVars->BF_Pool.k23 - P700p) / 120;                // The amount of excited P700; micromole m-2 leaf area
    theVars->BF_Vel.Vbf15 = P700e * theVars->BF_RC.K15 * A / theVars->BF_Pool.k1;        // The rate of PSI primary charge separation; unit: micromole m-2 leaf area per second
    theVars->BF_Vel.Vbf16 = BF_con->An * theVars->BF_RC.K16 * Fd / theVars->BF_Pool.kU_f; // The rate of electron transport from the electron acceptor of PSI to Fd; Unit: micromole m-2 leaf area s-1;

    ////////////////////////////////////////////////////////////////////////////////

    const double RT = 298 * 8.314;             // Gas constnat and the temperature
    const double DeltaGo = 7.3 * 4184;         //   The free energy change for ATP synthesis from ADP and Pi
    const double DiffPH = BF_con->PHs - BF_con->PHl;
    const double DeltaG11 = DeltaGo - 2.3 * RT * theVars->HPR * DiffPH + theVars->HPR * MPotential * 9.6 * pow(10, 4);
    const double KE11 = exp(-DeltaG11 / (RT)); // The equilibrium constant of ATP synthesis

    const double Temp = theVars->BF_RC.Vmax11 * (ADP * Pi - BF_con->ATP / KE11) / ((theVars->BF_RC.KM1ADP * theVars->BF_RC.KM1PI) * (1 + ADP / theVars->BF_RC.KM1ADP + Pi / theVars->BF_RC.KM1PI + ADP * Pi / (theVars->BF_RC.KM1ADP * theVars->BF_RC.KM1PI))); // Unit: mmol l- s-1; The stroma volume is used as a basis for the volume
    double Vbf11 = Temp;

    if (Vbf11 < 0.)
        Vbf11 = 0.;


    // The EPS_ATP_Rate is used in the overall model for the calculation of the mass balance equation of ATP.
    theVars->EPS_ATP_Rate = Vbf11;

    theVars->BF_Vel.vbfn2 = 2 * theVars->BF_RC.V2M * (BF_con->Fdn * NADP / theVars->BF_Pool.kU_f - Fd * BF_con->NADPH / (theVars->BF_Pool.kU_f * theVars->BF_RC.KE2)) / (theVars->BF_RC.KM2NADP * (1 + NADP / theVars->BF_RC.KM2NADP + BF_con->NADPH / theVars->BF_RC.KM2NADPH)); // mmol/l/s  //QF add 2*

    theVars->BF_Vel.vcet = theVars->BF_RC.V2M * BF_con->Qi * BF_con->Fdn / theVars->BF_Pool.kU_f * CoeffVol;

    ////////////////////////////////////////////////////////////////////////////////

    //   Transfer infroamtion between models                     //
    ////////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////////
    //   Part V Output of Velocity for plot //
    ////////////////////////////////////////////////////////////////////////////////

    if (theVars->BF_TIME_N == 0)
        theVars->BF_TIME_N = 1;

    if (t > theVars->BF_OLD_TIME) {
        theVars->BF_TIME_N = theVars->BF_TIME_N + 1;
        theVars->BF_OLD_TIME = t;
    }

    theVars->BF_Vel.Vbf11  = Vbf11;

    DEBUG_INTERNAL(theVars->BF_Vel)
    if (theVars->record) {
        theVars->BF_VEL.insert(theVars->BF_TIME_N - 1, t, theVars->BF_Vel);
        theVars->BF2OUT[0] = BF_con->Fdn;
        theVars->BF2OUT[1] = BF_con->PHs;
        theVars->BF2OUT[2] = BF_con->PHl;
        theVars->BF2OUT[3] = BF_con->NADPH;
        theVars->BF2OUT[4] = BF_con->ATP;
    }
}
