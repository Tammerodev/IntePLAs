#pragma once

#include <SFML/Graphics.hpp>
#include "Voxel.hpp"
#include "Chunk.hpp"

class VoxelContainer {
    public: 
        // Initialization
        virtual void init() = 0;

        // Manipulation
        virtual Voxel& getVoxelAt(const int x, const int y) = 0;
        virtual void heatVoxelAt(const int x, const int y, const int a) = 0;
        virtual const sf::Color getImagePixelAt(const int x, const int y) = 0;
        virtual void setImagePixelAt(const int x, const int y, const sf::Color& new_color) = 0;
        virtual void update() = 0;

        // Bounding
        virtual void boundVector(sf::Vector2i &v) = 0;
        virtual const sf::Vector2i getBoundedVector(const sf::Vector2i &p) = 0;
        virtual void boundHeatVoxelAt(const int x, const int y, const int a) = 0;

        virtual Chunk& boundGetChunkAt(const int x, const int y) = 0;

        // Bounded manipulation
        virtual Voxel& boundGetVoxelAt(const int x, const int y) = 0;
        //virtual void boundHeatVoxelAt(const int x, const int y) = 0;
        //virtual const sf::Color& boundGetImagePixelAt(const int x, const int y) = 0;
        virtual void boundSetImagePixelAt(const int x, const int y, const sf::Color& new_color) = 0;

        // Utils
        virtual void clearVoxelAt(const int x, const int y) = 0;
        virtual void damageVoxelAt(const int x, const int y) = 0;

        // Chunking (All child classes might not use these)
        // getChunkAt returns nullptr if not implemented
        virtual Chunk& getChunkAt(const int x, const int y) = 0;
        virtual Chunk& getChunkAt(const sf::Vector2i &pos) = 0;
        virtual const sf::Vector2i getChunkFromPos(const int x, const int y) = 0;
    private:
};