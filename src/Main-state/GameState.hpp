#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Sound/SoundFX.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/Player.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/math.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/IObjects/Gun.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Background.hpp"

class GameState : public MainState {
public:
	virtual bool load() {

		view.setSize(800,800);
		view.zoom(0.5);

		if(!vx_manager.load())
			perror("VoxelManager failed to load");
		if(!bg.load()) 
			perror("Background failed to load");
		if(!player.load()) 
			perror("Player failed to load");
		if(!SFX::load())
			perror("Failed to load sound effect");
			
		music.openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Calm.wav");
		music.play();

		thread1 = std::thread(displayDebugInfo, std::ref(delta_T));

		return true;
	}
    virtual void update();
	virtual void input(sf::Event &ev);

	virtual void draw(sf::RenderTarget&window);
private:

	bool slowmo = false;
	static void displayDebugInfo(float &delta_T) {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "Time : " << time(0) << '\n';
			std::cout << "DT :" << delta_T << '\n';
		}
	}

	Player player;
	Gun gun = Gun(vx_manager);
	Background bg;

	sf::View view;

	sf::Music music;

	VoxelManager vx_manager {};

	float delta_T = 0.f;
	sf::Clock deltaClock;


	std::thread thread1{};
};