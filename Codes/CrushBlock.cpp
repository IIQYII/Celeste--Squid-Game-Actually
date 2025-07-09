#include "CrushBlock.h"

CrushBlock::CrushBlock(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Texture& Tex) : sprite(Tex)
{
    setSize(size);
    setPosition(position * 5.f);
    //setFillColor(sf::Color::White);
    sprite.setPosition(position * 5.f);
    setTexture(&Tex);
    crushTimer = 0.f;
    restoreTimer = 0.f;
}

void CrushBlock::UpdateC(float dt)
{
    //当玩家踩上去后，消失延迟的消失计时器
    if (crushTiming)
    {
        crushTimer -= dt;
        if (crushTimer <= 0.f)
        {
            crushTiming = false;
            isSee = false;
        }
    }

    //消失后，重新出现的恢复计时器
    if (restoreTiming)
    {
        restoreTimer -= dt;
        if (restoreTimer <= 0.f)
        {
            restoreTiming = false;
            isSee = true;
        }
    }

    //如果有玩家踩上去，同时消失计时器不在运行，且平台可见，则启动消失计时器
    if (isRiding && !crushTiming && isSee)
    {
        crushTimer = crushDur;
        crushTiming = true;
    }

    //如果平台消失并且恢复计时器不在运行，则启动恢复计时器
    if (!isSee && !restoreTiming)
    {
        restoreTimer = restoreDur;
        restoreTiming = true;
    }
}

void CrushBlock::reset()
{
    crushTimer = 0.f;
    restoreTimer = 0.f;
    isSee = true;
}