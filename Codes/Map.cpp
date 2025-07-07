#include "Map.h"

Map::Map(){}

void Map::load(int mapID) {
    BaseMap::load(mapID);
    spikes.clear();
    springs.clear();

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
    }
    else if (mapID == 5)
    {
		//map5
		spawnPoint = { 40, 150 };
		setTransfer(1, 4);

    }
}

void Map::draw(sf::RenderWindow& Win) const
{
    BaseMap::draw(Win);
    for (const auto& plat : platforms)
    {
        Win.draw(plat);
    }
    for (const auto& spike : spikes)
    {
        Win.draw(spike);
    }
    for (const auto& spring : springs)
    {
		Win.draw(spring);
    }
}

void Map::addSpike(const sf::Vector2f& size, const sf::Vector2f& position)
{
    sf::RectangleShape spike;
    spike.setSize(size);
    spike.setPosition(position);
    spike.setFillColor(sf::Color::Red); // �� ��ɫ
    spikes.push_back(spike);
}

void Map::addSpring(const sf::Vector2f& size, const sf::Vector2f& position)
{
	sf::RectangleShape spring;
	spring.setSize(size);
	spring.setPosition(position);
	spring.setFillColor(sf::Color::Green); // ���� ��ɫ
	springs.push_back(spring);
}

void Map::clear()
{
    platforms.clear();
    spikes.clear();
	springs.clear();
}