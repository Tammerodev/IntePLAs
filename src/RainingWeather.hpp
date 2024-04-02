#pragma once
#include "WeatherState.hpp"
#include "VoxelManager.hpp"
#include "Snow.hpp"
#include "RainParticle.hpp"

class RainWeather : public WeatherState {
    public:
        void load() {
            timer = 0;
            max_time = math::randIntInRange(500, 1000);

            setArea();
        }

        void update(VoxelManager& world, Player& player) {
            for(int i = WeatherState::areaStart; i < WeatherState::areaEnd; i++) {
                if(math::randIntInRange(0, 100) < rainingIntensity) {
                    
                    sf::Vector2i position = sf::Vector2i(i, 0);
                    position.x += math::randIntInRange(-50, 50);

                    world.getChunkIndexer().boundVector(position);

                    world.launchParticle(std::make_shared<RainParticle>(sf::Vector2f(position)));
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
        int rainingIntensity = 20;
        int timer = 0;
        int max_time = 0;
};