#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Sprite {
public:
	enum ButtonState {
		None, Hover, Click
	};
	Button() {};
	Button(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f scale, sf::IntRect area) {
		setTexture(texture);
		setTextureRect(area);
		setPosition(pos);
		setScale(scale);
	}
	Button(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f scale, sf::IntRect area, sf::Font font) {
		fontc = font;
		setTexture(texture);
		setTextureRect(area);
		setPosition(pos);
		setScale(scale);

		text.setFont(fontc);
		text.setPosition(pos);
		
	}

	const std::string getText() const {
		return text.getString();
	}

	void drawText(sf::RenderTarget&r) {
		r.draw(text);
	}

	void setString(std::string str) {
		text.setString(str);
	}

	ButtonState getState(const sf::Vector2f &mousePos) {
		if (getGlobalBounds().contains(mousePos)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				return ButtonState::Click;
			}
			return ButtonState::Hover;
		}
		return ButtonState::None;
	}
private:
	sf::Font fontc;
	sf::Text text;
};