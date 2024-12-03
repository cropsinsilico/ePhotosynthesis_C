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

#include "ModuleBase.hpp"
#include "conditions/SUCSCondition.hpp"
#include "definitions.hpp"

namespace ePhotosynthesis {

FORWARD_DECLARE_MODULE(SUCS);
  
namespace modules {

/**
 Class for SUCS related functions and common variables
 */
class SUCS : public MODULE_BASE(SUCS) {
public:
    DECLARE_MODULE(SUCS)
private:

    /**
      Reset the static member variables to their default values.
      */
    static void _reset(const bool noChildren = false);
    SET_GET(KE501)
    SET_GET(KE51)
    SET_GET(KE52)
    SET_GET(KE531)
    SET_GET(KE541)
    SET_GET(KE55)
    SET_GET(KE56)
    SET_GET(KE57)
    SET_GET(KE59)
    SET_GET(KE61)
    SET_GET(KI521)
    SET_GET(KI522)
    SET_GET(KI523)
    SET_GET(KI561)
    SET_GET(KI562)
    SET_GET(KI563)
    SET_GET(KI564)
    SET_GET(KI565)
    SET_GET(KI581)
    SET_GET(KI582)
    SET_GET(KI591)
    SET_GET(Ki572)
    SET_GET(Km511)
    SET_GET(Km512)
    SET_GET(Km513)
    SET_GET(Km521)
    SET_GET(Km551)
    SET_GET(Km552)
    SET_GET(Km553)
    SET_GET(Km554)
    SET_GET(Km561)
    SET_GET(Km562)
    SET_GET(Km571)
    SET_GET(Km581)
    SET_GET(Km591)
    SET_GET(Km593)
    SET_GET(Km621)
    SET_GET(V51)   ///< DHAP+GAP --FBP
    SET_GET(V52)   ///< FBP --F6P + Pi
    SET_GET(V55)   ///< G1P+UTP --OPOP+UDPG
    SET_GET(V56)   ///< UDPG+F6P--SUCP + UDP
    SET_GET(V57)   ///< SUCP--Pi + SUC
    SET_GET(V58)   ///< F26BP--F6P + Pi
    SET_GET(V59)   ///< F6P + ATP --ADP + F26BP
    SET_GET(V60)
    SET_GET(V61)
    SET_GET(V62)   ///< SUC Sink
    SET_GET(Vdhap_in) ///< DHAP export from chloroplast
    SET_GET(Vgap_in)  ///< GAP export from chloroplast
    SET_GET(Vpga_in)  ///< PGA export from chloropalst
    SET_GET(Km592)
    SET_GET(KI592)
    SET_GET(Km601)
    SET_GET(Km602)
    SET_GET(Km603)
    SET_GET(Km604)
    SET_GET(KE60)
    SET_GET(Vfactor51)
    SET_GET(Vfactor52)
    SET_GET(Vfactor56)
    SET_GET(Vfactor57)
    SET_GET(Vfactor59)
    SET_GET(Vf_T51)
    SET_GET(Vf_T52)
    SET_GET(Vf_T56)
    SET_GET(Vf_T57)
    SET_GET(Vf_T59)
    SET_GET(UTPc)
    SET_GET(ATPc)

    SET_GET(SUCS2PS_Pic)

};

  DEFINE_MODULE_HEADER(SUCS);

}  // namespace modules
}  // namespace ePhotosynthesis
