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

class RACon;

/**
 Class for holding the inputs to RuACT_mb
 */
class RuACTCon {
public:
    RuACTCon(RACon *par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RuACTCon object to copy
      */
    RuACTCon(const RuACTCon* other) {
        ER = other->ER;
        Eaf = other->Eaf;
        ECMR = other->ECMR;
        RuBP = other->RuBP;
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
    static size_t size() {
        return count;
    }

    void setParent(RACon* par) {parent = par;}
    double ER = 0.;   ///< The concentration of inactive ER
    double Eaf = 0.;  ///< The total concentration of E, EC, AND ECM
    double ECMR = 0.; ///< The concentration of ECMR
    double RuBP = 0.; ///< The concentration of ECMR
    RACon* parent;
private:
    static const size_t count;

};
