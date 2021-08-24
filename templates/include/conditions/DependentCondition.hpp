#pragma once

#include "conditions/ACondition.hpp"
#include "conditions/BCondition.hpp"

/** ************************************************************************************************
 * This file provides the template for adding new Condition methods.
 * Replace Dependent with the name of the new condition and save this file as <new_condition_name>.hpp
 * in the include/conditions directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 * Parent_condition is the Condition in the next level up which holds this object. If this
 * is the top most object then replace Parent_Condition with the name of this object.
 ************************************************************************************************* */
namespace ePhotosynthesis {
namespace modules {
class DependentModule;
}
namespace conditions {

class Parent_Condition;

class DependentCondition : public ConditionBase<DependentCondition, Parent_Condition> {
public:
    DependentCondition(Parent_Condition* par = nullptr) : A_con(new ACondition(this)), B_con(new BCondition(this)) {
        setParent(par);
    }

    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCon object to copy
      */
    DependentCondition(const DependentCondition* const other);

    DependentCondition(ACondition* aother, BCondition* bother);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    DependentCondition(const arr &vec, const std::size_t offset = 0);

    ~DependentCondition () override {
        _clear();
    }
    // Data members go here

    ACondition* A_con;
    BCondition* B_con;
    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
private:
    friend ConditionBase;
    friend class modules::DependentModule;
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, const size_t offset = 0) override;
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray() const override;

    /**
      Get the size of the data vector
      */
    static std::size_t _size() {
        if (count == 0)
            count = ACondition::size() + BCondition::size();
        return count;
    }

    void _clear() override;
    static void reset() {
        count = 0;
    }
    // the number of elements in this Condition when it is turned into a vector.
    static std::size_t count;
#ifdef INCDEBUG
    const static Debug::DebugLevel _dlevel = Debug::Low;
#endif
};

}  // namespace conditions
}  // namespace ePhotosynthesis
