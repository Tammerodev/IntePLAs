#pragma once
#include "Type.hpp"
#include "GL/glu.h"
#include <SFML/OpenGL.hpp>

namespace dep {
    class Camera {
        public:
            dep::vec3D Position = {0.f, 0.f, -100.f};
            dep::vec3D Rotation = {0.f, 0.f, 0.f};
        public:

            void Apply() {
                glRotatef(Rotation.x, 1.f, 0.f, 0.f);
                glRotatef(Rotation.y, 0.f, 1.f, 0.f);
                glRotatef(Rotation.z, 0.f, 0.f, 1.f);

                glTranslatef(Position.x, Position.y, Position.z);
            }

            void Revert() {
                glTranslatef(-Position.x, -Position.y, -Position.z);


                glRotatef(-Rotation.x, 1.f, 0.f, 0.f);
                glRotatef(-Rotation.y, 0.f, 1.f, 0.f);
                glRotatef(-Rotation.z, 0.f, 0.f, 1.f);
            }
    };
}
