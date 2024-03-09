#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <iostream>

namespace dep {
	struct Color {
		Color(float nr, float ng, float nb, float na) {
			r = nr;
			g = ng;
			b = nb;
			a = na;
		}

		float r, g, b, a;
	};

	typedef sf::Vector2f vec2D;
	typedef sf::Vector2i vec2I;

	typedef sf::Vector3f vec3D;

	typedef std::string string;
	typedef unsigned int GL_BIT;

	void log(const std::string& m) {
		std::cout << m << '\n';
	}
}