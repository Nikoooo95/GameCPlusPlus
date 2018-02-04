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

    void Sample_Scene::update (float )
    {
        // Mientras se está cargando la escena, cuando se consigue acceso al contexto gráfico por
        // primera vez, es cuando se puede calcular el aspect ratio y cargar el atlas:

        if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                float real_aspect_ratio = float( context->get_surface_width () ) / context->get_surface_height ();

                canvas_width = unsigned( canvas_height * real_aspect_ratio );

                atlas.reset (new Atlas("animals.sprites", context));

                state = atlas->good () ? READY : ERROR;
            }
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
                canvas->set_clear_color (.5f, .5f, .5f);
                canvas->clear ();

                if (state == READY)
                {
                    draw_slice (canvas, { canvas_width * 0.25f, canvas_height * 0.25f }, *atlas, ID(rabbit) );
                    draw_slice (canvas, { canvas_width * 0.50f, canvas_height * 0.25f }, *atlas, ID(pig)    );
                    draw_slice (canvas, { canvas_width * 0.75f, canvas_height * 0.25f }, *atlas, ID(monkey) );
                    draw_slice (canvas, { canvas_width * 0.25f, canvas_height * 0.50f }, *atlas, ID(cat)    );
                    draw_slice (canvas, { canvas_width * 0.50f, canvas_height * 0.50f }, *atlas, ID(dog)    );
                    draw_slice (canvas, { canvas_width * 0.75f, canvas_height * 0.50f }, *atlas, ID(goat)   );
                    draw_slice (canvas, { canvas_width * 0.25f, canvas_height * 0.75f }, *atlas, ID(chicken));
                    draw_slice (canvas, { canvas_width * 0.50f, canvas_height * 0.75f }, *atlas, ID(mouse)  );
                    draw_slice (canvas, { canvas_width * 0.75f, canvas_height * 0.75f }, *atlas, ID(cow)    );
                }
            }
        }
    }

    void Sample_Scene::draw_slice (Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id)
    {
        const Atlas::Slice * slice = atlas.get_slice (slice_id);

        if (slice)
        {
            canvas->fill_rectangle (where, { slice->width, slice->height }, slice);
        }
    }

}
