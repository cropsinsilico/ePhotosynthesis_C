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
#include "modules/AE_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

double AE::TIME = 0.;
std::size_t AE::N = 1;

AECondition* AE::_init(Variables *theVars) {

    AECondition* AE_con = new AECondition();

    //Initial value

    AE_con->Mchl_ActATPsynthase = 0.;
    AE_con->Mchl_ActGAPDH = 0.;
    AE_con->Mchl_ActNADPMDH = 0.;
    AE_con->Bchl_ActATPsynthase = 3.;
    AE_con->Bchl_ActPEPC = 0.05;
    AE_con->Bchl_ActGAPDH = 0.3;
    AE_con->Bchl_ActFBPase = 0.3;
    AE_con->Bchl_ActSBPase = 0.3;
    AE_con->Bchl_ActPRK = 0.3;
    AE_con->Bchl_ActRubisco = 0.1;
    AE_con->Bchl_ActRCA = 0.05;

    return AE_con;
}

void AE::_reset() {

}
