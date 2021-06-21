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
#include "modules/FIBF.hpp"
#include "modules/FI.hpp"
#include "modules/BF.hpp"

// This function calculate the mass balance equation for the complete model of the light reactions.

arr FIBF::_MB(const double t, const FIBFContainer* FIBF_Con, Variables *theVars) {

    // First Get the variables needed for the calcualtion step
    BFContainer* BF_con = FIBF_Con->BF_con;

    FIContainer* FI_Con = FIBF_Con->FI_con;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //       Calculate auxilary variable, PQ             //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    const double PQ = theVars->FIBF_Pool.PQT - theVars->FI_Pool.QBt - FI_Con->PQn - BF_con->Qi - BF_con->Qn - BF_con->Qr - BF_con->ISPoQH2 - BF_con->QHsemi;

    theVars->FIBF2FI_PQa = theVars->FI_Pool.QBt + BF_con->Qi + BF_con->Qn + BF_con->Qr + BF_con->ISPoQH2 + BF_con->QHsemi;
    BF_con->Q = PQ;

    theVars->FIBF2FI_PQ = PQ;

    theVars->FI_RC.kA_d = FIBF_Con->kd;
    theVars->FI_RC.kU_d = FIBF_Con->kd;

    theVars->BF_RC.Kd = FIBF_Con->kd;

    arr BF_mb = BF::MB(t, BF_con, theVars);
    arr FI_mb = FI::MB(t, FI_Con, theVars);

    // Assign the value of the calcualted BF_mb and FI_mb to FIBF_MB variable
    arr FIBF_mb;
    FIBF_mb.insert(FIBF_mb.end(), BF_mb.begin(), BF_mb.end());
    FIBF_mb.insert(FIBF_mb.end(), FI_mb.begin(), FI_mb.end());


    // Now specially calcualte the mass balance equation for the rate constant of the heat dissipation

    //const double PHl = BF_con.PHl;           // Get the PH value of the lumen
    const double Hl = pow(10, BF_con->PHl);
    const double QH = pow(10, (5.5)) / (Hl + pow(10, (5.5)));

    const double RC = 0.1;                   // RC is the relaxation constant, which is one term borrowed from Laisk et al., 1997;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Here is the section implementing the nonphotochemical quenching.
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //PHl = theVars->BF2FIBFMB_PHl;

    double dmax = 5 * pow(10, 8) * QH;

    if (theVars->XanCycle_BF_com) {
        if (theVars->XanCycle2FIBF_Xstate > 0.3) {
            dmax = dmax * theVars->XanCycle2FIBF_Xstate / 0.3;
        }
    }
    FIBF_mb[51] = RC * (dmax - FIBF_Con->kd);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate the proton generation rate from the model of FI and use that to calculate the lumen PH //            //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //std::cout << theVars->FI_Vel;
    //std::cout << theVars->BF_Vel;
    // one molecular water molecules to release four protons.
    const double Hroe = 4 * theVars->FI_Vel.vS3_S0 / theVars->AVR; // 27 is the conversion of unit from micromole per meter squre leaf area to mmol per liter.
    const double Hvqo1 = theVars->BF_Vel.Vbf8 / theVars->AVR;      // The rate of release of protons into lumen through Qo site
    const double Hvqo2 = theVars->BF_Vel.Vbf3 / theVars->AVR;      // The rate of proton release into lumen through Qo site

    BF_mb[25] = (Hvqo1 + Hvqo2 + Hroe - theVars->HPR * theVars->BF_Vel.Vbf11); // BFHl The proton and protonated buffer species in lumen, similarly, we can only use the buff concentration, but, the proton concentration can not be used here.
    BF_mb[27] = - (Hvqo1 + Hvqo2 + Hroe - theVars->HPR * theVars->BF_Vel.Vbf11) / 1000 / 0.015;//   PHl  The changes in PH of lumen, 0.03 is from Curz et al., 2001, Biochemistry.

    FIBF_mb[25] = BF_mb[25];
    FIBF_mb[27] = BF_mb[27];

    ////////////////////////////////////////////////////////////////////////////////////////////
    //          Calculate the PH of stroma        //
    ////////////////////////////////////////////////////////////////////////////////////////////

    //const double v3 = theVars->FI_Vel.v3 ; // v3 The rate of exchange of QAQBH2 with PQ; There is two proton uptake from stroma
    //const double v_r3 = theVars->FI_Vel.v_r3 ; // v_r3 The rate of exchange of QAQB with PQH2; there is two proton release into stroma
    //const double v3_n = theVars->FI_Vel.v3_n ; // v3_n The rate of exchange of QAnQBH2 with PQ; there is two protons uptake from stroma
    //const double v_r3_n = theVars->FI_Vel.v_r3_n ; // v_r3_n The rate of exchange of QAnQB with PQH2; there is two protons release into stroma

    const double GPQH2_qb = theVars->FI_Vel.v3 - theVars->FI_Vel.v_r3 + theVars->FI_Vel.v3_n - theVars->FI_Vel.v_r3_n;
    const double vqb = 2 * GPQH2_qb;//   The total rate of proton uptake at the QB site of PSII.

    //const double Vqi = theVars->BF_Vel.Vqi ;   //   The rate of proton uptake from the stroma side
    const double Hvqi = theVars->BF_Vel.Vqi / theVars->AVR;  // The rate of proton uptake from stroma at BF_con.Qi site of cytbc1 complex
    //const double vbfn2 = theVars->BF_Vel.vbfn2 ;   // The rate of proton consumption by formation of NADPH
    const double Hrqb = vqb / theVars->AVR; // Convert the unit of vqb from micormole per meter square per second to mM s-1; vqb is the rate of QB2- reduction in thylakoid membrane.

    BF_mb[24] = (theVars->HPR * theVars->BF_Vel.Vbf11 - Hrqb - Hvqi - theVars->BF_Vel.vbfn2);  // BFHs The proton and protonated buffer species in stroma. The proton concentration is not used in the MB procedure. The reason is that the proton concentration is buffered and therefore did not changed linerly with the generation of the protons.
    BF_mb[26] = - (theVars->HPR * theVars->BF_Vel.Vbf11 - Hrqb - Hvqi - theVars->BF_Vel.vbfn2) / 1000 / 0.015;  // PHs, The changes of PH in stoma, 0.03 mol /PH from Laisk et al.

    FIBF_mb[24] = BF_mb[24];
    FIBF_mb[26] = BF_mb[26];
    //const double Vbf1 = theVars->BF_Vel.Vbf1 ;          // The rate of PQH2 utilization when forming the PQH2.ISP complex
    const double GPQH2_t = GPQH2_qb - theVars->BF_Vel.Vbf1 + theVars->BF_Vel.Vqi / 2;  // This is the total rate of PQH2 generation

    FIBF_mb[7] = 0;        // Q Quinone in thylakoid membrane in free form
    FIBF_mb[11] = GPQH2_t; // QH2 The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.
    FIBF_mb[50] = GPQH2_t; // QH2 The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.
    //std::cout << theVars->FI_Vel;
    //std::cout << theVars->BF_Vel;
    DEBUG_DELTA(FIBF_mb)
    return FIBF_mb;
}
