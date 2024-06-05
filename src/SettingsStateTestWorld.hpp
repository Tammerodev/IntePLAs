#pragma once
#include "GameEvent.hpp"

class SettingsTestWorld {
    public:
        void load(tgui::BackendGui &gui, int width, int height) {
            world.init("_TEST", &player.getPhysicsComponent().transform_position);
            player.load();
            player.getPhysicsComponent().transform_position = {50, 0};

            camera.setCameraMode(CameraMode::Leap);
            camera.setLeapSpeed(1.f);
            camera.setSize(sf::Vector2u(width, height));
            camera.zoom(0.5);

            bg.load();
            visualEffects.load(width, height);
            renderTexture.create(width, height);
        }

        void render(sf::RenderWindow& window, tgui::BackendGui &gui) {
            renderTexture.clear();

            camera.setViewTo(renderTexture);
            camera.setTarget(player.getHeadPosition());
            camera.update(1.0f);

            bg.render(renderTexture);
            world.render(renderTexture, renderTexture, camera.getCenterPosition());
            player.draw(renderTexture);

            renderTexture.display();
            renderSprite.setTexture(renderTexture.getTexture());
            
            visualEffects.render(window, renderSprite);
            visualEffects.finalRender(window);
        }

        void input(sf::Event &ev) {
            if(ev.type == sf::Event::Resized) {
                camera.setSize(sf::Vector2u(ev.size.width, ev.size.height));
            }
        }

        void update() {
            player.update(world.main_world.getChunkIndexer(), 1.0f, camera);

            world.update(1.0f, player, gameEvent, visualEffects);
            world.handleCollisionsWithPlayer(player);

            bg.update(camera.getView(), dt);
        }
    private:
        Player player;
        World world;
        Camera camera;
        ShaderEffect visualEffects;
        Background bg;

        float dt = 1.f;

        sf::RenderTexture renderTexture;
        sf::Sprite renderSprite;

        GameEventEnum::Event gameEvent = GameEventEnum::Event::Default;
};