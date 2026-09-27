#pragma once

#include "grove/application.h"

namespace grove
{

class game final : public application
{
public:
    game();

private:
    void update(float dt) override;
    void draw() override;

    float x = 320.f;
    float y = 240.f;
    float speed = 120.f;
    static constexpr float size = 32.f;
};

}
