#include "Player.h"
#include <cmath>


Player::Player(const sf::Texture& Tex) : sprite(Tex)
{
    //这个player太大了，如果要用逻辑像素检测，记得把游戏角色改小点（
    sprite.setScale({0.1f,0.1f});
    Position={20,140};
    sprite.setPosition(Position*5.f);

    Velocity={0.f,0.f};
    Remainder={0.f,0.f};

    dashCharge=1;
    stamina=maxStamina;
}

void Player::DrawP(sf::RenderWindow& Win)
{
    sprite.setPosition(Position*5.f);
    Win.draw(sprite);

    //体力条（）
    Win.draw(bg);
    Win.draw(sta);
}

void Player::ApplyG(float dt)
{
    if(!onGround && !isDashing && !isJumping && !isWallJumping)
    {
        Velocity.y+=gG*gCoe*dt;
        if(Velocity.y>MaxFall)
        {
            Velocity.y=MaxFall;
        }
    }
}

void Player::MoveX(int facing,float dt,const std::vector<sf::RectangleShape>& platforms)
{
    float R=onGround ? gR : gAir;
    if(isJumpingWall)
    {
        R=260.f;
    }
    if(isDashing) {}
    else if(facing!=0)
    {
        
        if(std::abs(Velocity.x)>=MaxRun)
        {
            Velocity.x-=speedFacing*dt*R;
            if(std::abs(Velocity.x)<=MaxRun && facing*Velocity.x>0)
            {
                Velocity.x=facing*MaxRun;
            }
        }
        else
        {
            if(facing*Velocity.x>0)
                Velocity.x+=facing*gA*dt;
            else
                Velocity.x+=facing*R*dt;
        }

    }
    else
    {
        if(Velocity.x>0)
        {
            Velocity.x-=R*dt;
            if(Velocity.x<0) Velocity.x=0;
        }
        else if(Velocity.x<0)
        {
            Velocity.x+=R*dt;
            if(Velocity.x>0) Velocity.x=0;
        }
    }

    float move=Velocity.x*dt+Remainder.x;
    int pixels=static_cast<int>(std::round(move));
    Remainder.x=move-pixels;

    int step=(pixels>0) ? 1 : -1;
    while(pixels!=0)
    {
        Position.x+=step;
        sprite.setPosition(Position*5.f);
        bool collided=false;
        for(const auto& plat : platforms)
        {
            if(sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
            {
                collided=true;
                break;
            }
        }

        if(collided)
        {
            Position.x-=step;
            sprite.setPosition(Position*5.f);
            Velocity.x=0;
            Remainder.x=0;
            if(isDashing)
            {
                isDashing=false;
                dashCooldownTimer=dashCooldown;
                Velocity.y=0;
            }
            break;
        }
        pixels-=step;
    }
}

void Player::MoveY(float dt,const std::vector<sf::RectangleShape>& platforms)
{
    float move=Velocity.y*dt+Remainder.y;
    int pixels=static_cast<int>(std::round(move));
    Remainder.y=move-pixels;

    int step=(pixels>0) ? 1 : -1;
    while(pixels!=0)
    {
        Position.y+=step;
        sprite.setPosition(Position*5.f);
        bool collided=false;
        for(const auto& plat : platforms)
        {
            if(sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
            {
                collided=true;
                break;
            }
        }

        if(collided)
        {
            Position.y-=step;
            sprite.setPosition(Position*5.f);
            if(step>0)
            {
            }
            Velocity.y=0;
            Remainder.y=0;
            if(isDashing)
            {
                isDashing=false;
                dashCooldownTimer=dashCooldown;
                Velocity.x=0;
            }
            break;
        }
        pixels-=step;
    }
}

void Player::Jump()
{
    if(canJump && !isDashing)
    {
        if(facing!=0)
        {
            //跳跃加速 JumpBoost
            Velocity.x+=facing*40.f;
        }
        Velocity.y=MaxJump;
        canJump=false;
        isJumping=true;
        jumpTimer=jumpDur;
    }
}

void Player::GrabWallJump()
{
    if(isGrabbing && stamina>0)
    {
        stamina-=wallJumpCost;
        if(stamina<0) stamina=0;

        isGrabbing=false;
        canGrab=false;
        checkWall=true;

        isWallJumping=true;
        wallJumpTimer=wallJumpDur;

        Velocity.x=0;
        Velocity.y=wallJumpSpeedY;
    }
}

void Player::WallJump()
{
    Velocity.x=-wallGrabDir*wallJumpSpeedX;
    Velocity.y=MaxJump;
    isJumping=true;
    if(facing!=0)
    {
        isJumpingWall=true;
    }
    jumpTimer=jumpDur;
}

void Player::Dash(sf::Vector2f dir)
{
    if(!isDashing && dashCooldownTimer<=0.f && dashCharge>0)
    {
        isDashing=true;
        dashTimer=dashDuration;
        dashCharge--;
        
        if(dir.x==0.f && dir.y==0.f)
        {
            dir.x=static_cast<float>(F);
        }

        dashDir=dir;

        Velocity.x=dashDir.x*dashSpeed;
        Velocity.y=dashDir.y*dashSpeed;

        Remainder={0.f,0.f};
    }
}

void Player::CheckGrab(const std::vector<sf::RectangleShape>& platforms)
{
    canGrab=false;

    if(isDashing) return;

    sf::FloatRect playerbounds=sprite.getGlobalBounds();
    float dis=5.f;

    
    sf::FloatRect left({playerbounds.position.x-dis,playerbounds.position.y},{dis,playerbounds.size.y});
    sf::FloatRect right({playerbounds.position.x+playerbounds.size.x,playerbounds.position.y},{dis,playerbounds.size.y});

    for(const auto& plat : platforms)
    {
        if(left.findIntersection(plat.getGlobalBounds()))
        {
            canGrab=true;
            wallGrabDir=-1;
            break;
        }
        if(right.findIntersection(plat.getGlobalBounds()))
        {
            canGrab=true;
            wallGrabDir=1;
            break;
        }
        wallGrabDir=0;
    }
}

void Player::Die(const sf::Vector2f& spawnPoint)
{
    Position = spawnPoint;
    sprite.setPosition(Position * 5.f);

    //重置
    Velocity={0.f,0.f};
    Remainder={0.f,0.f};

    facing=0; //1右，0不动，-1左
    F=1;    //上面那个facing是判断移动的朝向，这个就是纯粹的朝向
    speedFacing=0;
    Ducking=false;
    canJump=false;
    CisHold=false;

    isDashing = false;
    dashCharge=1;
    dashTimer = 0.f;
    dashCooldownTimer = 0.f;

    canGrab=false;
    isGrabbing=false;
    isSliding=false;
    checkZ=true;
    wallGrabDir=0; //1:Right, -1:Left
    stamina=110.f;

    isWallJumping=false;
    checkWall=false;
    wallJumpTimer=0.f;

    isJumping=false;
    jumpTimer=0.f;
    gCoe=1.f; //重力系数
}

void Player::UpdateP(const float dt,const std::vector<sf::RectangleShape>& platforms,const std::vector<sf::RectangleShape>& springs)
{

    std::cout << "gCoe: " << gCoe << std::endl;
    std::cout << "speedX: " << Velocity.x << std::endl << "speedY: " << Velocity.y << std::endl;
    std::cout << "PX: " << Position.x << std::endl << "PY: " << Position.y << std::endl;

    if(dashCooldownTimer>0.f)
    {
        dashCooldownTimer-=dt;
        if(dashCooldownTimer<0.f)
        {
            dashCooldownTimer=0.f;
        }
    }

    if(isJumping)
    {
        jumpTimer-=dt;
        if(!isSpring)
        {
            if(jumpTimer<=0.f || !CisHold)
            {
                isJumping=false;
                isJumpingWall=false;
            }
        }
        else
        {
            if(jumpTimer<=0.f)
            {
                isJumping=false;
                isSpring=false;
            }
        }
        isGrabbing=false;
    }

    if (isWallJumping)
    {
        wallJumpTimer -= dt;
        if (wallJumpTimer <= 0.f || !CisHold)
        {
            isWallJumping = false;
        }
    }
    
    if(!isWallJumping)
    {
        CheckGrab(platforms);
    }
    else
    {
        canGrab=false;
    }

    if(isGrabbing && stamina<=0)
    {
        checkZ=false;
        isGrabbing=false;
        Velocity.x=0;
        Velocity.y=0;
    }

    if(std::abs(Velocity.y)<=40.f)
    {
        gCoe=0.5f;
    }
    else
    {
        gCoe=1.f;
    }

    if(isDashing)
    {
        std::cout << "isDashing" << std::endl;

        dashTimer-=dt;
        if(dashTimer<=0.f)
        {
            isDashing=false;
            dashCooldownTimer=dashCooldown;
            
            if(dashDir.y<0)
            {
                Velocity.y=-120.f;
            }
            //Velocity.y=0.f;     //上冲，结束后y速度重置为120，速度<40时，启用半重力（我觉得是跟跳跃的半重力用了同一段代码）
            if(dashDir.x!=0)
            {
                Velocity.x=std::round(dashDir.x)*160.f;
            }
        }
        MoveX(0,dt,platforms);
        MoveY(dt,platforms);
    }
    else if(isGrabbing)
    {
        std::cout << "isGrabbing" << std::endl;

        Velocity.x=0;
        Remainder.x=0;
        canJump=false;

        stamina-=wallGrabCost*dt;
        if(stamina<0) stamina=0;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        {
            Velocity.y=-climbSpeed;
            stamina-=(wallClimbCost-wallGrabCost)*dt;
            if(stamina<0) stamina=0;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
        {
            stamina+=wallGrabCost*dt;
            Velocity.y=slideSpeed;
        }
        else
        {
            if(Velocity.y<0.f && checkWall)
            {
                Velocity.y+=gG*dt;
            }
            if(Velocity.y>=0.f)
            {
                checkWall=false;
            }
            if(!checkWall)
            {
                Velocity.y=0.f;
                Remainder.y=0.f;
            }
        }
        MoveY(dt,platforms);
    }
    
    else if(canGrab && (stamina<=0 || !isGrabbing) && ((wallGrabDir==1 && facing==1) || (wallGrabDir==-1 && facing==-1)) && Velocity.y>=0)
    {
        std::cout << "isSliding" << std::endl;

        if(Velocity.y>20.f && !isSliding)
        {
            Velocity.y=20.f;
            isSliding=true;
        }

        Velocity.y+=gS*dt;
        if(Velocity.y>maxSlideSpeed)
        {
            Velocity.y=maxSlideSpeed;
        }
        Velocity.x=0;
        MoveY(dt,platforms);
    }
    else
    {
        std::cout << "isNormal" << std::endl;
        
        isSliding=false;
        ApplyG(dt);
        MoveX(facing,dt,platforms);
        MoveY(dt,platforms);
    }

    if(Velocity.x==0) speedFacing=0;
    else speedFacing=(Velocity.x>0) ? 1 : -1;

    //当所有移动执行完之后，一个更暴力的地面检测

    bool wasOnGround=onGround;
    onGround=false;
    Position.y+=1.f;
    sprite.setPosition(Position*5.f);
    
    bool currentlyOnGround=false;
    for(const auto& plat : platforms)
    {
        if(sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
        {
            currentlyOnGround=true;
            break;
        }
    }
    Position.y-=1.f;
    sprite.setPosition(Position*5.f);

    onGround=currentlyOnGround;

    if(wallGrabDir==0)
    {
        isGrabbing=false;
    }

    if(stamina>0)
    {
        checkZ=true;
    }

    if(onGround)
    {
        dashCharge=1;
        stamina=maxStamina;
        canJump=true;
        isGrabbing=false;
        Velocity.y=0.f;
        Remainder.y=0.f;
    }
    else
    {
        canJump=false;
    }

    for(const auto& spring : springs)
    {
        if(sprite.getGlobalBounds().findIntersection(spring.getGlobalBounds()))
        {
            Velocity.x=0.f;
            Velocity.y=-185.f;
            dashCharge=1;
            isJumping=true;
            isSpring=true;
            jumpTimer=jumpDur;
            break;
        }
    }


    //体力条（）
    bg.setSize({55*5.f,3*5.f});
    bg.setPosition({5*5.f,5*5.f});
    bg.setFillColor(sf::Color::White);

    sta.setSize({stamina/2*5.f,3*5.f});
    sta.setPosition({5*5.f,5*5.f});
    sta.setFillColor(sf::Color::Green);

}

