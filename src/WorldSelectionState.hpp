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

			saves_list = tgui::ScrollablePanel::create(tgui::Layout2d(300, 600), "Saves");

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

                position.x = 0;
                position.y = 300 + buttonIndex * button->getSize().x + 6;

                button->setPosition(position);
                button->onClick(loadGame, button->getText(), std::ref(gui));
                saves_list->add(button);

                buttonIndex++;
            }
        }
    }

};