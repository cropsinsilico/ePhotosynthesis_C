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

#include "ContainerBase.hpp"

class trDynaPSContainer;

/**
 Class for holding the inputs to RROEA_mb
 */
class RROEAContainer : public ContainerBase<RROEAContainer, trDynaPSContainer> {
public:
    RROEAContainer(trDynaPSContainer* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The RROEACon object to copy
      */
    RROEAContainer(const RROEAContainer* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    RROEAContainer(const arr &vec, size_t offset = 0);

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
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;

private:
    friend ContainerBase;
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, size_t offset = 0);
    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return count;
    }

    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    void _clear() {}

    static const size_t count;
};
