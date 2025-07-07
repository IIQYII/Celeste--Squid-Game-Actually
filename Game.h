#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Time.h"
#include "Player.h"
#include "Map.h"

//To Do List

//用GameStatus实现标题画面、暂停、关卡加载

enum class GameState
{
    MainMenu,
    Playing,
    Pauseing
};

class Game
{
private:
    unsigned int Win_W,Win_H;

    sf::RenderWindow window;
    sf::VideoMode VM;
    std::string Title;
    sf::Texture PlayerTex;
    Player player;
    Time time;
    std::vector<sf::Keyboard::Scancode> pressedKeys;
    
    std::vector<std::unique_ptr<BaseMap>> maps;
    int currentMapIndex = 0;

    Time gameTime;

    bool CisPressed=false;
    bool XisPressed=false;

    void Update();
    void pollEvents();
    void initP(const std::string& TexFile);
    void UpdateTex(const std::string& NewTexFile);
    void drawMenu();

    GameState gameState=GameState::MainMenu;

public:
    Game();

    void run();
};

#endif