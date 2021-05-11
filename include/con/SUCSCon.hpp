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

class CMCon;
/**
 Class for holding the inputs for SUCS_mb
 */
class SUCSCon {
public:
    SUCSCon(CMCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The SUCSCon object to copy
      */
    SUCSCon(const SUCSCon &other) {
        T3Pc = other.T3Pc;
        FBPc = other.FBPc;
        HexPc = other.HexPc;
        F26BPc = other.F26BPc;
        ATPc = other.ATPc;
        ADPc = other.ADPc;
        OPOPc = other.OPOPc;
        UDPGc = other.UDPGc;
        UTPc = other.UTPc;
        SUCP = other.SUCP;
        SUC = other.SUC;
        PGAc = other.PGAc;
    }

    SUCSCon(const SUCSCon *other) {
        T3Pc = other->T3Pc;
        FBPc = other->FBPc;
        HexPc = other->HexPc;
        F26BPc = other->F26BPc;
        ATPc = other->ATPc;
        ADPc = other->ADPc;
        OPOPc = other->OPOPc;
        UDPGc = other->UDPGc;
        UTPc = other->UTPc;
        SUCP = other->SUCP;
        SUC = other->SUC;
        PGAc = other->PGAc;
    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    SUCSCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, const size_t offset = 0) {
        T3Pc = vec[offset];
        FBPc = vec[offset + 1];
        HexPc = vec[offset + 2];
        F26BPc = vec[offset + 3];
        ATPc = vec[offset + 4];
        ADPc = vec[offset + 5];
        OPOPc = vec[offset + 6];
        UDPGc = vec[offset + 7];
        UTPc = vec[offset + 8];
        SUCP = vec[offset + 9];
        SUC = vec[offset + 10];
        PGAc = vec[offset + 11];
    }
    /**
      Set all data members to 0.
      */
    void clear() {
        T3Pc = 0.;
        FBPc = 0.;
        HexPc = 0.;
        F26BPc = 0.;
        ATPc = 0.;
        ADPc = 0.;
        OPOPc = 0.;
        UDPGc = 0.;
        UTPc = 0.;
        SUCP = 0.;
        SUC = 0.;
        PGAc = 0.;
    }
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr vec = {T3Pc, FBPc, HexPc, F26BPc, ATPc, ADPc, OPOPc, UDPGc, UTPc, SUCP, SUC, PGAc};
        return vec;
    }
    /**
      Get the size of the data vector
      */
    static size_t size() {
        return count;
    }

    void setParent(CMCon* par) {parent = par;}
    friend std::ostream& operator<<(std::ostream &out, const SUCSCon &in);

    double T3Pc = 0.;
    double FBPc = 0.;
    double HexPc = 0.;
    double F26BPc = 0.;
    double ATPc = 0.;
    double ADPc = 0.;
    double OPOPc = 0.;
    double UDPGc = 0.;
    double UTPc = 0.;
    double SUCP = 0.;
    double SUC = 0.;
    double PGAc = 0.;
    CMCon* parent;
private:
    static const size_t count;

};
