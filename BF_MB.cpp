#include "Variables.hpp"
#include "globals.hpp"
#include "BF.hpp"
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

arr BF_Mb(double t, BFCon &BF_con, Variables *myVars) {
    Condition(t, myVars);
    const double light = myVars->GLight;

    myVars->BF_Param[0] = light;

    //////////////////////////////////////////////////////////////////
    //   Calculate the rates BFrst   //
    //////////////////////////////////////////////////////////////////
    BF_Rate(t, BF_con, myVars);

    ////////////////////////////////////////////////////////////////////////////////
    //   Get the rate of different reactions//
    ////////////////////////////////////////////////////////////////////////////////

    //	Assign velocities of different reactions in the model
    const double Vbf1 = myVars->BF_Vel.Vbf1 ;
    const double Vbf2 = myVars->BF_Vel.Vbf2 ;
    const double Vbf3 = myVars->BF_Vel.Vbf3 ;
    const double Vbf4 = myVars->BF_Vel.Vbf4 ;
    const double Vbf5 = myVars->BF_Vel.Vbf5 ;

    const double Vbf6 = myVars->BF_Vel.Vbf6 ;
    const double Vbf7 = myVars->BF_Vel.Vbf7 ;
    const double Vbf8 = myVars->BF_Vel.Vbf8 ;
    const double Vbf9 = myVars->BF_Vel.Vbf9 ;
    const double Vbf10 = myVars->BF_Vel.Vbf10 ;

    const double Vbf11 = myVars->BF_Vel.Vbf11 ;
    const double Vqi = myVars->BF_Vel.Vqi ;
    const double Vipc = myVars->BF_Vel.Vipc ;
    const double Vicp = myVars->BF_Vel.Vicp ;
    const double Vinc = myVars->BF_Vel.Vinc ;

    const double Vinp = myVars->BF_Vel.Vinp ;
    const double Vdp = myVars->BF_Vel.Vdp ;
    const double Vdc = myVars->BF_Vel.Vdc ;
    const double Vfp = myVars->BF_Vel.Vfp ;
    const double Vfc = myVars->BF_Vel.Vfc ;

    // Vsfd = myVars->BF_Vel.Vsfd ;// --unused
    const double VsATP = myVars->BF_Vel.VsATP ;
    const double VgPQH2 = myVars->BF_Vel.VgPQH2 ;
    const double Vbf12 = myVars->BF_Vel.JKc ;
    const double Vbf13 = myVars->BF_Vel.JMgc ;

    const double Vbf14 = myVars->BF_Vel.JClc ;
    const double Vbf15 = myVars->BF_Vel.Vbf15 ;
    const double Vbf16 = myVars->BF_Vel.Vbf16 ;
    const double vbfn2 = myVars->BF_Vel.vbfn2 ;
    const double VsNADPH = myVars->BF_Vel.VsNADPH ;
    const double vcet = myVars->BF_Vel.vcet ;

    ////////////////////////////////////////////////////////////////
    // Get the mass balance equation //
    ////////////////////////////////////////////////////////////////
    arr BF_mb = zeros(29);

    const double CoeffVol = myVars->AVR; // This is the conversion factor between volume and area.

    BF_mb[0] = Vbf2 - Vbf8;//	ISPHr	The reduced ion sulfer protein (ISPH)
    BF_mb[1] = Vbf9 - Vbf8;//	cytc1	The oxidized state of cytc1
    BF_mb[2] = Vbf8 - Vbf1;//	ISPo	The oxidized ion sulfer protein (ISP)
    BF_mb[3] = Vbf1 - Vbf2;//	ISPoQH2	The complex of oxidized ion sulfer protein and reduced quinone
    BF_mb[4] = Vbf2 - Vbf3;//	QHsemi	Semiquinone

    BF_mb[5] = Vbf4 - Vbf3;//	cytbL	The oxidized cytbL
    BF_mb[6] = Vbf7 - Vbf5 - vcet;//	Qi	The quinone bound in the Qi site of cytbf complex  ????

    // DEBUG
    BF_mb[7] = Vbf3 - Vbf7 - VgPQH2;//	Q	Quinone in thylakoid membrane in free form
    BF_mb[8] = Vbf6 - Vbf4 + Vbf5;//	cytbH	The oxidized form of cytbH
    BF_mb[9] = Vbf5 - Vbf6 + vcet;//	Qn	Q-
    BF_mb[10] = Vbf6 - Vqi / 2;//	Qr	Q2-
    BF_mb[11] = Vqi / 2 - Vbf1 + VgPQH2;//	QH2	The PQH2 concentration; the coefficient 2 represent the fact that 2 protons were taken up by one Q2-.

    BF_mb[12] = Vbf10 - Vbf9;//	cytc2	oxidized cytc2
    BF_mb[13] = Vbf10 - Vbf15;//	P700	The reduced state of P700, including both P700 and excited P700

    BF_mb[15] = 0;	//	Pi	Phosphate in stroma

    BF_mb[14] = VsATP - Vbf11;//	ADP	ADP in stroma
    BF_mb[16] = Vbf11 - VsATP;//	ATP	ATP in stroma


    BF_mb[17] = Vbf12;	//	Ks	K ions in stroma
    BF_mb[18] = Vbf13;	//	Mgs	Mg ions in stroma
    BF_mb[19] = Vbf14;	//	Cls	Cl ions in stroma

    BF_mb[20] = Vicp + Vinp - Vipc - Vdp - Vfp;//	Aip	The number of photons in peripheral antenna
    BF_mb[21] = Vipc + Vinc - Vicp - Vdc - Vfc;//	Ui	The number of photons in core antenna
    BF_mb[22] = Vbf15 - Vbf16;//	An:	the reduced electron acceptor in PSI
    //BF_mb	(	24	)	=	Vbf16 - vbfn2 * CoeffVol	- vcet;	//	Fdn	The reduced ferrodoxin; unit: mircomol m-2; Therefore,the the rate of NADPH formation need to micromole per meter square.
    BF_mb[23] = Vbf16 / 2 - vbfn2 / 2 * CoeffVol - vcet / 2;


    const double vqb = VgPQH2 * 2;//	The rate of quinone protonation
    const double roe = VgPQH2 * 2;//	The rate of proton generation from oxygen evolution complex

    const double Hroe = roe / CoeffVol;//	Convert the unit of rate of oxygen evolution (roe) from micormole per meter square per second to mM s-1
    const double Hrqb = vqb / CoeffVol;//	Convert the unit of vqb from micormole per meter square per second to mM s-1; vqb is the rate of QB2- reduction in thylakoid membrane.
    const double Hvqo1 = Vbf8 / CoeffVol;//	The rate of release of protons into lumen through Qo site
    const double Hvqo2 = Vbf3 / CoeffVol;//	The rate of proton release into lumen through Qo site
    const double Hvqi = Vqi / CoeffVol;//	The rate of proton uptake from stroma at Qi site of cytbc1 complex

    BF_mb[24] = (myVars->HPR * Vbf11 - Hrqb - Hvqi - vbfn2);//	BFHs	The proton and protonated buffer species in stroma. The proton concentration is not used in the MB procedure. The reason is that the proton concentration is buffered and therefore did not changed linerly with the generation of the protons.
    BF_mb[25] = (Hvqo1 + Hvqo2 + Hroe - myVars->HPR * Vbf11);//	BFHl	The proton and protonated buffer species in lumen, similarly, we can only use the buff concentration, but, the proton concentration can not be used here.
    BF_mb[26] = - (myVars->HPR * Vbf11 - Hrqb - Hvqi - vbfn2) / 1000 / 0.015;//	PHs, The changes of PH in stoma, 0.03 mol /PH from Laisk et al.
    BF_mb[27] = - (Hvqo1 + Hvqo2 + Hroe - myVars->HPR * Vbf11) / 1000 / 0.015;//   PHl  The changes in PH of lumen, 0.03 is from Curz et al., 2001, Biochemistry.
    BF_mb[28] = vbfn2 - VsNADPH;
    return BF_mb;
}
