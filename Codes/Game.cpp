#include "Game.h"
#include <iostream>

Game::Game() : player(PlayerTex)
{
    Win_W=320*5;
    Win_H=180*5;
    VM=sf::VideoMode({Win_W,Win_H});
    Title="F**K SFML";
    window=sf::RenderWindow(VM,Title);
    window.setFramerateLimit(60);
    initP("./pic/tyR.jpg");
    
    int n=5;
    maps.resize(n);

    for(int i=0;i<n;i++)
    {
        maps[i]=std::make_unique<Map>();
        maps[i]->load(i+1);
    }
}

//其实是我懒得丢到Player里面去了，好麻烦————
void Game::pollEvents()
{

    //while外面是持续检测（不会有停顿一下的效果）
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Z))
    {
        if(player.canGrab && player.checkZ)
        {
            player.isGrabbing=true;
        }
    }
    else
    {
        player.isGrabbing=false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::C))
    {
        if(player.isGrabbing)
        {
            if(!CisPressed)
            {
                //bool noXYInput=!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right);
                //bool facingInput=(player.wallGrabDir == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) || (player.wallGrabDir == -1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left));

                if(player.facing==0 || player.wallGrabDir==player.facing)
                {
                    player.GrabWallJump();
                }
                else if(player.facing!=0 && player.wallGrabDir!=player.facing)
                {
                    player.WallJump();
                }
            }
        }
        else
        {
            if(!CisPressed)
            {
                if(player.wallGrabDir!=0 && !player.onGround)
                {
                    player.WallJump();
                }
                else
                {
                    player.Jump();
                }
            }
        }
        CisPressed=true;
        player.CisHold=true;
    }
    else
    {
        CisPressed=false;
        player.CisHold=false;
    }
    
    while(const std::optional event=window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
        {
            window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::X))
        {
            sf::Vector2f dashDir={0.f,0.f};
            bool hasDashDirInput=false;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            {
                dashDir.x=1.f;
                hasDashDirInput=true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            {
                dashDir.x=-1.f;
                hasDashDirInput=true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
            {
                dashDir.y=-1.f;
                hasDashDirInput=true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
            {
                dashDir.y=1.f;
                hasDashDirInput=true;
            }

            if(!hasDashDirInput)
            {
                dashDir.x=static_cast<float>(player.F);
            }

            float length=std::sqrt(dashDir.x*dashDir.x+dashDir.y*dashDir.y);
            if(length>0)
            {
                dashDir.x/=length;
                dashDir.y/=length;
            }
            else
            {
                dashDir.x=1.f;
            }

            if(!XisPressed)
            {
                player.Dash(dashDir);
            }
            XisPressed=true;
        }
        else
        {
            XisPressed=false;
        }

        //为了实现左右移动 后按覆盖先按，进行“栈”操作（vector<sf::Keyboard::Scancode> pressedKeys;）
        //以及这边也可以兼职判断某个按键是否按下（单次检测？）
        if(auto* key=event->getIf<sf::Event::KeyPressed>())
        {
            auto code=key->scancode;
            if((code==sf::Keyboard::Scancode::Left || key->scancode==sf::Keyboard::Scancode::Right) && std::find(pressedKeys.begin(),pressedKeys.end(),code)==pressedKeys.end())
            {
                pressedKeys.push_back(code);
            }
            
            if(key->scancode==sf::Keyboard::Scancode::Down && !player.isGrabbing)
            {
                player.Ducking=true;
            }
        }

        //判断某个键是否松开，让先松开的左右键出栈
        if(auto* key=event->getIf<sf::Event::KeyReleased>())
        {
            auto code=key->scancode;
            auto it=std::find(pressedKeys.begin(),pressedKeys.end(),code);
            if(it!=pressedKeys.end())
            {
                pressedKeys.erase(it);
            }

            if(key->scancode==sf::Keyboard::Scancode::Down)
            {
                player.Ducking=false;
            }
        }
    }

    if(!player.isDashing)
    {
        //实现左右移动 后按覆盖先按
        if(!pressedKeys.empty())
        {
            auto code=pressedKeys.back();
            if(code==sf::Keyboard::Scancode::Right)
            {
                if(!player.Ducking)
                    UpdateTex("./pic/tyR.jpg");
                else
                    UpdateTex("./pic/DuckingtyR.jpg");
                player.facing=1;
                player.F=1;
            }
            else if(code==sf::Keyboard::Scancode::Left)
            {
                if(!player.Ducking)
                    UpdateTex("./pic/tyL.jpg");
                else
                    UpdateTex("./pic/DuckingtyL.jpg");
                player.facing=-1;
                player.F=-1;
            }
        }
        else
        {
            if (player.wallGrabDir == 1)
            {
                UpdateTex("./pic/tyR.jpg");
                player.F = 1;
            }
            else if (player.wallGrabDir == -1)
            {
                UpdateTex("./pic/tyL.jpg");
                player.F = -1;
            }

            //有点想写一个ty.h来存储ty的各种状态（左站着右站着左蹲下右蹲下冲抓跳跑）
            if(player.Ducking && player.F==1)
            {
                UpdateTex("./pic/DuckingtyR.jpg");
            }
            if(!player.Ducking && player.F==1)
            {
                UpdateTex("./pic/tyR.jpg");
            }
            if(player.Ducking && player.F==-1)
            {
                UpdateTex("./pic/DuckingtyL.jpg");
            }
            if(!player.Ducking && player.F==-1)
            {
                UpdateTex("./pic/tyL.jpg");
            }
            player.facing=0;
        }
    }
    else
    {
        player.facing=0;
        //可以加冲刺动画
    }
}

void Game::drawMenu()
{
    sf::CircleShape mountain;
    mountain.setRadius(100.f);

    window.clear(sf::Color(64,64,64));
    window.draw(mountain);
    window.display();
}

void Game::initP(const std::string& TexFile)
{
    if(!PlayerTex.loadFromFile(TexFile))
    {
        std::cerr << "ty doesn't exist!" << std::endl;
    }
    player=Player(PlayerTex);
}

void Game::UpdateTex(const std::string& NewTexFile)
{
    if(!PlayerTex.loadFromFile(NewTexFile))
    {
        std::cerr << "ty doesn't exist!" << std::endl;
    }
    player.sprite.setTexture(PlayerTex,true);
    sf::FloatRect bounds=player.sprite.getLocalBounds();
    player.sprite.setOrigin({bounds.size.x/2.f,bounds.size.y});
}

void Game::Update()
{
    time.update();
    pollEvents();
    player.UpdateP(time.deltaTime(),maps[currentMapIndex]->getPlatforms(),static_cast<Map*>(maps[currentMapIndex].get())->getSprings());

    if (player.getPositionP().y > 200)
    {
        player.Die(maps[currentMapIndex]->getSpawnPoint());
        return;
    }

    const auto& spikes=static_cast<Map*>(maps[currentMapIndex].get())->getSpikes();
    player.sprite.setPosition(player.getPositionP()*5.f);
    for(const auto& spike : spikes)
    {
        if(player.sprite.getGlobalBounds().findIntersection(spike.getGlobalBounds()))
        {
            player.Die(maps[currentMapIndex]->getSpawnPoint());
            break;
        }
    }


    const auto& map = maps[currentMapIndex];
    int dir = map->getTransferDirection();
    int targetIdx = map->getTargetMapIndex();

    if (targetIdx < 0 || targetIdx >= maps.size()) return;

    sf::Vector2f pos = player.getPositionP(); // 单位：逻辑坐标（原图单位）
    bool shouldTransfer = false;

    switch (dir)
    {
        case 1: // 上
            shouldTransfer = pos.y < 0;break;
        /*case 2: // 下 
            shouldTransfer = pos.y > 180;break;*/
        case 3: // 左
            shouldTransfer = pos.x < 0;break;
        case 4: // 右
            shouldTransfer = pos.x > 320;break;
        default:break;
    }

    if (shouldTransfer)
    {
        currentMapIndex = targetIdx;
        player.setPosition(maps[currentMapIndex]->getSpawnPoint());
    }
}

void Game::run()
{
    while(window.isOpen())
    {
        if (gameState == GameState::MainMenu)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();

                if (event->is<sf::Event::KeyPressed>())
                {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Enter)
                        gameState = GameState::Playing;
                    if (key == sf::Keyboard::Key::Escape)
                        window.close();
                }
            }
            drawMenu();
        }
        else if (gameState == GameState::Playing)
        {
            Update();
            window.clear(sf::Color(64, 64, 64));
            maps[currentMapIndex]->draw(window);
            player.DrawP(window);
            window.display();
        }
    }
}
