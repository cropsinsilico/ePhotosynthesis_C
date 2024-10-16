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
#include "ValueSet.hpp"

#include "vel/VelAll.hpp"
#include "ke/KEAll.hpp"
#include "pool/PoolAll.hpp"
#include "rc/RCAll.hpp"
#include "conditions/ConditionAll.hpp"

#include <sundials/sundials_context.h>

// Parameters for param_types
// 1. namespace of class
// 2. class suffix
// 3. variable suffix
#define VARS_PARAM_PT_COND conditions, Condition, 2OUT
#define VARS_PARAM_PT_POOL pool, Pool, _Pool
#define VARS_PARAM_PT_RC RC, RC, _RC
#define VARS_PARAM_PT_KE KE, KE, _KE
#define VARS_PARAM_PT_VEL_SERIES vel, Vel, _VEL.getLastData()
#define VARS_PARAM_PT_VEL vel, Vel, _Vel
#define VARS_PARAM_PT_MOD modules, ,NONE
#define VARS_PARAM_PT(pt) (VARS_PARAM_PT_ ## pt)
#define VARS_PARAM_PT_NAMESPACE_(ns, cs, vs) ns
#define VARS_PARAM_PT_CLASS_SUFFIX_(ns, cs, vs) cs
#define VARS_PARAM_PT_VAR_SUFFIX_(ns, cs, vs) vs
#define VARS_PARAM_PT_NAMESPACE(pt)		\
  VARS_PARAM_PT_NAMESPACE_ VARS_PARAM_PT(pt)
#define VARS_PARAM_PT_CLASS_SUFFIX(pt)		\
  VARS_PARAM_PT_CLASS_SUFFIX_ VARS_PARAM_PT(pt)
#define VARS_PARAM_PT_VAR_SUFFIX(pt)		\
  VARS_PARAM_PT_VAR_SUFFIX_ VARS_PARAM_PT(pt)
#define VARS_CLASS_VAR(mod, pt)			\
  VARS_PARAM_PT_NAMESPACE(pt)::CONCATENATE(mod, VARS_PARAM_PT_CLASS_SUFFIX(pt))
#define VARS_INST_VAR(mod, pt)			\
  CONCATENATE(mod, VARS_PARAM_PT_VAR_SUFFIX(pt))

#define VARS_INST_MODULES (BF, FI, PR, PS, RROEA, RedoxReg, RuACT, SUCS, XanCycle)
#define VARS_INST_PARAM_TYPES (COND, POOL, RC, KE, VEL)

#define VARS_CLASS_MODULES (PACK_MACRO(MEMBER_NAMES_MODULE))
#define VARS_CLASS_PARAM_TYPES PACK_MACRO((MEMBER_NAMES_PARAM))

// X ## _STATIC should have last arguments as ... mod
#define VARS_INST_SWITCH(mod, pt, X, ...)				\
    if (pt == PARAM_TYPE_MOD) {						\
	SWITCH_MOD(mod, VARS_CLASS_MODULES, X ## _STATIC, __VA_ARGS__)	\
    }								\
    if (mod == MODULE_FIBF && pt == PARAM_TYPE_POOL) {			\
	X(__VA_ARGS__, FIBF, POOL);					\
    }									\
    SWITCH_MOD_AND_PT(mod, VARS_INST_MODULES,				\
		      pt, VARS_INST_PARAM_TYPES,			\
		      X, __VA_ARGS__)

#define VARS_CLASS_SWITCH(mod, pt, X, ...)				\
    SWITCH_MOD_AND_PT(mod, VARS_CLASS_MODULES,				\
		      pt, VARS_CLASS_PARAM_TYPES,			\
		      PACK_MACRO(X), __VA_ARGS__)

namespace ePhotosynthesis {

/**
  Structure to hold global variables
  */
class Variables { // : public ValueSet<MODULE_NONE, PARAM_TYPE_COND> {
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
    void initParamDefaults() {
      std::string filename;
      std::map<PARAM_TYPE, std::map<MODULE, std::string> >::iterator it_pt = files.find(T::param_type);
      if (it_pt != files.end()) {
	std::map<MODULE, std::string>::iterator it_mod = it_pt->second.find(T::module);
	if (it_mod != it_pt->second.end()) {
	  filename = it_mod->second;
	  it_pt->second.erase(it_mod);
	}
      }
      T::initDefaults(useC3, filename);
    }
    
    template<typename T>
    void initParam(T& param) {
      initParamDefaults<T>();
      param.initValues();
    }
    template<typename T>
    void initParamStatic() {
      initParamDefaults<T>();
      T::initValues();
    }

    static void initDefaults(const MODULE& module,
			     const PARAM_TYPE& param_type,
			     const bool useC3=false,
			     const std::string& filename="");
    static std::string parseVar(const std::string& k,
				MODULE& mod, PARAM_TYPE& pt);
    static bool hasVar(const MODULE& mod, const PARAM_TYPE& pt,
		       const std::string& name);
    void setVar(const MODULE& mod, const PARAM_TYPE& pt,
		const std::string& name, const double& value);
    void setVar(const std::string& k, const double& value) {
	std::string name;
	MODULE mod = MODULE_NONE;
	PARAM_TYPE pt = PARAM_TYPE_NONE;
	name = parseVar(k, mod, pt);
	return setVar(mod, pt, name, value);
    }

    double getVar(const MODULE& mod, const PARAM_TYPE& pt,
		  const std::string& name) const;
    double getVar(const std::string& k) const {
	std::string name;
	MODULE mod = MODULE_NONE;
	PARAM_TYPE pt = PARAM_TYPE_NONE;
	name = parseVar(k, mod, pt);
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
