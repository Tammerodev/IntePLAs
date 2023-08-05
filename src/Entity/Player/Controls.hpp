#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

namespace Controls {

    static sf::Keyboard::Key keybind_right = sf::Keyboard::D;
    static sf::Keyboard::Key keybind_left = sf::Keyboard::A;
    static sf::Keyboard::Key keybind_jump = sf::Keyboard::Space;
    static sf::Mouse::Button mousebind_useitem = sf::Mouse::Left;
    static sf::Mouse::Button mousebind_switchitem = sf::Mouse::Middle;



    static uint16_t joystick_bind_jump = 0;
    static uint16_t joystick_bind_switchitem = 1;



    static float joystick_treshold = 50; 

    static enum class ControllerType {
        Keyboard, Joystick
    } ctrl;

    static ControllerType currentController = ControllerType::Keyboard;
    
    static void searchForDevices() {
        // TODO
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

    static bool useItem(sf::Event &event) {
        if(currentController == ControllerType::Keyboard) 
            return event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == mousebind_useitem;
        else if(currentController == ControllerType::Joystick)
            return event.type == sf::Event::JoystickMoved && event.joystickMove.axis == sf::Joystick::Axis::R;
        return false;
    }

    static bool switchItem(sf::Event &event) {
        if(currentController == ControllerType::Keyboard) 
            return event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(mousebind_switchitem);
        else if(currentController == ControllerType::Joystick)
            return sf::Joystick::isButtonPressed(0, joystick_bind_switchitem);
        return false;
    }
}