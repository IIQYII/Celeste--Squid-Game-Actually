#include "Mover.h"
#include <cmath>
#include <iostream>

Mover::Mover(const sf::Vector2f& size, const sf::Vector2f& startPos, const sf::Vector2f& endPos, float speed, const sf::Texture& Tex)
    : StartPos(startPos), EndPos(endPos), Speed(speed), State(MoverState::Idle), Activated(false), sprite(Tex)
{
    setSize(size);
    setPosition(startPos * 5.f);
    //setFillColor(sf::Color::Blue);
    sprite.setPosition(startPos * 5.f);
    setTexture(&Tex);
    Position = startPos;
    i = 0;
}

void Mover::UpdateM(float dt)
{
    lastPosition = Position;

    sf::Vector2f currentPos = Position;

    if (Activated && State == MoverState::Idle)
    {
        State = MoverState::Moving;
        i = 1;

        if (!M)
        {
            moveDelayTimer = moveDelay;
            M = true;
        }
    }

    //延迟运行
    if (M)
    {
        moveDelayTimer -= dt;
    }

    sf::Vector2f targetPos;

    if (State == MoverState::Moving && moveDelayTimer <= 0)
    {
        targetPos = EndPos;
        M = false;
    }

    else if (State == MoverState::Return)
        targetPos = StartPos;

    else
    {
        currentVelocity = { 0.f,0.f };
        setPosition(Position * 5.f);
        return;
    }

    //实现逐像素移动
    static sf::Vector2f Remainder = { 0.f,0.f };

    sf::Vector2f delta = targetPos - currentPos;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);


    float idx = (State == MoverState::Moving) ? 1.f : 0.2f;
    sf::Vector2f moveVec = (distance > 0.001f) ? (delta / distance) : sf::Vector2f({ 0.f,0.f });
    sf::Vector2f move = moveVec * (Speed * idx * dt) + Remainder;

    sf::Vector2i pixels = { static_cast<int>(std::round(move.x)),static_cast<int>(std::round(move.y)) };
    Remainder = move - sf::Vector2f(pixels);

    int steps = std::max(std::abs(pixels.x), std::abs(pixels.y));
    int stepX = (pixels.x > 0) ? 1 : (pixels.x < 0) ? -1 : 0;
    int stepY = (pixels.y > 0) ? 1 : (pixels.y < 0) ? -1 : 0;

    for (int i = 0; i < steps; i++)
    {
        if (i < std::abs(pixels.x))
            Position.x += stepX;

        if (i < std::abs(pixels.y))
            Position.y += stepY;
    }

    if (std::abs(Position.x - targetPos.x) < steps && std::abs(Position.y - targetPos.y) < steps)
    {
        currentVelocity = { 0.f,0.f };
        setPosition(targetPos * 5.f);
        Position = targetPos;

        if (State == MoverState::Moving)
        {
            if (!R)
            {
                returnDelayTimer = returnDelay;
                R = true;
            }
        }

        if (State == MoverState::Return)
        {
            State = MoverState::Idle;
            i = 0;
        }

        Activated = false;
    }

    else
    {
        currentVelocity = moveVec * idx * Speed;
        setPosition(Position * 5.f);
    }

    //延迟启动
    if (R)
    {
        returnDelayTimer -= dt;

        if (returnDelayTimer <= 0.f)
        {
            if (State == MoverState::Moving)
            {
                State = MoverState::Return;
                Activated = true;
                i = 2;
            }

            R = false;
        }
    }

    sprite.setPosition(Position * 5.f);
}

void Mover::activate()
{
    if (State == MoverState::Idle)
        Activated = true;
}

void Mover::reset()
{
    Position = StartPos;
    setPosition(Position * 5.f);
    Activated = false;
    State = MoverState::Idle;
    currentVelocity = { 0.f,0.f };
    moveDelayTimer = 0.f;
    returnDelayTimer = 0.f;
    M = false;
    R = false;
}