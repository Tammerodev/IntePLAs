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
#include "GeigerCounter.hpp"
#include "Jetpack.hpp"
#include "IronPickaxe.hpp"

#include "HealthBar.hpp"
#include "RadiationBar.hpp"



class PlayerUI {
    public:
        void load(const int sizeX, const int sizeY) {
            healthBar.load(sizeX, sizeY);
            radiationBar.load(sizeX, sizeY);
        }

        void render(sf::RenderTarget& targ) {
            healthBar.render(targ);
            radiationBar.render(targ);
        }
        
        void update(Player &player) {
            healthBar.update();
            radiationBar.update();
        }

    private:
        HealthBar healthBar;
        RadiationBar radiationBar;
};

class Inventory {
public:
    void load(VoxelManager&vx) {
        inventory.push_back(std::make_shared<RocketLauncher>(vx));
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
        inventory.push_back(std::make_shared<HeatGun>(vx));
        inventory.push_back(std::make_shared<GeigerCounter>(vx));
        inventory.push_back(std::make_shared<Jetpack>(vx));
        inventory.push_back(std::make_shared<IronPickaxe>(vx));

    } 

    void addItem(VoxelManager&vx, std::shared_ptr<Item> newItem) {
        inventory.push_back(newItem);
    }

    void input(sf::Event &ev) {

        currentItemIndex += Controls::inventoryMove(ev);

        // Bound checking
        if(currentItemIndex < 0)
            currentItemIndex = inventory.size() - 1;

        if(currentItemIndex >= (int16_t)inventory.size())
            currentItemIndex = 0;

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

        sf::RectangleShape bg;
        sf::RectangleShape seperator;

        seperator.setSize(sf::Vector2f(32, 32));
        seperator.setFillColor(sf::Color(0,0,0,100));
        seperator.setOutlineThickness(-1);

        bg.setSize(sf::Vector2f(32, 2000));
        bg.setPosition(0, 0);
        bg.setFillColor(sf::Color(32, 32, 32));

        int index = 0;

        targ.draw(bg);

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

            const sf::Vector2f position = sf::Vector2f(0, (index * 32));

            seperator.setPosition(position);

            //targ.draw(seperator);
            item->inventory_render(targ, position + sf::Vector2f(10, 10));

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