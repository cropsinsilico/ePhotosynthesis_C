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
#include "BF.hpp"

arr BF::BF_Mb(double t, BFCon &BF_con, Variables *myVars) {
    Condition(t, myVars);

    myVars->BF_Param[0] = myVars->GLight;

    //////////////////////////////////////////////////////////////////
    //   Calculate the rates BFrst   //
    //////////////////////////////////////////////////////////////////
    BF_Rate(t, BF_con, myVars);

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation //
    ////////////////////////////////////////////////////////////////
    arr BF_mb = zeros(29);

    BF_mb[0] = myVars->BF_Vel.Vbf2 - myVars->BF_Vel.Vbf8;//	ISPHr	The reduced ion sulfer protein (ISPH)
    BF_mb[1] = myVars->BF_Vel.Vbf9 - myVars->BF_Vel.Vbf8;//	cytc1	The oxidized state of cytc1
    BF_mb[2] = myVars->BF_Vel.Vbf8 - myVars->BF_Vel.Vbf1;//	ISPo	The oxidized ion sulfer protein (ISP)
    BF_mb[3] = myVars->BF_Vel.Vbf1 - myVars->BF_Vel.Vbf2;//	ISPoQH2	The complex of oxidized ion sulfer protein and reduced quinone
    BF_mb[4] = myVars->BF_Vel.Vbf2 - myVars->BF_Vel.Vbf3;//	QHsemi	Semiquinone

    BF_mb[5] = myVars->BF_Vel.Vbf4 - myVars->BF_Vel.Vbf3;//	cytbL	The oxidized cytbL
    BF_mb[6] = myVars->BF_Vel.Vbf7 - myVars->BF_Vel.Vbf5 - myVars->BF_Vel.vcet;//	Qi	The quinone bound in the Qi site of cytbf complex  ????

    // DEBUG
    BF_mb[7] = myVars->BF_Vel.Vbf3 - myVars->BF_Vel.Vbf7 - myVars->BF_Vel.VgPQH2;//	Q	Quinone in thylakoid membrane in free form
    BF_mb[8] = myVars->BF_Vel.Vbf6 - myVars->BF_Vel.Vbf4 + myVars->BF_Vel.Vbf5;//	cytbH	The oxidized form of cytbH
    BF_mb[9] = myVars->BF_Vel.Vbf5 - myVars->BF_Vel.Vbf6 + myVars->BF_Vel.vcet;//	Qn	Q-
    BF_mb[10] = myVars->BF_Vel.Vbf6 - myVars->BF_Vel.Vqi / 2;//	Qr	Q2-
    BF_mb[11] = myVars->BF_Vel.Vqi / 2 - myVars->BF_Vel.Vbf1 + myVars->BF_Vel.VgPQH2;//	QH2	The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.

    BF_mb[12] = myVars->BF_Vel.Vbf10 - myVars->BF_Vel.Vbf9;//	cytc2	oxidized cytc2
    BF_mb[13] = myVars->BF_Vel.Vbf10 - myVars->BF_Vel.Vbf15;//	P700	The reduced state of P700, including both P700 and excited P700

    BF_mb[15] = 0;	//	Pi	Phosphate in stroma

    BF_mb[14] = myVars->BF_Vel.VsATP - myVars->BF_Vel.Vbf11;//	ADP	ADP in stroma
    BF_mb[16] = myVars->BF_Vel.Vbf11 - myVars->BF_Vel.VsATP;//	ATP	ATP in stroma


    BF_mb[17] = myVars->BF_Vel.JKc;	//	Ks	K ions in stroma
    BF_mb[18] = myVars->BF_Vel.JMgc;	//	Mgs	Mg ions in stroma
    BF_mb[19] = myVars->BF_Vel.JClc;	//	Cls	Cl ions in stroma

    BF_mb[20] = myVars->BF_Vel.Vicp + myVars->BF_Vel.Vinp - myVars->BF_Vel.Vipc - myVars->BF_Vel.Vdp - myVars->BF_Vel.Vfp;//	Aip	The number of photons in peripheral antenna
    BF_mb[21] = myVars->BF_Vel.Vipc + myVars->BF_Vel.Vinc - myVars->BF_Vel.Vicp - myVars->BF_Vel.Vdc - myVars->BF_Vel.Vfc;//	Ui	The number of photons in core antenna
    BF_mb[22] = myVars->BF_Vel.Vbf15 - myVars->BF_Vel.Vbf16;//	An:	the reduced electron acceptor in PSI
    //BF_mb	(	24	)	=	Vbf16 - myVars->BF_Vel.vbfn2 * CoeffVol	- myVars->BF_Vel.vcet;	//	Fdn	The reduced ferrodoxin; unit: mircomol m-2; Therefore,the the rate of NADPH formation need to micromole per meter square.
    BF_mb[23] = myVars->BF_Vel.Vbf16 / 2 - myVars->BF_Vel.vbfn2 / 2 * myVars->AVR - myVars->BF_Vel.vcet / 2;


    const double vqb = myVars->BF_Vel.VgPQH2 * 2;//	The rate of quinone protonation
    const double roe = myVars->BF_Vel.VgPQH2 * 2;//	The rate of proton generation from oxygen evolution complex

    const double Hroe = roe / myVars->AVR;//	Convert the unit of rate of oxygen evolution (roe) from micormole per meter square per second to mM s-1
    const double Hrqb = vqb / myVars->AVR;//	Convert the unit of vqb from micormole per meter square per second to mM s-1; vqb is the rate of QB2- reduction in thylakoid membrane.
    const double Hvqo1 = myVars->BF_Vel.Vbf8 / myVars->AVR;//	The rate of release of protons into lumen through Qo site
    const double Hvqo2 = myVars->BF_Vel.Vbf3 / myVars->AVR;//	The rate of proton release into lumen through Qo site
    const double Hvqi = myVars->BF_Vel.Vqi / myVars->AVR;//	The rate of proton uptake from stroma at Qi site of cytbc1 complex

    BF_mb[24] = (myVars->HPR * myVars->BF_Vel.Vbf11 - Hrqb - Hvqi - myVars->BF_Vel.vbfn2);//	BFHs	The proton and protonated buffer species in stroma. The proton concentration is not used in the MB procedure. The reason is that the proton concentration is buffered and therefore did not changed linerly with the generation of the protons.
    BF_mb[25] = (Hvqo1 + Hvqo2 + Hroe - myVars->HPR * myVars->BF_Vel.Vbf11);//	BFHl	The proton and protonated buffer species in lumen, similarly, we can only use the buff concentration, but, the proton concentration can not be used here.
    BF_mb[26] = -(myVars->HPR * myVars->BF_Vel.Vbf11 - Hrqb - Hvqi - myVars->BF_Vel.vbfn2) / 1000 / 0.015;//	PHs, The changes of PH in stoma, 0.03 mol /PH from Laisk et al.
    BF_mb[27] = -(Hvqo1 + Hvqo2 + Hroe - myVars->HPR * myVars->BF_Vel.Vbf11) / 1000 / 0.015;//   PHl  The changes in PH of lumen, 0.03 is from Curz et al., 2001, Biochemistry.
    BF_mb[28] = myVars->BF_Vel.vbfn2 - myVars->BF_Vel.VsNADPH;
    return BF_mb;
}
