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

#include "RAContainer.hpp"
#include "XanCycleContainer.hpp"
#include <sundials/sundials_types.h>

class trDynaPSContainer;
/**
 Class for holding inputs to DynaPS_mb
 */
class DynaPSContainer : public ContainerBase<DynaPSContainer, trDynaPSContainer> {
public:
    DynaPSContainer(trDynaPSContainer* par = nullptr) : RA_con(new RAContainer(this)), XanCycle_con(new XanCycleContainer(this)) {
        setParent(par);
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The DynaPSCon object to copy
      */
    DynaPSContainer(const DynaPSContainer* other);

    /**
      Constructor to create an object from the input pointer

      @param x The pointer to get the data from
      */
    DynaPSContainer(realtype *x);

    /**
      Constructor to create an object from the contained classes

      @param rother A RACon object to incorporate
      @param xother A XanCycleCon object to incorporate
      */
    DynaPSContainer(RAContainer* rother, XanCycleContainer* xother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    DynaPSContainer(const arr &vec, size_t offset = 0);

    static void setTop() {DynaPSContainer::_dlevel = Debug::Top;}

    RAContainer* RA_con = nullptr;
    XanCycleContainer* XanCycle_con = nullptr;
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
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return 120;
//return RACon::size() + XanCycleCon::size();
    }
    void _clear();
#ifdef INCDEBUG
    static Debug::DebugLevel _dlevel;
#endif

};
