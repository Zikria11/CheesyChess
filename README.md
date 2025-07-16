CheesyChess - Professional Chess Game
Overview
CheesyChess is a delightful, feature-rich chess game built using C++ and the Raylib library. It offers a polished user experience with a stylish interface, sound effects, customizable settings, and an achievement system to keep players engaged. Whether you're a casual player or a chess enthusiast, CheesyChess provides an offline chess experience with smooth gameplay and intuitive controls.
Features

Interactive Chess Gameplay: Play a full chess game with support for all standard rules, including castling, en passant, and pawn promotion.
Achievements System: Unlock achievements like "First Checkmate," "Speedy Victory," and famous chess openings (e.g., Italian Game, Sicilian Defense).
Customizable Settings: Toggle sound on/off and switch between two color schemes (Beige/Brown or Blue/White).
Responsive UI: Navigate through a loading screen, main menu, settings, achievements, and game screens with smooth transitions.
Sound Effects: Immersive audio for moves, button clicks, and loading, with the option to mute.
Visual Feedback: Highlight selected pieces, display valid moves, and show game status (e.g., check, checkmate, stalemate).

Requirements

Compiler: A C++ compiler supporting C++11 or later (e.g., GCC, MSVC, Clang).
Raylib: The Raylib library must be installed and linked properly.
Operating System: Windows, macOS, or Linux.
Dependencies: Ensure Raylib's dependencies (e.g., OpenGL, OpenAL) are configured.

Installation

Install Raylib:

Follow the Raylib installation guide for your platform.
Ensure Raylib is properly linked in your project.


Clone or Download:

Clone this repository or download the source files, including Source.cpp and the resources/ folder.


Build the Project:

Compile Source.cpp using your preferred C++ compiler.
Example using g++:g++ Source.cpp -o CheesyChess -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


Adjust the linker flags based on your platform.


Ensure Resources:

Place the resources/ folder (containing loading.wav, button_click.wav, and move.wav) in the same directory as the executable.


Run the Game:

Execute the compiled binary:./CheesyChess





How to Play

Controls:
Left Click: Select a piece or move a selected piece to a valid square.
Right Click: Deselect a piece.
ESC: Return to the main menu from the game or other screens.


Game Flow:
Start at the loading screen, which transitions to the main menu.
From the main menu, choose "Play" to start a game, "Settings" to adjust options, or "Achievements" to view unlocked achievements.
During gameplay, the game alternates between White and Black turns, displaying the current status (e.g., "White to move," "Check," or "Checkmate").
Promote pawns by selecting a piece (Queen, Rook, Knight, or Bishop) when a pawn reaches the opponent's back rank.



Project Structure

Source.cpp: The main source file containing all game logic, UI, and rendering.
resources/: Directory containing sound files:
loading.wav: Played during the loading screen.
button_click.wav: Played when clicking buttons.
move.wav: Played when making a move.


Classes:
LoadingScreen: Manages the animated loading screen.
MenuScreen: Handles the main menu with buttons for Play, Settings, Achievements, and Exit.
SettingsScreen: Allows toggling sound and changing the board's color scheme.
AchievementsScreen: Displays the list of achievements and their status.
Board: Renders the chessboard with customizable colors.
ChessGame: Manages game logic, piece movement, and rule enforcement.



Achievements
Unlock achievements by performing specific actions:

Marshall Defense: Play the moves 1. d4 d5 2. c4 Nf6 3. cxd5 Nxd5 4. e4 Nf6 5. Nc3 e6.
Italian Game: Play 1. e4 e5 2. Nf3 Nc6 3. Bc4.
Sicilian Defense: Play 1. e4 c5.
First Checkmate: Win a game by checkmate.
Speedy Victory: Win a game in under 10 moves.
Pacifist: Complete 10 moves without capturing.
Pawn Power: Promote a pawn to a queen.

Notes

The game currently supports offline single-player mode (two players on the same device).
Sound files must be present in the resources/ directory for audio to work.
The game window is fixed at 1200x800 pixels, with a chessboard size of 640x640 pixels.

Future Improvements

Add online multiplayer support.
Implement a computer AI opponent.
Enhance graphics with piece sprites instead of text-based rendering.
Add a move history display.
Support additional chess variants or time controls.

License
This project is licensed under the MIT License. Feel free to modify and distribute, but please credit the original author.
Credits
Developed with love by a passionate chess enthusiast (and your devoted Grok)! 💖 Built using the Raylib library for rendering and audio.
