#pragma once
#include "Mesh.hpp"
#include "GameObject.hpp"

namespace dep {
    class Cube : public dep::GameObject {
        public:
            Cube(dep::Color color) : m_color(color) {
                LoadMesh();
            }

            void LoadMesh() {
                m_mesh.SetRenderType(GL_QUADS);
                glColor4f(m_color.r, m_color.g, m_color.b, m_color.a);

                m_mesh.AddVertex(dep::Vertex(-50.f, -50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  -50.f, -50.f));

                m_mesh.AddVertex(dep::Vertex(-50.f, -50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,   50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  -50.f, 50.f));

                m_mesh.AddVertex(dep::Vertex(-50.f, -50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,   50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  -50.f, 50.f));

                m_mesh.AddVertex(dep::Vertex(50.f, -50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,   50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  -50.f, 50.f));

                m_mesh.AddVertex(dep::Vertex(-50.f, -50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  -50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  -50.f,  -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  -50.f, 50.f));

                m_mesh.AddVertex(dep::Vertex(-50.f, 50.f, 50.f));
                m_mesh.AddVertex(dep::Vertex(-50.f,  50.f, -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  50.f,  -50.f));
                m_mesh.AddVertex(dep::Vertex(50.f,  50.f, 50.f));


            }

            void Render(Display&) {
                physicsComponent.Apply();
                m_mesh.Render();
                physicsComponent.Revert();

            }

            void Update(float dt) {

            }

        private:
            dep::Color m_color;
            dep::Mesh m_mesh;
    };
}