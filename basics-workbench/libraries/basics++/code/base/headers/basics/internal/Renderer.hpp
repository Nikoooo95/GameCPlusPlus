/*
 * RENDERER
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801091245
 */

#ifndef BASICS_RENDERER_HEADER
#define BASICS_RENDERER_HEADER

    #include <basics/Size>

    namespace basics
    {

        class Renderer
        {

            bool ok;

        protected:

            Renderer()
            {
                ok = true;
            }

            virtual ~Renderer()
            {
            }

        public:

            virtual bool is_ok ()
            {
                return ok;
            }

            virtual void set_view_size (const Size2u & view) { }
            virtual void clear () { }
            virtual void draw_rectangle (float left_x, float bottom_y, float width, float height) { }

        };

    }

#endif
