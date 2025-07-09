#ifndef MOVER_H
#define MOVER_H

#include <SFML/Graphics.hpp>
#include "Time.h"

enum class MoverState
{
    Idle,
    Moving,
    Return
};

//这是一个踩上去会带着你移动的平台
class Mover : public sf::RectangleShape
{
private:
    sf::Vector2f StartPos, EndPos;
    float Speed;
    MoverState State;
    int i;
    bool Activated;
    sf::Vector2f currentVelocity;
    sf::Vector2f Position;

    float moveDelayTimer;
    static constexpr float moveDelay = 0.2f;
    float returnDelayTimer;
    static constexpr float returnDelay = 0.5f;
    bool M = false;
    bool R = false;

public:
    Mover(const sf::Vector2f& size, const sf::Vector2f& startPos, const sf::Vector2f& endPos, float speed, const sf::Texture& Tex);

    void UpdateM(float dt);
    void activate();
    void deactivate() { Activated = false; }
    bool isActivated() const { return Activated; }
    MoverState getState() const { return State; }
    void reset();
    sf::Sprite sprite;
    sf::Vector2f GetCurrentVelocity() const { return currentVelocity; }
    sf::Vector2f lastPosition = { 0.f,0.f };
    sf::Vector2f getDelta() const { return Position - lastPosition; }
};

#endif