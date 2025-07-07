#ifndef MAP_H
#define MAP_H

#include "BaseMap.h"

class Map : public BaseMap
{
private:
    std::vector<sf::RectangleShape> spikes;
    std::vector<sf::RectangleShape> ZIPs;
    std::vector<sf::RectangleShape> springs;
    std::vector<sf::RectangleShape> savePoint;

public:
    Map();

    void load(int mapID) override;
    void draw(sf::RenderWindow& win) const override;

    void addSpike(const sf::Vector2f& size, const sf::Vector2f& position);
	void addSpring(const sf::Vector2f& size, const sf::Vector2f& position);
    const std::vector<sf::RectangleShape>& getSpikes() const { return spikes; }
    const std::vector<sf::RectangleShape>& getSprings() const { return springs; }

    void clear();
};

#endif