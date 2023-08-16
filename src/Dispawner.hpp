#pragma once
#include <SFML/Graphics.hpp>

class VoxelObject {
    public:
        virtual std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider) = 0;
        virtual std::pair<bool, sf::FloatRect> getOvelapWithRectY(const sf::FloatRect &collider) = 0;

        virtual int load(const sf::Image &copy_img) = 0;
        virtual Voxel &getVoxelAt(const uint64_t x, const uint64_t y) = 0;
        //virtual void destroyPart(T& main_world, sf::FloatRect destroyArea) = 0;
    private:
};

class DispawnableVoxelObject : public VoxelObject {
    public:
        virtual void dispawn() = 0;
        
        bool getDestroyed() {
            return destroyed;
        }

        void setDestroyed(const bool val) {
            destroyed = val;
        }

    protected:
        bool destroyed = false;
};

class Dispawner {
    public:
        void update(const sf::Vector2f player, const sf::Vector2f& pos) {
            static long DESTROY_DIST = 100;
            
            // TODO abs(player.x - pos.x) > DESTROY_DIST || abs(player.y - pos.y) > DESTROY_DIST;
        }
    private:
};