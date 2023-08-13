#pragma once
#include "UIState.hpp"
#include "ItemCreatorUIState.hpp"
#include "NoUIState.hpp"

#include "Entity/Player/Controls.hpp"

class UIStateManager {
public:

    bool load();

    void update(const sf::Vector2f&);

    void render(sf::RenderTarget& target);

    void input(sf::Event &);

private:
};