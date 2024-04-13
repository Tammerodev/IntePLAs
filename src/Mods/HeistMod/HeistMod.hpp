#pragma once 
#include "../CustomGameMod.hpp"
#include "Helicopter.hpp"
#include "HelicopterTurret.hpp"
#include "../../Sound/Sound.hpp"
#include "HeistInfoGui.hpp"
#include "CollectableBox.hpp"
#include "../../UI/PointIndicator.hpp"

class HeistMod : public CustomGameMod {
    public:
        HeistMod() {

        }

        const std::string getModInfo() {
            std::ifstream t("info.txt");
            std::stringstream buffer;
            buffer << t.rdbuf();

            t.close();

            return buffer.str();
        }

        const std::string getModName() {
            return "[OFFICIAL] Heist mod";
        }

        void load(Game& game, tgui::BackendGui& gui) {
            helicopter.load();
            turret.load();
            machineGun.load();
            infoGui.load(gui);

            helicopter_sound.load("res/mod/sound/helicopter.wav");
            helicopter_sound.setLooping(true);
            helicopter_sound.play();

            collectBox.load();
            pointToBox.load(gui);

            for(int i = 0; i < 10; i++)  {
                sf::Vector2i position;
                position.x = math::randIntInRange(0, worldSize::world_sx - 1);
                position.y = 2048 - game.world.main_world.procGen.getHeightOnMap(position.x);

                if(game.world.main_world.procGen.getBiomeAtPosition(position.x, game.world.main_world.getChunkIndexer()).getName() != "Ocean") {
                    sf::Image image;
                    image.loadFromFile("res/mod/img/house.png");

                    sf::Vector2u size = image.getSize();
                    position -= sf::Vector2i(0, 64);

                    positions.push_back(position);
                    game.world.main_world.build_image(position, image, nullptr);
                }
            }

           chooseNewPos();
        }

        void update(Game& game) {
            helicopter.update(1.0f, game, game.player.getHeadPosition());

            turret.update(1.0f, game.world);
            machineGun.update(1.0f, game.world);

            infoGui.update(collected);
            pointToBox.update(game.player.getHeadPosition(), collectBox.sprite.getPosition());

            if(collectBox.update(game.player.getHeadPosition())) {
                chooseNewPos();
            }

            helicopter_sound.setVolume(std::clamp(500.f - math::distance(helicopter.getPosition(), game.player.getHeadPosition()),  5.f, 100.f));

            timer++;
            timer2++;

            if(timer > 300) {
                timer = 0;
                turret.use(game.player, helicopter.getTurretPosition(), Controls::worldCursorPos, game.world);
            }

            if(helicopter.ammo <= 0) {
                if(timer2 > 500) {
                    helicopter.ammo = helicopter.max_ammo;
                }
            
                return;
            }

            if(timer2 > 3) {
                timer2 = 0;
                machineGun.use(game.player, helicopter.getTurretPosition(), Controls::worldCursorPos, game.world);
                helicopter.ammo--;
            }
        }

        void render(sf::RenderTarget& target, Game& game, tgui::BackendGui& gui) {
            helicopter.render(target);
            turret.render(target);
            machineGun.render(target);

            collectBox.render(target);
            pointToBox.render(target);
        }

        void input(sf::Event &ev) {

        }

    private:
        void chooseNewPos() {
            if(positions.size() > 0) {
                collectBox.sprite.setPosition(sf::Vector2f(positions.at(math::randIntInRange(0, positions.size() - 1))));
                collected++;
            }
        }

    private:
        std::vector<sf::Vector2i> positions;
        CollectableBox collectBox;

        Sound helicopter_sound;
        int collected = 0;

        int timer = 0;
        int timer2 = 0;

        HelicopterTurret turret;
        HelicopterMachineGun machineGun;
        Helicopter helicopter;
        HeistInfoGui infoGui;

        PointIndicator pointToBox;
};