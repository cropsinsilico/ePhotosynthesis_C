#include "vel/TemplateVel.hpp"

namespace ePhotosynthesis {
namespace vel {
std::ostream& operator<<(std::ostream &out, const TemplateVel &in) {
    out << "TemplateVel" << std::endl;
    out << "  Var = " << in.var << std::endl;

    return out;
}

}  // namespace vel
}  // namespace ePhotosynthesis
