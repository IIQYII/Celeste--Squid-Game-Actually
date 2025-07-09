#include "Player.h"
#include <cmath>


Player::Player(const sf::Texture& Tex) : sprite(Tex)
{
    Position = { 20,140 };
    sprite.setPosition(Position * 5.f);

    Velocity = { 0.f,0.f };
    Remainder = { 0.f,0.f };

    dashCharge = 1;
    stamina = maxStamina;

    if(!DeadBuffer.loadFromFile("./Music/Death.wav"))
    {
        std::cerr << "Death Err" << std::endl;
        Dead=nullptr;
    }
    else
    {
        Dead=std::make_unique<sf::Sound>(DeadBuffer);
    }
}

void Player::DrawP(sf::RenderWindow& Win)
{
    sprite.setPosition(Position * 5.f);
    Win.draw(sprite);

    //体力条
    Win.draw(bg);
    Win.draw(sta);
}

//应用重力效果
void Player::ApplyG(float dt)
{
    if (!onGround && !isDashing && !isJumping && !isWallJumping)
    {
        Velocity.y += gG * gCoe * dt;
        if (Velocity.y > MaxFall)
        {
            Velocity.y = MaxFall;
        }
    }
}

//处理X轴的移动，实现逐像素移动
void Player::MoveX(int facing, float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes)
{
    float R = onGround ? gR : gAir;
    if (isJumpingWall)
    {
        R = 260.f;
    }
    if (isDashing) {}
    else if (facing != 0)
    {

        if (std::abs(Velocity.x) >= MaxRun)
        {
            Velocity.x -= speedFacing * dt * R;
            if (std::abs(Velocity.x) <= MaxRun && facing * Velocity.x > 0)
            {
                Velocity.x = facing * MaxRun;
            }
        }
        else
        {
            if (facing * Velocity.x > 0)
                Velocity.x += facing * gA * dt;
            else
                Velocity.x += facing * R * dt;
        }

    }
    else
    {
        if (Velocity.x > 0)
        {
            Velocity.x -= R * dt;
            if (Velocity.x < 0) Velocity.x = 0;
        }
        else if (Velocity.x < 0)
        {
            Velocity.x += R * dt;
            if (Velocity.x > 0) Velocity.x = 0;
        }
    }

    float move = Velocity.x * dt + Remainder.x;
    int pixels = static_cast<int>(std::round(move));
    Remainder.x = move - pixels;

    int step = (pixels > 0) ? 1 : -1;
    while (pixels != 0)
    {
        //判断碰撞
        Position.x += step;
        sprite.setPosition(Position * 5.f);
        bool collided = false;
        for (const auto& plat : platforms)
        {
            if (sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
            {
                collided = true;
                break;
            }
        }
        if (!collided)
        {
            for (const auto& mover : movers)
            {
                if (sprite.getGlobalBounds().findIntersection(mover.getGlobalBounds()))
                {
                    collided = true;
                    break;
                }
            }
            for (const auto& crush : crushes)
            {
                if (crush.isSee)
                {
                    if (sprite.getGlobalBounds().findIntersection(crush.getGlobalBounds()))
                    {
                        collided = true;
                        break;
                    }
                }
            }
        }

        if (collided)
        {
            Position.x -= step;
            sprite.setPosition(Position * 5.f);
            Velocity.x = 0;
            Remainder.x = 0;
            if (isDashing)
            {
                isDashing = false;
                dashCooldownTimer = dashCooldown;
                Velocity.y = 0;
            }
            break;
        }
        pixels -= step;
    }
}

//处理Y轴运动，实现逐像素移动
void Player::MoveY(float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes)
{
    float move = Velocity.y * dt + Remainder.y;
    int pixels = static_cast<int>(std::round(move));
    Remainder.y = move - pixels;

    int step = (pixels > 0) ? 1 : -1;
    while (pixels != 0)
    {
        //判断碰撞
        Position.y += step;
        sprite.setPosition(Position * 5.f);
        bool collided = false;
        for (const auto& plat : platforms)
        {
            if (sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
            {
                collided = true;
                break;
            }
        }
        if (!collided)
        {
            for (auto& mover : const_cast<std::vector<Mover>&>(movers))
            {
                if (sprite.getGlobalBounds().findIntersection(mover.getGlobalBounds()))
                {
                    collided = true;
                    break;
                }
            }
            for (const auto& crush : crushes)
            {
                if (crush.isSee)
                {
                    if (sprite.getGlobalBounds().findIntersection(crush.getGlobalBounds()))
                    {
                        collided = true;
                        break;
                    }
                }
            }
        }

        if (collided)
        {
            Position.y -= step;
            sprite.setPosition(Position * 5.f);
            Velocity.y = 0;
            Remainder.y = 0;
            if (isDashing)
            {
                isDashing = false;
                dashCooldownTimer = dashCooldown;
                Velocity.x = 0;
            }
            break;
        }
        pixels -= step;
    }
}

//跳跃
void Player::Jump()
{
    if (canJump && !isDashing)
    {
        if (facing != 0)
        {
            //跳跃加速 JumpBoost
            Velocity.x += facing * 40.f;
        }
        Velocity.y = MaxJump;
        canJump = false;
        isJumping = true;
        jumpTimer = jumpDur;
    }
}

//抓墙跳
void Player::GrabWallJump()
{
    if (isGrabbing && stamina > 0)
    {
        stamina -= wallJumpCost;
        if (stamina < 0) stamina = 0;

        isGrabbing = false;
        canGrab = false;
        checkWall = true;

        isWallJumping = true;
        wallJumpTimer = wallJumpDur;

        Velocity.x = 0;
        Velocity.y = wallJumpSpeedY;
    }
}

//墙跳
void Player::WallJump()
{
    Velocity.x = -wallGrabDir * wallJumpSpeedX;
    Velocity.y = MaxJump;
    isJumping = true;
    if (facing != 0)
    {
        isJumpingWall = true;
    }
    jumpTimer = jumpDur;
}

//冲刺
void Player::Dash(sf::Vector2f dir)
{
    if (!isDashing && dashCooldownTimer <= 0.f && dashCharge > 0)
    {
        isDashing = true;
        dashTimer = dashDuration;
        dashCharge--;

        if (dir.x == 0.f && dir.y == 0.f)
        {
            dir.x = static_cast<float>(F);
        }

        dashDir = dir;

        Velocity.x = dashDir.x * dashSpeed;
        Velocity.y = dashDir.y * dashSpeed;

        Remainder = { 0.f,0.f };
    }
}

//判断角色是否可以执行抓墙操作
void Player::CheckGrab(const std::vector<sf::RectangleShape>& platforms, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes)
{
    canGrab = false;
    attachedMover = nullptr;

    if (isDashing) return;

    sf::FloatRect playerbounds = sprite.getGlobalBounds();
    float dis = 5.f;


    sf::FloatRect left({ playerbounds.position.x - dis,playerbounds.position.y }, { dis,playerbounds.size.y });
    sf::FloatRect right({ playerbounds.position.x + playerbounds.size.x,playerbounds.position.y }, { dis,playerbounds.size.y });

    for (const auto& plat : platforms)
    {
        if (left.findIntersection(plat.getGlobalBounds()))
        {
            canGrab = true;
            wallGrabDir = -1;
            break;
        }
        if (right.findIntersection(plat.getGlobalBounds()))
        {
            canGrab = true;
            wallGrabDir = 1;
            break;
        }
        wallGrabDir = 0;
    }

    if (wallGrabDir == 0)
    {
        for (auto& mover : const_cast<std::vector<Mover>&>(movers))
        {
            if (left.findIntersection(mover.getGlobalBounds()))
            {
                canGrab = true;
                wallGrabDir = -1;
                if (isGrabbing && mover.getState() == MoverState::Idle)
                {
                    mover.activate();
                    attachedMover = &mover;
                }
                break;
            }
            if (right.findIntersection(mover.getGlobalBounds()))
            {
                canGrab = true;
                wallGrabDir = 1;
                if (isGrabbing && mover.getState() == MoverState::Idle)
                {
                    mover.activate();
                    attachedMover = &mover;
                }
                break;
            }
        }
    }
}

//角色死亡
void Player::Die(const sf::Vector2f& spawnPoint)
{
    Position = spawnPoint;
    sprite.setPosition(Position * 5.f);

    if(Dead)
    {
        Dead->play();
    }

    //死亡后重置状态
    Velocity = { 0.f,0.f };
    Remainder = { 0.f,0.f };

    facing = 0;
    F = 1;
    speedFacing = 0;
    Ducking = false;
    canJump = false;
    CisHold = false;

    isDashing = false;
    dashCharge = 1;
    dashTimer = 0.f;
    dashCooldownTimer = 0.f;

    canGrab = false;
    isGrabbing = false;
    isSliding = false;
    checkZ = true;
    wallGrabDir = 0;
    stamina = 110.f;

    isWallJumping = false;
    checkWall = false;
    wallJumpTimer = 0.f;

    isJumping = false;
    jumpTimer = 0.f;
    gCoe = 1.f;
}

//角色的主更新循环
//处理各种计时器、角色当前状态（正常、冲刺、抓墙等）、角色是否位于地面、是否激活移动块和碎裂块等
void Player::UpdateP(const float dt, const std::vector<sf::RectangleShape>& platforms, const std::vector<sf::RectangleShape>& springs, const std::vector<Mover>& movers, const std::vector<CrushBlock>& crushes)
{
    std::cout << "speedX: " << Velocity.x << std::endl << "speedY: " << Velocity.y << std::endl;
    std::cout << "PX: " << Position.x << std::endl << "PY: " << Position.y << std::endl;

    attachedMover = nullptr;

    if (dashCooldownTimer > 0.f)
    {
        dashCooldownTimer -= dt;
        if (dashCooldownTimer < 0.f)
        {
            dashCooldownTimer = 0.f;
        }
    }

    if (isJumping)
    {
        jumpTimer -= dt;
        if (!isSpring)
        {
            if (jumpTimer <= 0.f || !CisHold)
            {
                isJumping = false;
                isJumpingWall = false;
            }
        }
        else
        {
            if (jumpTimer <= 0.f)
            {
                isJumping = false;
                isSpring = false;
            }
        }
        isGrabbing = false;
    }

    if (isWallJumping)
    {
        wallJumpTimer -= dt;
        if (wallJumpTimer <= 0.f || !CisHold)
        {
            isWallJumping = false;
        }
    }

    if (!isWallJumping)
    {
        CheckGrab(platforms, movers, crushes);
    }
    else
    {
        canGrab = false;
    }

    if (isGrabbing && stamina <= 0)
    {
        checkZ = false;
        isGrabbing = false;
        Velocity.x = 0;
        Velocity.y = 0;
    }

    if (std::abs(Velocity.y) <= 40.f)
    {
        gCoe = 0.5f;
    }
    else
    {
        gCoe = 1.f;
    }

    if (isDashing)
    {
        std::cout << "isDashing" << std::endl;

        dashTimer -= dt;
        if (dashTimer <= 0.f)
        {
            isDashing = false;
            dashCooldownTimer = dashCooldown;

            if (dashDir.y < 0)
            {
                Velocity.y = -120.f;
            }
            if (dashDir.x != 0)
            {
                Velocity.x = std::round(dashDir.x) * 160.f;
            }
        }
        MoveX(0, dt, platforms, movers, crushes);
        MoveY(dt, platforms, movers, crushes);
    }
    else if (isGrabbing)
    {
        std::cout << "isGrabbing" << std::endl;

        Velocity.x = 0;
        Remainder.x = 0;
        canJump = false;

        stamina -= wallGrabCost * dt;
        if (stamina < 0) stamina = 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        {
            Velocity.y = -climbSpeed;
            stamina -= (wallClimbCost - wallGrabCost) * dt;
            if (stamina < 0) stamina = 0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
        {
            stamina += wallGrabCost * dt;
            Velocity.y = slideSpeed;
        }
        else
        {
            if (Velocity.y < 0.f && checkWall)
            {
                Velocity.y += gG * dt;
            }
            if (Velocity.y >= 0.f)
            {
                checkWall = false;
            }
            if (!checkWall)
            {
                Velocity.y = 0.f;
                Remainder.y = 0.f;
            }
        }
        MoveY(dt, platforms, movers, crushes);
    }

    else if (canGrab && (stamina <= 0 || !isGrabbing) && ((wallGrabDir == 1 && facing == 1) || (wallGrabDir == -1 && facing == -1)) && Velocity.y >= 0)
    {
        std::cout << "isSliding" << std::endl;

        if (Velocity.y > 20.f && !isSliding)
        {
            Velocity.y = 20.f;
            isSliding = true;
        }

        Velocity.y += gS * dt;
        if (Velocity.y > maxSlideSpeed)
        {
            Velocity.y = maxSlideSpeed;
        }
        Velocity.x = 0;
        MoveY(dt, platforms, movers, crushes);
    }
    else
    {
        std::cout << "isNormal" << std::endl;

        isSliding = false;
        ApplyG(dt);
        MoveX(facing, dt, platforms, movers, crushes);
        MoveY(dt, platforms, movers, crushes);
    }

    if (Velocity.x == 0) speedFacing = 0;
    else speedFacing = (Velocity.x > 0) ? 1 : -1;

    //地面检测
    bool wasOnGround = onGround;
    bool wasOnMover = onMover;
    onMover = false;
    onGround = false;
    Position.y += 1.f;
    sprite.setPosition(Position * 5.f);

    bool currentlyOnGround = false;
    bool currentlyOnMover = false;
    for (const auto& plat : platforms)
    {
        if (sprite.getGlobalBounds().findIntersection(plat.getGlobalBounds()))
        {
            currentlyOnGround = true;
            break;
        }
    }
    if (!currentlyOnGround)
    {
        for (auto& mover : const_cast<std::vector<Mover>&>(movers))
        {
            if (sprite.getGlobalBounds().findIntersection(mover.getGlobalBounds()))
            {
                currentlyOnGround = true;
                currentlyOnMover = true;
                attachedMover = &mover;
                if (mover.getState() == MoverState::Idle)
                {
                    mover.activate();
                }
                break;
            }
        }
        for (auto& crush : const_cast<std::vector<CrushBlock>&>(crushes))
        {
            if (sprite.getGlobalBounds().findIntersection(crush.getGlobalBounds()) && crush.isSee)
            {
                currentlyOnGround = true;
                crush.isRiding = true;
                break;
            }
            else
            {
                crush.isRiding = false;
            }
        }
    }
    Position.y -= 1.f;
    sprite.setPosition(Position * 5.f);

    onGround = currentlyOnGround;
    onMover = currentlyOnMover;

    if (wallGrabDir == 0)
    {
        isGrabbing = false;
    }

    if (stamina > 0)
    {
        checkZ = true;
    }

    if (onGround)
    {
        dashCharge = 1;
        stamina = maxStamina;
        canJump = true;
        isGrabbing = false;
        Velocity.y = 0.f;
        Remainder.y = 0.f;
    }
    else
    {
        canJump = false;
    }

    //处理弹簧碰撞
    for (const auto& spring : springs)
    {
        if (sprite.getGlobalBounds().findIntersection(spring.getGlobalBounds()))
        {
            Velocity.x = 0.f;
            Velocity.y = -185.f;
            dashCharge = 1;
            isJumping = true;
            isSpring = true;
            jumpTimer = jumpDur;
            break;
        }
    }

    //使角色跟随移动块移动
    if (attachedMover)
    {
        moverTimer = moverDur;
        wasMover = attachedMover;
    }
    else
    {
        moverTimer -= dt;
        if (moverTimer < 0.f)
        {
            wasMover = nullptr;
        }
    }

    if (attachedMover)
    {
        sf::Vector2f delta = attachedMover->getDelta();
        Position += delta;
    }
    else if (moverTimer >= 0.f)
    {
        sf::Vector2f delta = wasMover->getDelta();
        Position += delta;
    }

    //体力条
    bg.setSize({ 55 * 5.f,3 * 5.f });
    bg.setPosition({ 5 * 5.f,5 * 5.f });
    bg.setFillColor(sf::Color::White);

    sta.setSize({ stamina / 2 * 5.f,3 * 5.f });
    sta.setPosition({ 5 * 5.f,5 * 5.f });
    sta.setFillColor(sf::Color::Green);

}

