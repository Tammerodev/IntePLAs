#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

class UIState {
public:
	virtual bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) = 0;
	virtual void update(const sf::Vector2f&) = 0;
	virtual void input(sf::Event &) = 0;
	virtual void draw(sf::RenderTarget&, tgui::BackendGui& gui) = 0;
	virtual void statexit() = 0;
	static UIState* currentState;
	static UIState *nostate, *itemcreator, *photoMode, *death, *pause, *saveExit;
private:

};