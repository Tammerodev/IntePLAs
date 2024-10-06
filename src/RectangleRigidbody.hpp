#include "Rigidbody.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include "math.hpp"
#include "Graphics/VertexSprite.hpp"
#include "Settings.hpp"

constexpr float GRAVITY = 0.01f; // Gravity acceleration
constexpr float ROTATE_SPEED_INCREMENT = 0.01f;
constexpr float EPSILON = 0.0001f;
constexpr float DAMPING = 0.8f;
constexpr float MAX_VELOCITY = 1.5f;

class RectangleRigidbody : public RigidBody {
public:
    // Other methods...
    void setVertexSpritePoints(VertexSprite& sprite) {
        int ind = 0;

        for(auto &p : points) {
            sprite.getVertex(ind).position = p.transform_position;
            ++ind;
        }
    }

    void loadFromRectangle(const sf::FloatRect& rect) {
        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());

        setPoints0({rect.left, rect.top + rect.height}, rect);

        generateConstraints();

        cog.transform_position = calculateCenterOfGravity();
    }

    void generateConstraints() {
        constraints.clear();

        for (uint8_t i = 0; i < 4; ++i) {
            uint8_t nextIndex = (i + 1) % 4;

            float restLength = vectorLength(points[nextIndex].transform_position - points[i].transform_position);

            constraints.push_back({i, nextIndex, restLength});
        }

        // Diagonal constraints (Makes it more stable)
        constraints.push_back({0, 2, vectorLength(points[2].transform_position - points[0].transform_position)});
        constraints.push_back({1, 3, vectorLength(points[3].transform_position - points[1].transform_position)});
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

    // Handle collisions and update velocities
    void handleCollisions(float dt, ChunkIndexer& world) {
        for (auto& point : points) {
                                                                                // Predict collision
            const auto collision = world.getPixelCollision(point.transform_position + point.velocity * dt);

            if(collision.first) 
                cog.transform_position += (point.transform_position - cog.transform_position) / 2.0f;       // Move center of gravity

            if (collision.first == ChunkIndexer::CollisionType::Yes) {
                point.transform_position.y -= collision.second.y;
                point.transform_position.y -= GRAVITY;

                point.transform_position.x -= point.transform_position.x - 0.5f - sf::Vector2i(point.transform_position + point.velocity * dt).x;
                
                if(point.velocity.y > 0.f) {
                    point.velocity *= -0.3f;
                    point.transform_position -= point.velocity * dt;
                } else {
                    point.transform_position += point.velocity * dt;
                }
            } else if(collision.first == ChunkIndexer::CollisionType::Fluid) {
                point.velocity /= 1.5f;
            }
            
        }
    }

    void applyGravityToPoints(float dt) {
        for (auto& point : points) {
            point.update(dt, GRAVITY);

            if(point.velocity.y > MAX_VELOCITY) {
                point.velocity.y = MAX_VELOCITY;
            }
        }
    }

    void rotatePoints(float dt) {
        sf::Vector2f centerOfGravity = calculateCenterOfGravity();
        cog.transform_position += (centerOfGravity - cog.transform_position) / 2.f;
    }

    void updatePoints(float dt) {
        for (auto& point : points) {
            point.transform_position += point.velocity * dt;
        }
    }

    float vectorLength(const sf::Vector2f& vec) const {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    void satisfyConstraints() {
        for (size_t i = 0; i < constraints.size(); ++i) {
            auto& constraint = constraints[i];

            const sf::Vector2f delta = points[constraint.indexB].transform_position - points[constraint.indexA].transform_position;

            const float currentDistance = vectorLength(delta);

            const float error = currentDistance - constraint.restLength;        // Difference between current and target distance

            sf::Vector2f correction = (error / currentDistance) * 0.5f * delta;

            points[constraint.indexA].transform_position += correction;
            points[constraint.indexB].transform_position -= correction;
        }
    }

    // Update simulation for a given time step
    void update(float dt, ChunkIndexer& world) {
        applyGravityToPoints(dt);

        for(int i = 0; i < PhysicsSettings::substeps; ++i) {
            float subDt = dt / (float)PhysicsSettings::substeps;
            satisfyConstraints();

            handleCollisions(subDt, world);
            rotatePoints(subDt);
            updatePoints(subDt);
        }
    }

private:
    sf::Vector2f calculateCenterOfGravity() {
        sf::Vector2f centerOfGravity = {0.f, 0.f};

        for (const auto& point : points) {
            centerOfGravity += point.transform_position;
        }

        return centerOfGravity / (float)points.size();
    }

};
