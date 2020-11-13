#include "globals.hpp"

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

void IniModelCom(varptr *myVars) {
    
    //global RuACT_EPS_com;
    myVars->RuACT_EPS_com = false;
    
    //global BF_FI_com;            // The combination of BF and FI model
    myVars->BF_FI_com = false;
    
    //global PR_PS_com;    // This is a variable indicating whether the PR model is actually need to be combined with PS or not. If 1 then means combined; 0 means not.
    myVars->PR_PS_com = false;
    
    //global FIBF_PSPR_com; // 1 means that the overall EPS model is used. 0 means partial model of FIBF is used.
    myVars->FIBF_PSPR_com = false;
    
    //global ATPActive;
    myVars->ATPActive = 0;
    
    //global RedoxReg_RA_com;
    myVars->RedoxReg_RA_com = false;
    
    //global XanCycle_BF_com;
    myVars->XanCycle_BF_com = false;
    
    //global RROEA_EPS_com;
    myVars->RROEA_EPS_com = false;
    
    //global StomCond_TrDynaPS_com;
    myVars->StomCond_TrDynaPS_com = false;
    
    //global PSPR_SUCS_com;
    myVars->PSPR_SUCS_com = false;
    
    //global trDynaPS_SUCS_com;
    //myVars->trDynaPS_SUCS_com = false;  // --unused
    
    //global EPS_SUCS_com;
    myVars->EPS_SUCS_com = false;
    
    //const double ModelComb = 1;
    //return ModelComb;
}
