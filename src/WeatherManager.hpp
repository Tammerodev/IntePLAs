#pragma once
#include "VoxelManager.hpp"
#include "ClearWeather.hpp"
#include "SnowingWeather.hpp"
#include "RainingWeather.hpp"

class WeatherManager {
    public:
        void load() {
            WeatherState::currentState->load();
        }

        void update(VoxelManager& world, Player& player) {
            WeatherState::currentState->update(world, player);
        }
    private:
};