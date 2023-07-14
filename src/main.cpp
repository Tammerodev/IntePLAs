/*
    COMPILE :
    g++ src/Start/main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
*/


#include <iostream>
#include "StateManager.hpp"
#include <memory>

int main() {
    std::unique_ptr<StateManager> stateManager = std::make_unique<StateManager>();
    srand(time(0));
    stateManager->createWindow(1200,1200,"IntePLAs",false, 75);
    stateManager->init();
    stateManager->gameLoop();
    return 0;
}