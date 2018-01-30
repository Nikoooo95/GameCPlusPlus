/*
 * CIRCLE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#pragma once

#include "Shape.hpp"

namespace example
{

    class Circle : public Shape
    {

        float graphics_diameter;

    public:

        Circle(World & world, Type type, float physics_radius, float physics_to_graphics_scale, Texture_2D * texture = nullptr);

        virtual ~Circle() = default;

        void render (Canvas & canvas) override;

    };

}
