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

#include "conditions/AECondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;
const std::size_t AECondition::count = 11;

AECondition::AECondition(const AECondition* const other) {
    Mchl_ActATPsynthase = other->Mchl_ActATPsynthase;
    Mchl_ActGAPDH = other->Mchl_ActGAPDH;
    Mchl_ActNADPMDH = other->Mchl_ActNADPMDH;
    Bchl_ActATPsynthase = other->Bchl_ActATPsynthase;
    Bchl_ActPEPC = other->Bchl_ActPEPC;
    Bchl_ActGAPDH = other->Bchl_ActGAPDH;
    Bchl_ActFBPase = other->Bchl_ActFBPase;
    Bchl_ActSBPase = other->Bchl_ActSBPase;
    Bchl_ActPRK = other->Bchl_ActPRK;
    Bchl_ActRubisco = other->Bchl_ActRubisco;
    Bchl_ActRCA = other->Bchl_ActRCA;
}

AECondition::AECondition(const arr &vec, const std::size_t offset) {
    fromArray(vec, offset);
}

void AECondition::_fromArray(const arr &vec, const std::size_t offset) {
    std::size_t counter = 0;
    Mchl_ActATPsynthase = vec[offset + counter++];
    Mchl_ActGAPDH = vec[offset + counter++];
    Mchl_ActNADPMDH = vec[offset + counter++];
    Bchl_ActATPsynthase = vec[offset + counter++];
    Bchl_ActPEPC = vec[offset + counter++];
    Bchl_ActGAPDH = vec[offset + counter++];
    Bchl_ActFBPase = vec[offset + counter++];
    Bchl_ActSBPase = vec[offset + counter++];
    Bchl_ActPRK = vec[offset + counter++];
    Bchl_ActRubisco = vec[offset + counter++];
    Bchl_ActRCA = vec[offset + counter++];
}

arr AECondition::_toArray() const {
    return {Mchl_ActATPsynthase, Mchl_ActGAPDH, Mchl_ActNADPMDH,
                Bchl_ActATPsynthase, Bchl_ActPEPC, Bchl_ActGAPDH,
                Bchl_ActFBPase, Bchl_ActSBPase, Bchl_ActPRK,
                Bchl_ActRubisco, Bchl_ActRCA};
}
