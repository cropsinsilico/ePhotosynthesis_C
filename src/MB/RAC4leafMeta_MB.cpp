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
#include "globals.hpp"
#include "modules/RAC4leafMeta.hpp"
#include "modules/Leaf_C4.hpp"
#include "modules/AE_C4.hpp"
#include "modules/Enzyme_C4.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::modules;
using namespace ePhotosynthesis::conditions;

const double t00 = 300;// --unused
const double Jsen = 1.;

RAC4leafMetaCondition* RAC4leafMeta::_MB_con(const double t, const RAC4leafMetaCondition* RAC_con, Variables *theVars) {
    //arr temp = RAC_con->_toArray();
    //std::cout << "t = " << t << std::endl << "    ";
    //for (auto x : temp) {
    //    std::cout << "    " << x << std::endl;
    //}
    //std::cout << std::endl;
    if (t < t00)
        theVars->TestLi = 0 / CONVERT * Jsen;

    if (t >= t00)
        theVars->TestLi = theVars->Radiation_PARo / CONVERT * Jsen;
    Leaf::TempResponseEnzymes(theVars, RAC_con->Leaf_con->Tleaf);
    // if t>=t00+1800&&t<t00+3600
    //     theVars->TestLi = 200/Convert*Jsen;
    // end
    // if t>=t00+3600&&t<t00+5400
    //     theVars->TestLi = 1800/Convert*Jsen;
    // end

    LeafCondition* ldydt = Leaf::MB_con(t, RAC_con->Leaf_con, theVars);
    AECondition* adydt = AE::MB_con(t, RAC_con->AE_con, theVars);
    EnzymeCondition* edydt = Enzyme::MB_con(t, RAC_con->Enzyme_con, theVars);

    RAC4leafMetaCondition* dydt = new RAC4leafMetaCondition(ldydt, edydt, adydt);
#ifdef INCDEBUG
    DEBUG_DELTA(dydt)
#endif

    // ////////////////////////////////////////////////
    // //Redox regulaltion ODEs
    // //Delta_Mchl_ActThioredoxin=(vReTRX_M-vReATPsyn_M-vReGAPDH_M-vReNADPMDH)/VolMchl;
    // Delta_Mchl_ActATPsynthase=(vReATPsyn_M)/VolMchl;
    // Delta_Mchl_ActGAPDH=(vReGAPDH_M)/VolMchl;
    // Delta_Mchl_ActNADPMDH=(vReNADPMDH)/VolMchl;
    //
    // //Delta_Bchl_ActThioredoxin=(vReTRX_B-vReATPsyn_B-vReRca_B-vReGAPDH_B+vReFBPase_B+vReSBPase_B+vRePRK_B)/VolBchl;
    // Delta_Bchl_ActATPsynthase=(vReATPsyn_B)/VolBchl;
    // Delta_Bchl_ActRca=(vReRca_B)/VolBchl;
    // Delta_Bchl_ActGAPDH=(vReGAPDH_B)/VolBchl;
    // Delta_Bchl_ActFBPase=(vReFBPase_B)/VolBchl;
    // Delta_Bchl_ActSBPase=(vReSBPase_B)/VolBchl;
    // Delta_Bchl_ActPRK=(vRePRK_B)/VolBchl;
    // Delta_Bchl_ActRubisco=(vPiRubisco)/VolBchl;
    //
    // //Enzyme activation
    //##LM_DYDT(99:109,1)=AEMB;
    // ////////////////////////////////////////////////
    // //Redox regulaltion ODEs
    // Delta_Mchl_ActThioredoxin=vReTRX_M-vReATPsyn_M-vReGAPDH_M-vReNADPMDH;
    // Delta_Mchl_ActATPsynthase=vReATPsyn_M;
    // Delta_Mchl_ActGAPDH=vReGAPDH_M;
    // Delta_Mchl_ActNADPMDH=vReNADPMDH;
    // Delta_Bchl_ActATPsynthase=vReATPsyn_M;
    // Delta_Bchl_ActRca=vReRca_B;
    // Delta_Bchl_ActGAPDH=vReGAPDH_B;
    // Delta_Bchl_ActFBPase=vReFBPase_B;
    // Delta_Bchl_ActSBPase=vReSBPase_B;
    // Delta_Bchl_ActPRK=vRePRK_B;
    // Delta_Bchl_ActRubisco=vPiRubisco;
    // RDMB=zeros(11,1);
    // RDMB(1)=Delta_Mchl_ActThioredoxin;
    // RDMB(2)=Delta_Mchl_ActATPsynthase;
    // RDMB(3)=Delta_Mchl_ActGAPDH;
    // RDMB(4)=Delta_Mchl_ActNADPMDH;
    // RDMB(5)=Delta_Bchl_ActATPsynthase;
    // RDMB(6)=Delta_Bchl_ActRca;
    // RDMB(7)=Delta_Bchl_ActGAPDH;
    // RDMB(8)=Delta_Bchl_ActFBPase;
    // RDMB(9)=Delta_Bchl_ActSBPase;
    // RDMB(10)=Delta_Bchl_ActPRK;
    // RDMB(11)=Delta_Bchl_ActRubisco;

    //vCO2s=reaction_flux(3);

    // //leaf air space 20//
    // Vol_airspace=0.04;//L
    // Molar_Volume=22.4/273*(WeatherTemperature+273);
    // LM_DYDT(m)=(vCO2s-Ameta)/(Vol_airspace/Molar_Volume);
    //std::cout << "  END" << std::endl << "  ";
    //arr temp2 = dydt->_toArray();
    //for (auto x : temp2) {
    //    std::cout << "    " << x << std::endl;;
    //}
    //std::cout << std::endl;
    return dydt;
}

arr RAC4leafMeta::_MB(const double t, const RAC4leafMetaCondition* const condition, Variables *theVars) {
    RAC4leafMetaCondition *dydt = _MB_con(t, condition, theVars);
    arr tmp = dydt->toArray();
    delete dydt;
    return tmp;
}
