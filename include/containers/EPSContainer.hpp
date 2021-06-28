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

#include "FIBFContainer.hpp"
#include "CMContainer.hpp"

class RAContainer;
/**
 Class for holding the inputs to EPS_mb
 */
class EPSContainer : public ContainerBase<EPSContainer, RAContainer> {
public:
    EPSContainer(RAContainer* par = nullptr) : CM_con(new CMContainer(this)), FIBF_con(new FIBFContainer(this)) {
        setParent(par);
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The EPSCon object to copy
      */
    EPSContainer(const EPSContainer* other);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    EPSContainer(realtype *x, const uint adjust = 0);

    /**
      Constructor to create an object from the contained classes

      @param fother A FIBFCon object to incorporate
      @param cother A CMCon object to incorporate
      */
    EPSContainer(FIBFContainer* fother, CMContainer* cother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    EPSContainer(const arr &vec, const size_t offset = 0);

    using ContainerBase::fromArray;
    /**
      Copy items from the given pointer to the data members

      @param x The input pointer to copy from
      */
    void fromArray(realtype *x, const uint adjust = 0);
#ifdef INCDEBUG
    static void setTop() {EPSContainer::_dlevel = Debug::Top;}
#endif

    CMContainer* CM_con = nullptr;
    FIBFContainer* FIBF_con = nullptr;
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;

private:
    friend ContainerBase;

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
        return CMContainer::size() + FIBFContainer::size();
    }

    void _clear();
#ifdef INCDEBUG
    static Debug::DebugLevel _dlevel;
#endif
};
