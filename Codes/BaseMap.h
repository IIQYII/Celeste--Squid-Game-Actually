#ifndef BASEMAP_H
#define BASEMAP_H

#include <SFML/Graphics.hpp>
#include <vector>

class BaseMap {
protected:
    std::vector<sf::RectangleShape> platforms;
    sf::Vector2f spawnPoint;
    int transferDirection = 0; // ???????1? 2? 3? 4?
    int targetMapIndex = -1;

public:
    virtual ~BaseMap() = default;

    virtual void load(int mapID) = 0;
    virtual void draw(sf::RenderWindow& win) const;

    const std::vector<sf::RectangleShape>& getPlatforms() const { return platforms; }

    sf::Vector2f getSpawnPoint() const { return spawnPoint; }
    void setSpawnPoint(const sf::Vector2f& point) { spawnPoint = point; }

    void setTransfer(int direction, int targetMap) {
        transferDirection = direction;
        targetMapIndex = targetMap;
    }
    int getTransferDirection() const { return transferDirection; }
    int getTargetMapIndex() const { return targetMapIndex; }

    virtual void reset() = 0;

    void addPlatform(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color);
};

#endif
