#pragma once
#include "../Entity.hpp"
#include "../VoxelManager.hpp"
#include "../MobInfoBar.hpp"
#include "../Elements/Blood.hpp"
#include "../MobType.hpp"
#include "../AddVoxelGroups.hpp"
#include "../VoxelGroupPropeties/MeatPropeties.hpp"

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

        void update(const sf::Vector2f& pos, const std::string& name, int health, VoxelManager& vx, VoxelGroupPropeties& propeties) {
            mobInfoBar.setPosition(pos + sf::Vector2f(0, -12));
            mobInfoBar.update(name, health);

            if(health <= 0) {
                default_death(pos.x, pos.y, vx, propeties);
            }
            
        }

        void default_death(int x, int y, VoxelManager& voxelManager, VoxelGroupPropeties& propeties) {
            splashBlood(x, y, voxelManager);

            sf::Texture itemTex;
            itemTex.loadFromFile("res/img/Item/meats.png", sf::IntRect(sf::Vector2i(meatTextureIndex * 16, 0), sf::Vector2i(16, 16)));

            auto item = std::make_shared<VoxelGroup>();
            item->load(itemTex.copyToImage());
            item->setPosition(sf::Vector2f(x, y));
            item->setVelocity(sf::Vector2f(abs(math::randFloat()) / 5.f, math::randFloat() / 5.f));
            item->setPropeties(propeties);

            AddVoxelGroups::add_more_worlds.emplace_back(item);

            remove_mob = true;
        }

        bool generalDamageBehaviour(int &health, int damage) {
            health -= damage;

            return health <= 0;
        }
    public:

        int meatTextureIndex = 0;
        bool remove_mob = false;
        
    private:

        void splashBlood(int x, int y, VoxelManager& voxelManager, int amount = 50) {
            for(int i = 0; i < amount; i++) {
                sf::Vector2i pos(x, y - i);
                pos += sf::Vector2i(math::randIntInRange(-5, 5), math::randIntInRange(-5, 5));

                sf::Vector2i vel {0,0};
                vel = sf::Vector2i(math::randIntInRange(-2, 2), math::randIntInRange(-2, 0));

                voxelManager.getChunkIndexer().boundVector(pos);

                voxelManager.getChunkIndexer().getVoxelAt(pos.x, pos.y).value = VoxelValues::BLOOD;
                voxelManager.getChunkIndexer().setImagePixelAt(pos.x, pos.y, elm::getInfoFromType(VoxelValues::BLOOD).color);

                voxelManager.addElement(pos.x, pos.y, std::make_shared<Blood>(pos.x, pos.y, vel));
            }
        }

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
        virtual const sf::FloatRect getHitbox() const {
            return sprite.getGlobalBounds();
        }

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

        unsigned int maxHealth = 0;
        int health = 0;

        int entity_id;

        bool remove_mob = false;
};