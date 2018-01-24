/*
 * OPENGL ES 2 CANVAS
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Transformation>
#include <basics/opengles/OpenGL_ES2>
#include <basics/opengles/Canvas_ES2>
#include <basics/opengles/Shader_Program>
#include <basics/opengles/Texture_2D>

// glTexCoordPointer (2, GL_FLOAT, 0, tex_coords);

namespace basics { namespace opengles
{

    const char * Canvas_ES2::internal_vertex_shader_f =
        "precision mediump float;"
        "uniform   mat3 transform;"
        "uniform   mat3 projection;"
        "attribute vec2 vertex_position;"
        "void main()"
        "{"
            "gl_Position = vec4((vec3(vertex_position, 1.0) * transform * projection).xy, 0.0, 1.0);"
        "}";

    const char * Canvas_ES2::internal_vertex_shader_t =
        "precision mediump float;"
        "uniform   mat3 transform;"
        "uniform   mat3 projection;"
        "attribute vec2 vertex_position;"
        "attribute vec2 vertex_texture_uv;"
        "varying   vec2 varying_uv;"
        "void main()"
        "{"
            "varying_uv  = vertex_texture_uv;"
            "gl_Position = vec4((vec3(vertex_position, 1.0) * transform * projection).xy, 0.0, 1.0);"
        "}";

    const char * Canvas_ES2::internal_fragment_shader_f =
        "precision mediump float;"
        "uniform vec4 color;"
        "void main()"
        "{"
            "gl_FragColor = color;"
        "}";

    const char * Canvas_ES2::internal_fragment_shader_t =
        "precision mediump   float;"
        "uniform   sampler2D sampler;"
        "varying   vec2      varying_uv;"
        "void main()"
        "{"
            "gl_FragColor = texture2D (sampler, varying_uv);"
        "}";

    Canvas * Canvas_ES2::create (Id id, Graphics_Context::Accessor & context, const Options & options)
    {
        std::shared_ptr< Canvas >  canvas(new Canvas_ES2(context, options.size));

        context->add (id, canvas);

        return canvas.get ();
    }

    Canvas_ES2::Canvas_ES2(Graphics_Context::Accessor & context, const Size2u & size)
    :
        size{ float(size.width), float(size.height) }
    {
        shader_program_f.reset (new Shader_Program);

        shader_program_f->add (Shader::Source_Code::from_string (internal_vertex_shader_f,   Shader::Source_Code::VERTEX  ));
        shader_program_f->add (Shader::Source_Code::from_string (internal_fragment_shader_f, Shader::Source_Code::FRAGMENT));

        context->add (shader_program_f);

        if (shader_program_f->is_usable ())
        {
            shader_program_f->use ();

             transform_f_id = shader_program_f->get_uniform_id ("transform" );
            projection_f_id = shader_program_f->get_uniform_id ("projection");
                 color_f_id = shader_program_f->get_uniform_id ("color"     );
        }

        shader_program_t.reset (new Shader_Program);

        shader_program_t->add (Shader::Source_Code::from_string (internal_vertex_shader_t,   Shader::Source_Code::VERTEX  ));
        shader_program_t->add (Shader::Source_Code::from_string (internal_fragment_shader_t, Shader::Source_Code::FRAGMENT));

        context->add (shader_program_t);

        if (shader_program_t->is_usable ())
        {
            shader_program_t->use ();

             transform_t_id = shader_program_t->get_uniform_id ("transform" );
            projection_t_id = shader_program_t->get_uniform_id ("projection");
               sampler_t_id = shader_program_t->get_uniform_id ("sampler"   );
               //color_t_id = shader_program_t->get_uniform_id ("color"     );

              vertex_position_location_t = shader_program_t->get_vertex_attribute_id ("vertex_position"  );
            vertex_texture_uv_location_t = shader_program_t->get_vertex_attribute_id ("vertex_texture_uv");

            shader_program_t->set_uniform_value (sampler_t_id, 0);
        }

        glEnable      (GL_BLEND);
        glBlendFunc   (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor  (0.f, 0.f, 0.f, 1.f);

        color[3] = 1.f;

        set_size      (size);
        set_transform (Transformation2f());
        set_color     (1.f, 1.f, 1.f);
        set_viewport  ({ 0, 0 }, { context->get_surface_width (), context->get_surface_height () });
    }

    void Canvas_ES2::set_size (const Size2u & new_viewport_size)
    {
        size.width  = float(new_viewport_size.width );
        size.height = float(new_viewport_size.height);
        half_size   = size * 0.5f;
        projection  = translate_then_scale_2d (Vector2f{ -half_size.width, -half_size.height }, 2.f / size.width, 2.f / size.height);

        shader_program_f->use ();
        shader_program_f->set_uniform_value (projection_f_id, projection.matrix);

        shader_program_t->use ();
        shader_program_t->set_uniform_value (projection_t_id, projection.matrix);
    }

    void Canvas_ES2::set_viewport (const Point2u & bottom_left, const Size2u & size)
    {
        glViewport (bottom_left[0], bottom_left[1], size.width, size.height);
    }

    void Canvas_ES2::set_clear_color (float r, float g, float b)
    {
        glClearColor (r, g, b, 1.f);
    }

    void Canvas_ES2::set_color (float r, float g, float b)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;

        shader_program_f->use ();
        shader_program_f->set_uniform_value (color_f_id, color);

      //shader_program_t->use ();
      //shader_program_t->set_uniform_value (color_t_id, color);
    }

    void Canvas_ES2::set_transform (const Transformation2f & new_transform)
    {
        transform = new_transform;

        shader_program_f->use ();
        shader_program_f->set_uniform_value (transform_f_id, transform.matrix);

        shader_program_t->use ();
        shader_program_t->set_uniform_value (transform_t_id, transform.matrix);
    }

    void Canvas_ES2::apply_transform (const Transformation2f & t)
    {
        transform = t * transform;

        shader_program_f->use ();
        shader_program_f->set_uniform_value (transform_f_id, transform.matrix);

        shader_program_t->use ();
        shader_program_t->set_uniform_value (transform_t_id, transform.matrix);
    }

    void Canvas_ES2::clear ()
    {
        glClear (GL_COLOR_BUFFER_BIT);
    }

    void Canvas_ES2::draw_point (const Point2f & position)
    {
        shader_program_f->use ();

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, position.coordinates);
        glDrawArrays               (GL_POINTS, 0, 1);
    }

    void Canvas_ES2::draw_segment (const Point2f & a, const Point2f & b)
    {
        shader_program_f->use ();

        const Point2f coordinates[] = { a, b };

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays               (GL_LINES, 0, 2);
    }

    void Canvas_ES2::draw_triangle (const Point2f & a, const Point2f & b, const Point2f & c)
    {
        shader_program_f->use ();

        const Point2f coordinates[] = { a, b, c, a };

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays               (GL_LINE_STRIP, 0, 4);
    }

    void Canvas_ES2::fill_triangle (const Point2f & a, const Point2f & b, const Point2f & c)
    {
        shader_program_f->use ();

        const Point2f coordinates[] = { a, b, c };

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays               (GL_TRIANGLES, 0, 3);
    }

    void Canvas_ES2::draw_rectangle (const Point2f & bottom_left, const Size2f & size)
    {
        shader_program_f->use ();

        Point2f top_right{ bottom_left.coordinates.x () + size.width, bottom_left.coordinates.y () + size.height };

        const Point2f coordinates[] =
        {
              bottom_left,
            {   top_right.coordinates.x (), bottom_left.coordinates.y () },
                top_right,
            { bottom_left.coordinates.x (),   top_right.coordinates.y () },
              bottom_left
        };

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays               (GL_LINE_STRIP, 0, 5);
    }

    void Canvas_ES2::fill_rectangle (const Point2f & bottom_left, const Size2f & size)
    {
        shader_program_f->use ();

        Point2f top_right{ bottom_left.coordinates.x () + size.width, bottom_left.coordinates.y () + size.height };

        const Point2f coordinates[] =
        {
              bottom_left,
            { bottom_left.coordinates.x (),   top_right.coordinates.y () },
            {   top_right.coordinates.x (), bottom_left.coordinates.y () },
                top_right,
        };

        glEnableVertexAttribArray  (0);
        glDisableVertexAttribArray (1);
        glVertexAttribPointer      (0, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
        glDrawArrays               (GL_TRIANGLE_STRIP, 0, 4);
    }

    void Canvas_ES2::fill_rectangle (const Point2f & bottom_left, const Size2f & size, const basics::Texture_2D * texture)
    {
        const opengles::Texture_2D * opengl_es_texture = dynamic_cast< const opengles::Texture_2D * >(texture);

        if (opengl_es_texture)
        {
            opengl_es_texture->use ();
            shader_program_t ->use ();

            Point2f top_right{ bottom_left.coordinates.x () + size.width, bottom_left.coordinates.y () + size.height };

            const Point2f coordinates[] =
            {
                bottom_left,
                { bottom_left.coordinates.x (),   top_right.coordinates.y () },
                {   top_right.coordinates.x (), bottom_left.coordinates.y () },
                top_right,
            };

            static const Point2f texture_uvs[] =
            {
                { 0.f, 1.f },
                { 0.f, 0.f },
                { 1.f, 1.f },
                { 1.f, 0.f },
            };

            /* NOT WORKING...
            shader_program_t->set_vertex_attribute_id ("vertex_position",   0);
            shader_program_t->set_vertex_attribute_id ("vertex_texture_uv", 1);
            int p = shader_program_t->get_vertex_attribute_id ("vertex_position");
            int x = shader_program_t->get_vertex_attribute_id ("vertex_texture_uv");*/

            glEnableVertexAttribArray (  vertex_position_location_t);
            glEnableVertexAttribArray (vertex_texture_uv_location_t);
            glVertexAttribPointer     (  vertex_position_location_t, 2, GL_FLOAT, GL_FALSE, 0, coordinates);
            glVertexAttribPointer     (vertex_texture_uv_location_t, 2, GL_FLOAT, GL_FALSE, 0, texture_uvs);
            glDrawArrays              (GL_TRIANGLE_STRIP, 0, 4);
        }
    }

}}
