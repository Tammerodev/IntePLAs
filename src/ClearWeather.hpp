#pragma once
#include "WeatherState.hpp"

class ClearWeather : public WeatherState {
    public:
        void load() {
            timer = 0;
            max_time = math::randIntInRange(500, 1000);
        }

        void update(VoxelManager& vox, Player&) {
            timer++;
            if(timer > max_time) {
                const int nextWeather = math::randIntInRange(0, 1);

                WeatherState::currentState->leave();

                if(nextWeather)
                    WeatherState::currentState = WeatherState::rainingWeather;
                else 
                    WeatherState::currentState = WeatherState::clearWeather;

                WeatherState::currentState->load();

            }
        }

        void leave() {

        }
    private:
        int timer = 0;
        int max_time = 0;
};