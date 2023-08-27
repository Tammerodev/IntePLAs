#pragma once
#include <SFML/Graphics/Shader.hpp>

class Shader : public sf::Shader{
public:
    Shader() = default;

    Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        if(!loadFromFile(vertexPath, fragmentPath)) {
            throw "Error loading shaders";
        } else {
            loadedCorrectly = true;
        }
    }

    bool load(const std::string &vertexPath, const std::string &fragmentPath) {
        return loadFromFile(vertexPath, fragmentPath);
    }
private:

    bool loadedCorrectly = false;

};