#include "containers/ContainerTemplate.hpp"

ContainerTemplate::ContainerTemplate(const ContainerTemplate* other) {
    // here, any data members are copied from 'other' to this object
}

ContainerTemplate::ContainerTemplate(const arr &vec, const size_t offset) {
    fromArray(vec, offset);
}

void ContainerTemplate::_fromArray(const arr &vec, const size_t offset) {
    // here, any data members have their values set to values in the input array
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the below _toArray method.

}

arr ContainerTemplate::_toArray() {
    // here any data members are copied to the output array.
    //   IMPORTANT: The order and number of the data members must be the same
    //              as in the above _fromArray method.
    arr vec = {};
    return vec;
}