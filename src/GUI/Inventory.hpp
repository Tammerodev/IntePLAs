#pragma once
#include <iostream>
#include <vector>
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/IObjects/RocketLauncher.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/IObjects/DebugPlacer.hpp"

class Inventory {
public:
    void load(VoxelManager&vx) {
        inventory.push_back(std::make_shared<RocketLauncher>(vx));
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
    } 

    void switchItem() {
        currentItemIndex++;
        if(currentItemIndex >= inventory.size()) {
            currentItemIndex = 0;
        }
    }

    void use(const sf::Vector2f &p,const sf::Vector2f &m ) {
        inventory[currentItemIndex]->use(p,m);
    }
    void render(sf::RenderTarget& targ) {
        // Render current item normally
        inventory[currentItemIndex]->getSprite().setColor(sf::Color(255,255,255,255));

        inventory[currentItemIndex]->getSprite().setScale(1.0f, 1.0f);
        inventory[currentItemIndex]->render(targ);
    }
    void renderUI(sf::RenderTarget& targ) {
        
        
    }

    std::shared_ptr<Item> getCurrentItem() {
        return inventory[currentItemIndex];
    }
private:
    std::vector<std::shared_ptr<Item>> inventory;
    uint16_t currentItemIndex = 0;
};