#pragma once
#include "Display.hpp"
#include "Type.hpp"
#include "Physics.hpp"

namespace dep {

    class GameObject {
        public:
            virtual void LoadMesh() = 0;
            virtual void Update(float dt) = 0;
            virtual void Render(dep::Display&) = 0;

            dep::PhysicsComponent& getPhysicsComponent() {
                return physicsComponent;
            }

        protected:
            dep::PhysicsComponent physicsComponent;
    };

}