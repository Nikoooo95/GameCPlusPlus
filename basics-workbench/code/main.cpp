/*
 * MAIN
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Director>
#include <basics/OpenGLES_Context>
#include <basics/OpenGLES_Renderer>
#include <basics/Window>
#include "Sample_Scene.hpp"

using namespace basics;
using namespace example;
using namespace std;

int main ()
{
    director.run_scene
    (
        shared_ptr< Scene >
        (
            new Sample_Scene(Size2u{800, 480})
        )
    );

    return 0;
}

void keep_link ()
{
    const bool &c = Window::can_be_instantiated;
    Window::Accessor window;
    OpenGLES_Context::create(window);
    OpenGLES_Renderer x(Size2u{0, 0});
}