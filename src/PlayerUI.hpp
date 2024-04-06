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
#include "Backpack.hpp"
#include "Laser.hpp"

#include "HealthBar.hpp"
#include "TemperatureBar.hpp"

#include "InventoryBar.hpp"
#include "InventoryToolBar.hpp"
#include "InventoryUserCreatedBar.hpp"


class PlayerUI {
    public:
        void load(const int sizeX, const int sizeY) {
            healthBar.load(sizeX, sizeY);
            tempBar.load(sizeX, sizeY);
        }

        void render(sf::RenderTarget& targ) {
            healthBar.render(targ);
            tempBar.render(targ);
        }
        
        void update(Player &player) {
            healthBar.update();
            tempBar.update();
        }

    private:
        HealthBar healthBar;
        TemperatureBar tempBar;
};

class Inventory {
public:
    void load(VoxelManager&vx) {

        currentInventoryBar = "tool";

        inventorybars.insert({"tool", std::make_shared<InventoryToolBar>()});
        inventorybars.insert({"usercreated", std::make_shared<InventoryUserCreatedBar>()});

        addItem(vx, std::make_shared<RocketLauncher>(vx));
        addItem(vx, std::make_shared<DebugPlacer>(vx));
        addItem(vx, std::make_shared<HeatGun>(vx));
        addItem(vx, std::make_shared<GeigerCounter>(vx));
        addItem(vx, std::make_shared<Jetpack>(vx));
        addItem(vx, std::make_shared<IronPickaxe>(vx));
        addItem(vx, std::make_shared<Backpack>(vx));
        addItem(vx, std::make_shared<Laser>(vx));

    } 

    void addItem(VoxelManager&vx, std::shared_ptr<Item> newItem) {
        if(newItem == nullptr) return;

        if(newItem->getCategory() == Item::Category::Pairing)
            inventorybars.at("tool")->addItem(vx, newItem);

        if(newItem->getCategory() == Item::Category::Tool)
            inventorybars.at("tool")->addItem(vx, newItem);

        if(newItem->getCategory() == Item::Category::Usercreated)
            inventorybars.at("usercreated")->addItem(vx, newItem);

    }

    void input(sf::Event &ev) {
        inventorybars.at(currentInventoryBar)->input(ev, currentItemIndex, currentInventoryBar);
    }

    void use(Player& player,const sf::Vector2f &m, World& world) {
        inventorybars.at(currentInventoryBar)->useItem(player, m, world, currentItemIndex);
    }

    void render(sf::RenderTarget& targ) {
        inventorybars.at(currentInventoryBar)->render(targ, currentItemIndex);
    }

    void renderUI(sf::RenderTarget& targ) {
        for(const auto &bar : inventorybars) {
            bar.second->renderUI(targ, currentItemIndex, bar.first == currentInventoryBar);

            if(bar.second->isPointInInventory(Controls::windowCursorPos)) {
                bar.second->onHover();

                if(Controls::useUI()) {
                    currentInventoryBar = bar.first;
                    currentItemIndex = 0;
                }
            }
        }
    }

    std::shared_ptr<Item> getCurrentItem() {
        return inventorybars.at(currentInventoryBar)->getItemAtIndex(currentItemIndex);
    }
    
private:

    std::map<std::string, std::shared_ptr<InventoryBar>> inventorybars;

    std::string currentInventoryBar;
    int currentItemIndex = 0;
};