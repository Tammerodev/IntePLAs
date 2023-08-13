#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class UIState {
public:
	virtual bool load() = 0;
	virtual void update(const sf::Vector2f&) = 0;
	virtual void input(sf::Event &) = 0;
	virtual void draw(sf::RenderTarget&) = 0;
	virtual void statexit() = 0;
	static UIState* currentState;
	static UIState *nostate, *itemcreator;
private:

};