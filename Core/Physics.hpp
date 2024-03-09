#pragma once
#include "Type.hpp"
#include "GL/glu.h"
#include <SFML/OpenGL.hpp>

namespace dep {
    class PhysicsComponent  {
        public:
            dep::vec3D Position;
            dep::vec3D Rotation;
            dep::vec3D Velocity;

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