#pragma once
#include "State.hpp"

class SettingsState : public MainState {
public:
	bool load(const std::string, tgui::BackendGui& gui) {
		return true;
	}
    void update() {printf("update settings state\n");}
	void input(sf::Event &) {}
	void draw(sf::RenderTarget&, tgui::BackendGui&) {}
	void statexit() {};
private:
};