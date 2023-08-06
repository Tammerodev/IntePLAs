#pragma once
#include <iostream>
#include <vector>
#include "RocketLauncher.hpp"
#include "DebugPlacer.hpp"
#include "HeatGun.hpp"
#include "MaterialPack.hpp"
#include "Panel.hpp"
#include "MaterialsUI.hpp"


class Inventory {
public:
    void load(VoxelManager&vx) {
        inventory.push_back(std::make_shared<RocketLauncher>(vx));
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
        inventory.push_back(std::make_shared<HeatGun>(vx));

        inventory.push_back(std::make_shared<PlaceItem>(vx,"res/img/Tool/fire.png", "res/img/Player/Player.png"));
    } 

    void addItem(VoxelManager&vx, Item* newItem) {
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
    }
    void switchItem() {
        currentItemIndex++;
        if(currentItemIndex >= inventory.size()) {
            currentItemIndex = 0;
        }
    }

    void use(const sf::Vector2f &p,const sf::Vector2f &m, World& world) {
        inventory[currentItemIndex]->use(p,m, world);
    }
    void render(sf::RenderTarget& targ) {
        // Render current item normally
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