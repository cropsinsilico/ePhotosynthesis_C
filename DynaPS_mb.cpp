#include "globals.hpp"
#include "DynaPS.hpp"
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


// This model includes the mass balance equations for the full model of photosynthesis.

std::vector<double> DynaPSmb(double t, DynaPSCon &DynaPS_con, varptr *myVars) {
    
    // Try out one new way of calculating the mass balance equation.
    // In this new way, all the previous calcuations of mass balance equation is preserved and only the necessary changes are made.
    
    //// Step One: Get the initialization of the concentrations for the RedoxReg model which will be used in the calculation of mb of RedoxReg.

    //arr RA_Con = zeros(92);
    //for (int m = 0; m < 92; m++)
    //    RA_Con[m] = x[m];
    
    //RACon RA_con(RA_Con);
    
    //arr XanCycle_Con = zeros(4);
    //for (int m = 0; m < 4; m++)
    //    XanCycle_Con[m] = x[m + 92];
    //XanCycleCon XanCycle_con(XanCycle_Con);
    
    // This is a sensitivity test to show that the model is stable udner fluctuating light
    
    const double light = 1;
    Condition(t, myVars);
    
    myVars->FI_Param[0] = light;
    myVars->BF_Param[0] = light;
    
    arr RA_DYDT = RA_mb(t, DynaPS_con.RA_con, myVars);
    arr XanCycle_DYDT = XanCycle_Mb(t, DynaPS_con.XanCycle_con, myVars);
    
    // Here get the rate of Thioredoxin reduction and oxidation and use it to construct the differential equation for both thio and fd.
    
    //arr DynaPS_DYDT = zeros(0);
    //global PRGlu;
    arr dxdt = zeros(96);
    for (int index = 0; index < 92; index++)
        dxdt[index] = RA_DYDT[index];
    
    
    for (int index = 0; index < 4; index++)
        dxdt[index + 92] = XanCycle_DYDT[index];
    
    // Temp = DynaPS_DYDT(24) -2*PRGlu;
    //DynaPS_DYDT(24) = Temp;
    return dxdt;
}
