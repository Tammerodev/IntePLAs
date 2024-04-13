#pragma once
#include "Rigidbody.hpp"
#include <SFML/Graphics.hpp>

class RectangleRigidbody : public RigidBody {
    public:
        void setSpritePoints(sf::Sprite& sprite) {
            sprite.setPosition(points.at(1).transform_position);
        }

        void setPoints0(sf::Vector2f sp, const sf::FloatRect& rect) {
            const sf::Vector2f point0 = sp + sf::Vector2f(0, 0);
            const sf::Vector2f point1 = sp + sf::Vector2f(0, -rect.height);
            const sf::Vector2f point2 = sp + sf::Vector2f(rect.width, -rect.height);
            const sf::Vector2f point3 = sp + sf::Vector2f(rect.width, 0);

            points.at(0).transform_position = point0;
            points.at(1).transform_position = point1;
            points.at(2).transform_position = point2;
            points.at(3).transform_position = point3;
        }

        void setPoints1(sf::Vector2f sp, const sf::FloatRect& rect) {
            const sf::Vector2f point0 = sp + sf::Vector2f(0, rect.height);
            const sf::Vector2f point1 = sp + sf::Vector2f(0, 0);
            const sf::Vector2f point2 = sp + sf::Vector2f(rect.width, 0);
            const sf::Vector2f point3 = sp + sf::Vector2f(rect.width, rect.height);

            points.at(0).transform_position = point0;
            points.at(1).transform_position = point1;
            points.at(2).transform_position = point2;
            points.at(3).transform_position = point3;
        }

        void setPoints2(sf::Vector2f sp, const sf::FloatRect& rect) {
            const sf::Vector2f point0 = sp + sf::Vector2f(-rect.width, rect.height);
            const sf::Vector2f point1 = sp + sf::Vector2f(-rect.width, 0);
            const sf::Vector2f point2 = sp + sf::Vector2f(0, 0);
            const sf::Vector2f point3 = sp + sf::Vector2f(0, rect.height);

            points.at(0).transform_position = point0;
            points.at(1).transform_position = point1;
            points.at(2).transform_position = point2;
            points.at(3).transform_position = point3;
        }

        void setPoints3(sf::Vector2f sp, const sf::FloatRect& rect) {
            const sf::Vector2f point0 = sp + sf::Vector2f(-rect.width, 0);
            const sf::Vector2f point1 = sp + sf::Vector2f(-rect.width, -rect.height);
            const sf::Vector2f point2 = sp + sf::Vector2f(0, -rect.height);
            const sf::Vector2f point3 = sp + sf::Vector2f(0, 0);

            points.at(0).transform_position = point0;
            points.at(1).transform_position = point1;
            points.at(2).transform_position = point2;
            points.at(3).transform_position = point3;
        }

        void loadFromRectangle(const sf::FloatRect& rect) {
            const sf::Vector2f sp = {rect.left, rect.top + rect.height};

            points.emplace_back(PhysicsComponent());
            points.emplace_back(PhysicsComponent());
            points.emplace_back(PhysicsComponent());
            points.emplace_back(PhysicsComponent());

            setPoints0(sp, rect);

            rect_ = rect;
        }

        // Dont include 'dip' point
        void reCalulatePoints(std::vector<int> dipArray) {
            if(dipArray.size() <= 0) return;

            for(auto &dip : dipArray) {
                if(dip == 0) 
                    setPoints0(points.at(0).transform_position, rect_);

                if(dip == 1)
                    setPoints1(points.at(1).transform_position, rect_);

                if(dip == 2)
                    setPoints2(points.at(2).transform_position, rect_);

                if(dip == 3)
                    setPoints3(points.at(3).transform_position, rect_);
            }

            setVelocityInAllPoints();
        }

        void setVelocityInAllPoints(const sf::Vector2f& vel = {0.f, 0.f}) {
            points.at(0).velocity = vel;
            points.at(1).velocity = vel;
            points.at(2).velocity = vel;
            points.at(3).velocity = vel;
        }

        void setRect(const sf::FloatRect& rect) {
            rect_ = rect;
        }

        void calculatePoints(ChunkIndexer& world) {
            int index = 0;
            std::vector<int> dipPoints {};

            for(auto &point : points) {
                const int collision = world.getPixelCollision(point.transform_position).first;
                
                if(collision) { 
                    point.velocity.y--;
                    point.velocity.y = 0.f;

                    dipPoints.emplace_back(index);
                } else {
                    point.update(1.f);
                }

                reCalulatePoints(dipPoints);

                loginf(point.transform_position.x, "<x-y>", point.transform_position.y);

                ++index;
            }  
        }

    private:

        sf::FloatRect rect_;
};