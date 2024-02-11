#pragma once
#include <SFML/Graphics/Vertex.hpp>

class RadioactiveElement : public sf::Vector2i {
    public:
        RadioactiveElement() {
            age.restart();
        }

        virtual void update(ChunkIndexer &) = 0;
        virtual int getRadiationStrength() = 0;
        virtual bool clear() = 0;

        bool isDecayed() {
            return age.getElapsedTime().asSeconds() > half_life;
        }
        
    protected:

        int half_life = 10000;
        sf::Clock age;
};