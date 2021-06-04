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

#include "ConBase.hpp"

class FIBFCon;

/**
 Class for holding the inputs to FI_mb
 */
class FICon : public ConBase<FICon> {
public:
    FICon(FIBFCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The FICon object to copy
      */
    FICon(const FICon* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    FICon(const arr &vec, const size_t offset = 0);

    void setParent(FIBFCon* par) {parent = par;}

    friend std::ostream& operator<<(std::ostream &out, const FICon &in);

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, const size_t offset = 0);

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return count;
    }

    void _clear() {}

    double A = 0.;          ///< The concentration of excitons in the peripheral antenna
    double U = 0.;          ///< The concentration fo excitons in the core antenna
    double P680ePheo = 0.;  ///< QF add
    double P680pPheon = 0.; ///< The concentration for the P680+ Pheo-
    double P680pPheo = 0.;  ///< The concentration of P680+ Pheo
    double P680Pheon = 0.;  ///< The concentration of P680Pheo-
    double Yz = 0.;         ///< The concentration of reduced tyrosine// --unused
    double S1T = 0.;        ///< The concentration of S1 in combination with reduced tyrosine
    double S2T = 0.;        ///< The concentration of S2 in combination with reduced tyrosine
    double S3T = 0.;        ///< The concentration of S3 in combination with reduced tyrosine
    double S0T = 0.;        ///< The concentration of S0 in combination with reduced tyrosine
    double S1Tp = 0.;       ///< The concentration of S1 in combination with oxidized tyrosine
    double S2Tp = 0.;       ///< The concentration of S2 in combination with oxidized tyrosine
    double S3Tp = 0.;       ///< The concentration of S3 in combination with oxidized tyrosine
    double S0Tp = 0.;       ///< The concentration of S0 in combination with oxidized tyrosine
    double QAQB = 0.;       ///< The concentration of [QAQB]
    double QAnQB = 0.;      ///< The concentration of [QA-QB]
    double QAQBn = 0.;      ///< The concentration of [QAQB-]
    double QAnQBn = 0.;     ///< The concentration of [QA-QB-]
    double QAQB2n = 0.;     ///< The concentration of [QAQB2-]
    double QAnQB2n = 0.;    ///< The concentration of [QA-QB2-]
    double PQn = 0.;        ///< The concentration of reduced PQ, i.e. PQH2;
    FIBFCon* parent;
private:
    static const size_t count;
};
