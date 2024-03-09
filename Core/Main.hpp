#pragma once

#include <memory>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <thread>

#include "Display.hpp"
#include "State.hpp"
#include "GameObject.hpp"
#include "OpenGL.hpp"


namespace dep {

	bool application_running = false;

	std::map<dep::string, std::shared_ptr<dep::GameObject>> GameObjects;

	void AddState(const char* stateName, const std::shared_ptr<State> state) {
		State::s_states.emplace_back(stateName, state);
	}

	bool SetCurrentState(const char* stateName) {
		for (auto& n : State::s_states) {
			if (n.first ==  stateName) {
				State::s_currentState = n.second;
				return true;
			}
		}
		return false;
	}

	void BeginApplication(Display& display, dep::opengl::OpenGLSettings settings) {
		application_running = true;

		sf::Event ev;


		dep::opengl::initOpenGL(settings);

		while (application_running) {
			if (State::s_currentState == nullptr) return;

			while (display.pollEvent(ev)) {
				State::s_currentState->input(ev);
			}

			State::s_currentState->update(display);
			State::s_currentState->render(display);

			display.display();
		}
	}

	void RenderGameObjectsTo(Display&d) {
		for(const auto &gameObject : GameObjects) {
			gameObject.second->Render(d);
		}
	}

	void DisplayScreen(Display&d) {
		d.display();
	}

	void UpdateGameObjects(const float dt) {
		for(const auto &gameObject : GameObjects) {
			gameObject.second->Update(dt);
		}
	}

}
