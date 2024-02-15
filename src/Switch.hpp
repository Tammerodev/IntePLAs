#pragma once
#include "ElectricComponent.hpp"
#include "math.hpp"
#include "Controls.hpp"

class Switch : public ElectricComponent {
    public:
        Switch(int x_, int y_) : ElectricComponent() {
            indicator.setPosition(x_, y_);
            indicator.setSize(sf::Vector2f(1, 1));
            indicator.setFillColor(sf::Color(0,0,0,0));

            indicator.setOutlineThickness(0.5f);
        }

        void render(sf::RenderTarget& target) {
            target.draw(indicator);
        }


        component_data update(ChunkIndexer& world) {
            Voxel& voxel = world.boundGetVoxelAt(x, y);

            sf::Vector2i pos(Controls::worldCursorPos.x, Controls::worldCursorPos.y);

            cooldown++;

            if(*this == pos) {
                if(Controls::use() && cooldown > cooldownTime) {
                    cooldown = 0;

                    if(voxel.value == VoxelValues::SWITCH) {
                        voxel.value = VoxelValues::COPPER;
                        color.b = 255;
                    } else {
                        voxel.value = VoxelValues::SWITCH;
                        color.b = 0;
                    }
                }
            }

            world.boundSetImagePixelAt(x, y, color);
            indicator.setOutlineColor(color);

            component_data data {};
            return data;
        }   

        int getVoltage() {
            return voltage;
        }

        void setVoltage(int) {

        }

        bool clear() {
            return rem;
        }
    private:
        sf::Color color = elm::getInfoFromType(VoxelValues::SWITCH).color;
        sf::RectangleShape indicator;

        bool rem = false;

        int treshold = 10;
        int voltage = 0;

        int cooldown = 0;
        const int cooldownTime = 10;
};