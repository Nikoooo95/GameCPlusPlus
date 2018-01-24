/*
 * PNG DECODE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "lodepng.h"
#include <basics/png_decode>

namespace basics
{

    bool png_decode (const std::vector< byte > & encoded_data, Color_Buffer< Rgba8888 > & color_buffer)
    {
        std::vector< byte > decoded_data;
        unsigned            decoded_width;
        unsigned            decoded_height;

        int error = lodepng::decode
        (
            decoded_data,
            decoded_width,
            decoded_height,
            encoded_data,
            LCT_RGBA,
            8
        );

        if (!error)
        {
            color_buffer.resize (decoded_width, decoded_height);

            byte * buffer = color_buffer;

            for (auto i = decoded_data.begin (); i != decoded_data.end (); ++i)
            {
                *buffer++ = *i;
            }

            return true;
        }

        return false;
    }

}
