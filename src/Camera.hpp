#pragma once 
#include <SFML/Graphics.hpp>

enum CameraMode {
    Static, Leap
};

class Camera {
public:
    Camera() = default;
    Camera(const CameraMode mode) {
        setCameraMode(mode);
    }

    void setCameraMode(const CameraMode mode) {
        this->mode = mode;
    }

    void shake(const float intensity) {
        shakeTimer = 0;
        shakeTime = intensity / 10.f;
        shakeIntensity = intensity / 100.f;
    }

    void setLeapTarget(const sf::Vector2f& target) {
        this->target = target;
    }

    void zoom(const float zoom) {
        view.zoom(zoom);
        currZoom = zoom;
    }

    void move(float x, float y) {
        view.move(x, y);
    }

    const float getZoom() const {
        return currZoom;
    }

    sf::View &getView() {
        return view;
    }

    void update(const float dt) {
        if(mode == CameraMode::Leap) {
            sf::Vector2f movement = target - view.getCenter();
            view.move(movement / 10.0f * dt);
        }
        
        if(shakeIntensity != 0.f) {
            if(shakeTimer <= shakeTime) {
                view.move(math::randFloat() * shakeIntensity, math::randFloat() * shakeIntensity);
                shakeTimer++;
            } else {
                shakeIntensity = 0.f;
                shakeTime = 0.f;
                shakeTimer = 0.f;
            }
        }
    }

    void setSize(sf::Vector2u size) {
        view.setSize(size.x, size.y);
    }

    void setViewTo(sf::RenderTarget& target) {
        target.setView(view);
    }

    const sf::Vector2f& getCenterPosition() const {
        return view.getCenter();
    }

    void setCenterPosition(const sf::Vector2f& pos) {
        view.setCenter(pos);
    }

    void setTarget(const sf::Vector2f &target) {
        this->target = target;
    }

    void setLeapSpeed(const float leap_speed) {
        this->leap_speed = leap_speed;
    }

    void setZoomLimited(bool lim) {
        limitZoom = lim;
    }

private:

    float currZoom = 1.00f;
    float zoomMax = 0.95;
    float zoomMin = 1.05;
    bool limitZoom = false;

    float shakeTime = 0;
    float shakeTimer = 0;
    float shakeIntensity = 0;

    sf::View view;
    CameraMode mode = CameraMode::Static;
    sf::Vector2f target {100,100};
    float leap_speed = 1.0f;
};