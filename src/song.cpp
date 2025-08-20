#include "song.hpp"

using namespace std;

Song::Song(const string& lyricsFile, const string& _musicFile) :musicFile(_musicFile),audioInitialized(false) {
    if (!loadLyricsFromFile(lyricsFile)) {
        throw invalid_argument("Lyrics file invalid or not found: " + lyricsFile);
    }
    
    if (!loadMusic(musicFile)) {
        throw runtime_error("The music file could not be loaded.: " + musicFile);
    }

    ConsoleUtils::enableUTF8Encoding();
    ConsoleUtils::setConsoleSize(maxLyricLength+10,20);
    ConsoleUtils::setWindowResizeable(false);
}

Song::~Song() {
    if (audioInitialized) {
        ma_sound_uninit(&music);
        ma_engine_uninit(&audioEngine);
    }
}

bool Song::loadLyricsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr<< endl << "Error: The file could not be opened: " << filename << endl;
        return false;
    }
    
    string line;
    regex lrcRegex(R"(\[(\d+:\d+\.\d+)\](.*))");
    regex lengthRegex(R"(\[length: (\d+:\d+)\])");
    regex tagRegex(R"(\[(\w+):\s*(.*)\])");
    smatch matches;
    
    while (getline(file, line)) {
        // total duration
        if (regex_match(line, matches, lengthRegex)) {
            totalLength = matches[1].str();
            continue;
        }

        // optional tags
        if (regex_match(line, matches, tagRegex)) {
            string tagName = matches[1].str();
            string tagValue = matches[2].str();

            if(tagName == "title" || tagName == "ti"){
                title = tagValue;
            }
            else if (tagName == "artist" || tagName == "ar"){
                artist = tagValue;
            }
            continue;
        }
        
        // lines of letters
        if (regex_match(line, matches, lrcRegex)) {
            string timeStr = matches[1].str();
            string lyricText = matches[2].str();
            
            double timeInSeconds = LyricLine::parseTime(timeStr);
            lyrics.emplace_back(timeInSeconds, lyricText);

            if (lyricText.size() > maxLyricLength) {
                maxLyricLength = lyricText.size();
            }
        }
    }
    
    file.close();
    
    if (lyrics.empty()) {
        cerr << "Error: No valid letters were found in the file" << endl;
        return false;
    }
    ConsoleUtils::setTextColor(RESET);
    
    return true;
}

bool Song::loadMusic(const string& musicFile){
    // Initialize engine
    ma_result result = ma_engine_init(NULL, &audioEngine);
    if (result != MA_SUCCESS) {
        return false;
    }
    
    // load song
    result = ma_sound_init_from_file(&audioEngine, musicFile.c_str(), 
                                   MA_SOUND_FLAG_STREAM, NULL, NULL, &music);
    
    if (result != MA_SUCCESS) {
        ma_engine_uninit(&audioEngine);
        return false;
    }
    
    ma_sound_set_looping(&music, MA_FALSE);
    audioInitialized = true;
    return true;
}

void Song::playMusic(){
    if (audioInitialized) {
        ma_sound_start(&music);
    }
}

double Song::getCurrentMusicTime(){
    if (!audioInitialized) return 0.0;
    
    float cursor;
    ma_result result = ma_sound_get_cursor_in_seconds(&music, &cursor);
    
    if (result == MA_SUCCESS) {
        return static_cast<double>(cursor);
    }
    return 0.0;
} 

string Song::getRandomEmoji(){
    static auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    static mt19937_64 rng(seed);
    
    uniform_int_distribution<int> dist(0, emojis.size() - 1);
    
    return emojis[dist(rng)];
}

void Song::displayLyricWithEffect(const string& text, size_t currentIndex) {
    if (text.empty()) return;
    
    // Calculate time available until the next line
    double availableTime = 0.0;
    if (currentIndex + 1 < lyrics.size()) {
        availableTime = lyrics[currentIndex + 1].timeInSeconds - lyrics[currentIndex].timeInSeconds;
    } 
    else {
        availableTime = 2.0;
    }
    
    double typingTime = availableTime * 0.7;
    
    int textLength = text.length();
    double delayPerChar = typingTime / textLength;
    
    int delayMs = static_cast<int>(max(0.02, min(0.2, delayPerChar)) * 1000);
    
    ConsoleUtils::setTextColor(LIGHT_BLUE);
    ConsoleUtils::moveCursor(1, 7);
    cout<<string(ConsoleUtils::consoleWidth-2,' ');
    ConsoleUtils::moveCursor(1,7);

    string emoji = getRandomEmoji();
    cout<<emoji<<" ";
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << " "<<emoji;
    ConsoleUtils::setTextColor(RESET);
}

void Song::displayUpcomingLines(size_t currentIndex) {
    int row = 9;

    ConsoleUtils::setTextColor(GRAY);
    ConsoleUtils::moveCursor(1,row);
    
    for (size_t i = currentIndex + 1; i < min(currentIndex + 4, lyrics.size()); i++) {
        if (!lyrics[i].isEmpty) {
            row++;
            ConsoleUtils::moveCursor(1,row);
            cout<<string(ConsoleUtils::consoleWidth-2,' ');
            ConsoleUtils::moveCursor(1,row);
            cout<< "- " << lyrics[i].text;
            
        }
    }
    ConsoleUtils::setTextColor(RESET);
}

void Song::displayPreviousLines(size_t currentIndex) {
    if (currentIndex == 0) return;
    
    int row = (currentIndex >= 2) ? 3 : 4;
    size_t startIdx = (currentIndex >= 2) ? currentIndex - 2 : 0;
    ConsoleUtils::setTextColor(GRAY);
    
    for (size_t i = startIdx; i < currentIndex; i++) {
        if (!lyrics[i].isEmpty) {
            ConsoleUtils::moveCursor(1, row);
            cout<<string(ConsoleUtils::consoleWidth-2,' ');
            ConsoleUtils::moveCursor(1,row);
            cout<<"- " << lyrics[i].text;
            row++;
        }
    }
    ConsoleUtils::setTextColor(RESET);
}

void Song::displayMusicAnimation(double availableTimeInSeconds) {
    vector<string> frames = {
        "♪   ♫   ♪   ♫",
        " ♪   ♫   ♪   ♫ ",
        "  ♪   ♫   ♪   ♫  ",
        " ♪   ♫   ♪   ♫ ",
        "♪   ♫   ♪   ♫"
    };

    auto start = chrono::steady_clock::now();
    int frameIndex = 0;

    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - start).count();

        if (elapsed >= availableTimeInSeconds) break;
        displayProgressBar(getCurrentMusicTime(),totalTimeInSeconds);

        ConsoleUtils::setTextColor(LIGHT_MAGENTA);
        ConsoleUtils::moveCursor(1, 7);
        cout << string(ConsoleUtils::consoleWidth-5, ' ');
        ConsoleUtils::moveCursor(1, 7);
        cout << frames[frameIndex % frames.size()] << flush;
        ConsoleUtils::setTextColor(RESET);

        frameIndex++;
        this_thread::sleep_for(chrono::milliseconds(250));
    }
}

void Song::displayProgressBar(double currentTime, double totalTimeInSeconds) {
    const int barWidth = ConsoleUtils::consoleWidth -8;
    double progress = min(currentTime / totalTimeInSeconds, 1.0);
    int pos = static_cast<int>(barWidth * progress);

    ConsoleUtils::moveCursor(1, 16);
    cout << string(ConsoleUtils::consoleWidth-2, ' ');
    ConsoleUtils::moveCursor(1, 16);

    ConsoleUtils::setTextColor(LIGHT_WHITE);
    cout << "♫ [";

    ConsoleUtils::setTextColor(AQUA);

    for (int i = 0; i < pos; i++) {
        cout << "─";
    }

    cout << "●";

    ConsoleUtils::setTextColor(GRAY);
    for (int i = pos + 1; i < barWidth; i++) {
        cout << "─";
    }

    ConsoleUtils::setTextColor(LIGHT_WHITE);
    cout << "] ♫";
    
    int minutes = static_cast<int>(currentTime) / 60;
    int seconds = static_cast<int>(currentTime) % 60;
    
    ConsoleUtils::setTextColor(LIGHT_WHITE);
    ConsoleUtils::moveCursor(1, 17);
    cout << string(ConsoleUtils::consoleWidth-2, ' ');
    ConsoleUtils::moveCursor(1, 17);

    cout<< setfill('0') << setw(2) << minutes 
              << ":" << setw(2) << seconds;
    
    ConsoleUtils::moveCursor(ConsoleUtils::consoleWidth-6, 17);
    cout<< totalLength;
    ConsoleUtils::setTextColor(RESET);
}

double Song::getTotalTimeInSeconds() {
    if (totalLength.empty()) return 300.0;
    
    regex timeRegex(R"((\d+):(\d+))");
    smatch matches;
    
    if (regex_match(totalLength, matches, timeRegex)) {
        int minutes = stoi(matches[1].str());
        int seconds = stoi(matches[2].str());
        return minutes * 60.0 + seconds;
    }
    return 300.0;
}

void Song::play() {
    if (lyrics.empty()) {
        cout << "Error: There are no letters to play" << endl;
        return;
    }
    
    string consoleTitle = "Playing song";

    if (!title.empty() && !artist.empty()) {
        consoleTitle = title + " - " + artist;
    } else if (!title.empty()) {
        consoleTitle = title;
    } else if (!artist.empty()) {
        consoleTitle = artist;
    }

    ConsoleUtils::setConsoleTitle(consoleTitle);
    ConsoleUtils::clearConsole();
    ConsoleUtils::setTextColor(LIGHT_GREEN);
    cout<<"ALL READY"<<endl;
    cout<<"Press enter to start the song";
    cin.get();
    ConsoleUtils::setTextColor(RESET);
    ConsoleUtils::setConsoleCursorVisibility(false);
    ConsoleUtils::clearConsole();
    ConsoleUtils::drawBox(0,ConsoleUtils::consoleWidth-1,0,ConsoleUtils::consoleHeight-1,GRAY);
    
    startTime = chrono::steady_clock::now();
    size_t currentLineIndex = 0;
    totalTimeInSeconds = getTotalTimeInSeconds();

    playMusic();

    while (currentLineIndex < lyrics.size()) {
        // Calculate elapsed time
        auto currentTime = chrono::steady_clock::now();
        elapsedTime = chrono::duration<double>(currentTime - startTime).count();

        // check if there is time before the first line
        if (currentLineIndex == 0 && elapsedTime < lyrics[0].timeInSeconds) {
            double timeToFirstLine = lyrics[0].timeInSeconds - elapsedTime;

            displayProgressBar(elapsedTime, totalTimeInSeconds);
            displayMusicAnimation(timeToFirstLine);
            continue;
        }
        
        // Check if it's time to display the current line
        if (elapsedTime >= lyrics[currentLineIndex].timeInSeconds) {

            displayProgressBar(elapsedTime,totalTimeInSeconds);
            displayPreviousLines(currentLineIndex);
            displayUpcomingLines(currentLineIndex);
            
            if (lyrics[currentLineIndex].isEmpty) {
                double availableTime = 0.0;
                if (currentLineIndex + 1 < lyrics.size()) {
                    availableTime = lyrics[currentLineIndex + 1].timeInSeconds - lyrics[currentLineIndex].timeInSeconds;
                }
                else {
                    availableTime = 2.0; // default
                }
                displayMusicAnimation(availableTime);
            }
            else {
                displayLyricWithEffect(lyrics[currentLineIndex].text, currentLineIndex);
            }
            
            currentLineIndex++;
        }
        
        this_thread::sleep_for(chrono::milliseconds(5));
    }

    // After the last line, if there is time remaining
    double remainingTime = totalTimeInSeconds - elapsedTime;
    if (remainingTime > 0.05) {
        displayMusicAnimation(remainingTime);
    }
    
    ConsoleUtils::clearConsole();
    ConsoleUtils::setTextColor(LIGHT_MAGENTA);
    ConsoleUtils::setConsoleCursorVisibility(true);
    cout<<"END"<<endl;
    cout<<"Press enter to close";
    ConsoleUtils::setTextColor(RESET);
    cin.get();
}