#pragma once
#include "Entity.hpp"
#include "VoxelManager.hpp"
#include "MobInfoBar.hpp"
#include "Blood.hpp"
#include "MobType.hpp"

struct MobInvoke {
    float damage = 0.f;
    float distanceToPlayer = 0.f;

    sf::Vector2f playerSubVector = sf::Vector2f(0.f, 0.f);
};

class DefaultBehaviour {
    public:

        void default_load() {
            mobInfoBar.load();
        }

        void default_render(sf::RenderTarget& target) {
            mobInfoBar.render(target);
        }

        void update(const sf::Vector2f& pos, const std::string& name, int health) {
            mobInfoBar.setPosition(pos + sf::Vector2f(0, -12));
            mobInfoBar.update(name, health);
        }

        void default_death(int x, int y, VoxelManager& voxelManager) {
            for(int i = 0; i < 10; i++) {
                sf::Vector2i pos(x, y - i);

                voxelManager.getChunkIndexer().boundVector(pos);

                voxelManager.getChunkIndexer().getVoxelAt(pos.x, pos.y).value = VoxelValues::BLOOD;
                voxelManager.getChunkIndexer().setImagePixelAt(pos.x, pos.y, elm::getInfoFromType(VoxelValues::BLOOD).color);

                voxelManager.getHandleVoxel(elm::getInfoFromType(VoxelValues::BLOOD).color, pos, true);
            }
        }

    private:
        MobInfoBar mobInfoBar;
};

class Mob : public Entity {
    public:
        virtual void load() = 0;
        virtual void update(const float dt) = 0;
        virtual void collisionCheck(VoxelManager&) = 0;
        virtual void render(sf::RenderTarget&) = 0;

        virtual void invoke(const MobInvoke&) = 0;
        virtual bool remove(VoxelManager&) = 0;

        virtual PhysicsComponent& getPhysicsComponent() = 0;

        virtual MobType getMobType() {
            return mobType;
        }

        int getEntityID() {
            return entity_id;
        }

        void setID(const int id) {
            entity_id = id;
        }
 
    protected:

        MobType mobType = MobType::Undefined;

        void generalDamageBehaviour(int damage) {
            health -= damage;

            if(health <= 0) {
                remove_mob = true;
            }
        }

        unsigned int maxHealth = 0;
        int health = 0;

        int entity_id;

        bool remove_mob = false;
};