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
#include <basics/Size>

namespace example
{

    class Box : public Shape
    {

        basics::Size2f graphics_size;

    public:

        Box(World & world, Type type, const basics::Size2f & physics_size, float physics_to_graphics_scale, Texture_2D * texture = nullptr);

        virtual ~Box() = default;

        void render (Canvas & canvas) override;

    };

}
