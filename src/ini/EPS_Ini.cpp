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

#include "Variables.hpp"
#include "modules/EPS.hpp"
#include "modules/FIBF.hpp"
#include "modules/FI.hpp"
#include "modules/BF.hpp"
#include "modules/CM.hpp"

EPSContainer* EPS::_init(Variables *theVars) {
    FIBF::init(theVars);
    FIContainer* FI_Con = FI::init(theVars);
    BFContainer* BF_con = BF::init(theVars);
    FIBFContainer* FIBF_con = new FIBFContainer(BF_con, FI_Con);
    //// Step 1 Get the initialization of the variables for BF

    theVars->BF_OLD_TIME = 0;
    theVars->BF_TIME_N = 1;

    //// Get the initialization of the variables for FI

    theVars->FI_OLD_TIME = 0;
    theVars->FI_TIME_N = 1;
    theVars->FI_Pool.PQT = theVars->FIBF_Pool.PQT;
    theVars->BF_Pool.k_r1 = theVars->FIBF_Pool.PQT;

    // Second, try to get the iniitalzation files for the PSPR model

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Global variables used for obtaining flux and concentration data //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    theVars->PS_OLD_TIME = 0;
    theVars->PS_TIME_N = 1;
    theVars->PR_OLD_TIME = 0;
    theVars->PR_TIME_N = 1;

    ////////////////////////////////////////////////
    //   Initialation step //
    ////////////////////////////////////////////////

    CMContainer* CM_con = CM::init(theVars);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //   Use the initialized variables to construct variables that will be transfered to the Drive file. ////////////
    // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    EPSContainer* EPS_con = new EPSContainer(FIBF_con, CM_con);

    return EPS_con;
}
