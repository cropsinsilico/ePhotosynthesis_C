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
#include <stdexcept>
#include <sundials/sundials_types.h>

template<class T>
class ConBase {
public:
    ~ConBase() {}
    //virtual void fromArray(realtype *x) = 0;
    void fromArray(const arr &vec, size_t offset = 0) {static_cast<T*>(this)->_fromArray(vec, offset);}
    arr toArray() {return static_cast<T*>(this)->_toArray();}
    static size_t size() {return T::_size();}
    void fromArray(realtype *x) {
        arr vec(size());
        for (size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }
    void clear() {static_cast<T*>(this)->_clear();}

protected:
    ConBase() {}

};
