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
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Raster_Font>
#include <basics/Scene>
#include <basics/Texture_2D>

namespace example
{

    class Sample_Scene : public basics::Scene
    {
    private:

        typedef std::unique_ptr< basics::Atlas       > Atlas_Handle;
        typedef std::unique_ptr< basics::Raster_Font >  Font_Handle;

        enum State
        {
            LOADING,
            READY,
            ERROR,
        };

        State        state;
        bool         suspended;                     ///< true cuando la aplicación está en segundo plano

        unsigned     canvas_width;                  ///< Resolución virtual del display
        unsigned     canvas_height;

        Atlas_Handle atlas;
        Font_Handle  font;

    public:

        Sample_Scene()
        {
            state         = LOADING;
            canvas_width  = 1280;                   // Todavía no se puede calcular el aspect ratio, por lo que se establece
            canvas_height =  720;                   // un tamaño por defecto hasta poder calcular el tamaño final
        }

        basics::Size2u get_view_size () override
        {
            return { canvas_width, canvas_height };
        }

        bool initialize () override
        {
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

    private:

        void draw_slice (basics::Canvas * canvas, const basics::Point2f & where, basics::Atlas & atlas, basics::Id slice_id);

    };

}
