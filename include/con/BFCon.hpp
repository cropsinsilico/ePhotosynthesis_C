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

class FIBFCon;

/**
 Class for holding the inputs to BF_mb
 */
class BFCon {
public:
    BFCon(FIBFCon* par = nullptr) : parent(par) {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCon object to copy
      */
    BFCon(const BFCon* other){
        ISPHr = other->ISPHr;
        cytc1 = other->cytc1;
        ISPo = other->ISPo;
        ISPoQH2 = other->ISPoQH2;
        QHsemi = other->QHsemi;
        cytbL = other->cytbL;
        Qi = other->Qi;
        Q = other->Q;
        cytbH = other->cytbH;
        Qn = other->Qn;
        Qr = other->Qr;
        QH2 = other->QH2;
        cytc2 = other->cytc2;
        P700 = other->P700;
        ADP = other->ADP;
        Pi = other->Pi;
        ATP = other->ATP;
        Ks = other->Ks;
        Mgs = other->Mgs;
        Cls = other->Cls;
        Aip = other->Aip;
        U = other->U;
        An = other->An;
        Fdn = other->Fdn;
        BFHs = other->BFHs;
        BFHl = other->BFHl;
        PHs = other->PHs;
        PHl = other->PHl;
        NADPH = other->NADPH;

    }
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    BFCon(const arr &vec, const size_t offset = 0) {
        fromArray(vec, offset);
    }

    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void fromArray(const arr &vec, const size_t offset = 0) {
        ISPHr = vec[offset];
        cytc1 = vec[offset + 1];
        ISPo = vec[offset + 2];
        ISPoQH2 = vec[offset + 3];
        QHsemi = vec[offset + 4];
        cytbL = vec[offset + 5];
        Qi = vec[offset + 6];
        Q = vec[offset + 7];
        cytbH = vec[offset + 8];
        Qn = vec[offset + 9];
        Qr = vec[offset + 10];
        QH2 = vec[offset + 11];
        cytc2 = vec[offset + 12];
        P700 = vec[offset + 13];
        ADP = vec[offset + 14];
        Pi = vec[offset + 15];
        ATP = vec[offset + 16];
        Ks = vec[offset + 17];
        Mgs = vec[offset + 18];
        Cls = vec[offset + 19];
        Aip = vec[offset + 20];
        U = vec[offset + 21];
        An = vec[offset + 22];
        Fdn = vec[offset + 23];
        BFHs = vec[offset + 24];
        BFHl = vec[offset + 25];
        PHs = vec[offset + 26];
        PHl = vec[offset + 27];
        NADPH = vec[offset + 28];
    }

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr toArray() {
        arr vec = {ISPHr, cytc1, ISPo, ISPoQH2, QHsemi, cytbL, Qi, Q, cytbH, Qn, Qr, QH2, cytc2, P700, ADP, Pi, ATP, Ks, Mgs, Cls, Aip, U, An, Fdn, BFHs, BFHl, PHs, PHl, NADPH};
        return vec;
    }

    /**
      Get the size of the data vector
      */
    static size_t size() {
        return count;
    }

    void setParent(FIBFCon* par) {parent = par;}
    friend std::ostream& operator<<(std::ostream &out, const BFCon &in);

    double ISPHr = 0.;   ///< The reduced ion sulfer protein (ISPH); unit: micromole per m2
    double cytc1 = 0.;   ///< The oxidized state of cytc1; unit: micromole per meter square
    double ISPo = 0.;    ///< The oxidized ion sulfer protein (ISP); unit: micromole per meter square
    double ISPoQH2 = 0.; ///< The complex of oxidized ion sulfer protein and reduced quinone; unit: micromole per meter square
    double QHsemi = 0.;  ///< Semiquinone; micromole per meter square
    double cytbL = 0.;   ///< The oxidized cytbL; micromole per meter square
    double Qi = 0.;      ///< The binding Quinone; micromole per meter square
    double Q = 0.;       ///< Quinone; micromole per meter square
    double cytbH = 0.;   ///< The oxidized form of cytbH; micromole per meter square
    double Qn = 0.;      ///< Q-; unit: micromole per meter square
    double Qr = 0.;      ///< The reduced quinone Q2-; micromole per meter square
    double QH2 = 0.;     ///< The reduced quinone PQH2; micromole per meter square
    double cytc2 = 0.;   ///< oxidized cytc2; micromole per meter square
    double P700 = 0.;    ///< The reduced state of P700, including both P700 and excited P700; micromole per meter square
    double ADP = 0.;     ///< ADP in stroma, from the earlier photorespiration model; mmol l-1
    double Pi = 0.;      ///< Phosphate in stroma, from the photorespiration model; mmol l-1
    double ATP = 0.;     ///< ATP in stroma, from the photorespiration model; mmol l-1
    double Ks = 0.;      ///< K ions in stroma, mM, from the literature; mmol l-1; 90 might be an default;
    double Mgs = 0.;     ///< Mg ions in stroma, mM, from the literature of the ion estimate
    double Cls = 0.;     ///< Cl ions in stroma, mM, from the literature of the ion estimate
    double Aip = 0.;     ///< The number of photons in peripheral antenna; micromole per meter square
    double U = 0.;       ///< The number of photons in core antenna; micromole per meter square
    double An = 0.;      ///< The reduced electron acceptor in PSI; micromole per meter square
    double Fdn = 0.;     ///< The reduced ferrodoxin; micromole per meter square leaf area
    double BFHs = 0.;    ///< The protonated buffer species  and free proton together in stroma; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number.; default 25
    double BFHl = 0.;    ///< The protonated buffer species and free proton together in lumen; mmol l-1; The value follows Laisk and Walker, 1989. But they did not give reference about the source of this number. ; default 5
    double PHs = 0.;     ///< The PH value of the stroma
    double PHl = 0.;     ///< The PH value of the lumen
    double NADPH = 0.;   ///< The NADPH concentration in stroma, Unit: mmol l-1;
    FIBFCon* parent;
private:
    static const size_t count;

};
