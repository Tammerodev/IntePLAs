#pragma once
#include "State.hpp"
#include "Button.hpp"
#include <filesystem>

class MenuState : public MainState {
public:
	bool load(const std::string) {
		logoTx.loadFromFile("res/img/title/logo.png");
		logo.setTexture(logoTx);
		logo.setPosition(0,0);
		logo.setScale(5,5);


		playbtn_tx.loadFromFile("res/img/title/playbtn.png");

		font.loadFromFile("res/Fonts/VT323.ttf");
		int index = 0;
		for (const auto & entry : std::filesystem::directory_iterator(path)) {
			Button* b = new Button(playbtn_tx, sf::Vector2f(150, 100 + index * 50), sf::Vector2f(2,2), sf::IntRect(0,0,64,24), font);
			b->setString(entry.path());
			b->move(-150,0);
        	clickables.push_back(b);
			++index;
		}

		tx.loadFromFile("res/world/forest.png");
		background.setTexture(tx);
		background.move(0, 500);

		return true;
	}
    void update() {
		background.move(0, -0.1f);
	}
	void input(sf::Event &e) {
		if(e.type == sf::Event::Closed) exit(0);
	}
	void draw(sf::RenderTarget& window) {
		window.clear(sf::Color(20, 22, 33));
		logo.setTexture(logoTx);
		window.draw(background);
		window.draw(logo);

		for(auto &i : clickables) {
			window.draw(*i);
			i->drawText(window);

			const sf::Vector2f pos = sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition())));

			if(i->getState(pos) == Button::ButtonState::Click) { 
				statexit();
				MainState::currentState = MainState::gameState;
				MainState::currentState->load(i->getText()); 
			}
			if(i->getState(pos) == Button::ButtonState::Hover) { 
				i->setTextureRect(sf::IntRect(0,24,64,24));
			} else {
				i->setTextureRect(sf::IntRect(0,0,64,24));
			}
		}
	}
	void statexit() {};
private:
	sf::Texture tx;
	sf::Sprite background;
	std::vector<Button*> clickables;
	sf::Font font;

	std::string path = "res/saves/";
	sf::Texture logoTx;
	sf::Sprite logo;

	sf::Texture playbtn_tx;
};