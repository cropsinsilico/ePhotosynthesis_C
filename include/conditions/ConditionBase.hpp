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
#include "enums/enums_utils.hpp"
#include "ValueSet.hpp"
#include <stdexcept>
#include <sundials/sundials_types.h>

#ifndef realtype
#define realtype sunrealtype
#endif

#define MODULES_COND BF, CM, DynaPS, EPS, FIBF, FI, PR, PS, PS_PR, RA, RROEA, RedoxReg, RuACT, SUCS, XanCycle, trDynaPS

namespace ePhotosynthesis {
namespace conditions {

/**
  This template class provides a common set of funtions for every Condition subclass. This makes
  calling the underlying functions more straight forward.

  \tparam T The class type to work with
  \tparam U The parent Condition class
  */
template<class T, class U, MODULE ID = MODULE_NONE>
class ConditionBase : public ValueSet<ID, PARAM_TYPE_COND> {
public:
    DECLARE_VALUE_SET_BASE(ConditionBase, ValueSet<ID, PARAM_TYPE_COND>)
    virtual ~ConditionBase() {}

    /**
      Common, public interface for the private _fromArray function.

      \param vec Vector containing the data members of this class.
      \param offset Used to indicated the index of the first element to use.
      */
    void fromArray(const arr &vec, const std::size_t offset = 0) {
        static_cast<T*>(this)->_fromArray(vec, offset);
#ifdef CHECK_VALUE_SET_ALTS
	ValueSetClass::fromArray(vec, offset);
#endif // CHECK_VALUE_SET_ALTS
    }

    /**
      Common, public interface for the private _toArray function.

      \returns Vector of doubles containing the serialized contents of this class.
      */
    arr toArray() {
        return static_cast<T*>(this)->_toArray();
    }

    /**
      Common, public interface for the private _size function.

      \returns The number of active data members in this class.
      */
    static std::size_t size() {
        if (T::module != MODULE_NONE) {
	  size_t alt = T::memberCount();
	  if (alt != T::_size()) {
	    std::cerr << T::memberState() << std::endl;
	    throw std::runtime_error(error_prefix()
				     + "Size of default_values ("
				     + std::to_string(alt)
				     + ") does not match expected count ("
				     + std::to_string(T::_size()) + ")");
	  }
	}
        return T::_size();
    }

    /**
      Converts a realtype* to a vector and then copies the data into this class.

      \param x Array of realtype's
      */
    void fromArray(realtype *x) {
        arr vec(size());
        for (std::size_t i = 0; i < size(); i++)
            vec[i] = x[i];
        fromArray(vec);
    }

    /**
      Common, puiblic interface for the private _clear function.
      */
    void clear() {
        static_cast<T*>(this)->_clear();
    }

    /**
       Common, public interface for the private _reset function.
    */
    static void reset() {
        T::_reset();
        ValueSetClass::reset();
    }

    /**
      Set the parent class pointer to the given value.

      \param par Pointer to the parent class instance.
      */
    void setParent(U* par) {parent = par;}

    /**
      Common, public interface for the private _print function.

      \param out output stream to write to.
      \param tab The level of indentation to use.
      */
    std::ostream& print(std::ostream &out, const uint tab = 0) {return static_cast<T*>(this)->_print(out, tab);}

    /**
      Overloaded output stream function.

      \param out The output stream to write to.
      \param in The instance of the class to write out.
      \returns The output stream.
      */
    friend std::ostream& operator<<(std::ostream &out, const T &in) {
        return static_cast<const T*>(&in)->_print(out, 0);
    }

    /**
      Overloaded output stream function.

      \param out The output stream to write to.
      \param in The instance of the class to write out.
      \returns The output stream.
      */
    friend std::ostream& operator<<(std::ostream &out, const T* const in) {
        return in->_print(out, 0);
    }

    U* parent = nullptr;  //!< The parent Condition instance.
#ifdef INCDEBUG
    Debug::DebugLevel debugLevel() const {return static_cast<const T*>(this)->_dlevel;}
#endif
protected:
    ConditionBase() : ValueSet<ID, PARAM_TYPE_COND>() {}
    ConditionBase(const ConditionBase* const other) :
      ValueSet<ID, PARAM_TYPE_COND>(*other) {}

    /**
      Serialize the data members of this object to a vector of double's. This is the opposite of
      _fromArray().

      \returns std::vector<double> containing the data members.
      */
    virtual arr _toArray() const = 0;

    /**
      Reset any child instances.
      */
    virtual void _clear() = 0;

    /**
      De-serialize the given vector into this object. This is the opposite of _toArray()

      \param vec A vector containing the serialized version of this object.
      \param offset The index of the first data member for the de-serialization
      */
    virtual void _fromArray(const arr &vec, const std::size_t offset = 0) = 0;
};

}  // namespace conditions
}  // namespace ePhotosynthesis
