#include "UIStateManager.hpp"

UIState* UIState::itemcreator = new ItemCreator();
UIState* UIState::nostate = new NoUIState();

UIState *UIState::currentState = UIState::itemcreator;

bool UIStateManager::load(tgui::BackendGui& gui, Inventory &inv, VoxelManager &vx) {
    return UIState::currentState->load(gui, inv, vx);
}

void UIStateManager::update(const sf::Vector2f&pos) {
    UIState::currentState->update(pos);
}

void UIStateManager::render(sf::RenderTarget &target, tgui::BackendGui& gui) {
    UIState::currentState->draw(target, gui);
}

void UIStateManager::input(sf::Event &e) {
    UIState::currentState->input(e);
}
