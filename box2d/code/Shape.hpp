/*
 * SHAPE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#pragma once

#include <memory>
#include <basics/Canvas>
#include <basics/Texture_2D>
#include <basics/Point>

class b2Body;

namespace example
{

    class World;
    using basics::Canvas;
    using basics::Point2f;
    using basics::Texture_2D;

    /**
     * Es la clase base de diversas formas geométricas 2D (círculo, caja, etc.) que se controlan
     * usando un motor de física (Box2D).
     */
    class Shape
    {
    public:

        enum Type
        {
            STATIC,
            DYNAMIC,
            KINEMATIC
        };

    protected:

        static const int shape_type_to_box2d_type[];

    protected:

        b2Body     * rigid_body;                        ///< Puntero al cuerpo rígido de Box2D
        Texture_2D * texture;                           ///< Puntero a la textura con la imagen (si es nullptr no se dibuja nada)
        float        physics_to_graphics_scale;         ///< Factor de escala para convertir de metros (física) a unidades virtuales

    public:

        Shape(Texture_2D * texture, float physics_to_graphics_scale)
        :
            texture(texture),
            physics_to_graphics_scale(physics_to_graphics_scale)
        {
            rigid_body = nullptr;
        }

        virtual ~Shape() = default;

        /**
         * Establece la posición de la shape.
         * @param new_position Nueva posición en coordenadas virtuales.
         */
        void set_position (const Point2f & new_position);

        /**
         * Establece la orientación de la shape.
         * @param new_angle Nuevo ángulo en radianes.
         */
        void set_angle (float new_angle);

        /**
         * Devuelve la posición actual de la shape.
         * @return Posición actual en coordenadas virtuales.
         */
        Point2f get_position () const;

        /**
         * Devuelve la orientación actual de la shape.
         * @return Orientiación actual en radianes.
         */
        float get_angle  () const;

        /**
         * Método que se puede reimplementar opcionalmente para actualizar algo después de actualizarse
         * la física.
         */
        virtual void update (float ) { }

        /**
         * Método que debe implementar cada clase que hereda de Shape para dibujarse.
         * @param canvas Referencia al canvas que permite dibujar la shape.
         */
        virtual void render (Canvas & canvas) = 0;

    };

}
