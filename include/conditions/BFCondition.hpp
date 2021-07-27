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

#include "ConditionBase.hpp"

class FIBFCondition;
class BF;

/**
 Class for holding the inputs to BF_mb
 */
class BFCondition : public ConditionBase<BFCondition, FIBFCondition> {
public:
    BFCondition(FIBFCondition* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCon object to copy
      */
    BFCondition(const BFCondition* other);
    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    BFCondition(const arr &vec, const size_t offset = 0);

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
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
    static void setPS_connect(const bool val) {PS_connect = val;}
private:
    friend ConditionBase;
    friend BF;
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
        size_t c = count;
        if (FI_connect)
            c--;
        if (PS_connect)
            c--;
        return c;
    }

    void _clear() {}
    static const size_t count;
    static bool FI_connect;
    static bool PS_connect;
#ifdef INCDEBUG
    const Debug::DebugLevel _dlevel = Debug::Low;
#endif
};
