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

    void input(sf::Event &ev) {
        if(ev.type == sf::Event::MouseWheelMoved) {
            const int moveAmount = ev.mouseWheel.delta;

            currentItemIndex += moveAmount;

            // Bound checking
            if(currentItemIndex < 0)
                currentItemIndex = inventory.size() - 1;

            if(currentItemIndex >= inventory.size())
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
        seperator.setFillColor(sf::Color(0,0,0,100));
        seperator.setOutlineThickness(-1);

        int index = 0;

        for(auto &item : inventory) {

            if(index == currentItemIndex) {
                seperator.setOutlineColor(sf::Color::White);
                seperator.setOutlineThickness(-3.f);
                item->getSprite().setScale(1.5, 1.5);
            }

            else { 
                seperator.setOutlineColor(sf::Color::Black); 
                seperator.setOutlineThickness(-1);
                item->getSprite().setScale(1.0, 1.0);

            }

            const sf::Vector2f position = sf::Vector2f((targ.getView().getSize().x / 3) + (index * 32), 64);
            seperator.setPosition(position - sf::Vector2f(32 / 2, 32 / 2));

            targ.draw(seperator);
            item->inventory_render(targ, position);

            ++index;
        }
    }

    std::shared_ptr<Item> getCurrentItem() {
        return inventory[currentItemIndex];
    }
    
private:

    sf::Vector2f inventoryPosition = sf::Vector2f();

    std::vector<std::shared_ptr<Item>> inventory;
    int16_t currentItemIndex = 0;
};