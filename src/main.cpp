#include <iostream>
#include <string>
#include "song.hpp"

using namespace std;

int main() {
    string filename;
    string musicFile;
    
    cout << "Enter the path to the lyrics file [or just filename if in current folder] (.lrc or .txt) : ";
    getline(cin, filename);

    cout << "Enter the path of the music file [or just filename if in current folder] (.wav, .flac, .mp3) : ";
    getline(cin, musicFile);

    try {
        Song song(filename, musicFile);
        song.play();
    } catch (const exception& e) {
        cerr << e.what() << endl<<endl;
        cout<<"Press enter to continue";
        cin.get();
        return 1;
    }
    
    return 0;
}