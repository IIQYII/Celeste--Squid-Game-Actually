#ifndef TIME_H
#define TIME_H

#include <chrono>

//��Ϸʱ��
//�ö��ģ��Ӹ�ע�;͸��ұ���������ʹ�ò����������͡�Time������
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

    //֡�������ȡ��ǰ֡����һ֡��ʱ�������룩
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