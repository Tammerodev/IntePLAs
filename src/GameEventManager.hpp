#pragma once
#include "GameEvent.hpp"
#include "DefaultGameEvent.hpp"
#include "NuclearExplosionGameEvent.hpp"
#include "GameEventEnum.hpp"

class GameEventManager {
    public:
        void load() {
            gameEvents.at(GameEventEnum::Event::Default) = std::make_shared<DefaultGameEvent>();
            gameEvents.at(GameEventEnum::Event::Nuclear_Explosion) = std::make_shared<NuclearExplosionGameEvent>();

        }

        void update(VoxelManager &vx) {
            lastEvent = currentEvent;
            gameEvents.at(currentEvent)->update(vx);
        }

        void render(sf::RenderTarget& target) {
            gameEvents.at(currentEvent)->render(target);

            if(currentEvent != lastEvent) {
                gameEvents.at(currentEvent)->enter();
            }
        }

        void switchEvent(GameEventEnum::Event newEvent) {
            currentEvent = newEvent;
            gameEvents.at(currentEvent)->enter();
        }

        GameEventEnum::Event &getEvent() {
            return currentEvent;
        }

    private:
        GameEventEnum::Event lastEvent;
        GameEventEnum::Event currentEvent = GameEventEnum::Event::Default;
        std::array<std::shared_ptr<GameEvent>, 3> gameEvents;
};