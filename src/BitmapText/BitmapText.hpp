#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>

/* 
    Small "library" for bitmap text rendering
*/

class BitmapTextFont {
public:
    bool loadFromFile(const std::string& path, const sf::Vector2u characterSize, const unsigned int characterSpacing) {
        if (!fontTexture.loadFromFile(path)) {
            std::cerr << "Error loading font: texture not loaded correctly. Path was: " << path << "." << std::endl;
            return false;
        }

        loaded = true;
        this->characterSize = characterSize;
        this->characterSpacing = characterSpacing;

        return true;
    }

public:
    bool loaded = false;

    sf::Texture fontTexture;
    sf::Vector2u characterSize;
    unsigned int characterSpacing;
};

class BitmapText {
public:
    void setFont(BitmapTextFont* font) {
        p_font = font;
        checkFontPointer();
    }

    void setPosition(const sf::Vector2f& pos) {
        position = pos;
    }

    void setScale(const float s) {
        scale = s;
    }

    void setNumber(const int number = 0) {
        checkFontPointer();

        if(number == 0) return;
        std::string text = std::to_string(number);

        sprites.clear();
        sprites.reserve(text.size());

        int index = 0;

        sf::Sprite sprite;
        sprite.setTexture(p_font->fontTexture);
        sprite.setScale(scale, scale);

        for (const auto c : text) {
            int charIndex = c - 48;
            if (charIndex < 0) {
                //std::cerr << "Character '" << c << "' not in font texture." << std::endl;
                continue; // Skip unsupported characters
            }

            sprite.setTextureRect(sf::IntRect(
                charIndex * p_font->characterSize.x, p_font->characterSize.y,
                p_font->characterSize.x, p_font->characterSize.y
            ));

            sprite.setPosition(scale * (index * p_font->characterSpacing), 0);
            sprite.move(position);

            sprites.push_back(sprite);

            ++index;
        }
    }

    void setText(const std::string& text) {
        checkFontPointer();

        sprites.clear();
        sprites.reserve(text.size());

        int index = 0;

        sf::Sprite sprite;
        sprite.setTexture(p_font->fontTexture);
        sprite.setScale(scale, scale);

        for (const auto c : text) {
            int charIndex = getAlphabeticalIndex(c);
            if (charIndex < 0) {
                //std::cerr << "Character '" << c << "' not in font texture." << std::endl;
                continue; // Skip unsupported characters
            }

            sprite.setTextureRect(sf::IntRect(
                charIndex * p_font->characterSize.x, 0,
                p_font->characterSize.x, p_font->characterSize.y
            ));

            sprite.setPosition(scale * (index * p_font->characterSpacing), 0);
            sprite.move(position);

            sprites.push_back(sprite);

            ++index;
        }
    }

    void render(tgui::CanvasSFML::Ptr canvas) {
        for (const auto& sprite : sprites) {
            canvas->draw(sprite);
        }
    }

    void render(sf::RenderTarget& target) {
        for (const auto& sprite : sprites) {
            target.draw(sprite);
        }
    }

private:
    std::vector<sf::Sprite> sprites;
    BitmapTextFont* p_font = nullptr;

    sf::Vector2f position{ 0, 0 };
    float scale = 1.f;

    void checkFontPointer() {
        if (p_font == nullptr) {
            std::cerr << "setFont() failed as input pointer (font) was null" << std::endl;
            return;
        } else if (!p_font->loaded) {
            std::cerr << "setFont() failed font is not loaded" << std::endl;
            return;
        }
    }

    const int getAlphabeticalIndex(const char ascii) {
        if (std::isalpha(ascii)) {
            return std::tolower(ascii) - 'a';
        }
        return -1; // Return -1 for unsupported characters
    }
};
