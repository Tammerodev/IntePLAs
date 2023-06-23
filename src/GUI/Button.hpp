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
};