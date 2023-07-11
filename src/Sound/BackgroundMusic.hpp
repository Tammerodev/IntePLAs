#pragma once
#include <SFML/Audio.hpp>
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"

namespace BGMusic {
    static enum Song {
      ONKES, KINESE, CALM, SANDFALL
    } current_song;
    static std::vector<sf::Music*> song_list;

    static int load() {
        current_song = Song::SANDFALL;
        for(int i=0;i<4;i++) {
          song_list.push_back(new sf::Music());
          prndd("Added object music to vector");
        }
        song_list[Song::CALM]->openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Calm.wav");
        prndd("Loaded song CALM");
        song_list[Song::ONKES]->openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Onkes.wav");
        prndd("Loaded song ONKES");
        song_list[Song::KINESE]->openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Kinese.wav");
        prndd("Loaded song KINESE");
        song_list[Song::SANDFALL]->openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Sandfall.wav");     
        prndd("Loaded song SANDFALL");
        prndd("Loading complete");

        song_list[current_song]->play();

        return 1;

    }
    static void update() {
      
    }

}