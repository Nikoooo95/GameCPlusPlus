/*
 * WORLD
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#pragma once

#include <map>
#include <memory>
#include <Box2D/Box2D.h>

#include <basics/Id>
#include <basics/assert>
#include <basics/Canvas>

namespace example
{

    class Shape;
    using basics::Id;
    using basics::Canvas;

    class World
    {

        typedef std::shared_ptr< Shape >     Shape_Handle;
        typedef std::map< Id, Shape_Handle > Shape_Map;

    private:

        b2World   physics;                      ///< Manejador de la física
        Shape_Map shapes;                       ///< Mapa de shapes

    public:

        World() : physics(b2Vec2{ 0, -10 })
        {
        }

        /**
         * Permite a las shapes añadir sus cuerpos rígidos al manejador de la física.
         * @return Una referencia al manejador de la física.
         */
        b2World & get_physics ()
        {
            return physics;
        }

        /**
         * Añade una nueva shape asociándole un id para poder acceder a ella más adelante.
         * @param id Id asignado a la shape. Si el ya id existe, se reemplaza a la shape existente.
         * @param shape Puntero a la shape.
         * @return Retorna un puntero a la shape por si se quiere guardar para manipularla directamente.
         */
        Shape * add (Id id, const Shape_Handle & shape)
        {
            assert (shape);
            return (shapes[id] = shape).get ();
        }

        /**
         * Permite acceder a una shape guardada previamente a partir de su id.
         * @param id Id asignado a la shape cuando se añadió.
         * @return Puntero a la shape o nullptr si no existe alguna shape con el id indicado.
         */
        Shape * operator [] (const Id id)
        {
            return shapes.count (id) > 0 ? shapes[id].get () : nullptr;
        }

        /**
         * Actualiza el estado de la física y llama al método update() de las shapes.
         * @param time Tiempo que debe avanzar la física.
         */
        void update (float time);

        /**
         * Llama al método render() de las shapes.
         * @param canvas
         */
        void render (Canvas & canvas);

    };

}
