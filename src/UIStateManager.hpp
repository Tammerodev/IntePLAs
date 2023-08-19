#pragma once
#include "UIState.hpp"
#include "ItemCreatorUIState.hpp"
#include "NoUIState.hpp"

#include "Entity/Player/Controls.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

class UIStateManager {
public:

    bool load(tgui::BackendGui& gui, Inventory &inv, VoxelManager &vx);

    void update(const sf::Vector2f&);

    void render(sf::RenderTarget& target, tgui::BackendGui& gui);

    void input(sf::Event &);

private:
};