#pragma once
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

namespace ePhotosynthesis {
/**
 Structure to store the Michaelis-Menten kinetic parameters
 */
class KValue {
public:
    double KmCO2_1 = 0.;
    double Ke_1 = 0.;
    double KmHCO3_2 = 0.;
    double KmPEP_2 = 0.;
    double Kimal_2 = 0.;
    double KmNADPH_3 = 0.;
    double KmOAA_3 = 0.;
    double KmNADP_3 = 0.;
    double Kmmal_3 = 0.;
    double Ke_3 = 0.;
    double KmCO2_4 = 0.;
    double KmNADP_4 = 0.;
    double KmNADPH_4 = 0.;
    double KmPyr_4 = 0.;
    double Kmmal_4 = 0.;
    double Ke_4 = 0.;
    double KiPEP_5 = 0.;
    double KmATP_5 = 0.;
    double KmPyr_5 = 0.;
    double KmCO2_6 = 0.;
    double KmO2_6 = 0.;
    double KmRuBP_6 = 0.;
    double KiPGA_6 = 0.;
    double KiFBP_6 = 0.;
    double KiSBP_6 = 0.;
    double KiPi_6 = 0.;
    double KiNADPH_6 = 0.;
    double KmADP_7 = 0.;
    double KmATP_7 = 0.;
    double KmPGA_7 = 0.;
    double KmDPGA_8 = 0.;
    double KmNADPH_8 = 0.;
    double Ke_9 = 0.;
    double KmDHAP_10 = 0.;
    double KmFBP_10 = 0.;
    double KmGAP_10 = 0.;
    double Ke_10 = 0.;
    double KiF6P_11 = 0.;
    double KiPi_11 = 0.;
    double KmFBP_11 = 0.;
    double Ke_11 = 0.;
    double KmDHAP_12 = 0.;
    double KmE4P_12 = 0.;
    double Ke_12 = 0.;
    double KiPi_13 = 0.;
    double KmSBP_13 = 0.;
    double Ke_13 = 0.;
    double KmE4P_14 = 0.;
    double KmF6P_14 = 0.;
    double KmGAP_14 = 0.;
    double KmXu5P = 0.;
    double Ke_14 = 0.;
    double KmGAP_15 = 0.;
    double KmRi5P_15 = 0.;
    double KmS7P_15 = 0.;
    double KmXu5P_15 = 0.;
    double Ke_15 = 0.;
    double Ke_16 = 0.;
    double Ke_17 = 0.;
    double KiADP_18 = 0.;
    double Ki_ADP_18 = 0.;
    double KiPGA_18 = 0.;
    double KiPi_18 = 0.;
    double KiRuBP_18 = 0.;
    double KmATP_18 = 0.;
    double KmRu5P_18 = 0.;
    double Ke_18 = 0.;
    double KmADP_7Mchl = 0.;
    double KmATP_7Mchl = 0.;
    double KmPGA_7Mchl = 0.;
    double KmDPGA_8Mchl = 0.;
    double KmNADPH_8Mchl = 0.;
    double KiADP_Starch = 0.;
    double KmATP_Starch = 0.;
    double KmG1P_Starch = 0.;
    double KaF6P_Starch = 0.;
    double KaFBP_Starch = 0.;
    double KaPGA_Starch = 0.;
    double Ke_Starch1 = 0.;
    double Ke_Starch2 = 0.;
    double KmPGA_PGASink = 0.;
    double KmDHAP_Suc1 = 0.;
    double KmGAP_Suc1 = 0.04 / 2.;
    double KmFBP_Suc1 = 0.;
    double Ke_Suc1 = 0.;
    double KiF26BP_Suc2 = 0.;
    double KiF6P_Suc2 = 0.;
    double KiPi_Suc2 = 0.;
    double KmFBP_Suc2 = 0.;
    double Ke_Suc2 = 0.;
    double Ke_Suc5 = 0.;
    double Ke_Suc6 = 0.;
    double KmG1P_Suc7 = 0.;
    double KmPPi_Suc7 = 0.;
    double KmUDPG_Suc7 = 0.;
    double KmUTP_Suc7 = 0.;
    double Ke_Suc7 = 0.;
    double KiFBP_Suc8 = 0.;
    double KiPi_Suc8 = 0.;
    double KiSuc_Suc8 = 0.;
    double KiSucP_Suc8 = 0.;
    double KiUDP_Suc8 = 0.;
    double KmF6P_Suc8 = 0.;
    double KmUDPG_Suc8 = 0.;
    double Ke_Suc8 = 0.;
    double KmSuc_Suc9 = 0.;
    double KmSucP_Suc9 = 0.;
    double Ke_Suc9 = 0.;
    double KmSuc_Suc10 = 0.;
    double KiADP_Suc3 = 0.;
    double KIDHAP_Suc3 = 0.;
    double KmATP_Suc3 = 0.;
    double KmF26BP_Suc3 = 0.;
    double KmF6P_Suc3 = 0.;
    double Ke_Suc3 = 0.;
    double KiF6P_Suc4 = 0.;
    double KiPi_Suc4 = 0.5 * 10.;
    double KmF26BP_Suc4 = 0.;
    double KePi = 0.;
    double KmADP_ATPM = 0.;
    double KmATP_ATPM = 0.;
    double KmPi_ATPM = 0.;
    double X = 0.;
    double Y = 0.;
    double F = 0.;
    double Q = 0.;
    double D = 0.;
    double Ke_ATPM = 0.;
    double KmNADP_NADPHM = 0.;
    double KmNADPH_NADPHM = 0.;
    double Ke_NADPHM = 0.;
    double E = 0.;
    double KmADP_ATPB = 0.;
    double KmPi_ATPB = 0.;
    double KmATP_ATPB = 0.;
    double Ke_ATPB = 0.;
    double G = 0.;
    double KmNADP_NADPHB = 0.;
    double KmNADPH_NADPHB = 0.;
    double Ke_NADPHB = 0.;
    double Voaa = 0.;
    double Vmal = 0.;
    double Vpyr = 0.;
    double Vpep = 0.;
    double Vt = 0.;
    double Vleak = 0.;
    double Vpga = 0.;
    double KmCO2_PR1 = 0.;
    double KmO2_PR1 = 0.;
    double KmRuBP_PR1 = 0.;
    double KiPGA_PR1 = 0.;
    double KiFBP_PR1 = 0.;
    double KiSBP_PR1 = 0.;
    double KiPi_PR1 = 0.9 * 3.;
    double KiNADPH_PR1 = 0.;
    double KmPGCA_PR2 = 0.;
    double KiPI_PR2 = 0.;
    double KiGCA_PR2 = 0.;
    double KmGCA_PR3 = 0.;
    double Ke_PS4 = 0.;
    double KmGOA_PS4 = 0.;
    double KmGLU_PS4 = 0.;
    double KiGLY_PS4 = 0.;
    double KmGLY_PS5 = 0.;
    double KiSER_PS5 = 0.;
    double Ke_PR6 = 0.;
    double KmGOA_PR6 = 0.;
    double KmSER_PR6 = 0.;
    double KmGLY_PR6 = 0.;
    double Ke_PR7 = 0.;
    double KiHPR_PR7 = 0.;
    double KmHPR_PR7 = 0.;
    double Ke_PR8 = 0.;
    double KmATP_PR8 = 0.;
    double KmGCEA_PR8 = 0.;
    double KiPGA_PR8 = 0.;
    double KmGCA_PR9 = 0.;
    double KiGCEA_PR9 = 0.;
    double KmGCEA_PR10 = 0.;
    double KiGCA_PR10 = 0.;
    double KmPGA_62 = 0.;
    double KmPEP_62 = 0.;
    double Ke_62 = 0.;

};

}
