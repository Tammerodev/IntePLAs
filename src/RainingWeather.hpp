#pragma once
#include "WeatherState.hpp"
#include "VoxelManager.hpp"
#include "Snow.hpp"

class RainWeather : public WeatherState {
    public:
        void load() {
            timer = 0;
            max_time = math::randIntInRange(500, 1000);
        }

        void update(VoxelManager& world, Player& player) {
            for(int i = 0; i < snowingIntensity; i++) {
                sf::Vector2i position = sf::Vector2i(player.getPhysicsComponent().transform_position);
                position.y = world.getUpdateArea().top;
                position.x += math::randIntInRange(-(world.getUpdateArea().width / 2), (world.getUpdateArea().width / 2));

                world.getChunkIndexer().boundVector(position);

                if(world.getChunkIndexer().getVoxelAt(position.x, position.y).value == 0) {
                    world.addElement(position.x, position.y, std::make_shared<Water>(position.x, position.y));
                }
            }

            timer++;
            if(timer > max_time) {
                const int nextWeather = math::randIntInRange(0, 1);

                WeatherState::currentState->leave();

                if(nextWeather)
                    WeatherState::currentState = WeatherState::snowingWeather;
                else 
                    WeatherState::currentState = WeatherState::clearWeather;

                WeatherState::currentState->load();

            }
        }

        void leave() {

        }
    private:
        int snowingIntensity = 2;
        int timer = 0;
        int max_time = 0;
};