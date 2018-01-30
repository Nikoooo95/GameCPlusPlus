/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <memory>
#include <basics/Scene>
#include <basics/Texture_2D>

namespace example
{

    class Sample_Scene : public basics::Scene
    {
    private:

        enum State
        {
            LOADING,
            READY,
        };

        State    state;
        bool     suspended;                         ///< true cuando la aplicación está en segundo plano

        unsigned canvas_width;                      ///< Resolución virtual del display
        unsigned canvas_height;
        bool     aspect_ratio_adjusted;             ///< false hasta que se ajuste el aspect ratio de la resolución
                                                    ///< virtual para que concincida con el de la real
    public:

        Sample_Scene()
        {
            // Todavía no se puede calcular el aspect ratio porque es necesario disponer de un Contexto
            // Gráfico para conocer el tamaño de la ventana. Por ello, se establece un tamaño por defecto
            // hasta poder calcular el tamaño final:

            canvas_width  = 1280;
            canvas_height =  720;
            aspect_ratio_adjusted = false;          // El aspect ratio se debe calcular más adelante,
        }                                           // en cuanto se tenga acceso al contexto gráfico

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

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

        void update (float ) override;
        void render (basics::Graphics_Context::Accessor & context) override;

    };

}
