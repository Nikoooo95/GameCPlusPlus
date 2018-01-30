/*
 * SHAPE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Shape.hpp"
#include <Box2D/Box2D.h>

namespace example
{

    const int Shape::shape_type_to_box2d_type[]
    {
        b2_staticBody,
        b2_dynamicBody,
        b2_kinematicBody
    };

    void Shape::set_position (const Point2f & new_position)
    {
        // Se ajusta el transform de el cuerpo rígido de Box2d convirtiendo antes las coordenadas de
        // virtuales a físicas (metros):

        rigid_body->SetTransform
        (
            b2Vec2{ new_position[0] / physics_to_graphics_scale, new_position[1] / physics_to_graphics_scale },
            rigid_body->GetAngle ()
        );
    }

    void Shape::set_angle (float new_angle)
    {
        rigid_body->SetTransform
        (
            rigid_body->GetTransform ().p,
            new_angle
        );
    }

    Point2f Shape::get_position () const
    {
        const b2Transform & transform = rigid_body->GetTransform ();

        // Se retornan las coordenadas físicas (metros) convertidas a virtuales:

        return Point2f
        {
            transform.p.x * physics_to_graphics_scale,
            transform.p.y * physics_to_graphics_scale
        };
    }

    float Shape::get_angle () const
    {
        return rigid_body->GetAngle ();
    }

};
