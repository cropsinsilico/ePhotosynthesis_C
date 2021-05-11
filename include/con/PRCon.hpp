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

class PS_PRCon;
/**
 Class for holding the inputs to PR_mb
 */
class PRCon {
public:
    PRCon(PS_PRCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PRCon object to copy
      */
    PRCon(const PRCon* other) {
        GCEA = other->GCEA;
        GCA = other->GCA;
        PGA = other->PGA;
        PGCA = other->PGCA;
        GCAc = other->GCAc;
        GOAc = other->GOAc;
        SERc = other->SERc;
        GLYc = other->GLYc;
        HPRc = other->HPRc;
        GCEAc = other->GCEAc;
        RuBP = other->RuBP;
        CO2 = other->CO2;
        O2 = other->O2;
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
        RuBP= vec[offset + 10];
        CO2= vec[offset + 11];
        O2= vec[offset + 12];
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr array = {GCEA, GCA, PGA, PGCA, GCAc, GOAc, SERc, GLYc, HPRc, GCEAc, RuBP, CO2, O2};
        return array;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return count;
    }
    void setParent(PS_PRCon* par) {parent = par;}
    friend std::ostream& operator<<(std::ostream &out, const PRCon &in);

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
    double RuBP = 0.;
    double CO2 = 0.;
    double O2 = 0.;
    double _v131;
    PS_PRCon* parent;
private:
    static const size_t count;

};
