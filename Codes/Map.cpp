#include "Map.h"

Map::Map(){}

//加载地图
void Map::load(int mapID)
{
    BaseMap::load(mapID);
    loadTextures();

    spikes.clear();
    springs.clear();
    movers.clear();
    crushes.clear();

    if (mapID == 1)
    {
        // map1
        spawnPoint = { 23, 144 };
        setTransfer(1, 1);
        addSpike({ 40 * 5.f, 2 * 5.f }, { 40 * 5.f, 166 * 5.f });
        addSpike({ 40 * 5.f, 2 * 5.f }, { 104 * 5.f, 150 * 5.f });
        addSpike({ 32 * 5.f, 2 * 5.f }, { 176 * 5.f, 126 * 5.f });
        addSpike({ 24 * 5.f, 2 * 5.f }, { 208 * 5.f, 158 * 5.f });
        addSpike({ 2 * 5.f, 16 * 5.f }, { 270 * 5.f, 80 * 5.f });
    }

    else if (mapID == 2)
    {
        // map2
        spawnPoint = { 40, 150 };
        setTransfer(1, 2);
        addSpike({ 40 * 5.f, 2 * 5.f }, { 160 * 5.f, 110 * 5.f });
        addSpike({ 16 * 5.f, 2 * 5.f }, { 192 * 5.f, 158 * 5.f });
        addSpike({ 16 * 5.f, 2 * 5.f }, { 208 * 5.f, 166 * 5.f });
        addSpring({ 16 * 5.f, 4 * 5.f }, { 112 * 5.f, 142 * 5.f });
    }

    else if (mapID == 3)
    {
        //map3
        spawnPoint = { 40, 150 };
        setTransfer(1, 3);
        addSpike({ 24 * 5.f, 2 * 5.f }, { 184 * 5.f, 158 * 5.f });
        addSpike({ 24 * 5.f, 2 * 5.f }, { 208 * 5.f, 134 * 5.f });
        addSpike({ 40 * 5.f, 2 * 5.f }, { 232 * 5.f, 142 * 5.f });
        addSpike({ 32 * 5.f, 2 * 5.f }, { 272 * 5.f, 158 * 5.f });
        addSpike({ 8 * 5.f, 2 * 5.f }, { 304 * 5.f, 150 * 5.f });
    }

    else if (mapID == 4)
    {
        //map4
        spawnPoint = { 40, 150 };
        setTransfer(1, 4);
        addSpike({ 2 * 5.f, 32 * 5.f }, { 16 * 5.f, 112 * 5.f });
        addSpike({ 2 * 5.f, 32 * 5.f }, { 70 * 5.f, 96 * 5.f });
        addSpike({ 16 * 5.f, 2 * 5.f }, { 80 * 5.f, 110 * 5.f });
        addSpike({ 2 * 5.f, 32 * 5.f }, { 222 * 5.f, 32 * 5.f });
        addMover({ 24 * 5.f, 16 * 5.f }, { 112, 80 }, { 184, 72 }, 200.f, textures["Mover1"]);
    }

    else if (mapID == 5)
    {
        //map5
        spawnPoint = { 40, 150 };
        setTransfer(1, 5);
        addCrushBlock({ 24 * 5.f, 8 * 5.f }, { 232, 152 }, textures["CrushBlock1"]);
        addCrushBlock({ 24 * 5.f, 8 * 5.f }, { 272, 128 }, textures["CrushBlock1"]);
        addCrushBlock({ 24 * 5.f, 8 * 5.f }, { 224, 104 }, textures["CrushBlock1"]);
        addCrushBlock({ 24 * 5.f, 8 * 5.f }, { 240, 64 }, textures["CrushBlock1"]);
    }

    else if (mapID == 6)
    {
        //map6
        spawnPoint = { 50, 150 };
    }
}

//绘制地图
void Map::draw(sf::RenderWindow& Win) const
{

    BaseMap::draw(Win);
    /*for (const auto& spike : spikes)
    {
        Win.draw(spike);
    }
    for (const auto& spring : springs)
    {
        Win.draw(spring);
    }*/
    for (const auto& mover : movers)
    {
        Win.draw(mover.sprite);
    }
    for (const auto& crush : crushes)
    {
        if (crush.isSee)
        {
            Win.draw(crush.sprite);
        }
    }
}

//加载地图实体的贴图
void Map::loadTextures()
{
    if (textures.empty())
    {
        if (!textures["CrushBlock1"].loadFromFile("./pic/CrushBlock.png"))
        {
            std::cerr << "Failed to load CrushBlock.png" << std::endl;
        }
        if (!textures["Mover1"].loadFromFile("./pic/Mover.png"))
        {
            std::cerr << "Failed to load Mover.png" << std::endl;
        }
    }
}

//添加实体“刺”（Spike）
void Map::addSpike(const sf::Vector2f& size, const sf::Vector2f& position)
{
    sf::RectangleShape spike;
    spike.setSize(size);
    spike.setPosition(position);
    spike.setFillColor(sf::Color::Red);
    spikes.push_back(spike);
}

//添加实体“弹簧”（Spring）
void Map::addSpring(const sf::Vector2f& size, const sf::Vector2f& position)
{
    sf::RectangleShape spring;
    spring.setSize(size);
    spring.setPosition(position);
    spring.setFillColor(sf::Color::Green);
    springs.push_back(spring);
}

//添加实体“移动块”（Mover）
void Map::addMover(const sf::Vector2f& size, const sf::Vector2f& startPos, const sf::Vector2f& endPos, float speed, const sf::Texture& texture)
{
    Mover mover(size, startPos, endPos, speed,texture);
    movers.push_back(mover);
}

//添加实体“碎裂快”（CrushBlock）
void Map::addCrushBlock(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& texture)
{
    CrushBlock crush(size, position,texture);
    crushes.push_back(crush);
}

//移动块和碎裂块的更新
void Map::update(float dt)
{
    for (auto& mover : movers)
    {
        mover.UpdateM(dt);
    }

    for (auto& crush : crushes)
    {
        crush.UpdateC(dt);
    }
}

void Map::reset()
{
    if (!movers.empty())
        for (auto& mover : movers)
        {
            mover.reset();
        }

    if (!crushes.empty())
        for (auto& crush : crushes)
        {
            crush.reset();
        }
}

void Map::clear()
{
    platforms.clear();
    spikes.clear();
    springs.clear();
    movers.clear();
    crushes.clear();
}