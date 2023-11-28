#pragma once
#include "ChunkIndexerVoxelContainer.hpp"

class WeatherState {
    public:
        virtual void load() = 0;
        virtual void update(VoxelManager&, Player&) = 0;
        virtual void leave() = 0;

        static WeatherState* currentState;
        static WeatherState* clearWeather;
        static WeatherState* snowingWeather;
        static WeatherState* rainingWeather;

    private:
};