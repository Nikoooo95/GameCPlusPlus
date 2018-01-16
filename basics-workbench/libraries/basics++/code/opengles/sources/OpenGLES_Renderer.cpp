/*
 * WINDOW
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801091703
 */

#include <basics/Log>
#include <basics/OpenGLES_Renderer>

namespace basics
{

    const char * OpenGLES_Renderer::vertex_shader_code =
        "attribute vec4 vertex_position;"
        "void main()"
        "{"
        "gl_Position = vertex_position;"
        "}";

    const char * OpenGLES_Renderer::fragment_shader_code =
        "precision mediump float;"
        "void main()"
        "{"
        "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
        "}";

    OpenGLES_Renderer::OpenGLES_Renderer(const Size2u & viewport)
    {
        GLuint  vertex_shader_id = compile_shader (vertex_shader_code, GL_VERTEX_SHADER);

        if (not vertex_shader_id)
        {
            log.e ("Failed to compile the vertex shader.");
            ok = false;
            return;
        }

        GLuint  fragment_shader_id = compile_shader (fragment_shader_code, GL_FRAGMENT_SHADER);

        if (not fragment_shader_id)
        {
            log.e ("Failed to compile the fragment shader.");
            ok = false;
            return;
        }

        GLuint  program_id = glCreateProgram ();

        if (not program_id)
        {
            log.e ("Failed to create the shader program.");
            ok = false;
            return;
        }

        glAttachShader (program_id,   vertex_shader_id);
        glAttachShader (program_id, fragment_shader_id);

        glBindAttribLocation (program_id, 0, "vertex_position");

        glLinkProgram (program_id);

        GLint succeeded = 0;

        glGetProgramiv (program_id, GL_LINK_STATUS, &succeeded);

        if (!succeeded)
        {
            GLint info_length = 0;

            glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_length);

            if(info_length > 1)
            {
                char * info_log = new char[info_length];

                glGetProgramInfoLog(program_id, info_length, NULL, info_log);

                log.e (info_log);

                delete [] info_log;
            }
            else
                log.e ("Failed to link the shader program.");

            glDeleteProgram (program_id);

            ok = false;
            return;
        }

        view_width  = viewport.width;
        view_height = viewport.height;

        glUseProgram (program_id);
        glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
        glViewport   (0, 0, viewport.width, viewport.height);
    }

    void OpenGLES_Renderer::set_view_size (const Size2u & view)
    {
        view_width  = float(view.width );
        view_height = float(view.height);
    }

    void OpenGLES_Renderer::clear ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLES_Renderer::draw_rectangle (float left_x, float bottom_y, float width, float height)
    {
        float right_x =   left_x + width;
        float   top_y = bottom_y + height;

        left_x   = (left_x   - view_width  / 2.f) / view_width  * 2.f;
        right_x  = (right_x  - view_width  / 2.f) / view_width  * 2.f;
        top_y    = (top_y    - view_height / 2.f) / view_height * 2.f;
        bottom_y = (bottom_y - view_height / 2.f) / view_height * 2.f;

        const GLfloat vertices[] =
        {
            left_x,  top_y,    0.0f,
            right_x, top_y,    0.0f,
            right_x, bottom_y, 0.0f,
            left_x,  top_y,    0.0f,
            right_x, bottom_y, 0.0f,
            left_x,  bottom_y, 0.0f,
        };

        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray (0);
        glDrawArrays (GL_TRIANGLES, 0, 6);
    }

    GLuint OpenGLES_Renderer::compile_shader (const char * source_code, GLenum type)
    {
        GLuint shader_id = glCreateShader (type);

        if (not shader_id) return 0;

        glShaderSource  (shader_id, 1, &source_code, nullptr);
        glCompileShader (shader_id);

        GLint succeeded;

        glGetShaderiv (shader_id, GL_COMPILE_STATUS, &succeeded);

        if (!succeeded)
        {
            GLint info_length = 0;

            glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_length);

            if (info_length > 1)
            {
                char * info_log = new char[info_length];

                glGetShaderInfoLog (shader_id, info_length, nullptr, info_log);

                log.e (info_log);

                delete [] info_log;
            }

            glDeleteShader(shader_id);

            return 0;
        }

        return shader_id;
    }

}
