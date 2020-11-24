
#include "globals.hpp"
#include "CM.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//   Copyright   Xin-Guang Zhu, Yu Wang, Donald R. ORT and Stephen P. LONG
//CAS-MPG Partner Institute for Computational Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//China Institute of Genomic Biology and Department of Plant Biology, Shanghai Institutes for Biological Sciences, CAS, Shanghai,200031
//University of Illinois at Urbana Champaign
//Global Change and Photosynthesis Research Unit, USDA/ARS, 1406 Institute of Genomic Biology, Urbana, IL 61801, USA.

//   This file is part of e-photosynthesis.

//    e-photosynthesis is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation;

//    e-photosynthesis is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License (GPL)
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



CMCon CM::CM_Ini() {
    
    //PS_PRCon PS_PR_con = PS_PRIni(myVars);
    //PS_PRCon PS_PR_con = PS_PRIni(myVars);
    //arr PS_PRs = PS_PR_con.toArray();
    //arr CMs = zeros(36);
    //arr PS_PRs = PS_PR_con.toArray();
    //for (int m = 0; m < 23; m++)
    //    CMs[m] = PS_PRs[m];
    
    
    //SUCSCon SUCS_Con = SUCS_Ini(myVars);
    //arr SUCSc = SUCS_Con.toArray();
    // The gap left is for later use.
    
    //for (int m = 0; m < 12; m++)
    //    CMs[23 + m] = SUCSc[m];
    
    
    //CMs[35] = PS_PRs[23];
    //CMCon CMs(PS_PR_con, SUCS_Con);
    return CMInit(myVars);
}

int CM::CM_mb(realtype t, N_Vector u, N_Vector u_dot, void *user_data) {
    //global TestSucPath;
    realtype *x = N_VGetArrayPointer(u);
    realtype *dxdt = N_VGetArrayPointer(u_dot);
    
    //arr CM_con = zeros(36);
    //for (int i = 0; i < 36; i++)
    //    CM_con[i] = x[i];
    CMCon CMs(x);
    arr ddxdt = CM_Mb(t, CMs, myVars);
    /*arr PSPR_Con = zeros(24);
    for (int m = 0; m < 23; m++)
        PSPR_Con[m] = x[m];
    
    
    arr SUCSc = zeros(12);
    for (int m = 0; m < 12; m++)
        SUCSc[m] = x[23 + m];
    SUCSCon SUCS_Con(SUCSc);
    
    PSPR_Con[23] = x[35];
    
    
    arr SUCS_DYDT = zeros(12);
    SUCS_DYDT = SUCS_Mb(t, SUCS_Con, myVars);
    PS_PRCon PS_PR_con(PSPR_Con);
    arr PSPR_DYDT = PS_PRmb(t, PS_PR_con, myVars);
    
    for (int m = 0; m < 23; m++)
        dxdt[m] = PSPR_DYDT[m];
    
    
    for (int m = 0; m < 12; m++)
        dxdt[m + 23] = SUCS_DYDT[m];
    
    
    dxdt[35] = PSPR_DYDT[23];
    
    //global PS2CM_vdhap;
    const double vdhap = myVars->PS2CM_vdhap;        // The rate of export out of chloroplast
    
    //global PS2CM_vgap;          // The rate of export out of chloroplast
    const double vgap = myVars->PS2CM_vgap;
    
    //global SUCS2CM_vdhap;       // The rate of import into the cytosol
    //global SUCS2CM_vgap;        // The rate of import into the cytosol
    const double vdhap_ins = myVars->SUCS2CM_vdhap;   //	DHAP IN
    const double vgap_ins = myVars->SUCS2CM_vgap;   //	GAP IN
    if (myVars->TestSucPath == 1)
        SUCS_DYDT[0] = SUCS_DYDT[0] + vdhap + vgap - (vdhap_ins + vgap_ins);
    
    if (myVars->TestSucPath == 0)
        SUCS_DYDT[0] = SUCS_DYDT[0];
    
    //;   //	T3Pc WY1905
    dxdt[23] = SUCS_DYDT[0];
    
    
    //global PS2CM_vpga;
    const double vpga = myVars->PS2CM_vpga;
    
    //global SUCS2CM_vpga;
    const double vpga_ins = myVars->SUCS2CM_vpga;                                       //	PGA export from chloroplast
    
    SUCS_DYDT[11] = SUCS_DYDT[11] - vpga_ins + vpga;//	pgaC
    dxdt[34] = SUCS_DYDT[11];
     */
    for (int i = 0; i < 36; i++)
        dxdt[i] = ddxdt[i];
    return 0;
}
