#pragma once
#include "State.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include <filesystem>
#include <SFML/Audio.hpp>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "Settings.hpp"
#include "Palettes/PaletteUI.hpp"

#include "WorldSelectionState.hpp"
#include <iostream>
#include "common.hpp"
#include "MenuBackground.hpp"

#include "Mods/Mods.hpp"

class ModButton {
    public:
		static void buttonCallBack(const tgui::String path, tgui::BackendGui& gui) {
			MainState::currentState->statexit();
			gui.removeAllWidgets();

			MainState::currentState = MainState::menuState;

			MainState::currentState->load(path.toStdString(), gui); 
		}


        void load(tgui::BackendGui& gui, const tgui::Layout2d &position, const std::shared_ptr<CustomGameMod> mod_) {
			mod = mod_;
            info = mod->getModInfo();

			tgui::Button::Ptr button = tgui::Button::create("Exit");
			button->setSize(100, 50);
			button->onClick(buttonCallBack, "", std::ref(gui));

            checkBox = tgui::CheckBox::create();
            checkBox->setPosition(position);

            label = tgui::Label::create(mod->getModName());
            label->setPosition(checkBox->getPosition() + tgui::Layout2d(checkBox->getSize().x * 2, 0));

			gui.add(button);
            gui.add(label);
            gui.add(checkBox);
        }

		void exit() {
			if (!checkBox->isChecked()) {
				mod->disable();
			} else {
				mod->enable();
			}
		}

		std::shared_ptr<CustomGameMod> mod = nullptr;
    private:
        tgui::CheckBox::Ptr checkBox = nullptr; 
        tgui::Label::Ptr label = nullptr;

        std::string info = "";
};

class ModMenu : public MainState {
public:


	bool load(const std::string, tgui::BackendGui& gui) {
		const std::string uipath = "res/img/UI/";
		SettingsLoader::loadSettings();

		backend_gui = &gui;
		MenuMusic::load();
		MenuMusic::play();

		MenuBackground::load();

        for(auto mod : modsInUse) {
            ModButton button;
            button.load(gui, tgui::Layout2d(Display::window_width / 2.5, Display::window_height / 4), mod);

            buttons.push_back(
               button
            );
        }

		try {
            
		} catch(std::exception& ex) {
			prnerr("TGUI failed with : ", ex.what());
		}
		return true;
	}

    void update() {
		MenuBackground::update();
	}

	void input(sf::Event &e) {
		
	}

	void draw(sf::RenderWindow& window, tgui::BackendGui& gui) {
		window.clear(Palette::PaletteUI::Black);

		MenuBackground::render(window);
		
   		gui.draw();

	}

	void statexit() {
		for(auto &button : buttons) {
			button.exit();
		}

        buttons.clear();
	}

private:
    std::vector<ModButton> buttons;
	tgui::BackendGui *backend_gui = nullptr;

	sf::Font font;
};