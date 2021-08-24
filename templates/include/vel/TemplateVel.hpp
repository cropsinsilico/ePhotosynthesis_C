#pragma once

#include "definitions.hpp"

namespace ePhotosynthesis {
namespace vel {
/** ************************************************************************************************
 * This file provides the template for adding new Vel class.
 * Replace Template with the name of the new module and save this file as <new_vel_name>.hpp
 * in the include/vel directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 ************************************************************************************************* */

/**
 Class for holding the result of the Template_Rate calculations
 */
class TemplateVel {
public:
    TemplateVel() {}
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFVel object to copy
      */
    TemplateVel(const TemplateVel &other) {
        // copy all values from other to this container
    }

    // put all data members here
    // all values must be of double type
    friend std::ostream& operator<<(std::ostream& out, const TemplateVel &in);
};

}  // namespace vel
}  // namespace ePhotosynthesis

