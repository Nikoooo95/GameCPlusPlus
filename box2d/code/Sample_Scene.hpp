/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#pragma once

#include <basics/Scene>
#include <basics/Texture_2D>
#include "World.hpp"

namespace example
{

    class Sample_Scene : public basics::Scene
    {

        typedef std::shared_ptr< basics::Texture_2D > Texture_Handle;

    private:

        /**
         * Representa el estado global de la escena en un momento dado.
         */
        enum State
        {
            LOADING,
            RUNNING,
            ERROR
        };

        State          state;
        bool           suspended;                       ///< true cuando la aplicación está en segundo plano.

        unsigned       canvas_width;                    ///< Alto  de la escena en coordenadas virtuales.
        unsigned       canvas_height;                   ///< Ancho de la escena en coordenadas virtuales.
        float          physics_to_graphics_ratio;       ///< Factor de escala para convertir de metros (física) a unidades virtuales.

        Texture_Handle ball_texture;
        Texture_Handle ground_texture;

        World          world;                           ///< Objeto que guarda y controla las formas geométricas con física.

    public:

        Sample_Scene()
        {
            canvas_width  = 1280;                       // La pantalla (de la resolución que sea) se divide en 1280x720
            canvas_height =  720;                       // unidades gráficas virtuales
            physics_to_graphics_ratio = 300.f;          // Un metro equivale a 300 unidades gráficas virtuales
        }

    public:

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

    public:

        bool initialize () override
        {
            state     = LOADING;
            suspended = false;
            return true;
        }

        void suspend () override
        {
            suspended = true;
        }

        void resume () override
        {
            suspended = false;
        }

        void update (float time) override;
        void render (basics::Graphics_Context::Accessor & context) override;

    private:

        void create_world ();

    };

}
