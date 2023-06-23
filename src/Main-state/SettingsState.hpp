#pragma once
#include "State.hpp"

class SettingsState : public MainState {
public:
	virtual bool load() {
		return true;
	}
    virtual void update() {printf("update settings state\n");}
	virtual void input(sf::Event &) {}
	virtual void draw(sf::RenderTarget&) {}
private:
};