#pragma once
#include "GameEvent.hpp"
#include "DefaultGameEvent.hpp"
#include "NuclearExplosionGameEvent.hpp"

namespace GameEventEnum {
    enum Event {
        Default, Nuclear_Explosion
    };
}

class GameEventManager {
    public:
        void load() {
            gameEvents.at(GameEventEnum::Event::Default) = std::make_shared<DefaultGameEvent>();
            gameEvents.at(GameEventEnum::Event::Nuclear_Explosion) = std::make_shared<NuclearExplosionGameEvent>();

        }

        void update(VoxelManager &vx) {
            gameEvents.at(currentEvent)->update(vx);
        }

        void render(sf::RenderTarget& target) {
            gameEvents.at(currentEvent)->render(target);
        }

        void switchEvent(GameEventEnum::Event newEvent) {
            currentEvent = newEvent;
            gameEvents.at(currentEvent)->enter();
        }

    private:
        GameEventEnum::Event currentEvent = GameEventEnum::Event::Default;
        std::array<std::shared_ptr<GameEvent>, 3> gameEvents;
};