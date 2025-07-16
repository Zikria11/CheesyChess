#define _CRT_SECURE_NO_WARNINGS
#include <raylib.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

const int screenWidth = 1200;
const int screenHeight = 800;
const int boardSize = 640;
const int squareSize = boardSize / 8;
const int boardOffsetX = 50;
const int boardOffsetY = 80;

enum GameState { LOADING, MENU, GAME, SETTINGS, PROMOTION, ACHIEVEMENTS };

GameState gameState = LOADING; // Global gameState
enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, NONE };

// Settings variables
bool soundEnabled = true;
int colorScheme = 0; // 0: Beige/Brown, 1: Blue/White

struct Piece {
    int x = 0, y = 0;
    bool isWhite = false;
    bool active = false;
    PieceType type = NONE;
    bool hasMoved = false;
};

struct Move {
    int fromX, fromY, toX, toY;
    PieceType pieceType;
    bool isCapture;
    PieceType capturedType = NONE;
};

class Achievement {
public:
    string name;
    string description;
    bool unlocked = false;
    Achievement(string n, string d) : name(n), description(d) {}
};

class LoadingScreen {
private:
    float progress = 0.0f;
    int frameCounter = 0;
    Sound loadingSound;

public:
    void Init() {
        InitAudioDevice();
        loadingSound = LoadSound("resources/loading.wav");
        if (!soundEnabled) SetSoundVolume(loadingSound, 0.0f);
    }

    void Update() {
        frameCounter++;
        progress = (frameCounter / 180.0f);
        if (progress >= 1.0f) {
            gameState = MENU;
            if (soundEnabled) StopSound(loadingSound);
        }
    }

    void Draw() {
        ClearBackground(RAYWHITE);
        DrawText("CHEESY", screenWidth / 2 - 120, screenHeight / 2 - 100, 48, GOLD);
        DrawText("CHESS", screenWidth / 2 - 80, screenHeight / 2 - 40, 48, GREEN);
        int barWidth = 300, barHeight = 20;
        int barX = screenWidth / 2 - barWidth / 2, barY = screenHeight / 2 + 50;
        Rectangle barRect = { (float)barX, (float)barY, (float)barWidth, (float)barHeight };
        DrawRectangleRounded(barRect, 0.3f, 8, DARKGRAY);
        DrawRectangleRounded({ (float)barX, (float)barY, barWidth * progress, (float)barHeight }, 0.3f, 8, GOLD);
        DrawRectangleRoundedLinesEx(barRect, 0.3f, 8, 2.0f, WHITE);
        DrawText("Loading...", screenWidth / 2 - 50, screenHeight / 2 + 100, 20, LIGHTGRAY);
        DrawText("v1.0", screenWidth - 50, screenHeight - 30, 16, GRAY);
    }

    void Unload() {
        UnloadSound(loadingSound);
        CloseAudioDevice();
    }
};

class MenuScreen {
private:
    struct Button {
        Rectangle rect;
        string text;
        Color color;
        Color hoverColor;
        bool isHovered = false;
        Sound clickSound;
    };
    vector<Button> buttons;

public:
    void Init() {
        buttons.clear();
        buttons.push_back({ {screenWidth / 2 - 150, 300, 300, 60}, "Play", GREEN, LIME, false, LoadSound("resources/button_click.wav") });
        buttons.push_back({ {screenWidth / 2 - 150, 380, 300, 60}, "Settings", BLUE, SKYBLUE, false, LoadSound("resources/button_click.wav") });
        buttons.push_back({ {screenWidth / 2 - 150, 460, 300, 60}, "Achievements", PURPLE, MAGENTA, false, LoadSound("resources/button_click.wav") });
        buttons.push_back({ {screenWidth / 2 - 150, 540, 300, 60}, "Exit", RED, MAROON, false, LoadSound("resources/button_click.wav") });
        if (!soundEnabled) {
            for (auto& button : buttons) SetSoundVolume(button.clickSound, 0.0f);
        }
    }

    void Update() {
        Vector2 mouse = GetMousePosition();
        for (auto& button : buttons) {
            button.isHovered = CheckCollisionPointRec(mouse, button.rect);
            if (button.isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (soundEnabled) PlaySound(button.clickSound);
                if (button.text == "Play") gameState = GAME;
                else if (button.text == "Settings") gameState = SETTINGS;
                else if (button.text == "Achievements") gameState = ACHIEVEMENTS;
                else if (button.text == "Exit") CloseWindow();
            }
        }
    }

    void Draw() {
        ClearBackground(DARKGRAY);
        for (int i = 0; i < screenWidth; i += 100) {
            for (int j = 0; j < screenHeight; j += 100) {
                DrawRectangle(i, j, 50, 50, Color{ 35, 35, 35, 255 });
            }
        }
        DrawText("CHEESY CHESS", screenWidth / 2 - 200, 150, 60, GOLD);
        DrawText("Master the Game", screenWidth / 2 - 120, 220, 24, LIGHTGRAY);
        for (const auto& button : buttons) {
            float scale = button.isHovered ? 1.05f : 1.0f;
            Rectangle scaledRect = {
                button.rect.x - (button.rect.width * scale - button.rect.width) / 2,
                button.rect.y - (button.rect.height * scale - button.rect.height) / 2,
                button.rect.width * scale,
                button.rect.height * scale
            };
            Color currentColor = button.isHovered ? button.hoverColor : button.color;
            DrawRectanglePro(scaledRect, { 0, 0 }, 0.0f, currentColor);
            DrawRectangleLinesEx(scaledRect, 2, WHITE);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), button.text.c_str(), 24, 0);
            DrawText(button.text.c_str(),
                scaledRect.x + scaledRect.width / 2 - textSize.x / 2,
                scaledRect.y + scaledRect.height / 2 - textSize.y / 2,
                24, WHITE);
        }
    }

    void Unload() {
        for (auto& button : buttons) UnloadSound(button.clickSound);
    }
};

class SettingsScreen {
private:
    struct Button {
        Rectangle rect;
        string text;
        Color color;
        Color hoverColor;
        bool isHovered = false;
        Sound clickSound;
    };
    vector<Button> buttons;

public:
    void Init() {
        buttons.clear();
        buttons.push_back({ {screenWidth / 2 - 150, 300, 300, 60}, soundEnabled ? "Sound: On" : "Sound: Off", GREEN, LIME, false, LoadSound("resources/button_click.wav") });
        buttons.push_back({ {screenWidth / 2 - 150, 380, 300, 60}, colorScheme == 0 ? "Color: Beige/Brown" : "Color: Blue/White", BLUE, SKYBLUE, false, LoadSound("resources/button_click.wav") });
        buttons.push_back({ {screenWidth / 2 - 150, 460, 300, 60}, "Back", RED, MAROON, false, LoadSound("resources/button_click.wav") });
        if (!soundEnabled) {
            for (auto& button : buttons) SetSoundVolume(button.clickSound, 0.0f);
        }
    }

    void Update() {
        Vector2 mouse = GetMousePosition();
        for (auto& button : buttons) {
            button.isHovered = CheckCollisionPointRec(mouse, button.rect);
            if (button.isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (soundEnabled) PlaySound(button.clickSound);
                if (button.text.find("Sound") != string::npos) {
                    soundEnabled = !soundEnabled;
                    button.text = soundEnabled ? "Sound: On" : "Sound: Off";
                    for (auto& b : buttons) SetSoundVolume(b.clickSound, soundEnabled ? 1.0f : 0.0f);
                }
                else if (button.text.find("Color") != string::npos) {
                    colorScheme = (colorScheme + 1) % 2;
                    button.text = colorScheme == 0 ? "Color: Beige/Brown" : "Color: Blue/White";
                }
                else if (button.text == "Back") gameState = MENU;
            }
        }
        if (IsKeyPressed(KEY_ESCAPE)) gameState = MENU;
    }

    void Draw() {
        ClearBackground(DARKGRAY);
        DrawText("Settings", screenWidth / 2 - 100, 150, 40, WHITE);
        for (const auto& button : buttons) {
            float scale = button.isHovered ? 1.05f : 1.0f;
            Rectangle scaledRect = {
                button.rect.x - (button.rect.width * scale - button.rect.width) / 2,
                button.rect.y - (button.rect.height * scale - button.rect.height) / 2,
                button.rect.width * scale,
                button.rect.height * scale
            };
            Color currentColor = button.isHovered ? button.hoverColor : button.color;
            DrawRectanglePro(scaledRect, { 0, 0 }, 0.0f, currentColor);
            DrawRectangleLinesEx(scaledRect, 2, WHITE);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), button.text.c_str(), 24, 0);
            DrawText(button.text.c_str(),
                scaledRect.x + scaledRect.width / 2 - textSize.x / 2,
                scaledRect.y + scaledRect.height / 2 - textSize.y / 2,
                24, WHITE);
        }
        DrawText("Press ESC to go back", 50, screenHeight - 50, 20, GRAY);
    }

    void Unload() {
        for (auto& button : buttons) UnloadSound(button.clickSound);
    }
};

class AchievementsScreen {
private:
    vector<Achievement> achievements;
    struct Button {
        Rectangle rect;
        string text;
        Color color;
        Color hoverColor;
        bool isHovered = false;
        Sound clickSound;
    };
    Button backButton;

public:
    void Init(vector<Achievement>& ach) {
        achievements = ach;
        backButton = { {screenWidth / 2 - 150, screenHeight - 100, 300, 60}, "Back", RED, MAROON, false, LoadSound("resources/button_click.wav") };
        if (!soundEnabled) SetSoundVolume(backButton.clickSound, 0.0f);
    }

    void Update() {
        Vector2 mouse = GetMousePosition();
        backButton.isHovered = CheckCollisionPointRec(mouse, backButton.rect);
        if (backButton.isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (soundEnabled) PlaySound(backButton.clickSound);
            gameState = MENU;
        }
        if (IsKeyPressed(KEY_ESCAPE)) gameState = MENU;
    }

    void Draw() {
        ClearBackground(DARKGRAY);
        DrawText("Achievements", screenWidth / 2 - 120, 100, 40, WHITE);
        int y = 150;
        for (const auto& ach : achievements) {
            string text = ach.name + (ach.unlocked ? " (Unlocked)" : " (Locked)");
            Color color = ach.unlocked ? GREEN : GRAY;
            DrawText(text.c_str(), screenWidth / 2 - 200, y, 20, color);
            DrawText(ach.description.c_str(), screenWidth / 2 - 200, y + 25, 16, LIGHTGRAY);
            y += 60;
        }
        float scale = backButton.isHovered ? 1.05f : 1.0f;
        Rectangle scaledRect = {
            backButton.rect.x - (backButton.rect.width * scale - backButton.rect.width) / 2,
            backButton.rect.y - (backButton.rect.height * scale - backButton.rect.height) / 2,
            backButton.rect.width * scale,
            backButton.rect.height * scale
        };
        DrawRectanglePro(scaledRect, { 0, 0 }, 0.0f, backButton.isHovered ? backButton.hoverColor : backButton.color);
        DrawRectangleLinesEx(scaledRect, 2, WHITE);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), backButton.text.c_str(), 24, 0);
        DrawText(backButton.text.c_str(),
            scaledRect.x + scaledRect.width / 2 - textSize.x / 2,
            scaledRect.y + scaledRect.height / 2 - textSize.y / 2,
            24, WHITE);
        DrawText("Press ESC to go back", 50, screenHeight - 50, 20, GRAY);
    }

    void Unload() {
        UnloadSound(backButton.clickSound);
    }
};

class Board {
public:
    void Draw() {
        DrawRectangle(boardOffsetX - 10, boardOffsetY - 10, boardSize + 20, boardSize + 20, DARKBROWN);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Color color = ((i + j) % 2 == 0) ?
                    (colorScheme == 0 ? BEIGE : WHITE) :
                    (colorScheme == 0 ? BROWN : BLUE);
                DrawRectangle(boardOffsetX + i * squareSize, boardOffsetY + j * squareSize, squareSize, squareSize, color);
            }
        }
        for (int i = 0; i < 8; i++) {
            char file = 'a' + i;
            DrawText(TextFormat("%c", file), boardOffsetX + i * squareSize + squareSize / 2 - 5, boardOffsetY + boardSize + 5, 16, DARKGRAY);
            char rank = '8' - i;
            DrawText(TextFormat("%c", rank), boardOffsetX - 20, boardOffsetY + i * squareSize + squareSize / 2 - 8, 16, DARKGRAY);
        }
    }
};

class ChessGame {
private:
    Board board;
    Piece pieces[32] = {};
    int selectedPiece = -1;
    bool whiteTurn = true;
    string gameStatus = "White to move";
    int moveCount = 0;
    Sound moveSound;
    int promotionPieceIndex = -1;
    vector<Rectangle> promotionButtons;
    vector<PieceType> promotionOptions = { QUEEN, ROOK, KNIGHT, BISHOP };
    vector<Move> moveHistory;
    vector<Achievement> achievements;
    int movesWithoutCapture = 0;
    bool gameEnded = false;
    int lastPawnDoubleMoveTurn = -1;
    int enPassantTargetX = -1, enPassantTargetY = -1;

public:
    void Init() {
        int index = 0;
        const PieceType backRow[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
        moveSound = LoadSound("resources/move.wav");
        if (!soundEnabled) SetSoundVolume(moveSound, 0.0f);
        for (int i = 0; i < 8; i++) {
            pieces[index++] = CreatePiece(backRow[i], i, 7, true);
            pieces[index++] = CreatePiece(PAWN, i, 6, true);
            pieces[index++] = CreatePiece(backRow[i], i, 0, false);
            pieces[index++] = CreatePiece(PAWN, i, 1, false);
        }
        whiteTurn = true;
        selectedPiece = -1;
        promotionPieceIndex = -1;
        gameStatus = "White to move";
        moveCount = 0;
        moveHistory.clear();
        movesWithoutCapture = 0;
        gameEnded = false;
        lastPawnDoubleMoveTurn = -1;
        enPassantTargetX = -1;
        enPassantTargetY = -1;
        achievements.clear();
        achievements.push_back(Achievement("Marshall Defense", "Play 1. d4 d5 2. c4 Nf6 3. cxd5 Nxd5 4. e4 Nf6 5. Nc3 e6"));
        achievements.push_back(Achievement("Italian Game", "Play 1. e4 e5 2. Nf3 Nc6 3. Bc4"));
        achievements.push_back(Achievement("Sicilian Defense", "Play 1. e4 c5"));
        achievements.push_back(Achievement("First Checkmate", "Win a game by checkmate"));
        achievements.push_back(Achievement("Speedy Victory", "Win a game in under 10 moves"));
        achievements.push_back(Achievement("Pacifist", "Complete 10 moves without capturing"));
        achievements.push_back(Achievement("Pawn Power", "Promote a pawn to a queen"));
    }

    Piece CreatePiece(PieceType type, int x, int y, bool isWhite) {
        Piece piece;
        piece.x = x;
        piece.y = y;
        piece.isWhite = isWhite;
        piece.active = true;
        piece.type = type;
        piece.hasMoved = false;
        return piece;
    }

    bool IsValidMove(int fromIndex, int toX, int toY) {
        if (toX < 0 || toX >= 8 || toY < 0 || toY >= 8 || gameEnded) return false;
        Piece& piece = pieces[fromIndex];
        if (piece.isWhite != whiteTurn) return false;

        // Check if destination has same color piece
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].x == toX && pieces[i].y == toY && pieces[i].isWhite == piece.isWhite) {
                return false;
            }
        }

        int dx = toX - piece.x;
        int dy = toY - piece.y;

        switch (piece.type) {
        case PAWN:
            if (piece.isWhite) {
                if (dx == 0 && dy == -1 && !IsPieceAt(toX, toY)) return true;
                if (dx == 0 && dy == -2 && !piece.hasMoved && !IsPieceAt(toX, toY) && !IsPieceAt(toX, toY + 1)) return true;
                if (dy == -1 && abs(dx) == 1 && IsOpponentPieceAt(toX, toY, piece.isWhite)) return true;
                if (dy == -1 && abs(dx) == 1 && toX == enPassantTargetX && toY == enPassantTargetY && lastPawnDoubleMoveTurn == moveCount) return true;
            }
            else {
                if (dx == 0 && dy == 1 && !IsPieceAt(toX, toY)) return true;
                if (dx == 0 && dy == 2 && !piece.hasMoved && !IsPieceAt(toX, toY) && !IsPieceAt(toX, toY - 1)) return true;
                if (dy == 1 && abs(dx) == 1 && IsOpponentPieceAt(toX, toY, piece.isWhite)) return true;
                if (dy == 1 && abs(dx) == 1 && toX == enPassantTargetX && toY == enPassantTargetY && lastPawnDoubleMoveTurn == moveCount) return true;
            }
            return false;
        case ROOK:
            return (dx == 0 || dy == 0) && IsPathClear(piece.x, piece.y, toX, toY);
        case KNIGHT:
            return (abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2);
        case BISHOP:
            return (abs(dx) == abs(dy)) && IsPathClear(piece.x, piece.y, toX, toY);
        case QUEEN:
            return (dx == 0 || dy == 0 || abs(dx) == abs(dy)) && IsPathClear(piece.x, piece.y, toX, toY);
        case KING:
            if (abs(dx) <= 1 && abs(dy) <= 1) return true;
            // Castling
            if (!piece.hasMoved && dy == 0 && abs(dx) == 2 && !IsInCheck(piece.isWhite)) {
                int rookX = dx > 0 ? 7 : 0;
                int rookIndex = -1;
                for (int i = 0; i < 32; i++) {
                    if (pieces[i].active && pieces[i].type == ROOK && pieces[i].x == rookX && pieces[i].y == piece.y && !pieces[i].hasMoved) {
                        rookIndex = i;
                        break;
                    }
                }
                if (rookIndex == -1) return false;
                if (!IsPathClear(piece.x, piece.y, rookX, piece.y)) return false;
                int kingStep = dx > 0 ? 1 : -1;
                for (int x = piece.x + kingStep; x != toX + kingStep; x += kingStep) {
                    Piece tempKing = piece;
                    tempKing.x = x;
                    pieces[fromIndex] = tempKing;
                    if (IsInCheck(piece.isWhite)) {
                        pieces[fromIndex] = piece;
                        return false;
                    }
                    pieces[fromIndex] = piece;
                }
                return true;
            }
            return false;
        }
        return false;
    }

    bool IsPieceAt(int x, int y) {
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].x == x && pieces[i].y == y) return true;
        }
        return false;
    }

    bool IsOpponentPieceAt(int x, int y, bool isWhite) {
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].x == x && pieces[i].y == y && pieces[i].isWhite != isWhite) return true;
        }
        return false;
    }

    bool IsPathClear(int fromX, int fromY, int toX, int toY) {
        int dx = toX - fromX;
        int dy = toY - fromY;
        int steps = std::max(abs(dx), abs(dy));
        if (steps == 0) return true;
        int stepX = dx == 0 ? 0 : dx / abs(dx);
        int stepY = dy == 0 ? 0 : dy / abs(dy);
        for (int i = 1; i < steps; i++) {
            if (IsPieceAt(fromX + i * stepX, fromY + i * stepY)) return false;
        }
        return true;
    }

    bool IsInCheck(bool isWhite) {
        int kingIndex = -1;
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].type == KING && pieces[i].isWhite == isWhite) {
                kingIndex = i;
                break;
            }
        }
        if (kingIndex == -1) return false;
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].isWhite != isWhite) {
                if (IsValidMove(i, pieces[kingIndex].x, pieces[kingIndex].y)) return true;
            }
        }
        return false;
    }

    bool IsCheckmate(bool isWhite) {
        if (!IsInCheck(isWhite)) return false;
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].isWhite == isWhite) {
                int fromX = pieces[i].x, fromY = pieces[i].y;
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (IsValidMove(i, x, y) && IsLegalMove(i, x, y)) return false;
                    }
                }
            }
        }
        return true;
    }

    bool IsStalemate(bool isWhite) {
        if (IsInCheck(isWhite)) return false; 

        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].isWhite == isWhite) {
                int fromX = pieces[i].x, fromY = pieces[i].y;
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (IsValidMove(i, x, y) && IsLegalMove(i, x, y)) {
                            return false;  // Found at least one legal move
                        }
                    }
                }
            }
        }
        return true;  // No legal moves found
    }

    bool IsLegalMove(int pieceIndex, int toX, int toY) {
        Piece temp = pieces[pieceIndex];
        bool captured = false;
        int capturedIndex = -1;
        PieceType capturedType = NONE;
        for (int i = 0; i < 32; i++) {
            if (pieces[i].active && pieces[i].x == toX && pieces[i].y == toY) {
                captured = true;
                capturedIndex = i;
                capturedType = pieces[i].type;
                pieces[i].active = false;
            }
        }
        bool isEnPassant = (pieces[pieceIndex].type == PAWN && toX == enPassantTargetX && toY == enPassantTargetY && lastPawnDoubleMoveTurn == moveCount);
        int enPassantCapturedIndex = -1;
        if (isEnPassant) {
            int capturedPawnY = pieces[pieceIndex].isWhite ? toY + 1 : toY - 1;
            for (int i = 0; i < 32; i++) {
                if (pieces[i].active && pieces[i].x == toX && pieces[i].y == capturedPawnY) {
                    capturedIndex = i;
                    capturedType = pieces[i].type;
                    pieces[i].active = false;
                    enPassantCapturedIndex = i;
                    break;
                }
            }
        }
        pieces[pieceIndex].x = toX;
        pieces[pieceIndex].y = toY;
        bool inCheck = IsInCheck(pieces[pieceIndex].isWhite);
        pieces[pieceIndex] = temp;
        if (captured) pieces[capturedIndex].active = true;
        if (isEnPassant && enPassantCapturedIndex != -1) pieces[enPassantCapturedIndex].active = true;
        return !inCheck;
    }

    void CheckAchievements(const Move& move) {
        moveHistory.push_back(move);
        if (move.isCapture) movesWithoutCapture = 0;
        else movesWithoutCapture++;
        if (movesWithoutCapture >= 10) {
            for (auto& ach : achievements) {
                if (ach.name == "Pacifist") ach.unlocked = true;
            }
        }
        if (move.pieceType == PAWN && move.toY == (move.pieceType == PAWN && pieces[move.fromX + move.fromY * 8].isWhite ? 0 : 7)) {
            for (auto& ach : achievements) {
                if (ach.name == "Pawn Power") ach.unlocked = true;
            }
        }
        if (moveCount <= 10 && gameEnded && gameStatus.find("checkmate") != string::npos) {
            for (auto& ach : achievements) {
                if (ach.name == "Speedy Victory") ach.unlocked = true;
            }
        }
        if (gameEnded && gameStatus.find("checkmate") != string::npos) {
            for (auto& ach : achievements) {
                if (ach.name == "First Checkmate") ach.unlocked = true;
            }
        }
        // Check for openings (simplified: check move sequence)
        if (moveHistory.size() >= 5) {
            // Marshall Defense: 1. d4 d5 2. c4 Nf6 3. cxd5 Nxd5 4. e4 Nf6 5. Nc3 e6
            if (moveHistory.size() >= 10 &&
                moveHistory[0].pieceType == PAWN && moveHistory[0].fromX == 3 && moveHistory[0].fromY == 6 && moveHistory[0].toY == 4 && // d4
                moveHistory[1].pieceType == PAWN && moveHistory[1].fromX == 3 && moveHistory[1].fromY == 1 && moveHistory[1].toY == 3 && // d5
                moveHistory[2].pieceType == PAWN && moveHistory[2].fromX == 2 && moveHistory[2].fromY == 6 && moveHistory[2].toY == 4 && // c4
                moveHistory[3].pieceType == KNIGHT && moveHistory[3].fromX == 6 && moveHistory[3].fromY == 0 && moveHistory[3].toX == 5 && moveHistory[3].toY == 2 && // Nf6
                moveHistory[4].pieceType == PAWN && moveHistory[4].fromX == 2 && moveHistory[4].fromY == 4 && moveHistory[4].toX == 3 && moveHistory[4].toY == 3 && moveHistory[4].isCapture && // cxd5
                moveHistory[5].pieceType == KNIGHT && moveHistory[5].fromX == 5 && moveHistory[5].fromY == 2 && moveHistory[5].toX == 3 && moveHistory[5].toY == 3 && // Nxd5
                moveHistory[6].pieceType == PAWN && moveHistory[6].fromX == 4 && moveHistory[6].fromY == 6 && moveHistory[6].toY == 4 && // e4
                moveHistory[7].pieceType == KNIGHT && moveHistory[7].fromX == 3 && moveHistory[7].fromY == 3 && moveHistory[7].toX == 5 && moveHistory[7].toY == 2 && // Nf6
                moveHistory[8].pieceType == KNIGHT && moveHistory[8].fromX == 2 && moveHistory[8].fromY == 7 && moveHistory[8].toX == 2 && moveHistory[8].toY == 5 && // Nc3
                moveHistory[9].pieceType == PAWN && moveHistory[9].fromX == 4 && moveHistory[9].fromY == 1 && moveHistory[9].toY == 3) { // e6
                for (auto& ach : achievements) {
                    if (ach.name == "Marshall Defense") ach.unlocked = true;
                }
            }
        }
        if (moveHistory.size() >= 5) {
            // Italian Game: 1. e4 e5 2. Nf3 Nc6 3. Bc4
            if (moveHistory[0].pieceType == PAWN && moveHistory[0].fromX == 4 && moveHistory[0].fromY == 6 && moveHistory[0].toY == 4 &&
                moveHistory[1].pieceType == PAWN && moveHistory[1].fromX == 4 && moveHistory[1].fromY == 1 && moveHistory[1].toY == 3 &&
                moveHistory[2].pieceType == KNIGHT && moveHistory[2].fromX == 6 && moveHistory[2].fromY == 7 && moveHistory[2].toX == 5 && moveHistory[2].toY == 5 &&
                moveHistory[3].pieceType == KNIGHT && moveHistory[3].fromX == 2 && moveHistory[3].fromY == 0 && moveHistory[3].toX == 2 && moveHistory[3].toY == 2 &&
                moveHistory[4].pieceType == BISHOP && moveHistory[4].fromX == 5 && moveHistory[4].fromY == 7 && moveHistory[4].toX == 2 && moveHistory[4].toY == 4) {
                for (auto& ach : achievements) {
                    if (ach.name == "Italian Game") ach.unlocked = true;
                }
            }
        }
        if (moveHistory.size() >= 2) {
            // Sicilian Defense: 1. e4 c5
            if (moveHistory[0].pieceType == PAWN && moveHistory[0].fromX == 4 && moveHistory[0].fromY == 6 && moveHistory[0].toY == 4 &&
                moveHistory[1].pieceType == PAWN && moveHistory[1].fromX == 2 && moveHistory[1].fromY == 1 && moveHistory[1].toY == 3) {
                for (auto& ach : achievements) {
                    if (ach.name == "Sicilian Defense") ach.unlocked = true;
                }
            }
        }
    }

    void PromotePawn(int pieceIndex, PieceType newType) {
        pieces[pieceIndex].type = newType;
        promotionPieceIndex = -1;
        gameState = GAME;
        whiteTurn = !whiteTurn;
        moveCount++;
        if (newType == QUEEN) {
            for (auto& ach : achievements) {
                if (ach.name == "Pawn Power") ach.unlocked = true;
            }
        }
        if (IsInCheck(!whiteTurn)) {
            if (IsCheckmate(!whiteTurn)) {
                gameStatus = whiteTurn ? "Black wins by checkmate!" : "White wins by checkmate!";
                gameEnded = true;
                for (auto& ach : achievements) {
                    if (ach.name == "First Checkmate") ach.unlocked = true;
                    if (moveCount <= 10 && ach.name == "Speedy Victory") ach.unlocked = true;
                }
            }
            else gameStatus = whiteTurn ? "Black is in check!" : "White is in check!";
        }
        else if (IsStalemate(whiteTurn)) {  
            gameStatus = "Stalemate! Game is a draw.";
            gameEnded = true;
        }
    }

    void HandleMouse() {
        if (gameEnded) return;
        if (gameState == PROMOTION) {
            Vector2 mouse = GetMousePosition();
            for (size_t i = 0; i < promotionButtons.size(); i++) {
                if (CheckCollisionPointRec(mouse, promotionButtons[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (soundEnabled) PlaySound(moveSound);
                    PromotePawn(promotionPieceIndex, promotionOptions[i]);
                    return;
                }
            }
            return;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            int mx = (int)(mouse.x - boardOffsetX) / squareSize;
            int my = (int)(mouse.y - boardOffsetY) / squareSize;
            if (mx < 0 || mx >= 8 || my < 0 || my >= 8) return;
            if (selectedPiece == -1) {
                for (int i = 0; i < 32; i++) {
                    if (pieces[i].active && pieces[i].x == mx && pieces[i].y == my && pieces[i].isWhite == whiteTurn) {
                        selectedPiece = i;
                        break;
                    }
                }
            }
            else {
                if (IsValidMove(selectedPiece, mx, my) && IsLegalMove(selectedPiece, mx, my)) {
                    if (soundEnabled) PlaySound(moveSound);
                    Move move = { pieces[selectedPiece].x, pieces[selectedPiece].y, mx, my, pieces[selectedPiece].type, false };
                    bool isEnPassant = (pieces[selectedPiece].type == PAWN && mx == enPassantTargetX && my == enPassantTargetY && lastPawnDoubleMoveTurn == moveCount);
                    for (int i = 0; i < 32; i++) {
                        if (i != selectedPiece && pieces[i].active && pieces[i].x == mx && pieces[i].y == my) {
                            move.isCapture = true;
                            move.capturedType = pieces[i].type;
                            pieces[i].active = false;
                            break;
                        }
                    }
                    if (isEnPassant) {
                        move.isCapture = true;
                        move.capturedType = PAWN;
                        int capturedPawnY = pieces[selectedPiece].isWhite ? my + 1 : my - 1;
                        for (int i = 0; i < 32; i++) {
                            if (pieces[i].active && pieces[i].x == mx && pieces[i].y == capturedPawnY) {
                                pieces[i].active = false;
                                break;
                            }
                        }
                    }
                    pieces[selectedPiece].x = mx;
                    pieces[selectedPiece].y = my;
                    pieces[selectedPiece].hasMoved = true;
                    if (pieces[selectedPiece].type == PAWN && abs(pieces[selectedPiece].y - move.fromY) == 2) {
                        lastPawnDoubleMoveTurn = moveCount + 1;
                        enPassantTargetX = mx;
                        enPassantTargetY = pieces[selectedPiece].isWhite ? my + 1 : my - 1;
                    }
                    else {
                        lastPawnDoubleMoveTurn = -1;
                        enPassantTargetX = -1;
                        enPassantTargetY = -1;
                    }
                    if (pieces[selectedPiece].type == KING && abs(mx - move.fromX) == 2) {
                        int rookFromX = mx > move.fromX ? 7 : 0;
                        int rookToX = mx > move.fromX ? move.fromX + 1 : move.fromX - 1;
                        for (int i = 0; i < 32; i++) {
                            if (pieces[i].active && pieces[i].type == ROOK && pieces[i].x == rookFromX && pieces[i].y == move.fromY) {
                                pieces[i].x = rookToX;
                                pieces[i].hasMoved = true;
                                break;
                            }
                        }
                    }
                    if (pieces[selectedPiece].type == PAWN && ((pieces[selectedPiece].isWhite && my == 0) || (!pieces[selectedPiece].isWhite && my == 7))) {
                        promotionPieceIndex = selectedPiece;
                        gameState = PROMOTION;
                        promotionButtons.clear();
                        float startX = screenWidth / 2 - 200;
                        for (int i = 0; i < 4; i++) {
                            promotionButtons.push_back({ startX + i * 100, screenHeight / 2 - 50, 80, 80 });
                        }
                    }
                    else {
                        whiteTurn = !whiteTurn;
                        moveCount++;
                        CheckAchievements(move);
                        if (IsInCheck(!whiteTurn)) {
                            if (IsCheckmate(!whiteTurn)) {
                                gameStatus = whiteTurn ? "Black wins by checkmate!" : "White wins by checkmate!";
                                gameEnded = true;
                                for (auto& ach : achievements) {
                                    if (ach.name == "First Checkmate") ach.unlocked = true;
                                    if (moveCount <= 10 && ach.name == "Speedy Victory") ach.unlocked = true;
                                }
                            }
                            else gameStatus = whiteTurn ? "Black is in check!" : "White is in check!";
                        }
                        else if (IsStalemate(whiteTurn)) {  
                            gameStatus = "Stalemate! Game is a draw.";
                            gameEnded = true;
                        }
                        else gameStatus = whiteTurn ? "White to move" : "Black to move";
                    }
                }
                selectedPiece = -1;
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) selectedPiece = -1;
    }

    void Draw() {
        ClearBackground(DARKGRAY);
        board.Draw();
        for (int i = 0; i < 32; i++) {
            if (!pieces[i].active) continue;
            int pixelX = boardOffsetX + pieces[i].x * squareSize + 5;
            int pixelY = boardOffsetY + pieces[i].y * squareSize + 5;
            DrawRectangle(pixelX, pixelY, squareSize - 10, squareSize - 10, pieces[i].isWhite ? WHITE : BLACK);
            string pieceChar = GetPieceChar(pieces[i].type);
            Color textColor = pieces[i].isWhite ? BLACK : WHITE;
            DrawText(pieceChar.c_str(), pixelX + squareSize / 2 - 8, pixelY + squareSize / 2 - 8, 16, textColor);
        }
        if (selectedPiece != -1) {
            int highlightX = boardOffsetX + pieces[selectedPiece].x * squareSize;
            int highlightY = boardOffsetY + pieces[selectedPiece].y * squareSize;
            DrawRectangleLines(highlightX, highlightY, squareSize, squareSize, YELLOW);
            DrawRectangleLines(highlightX + 1, highlightY + 1, squareSize - 2, squareSize - 2, YELLOW);
        }
        if (gameState == PROMOTION) {
            DrawRectangle(screenWidth / 2 - 220, screenHeight / 2 - 100, 440, 200, Color{ 30, 30, 30, 200 });
            DrawText("Select Promotion", screenWidth / 2 - 100, screenHeight / 2 - 80, 24, WHITE);
            for (size_t i = 0; i < promotionButtons.size(); i++) {
                bool isHovered = CheckCollisionPointRec(GetMousePosition(), promotionButtons[i]);
                DrawRectangleRec(promotionButtons[i], isHovered ? LIME : GREEN);
                DrawRectangleLinesEx(promotionButtons[i], 2, WHITE);
                string option = GetPieceChar(promotionOptions[i]);
                Vector2 textSize = MeasureTextEx(GetFontDefault(), option.c_str(), 24, 0);
                DrawText(option.c_str(),
                    promotionButtons[i].x + promotionButtons[i].width / 2 - textSize.x / 2,
                    promotionButtons[i].y + promotionButtons[i].height / 2 - textSize.y / 2,
                    24, WHITE);
            }
        }
        DrawRectangle(boardOffsetX + boardSize + 30, boardOffsetY, 300, 400, Color{ 30, 30, 30, 255 });
        DrawRectangleLines(boardOffsetX + boardSize + 30, boardOffsetY, 300, 400, WHITE);
        DrawText("Game Info", boardOffsetX + boardSize + 50, boardOffsetY + 20, 24, WHITE);
        DrawText(gameStatus.c_str(), boardOffsetX + boardSize + 50, boardOffsetY + 60, 20, LIGHTGRAY);
        DrawText(TextFormat("Move: %d", moveCount), boardOffsetX + boardSize + 50, boardOffsetY + 90, 20, LIGHTGRAY);
        DrawText("Mode: Offline", boardOffsetX + boardSize + 50, boardOffsetY + 120, 20, LIGHTGRAY);
        DrawText("Controls:", boardOffsetX + boardSize + 50, boardOffsetY + 200, 20, YELLOW);
        DrawText("Left click: Select/Move", boardOffsetX + boardSize + 50, boardOffsetY + 230, 16, LIGHTGRAY);
        DrawText("Right click: Deselect", boardOffsetX + boardSize + 50, boardOffsetY + 250, 16, LIGHTGRAY);
        DrawText("ESC: Back to menu", boardOffsetX + boardSize + 50, boardOffsetY + 270, 16, LIGHTGRAY);
        if ((gameState == GAME || gameState == PROMOTION) && IsKeyPressed(KEY_ESCAPE)) {
            gameState = MENU;
            gameEnded = true;
        }
    }

    string GetPieceChar(PieceType type) {
        switch (type) {
        case PAWN: return "P";
        case ROOK: return "R";
        case KNIGHT: return "N";
        case BISHOP: return "B";
        case QUEEN: return "Q";
        case KING: return "K";
        default: return "";
        }
    }

    vector<Achievement>& GetAchievements() { return achievements; }

    void Unload() {
        UnloadSound(moveSound);
    }
};

// Global instances
LoadingScreen loadingScreen;
MenuScreen menuScreen;
SettingsScreen settingsScreen;
AchievementsScreen achievementsScreen;
ChessGame game;
static bool gameInitialized = false;

int main() {
    InitWindow(screenWidth, screenHeight, "CheesyChess - Professional Chess Game");
    SetTargetFPS(60);
    loadingScreen.Init();
    menuScreen.Init();
    settingsScreen.Init();
    game.Init();
    achievementsScreen.Init(game.GetAchievements());
    while (!WindowShouldClose()) {
        BeginDrawing();
        switch (gameState) {
        case LOADING:
            loadingScreen.Update();
            loadingScreen.Draw();
            break;
        case MENU:
            menuScreen.Update();
            menuScreen.Draw();
            break;
        case GAME:
        case PROMOTION:
            if (!gameInitialized) {
                game.Init();
                achievementsScreen.Init(game.GetAchievements());
                gameInitialized = true;
            }
            game.HandleMouse();
            game.Draw();
            break;
        case SETTINGS:
            settingsScreen.Update();
            settingsScreen.Draw();
            break;
        case ACHIEVEMENTS:
            achievementsScreen.Update();
            achievementsScreen.Draw();
            break;
        }
        EndDrawing();
    }
    if (gameState == GAME || gameState == PROMOTION) game.Unload();
    loadingScreen.Unload();
    menuScreen.Unload();
    settingsScreen.Unload();
    achievementsScreen.Unload();
    CloseWindow();
    return 0;
}
