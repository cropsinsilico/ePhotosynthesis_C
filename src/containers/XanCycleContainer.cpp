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

#include "containers/XanCycleContainer.hpp"

XanCycleContainer::XanCycleContainer(const XanCycleContainer* other) {
    Vx = other->Vx;
    Ax = other->Ax;
    Zx = other->Zx;
    ABA = other->ABA;
}

XanCycleContainer::XanCycleContainer(const arr &vec, const size_t offset) {
    fromArray(vec, offset);
}

void XanCycleContainer::_fromArray(const arr &vec, const size_t offset) {
    Vx = vec[offset];
    Ax = vec[offset + 1];
    Zx = vec[offset + 2];
    ABA = vec[offset + 3];
}

arr XanCycleContainer::_toArray() {
    arr array = {Vx, Ax, Zx, ABA};
    return array;
}

void XanCycleContainer::_clear() {
    Vx = 0.;
    Ax = 0.;
    Zx = 0.;
    ABA = 0.;
}