#ifndef __CONSOLEUTILS_HPP__
#define __CONSOLEUTILS_HPP__
#include <iostream>

#ifdef _WIN32
#include <windows.h>
const int RESET = 7;
const int BLACK = 0;              
const int RED = 4;                
const int GREEN = 2;              
const int YELLOW = 6;             
const int BLUE = 1;               
const int MAGENTA = 5;            
const int AQUA = 3;               
const int WHITE = 7;              
const int GRAY = 8;               
const int LIGHT_BLUE = 9;         
const int LIGHT_GREEN = 10;       
const int LIGHT_MAGENTA = 13;     
const int LIGHT_YELLOW = 14;   
const int LIGHT_WHITE = 15;   
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

const int RESET = 0;
const int BLACK = 30;
const int RED = 31;
const int GREEN = 32;
const int YELLOW = 33;
const int BLUE = 34;
const int MAGENTA = 35;
const int AQUA = 36;
const int WHITE = 37;
const int GRAY = 90;
const int LIGHT_BLUE = 94;
const int LIGHT_GREEN = 92;
const int LIGHT_YELLOW = 93;
const int LIGHT_MAGENTA = 95;
const int LIGHT_WHITE = 97;
#endif

const int MAX_X = 98;
const int MIN_X = 1;
const int MAX_Y = 48;
const int MIN_Y = 1;


struct Rect {
    int x;
    int y;
    int width;
    int height;
};


class ConsoleUtils {
    private:
        ConsoleUtils() = delete;
        ~ConsoleUtils() = delete;
        #ifdef _WIN32
            static HANDLE hConsole;
        #endif
    public:
        static int consoleWidth;
        static int consoleHeight;

        static void clearConsole();
        static void setTextColor(int);
        static void moveCursor(int,int);
        static void drawBox(int,int,int,int,int);
        static void setConsoleSize(int, int);
        static void setConsoleCursorVisibility(bool,int size = 1);
        static void setWindowResizeable(bool);
        static void setConsoleTitle(const std::string&);
        static void enableUTF8Encoding();
        static void restoreEncoding();
        static Rect getConsoleRect();
        static void hideConsole();
        static void pause();
};
#endif // __CONSOLEUTILS_HPP__