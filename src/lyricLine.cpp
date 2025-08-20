#include "lyricLine.hpp"

using namespace std;

LyricLine::LyricLine(double time, const string& lyric) : timeInSeconds(time), text(lyric),
isEmpty(lyric.empty()) {}

double LyricLine::parseTime(const string& timeStr) {
    regex timeRegex(R"((\d+):(\d+)\.(\d+))");
    smatch matches;
    
    if (regex_match(timeStr, matches, timeRegex)) {
        int minutes = stoi(matches[1].str());
        int seconds = stoi(matches[2].str());
        int centiseconds = stoi(matches[3].str());
        
        return minutes * 60.0 + seconds + centiseconds / 100.0;
    }
    return 0.0;
}