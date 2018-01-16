/*
 * OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712200139
 */

#ifndef BASICS_OPENGL_ES_CONTEXT_HEADER
#define BASICS_OPENGL_ES_CONTEXT_HEADER

    #include <memory>
    #include <basics/Window>
    #include <basics/Graphics_Context>

    namespace basics
    {

        class OpenGLES_Context : public Graphics_Context
        {
        public:

            // Este método debe recibir los atributos deseados para el contexto...
            static bool create (Window::Accessor & window);

        protected:

            OpenGLES_Context(Window & window) : Graphics_Context(window)
            {
            }

            virtual ~OpenGLES_Context() = default;

        };

    }

#endif
