#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

namespace Controls {

    static sf::Keyboard::Key keybind_right = sf::Keyboard::D;
    static sf::Keyboard::Key keybind_left = sf::Keyboard::A;
    static sf::Keyboard::Key keybind_jump = sf::Keyboard::Space;
    static sf::Keyboard::Key keybind_down = sf::Keyboard::S;

    static sf::Keyboard::Key keybind_zoomin = sf::Keyboard::Up;
    static sf::Keyboard::Key keybind_zoomout = sf::Keyboard::Down;

    static sf::Keyboard::Key keybind_openItemCreator = sf::Keyboard::E;

    static sf::Mouse::Button mousebind_useitem = sf::Mouse::Left;
    static sf::Mouse::Button mousebind_switchitem = sf::Mouse::Middle;

    static uint16_t joystick_bind_jump = 0;
    static uint16_t joystick_bind_switchitem = 1;

    

    static float joystick_treshold = 50; 

    static enum class ControllerType {
        Keyboard, Joystick
    } ctrl;

    inline ControllerType currentController = ControllerType::Keyboard;
    inline sf::Vector2f windowCursorPos = sf::Vector2f(0, 0);
    inline sf::Vector2f worldCursorPos = sf::Vector2f(0, 0);
    inline sf::Vector2f gameCameraCenterPos = sf::Vector2f(0, 0);
    
    static void searchForDevices() {
        if(sf::Joystick::isConnected(0))
            currentController = ControllerType::Joystick;
        else 
            currentController = ControllerType::Keyboard;
    }

    static void setWindowMouseposition(sf::Event& ev) {
        if(currentController == ControllerType::Keyboard) {
            if(ev.type == sf::Event::MouseMoved) {
                windowCursorPos.x = ev.mouseMove.x;
                windowCursorPos.y = ev.mouseMove.y;
            }
        }
    }

    static void setWorldMouseposition(sf::RenderTarget &target) {
        if(currentController == ControllerType::Joystick) {
            windowCursorPos.x += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U) / 10.0;
            windowCursorPos.y += sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V) / 10.0;
        }
        
        worldCursorPos = target.mapPixelToCoords(sf::Vector2i(windowCursorPos));
    }

    static bool moveRight() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_right);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > joystick_treshold;
        return false;
    }

    static bool moveLeft() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_left);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -joystick_treshold;
        return false;
    }

    static bool jump() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_jump);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::isButtonPressed(0, joystick_bind_jump);
        return false;
    }

    static bool moveDown() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_down);
        else if(currentController == ControllerType::Joystick)
            throw "Not implemented";
        return false;
    }

    static bool useItem(sf::Event &event) {
        if(currentController == ControllerType::Keyboard) 
            return event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == mousebind_useitem;
        else if(currentController == ControllerType::Joystick)
            return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == sf::Joystick::Axis::R;
        return false;
    }

    static bool useUI() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Mouse::isButtonPressed(sf::Mouse::Button(mousebind_useitem));
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::isButtonPressed(0, joystick_bind_jump);
        return false;
    }

    static bool openItemCreator() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_openItemCreator);
        else if(currentController == ControllerType::Joystick)
            throw "Not implemented";
        return false;
    }

    static bool switchItem(sf::Event &event) {
        if(currentController == ControllerType::Keyboard) 
            return event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(mousebind_switchitem);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::isButtonPressed(0, joystick_bind_switchitem);
        return false;
    }

    static bool zoomin() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_zoomin);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) <= -1;
        return false;
    }

    static bool zoomout() {
        if(currentController == ControllerType::Keyboard) 
            return sf::Keyboard::isKeyPressed(keybind_zoomout);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) >= 1;
        return false;
    }


    static sf::Vector2f cursorPos;


    static const sf::Vector2f &getCursorPosition() {
        return cursorPos;
    }

    static void setCursorPosition(const sf::Vector2f& newPos) {
        cursorPos = newPos;
    }
}