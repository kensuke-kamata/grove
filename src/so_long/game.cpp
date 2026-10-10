#include "game.h"

#include <hierstate.h>
#include <raylib.h>

#include <memory>
#include <stdexcept>

namespace grove
{

namespace
{

namespace id
{

enum class scene
{
    title,
    game,
};

enum class game
{
    play,
    clear,
};

} // namespace id

struct data
{
    float dt = 0.f;
    float x = 320.f;
    float y = 240.f;
    float speed = 120.f;
};

class title final : public hierstate::state
{
public:
    explicit title(hierstate::group<id::scene>& parent)
        : parent(parent)
    {
    }

    void update() override
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            parent.request(id::scene::game);
        }
    }

private:
    hierstate::group<id::scene>& parent;
};

class play final : public hierstate::state
{
public:
    play(hierstate::group<id::game>& parent, data& data)
        : parent(parent)
        , data(data)
    {
    }

    void update() override
    {
        if (IsKeyDown(KEY_D))
        {
            data.x += data.speed * data.dt;
        }
        else if (IsKeyDown(KEY_A))
        {
            data.x -= data.speed * data.dt;
        }
        else if (IsKeyDown(KEY_S))
        {
            data.y += data.speed * data.dt;
        }
        else if (IsKeyDown(KEY_W))
        {
            data.y -= data.speed * data.dt;
        }

        if (IsKeyPressed(KEY_C))
        {
            parent.request(id::game::clear);
        }
    }

private:
    hierstate::group<id::game>& parent;
    data& data;
};

class clear final : public hierstate::state
{
public:
    explicit clear(hierstate::group<id::scene>& root)
        : root(root)
    {
    }

    void update() override
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            root.request(id::scene::title);
        }
    }

private:
    hierstate::group<id::scene>& root;
};

class stage final : public hierstate::group<id::game>
{
public:
    stage(hierstate::group<id::scene>& root, data& data)
        : root(root)
        , data(data)
    {
        data.x = 320.f;
        data.y = 240.f;
        start(id::game::play);
    }

    void draw() const
    {
        if (current() == id::game::play)
        {
            constexpr float size = 32.f;
            DrawRectangleRec({data.x - size / 2, data.y - size / 2, size, size}, BLUE);
            DrawText("WASD: move / C: clear", 16, 48, 20, DARKGRAY);
        }
        else
        {
            DrawText("CLEAR", 260, 190, 36, DARKGREEN);
            DrawText("Space: return to title", 205, 250, 20, DARKGRAY);
        }
    }

private:
    std::unique_ptr<hierstate::state> create(id::game id) override
    {
        switch (id)
        {
        case id::game::play:
            return std::make_unique<play>(*this, data);
        case id::game::clear:
            return std::make_unique<clear>(root);
        }
        throw std::invalid_argument("unknown game state");
    }

    hierstate::group<id::scene>& root;
    data& data;
};

} // namespace

class root final : public hierstate::group<id::scene>
{
public:
    root()
    {
        start(id::scene::title);
    }

    void set_frame_time(float dt)
    {
        data.dt = dt;
    }

    void draw() const
    {
        if (current() == id::scene::title)
        {
            DrawText("SO LONG", 240, 190, 36, DARKGRAY);
            DrawText("Space: start", 255, 250, 20, DARKGRAY);
        }
        else
        {
            static_cast<const stage*>(get())->draw();
        }
    }

private:
    std::unique_ptr<hierstate::state> create(id::scene id) override
    {
        switch (id)
        {
        case id::scene::title:
            return std::make_unique<title>(*this);
        case id::scene::game:
            return std::make_unique<stage>(*this, data);
        }
        throw std::invalid_argument("unknown scene");
    }

    data data;
};

game::game()
    : application("grove: so long", 640, 480, 60)
    , scene(std::make_unique<root>())
{
}

game::~game()
{
    scene->exit();
}

void game::update(float dt)
{
    scene->set_frame_time(dt);
    scene->update();
}

void game::draw()
{
    scene->draw();
}

}
