#pragma once

#include <map>
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

#include "Display.hpp"

namespace dep {
	class State {
		public:
			virtual void input(sf::Event&) = 0;
			virtual void update(dep::Display& display) = 0;
			virtual void render(dep::Display&) = 0;
			virtual void leave() = 0;

			inline static std::vector<std::pair<dep::string, std::shared_ptr<State>>> s_states = {};
			inline static std::shared_ptr<State> s_currentState = nullptr;
	};
}