#include "conditions/DependentCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

// update this with the number of data elements in the container
std::size_t DependentCondition::count = 0;

DependentCondition::DependentCondition(const DependentCondition* const other) {
    A_con = new ACondition(other->A_con);
    B_con = new BCondition(other->B_con);
    A_con->setParent(this);
    B_con->setParent(this);
}

DependentCondition::DependentCondition(ACondition* aother, BCondition* bother) {
    if (aother->parent == nullptr) {
        A_con = aother;
    } else {
        A_con = new ACondition(aother);
    }
    if (bother->parent == nullptr) {
        B_con = bother;
    } else {
        B_con = new BCondition(bother);
    }
    A_con->setParent(this);
    B_con->setParent(this);
}

DependentCondition::DependentCondition(const arr &vec, const size_t offset) {
    fromArray(vec, offset);
}

void DependentCondition::_fromArray(const arr &vec, const size_t offset) {
    // here, any data members have their values set to values in the input array
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the below _toArray method.
    if (A_con == nullptr)
        A_con = new ACondition(this);
    if (B_con == nullptr)
        B_con = new BCondition(this);
    A_con->fromArray(vec, offset);
    B_con->fromArray(vec, offset + ACondition::size());
}

arr DependentCondition::_toArray() const {
    // here any data members are copied to the output array.
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the above _fromArray method.
    arr avec = A_con->toArray();
    arr bvec = B_con->toArray();
    avec.insert(avec.end(), bvec.begin(), bvec.end());
    return avec;
}

void DependentCondition::_clear() {
    if (A_con != nullptr) {
        delete A_con;
        A_con = nullptr;
    }
    if (B_con != nullptr) {
        delete B_con;
        B_con = nullptr;
    }
}