#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Time.h"
#include "Player.h"
#include "Map.h"

enum class GameState
{
    Start,
    Menu,
    Help,
    LoadGame,
    Playing,
    Pausing,
    End
};

class Game
{
private:
    unsigned int Win_W, Win_H;

    sf::RenderWindow window;
    sf::VideoMode VM;
    std::string Title;
    sf::Texture PlayerTex;
    Player player;
    Time time;
    std::vector<sf::Keyboard::Scancode> pressedKeys;

    std::vector<std::unique_ptr<BaseMap>> maps;
    int currentMapIndex = 0;
    int currentSaveIndex = 0;

    bool CisPressed = false;
    bool XisPressed = false;

    sf::Music musicPrologue;
    sf::Music musicFirstSteps;

    bool isMusicPlaying=false;
    std::string currentMusic="";

    void Update();
    void pollEvents();
    void initP(const std::string& TexFile);
    void UpdateTex(const std::string& NewTexFile);
    void drawStartScreen();
    void drawMenuScreen();
    void drawLoadScreen();
    void drawHelpScreen();
    void drawPauseScreen();
    void drawEndScreen();
    void playMusic(const std::string& musicName);
    void stopMusic();

    std::string selectedOption = "Start Game";
    std::vector<std::string>menueOptions = { "StartGame","Help","Exit Game" };
    std::vector<std::string>loadOptions = { "Save1","Save2","Save3" };
    std::vector<std::string>pauseOptions = { "Continue","Retry","Exit" };
    int selectedLoadIndex = 0;
    int selectedPauseIndex = 0;
    bool isPaused = false;

    GameState gameState = GameState::Start;

    void saveGame(int saveIndex);
    void loadGame(int loadIndex);

public:
    Game();

    void run();
};

#endif