#pragma once
#include <iostream>
#include <vector>
#include "RocketLauncher.hpp"
#include "DebugPlacer.hpp"
#include "HeatGun.hpp"
#include "MaterialPack.hpp"


class MaterialsUI {
public:
    void load() {
        for(int i = 0; i < barChart.size(); i++) {
            barChart.at(i) = sf::RectangleShape();
            barChart.at(i).setFillColor(sf::Color::Red);
            barChart.at(i).setPosition(-100, i * barDistance);
            barChart.at(i).setSize(sf::Vector2f(10, barHeight));
        }
    }

    void render(sf::RenderTarget& target) {
        for(auto &bar : barChart) {
            target.draw(bar);
        }
    }

    void update() {
        if(modified) {
            barChart.at(0).setSize(sf::Vector2f(10 + materials.carbon, barHeight));
            barChart.at(1).setSize(sf::Vector2f(10 + materials.lithium, barHeight));
            barChart.at(2).setSize(sf::Vector2f(10 + materials.magnesium, barHeight));
            // TODO extend
            modified = false;
        }
    }
private:
    MaterialPack  materials;

    const float barHeight = 10;
    const float barDistance = 15;

    std::array<sf::RectangleShape, 9> barChart;

    bool modified = false;
private:

    void addPack(const MaterialPack& matPack) {
        materials.carbon += matPack.carbon;
        materials.lithium += matPack.lithium;
        materials.magnesium += matPack.magnesium;
        materials.sodium+= matPack.sodium;
        materials.aluminium += matPack.aluminium;
        materials.silicon += matPack.silicon;
        materials.copper += matPack.copper;
        materials.titanium += matPack.titanium;
        materials.lead += matPack.lead;

        modified = true;
    }
};


class Inventory {
public:
    void load(VoxelManager&vx) {
        inventory.push_back(std::make_shared<RocketLauncher>(vx));
        inventory.push_back(std::make_shared<DebugPlacer>(vx));
        inventory.push_back(std::make_shared<HeatGun>(vx));
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