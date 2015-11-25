#include "Utilities.hpp"

std::ostream& operator<<(std::ostream& out, const glm::vec2 v){
    out << to_string(v);
    return out;
}


std::ostream& operator<<(std::ostream& out, const glm::vec3 v){
    out << to_string(v);
    return out;
}

std::ostream& operator<<(std::ostream& out, const glm::vec4 v){
    out << to_string(v);
    return out;
}
