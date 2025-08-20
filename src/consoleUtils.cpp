#include "consoleUtils.hpp"

using namespace std;

#ifdef _WIN32
HANDLE ConsoleUtils::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
static UINT originalCP = 0;
#endif

int ConsoleUtils::consoleWidth = 80;  
int ConsoleUtils::consoleHeight = 25;  

void ConsoleUtils::clearConsole() {
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count, cellCount;
        COORD homeCoords = {0, 0};

        if (hConsole == INVALID_HANDLE_VALUE) return;
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

        cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
        SetConsoleCursorPosition(hConsole, homeCoords);
    #else
        system("clear");
    #endif
}

void ConsoleUtils::setTextColor(int color){
    #ifdef _WIN32
        SetConsoleTextAttribute(hConsole, color);
    #else
        cout << "\033[" << color << "m";
    #endif
}

void ConsoleUtils::moveCursor(int x,int y){
    #ifdef _WIN32
        COORD position = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
        SetConsoleCursorPosition(hConsole, position);
    #else
        cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    #endif
}

void ConsoleUtils::drawBox(int x1,int x2,int y1,int y2,int color){

    setTextColor(color);

    // Draw the top and bottom
    for (int i = x1; i <= x2; i++) {
        moveCursor(i, y1); cout << "─"; // Top
        moveCursor(i, y2); cout << "─"; // Bottom
    }

    // Draw the left and right borders
    for (int i = y1; i <= y2; i++) {
        moveCursor(x1, i); cout << "│"; // Left side
        moveCursor(x2, i); cout << "│"; // Right side
    }

    // Corners
    moveCursor(x1, y1); cout << "┌";
    moveCursor(x1, y2); cout << "└";
    moveCursor(x2, y1); cout << "┐";
    moveCursor(x2, y2); cout << "┘";

    setTextColor(RESET);
    //moveCursor(40,12);
}

void ConsoleUtils::setConsoleSize(int width, int height) {
    if (width < 1 || height < 1) {
        cerr << "invalid dimensions" << endl;
        return;
    }
    
#ifdef _WIN32
    string command = "mode con: cols=" + to_string(width) + 
                         " lines=" + to_string(height);
    
    int result = system(command.c_str());
    if (result != 0) {
        cerr << "error resizing the console: " << endl;
    }
    else{
        consoleWidth = width;
        consoleHeight = height;
    }
    
#else
    struct winsize w;
    w.ws_row = height;
    w.ws_col = width;
    w.ws_xpixel = 0;
    w.ws_ypixel = 0;

    if (ioctl(STDOUT_FILENO, TIOCSWINSZ, &w) == -1) {
        perror("ioctl failed");

        cout << "\033[8;" << height << ";" << width << "t" << flush;
        string command = "command -v resize > /dev/null 2>&1 && resize -s " +
                         to_string(height) + " " + to_string(width) +
                         " > /dev/null 2>&1";
        int result = system(command.c_str());
        if (result != 0) {
            cerr << "error resizing console" << endl;
        }
    } 
    else {
        consoleWidth = width;
        consoleHeight = height;
    }
    
#endif
}

void ConsoleUtils::setConsoleCursorVisibility(bool visible, int size) {
    #ifdef _WIN32
        if (visible && size == 1) {
            size = 50;
        }
        CONSOLE_CURSOR_INFO lpCursor;
        lpCursor.bVisible = visible;
        lpCursor.dwSize = static_cast<DWORD>(size);
        SetConsoleCursorInfo(hConsole, &lpCursor);
    #else
        if (visible) {
            cout << "\033[?25h";  // Show
        } else {
            cout << "\033[?25l";  // Hide
        }
    #endif
}

void ConsoleUtils::setWindowResizeable(bool enable){
#ifdef _WIN32
    // In Windows only, we enable or disable the maximize/resize option
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);

    if (enable) {
        // Enable maximize and resize
        style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);
    } else {
        // Disable maximize and resize
        style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
    }

    SetWindowLong(consoleWindow, GWL_STYLE, style);
#else
    cout << "Cannot control console maximization/resizing on this OS."<<endl;
#endif
}

void ConsoleUtils::setConsoleTitle(const string& title) {
    #ifdef _WIN32
        SetConsoleTitleW(wstring(title.begin(), title.end()).c_str());
    #else
        cout << "\033]0;" << title << "\007";
    #endif
}

void ConsoleUtils::enableUTF8Encoding() {
#ifdef _WIN32
    originalCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void ConsoleUtils::restoreEncoding() {
#ifdef _WIN32
    if (originalCP != 0)
        SetConsoleOutputCP(originalCP);
#endif
}


Rect ConsoleUtils::getConsoleRect() {
#ifdef _WIN32
    HWND hwnd = GetConsoleWindow(); 
    RECT rect;
    GetWindowRect(hwnd, &rect);

    Rect r;
    r.x = rect.left;
    r.y = rect.top;
    r.width = rect.right - rect.left;
    r.height = rect.bottom - rect.top;
    return r;
#endif
}

void ConsoleUtils::hideConsole(){
#ifdef _WIN32
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

void ConsoleUtils::pause(){
    moveCursor(3,22);
    cout<<"Press Enter to continue...";
    cin.get();
}



