#pragma once
#include <SFML/Graphics/Shader.hpp>

class Shader : sf::Shader{
public:
    Shader() = default;

    Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        checkShaderAvailability();
        if(!shaders_available) return;

        if(!loadFromFile(vertexPath, fragmentPath)) {
            throw "Error loading shaders";
        } else {
            loadedCorrectly = true;
        }
    }

    bool load(const std::string &vertexPath, const std::string &fragmentPath) {
        checkShaderAvailability();
        if(!shaders_available) return false;

        return loadFromFile(vertexPath, fragmentPath);
    }

    void renderTo(sf::Drawable& draw, sf::RenderTarget &targ) {
        if(shaders_available) {
            targ.draw(draw, this);
        } else {
            targ.draw(draw);
        }
    }

    void o_setUniform(const std::string &name, int x) {
        if(!shaders_available) return;
        this->setUniform(name, x);
    }

    void o_setUniform(const std::string &name, float x) {
        if(!shaders_available) return;
        this->setUniform(name, x);
    }

    void o_setUniform(const std::string &name, const sf::Glsl::Vec2 &vector) {
        if(!shaders_available) return;
        this->setUniform(name, vector);
    }

    void o_setUniform(const std::string &name, const sf::Glsl::Vec3 &vector) {
        if(!shaders_available) return;
        this->setUniform(name, vector);
    }

private:

    void checkShaderAvailability() {
        shaders_available = sf::Shader::isAvailable();

        if(!shaders_available) {
            prnerr("***********************************************", "");
            prnerr("ERROR : ", " Shaders are not supported on this system");
            prnerr("sf::Shader::isAvailable() failed (return value is 0)", "!");
            prnerr("Try updating your graphics driver.", "!");

            prnerr("***********************************************", "");

        }
    }

    bool loadedCorrectly = false;
    bool shaders_available = true;
};