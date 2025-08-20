#ifndef __SONG_HPP__
#define __SONG_HPP__

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <atomic>
#include <random>
#include <stdexcept>
#include "lyricLine.hpp"
#include "consoleUtils.hpp"
#include "miniaudio.h"

class Song {
private:
    std::vector<LyricLine> lyrics;
    std::string title;
    std::string artist;
    std::string totalLength;

    
    ma_engine audioEngine;
    ma_sound music;
    bool audioInitialized;

    std::string musicFile;
    std::atomic<double> elapsedTime{0.0};
    
    size_t maxLyricLength = 0;
    std::chrono::steady_clock::time_point startTime;

    double totalTimeInSeconds;

    std::vector<std::string> emojis = {
    "♪", "♪", "♫"
    };
    

public:
    Song(const std::string&, const std::string&);
    ~Song();
    
    bool loadLyricsFromFile(const std::string&);
    bool loadMusic(const std::string&);

    void playMusic();
    double getCurrentMusicTime(); 

    std::string getRandomEmoji();
    
    void displayMusicAnimation(double);

    void displayProgressBar(double, double);
    
    double getTotalTimeInSeconds();
    
    void displayLyricWithEffect(const std::string&, size_t);
    
    void displayUpcomingLines(size_t);

    void displayPreviousLines(size_t );
    
    void play();
};
#endif // __SONG_HPP__