#pragma once
#include <vector>
#include "Type.hpp"
#include <SFML/OpenGL.hpp>

namespace dep {
    typedef dep::vec3D Vertex; 

    class Mesh {
        public:
            void SetRenderType(const GL_BIT renderType) {
                m_renderType = renderType;
            }

            void AddVertex(const Vertex& vertex) {
                m_vertices.push_back(vertex);
            }

            void Render() {
                glBegin(m_renderType);

                for(const auto &vertex : m_vertices) {
                    glVertex3f(vertex.x, vertex.y, vertex.z);
                }

                glEnd();
            }

        private:
            std::vector<Vertex> m_vertices;
            GL_BIT m_renderType;
    };
}