#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <memory>
#include "Time.h"
#include "Mover.h"
#include "CrushBlock.h"

//游戏主角色
//使用大量static constexpr是因为大部分数据都是固定数据，并且所有Player共用
class Player
{
private:
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::Vector2f Remainder;
    sf::Vector2f LiftBoost;
    Mover* attachedMover = nullptr;
    Mover* wasMover = nullptr;

    static constexpr float MaxRun = 90.f;
    static constexpr float MaxJump = -105.f;
    static constexpr float MaxFall = 160.f;
    static constexpr float gA = 1000.f;
    static constexpr float gR = 1000.f;
    static constexpr float gAir = 650.f;
    static constexpr float gG = 900.f;
    static constexpr float gS = 114.f;

    static constexpr float maxStamina = 110.f;
    static constexpr float wallGrabCost = 10.f;
    static constexpr float wallJumpCost = 27.5f;
    static constexpr float wallClimbCost = 45.45f;
    static constexpr float climbSpeed = 45.f;
    static constexpr float slideSpeed = 80.f;
    static constexpr float maxSlideSpeed = MaxFall;
    static constexpr float wallJumpSpeedY = -105.f;
    static constexpr float wallJumpSpeedX = 130.f;
    static constexpr float wallJumpDur = 0.2f;
    static constexpr float jumpDur = 0.2f;

    static constexpr float dashSpeed = 240.f;
    static constexpr float dashDuration = 0.15f;
    static constexpr float dashCooldown = 0.2f;

    static constexpr float moverDur = 0.13f;

    //体力条
    sf::RectangleShape bg, sta;


public:
    int facing = 0;
    int F = 1;
    int speedFacing = 0;
    bool Ducking = false;
    bool onGround = true;
    bool canJump = false;
    bool CisHold = false;
    bool onMover = false;
    bool grabMover = false;

    bool isDashing = false;
    int dashCharge = 1;
    float dashTimer = 0.f;
    float dashCooldownTimer = 0.f;
    sf::Vector2f dashDir = { 0.f, 0.f };

    bool canGrab = false;
    bool isGrabbing = false;
    bool isSliding = false;
    bool checkZ = true;
    int wallGrabDir = 0; //1:Right, -1:Left
    float stamina = 110.f;

    bool isWallJumping = false;
    bool isJumpingWall = false;
    bool checkWall = false;
    float wallJumpTimer = 0.f;

    bool isJumping = false;
    bool isSpring = false;
    float jumpTimer = 0.f;
    float gCoe = 1.f;

    float moverTimer = 0.f;

    sf::Sprite sprite;

    sf::SoundBuffer DeadBuffer;
    std::unique_ptr<sf::Sound> Dead;

    Player(const sf::Texture& Tex);
    void DrawP(sf::RenderWindow& Win);
    sf::Vector2f getPositionP() const { return Position; }
    void setPosition(sf::Vector2f spawnPoint) { Position = spawnPoint; }
    Mover* getAttached() { return attachedMover; }

    void MoveX(int facing, float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes);
    void MoveY(float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes);
    void Jump();
    void GrabWallJump();
    void WallJump();
    void CheckGrab(const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes);
    void Dash(sf::Vector2f dir);
    void ApplyG(float dt);
    void UpdateP(const float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<sf::RectangleShape>& springs, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes);

    void Die(const sf::Vector2f& spawnPoint);
};

#endif