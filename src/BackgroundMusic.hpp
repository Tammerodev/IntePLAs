#pragma once
#include <SFML/Audio.hpp>
#include "common.hpp"

namespace BGMusic {
    static enum Song {
      ONKES, KINESE, CALM, SANDFALL, BLASS,
    } current_song;
    static std::vector<sf::Music*> song_list;

    static Song getRandomSong() {
      return (Song)(rand()%5);
    }

    static int load() {
        current_song = BLASS;
        for(int i=0;i<5;i++) {
          song_list.push_back(new sf::Music());
          prndd("Added object music to vector");
        }
        song_list[Song::CALM]->openFromFile("res/music/Calm.wav");
        prndd("Loaded song CALM");
        song_list[Song::ONKES]->openFromFile("res/music/Onkes.wav");
        prndd("Loaded song ONKES");
        song_list[Song::KINESE]->openFromFile("res/music/Kinese.wav");
        prndd("Loaded song KINESE");
        song_list[Song::SANDFALL]->openFromFile("res/music/Sandfall.wav");     
        prndd("Loaded song SANDFALL");
        song_list[Song::BLASS]->openFromFile("res/music/Blass.wav");     
        prndd("Loaded song BLASS");
        prndd("Loading complete");

        song_list[current_song]->play();
        song_list[current_song]->setVolume(0);

        return 1;

    }

    static void stop() {
      song_list[current_song]->stop();
    }
    static void update() {
      if(song_list[current_song]->getStatus() != sf::Music::Playing) {
        current_song = getRandomSong();

        song_list[current_song]->play();
        song_list[current_song]->setVolume(0);
      }
    }

}