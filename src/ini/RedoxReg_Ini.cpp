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
#include "modules/RedoxReg.hpp"
#include "modules/RA.hpp"
#include "modules/PS.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double RedoxRegCondition::V13 = 0.;
double RedoxRegCondition::V16 = 0.;
double RedoxRegCondition::V6 = 0.;
double RedoxRegCondition::V9 = 0.;
double RedoxReg::TIME = 0.;
std::size_t RedoxReg::N = 1;
bool RedoxReg::trDynaPS2RedReg_cal = false;

DEFINE_MODULE_COMPOSITE(RedoxReg);

void RedoxReg::_initOrig(Variables *theVars,
			 RedoxRegCondition* RedoxReg_con) {

    const double Thion = 0.25;     // This is a wild guess
    
    RedoxReg_con->Thion = Thion;
    RedoxReg::Fd_Thio_ET = 500.;
    RedoxReg::ThioT = 0.5;
    RedoxReg::Thio_Oxidation = 0.1;

    RedoxReg::RedoxReg_VMAX6 = PS::getV6();
    RedoxReg::RedoxReg_VMAX9 = PS::getV9();
    RedoxReg::RedoxReg_VMAX13 = PS::getV13();
    RedoxReg::RedoxReg_VMAX16 = PS::getV16();

    for (int i = 0; i < 5; i++)
        theVars->RedoxReg_MP.push_back(zeros(3));
    theVars->RedoxReg_MP[0][0] = 1000.;
    theVars->RedoxReg_MP[0][1] = - 0.3;
    theVars->RedoxReg_MP[0][2] = 0.5;

    theVars->RedoxReg_MP[1][0] = 6.;             // FBPase
    theVars->RedoxReg_MP[1][1] = - 0.305;
    theVars->RedoxReg_MP[1][2] = 0.5;

    theVars->RedoxReg_MP[2][0] = 9.;             // SBPase
    theVars->RedoxReg_MP[2][1] = - 0.3;
    theVars->RedoxReg_MP[2][2] = 0.5;

    theVars->RedoxReg_MP[3][0] = 13.;            // PRK
    theVars->RedoxReg_MP[3][1] = - 0.295;
    theVars->RedoxReg_MP[3][2] = 0.5;

    theVars->RedoxReg_MP[4][0] = 16.;            // ATPase
    theVars->RedoxReg_MP[4][1] = - 0.28;
    theVars->RedoxReg_MP[4][2] = 0.5;

    //theVars->BF2RedoxReg_Fdt = theVars->BF_Pool.kU_f;
}

void RedoxReg::_initCalc(Variables *theVars, RedoxRegCondition* RedoxReg_con) {
    RedoxReg::RedoxReg_VMAX6 = PS::getV6();
    RedoxReg::RedoxReg_VMAX9 = PS::getV9();
    RedoxReg::RedoxReg_VMAX13 = PS::getV13();
    RedoxReg::RedoxReg_VMAX16 = PS::getV16();

    for (int i = 0; i < 5; i++)
        theVars->RedoxReg_MP.push_back(zeros(3));
    theVars->RedoxReg_MP[0][0] = 1000.;
    theVars->RedoxReg_MP[0][1] = - 0.3;
    theVars->RedoxReg_MP[0][2] = 0.5;

    theVars->RedoxReg_MP[1][0] = 6.;             // FBPase
    theVars->RedoxReg_MP[1][1] = - 0.305;
    theVars->RedoxReg_MP[1][2] = 0.5;

    theVars->RedoxReg_MP[2][0] = 9.;             // SBPase
    theVars->RedoxReg_MP[2][1] = - 0.3;
    theVars->RedoxReg_MP[2][2] = 0.5;

    theVars->RedoxReg_MP[3][0] = 13.;            // PRK
    theVars->RedoxReg_MP[3][1] = - 0.295;
    theVars->RedoxReg_MP[3][2] = 0.5;

    theVars->RedoxReg_MP[4][0] = 16.;            // ATPase
    theVars->RedoxReg_MP[4][1] = - 0.28;
    theVars->RedoxReg_MP[4][2] = 0.5;

    //theVars->BF2RedoxReg_Fdt = theVars->BF_Pool.kU_f;
}

DEFINE_DEFAULT_CHECKALT(RedoxReg)
