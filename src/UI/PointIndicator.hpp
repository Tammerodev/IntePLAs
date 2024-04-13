#pragma once
#include "../Entity.hpp"
#include "../Settings.hpp"

class PointIndicator {
    public:
        void load(tgui::BackendGui& gui) {
            texture.load("res/img/UI/pointer.png");

            picture = tgui::BitmapButton::create();
            picture->setImage(texture);
            picture->setScale(5.f);
            picture->setWidgetName(DONT_REMOVE_WIDGET);
            picture->setOrigin(0.5f, 0.5f);
            
            gui.add(picture);
        }

        void update(const sf::Vector2f &view_pos, const sf::Vector2f &target_pos) {
            const int distance = view_pos.x - target_pos.x;
            const int leftBorder  = -viewRect.width / 2;
            const int rightBorder =  viewRect.width / 2;

            sf::Vector2f position {0,0};

            picture->setVisible(true);

            if(distance < leftBorder) {
                position = sf::Vector2f(Display::window_width - 16 * 5, Display::window_height / 2);
                picture->setRotation(180);
            } else if(distance > rightBorder) {
                position = sf::Vector2f(32, Display::window_height / 2);
                picture->setRotation(0);
            } else {
                picture->setVisible(false);
            }

            picture->setPosition(tgui::Layout2d(position));
        }

        void render(sf::RenderTarget& target) {
            viewRect = sf::FloatRect(target.getView().getCenter() - target.getView().getSize() / 2.f, target.getView().getSize());
            
        }

    private:
        sf::FloatRect viewRect;

        tgui::Texture texture;
        tgui::BitmapButton::Ptr picture;
};