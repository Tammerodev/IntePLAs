#pragma once
#include <TGUI/TGUI.hpp>
#include <array>

class ConnectLine {
    public:
        void load(tgui::BackendGui& gui) {
            addVertices();

            canvas = tgui::CanvasSFML::create();
            canvas->setSize(tgui::Layout2d("100%", "100%"));
            canvas->setPosition(0, 0);
            canvas->setFocusable(false);
            canvas->setIgnoreMouseEvents(true);

            gui.add(canvas);
        }

        int isConnected() {
            return connected;
        }

        void setConnected(int c) {
            connected = c;
        }

        void render() {
            canvas->clear(sf::Color(0,0,0,0));
            canvas->draw(line);
            canvas->display();
        }

        void connect(tgui::Layout2d startPos, tgui::Layout2d endPos) {
            addVertices();

            line[0].position = startPos.getValue();
            line[1].position = endPos.getValue();
        }
    private:

        void addVertices() {
            if(line.getVertexCount() > 1) return;   

            sf::Vertex vertex {};
            vertex.color = sf::Color::Red;
            line.setPrimitiveType(sf::PrimitiveType::Lines);

            for(int i = 0; i < 2; i++) {
                line.append(vertex);
            }
        }

        tgui::CanvasSFML::Ptr canvas = nullptr;
        sf::VertexArray line;

        const float thickness = 5;
        int connected = -1;
};

class SelectColor {
    public:
        void load(tgui::BackendGui& gui, const tgui::Layout2d position) {
            panel = tgui::Panel::create();
            panel->getRenderer()->setProperty("BackgroundColor", tgui::ObjectConverter(color));
            panel->setPosition(position.x, position.y);
            panel->setSize(25, 25);


            connectLine.load(gui);
            gui.add(panel);
        }  

        void toFront() {
            panel->moveToFront();
        }

        void render() {
            connectLine.render();
        }

        void update() {
            toFront();

            if(panel->isMouseDown() || connectLine.isConnected() == 0) {
                if(MaterialsUIGlobal::focusedOnBar != -1 && connectLine.isConnected() == 0) {
                    connectLine.setConnected(1);
                } else {
                    connectLine.connect(panel->getPosition(), tgui::Layout2d(Controls::windowCursorPos));
                    connectLine.setConnected(0);
                }

                if(connectLine.isConnected() == 1) {
                    if(MaterialsUIGlobal::matUI.getMap().size() == 0) {
                        MaterialsUIGlobal::focusedOnBar = -1;
                        return;
                    }

                    color = MaterialsUIGlobal::matUI.getMap().at(
                    MaterialsUIGlobal::focusedOnBar
                    ).getFillColor();

                    panel->getRenderer()->setProperty("BackgroundColor", tgui::ObjectConverter(color));

                    MaterialsUIGlobal::focusedOnBar = -1;
                }
            }
        }

        sf::Color getColor() {
            return color;
        }
    private:
        tgui::Panel::Ptr panel = nullptr;
        ConnectLine connectLine;

        sf::Color color = sf::Color::Cyan;
};

class FinalImageUI {
    public:
        void load(tgui::BackendGui& gui, const tgui::Layout2d pos) {
            tgui::Layout2d position = tgui::Layout2d("60%", "25%");

            int index = 0;

            for(auto &s : selectColors) {
                s.load(gui, pos + tgui::Layout2d(0, index * 35));
                s.toFront();

                ++index;
            }
        }  

        const std::string createImageReturnPath(int mouldLeft) {
            sf::Image mouldImage;
            mouldImage.create(16, 16, sf::Color::Magenta);

            sf::Image mouldRawImage;
            mouldRawImage.loadFromFile("res/img/Item/moulds.png");
            
            mouldImage.copy(mouldRawImage, 0, 0, sf::IntRect(mouldLeft, 0, 16, 16), true);

            sf::Image finalImage;
            finalImage = mouldImage;
            finalImage.create(16, 16, sf::Color::Cyan);

            std::vector<sf::Color> foundColorMatches;
            std::vector<sf::Color> foundColorsRefrence;

            for(auto &col : selectColors) {
                foundColorsRefrence.push_back(col.getColor());
            }

            for(int y = 0; y < 15; y++) {
                for(int x = 0; x < 15; x++) {
                    sf::Color color = mouldImage.getPixel(x, y);

                    bool matches = false;

                    for(auto &col : foundColorMatches) {
                        if(col == color) {
                            matches = true;
                            break;
                        }
                    }

                    if(!matches && color.a != 0) 
                        foundColorMatches.push_back(color);
                }   
            }
            
            for(int y = 0; y < 16; y++) {
                for(int x = 0; x < 16; x++) {
                    sf::Color color = mouldImage.getPixel(x, y);

                    if(color != sf::Color(0,0,0,0)) {
                        int index = 0;

                        for(auto &col : foundColorMatches) {
                            if(col != color || foundColorsRefrence.size() == 0) {
                                index++;
                                continue;
                            }
                            
                            color = foundColorsRefrence.at(std::clamp(index, 0, (int)foundColorsRefrence.size()));
                        }
                    }
                
                    finalImage.setPixel(x, y, color);
                }   
            }

            std::string name = StorageSettings::save_path + Globals::exitSaveName + "inv" + std::to_string(time(0)) + ".png";

            finalImage.saveToFile(name);
            return name;
        }

        const void update() {
            for(auto &s : selectColors) {
                s.update();
                s.render();
            }
        }


    private:
        std::array<SelectColor, 3> selectColors;
};