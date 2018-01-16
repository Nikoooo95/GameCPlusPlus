/*
 * OPENGL ES RENDERER
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712200139
 */

#ifndef BASICS_OPENGL_ES_RENDERER_HEADER
#define BASICS_OPENGL_ES_RENDERER_HEADER

    #include <GLES2/gl2.h>
    #include <basics/Graphics_Context>
    #include <basics/Renderer>
    #include <basics/Size>

    namespace basics
    {

        class OpenGLES_Renderer : public Renderer
        {

            static const char *   vertex_shader_code;
            static const char * fragment_shader_code;

            bool ok;

            float view_width;
            float view_height;

        public:

            OpenGLES_Renderer(const Size2u & viewport);

            void set_view_size (const Size2u & view) override;
            void clear () override;
            void draw_rectangle (float left_x, float bottom_y, float width, float height) override;

        private:

            GLuint compile_shader (const char * source_code, GLenum type);

        };

    }

#endif
