#include <SFML/Graphics.hpp>
#include <memory>

#include "Core/Main.hpp"
#include "Core/Type.hpp"
#include "Core/Display.hpp"
#include "Core/State.hpp"

#include "GL/glu.h"
#include "Core/Cube.hpp"
#include "Core/Camera.hpp"

sf::Clock Clock;

class AppState : public dep::State {
public:
    AppState() {
        dep::GameObjects.insert({"Cube", std::make_shared<dep::Cube>(dep::Color(0.820, 0.755, 0.385, 0.5f))});
    }

    void input(sf::Event&e) {
        
    }

    void update(dep::Display& display) {
        float s = 10.f;

        dep::GameObjects.at("Cube")->getPhysicsComponent().Rotation.x += 1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam.Position.z += s;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam.Position.z -= s;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam.Position.x += s;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam.Position.x -= s;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            cam.Rotation.y -= s / 10.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            cam.Rotation.y += s / 10.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            cam.Rotation.z += s / 10.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            cam.Rotation.z -= s / 10.f;

        dep::UpdateGameObjects(1.f);
    }

    void render(dep::Display&dis) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        cam.Apply();

        dep::RenderGameObjectsTo(dis);

        dep::DisplayScreen(dis);

        cam.Revert();
    }

    void leave() {

    }

private:
    dep::Camera cam;
};

int main()
{
    dep::Display display(dep::vec2I(1000,800), "FP2 Physics Engine");
    display.setFramerateLimit(60);

    dep::AddState("app", std::make_shared<AppState>());
    dep::SetCurrentState("app");

    dep::opengl::OpenGLSettings settings;
    settings.clearColor = dep::Color(0.0285f, 0.0877f, 0.190f, 0.f);
    dep::BeginApplication(display, settings);
    return 0;
}