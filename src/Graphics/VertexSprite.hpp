#pragma once
#include <SFML/Graphics.hpp>

class VertexSprite {
    public:
        void load(sf::Texture& tx) {
            vert.clear();
            vert.setPrimitiveType(sf::PrimitiveType::TriangleFan);

            addVertex(sf::Vertex());
            addVertex(sf::Vertex());
            addVertex(sf::Vertex());
            addVertex(sf::Vertex());

            loadTexCoords(tx);
        }

        void loadTexCoords(sf::Texture& tx) {
            float w = (float)tx.getSize().x;
            float h = (float)tx.getSize().y;

            getVertex(0).texCoords = {0, 0};
            getVertex(1).texCoords = {0, h};
            getVertex(2).texCoords = {w, 0};
            getVertex(3).texCoords = {w, h};
        }

        sf::Vertex &getVertex(int index) {
            return vert[index];
        }

        void render(sf::RenderTarget& target, sf::Texture& tx) {
            target.draw(vert, &tx);
        }

    private:

        void addVertex(const sf::Vertex& vertex) {
            vert.append(vertex);
        }

        sf::VertexArray vert;
};