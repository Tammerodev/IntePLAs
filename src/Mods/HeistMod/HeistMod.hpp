#pragma once 
#include "../CustomGameMod.hpp"
#include "Helicopter.hpp"
#include "HelicopterTurret.hpp"
#include "../../Sound/Sound.hpp"

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

            helicopter_sound.load("res/mod/sound/helicopter.wav");
            helicopter_sound.setLooping(true);
            helicopter_sound.play();
        }

        void update(Game& game) {
            helicopter.update(1.0f, game.player.getHeadPosition());

            turret.update(1.0f, game.world);
            machineGun.update(1.0f, game.world);

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
        }

        void input(sf::Event &ev) {

        }

    private:
        Sound helicopter_sound;

        int timer = 0;
        int timer2 = 0;

        HelicopterTurret turret;
        HelicopterMachineGun machineGun;
        Helicopter helicopter;
};