#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>

class MainState {
public:
	virtual bool load() = 0;
	virtual void update() = 0;
	virtual void input(sf::Event &) = 0;
	virtual void draw(sf::RenderTarget&) = 0;
	static MainState* currentState;
	static MainState *menuState, *settingsState, *gameState;
private:

};