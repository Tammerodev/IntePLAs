#pragma once
#include <string>

namespace StringUtils {
    inline const bool stringToBool(const std::string& string) {
        return string == "true";
    }
}