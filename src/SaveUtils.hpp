#pragma once
#include <string>


namespace utils {

    inline std::string path = "";

    inline const std::string getPath(int x, int y) {
        const std::string x_string  = std::to_string(x);
        const std::string y_string  = std::to_string(y);

        const std::string filename = x_string + "_" + y_string;
        const std::string extension = ".png";

        return filename + extension;
    }
}