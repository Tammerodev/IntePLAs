#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "cmath"

class ItemCreator : public UIState {
public:

    ItemCreator() {
        
        edit_img.create(sizeX,sizeY,sf::Color::Red);
        edit_tex.create(sizeX,sizeY);

        // Perform update in orded to get correct drawingAreaPosition
        update(sf::Vector2f(0,0));

        for(int y = 0; y < sizeY; y++) {
            for(int x = 0; x < sizeX; x++) {
                sf::RectangleShape rect(sf::Vector2f(cellScale, cellScale));
                rect.setPosition(drawingAreaPosition + sf::Vector2f(x * cellScale, y * cellScale));
                rect.setOutlineThickness(-1.5f);
                rect.setOutlineColor(sf::Color::Black);
                rect.setFillColor(sf::Color::Transparent);
                grid.push_back(rect);
            }
        }

        windowSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    }

    bool load() {
        return true;
    }

    void update(const sf::Vector2f &mousepos) {
        edit_tex.update(edit_img);
        edit_spr.setTexture(edit_tex);
        edit_spr.setPosition(drawingAreaPosition);
        edit_spr.setScale(cellScale, cellScale);

        drawingAreaPosition.x = -edit_spr.getGlobalBounds().width / 2;
        drawingAreaPosition.y = -edit_spr.getGlobalBounds().height / 2;

        sf::Vector2f relativeMousePos = sf::Vector2f(mousepos.x - edit_spr.getLocalBounds().left, mousepos.y - edit_spr.getLocalBounds().top);
        
        relativeMousePos.x += edit_spr.getGlobalBounds().left;
        relativeMousePos.y += edit_spr.getGlobalBounds().top;


        sf::Vector2i gridSnappedPos;
        gridSnappedPos.x = floor(relativeMousePos.x / cellScale);
        gridSnappedPos.y = floor(relativeMousePos.y / cellScale);

        // Bounds checking

        if(gridSnappedPos.x < 0) gridSnappedPos.x = 0;
        if(gridSnappedPos.y < 0) gridSnappedPos.y = 0;
        if(gridSnappedPos.x >= sizeX) gridSnappedPos.x = sizeX - 1;
        if(gridSnappedPos.y >= sizeY) gridSnappedPos.y = sizeY - 1;

        loginf("X pos", gridSnappedPos.x, ".");
        loginf("Y pos", gridSnappedPos.y, ".");


        edit_img.setPixel(gridSnappedPos.x, gridSnappedPos.y, selectedColor);
    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target) {
        target.draw(edit_spr);

        for(auto &rect : grid) {
            target.draw(rect);
        }
    }

    void statexit() {

    }

private:

    sf::Vector2i windowSize;

    sf::Color selectedColor = sf::Color::Black;

    sf::Vector2f drawingAreaPosition { 0.f, 0.f};
    int sizeX = 16;
    int sizeY = 16;

    float cellScale = 15.0f;


    std::vector<sf::RectangleShape> grid; 
    sf::Image edit_img;
    sf::Texture edit_tex;
    sf::Sprite edit_spr;

};