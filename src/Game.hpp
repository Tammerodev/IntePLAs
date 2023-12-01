
#include "UIStateManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "SoundFX.hpp"
#include "Player.hpp"
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

class Game {
    public:
        const std::string load(const std::string s, tgui::BackendGui& gui, const int width, const int height);
        void update();

        void render(sf::RenderWindow& window, tgui::BackendGui& gui);

        void input(sf::Event &ev);

        void exit() {
            BGMusic::stop();
            world.save();
        }

        const bool getLoaded() const {
            return hasLoaded;
        }

    private:
        bool GUIfocusedOnObject = false;

        bool slowmo = false;
        bool hasLoaded = false;
        sf::Clock clock;


        float lastTime = 0;
        float fps = 1;

        Player player;
        Inventory inv;
        MaterialsUI matUI;
        Background bg;
        EffectOverlay effOverlay;
        UIStateManager uiStateManager;

        Cursor cursor;

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
};