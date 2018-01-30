/*
 * WORLD
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "World.hpp"
#include "Shape.hpp"

namespace example
{

    void World::update (float time)
    {
        // Se actualiza la física de todas las shapes (Box2D):

        physics.Step (time, 5, 5);

        // Se invoca al método update de dada shape individualmente por si necesita hacer algo:

        for (auto & key_value : shapes)
        {
            key_value.second->update (time);
        }
    }

    void World::render (Canvas & canvas)
    {
        for (auto & key_value : shapes)
        {
            key_value.second->render (canvas);
        }
    }

}
