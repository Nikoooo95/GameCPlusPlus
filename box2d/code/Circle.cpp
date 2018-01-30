/*
 * CIRCLE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Circle.hpp"
#include "World.hpp"
#include <basics/Transformation>

using namespace basics;

namespace example
{

    Circle::Circle(World & world, Type type, float graphics_radius, float physics_to_graphics_scale, Texture_2D * texture)
    :
        Shape(texture, physics_to_graphics_scale)
    {
        b2BodyDef     body_specs;
        b2FixtureDef  body_piece;
        b2CircleShape body_shape;

        body_specs.type        = b2BodyType(shape_type_to_box2d_type[type]);
        body_shape.m_radius    = graphics_radius / physics_to_graphics_scale;
        body_piece.shape       = &body_shape;
        body_piece.density     = 1.00f;
        body_piece.restitution = 0.75f;
        body_piece.friction    = 0.50f;

       (rigid_body        = world.get_physics ().CreateBody (&body_specs))->CreateFixture (&body_piece);
        graphics_diameter = graphics_radius * 2.f;
    }

    void Circle::render (Canvas & canvas)
    {
        // Solo se dibuja si tiene textura:

        if (texture)
        {
            // Se toma la posición y orientación del cuerpo rígido (física):

            const Point2f position = get_position ();
            const float   angle    = get_angle    ();

            // Se establece el transform al canvas para que lo aplique a lo que se dibuja a continuación:

            canvas.set_transform (rotate_then_translate_2d (angle, Vector2f{ position[0], position[1] }));

            // Se dibuja la textura que cubre el círculo (se espera que la textura tenga un dibujo
            // perfectamente circular) aplicando la transformación del canvas:

            canvas.fill_rectangle
            (
                { 0.f, 0.f },
                { graphics_diameter, graphics_diameter },
                texture
            );
        }
    }

}
