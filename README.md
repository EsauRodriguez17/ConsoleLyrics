# LYRICS - Synchronized Lyrics Player 

A cross-platform console-based C++ application that plays music files while displaying synchronized lyrics with visual effects.

## Features

- **Synchronized Lyrics Display**: Displays lyrics in perfect sync with the music
- **Multiple Audio Formats**: Supports `.wav`, `.flac`, and `.mp3` files
- **LRC File Support**: Compatible with standard `.lrc` lyrics format and plain `.txt` files
- **Visual Effects**: 
  - Typewriter effect for lyrics display
  - Animated loading sequences with musical notes
  - Progress bar
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Console-Based UI**: Clean, resizable console interface

## Requirements 

- **Compiler**: g++ (GCC) or MinGW-w64 (for Windows)
- **Operating System**: Windows, Linux, or macOS
- **Dependencies**: 
  - miniaudio (included)
  - Standard C++ libraries

## Project Structure 📁

```
lyrics/
├── src/
│   ├── main.cpp          # Main application entry point
│   ├── song.cpp          # Core song playback and lyrics sync
│   ├── consoleUtils.cpp  # Console manipulation utilities
│   ├── lyricLine.cpp     # Lyrics parsing and timing
│   └── miniaudio.c       # Audio playback library
├── include/
│   ├── song.hpp
│   ├── consoleUtils.hpp
│   ├── lyricLine.hpp
│   └── miniaudio.h
├── output/               # Build output directory
├── Makefile             # Cross-platform build configuration
└── README.md
```

## Installation

### Prerequisites
Make sure you have a C++ compatible compiler installed:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential
```

**Windows:**
Install MinGW-w64 or use MSYS2

**macOS:**
```bash
xcode-select --install
```

### Build Instructions

1. **Clone the repository:**
```bash
git clone https://github.com/EsauRodriguez17/ConsoleLyrics.git
cd lyrics
```

2. **Compile the project:**

**Linux/macOS:**
```bash
make
```

**Windows (MinGW):**
```bash
mingw32-make
```

3. **Run the application:**

**Linux/macOS:**
```bash
make run
```

**Windows (MinGW):**
```bash
mingw32-make run
```

## Usage 🎤

1. **Prepare your files:**
   - Audio file: `.wav`, `.flac`, or `.mp3`
   - Lyrics file: `.lrc` or `.txt` with timestamps
   - You can place them in the output folder or specify a full/relative path.

2. **Run the application:**
```bash
# Linux/macOS
./output/main

# Windows
output\main.exe
```

3. **Follow the prompts:**
   - Enter the path to the lyrics file (full or relative)
   - Enter the path to the music file (full or relative)
   - Press Enter to start playback

## LRC File Format 📝

The application supports standard LRC format:

```lrc
[ti:Song Title]
[ar:Artist Name]
[length: 3:45]

[00:12.50]First line of lyrics
[00:18.20]Second line of lyrics
[00:25.00]
[00:30.10]Line after instrumental break
```

You can find .lrc files with the required format for many songs at [Lyricsify](https://www.lyricsify.com/).

**Supported tags:**
- `[ti:title]` or `[title:title]` - Song title
- `[ar:artist]` or `[artist:artist]` - Artist name
- `[length: mm:ss]` - Total song duration
- `[mm:ss.xx]` - Timestamp for lyrics line

## Features in Detail 

### Visual Effects
- **Typewriter Animation**: Lyrics appear character by character
- **Progress Bar**: Real-time playback progress with time display
- **Color Coding**: Different colors for current, previous, and upcoming lyrics
- **Musical Emojis**: Dynamic emoji display during playback

### Console Interface
- **Auto-resize**: Console adjusts to accommodate longest lyric line

### Audio Playback
- **Lightweight**: Uses miniaudio for efficient audio processing
- **Multiple Formats**: Supports common audio file formats
- **Precise Timing**: Accurate synchronization with lyrics timestamps

## Troubleshooting 

**Common Issues:**

1. **"Lyrics file invalid or not found"**
   - Check file path and format
   - Ensure LRC timestamps are properly formatted

2. **"The music file could not be loaded"**
   - Verify audio file format is supported
   - Check file permissions and path

3. **Build errors on Windows**
   - Ensure MinGW is properly installed
   - Use `mingw32-make` instead of `make` in a MinGW terminal
   - Make sure MinGW bin directory is in your PATH


## Dependencies

This project uses **[Miniaudio](https://miniaud.io/)**, a lightweight audio playback library:

- Repository: [https://github.com/mackron/miniaudio](https://github.com/mackron/miniaudio)  
- Official page / Documentation: [https://miniaud.io/](https://miniaud.io/)

