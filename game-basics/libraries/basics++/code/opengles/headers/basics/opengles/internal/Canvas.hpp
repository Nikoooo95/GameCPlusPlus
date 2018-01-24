/*
 * CANVAS
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef BASICS_OPENGLES_CANVAS_HEADER
#define BASICS_OPENGLES_CANVAS_HEADER

    #include <basics/Canvas>
    #include <basics/Size>

    namespace basics { namespace opengles
    {

        class Canvas : public basics::Canvas
        {

            Size2u size;

        public:

            Canvas(const Size2u & viewport);

        };

    }}

#endif
