#pragma once

#include "Game.hpp"

class GameState : public MainState {
public:
	GameState() {

	}
	bool load(const std::string s, tgui::BackendGui& gui);
    void update();
	void input(sf::Event &ev);
	void statexit();

	bool loaded() {
		return game.getLoaded();
	}

	uint64_t timer = 0.f;

	const sf::Event event_sig() {
		
		sf::Event tEvent;
		
		if(Controls::useUI()) {

			timer += 1;
			sf::Event::MouseButtonEvent press;
			press.button = sf::Mouse::Button::Left;
			press.x = Controls::windowCursorPos.x;
			press.y = Controls::windowCursorPos.y;

			if(timer < 5)
				tEvent.type = sf::Event::MouseButtonPressed;
			else {
				tEvent.type = sf::Event::MouseButtonReleased;
				timer = 0;
			}
			tEvent.mouseButton = press;
		} 
		else {
			tEvent.type = sf::Event::SensorChanged;
		}

		return tEvent;
	}

	void draw(sf::RenderWindow& window, tgui::BackendGui&);
private:
	sf::RenderWindow *window_ptr = nullptr;
	tgui::BackendGui *gui_ptr 	 = nullptr;

	Game game;
	std::string path = "";

	Cursor cursor;
};