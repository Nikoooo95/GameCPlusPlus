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
        // primera vez, es cuando se puede calcular el aspect ratio:

        if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                if (!aspect_ratio_adjusted)
                {
                    // En este ejemplo la ventana está bloqueada para permanecer horizontal.
                    // Por tanto, lo normal es que el ancho sea mayor que el alto. El alto de la resolución
                    // virtual se va a dejar fijo en 720 unidades (tal como se estableció en el constructor)
                    // y se va a escalar el ancho de la resolución virtual para que el aspect ratio virtual
                    // coincida con el real de la ventana:

                    float real_aspect_ratio = float( context->get_surface_width () ) / context->get_surface_height ();

                    canvas_width = unsigned( canvas_height * real_aspect_ratio );

                    aspect_ratio_adjusted = true;

                    state = READY;
                }
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
                canvas->clear ();

                if (state == READY)
                {
                    canvas->set_color (1, 1, 1);

                    // Se dibujan cuadrados que deberían permanecer cuadrados (sin ser estirados) indepen-
                    // dientemente del aspect ratio del dispositivo particular donde se ejecuta el ejemplo
                    // (siempre y cuando la resolución real tenga el mismo aspect ratio que el tamaño
                    // físico del display):

                    constexpr int square_size    = 150;
                    constexpr int square_spacing = 25;

                    for (int y = square_spacing, row = 0; y < canvas_height; y += square_size + square_spacing, row++)
                    {
                        bool fill = (row & 1) == 0;

                        for (int x = square_spacing; x < canvas_width; x += square_size + square_spacing)
                        {
                            if (fill)
                            {
                                canvas->fill_rectangle ({ x, y }, { square_size, square_size });
                                fill = false;
                            }
                            else
                            {
                                canvas->draw_rectangle ({ x, y }, { square_size, square_size });
                                fill = true;
                            }
                        }
                    }
                }
            }
        }
    }

}
