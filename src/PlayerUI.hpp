#pragma once
#include "World.hpp"
#include <iostream>
#include <vector>
#include "RocketLauncher.hpp"
#include "DebugPlacer.hpp"
#include "HeatGun.hpp"
#include "MaterialPack.hpp"
#include "Panel.hpp"
#include "MaterialsUI.hpp"
#include "ItemCreateItem.hpp"

class Inventory {
public:
    void load(VoxelManager&vx) {
        inventory.push_back(std::make_shared<RocketLauncher>(vx));
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
        inventory.push_back(std::make_shared<HeatGun>(vx));
    } 

    void addItem(VoxelManager&vx, std::shared_ptr<Item> newItem) {
        inventory.push_back(newItem);
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
        sf::RectangleShape seperator;
        seperator.setSize(sf::Vector2f(32, 32));
        seperator.setFillColor(sf::Color(0,0,0,0));
        seperator.setOutlineThickness(-1);
        int index = 0;

        for(auto &item : inventory) {
            if(index == currentItemIndex) seperator.setOutlineColor(sf::Color::Black);
            else seperator.setOutlineColor(sf::Color::White);
            const sf::Vector2f position = sf::Vector2f(300 + index * 32, 550);
            seperator.setPosition(position - sf::Vector2f(32 / 2, 32 / 2));
            item->inventory_render(targ, position);
            targ.draw(seperator);
            ++index;
        }
    }

    std::shared_ptr<Item> getCurrentItem() {
        return inventory[currentItemIndex];
    }
private:
    std::vector<std::shared_ptr<Item>> inventory;
    uint16_t currentItemIndex = 0;
};