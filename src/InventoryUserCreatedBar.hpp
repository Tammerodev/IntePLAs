#pragma once
#include "InventoryBar.hpp"

class InventoryUserCreatedBar : public InventoryBar{
    public:
        InventoryUserCreatedBar() : InventoryBar() {
            inventoryPosition = sf::Vector2f(48.f, 0.f);
        }
    private:

};