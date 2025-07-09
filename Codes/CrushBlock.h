#ifndef CRUSHBLOCK_H
#define CRUSHBLOCK_H

#include <SFML/Graphics.hpp>
#include <iostream>

//这是一个踩上去过一会会消失的平台
class CrushBlock : public sf::RectangleShape
{
private:
    static constexpr float crushDur = 0.5f;
    static constexpr float restoreDur = 1.f;

public:
    CrushBlock(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& Tex);

    float crushTimer;
    float restoreTimer;
    bool isRiding=false;
    bool isSee = true;

    bool crushTiming = false;
    bool restoreTiming = false;

    sf::Sprite sprite;

    void reset();
    void UpdateC(float dt);
};

#endif