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
 Class for holding the inputs to PS_mb
 */
class PSCon {
public:
    PSCon(PS_PRCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The PSCon object to copy
      */
    PSCon(const PSCon* other) {
        RuBP = other->RuBP;
        PGA = other->PGA;
        DPGA = other->DPGA;
        T3P = other->T3P;
        ADPG = other->ADPG;
        FBP = other->FBP;
        E4P = other->E4P;
        S7P = other->S7P;
        SBP = other->SBP;
        ATP = other->ATP;
        NADPH = other->NADPH;
        CO2 = other->CO2;
        O2 = other->O2;
        HexP = other->HexP;
        PenP = other->PenP;
        _Pi = other->_Pi;
        _ADP = other->_ADP;
        _v1 = other->_v1;
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
    static size_t size() {
        return count;
    }

    void setParent(PS_PRCon* par) {parent = par;}
    friend std::ostream& operator<<(std::ostream &out, const PSCon &in);

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
    PS_PRCon* parent;
private:
    static const size_t count;
};

