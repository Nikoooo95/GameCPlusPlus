/*
 * CIRCLE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Box.hpp"
#include "World.hpp"
#include <basics/Transformation>

using namespace basics;

namespace example
{

    Box::Box(World & world, Type type, const Size2f & graphics_size, float physics_to_graphics_scale, Texture_2D * texture)
    :
        Shape(texture, physics_to_graphics_scale),
        graphics_size(graphics_size)
    {
        b2BodyDef      body_specs;
        b2FixtureDef   body_piece;
        b2PolygonShape body_shape;

        body_specs.type        = b2BodyType(shape_type_to_box2d_type[type]);
        body_piece.shape       = &body_shape;
        body_piece.density     = 1.00f;
        body_piece.restitution = 0.75f;
        body_piece.friction    = 0.50f;

        body_shape.SetAsBox
        (
            graphics_size.width  / physics_to_graphics_scale,
            graphics_size.height / physics_to_graphics_scale
        );

        rigid_body = world.get_physics ().CreateBody (&body_specs);

        rigid_body->CreateFixture (&body_piece);
    }

    void Box::render (Canvas & canvas)
    {
        if (texture)
        {
            // Se toma la posición y orientación del cuerpo rígido (física):

            const Point2f position = get_position ();
            const float   angle    = get_angle    ();

            // Se establece el transform al canvas para que lo aplique a lo que se dibuja a continuación:

            canvas.set_transform  (rotate_then_translate_2d (angle, Vector2f{ position[0], position[1] }));

            // Se dibuja la textura rectangular:

            canvas.fill_rectangle ({ 0, 0 }, graphics_size, texture, BOTTOM | LEFT);
        }
    }

}
