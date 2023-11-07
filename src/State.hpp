#pragma once
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>


class MainState {
public:
	virtual bool load(const std::string, tgui::BackendGui&) = 0;
	virtual void update() = 0;
	virtual void input(sf::Event &) = 0;
	virtual void draw(sf::RenderWindow&, tgui::BackendGui&) = 0;
	virtual void statexit() = 0;
	virtual bool loaded() {return true; }

	virtual const sf::Event event_sig() {
		sf::Event mouseButtonEvent {};
		mouseButtonEvent.type = sf::Event::SensorChanged;

		return mouseButtonEvent;
	}

	static MainState* currentState;
	static MainState *menuState, *settingsState, *gameState, *loadState;
private:

};