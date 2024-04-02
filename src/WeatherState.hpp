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


        static inline float areaStart = 0.f;
        static inline float areaEnd   = 0.f;

        static void setArea() {
            areaStart = math::randIntInRange(0, worldSize::world_sx / 2);
            areaEnd = math::randIntInRange(worldSize::world_sx / 3, worldSize::world_sx / 1.5);

            loginf("Start: ", areaStart, ".");
            loginf("End: ", areaEnd, ".");

        }

    private:
};