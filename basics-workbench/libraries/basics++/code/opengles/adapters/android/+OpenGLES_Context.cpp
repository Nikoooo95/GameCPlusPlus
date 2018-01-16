/*
 * OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712202250
 */

#include <basics/macros>

#if defined(BASICS_ANDROID_OS)

    #include "Android_OpenGLES_Context.hpp"
    #include "../../../base/adapters/android/Native_Window.hpp"

    namespace basics
    {

        bool OpenGLES_Context::create (Window::Accessor & window)
        {
            if (window && window->is_available () && !window->has_graphics_context ())
            {
                std::shared_ptr< Graphics_Context > context
                (
                    new internal::Android_OpenGLES_Context
                    (
                        *static_cast< internal::Native_Window::Accessor & >(window).get ()
                    )
                );

                if (context->is_available () && window->set_graphics_context (context))
                {
                    return context->make_current ();
                }
            }

            return false;
        }

    };

#endif
