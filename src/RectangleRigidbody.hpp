#include "Rigidbody.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

constexpr float GRAVITY = 0.01f; // Gravity acceleration
constexpr float ROTATE_SPEED_INCREMENT = 0.01f;
constexpr float EPSILON = 0.0001f;
constexpr float DAMPING = 0.8f;



struct Constraint {
    size_t indexA;
    size_t indexB;
    float restLength;
};

class RectangleRigidbody : public RigidBody {
public:
    // Other methods...
    void setSpritePoints(sf::Sprite& sprite) {
        sprite.setPosition(points.at(1).transform_position);
    }

    void loadFromRectangle(const sf::FloatRect& rect) {
        const sf::Vector2f sp = {rect.left, rect.top + rect.height};

        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());
        points.emplace_back(PhysicsComponent());

        setPoints0(sp, rect);

        generateConstraints();

        cog.transform_position = calculateCenterOfGravity();

        rect_ = rect;
    }

    void generateConstraints() {
        constraints.clear();

        for (size_t i = 0; i < 4; ++i) {
            size_t nextIndex = (i + 1) % 4;

            // Calculate the rest length between adjacent points
            float restLength = vectorLength(points[nextIndex].transform_position - points[i].transform_position);

            // Add constraint between current point and the next point
            constraints.push_back({i, nextIndex, restLength});
        }

        // Define constraints for diagonal connections (optional)
        // Connect opposite corners to maintain rectangle shape
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
            if (collision.first) {
                point.transform_position -= point.velocity;
                point.transform_position -= collision.second;

                point.velocity *= -0.1f;

                cog.transform_position += (point.transform_position - cog.transform_position) / 2.0f;
            }
        }
    }

    // Apply gravity to all points
    void applyGravityToPoints(float dt) {
        for (auto& point : points) {
            point.velocity.y += GRAVITY * dt;
        }
    }

    // Rotate points around center of gravity
    void rotatePoints(float dt) {
        sf::Vector2f centerOfGravity = calculateCenterOfGravity();
        cog.transform_position += (centerOfGravity - cog.transform_position) / 2.f;
    }

    // Update positions of points
    void updatePoints(float dt) {
        for (auto& point : points) {
            point.transform_position += point.velocity * dt;
        }
    }

    std::vector<Constraint> constraints;

    // Calculate the length of a vector
    float vectorLength(const sf::Vector2f& vec) const {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    void satisfyConstraints() {
        for (size_t i = 0; i < constraints.size(); ++i) {
            auto& constraint = constraints[i];

            // Calculate delta vector
            const sf::Vector2f delta = points[constraint.indexB].transform_position - points[constraint.indexA].transform_position;

            // Calculate current distance between points
            const float currentDistance = vectorLength(delta);

            // Calculate error (difference between current distance and rest length)
            const float error = currentDistance - constraint.restLength;
            
            // Calculate correction vector
            sf::Vector2f correction = (error / currentDistance) * 0.5f * delta;

            // Apply correction to points
            points[constraint.indexA].transform_position += correction;
            points[constraint.indexB].transform_position -= correction;
        }
    }



    // Update simulation for a given time step
    void update(float dt, ChunkIndexer& world) {
        const int substeps = 100;
        applyGravityToPoints(dt);

        for(int i = 0; i < substeps; ++i) {
            float subDt = dt / (float)substeps;

            satisfyConstraints();

            handleCollisions(subDt, world);
            rotatePoints(subDt);
            updatePoints(subDt);
        }
    }
    
    void setVelocityInAllPoints(const sf::Vector2f& vel = {0.f, 0.f}) {
        points.at(0).velocity = vel;
        points.at(1).velocity = vel;
        points.at(2).velocity = vel;
        points.at(3).velocity = vel;
    }

    void dampVelocityInAllPoints(const float damping) {
        points.at(0).velocity /= damping;
        points.at(1).velocity /= damping;
        points.at(2).velocity /= damping;
        points.at(3).velocity /= damping;
    }

    void setRect(const sf::FloatRect& rect) {
        rect_ = rect;
    }

private:
    sf::FloatRect rect_;

    // Calculate center of gravity based on current points
    sf::Vector2f calculateCenterOfGravity() {
        sf::Vector2f centerOfGravity = {0.f, 0.f};

        for (const auto& point : points) {
            centerOfGravity += point.transform_position;
        }

        return centerOfGravity / static_cast<float>(points.size());
    }

    // Rotate a point around a given center by a specified angle
    void rotatePointAroundPoint(PhysicsComponent& p, const PhysicsComponent& center, float angle) {
        float s = std::sin(angle);
        float c = std::cos(angle);

        // Translate point back to origin
        p.transform_position -= center.transform_position;

        // Rotate point
        float x_new = p.transform_position.x * c - p.transform_position.y * s;
        float y_new = p.transform_position.x * s + p.transform_position.y * c;

        // Translate point back
        p.transform_position = {x_new + center.transform_position.x, y_new + center.transform_position.y};
    }
};
