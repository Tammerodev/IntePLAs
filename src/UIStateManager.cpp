#include "UIStateManager.hpp"

UIState* UIState::itemcreator = new ItemCreator();
UIState* UIState::nostate = new NoUIState();
UIState* UIState::death = new DeadUIState();
UIState* UIState::pause = new PauseUIState();
UIState* UIState::photoMode = new PhotoModeUIState();

UIState *UIState::currentState = UIState::nostate;

bool UIStateManager::load(tgui::BackendGui& gui, Inventory &inv, VoxelManager &vx) {

    this->gui = &gui;
    this->inv = &inv;
    this->vx = &vx;

    return UIState::currentState->load(gui, inv, vx);
}

void UIStateManager::update(const sf::Vector2f&pos) {
    UIState::currentState->update(pos);

    if(PlayerGlobal::health <= 0 && UIState::currentState != UIState::death) {
        
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx  == nullptr) return;
        
        UIState::currentState->statexit();
        UIState::currentState = UIState::death;
        UIState::currentState->load(*gui, *inv, *vx);
    }
}

void UIStateManager::render(sf::RenderTarget &target, tgui::BackendGui& gui) {
    UIState::currentState->draw(target, gui);
}

void UIStateManager::input(sf::Event &e) {
    UIState::currentState->input(e);
}
