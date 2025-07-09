#include "Game.h"
#include <iostream>
#include <fstream>

//创建游戏后，初始化玩家、地图和游戏窗口
Game::Game() : player(PlayerTex)
{
    Win_W = 320 * 5;
    Win_H = 180 * 5;
    VM = sf::VideoMode({ Win_W,Win_H });
    Title = "Mini Celeste";
    window = sf::RenderWindow(VM, Title);
    window.setFramerateLimit(60);
    initP("./pic/RRight.png");

    int n = 6;
    maps.resize(n);

    for (int i = 0; i < n; i++)
    {
        maps[i] = std::make_unique<Map>();
        maps[i]->load(i + 1);
    }

    //加载音乐
    if (!musicPrologue.openFromFile("./Music/Lena Raine - Prologue.wav")) 
    {
        std::cerr << "Failed to load prologue music!" << std::endl;
    }
    if (!musicFirstSteps.openFromFile("./Music/Lena Raine - First Steps~1.wav")) 
    {
        std::cerr << "Failed to load first_steps music!" << std::endl;
    }

    //设置音乐循环
    musicPrologue.setVolume(50.f);
    musicFirstSteps.setVolume(50.f);

    musicPrologue.setLooping(true);
    musicFirstSteps.setLooping(true);
}

//游戏事件处理
void Game::pollEvents()
{
    //按下Z键可以抓
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z))
    {
        if (player.canGrab && player.checkZ)
        {
            player.isGrabbing = true;
        }
    }
    else
    {
        player.isGrabbing = false;
    }

    //按下C键可以按条件实现普通跳跃、墙跳、抓墙跳
    //同时可以根据按下C键的时间长度决定跳跃高度
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
    {
        if (player.isGrabbing)
        {
            if (!CisPressed)
            {
                if (player.facing == 0 || player.wallGrabDir == player.facing)
                {
                    player.GrabWallJump();
                }
                else if (player.facing != 0 && player.wallGrabDir != player.facing)
                {
                    player.WallJump();
                }
            }
        }
        else
        {
            if (!CisPressed)
            {
                if (player.wallGrabDir != 0 && !player.onGround)
                {
                    player.WallJump();
                }
                else
                {
                    player.Jump();
                }
            }
        }
        CisPressed = true;
        player.CisHold = true;
    }
    else
    {
        CisPressed = false;
        player.CisHold = false;
    }

    //处理其余按键操作
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }

        //按下X键可以进行冲刺，冲刺拥有八个方向
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X))
        {
            sf::Vector2f dashDir = { 0.f,0.f };
            bool hasDashDirInput = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            {
                dashDir.x = 1.f;
                hasDashDirInput = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            {
                dashDir.x = -1.f;
                hasDashDirInput = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
            {
                dashDir.y = -1.f;
                hasDashDirInput = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
            {
                dashDir.y = 1.f;
                hasDashDirInput = true;
            }

            if (!hasDashDirInput)
            {
                dashDir.x = static_cast<float>(player.F);
            }

            float length = std::sqrt(dashDir.x * dashDir.x + dashDir.y * dashDir.y);
            if (length > 0)
            {
                dashDir.x /= length;
                dashDir.y /= length;
            }
            else
            {
                dashDir.x = 1.f;
            }

            if (!XisPressed)
            {
                player.Dash(dashDir);
            }
            XisPressed = true;
        }
        else
        {
            XisPressed = false;
        }

        //使用栈来判断左右移动，实现“后按覆盖先按”的效果 （vector<sf::Keyboard::::Scancode>）
        if (auto* key = event->getIf<sf::Event::KeyPressed>())
        {
            auto code = key->scancode;
            if ((code == sf::Keyboard::Scancode::Left || key->scancode == sf::Keyboard::Scancode::Right) && std::find(pressedKeys.begin(), pressedKeys.end(), code) == pressedKeys.end())
            {
                pressedKeys.push_back(code);
            }

            if (key->scancode == sf::Keyboard::Scancode::Down && !player.isGrabbing)
            {
                player.Ducking = true;
            }
        }

        if (auto* key = event->getIf<sf::Event::KeyReleased>())
        {
            auto code = key->scancode;
            auto it = std::find(pressedKeys.begin(), pressedKeys.end(), code);
            if (it != pressedKeys.end())
            {
                pressedKeys.erase(it);
            }

            if (key->scancode == sf::Keyboard::Scancode::Down)
            {
                player.Ducking = false;
            }
        }
    }

    if (!player.isDashing)
    {
        if (player.dashCharge == 1)
        {
            //判断角色左右移动状态
            if (!pressedKeys.empty())
            {
                auto code = pressedKeys.back();
                if (code == sf::Keyboard::Scancode::Right)
                {
                    if (!player.Ducking)
                        UpdateTex("./pic/RRight.png");
                    else
                        UpdateTex("./pic/DuckRRight.png");
                    player.facing = 1;
                    player.F = 1;
                }
                else if (code == sf::Keyboard::Scancode::Left)
                {
                    if (!player.Ducking)
                        UpdateTex("./pic/RLeft.png");
                    else
                        UpdateTex("./pic/DuckRLeft.png");
                    player.facing = -1;
                    player.F = -1;
                }
            }
            else
            {
                if (player.wallGrabDir)
                {
                    if (player.wallGrabDir == 1)
                    {
                        UpdateTex("./pic/GrabRRight.png");
                        player.F = 1;
                    }
                    else if (player.wallGrabDir == -1)
                    {
                        UpdateTex("./pic/GrabRLeft.png");
                        player.F = -1;
                    }
                }
                else {
                    if (player.Ducking && player.F == 1)
                    {
                        UpdateTex("./pic/DuckRRight.png");
                    }
                    if (!player.Ducking && player.F == 1)
                    {
                        UpdateTex("./pic/RRight.png");
                    }
                    if (player.Ducking && player.F == -1)
                    {
                        UpdateTex("./pic/DuckRLeft.png");
                    }
                    if (!player.Ducking && player.F == -1)
                    {
                        UpdateTex("./pic/RLeft.png");
                    }
                }
                player.facing = 0;
            }
        }
        else
        {
            if (!pressedKeys.empty())
            {
                auto code = pressedKeys.back();
                if (code == sf::Keyboard::Scancode::Right)
                {
                    if (!player.Ducking)
                        UpdateTex("./pic/BRight.png");
                    else
                        UpdateTex("./pic/DuckBRight.png");
                    player.facing = 1;
                    player.F = 1;
                }
                else if (code == sf::Keyboard::Scancode::Left)
                {
                    if (!player.Ducking)
                        UpdateTex("./pic/BLeft.png");
                    else
                        UpdateTex("./pic/DuckBLeft.png");
                    player.facing = -1;
                    player.F = -1;
                }
            }
            else
            {
                if (player.wallGrabDir)
                {
                    if (player.wallGrabDir == 1)
                    {
                        UpdateTex("./pic/GrabBRight.png");
                        player.F = 1;
                    }
                    else if (player.wallGrabDir == -1)
                    {
                        UpdateTex("./pic/GrabBLeft.png");
                        player.F = -1;
                    }
                }
                else {
                    if (player.Ducking && player.F == 1)
                    {
                        UpdateTex("./pic/DuckBRight.png");
                    }
                    if (!player.Ducking && player.F == 1)
                    {
                        UpdateTex("./pic/BRight.png");
                    }
                    if (player.Ducking && player.F == -1)
                    {
                        UpdateTex("./pic/DuckBLeft.png");
                    }
                    if (!player.Ducking && player.F == -1)
                    {
                        UpdateTex("./pic/BLeft.png");
                    }
                }
                player.facing = 0;
            }
        }
    }
    else
    {
        player.facing = 0;
    }

    //游戏中按下ESC可以暂停游戏
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape))
    {
        isPaused = true;
        time.pause();
        gameState = GameState::Pausing;
    }
}

//初始化游戏角色贴图
void Game::initP(const std::string& TexFile)
{
    if (!PlayerTex.loadFromFile(TexFile))
    {
        std::cerr << "ty doesn't exist!" << std::endl;
    }
    player = Player(PlayerTex);
}

//当角色处于不同状态时，更新角色贴图
void Game::UpdateTex(const std::string& NewTexFile)
{
    if (!PlayerTex.loadFromFile(NewTexFile))
    {
        std::cerr << "ty doesn't exist!" << std::endl;
    }
    player.sprite.setTexture(PlayerTex, true);
    sf::FloatRect bounds = player.sprite.getLocalBounds();
    player.sprite.setOrigin({ bounds.size.x / 2.f,bounds.size.y });
}

//游戏的主更新循环，实现物理效果、事件处理等功能
void Game::Update()
{
    time.update();
    pollEvents();

    static_cast<Map*>(maps[currentMapIndex].get())->update(time.deltaTime());
    player.UpdateP(time.deltaTime(), maps[currentMapIndex]->getPlatforms(), static_cast<Map*>(maps[currentMapIndex].get())->getSprings(), static_cast<Map*>(maps[currentMapIndex].get())->getMover(), static_cast<Map*>(maps[currentMapIndex].get())->getCrush());

    //如果玩家掉出屏幕，则判断玩家死亡
    if (player.getPositionP().y > 200)
    {
        player.Die(maps[currentMapIndex]->getSpawnPoint());
        maps[currentMapIndex]->reset();
        return;
    }

    //如果玩家碰到尖刺（Spike），则玩家死亡
    const auto& spikes = static_cast<Map*>(maps[currentMapIndex].get())->getSpikes();
    player.sprite.setPosition(player.getPositionP() * 5.f);
    for (const auto& spike : spikes)
    {
        if (player.sprite.getGlobalBounds().findIntersection(spike.getGlobalBounds()))
        {
            player.Die(maps[currentMapIndex]->getSpawnPoint());
            maps[currentMapIndex]->reset();
            break;
        }
    }

    //判断是否切换关卡
    const auto& map = maps[currentMapIndex];
    int dir = map->getTransferDirection();
    int targetIdx = map->getTargetMapIndex();

    if (targetIdx < 0 || targetIdx >= maps.size()) return;

    sf::Vector2f pos = player.getPositionP();
    bool shouldTransfer = false;

    switch (dir)
    {
    case 1: // 上
        shouldTransfer = pos.y < 0; break;
    case 3: // 左
        shouldTransfer = pos.x < 0; break;
    case 4: // 右
        shouldTransfer = pos.x > 320; break;
    default:break;
    }

    if (shouldTransfer)
    {
        currentMapIndex = targetIdx;
        player.setPosition(maps[currentMapIndex]->getSpawnPoint());
    }
}

//开始界面的绘制
void Game::drawStartScreen()
{
    sf::Font font("./word/word.ttf");
    sf::Texture texture_Start("./pic/Start.png");
   
    sf::Sprite Start(texture_Start);
    Start.setPosition({ 0.f,0.f });

    window.clear(sf::Color(32, 32, 32));
    window.draw(Start);
    window.display();
}

//菜单界面的绘制
void Game::drawMenuScreen()
{
    sf::Font font("./word/word.ttf");
    sf::Texture texture_Menu("./pic/Background.png");

    sf::Sprite Menu(texture_Menu);
    Menu.setPosition({ 0.f,0.f });

    sf::Text startGame(font, "Start Game", 60);
    startGame.setFillColor(selectedOption == "Start Game" ? sf::Color::Green : sf::Color::White);
    startGame.setPosition({ 200.f, 225.f });

    sf::Text help(font, "Help", 60);
    help.setFillColor(selectedOption == "Help" ? sf::Color::Green : sf::Color::White);
    help.setPosition({ 300.f, 375.f });

    sf::Text exitGame(font, "Exit Game", 60);
    exitGame.setFillColor(selectedOption == "Exit Game" ? sf::Color::Green : sf::Color::White);
    exitGame.setPosition({ 400.f, 525.f });

    window.clear(sf::Color(32, 32, 32));
    window.draw(Menu);
    window.draw(startGame);
    window.draw(help);
    window.draw(exitGame);
    window.display();
}

//帮助界面的绘制
void Game::drawHelpScreen()
{
    sf::Font font("./word/word.ttf");
    sf::Texture texture_Help ("./pic/Background.png");

    sf::Sprite Help(texture_Help);
    Help.setPosition({ 0.f,0.f });

    sf::Text helpTitle(font, "Help", 100);
    helpTitle.setFillColor(sf::Color::Black);
    helpTitle.setPosition({ 720.f, 100.f });

    sf::Text helpText(font, "Use arrow keys to move.\nPress C to jump.\nPress X to dash.", 60);
    helpText.setFillColor(sf::Color::Black);
    helpText.setPosition({ 440.f, 300.f });

    window.clear(sf::Color(32, 32, 32));
    window.draw(Help);
    window.draw(helpTitle);
    window.draw(helpText);
    window.display();
}

//游戏存档界面的绘制
void Game::drawLoadScreen()
{
    sf::Font font("./word/word.ttf");
    sf::Texture texture_Load("./pic/Background.png");

    sf::Sprite Load(texture_Load);
    Load.setPosition({ 0.f,0.f });

    sf::Text loadTitle(font, "Load Game", 100);
    loadTitle.setFillColor(sf::Color::Black);
    loadTitle.setPosition({ 620.f, 100.f });

    sf::Text load1(font, "Save 1", 60);
    load1.setFillColor(selectedLoadIndex == 0 ? sf::Color::Green : sf::Color::White);
    load1.setPosition({ 200.f, 275.f });

    sf::Text load2(font, "Save 2", 60);
    load2.setFillColor(selectedLoadIndex == 1 ? sf::Color::Green : sf::Color::White);
    load2.setPosition({ 300.f, 425.f });

    sf::Text load3(font, "Save 3", 60);
    load3.setFillColor(selectedLoadIndex == 2 ? sf::Color::Green : sf::Color::White);
    load3.setPosition({ 400.f, 575.f });

    window.clear(sf::Color(32, 32, 32));
    window.draw(Load);
    window.draw(loadTitle);
    window.draw(load1);
    window.draw(load2);
    window.draw(load3);
    window.display();
}

//游戏暂停界面的绘制
void Game::drawPauseScreen()
{
    sf::Font font("./word/word.ttf");

    sf::Text pauseTitle(font, "Paused", 80);
    pauseTitle.setFillColor(sf::Color::White);
    pauseTitle.setPosition({ Win_W / 2.f - pauseTitle.getGlobalBounds().size.x / 2.f, 100.f });

    sf::Text continueOption(font, "Continue", 40);
    continueOption.setFillColor(selectedPauseIndex == 0 ? sf::Color::Green : sf::Color::White);
    continueOption.setPosition({ Win_W / 2.f - continueOption.getGlobalBounds().size.x / 2.f, 200.f });

    sf::Text retryOption(font, "Retry", 40);
    retryOption.setFillColor(selectedPauseIndex == 1 ? sf::Color::Green : sf::Color::White);
    retryOption.setPosition({ Win_W / 2.f - retryOption.getGlobalBounds().size.x / 2.f, 250.f });

    sf::Text exitOption(font, "Save and Exit", 40);
    exitOption.setFillColor(selectedPauseIndex == 2 ? sf::Color::Green : sf::Color::White);
    exitOption.setPosition({ Win_W / 2.f - exitOption.getGlobalBounds().size.x / 2.f, 350.f });

    window.clear(sf::Color(32, 32, 32));
    window.draw(pauseTitle);
    window.draw(continueOption);
    window.draw(retryOption);
    window.draw(exitOption);
    window.display();
}

void Game::drawEndScreen()
{
    sf::Font font("./word/word.ttf");
    sf::Texture texture_Background_end("./pic/Background_end.jpg");

    sf::Sprite Background_end(texture_Background_end);
    Background_end.setPosition({ 0.f,0.f });

    sf::Text thanksText(font, "Thanks For Playing", 80);
    thanksText.setFillColor(sf::Color::White);
    thanksText.setStyle(sf::Text::Bold);

    thanksText.setPosition({ 620.f, 100.f });

    window.clear();
    window.draw(Background_end);
    window.draw(thanksText);
    window.display();

    while (window.isOpen())
    {
        if (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                    event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
            }
        }
    }
}

//游戏存档的实现，会将当前游戏数据保存在同文件夹的“save[saveIndex].txt”文件中
//需要在暂停界面选择“Save and Exit”
void Game::saveGame(int saveIndex)
{
    std::ofstream saveFile("save" + std::to_string(saveIndex) + ".txt",std::ios::out|std::ios::trunc);
    if (saveFile.is_open())
    {
        saveFile << currentMapIndex << std::endl;
        saveFile.close();
    }
}

//从文件中加载游戏存档
void Game::loadGame(int saveIndex)
{
    std::ifstream saveFile("save" + std::to_string(saveIndex) + ".txt");
    if (saveFile.is_open())
    {
        int savedMapIndex;
        saveFile >> savedMapIndex;

        currentMapIndex = savedMapIndex;

        player.Die(maps[currentMapIndex]->getSpawnPoint());

        currentSaveIndex = saveIndex;
        saveFile.close();
    }
    else
    {
        currentMapIndex = 0;

        player.Die(maps[currentMapIndex]->getSpawnPoint());

        currentSaveIndex = saveIndex;
        saveGame(saveIndex);
    }
}

void Game::playMusic(const std::string& musicName)
{
    if (currentMusic == musicName && isMusicPlaying) return;

    stopMusic();

    if (musicName == "prologue") {
        musicPrologue.play();
        currentMusic = "prologue";
    }
    else if (musicName == "first_steps") {
        musicFirstSteps.play();
        currentMusic = "first_steps";
    }

    isMusicPlaying = true;
}

void Game::stopMusic()
{
    if (currentMusic == "prologue") {
        musicPrologue.stop();
    }
    else if (currentMusic == "first_steps") {
        musicFirstSteps.stop();
    }

    currentMusic = "";
    isMusicPlaying = false;
}

//运行游戏，加载游戏关卡贴图
void Game::run()
{
    sf::Texture texture_Background1("./pic/Background1.jpg");
    sf::Texture texture_Background2("./pic/Background2.jpg");
    sf::Texture texture_Background3("./pic/Background3.jpg");
    sf::Texture texture_Background4("./pic/Background4.jpg");
    sf::Texture texture_Background5("./pic/Background5.jpg");
    sf::Texture texture_Background_end("./pic/Background_end.jpg");

    sf::Sprite Background1(texture_Background1);
    sf::Sprite Background2(texture_Background2);
    sf::Sprite Background3(texture_Background3);
    sf::Sprite Background4(texture_Background4);
    sf::Sprite Background5(texture_Background5);
    sf::Sprite Background_end(texture_Background_end);
    while (window.isOpen())
    {
        //根据游戏状态不同，加载不同界面
        switch (gameState)
        {
        case GameState::Start:
        case GameState::Menu:
        case GameState::Help:
        case GameState::LoadGame:
            playMusic("prologue");
            break;

        case GameState::Playing:
            playMusic("first_steps");
            break;

        case GameState::Pausing:
            break;
        }
        Background1.setPosition({ 0.f,0.f });

        if (gameState == GameState::Start)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::C)
                        gameState = GameState::Menu;
                }
            }
            drawStartScreen();
        }

        else if (gameState == GameState::Menu)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Up)
                    {
                        if (selectedOption == "Start Game")
                            selectedOption = "Exit Game";
                        else if (selectedOption == "Help")
                            selectedOption = "Start Game";
                        else if (selectedOption == "Exit Game")
                            selectedOption = "Help";
                    }
                    else if (key == sf::Keyboard::Key::Down)
                    {
                        if (selectedOption == "Start Game")
                            selectedOption = "Help";
                        else if (selectedOption == "Help")
                            selectedOption = "Exit Game";
                        else if (selectedOption == "Exit Game")
                            selectedOption = "Start Game";
                    }
                    else if (key == sf::Keyboard::Key::C)
                    {
                        if (selectedOption == "Start Game")
                            gameState = GameState::LoadGame;
                        else if (selectedOption == "Help")
                            gameState = GameState::Help;
                        else if (selectedOption == "Exit Game")
                            window.close();
                    }
                    else if (key == sf::Keyboard::Key::X)
                        gameState = GameState::Start;
                }
            }
            drawMenuScreen();
        }

        else if (gameState == GameState::LoadGame)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Up)
                        selectedLoadIndex = (selectedLoadIndex - 1 + loadOptions.size()) % loadOptions.size();
                    else if (key == sf::Keyboard::Key::Down)
                        selectedLoadIndex = (selectedLoadIndex + 1) % loadOptions.size();
                    else if (key == sf::Keyboard::Key::C)
                    {
                        loadGame(selectedLoadIndex);
                        gameState = GameState::Playing;
                    }
                    else if (key == sf::Keyboard::Key::X)
                        gameState = GameState::Menu;
                }
            }
            drawLoadScreen();
        }

        else if (gameState == GameState::Help)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::X)
                        gameState = GameState::Menu;
                }
            }
            drawHelpScreen();
        }

        else if (gameState == GameState::Playing)
        {
            if (!isPaused)
            {
                Update();
                if (currentMapIndex == 5 && player.getPositionP().x > 160)
                {
                    gameState = GameState::End;
                }
                window.clear(sf::Color(64, 64, 64));
                switch (currentMapIndex)
                {
                case 0:
                    window.draw(Background1);
                    break;
                case 1:
                    window.draw(Background2);
                    break;
                case 2:
                    window.draw(Background3);
                    break;
                case 3:
                    window.draw(Background4);
                    break;
                case 4:
                    window.draw(Background5);
                    break;
                case 5:
                    window.draw(Background_end);
                }
                maps[currentMapIndex]->draw(window);
                player.DrawP(window);
                window.display();
            }
        }

        else if (gameState == GameState::Pausing)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Up)
                    {
                        selectedPauseIndex = (selectedPauseIndex - 1 + pauseOptions.size()) % pauseOptions.size();
                    }
                    else if (key == sf::Keyboard::Key::Down)
                    {
                        selectedPauseIndex = (selectedPauseIndex + 1) % pauseOptions.size();
                    }
                    else if (key == sf::Keyboard::Key::X)
                    {
                        isPaused = false;
                        time.resume();
                        gameState = GameState::Playing;
                    }
                    else if (key == sf::Keyboard::Key::C)
                    {
                        if (selectedPauseIndex == 0)
                        {
                            isPaused = false;
                            time.resume();
                            gameState = GameState::Playing;
                        }
                        else if (selectedPauseIndex == 1)
                        {
                            player.Die(maps[currentMapIndex]->getSpawnPoint());
                            isPaused = false;
                            time.resume();
                            gameState = GameState::Playing;
                        }
                        else if (selectedPauseIndex == 2)
                        {
                            saveGame(selectedLoadIndex);
                            isPaused = false;
                            time.resume();
                            gameState = GameState::Menu;
                        }
                    }
                }
            }
            drawPauseScreen();
        }

        else if (gameState == GameState::End)
        {
            drawEndScreen();
        }
    }
    stopMusic();
}
