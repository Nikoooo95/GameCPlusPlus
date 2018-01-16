/*
 * SIZE
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712250337
 */

#ifndef BASICS_SIZE_HEADER
#define BASICS_SIZE_HEADER

    namespace basics
    {

        template< unsigned DIMENSION, typename NUMERIC_TYPE >
        struct Size;

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 1, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;

            Numeric_Type length;
        };

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 2, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;

            Numeric_Type width;
            Numeric_Type height;
        };

        // -----------------------------------------------------------------------------------------

        template< typename NUMERIC_TYPE >
        struct Size< 3, NUMERIC_TYPE >
        {
            typedef NUMERIC_TYPE Numeric_Type;

            Numeric_Type width;
            Numeric_Type height;
            Numeric_Type depth;
        };

        // -----------------------------------------------------------------------------------------

        typedef Size< 1, int      > Size1i;
        typedef Size< 1, unsigned > Size1u;
        typedef Size< 1, float    > Size1f;
        typedef Size< 1, double   > Size1d;

        typedef Size< 2, int      > Size2i;
        typedef Size< 2, unsigned > Size2u;
        typedef Size< 2, float    > Size2f;
        typedef Size< 2, double   > Size2d;

        typedef Size< 3, int      > Size3i;
        typedef Size< 3, unsigned > Size3u;
        typedef Size< 3, float    > Size3f;
        typedef Size< 3, double   > Size3d;

    }

#endif
