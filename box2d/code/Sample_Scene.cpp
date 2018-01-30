/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Box.hpp"
#include "Circle.hpp"
#include "Sample_Scene.hpp"

#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{

    void Sample_Scene::update (float time)
    {
        if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                // Se cargan las texturas y luego se configura el mundo:

                if
                (
                    !(  ball_texture = Texture_2D::create (ID(ball), context,   "ball.png")) ||
                    !(ground_texture = Texture_2D::create (ID(ball), context, "ground.png"))
                )
                {
                    state = ERROR;
                }
                else
                {
                    context->add (  ball_texture);
                    context->add (ground_texture);

                    create_world ();

                    state = RUNNING;
                }
            }
        }
        else
        if (state == RUNNING)
        {
            world.update (time);
        }
    }

    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas)
            {
                canvas->clear ();

                if (state == RUNNING)
                {
                    world.render (*canvas);
                }
            }
        }
    }

    void Sample_Scene::create_world ()
    {
        world.add
        (
            ID(ball1),
            shared_ptr< Shape >
            (
                new Circle(world, Shape::DYNAMIC, 90.f, physics_to_graphics_ratio, ball_texture.get ())
            )
        )
        ->set_position ({ canvas_width / 2.f - 50.f, canvas_height * 2.f });

        world.add
        (
            ID(ball2),
            shared_ptr< Shape >
            (
                new Circle(world, Shape::DYNAMIC, 90.f, physics_to_graphics_ratio, ball_texture.get ())
            )
        )
        ->set_position ({ canvas_width / 2.f + 50.f, canvas_height });

        world.add
        (
            ID(ground),
            shared_ptr< Shape >
            (
                new Box(world, Shape::STATIC, { 1280.f, 128.f }, physics_to_graphics_ratio, ground_texture.get ())
            )
        )
        ->set_position ({ 0.f, 0.f });

        world.add
        (
            ID(left-wall),
            shared_ptr< Shape >
            (
                new Box(world, Shape::STATIC, { 10.f, 720.f }, physics_to_graphics_ratio)
            )
        )
        ->set_position ({ -10.f, 0.f });

        world.add
        (
            ID(right-wall),
            shared_ptr< Shape >
            (
                new Box(world, Shape::STATIC, { 10.f, 720.f }, physics_to_graphics_ratio)
            )
        )
        ->set_position ({ 1280.f, 0.f });
    }

}
