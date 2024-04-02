#include "WeatherManager.hpp"

WeatherState* WeatherState::clearWeather = new ClearWeather();
WeatherState* WeatherState::snowingWeather = new SnowingWeather();
WeatherState* WeatherState::rainingWeather = new RainWeather();


WeatherState* WeatherState::currentState = WeatherState::snowingWeather;