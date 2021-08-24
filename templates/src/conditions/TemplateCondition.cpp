#include "conditions/TemplateCondition.hpp"

using namespace ePhotosynthesis;
using namespace ePhotosynthesis::conditions;

// update this with the number of data elements in the container
const std::size_t TemplateCondition::count = XX;

TemplateCondition::TemplateCondition(const TemplateCondition* const other) {
    // here, any data members are copied from 'other' to this object
}

TemplateCondition::TemplateCondition(const arr &vec, const size_t offset) {
    fromArray(vec, offset);
}

void TemplateCondition::_fromArray(const arr &vec, const size_t offset) {
    // here, any data members have their values set to values in the input array
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the below _toArray method.

}

arr TemplateCondition::_toArray() const {
    // here any data members are copied to the output array.
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the above _fromArray method.
    return {};
}