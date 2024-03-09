#pragma once
#include <SFML/Graphics.hpp>
#include "Type.hpp"

namespace dep {

	typedef sf::Event Event;
	typedef sf::Keyboard KeyBoard;


	class Display : public sf::RenderWindow {
	public:
		Display(dep::vec2I size, const char * title, const uint16_t fpslimit = 0) {
			create(sf::VideoMode(size.x,size.y), title);
			setFramerateLimit(fpslimit);
		}

	private:
	};
}