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

#include "definitions.hpp"

/**
 Class for holding the results of RuACT_Rate calculations
 */
class RuACTVel {
public:
    RuACTVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTVel object to copy
      */
    RuACTVel(const RuACTVel &other) {
        v1 = other.v1;
        vn1 = other.vn1;
        v7 = other.v7;
        vn7 = other.vn7;
        v6_1 = other.v6_1;
        v6_2 = other.v6_2;
    }
    double v1 = 0.;
    double vn1 = 0.;
    double v7 = 0.;
    double vn7 = 0.;
    double v6_1 = 0.;
    double v6_2 = 0.;
};

/**
 Class for RuACT_RC data
 */
class RuACTRC {
public:
    RuACTRC() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTRC object to copy
      */
    RuACTRC(const RuACTRC &other) {

        k1 = other.k1;
        kn1 = other.kn1;
        km1 = other.km1;
        Ke2 = other.Ke2;
        Ke3 = other.Ke3;
        k6 = other.k6;
        kc = other.kc;
        ko = other.ko;
        k7 = other.k7;
        kr = other.kr;
    }

    double k1 = 0;
    double kn1 = 0; ///< The rate constant of E inactivation by binding of RuBP; Lazar 1999, with a lifetime of 5 ns at closed reaction center
    double km1 = 0;
    double Ke2 = 0; ///< Data from Mate et al 1996. Unit: micormolar; Reference needed, a guess
    double Ke3 = 0; ///< Data from Mate et al 1996. Unit: micormolar;
    double k6 = 0;  ///< micromolar per meter square per second, transfered to unit
    double kc = 0;  ///< Michaelis menton constant for CO2
    double ko = 0;  ///< Michaelis menton constant for O2
    double k7 = 0;  ///< The rate constant for ecm to ecmr
    double kr = 0;
};

/**
 Class for RuACT_Pool data
 */
class RuACTPool {
public:
    RuACTPool() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTPool object to copy
      */
    RuACTPool(const RuACTPool &other) {

        ET = other.ET;
        Rac = other.Rac;
        C = other.C;
        O = other.O;
        M = other.M;
}

    double ET = 0;
    double Rac = 0;
    double C = 0;
    double O = 0;
    double M = 0;
};

/**
 Class for holding the inputs to RuACT_mb
 */
class RuACTCon {
public:
    RuACTCon() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTCon object to copy
      */
    RuACTCon(const RuACTCon &other) {
        ER = other.ER;
        Eaf = other.Eaf;
        ECMR = other.ECMR;
        RuBP = other.RuBP;
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RuACTCon(const arr vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr vec, size_t offset = 0){
        ER = vec[offset];
        Eaf = vec[offset + 1];
        ECMR = vec[offset + 2];
        RuBP = vec[offset + 3];
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr array = {ER, Eaf, ECMR, RuBP};
        return array;
    }
    /**
      Get the size of the data vector
      */
    size_t size() {
        return count;
    }
    double ER = 0.;   ///< The concentration of inactive ER
    double Eaf = 0.;  ///< The total concentration of E, EC, AND ECM
    double ECMR = 0.; ///< The concentration of ECMR
    double RuBP = 0.; ///< The concentration of ECMR
private:
    size_t count = 4;
};

/**
 Class for RuACT related functions
 */
class RuACT {
public:
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A RuACTCon object with values set base on the input
      */
    static RuACTCon RuACT_Ini(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param RuACT_Con RuACTCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr RuACT_Mb(const double t, const RuACTCon &RuACT_Con, Variables *theVars);

    /**
      Calculate the Rates of RuACT based on the inputs

      @param t The current timestamp
      @param RuACT_Con RuACTCon object giving the input parameters
      @param theVars The global variables
      */
    static void RuACT_Rate(const double t, const RuACTCon &RuACT_Con, Variables *theVars);
};
