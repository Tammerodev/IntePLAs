#pragma once
#include "Item.hpp"
#include "Palettes/PaletteUI.hpp"
#include "NoneItem.hpp"

class InventoryBar {
    public:

        InventoryBar() {
            inventory.push_back(std::make_shared<NoneItem>());
        }

        void addItem(VoxelManager&vx, std::shared_ptr<Item> newItem) {
            inventory.push_back(newItem);
        }

        void useItem(Player& player,const sf::Vector2f &m, World& world, int currentItemIndex) {
            inventory.at(std::clamp(currentItemIndex, 0, (int)inventory.size() - 1))->use(player, m, world);
        }

        std::shared_ptr<Item> getItemAtIndex(const int index) {
            return inventory.at(std::clamp(index, 0, (int)inventory.size() - 1));
        }

        void render(sf::RenderTarget &targ, int currentItemIndex) {
            inventory.at(std::clamp(currentItemIndex, 0, (int)inventory.size() - 1))->getSprite().setScale(1.0f, 1.0f);
            inventory.at(std::clamp(currentItemIndex, 0, (int)inventory.size() - 1))->render(targ);
        }

        void renderUI(sf::RenderTarget &targ, int currentItemIndex, bool isSelected) {
            sf::RectangleShape seperator;

            seperator.setSize(sf::Vector2f(32, 32));
            seperator.setFillColor(sf::Color(0,0,0,100));
            seperator.setOutlineThickness(-1);

            bg.setSize(sf::Vector2f(32, 2000));
            bg.setPosition(inventoryPosition);
            bg.setFillColor(sf::Color(32, 32, 32));

            int index = 0;

            targ.draw(bg);


            for(auto it = inventory.begin(); it != inventory.end();) {

                auto &item = *it; 

                if(index == currentItemIndex && isSelected) {
                    seperator.setOutlineColor(sf::Color::White);
                    seperator.setOutlineThickness(-3.f);
                    item->getSprite().setScale(1.5, 1.5);
                }

                else { 
                    seperator.setOutlineColor(sf::Color::Black); 
                    seperator.setOutlineThickness(-1);
                    item->getSprite().setScale(1.0, 1.0);

                }

                const sf::Vector2f position = inventoryPosition + sf::Vector2f(0, (index * 32));

                seperator.setPosition(position);

                targ.draw(seperator);
                item->inventory_render(targ, position + sf::Vector2f(10, 10));

                if(item->remove_item()) {
                    it = inventory.erase(it);
                } else {
                    ++it;
                }
                ++index;
            }

            bg.setOutlineThickness(0.f);
        }

        bool isPointInInventory(const sf::Vector2f &pos) {
            return bg.getGlobalBounds().contains(pos);
        }

        void onHover() {
            bg.setOutlineThickness(2.5);
            bg.setOutlineColor(Palette::PaletteUI::White);
        }

        void input(sf::Event &ev, int &currentItemIndex, std::string& currentBar) {
            currentItemIndex += Controls::inventoryMove(ev);

            // Bound checking
            if(currentBar == "tool") {
                if(currentItemIndex < 0)
                    currentItemIndex = 0;

                if(currentItemIndex >= (int16_t)inventory.size()) {
                    currentBar = "usercreated";
                    currentItemIndex = 0;
                }
            }

            if(currentBar == "usercreated") {
                if(currentItemIndex < 0) {
                    currentBar = "tool";
                    currentItemIndex = 0;
                }

                if(currentItemIndex >= (int16_t)inventory.size()) {
                    currentItemIndex = inventory.size() - 1;
                }
            }
        }

    protected:
        sf::RectangleShape bg;

        sf::Vector2f inventoryPosition = sf::Vector2f();
        std::vector<std::shared_ptr<Item>> inventory;
};