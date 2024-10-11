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
#include <map>
#include "definitions.hpp"
#include "enums/enums_utils.hpp"

#include "vel/VelAll.hpp"
#include "ke/KEAll.hpp"
#include "pool/PoolAll.hpp"
#include "rc/RCAll.hpp"
#include "conditions/ConditionAll.hpp"

#include <sundials/sundials_context.h>

namespace ePhotosynthesis {

/**
  Structure to hold global variables
  */
class Variables {
public:
    Variables(SUNContext* ctx = NULL) {
        context = ctx;
	if (!ctx) {
	    std::cout << "SUNContext is NULL" << std::endl;
	    throw  std::runtime_error("SUNContext is NULL");
	}
    }
    Variables(const Variables* other);
    Variables(const Variables& other);
    Variables* deepcopy() const;
    Variables& operator=(const Variables& other);
    friend std::ostream& operator<<(std::ostream &out, const Variables *in);
    template<typename T>
    void initParam(T& param) {
      param.initValues(useC3);
      std::map<PARAM_TYPE, std::map<MODULE, std::string> >::iterator it_pt = files.find(param.param_type);
      if (it_pt == files.end())
	return;
      std::map<MODULE, std::string>::iterator it_mod = it_pt->second.find(param.module);
      if (it_mod == it_pt->second.end())
	return;
      param.update_values(it_mod->second);
    }
    template<typename T>
    void initParamStatic() {
      T::initValues(useC3);
      std::map<PARAM_TYPE, std::map<MODULE, std::string> >::iterator it_pt = files.find(T::param_type);
      if (it_pt == files.end())
	return;
      std::map<MODULE, std::string>::iterator it_mod = it_pt->second.find(T::module);
      if (it_mod == it_pt->second.end())
	return;
      T::update_values(it_mod->second);
    }

    double getVar(const MODULE& mod, const PARAM_TYPE& pt,
		  const std::string& name) const;
    double getVar(const std::string& k) const {
	std::string split="::", modS, ptS, name;
	size_t idx1 = k.find(split);
	MODULE mod = MODULE_NONE;
	PARAM_TYPE pt = PARAM_TYPE_NONE;
	if (idx1 == std::string::npos) {
	    // TODO: Var
	    throw std::runtime_error("Could not find 1st '::' marking module");
	} else {
	    modS = k.substr(0, idx1);
	    size_t idx2 = k.find(split, idx1 + split.size());
	    if (idx2 == std::string::npos)
	      throw std::runtime_error("Could not find 2nd '::' marking parameter type");
	    ptS = k.substr(idx1 + split.size(),
			   idx2 - (idx1 + split.size()));
	    name = k.substr(idx2 + split.size());
	    mod = utils::enum_string2key<MODULE>(modS);
	    pt = utils::enum_string2key<PARAM_TYPE>(ptS);
	}
	std::cerr << "getVar[" << mod << ", " << pt << ", " << name <<
	  std::endl;
	return getVar(mod, pt, name);
    }

    SUNContext* context = NULL;
    bool record = false;
    bool BF_FI_com = false;
    bool EPS_SUCS_com = false;
    bool FIBF_PSPR_com = false;
    bool PR_PS_com = false;
    bool PSPR_SUCS_com = false;
    bool RROEA_EPS_com = false;
    bool RedoxReg_RA_com = false;
    bool RuACT_EPS_com = false;
    bool XanCycle_BF_com = false;

    int GP = 0;
    int GRNC = 0;
    int GRNT = 0;

    int RUBISCOMETHOD = 1;

    const double AVR = 30.;
    const double HPR = 4.66;
    const double O2 = 210.;

    double CO2_cond = 0.;

    double GLight = 0.;
    double O2_cond = 0.;
    double PS12ratio = 0.;
    double ADP = 0.;

    double Pi = 0.;

    double TestATPCost = 0.;
    double CO2_in = 0.;
    double TestLi = 0.;
    double PS2BF_Pi = 0.;
    double PS_PR_Param = 0;

    double Tp = 0;
    double alfa = 0.;
    double fc = 0.;
    double lightParam = 0.;
    const double alpha1 = 1.205;
    const double alpha2 = 2.06;

    // Files containing default conditions & constants
    std::map<PARAM_TYPE, std::map<MODULE, std::string> > files;

    std::map<std::string, double> EnzymeAct;

    arr VfactorCp = zeros(33);
    arr VfactorT = ones(28);

    // misc
    arr FluxTR = {};

    TimeSeries<std::vector<double> > CO2A = TimeSeries<std::vector<double> > ();
    std::vector<arr> RedoxReg_MP;

// #define MODULES_Pool BF, FIBF, FI, RROEA, RuACT, SUCS
// #define MODULES_RC BF, FI, RROEA, RuACT
// #define MODULES2OUT PR, PS, SUCS, XanCycle
// #define MODULES_KE RROEA

#define ADD_MOD_MEMBER(mod, type, suffix)	\
    type mod ## suffix
#define ADD_MOD_VALUE_SET(mod, ns, type_suffix, name_suffix)	\
    ADD_MOD_MEMBER(mod, ns::mod ## type_suffix, name_suffix)
#define ADD_MOD(mod, size)					\
    arr mod ## _Param = zeros(2);				\
    ADD_MOD_VALUE_SET(mod, vel, Vel, _Vel);			\
    arr mod ## Ratio = ones(size);				\
    ADD_MOD_VALUE_SET(mod, pool, Pool, _Pool);			\
    ADD_MOD_VALUE_SET(mod, RC, RC, _RC);			\
    ADD_MOD_VALUE_SET(mod, conditions, Condition, 2OUT);	\
    ADD_MOD_VALUE_SET(mod, KE, KE, _KE);			\
    TimeSeries<vel::mod ## Vel> mod ## _VEL = TimeSeries<vel::mod ## Vel> ()

    ADD_MOD(BF, 49);
    ADD_MOD(FI, 23);
    ADD_MOD(PR, 48);
    ADD_MOD(PS, 103);
    ADD_MOD(RROEA, 0);
    ADD_MOD(RedoxReg, 0);
    ADD_MOD(RuACT, 16);
    ADD_MOD(SUCS, 66);
    ADD_MOD(XanCycle, 4);
    // ADD_MOD(FIBF, 0);
    pool::FIBFPool FIBF_Pool;

#undef ADD_MOD
#undef ADD_MOD_VALUE_SET
#undef ADD_MOD_MEMBER

    bool useC3 = false;
#ifdef INCDEBUG
    Debug::RequestedDebug debuglevel = Debug::None;
#endif
};

}  // namespace ePhotosynthesis
