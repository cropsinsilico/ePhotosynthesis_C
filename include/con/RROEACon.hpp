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

class trDynaPSCon;

/**
 Class for holding the inputs to RROEA_mb
 */
class RROEACon {
public:
    RROEACon(trDynaPSCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEACon object to copy
      */
    RROEACon(const RROEACon* other) {
        GAPDH = other->GAPDH;
        FBPase = other->FBPase;
        SBPase = other->SBPase;
        PRK = other->PRK;
        ATPase = other->ATPase;
        ATPGPP = other->ATPGPP;
        MDH = other->MDH;
        Thio = other->Thio;
        Fd = other->Fd;
        RuACT = other->RuACT;
    }

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RROEACon(const arr &vec, size_t offset = 0) {
        fromArray(vec, offset);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, size_t offset = 0) {
        GAPDH = vec[offset];
        FBPase = vec[offset + 1];
        SBPase = vec[offset + 2];
        PRK = vec[offset + 3];
        ATPase = vec[offset + 4];
        ATPGPP = vec[offset + 5];
        MDH = vec[offset + 6];
        Thio = vec[offset + 7];
        Fd = vec[offset + 8];
        RuACT = vec[offset + 9];
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return count;
    }

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr array = {GAPDH, FBPase, SBPase, PRK, ATPase, ATPGPP, MDH, Thio, Fd, RuACT};
        return array;
    }
    void setParent(trDynaPSCon* par) {parent = par;}
    double GAPDH = 0.;  ///< The initial concentration of active GAPDH
    double FBPase = 0.; ///< The initial concentration of active FBPase
    double SBPase = 0.; ///< The initial concentration of active SBPase
    double PRK = 0.;    ///< The initial concentration of actove PRK
    double ATPase = 0.; ///< The initial concentration of actove ATPase
    double ATPGPP = 0.; ///< The initial concentration of actove ATPGPP
    double MDH = 0.;    ///< The initial concentration of actove MDH
    double Thio = 0.;   ///< The initial concentration of reduced thioredoxin
    double Fd = 0.;     ///< The initial concentration of reduced ferrodoxin
    double RuACT = 0.;  ///< The initial concentration of active Rubisco activase

    trDynaPSCon* parent;
private:
    static const size_t count;

};
