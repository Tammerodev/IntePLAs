#pragma once
#include "State.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/GUI/Button.hpp"

class MenuState : public MainState {
public:
	virtual bool load() {
		logoTx.loadFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/img/title/logo.png");
		logo.setTexture(logoTx);
		logo.setPosition(0,0);
		logo.setScale(5,5);


		playbtn_tx.loadFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/img/title/playbtn.png");
		playbtn = Button(playbtn_tx, sf::Vector2f(500, 600), sf::Vector2f(2,2), sf::IntRect(0,0,64,24));
		return true;
	}
    virtual void update() {
	}
	virtual void input(sf::Event &e) {
		if(e.type == sf::Event::Closed) exit(0);
	}
	virtual void draw(sf::RenderTarget& window) {
		window.clear(sf::Color::Magenta);
		logo.setTexture(logoTx);
		window.draw(logo);
		window.draw(playbtn);

		const sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition());

		if(playbtn.getState(pos) == Button::ButtonState::Click) { 
			MainState::currentState = MainState::gameState;
			MainState::currentState->load(); 
		}
		if(playbtn.getState(pos) == Button::ButtonState::Hover) { 
			playbtn.setTextureRect(sf::IntRect(0,24,64,24));
		} else {
			playbtn.setTextureRect(sf::IntRect(0,0,64,24));
		}
	}
private:
	sf::Texture logoTx;
	sf::Sprite logo;

	sf::Texture playbtn_tx;
	Button playbtn;
};