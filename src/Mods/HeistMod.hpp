#pragma once 
#include "CustomGameMod.hpp"

class HeistMod : public CustomGameMod {
    public:
        virtual const std::string& getModInfo() = 0;
        virtual const std::string& getModName() = 0;

        virtual void load(Game& game) = 0;
        virtual void update(Game& game) = 0;
        virtual void render(sf::RenderTarget& target, Game& game) = 0;
        virtual void input(sf::Event &ev) = 0;
};