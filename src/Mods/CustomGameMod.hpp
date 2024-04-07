#pragma once 
#include "../Game.hpp"

class CustomGameMod {
    public:
        virtual const std::string getModInfo() = 0;
        virtual const std::string getModName() = 0;

        virtual void load(Game& game, tgui::BackendGui& gui) = 0;
        virtual void update(Game& game) = 0;
        virtual void render(sf::RenderTarget& target, Game& game, tgui::BackendGui& gui) = 0;
        virtual void input(sf::Event &ev) = 0;
};