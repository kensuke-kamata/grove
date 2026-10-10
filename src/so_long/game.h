#pragma once

#include "grove/application.h"

#include <memory>

namespace grove
{

class root;

class game final : public application
{
public:
    game();
    ~game() override;

private:
    void update(float dt) override;
    void draw() override;

    std::unique_ptr<root> scene;
};

}
