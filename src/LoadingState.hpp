#pragma once
#include "State.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include <filesystem>
#include <SFML/Audio.hpp>

#include <future>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

std::atomic<bool> exit_thread_flag{false};

void load_screen(sf::RenderWindow& window) {
	while(!exit_thread_flag) {
		//window.clear(sf::Color::Red);
	}
}

class Loading : public MainState {
public:

	bool load(const std::string path, tgui::BackendGui& gui) { 
		path_local = path;
		gui_ref_local = &gui;

		return true;
	}
    void update() {

	}
	void input(sf::Event &e) {

	}
	void draw(sf::RenderWindow& window, tgui::BackendGui& gui) {
		// This needs to be in the draw function,
		// becouse we need access to the window

		// This will only be executed once, becouse 
		// gamestate->load blocks the thread and changes the 
		// state immediatly after loading

        thread = std::thread(load_screen, std::ref(window));

		MainState::gameState->load(path_local, *gui_ref_local);
		MainState::currentState = MainState::gameState;

		// Request thread to stop
		exit_thread_flag = true;

		// Wait for thread to response / terminate

		thread.join();
	}
	void statexit() {

	}

private:
	std::thread thread;
	std::string path_local;
	tgui::BackendGui *gui_ref_local;


};