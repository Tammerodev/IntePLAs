#pragma once
#include "WeatherState.hpp"
#include "VoxelManager.hpp"
#include "Elements/Snow.hpp"
#include "Particle/RainParticle.hpp"
#include "BackgroundMusic.hpp"

class RainWeather : public WeatherState {
    public:
        void load() {
            timer = 0;
            max_time = math::randIntInRange(1500, 3000);

            setArea();

            AmbientSounds::rainAmbience.play();

        }

        void update(VoxelManager& world, Player& player) {
            for(int i = WeatherState::areaStart; i < WeatherState::areaEnd; i++) {
                if(math::randIntInRange(0, 1000) < rainingIntensity) {
                    
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
                WeatherState::currentState = WeatherState::clearWeather;
                WeatherState::currentState->load();

            }
        }

        void leave() {
            AmbientSounds::rainAmbience.stop();

        }
    private:
        int rainingIntensity = 2;
        int timer = 0;
        int max_time = 0;
};