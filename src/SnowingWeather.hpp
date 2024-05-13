#pragma once
#include "WeatherState.hpp"
#include "VoxelManager.hpp"
#include "Elements/Acid.hpp"
#include "Particle/SnowParticle.hpp"
#include "BackgroundMusic.hpp"

class SnowingWeather : public WeatherState {
    public:
        void load() {
            timer = 0;
            max_time = math::randIntInRange(1500, 3000);

            AmbientSounds::snowAmbience.play();

            setArea();
        }

        void update(VoxelManager& world, Player& player) {
            for(int i = WeatherState::areaStart; i < WeatherState::areaEnd; i++) {
                if(math::randIntInRange(0, 1000) < snowingIntensity) {

                    sf::Vector2i position = sf::Vector2i(i, -100);
                    position.x += math::randIntInRange(-50, 50);

                    world.getChunkIndexer().boundVector(position);

                    world.launchParticle(std::make_shared<SnowParticle>(sf::Vector2f(position)));
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
            AmbientSounds::snowAmbience.stop();
        }
    private:
        int snowingIntensity = 2;
        int timer = 0;
        int max_time = 0;
};