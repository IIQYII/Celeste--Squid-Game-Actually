#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"

//To Do List

//缺少死亡判定，记得补啊（）

//角色画好导入进来之后记得使用逻辑像素

//如果可以的话把踢墙跳给做了
//当 (isGrabbing && wallGrabDir!=facing) || (!isGrabbing && wallGrabDir!=0 && facing!=0)（？）
//但是我很懒

//根据按C的时长改变跳跃高度
//在抵达最高点（y轴速度小于某值）的时候，如果仍然按C（跳），则降低重力，达到“漂浮”的效果
//这个一定要做啊，不然手感很抽象

//有一个小问题，X轴碰撞检测时，角色和墙面之间会有小于1px的间隔
//我觉得是ty脑袋的问题，后续改角色贴图之后再看看

class Player
{
private:
    sf::Vector2f Position;
    sf::Vector2f Velocity;
    sf::Vector2f Remainder;

    static constexpr float MaxRun=90.f;
    static constexpr float MaxJump=-105.f;
    static constexpr float MaxFall=160.f;
    static constexpr float gA=1000.f;
    static constexpr float gR=1000.f;
    static constexpr float gAir=650.f;//*0.65f;
    static constexpr float gG=900.f;    //你蔚重力加速度是15px/s·f，而帧率60f/s，换算一下就是900px/s^2
    static constexpr float gS=114.f;    //而滑行加速度是1.9px/s·f，换算一下114px/s^2

    static constexpr float maxStamina=110.f;
    static constexpr float wallGrabCost=10.f;
    static constexpr float wallJumpCost=27.5f;
    static constexpr float wallClimbCost=45.45f;
    static constexpr float climbSpeed=45.f;
    static constexpr float slideSpeed=80.f;
    static constexpr float maxSlideSpeed=MaxFall;
    static constexpr float wallJumpSpeedY=-105.f;
    static constexpr float wallJumpSpeedX=130.f;
    static constexpr float wallJumpDur=0.2f;
    static constexpr float jumpDur=0.2f;    //varJumpTime

    static constexpr float dashSpeed=240.f;
    static constexpr float dashDuration=0.15f;
    static constexpr float dashCooldown=0.2f;

    //体力条（）
    sf::RectangleShape bg,sta;


public:
    int facing=0; //1右，0不动，-1左
    int F=1;    //上面那个facing是判断移动的朝向，这个就是纯粹的朝向
    int speedFacing=0;
    bool Ducking=false;
    bool onGround=true;
    bool canJump=false;
    bool CisHold=false;

    bool isDashing = false;
    int dashCharge=1;
    float dashTimer = 0.f;
    float dashCooldownTimer = 0.f;
    sf::Vector2f dashDir = {0.f, 0.f};

    bool canGrab=false;
    bool isGrabbing=false;
    bool isSliding=false;
    bool checkZ=true;
    int wallGrabDir=0; //1:Right, -1:Left
    float stamina=110.f;

    bool isWallJumping=false;
    bool isJumpingWall=false;
    bool checkWall=false;
    float wallJumpTimer=0.f;

    bool isJumping=false;
    bool isSpring=false;
    float jumpTimer=0.f;
    float gCoe=1.f; //重力系数

    sf::Sprite sprite;

    Player(const sf::Texture& Tex);
    void DrawP(sf::RenderWindow& Win);
    sf::Vector2f getPositionP() const {return Position;}
    void setPosition(sf::Vector2f spawnPoint) {Position=spawnPoint;}

    void MoveX(int facing,float dt,const std::vector<sf::RectangleShape>& platforms);
    void MoveY(float dt,const std::vector<sf::RectangleShape>& platforms);
    void Jump();
    void GrabWallJump();
    void WallJump();
    void CheckGrab(const std::vector<sf::RectangleShape>& platforms);
    void Dash(sf::Vector2f dir);
    void ApplyG(float dt);
    void UpdateP(const float dt,const std::vector<sf::RectangleShape>& platforms,const std::vector<sf::RectangleShape>& springs);

    void Die(const sf::Vector2f& spawnPoint);
};

#endif