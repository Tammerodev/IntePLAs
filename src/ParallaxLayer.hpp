#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
public:
    void create(float amountOfParallax = 0.f) {
        parallaxShader.loadFromMemory(
        "uniform float offset;"
        "uniform float offsety;"

        "void main() {"
        "    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
        "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "    gl_TexCoord[0].x = gl_TexCoord[0].x + offset;" // magic
        "    gl_TexCoord[0].y = gl_TexCoord[0].y + offsety;" // magic
        "    gl_FrontColor = gl_Color;"
        "}"
        , sf::Shader::Vertex);

        parallax = amountOfParallax;
    }

    void load(const std::string &path) {
        tx.loadFromFile(path);
        tx.setRepeated(true);

        spr.setPosition(0, -1000);
        spr.setColor(sf::Color(255, 255, 255, 200));

        spr.setTexture(tx);
    }

    void update(sf::View &view) {
        const sf::FloatRect viewRect(view.getCenter() - view.getSize() / 2.f, view.getSize());

        // Ok. What the HELL is this value? Really?

        float center_x = view.getCenter().x / 1000.0f;
        float center_y = view.getCenter().y / 1000.0f;

        parallaxShader.setUniform("offset", (parallax * 0.1f) * center_x);
        parallaxShader.setUniform("offsety", (parallax * 0.1f) * center_y);


        sf::IntRect texture_rect(viewRect);

        if(texture_rect.top < 0) {
            texture_rect.top = 0;
        }

        spr.setTextureRect(texture_rect);

        spr.setPosition(texture_rect.left, texture_rect.top);
    }

    void render(sf::RenderTarget& target) {
        spr.setTexture(tx);
        target.draw(spr, &parallaxShader);
    }

private:
    float offset = 0.f;
    float parallax = 0.f;

    sf::Sprite spr;
    sf::Texture tx;

    sf::Shader parallaxShader;
};
