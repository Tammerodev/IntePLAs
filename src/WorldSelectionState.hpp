#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>
#include "common.hpp"
#include "Palettes/PaletteUI.hpp"
#include "Settings.hpp"

#include <filesystem>
#include <TGUI/AllWidgets.hpp>
#include "MenuBackground.hpp"

namespace fs = std::filesystem;


class WorldSelectionState : public MainState {
public:
	bool load(const std::string path, tgui::BackendGui& gui) {

		try {
        	theme.load("res/themes/nanogui.style");

			saves_list = tgui::ScrollablePanel::create(tgui::Layout2d(Display::window_width, Display::window_height), "Saves");
			saves_list->setPosition((Display::window_width / 2.f) - saves_list->getSize().x / 2.f, (Display::window_height / 2.f) - saves_list->getSize().y / 2.f);
			saves_list->setRenderer(theme.getRenderer("DarkPanel"));

            serchSaves(gui);

            gui.add(saves_list);

		} catch (std::exception& ex) {
			prnerr("Failed loading world selection gui with code : ", ex.what());
			return false;
		}

		return true;
	}

    void update() {
		
		MenuBackground::update();
	}

	void input(sf::Event &) {

	}

	void draw(sf::RenderWindow& window, tgui::BackendGui&gui) {
		window.clear(Palette::PaletteUI::Black);
		MenuBackground::render(window);
    	gui.draw();


	}

	void statexit() {

	}

private:
    tgui::ScrollablePanel::Ptr saves_list = nullptr;
	tgui::Theme theme;
private:

	static void loadGame(const tgui::String path, tgui::BackendGui& gui) {
		MainState::currentState->statexit();
		gui.removeAllWidgets();

		MainState::currentState = MainState::loadState;


		MainState::currentState->load(path.toStdString(), gui); 
	}


    void serchSaves(tgui::BackendGui &gui) {
        std::string savesFolderPath = StorageSettings::save_path;

        int buttonIndex = 0;

        for (const auto& entry : fs::directory_iterator(savesFolderPath)) {
            if (fs::is_directory(entry.path())) {
                tgui::Layout2d position;
                auto button = tgui::Button::create(entry.path().filename().c_str());
				button->setSize(300, 80);

				position.y = 100;
                position.x = (Display::window_width / 2) - button->getSize().x / 2;

				button->setRenderer(theme.getRenderer("Button"));

                button->setPosition(position);
                button->onClick(loadGame, button->getText(), std::ref(gui));
                saves_list->add(button);

                buttonIndex++;
            }
        }
    }

};