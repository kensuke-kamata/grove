#pragma once

#include <memory>

namespace grove
{

class profiler;

class application
{
public:
    application(const char* title, int width, int height, int fps);
    virtual ~application();

    void run();

protected:
    virtual void update(float dt) = 0;
    virtual void draw() = 0;

private:
    const char* title;

    int width;
    int height;
    int fps;

    std::unique_ptr<profiler> metrics;
};

}
