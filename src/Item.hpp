#pragma once
#include <SFML/Graphics.hpp>
#include "VoxelManager.hpp"
#include "World.hpp"
#include "Palettes/PaletteUI.hpp"
#include "FontManager.hpp"

class ItemUseCooldown {
    public:
        void setCooldown(const float cooldown) {
            cooldownTime = cooldown;
        }

        void render(sf::RenderTarget& target, const sf::Vector2f& pos, const sf::Vector2i& size) {
            if(cooldownTime - clock.getElapsedTime().asSeconds() < 0.25) return;

            sf::Text cooldownBar;
            cooldownBar.setFont(Fonts::font_pixelated);
            
            cooldownBar.setPosition(pos);
            cooldownBar.setString(std::to_string(cooldownTime - clock.getElapsedTime().asSeconds()));


            target.draw(cooldownBar);
        }

        void update() {
            
        }

        bool tryUse() {
            if(clock.getElapsedTime().asSeconds() >= cooldownTime) {
                clock.restart();
                return true;
            }

            return false;
        }
    private:
        float cooldownTime  = 0.f;
        sf::Clock clock;
};

class Item {
public:
    enum Category {
        Undefined, Tool, Usercreated, Pairing
    };

    virtual void use(Player& player, const sf::Vector2f&, World &world) = 0;
    virtual void render(sf::RenderTarget&) = 0;
    virtual void inventory_render(sf::RenderTarget&, const sf::Vector2f& pos) = 0;

    virtual void update(World&, const sf::Vector2f&, const sf::Vector2f&, const float, Player&) = 0;
    virtual void setPosition(const sf::Vector2f&) = 0;
    virtual sf::Sprite &getSprite() = 0;

    virtual bool remove_item() {
        return false;
    }

    virtual Category getCategory() = 0;

};