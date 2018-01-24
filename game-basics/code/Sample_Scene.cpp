/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Sample_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Log>
#include <basics/Scaling>
#include <basics/Rotation>
#include <basics/Translation>

using namespace basics;
using namespace std;

namespace example
{

    Sample_Scene::Sample_Scene()
    {
        canvas_width  = 1280;
        canvas_height =  720;
    }

    bool Sample_Scene::initialize ()
    {
        state  = LOADING;
        paused = true;
        x      = 640;
        y      = 360;

        return true;
    }

    void Sample_Scene::suspend ()
    {
        paused = true;
    }

    void Sample_Scene::resume ()
    {
        paused = false;
    }

    void Sample_Scene::handle (Event & event)
    {
        if (state == RUNNING)
        {
            switch (event.id)
            {
                case ID(touch-started):
                case ID(touch-moved):
                {
                    x = *event[ID(x)].as< var::Float > ();
                    y = *event[ID(y)].as< var::Float > ();
                    break;
                }
            }
        }
    }

    void Sample_Scene::update (float time)
    {
        switch (state)
        {
            case LOADING: load ();     break;
            case RUNNING: run  (time); break;
        }
    }

    void Sample_Scene::render (basics::Graphics_Context::Accessor & context)
    {
        if (state == RUNNING)
        {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            canvas->clear          ();
            canvas->set_color      (0, 0, 1);
            canvas->draw_point     ({  360, 360 });
            canvas->draw_segment   ({    0,   0 }, { 1280, 720 });
            canvas->draw_segment   ({    0, 720 }, { 1280,   0 });
            canvas->draw_triangle  ({  640, 520 }, {  200, 200 }, { 1080, 200 });
            canvas->fill_triangle  ({  640, 420 }, {  500, 300 }, {  780, 300 });
            canvas->draw_rectangle ({  100, 100 }, {  100, 100 });
            canvas->fill_rectangle ({ 1080, 520 }, {  100, 100 });

            if (texture)
            {
                canvas->fill_rectangle ({ x, y }, { 100, 100 }, texture.get ());
            }
        }
    }

    void Sample_Scene::load ()
    {
        if (!paused)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                texture = Texture_2D::create (ID(test), context, "test.png");

                if (texture)
                {
                    context->add (texture);

                    state = RUNNING;
                }
            }
        }
    }

    void Sample_Scene::run (float )
    {
    }

}
