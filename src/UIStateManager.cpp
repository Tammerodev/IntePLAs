#include "UIStateManager.hpp"

UIState* UIState::itemcreator = new ItemCreator();
UIState* UIState::nostate = new NoUIState();
UIState* UIState::death = new DeadUIState();
UIState* UIState::pause = new PauseUIState();
UIState* UIState::photoMode = new PhotoModeUIState();
UIState* UIState::saveExit = new SaveExitUIState();

UIState *UIState::currentState = UIState::nostate;

bool UIStateManager::load(tgui::BackendGui& gui, Inventory &inv, VoxelManager &vx) {

    this->gui = &gui;
    this->inv = &inv;
    this->vx = &vx;

    prndd("Loaded UI State manager.");

    return UIState::currentState->load(gui, inv, vx);
}

void UIStateManager::update(const sf::Vector2f&pos) {
    UIState::currentState->update(pos);

    if(gui == nullptr) return;
    if(inv == nullptr) return;
    if(vx  == nullptr) return;

    if(PlayerGlobal::health <= 0 && UIState::currentState != UIState::death) {
        
        UIState::currentState->statexit();
        UIState::currentState = UIState::death;
        UIState::currentState->load(*gui, *inv, *vx);
    }

    if(!UIStateChangeRequest::newState.empty()) {
        if(UIStateChangeRequest::newState == "i")
            UIState::currentState = UIState::itemcreator;
        if(UIStateChangeRequest::newState == "p")
            UIState::currentState = UIState::pause;
        if(UIStateChangeRequest::newState == "d")
            UIState::currentState = UIState::death;
        if(UIStateChangeRequest::newState == "n")
            UIState::currentState = UIState::nostate;
            
        UIState::currentState->load(*gui, *inv, *vx);

        UIStateChangeRequest::newState.clear();
    }
}

void UIStateManager::render(sf::RenderTarget &target, tgui::BackendGui& gui) {
    UIState::currentState->draw(target, gui);
}

void UIStateManager::input(sf::Event &e) {
    UIState::currentState->input(e);
}
