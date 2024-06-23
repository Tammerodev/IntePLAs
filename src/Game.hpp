
#include "UIStateManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "SoundFX.hpp"
#include "Player/Player.hpp"
#include "World.hpp"
#include "math.hpp"
#include "Background.hpp"
#include "EffectOverlay.hpp"
#include "Camera.hpp"
#include "BackgroundMusic.hpp"
#include "PlayerUI.hpp"
#include "Controls.hpp"
#include "Cursor.hpp"
#include "GameStatus.hpp"
#include "Settings.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "JsonManager.hpp"
#include "ShaderEffects.hpp"

#include "WeatherManager.hpp"
#include "SimulationManager.hpp"

#include "DebugDisplay.hpp"
#include "GameEventManager.hpp"
#include "Globals.hpp"

class Game {
    friend class CustomGameMod;

    public:
        const std::string load(const std::string s, tgui::BackendGui& gui, const int width, const int height);
        void update();

        void renderFirst(sf::RenderWindow& window, tgui::BackendGui& gui);
        void renderLast(sf::RenderWindow& window, tgui::BackendGui& gui);

        void renderUI(sf::RenderWindow& window, tgui::BackendGui& gui);

        void input(sf::Event &ev);

        void saveWorld() {
            world.save();
        }

        sf::RenderTexture& getRenderTexture() {
            return renderTexture;
        }

        void exit() {
            prndd("Exiting the game...");
            goingToSave = true;

            if(UIState::currentState != UIState::saveExit) {
                UIState::currentState = UIState::saveExit;
                UIState::currentState->load(*local_gui, inv, world.main_world);
            }

            if(!Globals::exitSaveName.empty()) {
                goingToSave = false;
                MainState::currentState->statexit();               

                prndd("Saving complete!");            
            }
        }

        const bool getLoaded() const {
            return hasLoaded;
        }

        const bool getSaved() const {
            return hasSaved;
        }

        void damageMobs(sf::Vector2f pos, int hp) {
            
        }

    public:

        bool slowmo = false;
        bool hasLoaded = false;
        bool hasSaved = false;

    private:
        tgui::BackendGui* local_gui = nullptr;

    public:
        bool GUIfocusedOnObject = false;

        bool goingToSave = false;

        sf::Clock clock;


        float lastTime = 0;
        float fps = 1;

        Player player;
        Inventory inv;
        Background bg;
        EffectOverlay effOverlay;
        UIStateManager uiStateManager;
        
        World world;

        Camera game_camera;
        Camera ui_camera;

        sf::RenderTexture renderTexture;
        sf::Sprite renderSprite;

        ShaderEffect shaderEffect;
        Shader shader_ambient;
        
        float dt = 0.f;
        sf::Clock deltaClock;

        sf::Clock shader_time;

        PlayerUI playerUI;

        DebugDisplay debugDisplay;

        GameEventManager gameEventManager;
};