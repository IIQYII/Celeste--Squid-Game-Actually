#ifndef TIME_H
#define TIME_H

#include <chrono>

//游戏时间
//好恶心，加个注释就给我报错“不允许使用不完整的类型“Time”。”
class Time
{
private:
    std::chrono::steady_clock::time_point start, last, pausePoint;
    std::chrono::steady_clock::duration pauseDuration;
    std::chrono::duration<float> delta;
    bool paused = false;

public:
    Time()
    {
        reset();
    }

    void reset()
    {
        start = std::chrono::steady_clock::now();
        last = start;
        paused = false;
        pauseDuration = std::chrono::steady_clock::duration::zero();
    }

    void update()
    {
        if (paused)
        {
            delta = std::chrono::duration<float>(0);
            return;
        }
        auto now = std::chrono::steady_clock::now();
        delta = std::chrono::duration<float>(now - last);
        last = now;
    }

    //帧间隔，获取当前帧与上一帧的时间间隔（秒）
    float deltaTime() const { return delta.count(); }

    float totalTime() const
    {
        if (paused)
            return std::chrono::duration<float>(pausePoint - start - pauseDuration).count();

        else
            return std::chrono::duration<float>(std::chrono::steady_clock::now() - start - pauseDuration).count();
    }

    void pause()
    {
        if (!paused)
        {
            paused = true;
            pausePoint = std::chrono::steady_clock::now();
        }
    }

    void resume()
    {
        if (paused)
        {
            paused = false;
            pauseDuration += std::chrono::steady_clock::now() - pausePoint;
            last = std::chrono::steady_clock::now();
        }
    }

    bool isPaused() const
    {
        return paused;
    }
};
 
#endif