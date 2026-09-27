#pragma once

namespace grove
{

class profiler
{
public:
    void update(float dt)
    {
        if (dt <= 0.f)
        {
            return;
        }

        sum += dt - samples[next];
        samples[next] = dt;

        next = (next + 1) % capacity;
        if (count < capacity)
        {
            ++count;
        }
    }

    float fps() const
    {
        if (sum <= 0.f)
        {
            return 0.f;
        }
        return static_cast<float>(count) / sum;
    }

private:
    static constexpr int capacity = 10;

    float samples[capacity] = {};
    float sum   = 0.f;
    int   count = 0;
    int   next  = 0;
};

}
