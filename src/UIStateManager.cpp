#include "UIStateManager.hpp"

UIState* UIState::itemcreator = new ItemCreator();
UIState* UIState::nostate = new NoUIState();

UIState *UIState::currentState = UIState::itemcreator;
bool UIStateManager::load() {
    return UIState::currentState->load();
}

void UIStateManager::update() {
    UIState::currentState->update();
}

void UIStateManager::render(sf::RenderTarget &target) {
    UIState::currentState->draw(target);
}

void UIStateManager::input(sf::Event &e) {
    UIState::currentState->input(e);
}
