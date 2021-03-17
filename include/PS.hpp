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
class PRCon;
class PR;

/**
 Class for holding the inputs to PS_mb
 */
class PSCon {
public:
    PSCon() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PSCon object to copy
      */
    PSCon(const PSCon &other) {
        RuBP = other.RuBP;
        PGA = other.PGA;
        DPGA = other.DPGA;
        T3P = other.T3P;
        ADPG = other.ADPG;
        FBP = other.FBP;
        E4P = other.E4P;
        S7P = other.S7P;
        SBP = other.SBP;
        ATP = other.ATP;
        NADPH = other.NADPH;
        CO2 = other.CO2;
        O2 = other.O2;
        HexP = other.HexP;
        PenP = other.PenP;
        _Pi = other._Pi;
        _ADP = other._ADP;
        _v1 = other._v1;
    }

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    PSCon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        RuBP = vec[offset];
        PGA = vec[offset + 1];
        DPGA = vec[offset + 2];
        T3P = vec[offset + 3];
        ADPG = vec[offset + 4];
        FBP = vec[offset + 5];
        E4P = vec[offset + 6];
        S7P = vec[offset + 7];
        SBP = vec[offset + 8];
        ATP = vec[offset + 9];
        NADPH = vec[offset + 10];
        CO2 = vec[offset + 11];
        O2 = vec[offset + 12];
        HexP = vec[offset + 13];
        PenP = vec[offset + 14];
    }

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
    */
    arr toArray() {
        arr array = {RuBP, PGA, DPGA, T3P, ADPG, FBP, E4P, S7P, SBP, ATP, NADPH, CO2, O2, HexP, PenP};
        return array;
    }
    /**
      Get the size of the data vector
      */
    size_t size() {
        return count;
    }
    double RuBP = 0.;
    double PGA = 0.;
    double DPGA = 0.;
    double T3P = 0.;
    double ADPG = 0.;
    double FBP = 0.;
    double E4P = 0.;
    double S7P = 0.;
    double SBP = 0.;
    double ATP = 0.;
    double NADPH = 0.;
    double CO2 = 0.;
    double O2 = 0.;
    double HexP = 0.;
    double PenP = 0.;
    double _Pi = 0.;
    double _ADP = 0.;
    double _v1 = 0.;
private:
    size_t count = 15;
};

/**
 Class for holding the results of PS_Rate calculations
 */
class PSVel {
public:
    PSVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PSVel object to copy
      */
    PSVel(const PSVel &other) {
        v1 = other.v1;
        v2 = other.v2;
        v3 = other.v3;
        v4 = other.v4;
        v5 = other.v5;
        v6 = other.v6;
        v7 = other.v7;
        v8 = other.v8;
        v9 = other.v9;
        v10 = other.v10;
        v13 = other.v13;
        v16 = other.v16;
        v23 = other.v23;
        v31 = other.v31;
        v32 = other.v32;
        v33 = other.v33;
        Pi = other.Pi;
        v24 = other.v24;
        v25 = other.v25;
    }
    /**
      Overloaded time-equal operator to multiply all data mambers by a constant value

      @param val The value to multiply the data members by.
      */
    PSVel& operator*=(const double val) {
        this->v1 *= val;
        this->v2 *= val;
        this->v3 *= val;
        this->v4 *= val;
        this->v5 *= val;
        this->v6 *= val;
        this->v7 *= val;
        this->v8 *= val;
        this->v9 *= val;
        this->v10 *= val;
        this->v13 *= val;
        this->v16 *= val;
        this->v23 *= val;
        this->v31 *= val;
        this->v32 *= val;
        this->v33 *= val;
        this->Pi *= val;
        this->v24 *= val;
        this->v25 *= val;
        return *this;
    }
    double v1 = 0.;
    double v2 = 0.;
    double v3 = 0.;
    double v4 = 0.;
    double v5 = 0.;
    double v6 = 0.;
    double v7 = 0.;
    double v8 = 0.;
    double v9 = 0.;
    double v10 = 0.;
    double v13 = 0.;
    double v16 = 0.;
    double v23 = 0.;
    double v31 = 0.;
    double v32 = 0.;
    double v33 = 0.;
    double Pi = 0.;
    double v24 = 0.;
    double v25 = 0.;

};

/**
 Class to for PS related functions and common variables
 */
class PS{
public:
    /**
      Calculate the output values based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr PS_Mb(const double t, const PSCon &PSs, const arr &Param, Variables *theVars);
    static PSCon PSI(Variables *theVars);

    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A PSCon object with values set base on the input
      */
    static PSCon PS_Ini(Variables *theVars);

    /**
      Calculate the Rates of PS based on the inputs

      @param t The current timestamp
      @param PSs PSCon object giving the input parameters
      @param theVars The global variables
      */
    static void PS_Rate(const double t, const PSCon &PSs, const arr &Param, Variables *theVars, const PRCon *prcon = nullptr);
private:
    friend PR;
    static double KA231;
    static double KE11;
    static double KE12;
    static double KE13;
    static double KE16;
    static double KE21;
    static double KE22;
    static double KE23;
    static double KE25;
    static double KE4;
    static double KE5;
    static double KE6;
    static double KE7;
    static double KE8;
    static double KE9;
    static double KI11;
    static double KI12;
    static double KI13;
    static double KI131;
    static double KI132;
    static double KI133;
    static double KI134;
    static double KI135;
    static double KI14;
    static double KI15;
    static double KI231;
    static double KI61;
    static double KI62;
    static double KI9;
    static double KM10;
    static double KM101;
    static double KM102;
    static double KM103;
    static double KM13;
    static double KM131;
    static double KM132;
    static double KM161;
    static double KM162;
    static double KM163;
    static double KM21;
    static double KM22;
    static double KM23;
    static double KM231;
    static double KM232;
    static double KM233;
    static double KM234;
    static double KM241;
    static double KM311;
    static double KM313;
    static double KM31a;
    static double KM32;
    static double KM32b;
    static double KM33;
    static double KM51;
    static double KM52;
    static double KM53;
    static double KM61;
    static double KM81;
    static double KM82;
    static double KM9;
    static double KVmo;
    static double PS_C_CA;
    static double PS_C_CP;
    static double PS_C_CN;
    static double PS_PEXT;
    static double V24;
    static double V31;
    static double V32;
    static double V33;
    static double V5;
    static double V7;
    static double V8;
    static double V10;
    static double KA232;
    static double KA233;
    static double KI23;
    static double KM312;
    static double KE10;
    static double KM11;
    static double KM12;
    static double KM71;
    static double KM72;
    static double KM73;
    static double KM74;
    static double Vfactor1;
    static double Vfactor2;
    static double Vfactor3;
    static double Vfactor5;
    static double Vfactor7;
    static double Vfactor13;
    static double Vfactor23;
    static double Vf_T3;
    static double Vf_T2;
    static double Vf_T1;
    static double Vf_T6;
    static double Vf_T5;
    static double Vf_T9;
    static double Vf_T13;
    static double Vf_T23;
    static double PsV1;
};
