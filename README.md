# ♟️ CheesyChess - Professional Chess Game

## 🧀 Overview

**CheesyChess** is a delightful, feature-rich chess game built using **C++** and the **Raylib** library. It offers a polished user experience with a stylish interface, sound effects, customizable settings, and an achievement system to keep players engaged.

Whether you're a casual player or a chess enthusiast, CheesyChess provides an offline chess experience with smooth gameplay and intuitive controls.

---

## ✨ Features

- **Interactive Chess Gameplay**  
  Play a full chess game with support for all standard rules, including:
  - Castling
  - En Passant
  - Pawn Promotion

- **Achievements System**  
  Unlock chess milestones like:
  - *First Checkmate*
  - *Speedy Victory*
  - *Famous Openings* (e.g., *Italian Game*, *Sicilian Defense*)

- **Customizable Settings**  
  - Toggle sound on/off  
  - Switch between two board color schemes: `Beige/Brown` or `Blue/White`

- **Responsive UI**  
  Smooth transitions across:
  - Loading screen  
  - Main menu  
  - Settings  
  - Achievements  
  - Game screen

- **Sound Effects**  
  Immersive audio for:
  - Piece movements  
  - Button clicks  
  - Loading screen  
  (with optional mute)

- **Visual Feedback**  
  - Highlights for selected pieces  
  - Display valid move options  
  - Show game states: `Check`, `Checkmate`, `Stalemate`

---

## ⚙️ Requirements

- **Compiler**: C++11 or later (e.g., `GCC`, `MSVC`, `Clang`)  
- **Library**: [Raylib](https://www.raylib.com/) (must be installed and linked)  
- **Operating System**: Windows, macOS, or Linux  
- **Dependencies**: OpenGL, OpenAL, and others as required by Raylib

---

## 🔧 Installation

### 1. Install Raylib

Follow the [Raylib installation guide](https://www.raylib.com/) for your platform.  
Ensure it's properly installed and linked in your build environment.

---

### 2. Clone or Download

Clone this repository or download the source files manually.  
Make sure you have the following structure:
<pre lang="markdown"> ``` CheesyChess/ ├── Source.cpp └── resources/ ├── loading.wav ├── button_click.wav └── move.wav ``` </pre>

---

### 3. Build the Project

Compile `Source.cpp` using your preferred C++ compiler.  
Example with `g++`:

```bash
g++ Source.cpp -o CheesyChess -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

⚠️ Adjust the linker flags based on your operating system.

### 4. Ensure Resources
Make sure the resources/ folder (containing loading.wav, button_click.wav, and move.wav) is in the same directory as the compiled binary.

### 5. Run the game
```bash
./CheesyChess
```
### 🕹️ How to Play
## 🎮 Controls
- Left Click: Select a piece or move it to a valid square
- Right Click: Deselect a selected piece
- ESC: Return to the main menu

### 🧭 Game Flow

- Start at the Loading Screen
- Transition to the Main Menu:
- Choose Play to start a game
- Select Settings to adjust options
# 2. View Achievements
# In-game:
- Alternate turns between White and Black
- See status messages like:
- White to move
- Check
- Checkmate
- Pawn Promotion: Choose from Queen, Rook, Knight, or Bishop when a pawn reaches the last rank.

## 🗂️ Project Structure

| File/Folder          | Description                          |
|----------------------|--------------------------------------|
| `Source.cpp`         | Main source file for game logic, UI  |
| `resources/`         | Sound files for loading and in-game FX |
| `loading.wav`        | Played on loading screen            |
| `button_click.wav`   | Played on button interactions       |
| `move.wav`           | Played when moving pieces           |


