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

#include "con/SUCSCon.hpp"
class PS;

/**
 Class for SUCS related functions and common variables
 */
class SUCS {
public:
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A SUCSCon object with values set base on the input
      */
    static SUCSCon* SUCS_Ini(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param SUCS_Con SUCSCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr SUCS_Mb(const double t, const SUCSCon* SUCS_Con, Variables *theVars);

    /**
      Calculate the Rates of SUCS based on the inputs

      @param t The current timestamp
      @param SUCS_Con SUCSCon object giving the input parameters
      @param theVars The global variables
      */
    static void SUCS_Rate(const double t, const SUCSCon* SUCS_Con, Variables *theVars);
    friend PS;
private:

    static double KE501;
    static double KE51;
    static double KE52;
    static double KE531;
    static double KE541;
    static double KE55;
    static double KE56;
    static double KE57;
    static double KE59;
    static double KE61;
    static double KI521;
    static double KI522;
    static double KI523;
    static double KI561;
    static double KI562;
    static double KI563;
    static double KI564;
    static double KI565;
    static double KI581;
    static double KI582;
    static double KI591;
    static double Ki572;
    static double Km511;
    static double Km512;
    static double Km513;
    static double Km521;
    static double Km551;
    static double Km552;
    static double Km553;
    static double Km554;
    static double Km561;
    static double Km562;
    static double Km571;
    static double Km581;
    static double Km591;
    static double Km593;
    static double Km621;
    static double V51;   ///< DHAP+GAP --FBP
    static double V52;   ///< FBP --F6P + Pi
    static double V55;   ///< G1P+UTP --OPOP+UDPG
    static double V56;   ///< UDPG+F6P--SUCP + UDP
    static double V57;   ///< SUCP--Pi + SUC
    static double V58;   ///< F26BP--F6P + Pi
    static double V59;   ///< F6P + ATP --ADP + F26BP
    static double V60;
    static double V61;
    static double V62;   ///< SUC Sink
    static double Vdhap_in; ///< DHAP export from chloroplast
    static double Vgap_in;  ///< GAP export from chloroplast
    static double Vpga_in;  ///< PGA export from chloropalst
    static double Km592;
    static double KI592;
    static double Km601;
    static double Km602;
    static double Km603;
    static double Km604;
    static double KE60;
    static double Vfactor51;
    static double Vfactor52;
    static double Vfactor56;
    static double Vfactor57;
    static double Vfactor59;
    static double Vf_T51;
    static double Vf_T52;
    static double Vf_T56;
    static double Vf_T57;
    static double Vf_T59;
};
