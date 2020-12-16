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
 Class for holding the inputs to PR_mb
 */
class PRCon {
public:
    PRCon() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PRCon object to copy
      */
    PRCon(const PRCon &other) {
        GCEA = other.GCEA;
        GCA = other.GCA;
        PGA = other.PGA;
        PGCA = other.PGCA;
        GCAc = other.GCAc;
        GOAc = other.GOAc;
        SERc = other.SERc;
        GLYc = other.GLYc;
        HPRc = other.HPRc;
        GCEAc = other.GCEAc;
        RUBP = other.RUBP;
        CO2 = other.CO2;
        O2 = other.O2;
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    PRCon(const arr vec, size_t offset = 0) {
        fromArray(vec, offset);
    }
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr vec, size_t offset = 0){
        GCEA= vec[offset];
        GCA= vec[offset + 1];
        PGA= vec[offset + 2];
        PGCA= vec[offset + 3];
        GCAc= vec[offset + 4];
        GOAc= vec[offset + 5];
        SERc= vec[offset + 6];
        GLYc= vec[offset + 7];
        HPRc= vec[offset + 8];
        GCEAc= vec[offset + 9];
        RUBP= vec[offset + 10];
        CO2= vec[offset + 11];
        O2= vec[offset + 12];
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr array = {GCEA, GCA, PGA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc, RUBP, CO2, O2};
        return array;
    }
    /**
      Get the size of the data vector
      */
    size_t size() {
        return count;
    }
    double GCEA = 0.;
    double GCA = 0.;
    double PGA = 0.;
    double PGCA = 0.;
    double GCAc = 0.;
    double GOAc = 0.;
    double SERc = 0.;
    double GLYc = 0.;
    double HPRc = 0.;
    double GCEAc = 0.;
    double RUBP = 0.;
    double CO2 = 0.;
    double O2 = 0.;
    double _v131;
private:
    size_t count = 13;
};

/**
 Class for holding the result of the PR_Rate calculations
 */
class PRVel {
public:
    PRVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PRVel object to copy
      */
    PRVel(const PRVel &other) {
        v111 = other.v111;
        v112 = other.v112;
        v113 = other.v113;
        v121 = other.v121;
        v122 = other.v122;
        v123 = other.v123;
        v124 = other.v124;
        v131 = other.v131;
        v1in = other.v1in;
        v2out = other.v2out;
    }
    /**
      Overload of the multiplication-equals operator to multiply all members of
      the class by a constant

      @param val The constant to multiply all the members by
      */
    PRVel& operator*=(const double val) {
        this->v111 *= val;
        this->v112 *= val;
        this->v113 *= val;
        this->v121 *= val;
        this->v122 *= val;
        this->v123 *= val;
        this->v124 *= val;
        this->v131 *= val;
        this->v1in *= val;
        this->v2out *= val;
        return *this;
    }
    double v111 = 0.;
    double v112 = 0.;
    double v113 = 0.;
    double v121 = 0.;
    double v122 = 0.;
    double v123 = 0.;
    double v124 = 0.;
    double v131 = 0.;
    double v1in = 0.;
    double v2out = 0.;
};

/**
  Class for grouping PR related functions and common variables
  */
class PR {
public:
    /**
      Initializer

      @param theVars Pointer to the global variables
      @return A PRCon object with values set base on the input
      */
    static PRCon PR_Ini(Variables *theVars);

    /**
      Calculate the output values based on the inputs

      @param PR_con PRCon object giving the input parameters
      @param theVars The global variables
      @return A vector containing the updated values
      */
    static arr PR_Mb(const double t, const PRCon &PR_con, Variables *theVars);

    /**
      Calculate the Rates of PR based on the inputs

      @param PR_con PRCon object giving the input parameters
      @param theVars The global variables
      */
    static void PR_Rate(const double t, const PRCon &PR_con, Variables *theVars);
private:
    static double KC;
    static double KE113;
    static double KE122;
    static double KE123;
    static double KE124;
    static double KGc;
    static double KI1011;
    static double KI1012;
    static double KI1121;
    static double KI1122;
    static double KI113;
    static double KI1221;
    static double KI123;
    static double KI1311;
    static double KM1011;
    static double KM1012;
    static double KM112;
    static double KM1131;
    static double KM1132;
    static double KM121;
    static double KM1221;
    static double KM1222;
    static double KM123;
    static double KM1241;
    static double KM1242;
    static double KM1311;
    static double KO;
    static double KR;
    static double NADHc;
    static double NADc;
    static double PR_ADP;
    static double PR_ATP;
    static double V111;
    static double V112;
    static double V113;
    static double V121;
    static double V122;
    static double V123;
    static double V124;
    static double V131;
    static double V2T;
};
