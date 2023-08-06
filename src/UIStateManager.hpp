#pragma once
#include "UIState.hpp"
#include "ItemCreatorUIState.hpp"
#include "NoUIState.hpp"

class UIStateManager {
public:

    bool load();

    void update();

    void render(sf::RenderTarget& target);

    void input(sf::Event &);

private:
};