#ifndef MAP_H
#define MAP_H

#include "BaseMap.h"
#include "Mover.h"
#include "CrushBlock.h"
#include <map>

class Map : public BaseMap
{
private:
    std::vector<sf::RectangleShape> spikes;
    std::vector<sf::RectangleShape> ZIPs;
    std::vector<sf::RectangleShape> springs;
    std::vector<Mover> movers;
    std::vector<CrushBlock> crushes;

    std::map<std::string, sf::Texture> textures;

public:
    Map();

    void load(int mapID) override;
    void draw(sf::RenderWindow& win) const override;
    void loadTextures();
    void update(float dt);

    void addSpike(const sf::Vector2f& size, const sf::Vector2f& position);
    void addSpring(const sf::Vector2f& size, const sf::Vector2f& position);
    void addMover(const sf::Vector2f& size, const sf::Vector2f& startPos, const sf::Vector2f& endPos, float speed, const sf::Texture& texture);
    void addCrushBlock(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& texture);
    const std::vector<sf::RectangleShape>& getSpikes() const { return spikes; }
    const std::vector<sf::RectangleShape>& getSprings() const { return springs; }
    const std::vector<Mover>& getMover() const { return movers; }
    const std::vector<CrushBlock>& getCrush() const { return crushes; }

    void reset() override;
    void clear();
};

#endif