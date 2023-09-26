#include <SFML/Graphics.hpp>
#include <iostream>
#include "StateManager.hpp"

int main() {

    auto path = std::filesystem::current_path(); //Get working path

    path = path.parent_path();                  // Move back one directory 

    std::cout << "Current working path" << path << std::endl;

    std::filesystem::current_path(path); //Set working path


    StateManager stateManager = StateManager();

    srand(time(0));

    stateManager.createWindow(1080,1920,"IntePLAs",false, 75);
    stateManager.init();
    stateManager.gameLoop();
    return 0;
}