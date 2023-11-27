#pragma once
#include <string>

namespace StringUtils {
    inline const bool stringToBool(const std::string& string) {
        if(string == "true")
            return true;
        else 
            return false;
    }
}