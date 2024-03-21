#include <SFML/Graphics.hpp>
#include <iostream>
#include "StateManager.hpp"

int main() {

    auto path = std::filesystem::current_path(); //Get working paths

    path = path.parent_path();

    std::cout << "Current working path" << path << std::endl;

    std::filesystem::current_path(path); //Set working path


    StateManager stateManager = StateManager();

    unsigned int seed = time(0);

    srand(seed);
    std::srand(seed);

    stateManager.createWindow(1080, 1920, "inteplas", false, 75);
    stateManager.init();
    stateManager.gameLoop();
    return 0;
}