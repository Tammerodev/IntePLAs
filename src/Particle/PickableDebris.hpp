#pragma once
#include "Particle.hpp"
#include "../math.hpp"
#include "../PhysicsComponent.hpp"
#include "../ChunkIndexerVoxelContainer.hpp"
#include "../CollisionManager.hpp"
#include "../SoundFX.hpp"

class PickableDebris : public Particle {
    public:
        PickableDebris() {
            x = 0.f;
            y = 0.f;

            energy = 2;

        }

        PickableDebris(const sf::Vector2f& pos, const sf::Vector2f &vel, const sf::Color &col, const int vox_value, ChunkIndexer* indexer) {
            value = vox_value;
            physComponent.transform_position = pos;
            this->x = pos.x;
            this->y = pos.y;

            world = indexer;
            physComponent.velocity = vel;

            rect.setSize(sf::Vector2f(2.0f, 2.0f));
            rect.setFillColor(col);


            rect.setRotation(math::randIntInRange(0, 360));

            energy = 2;
        }

        void update(const float dt, const sf::Vector2f &pos) {
            timer++;

            rect.setPosition(physComponent.transform_position);

            physComponent.update(1.0f);

            rect.setRotation(
                physComponent.transform_rotation
                );

            this->x = physComponent.transform_position.x;
            this->y = physComponent.transform_position.y;

            int a = rect.getFillColor().a - 1;
            if(a <= 0) a = 0;

            physComponent.transform_position.y -= 10.f;

            CollisionManager::CollisionState collision = CollisionManager::handleCollisionsWith(physComponent, *world, sf::Vector2u(rect.getSize()));

            physComponent.transform_position.y += 10.f;

            if(collision.hasCollision) {
                physComponent.velocity.x /= 2.0;
            }
            
            if(abs(pos.x - physComponent.transform_position.x) > 50) return;
            if(abs(pos.y - physComponent.transform_position.y) > 50) return;

            float dist = math::distance(rect.getPosition(), pos);
            if(dist < 50.f && world != nullptr) {
                if(dist < 10.f) {
                    //world->materialpack.addElementOfType(value, 1);
                    remove_ = true;

                    SFX::collect.play();
                    SFX::collect.setPitch(0.5f + math::randFloat() / 2.f);
                }

                // Set outline to inverse color
                rect.setOutlineColor(sf::Color(255 - rect.getFillColor().r, 255 - rect.getFillColor().g, 255 - rect.getFillColor().b));
                rect.setOutlineThickness(0.5f);

                rect.move((pos - rect.getPosition()) / 20.f);
                physComponent.velocity = sf::Vector2f(0.f, 0.f);
                physComponent.transform_position = rect.getPosition();
            } else {
                rect.setOutlineThickness(0.f);
            }

        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            bool rem = timer > lifetime;

            if(remove_) rem = true;
            return rem;
        }

        void collide() {
            physComponent.velocity.y = -physComponent.velocity.y;

            int r = math::randIntInRange(0, 1);
            if(r == 0) {
                physComponent.velocity.x = physComponent.velocity.x;
            } else {
                physComponent.velocity.x = -physComponent.velocity.x;
            }

            physComponent.velocity /= 1.5f;

            energy--;
        }

        ParticleType getType() {
            return ParticleType::PickableDebris;
        }

        virtual sf::Color getColor() {
            return rect.getFillColor();
        }

    private:
        bool remove_ = false; 
        int bounces = 0;

        sf::RectangleShape rect;
        PhysicsComponent physComponent;

        unsigned int timer = 0;
        unsigned int lifetime = 1000;

        int value = 0;

        ChunkIndexer* world;
};