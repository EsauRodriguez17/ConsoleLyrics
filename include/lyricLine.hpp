#ifndef __LYRICLINE_HPP__
#define __LYRICLINE_HPP__

#include <iostream>
#include <string>
#include <regex>

class LyricLine {
public:
    double timeInSeconds;
    std::string text;
    bool isEmpty;
    
    LyricLine(double, const std::string&);
    
    static double parseTime(const std::string&);
};
#endif // __LYRICLINE_HPP__