#pragma once
#include "State.hpp"
#include "common.hpp"
#include "JsonManager.hpp"
#include "Settings.hpp"
#include "SettingsStateTestWorld.hpp"

#include <TGUI/AllWidgets.hpp>
#include "Palettes/PaletteUI.hpp"


struct Writers {
	tgui::CheckBox::Ptr useParticleEffectsCheckBox = nullptr;
	tgui::TextArea::Ptr savePathtextArea = nullptr;

	JsonWriter jsonWriterGraphics;
	JsonWriter jsonWriterStorage;
	JsonWriter jsonWriterWorld;

	void writeGraphics() {
		if(useParticleEffectsCheckBox == nullptr) return;

		std::string isChecked;
		if(useParticleEffectsCheckBox->isChecked()) 
			isChecked = "true";
		else 
			isChecked = "false";

		jsonWriterGraphics.writeParameter("use-particle-effects", isChecked);
		jsonWriterGraphics.close();
	}

	void writeStorage() {
		if(savePathtextArea == nullptr) return;

		jsonWriterStorage.writeParameter("use-particle-effects", savePathtextArea->getText().toStdString());
		jsonWriterGraphics.close();
	} 

	void closeAll() {
		jsonWriterGraphics.close();
		jsonWriterStorage.close();
		jsonWriterWorld.close();
	}
};

class SettingsState : public MainState {
public:
	bool load(const std::string, tgui::BackendGui& gui) {
		try {
			tgui::Theme theme;
			theme.load("res/ui/settings_menu.txt");

			writers.useParticleEffectsCheckBox = tgui::CheckBox::create("Use particle effects");
			writers.useParticleEffectsCheckBox->setChecked(GraphicsSettings::particleEffects);

			writers.savePathtextArea = tgui::TextArea::create();
			writers.savePathtextArea->setText(StorageSettings::save_path);
			writers.savePathtextArea->setPosition(0, 50);

			writers.jsonWriterGraphics.open("json/graphicssettings.json");
			writers.jsonWriterStorage.open("json/storagesettings.json");
			writers.jsonWriterWorld.open("json/worldconfig.json");


			auto exit = tgui::Button::create("Back to menu");
			exit->onPress(backToMenu, "", std::ref(gui));
			exit->setPosition(tgui::Layout2d(500, 500));
			exit->setRenderer(theme.getRenderer("Button"));

			
			gui.add(writers.useParticleEffectsCheckBox);
			gui.add(writers.savePathtextArea);
			gui.add(exit);
		} catch (std::exception& ex) {
			prnerr("Failed loading settings gui with code : ", ex.what());
			return false;
		}
		
		world.load(gui, gui.getView().getSize().x, gui.getView().getSize().y);

		return true;
	}

    void update() {
		world.update();
	}

	void input(sf::Event &ev) {
		world.input(ev);
	}

	void draw(sf::RenderWindow& window, tgui::BackendGui& gui) {
		window.clear(Palette::PaletteUI::Black);

		world.render(window, gui);

    	gui.draw();
	}

	void statexit() {
		writers.writeGraphics();
		writers.writeStorage();

		writers.closeAll();
	}
private:
	Writers writers;
	SettingsTestWorld world;

private:

	static void backToMenu(const tgui::String path, tgui::BackendGui& gui) {
		// Reload
		MainState::currentState->statexit();

		SettingsLoader::loadSettings();

		gui.removeAllWidgets();

		MainState::currentState = MainState::menuState;

		MainState::currentState->load(path.toStdString(), gui); 
	}

};