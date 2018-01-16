/*
 * DIRECTOR
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1801072305
 */

#include <chrono>

#include <basics/Application>
#include <basics/Director>
#include <basics/Log>
#include <basics/OpenGLES_Context>
#include <basics/OpenGLES_Renderer>
#include <basics/Scene>
#include <basics/Window>

using std::chrono::high_resolution_clock;

namespace basics
{

    Director & director = Director::get_instance ();

    // ---------------------------------------------------------------------------------------------

    Director::Director()
    {
        kernel.running = false;
        kernel.exit    = false;
    }

    // ---------------------------------------------------------------------------------------------

    void Director::run_scene (const std::shared_ptr< Scene > & new_scene)
    {
        if (new_scene)
        {
            (current_scene = new_scene)->initialize ();

            if (!kernel.running)
            {
                run_kernel ();
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Director::run_kernel ()
    {
        kernel.running = true;
        kernel.exit    = false;

        Window::Handle window_handle;

        if (Window::can_be_instantiated)
        {
            Window::create_window (default_window_id);
        }

        float time = current_scene->get_frame_duration ();
        Event event;

        if (time < 0.f) time = 1.f / 60.f;

        do
        {
            high_resolution_clock::time_point start_time = high_resolution_clock::now ();

            State previous_state = state;

            while (application.poll (event))
            {
                switch (event.id)
                {
                    case Application::Event_Id::RESUME:
                    {
                        state.active = true;
                        break;
                    }

                    case Application::Event_Id::SUSPEND:
                    {
                        state.active = false;
                        break;
                    }

                    case Application::Event_Id::WINDOW_CREATED:
                    {
                        window_handle = Window::get_window (default_window_id);

                        Window::Accessor window = window_handle.lock ();

                        if (!window->has_graphics_context ())
                        {
                            if (!OpenGLES_Context::create (window))
                            {
                                log.e ("ERROR: failed to initialize the OpenGL ES context!");

                                return;
                            }
                        }

                        Graphics_Context::Accessor graphics_context = window->lock_graphics_context ();

                        if (graphics_context)
                        {
                            surface_width  = graphics_context->get_surface_width  ();
                            surface_height = graphics_context->get_surface_height ();

                            renderer.reset(new OpenGLES_Renderer(Size2u{ unsigned(surface_width), unsigned(surface_height) }));
                        }

                        state.graphics = true;

                        break;
                    }

                    case Application::Event_Id::WINDOW_DESTROYED:
                    {
                        state.graphics = false;

                        renderer.reset ();

                        break;
                    }

                    case Application::Event_Id::QUIT:
                    {
                        kernel.exit = true;
                        break;
                    }
                }
            }

            if (!kernel.exit)
            {
                Window::Accessor window = window_handle.lock ();

                if (window)
                {
                    while (window->poll (event))
                    {
                        switch (event.id)
                        {
                            case Window::GOT_FOCUS:
                            {
                                state.focused = true;
                                break;
                            }

                            case Window::LOST_FOCUS:
                            {
                                state.focused = false;
                                break;
                            }

                            case Window::LOST_GRAPHICS_CONTEXT:
                            {
                                break;
                            }
                        }
                    }

                    if (current_scene)
                    {
                        bool previously_active = previous_state;
                        bool  currently_active = state;

                        if (!previously_active &&  currently_active) current_scene->resume  (); else
                        if ( previously_active && !currently_active) current_scene->suspend ();

                        if (currently_active)
                        {
                            Size2u scene_view_size = current_scene->get_view_size ();

                            float  h_ratio = float(scene_view_size.width ) / surface_width;
                            float  v_ratio = float(scene_view_size.height) / surface_height;

                            while (event_queue.poll (event))
                            {
                                switch (event.id)
                                {
                                    case FNV(touch-started):
                                    case FNV(touch-moved):
                                    case FNV(touch-ended):
                                    {
                                        float x = *event.properties[FNV(x)].as< var::Float > ();
                                        float y = *event.properties[FNV(y)].as< var::Float > ();

                                        event.properties[FNV(x)] = x * h_ratio;
                                        event.properties[FNV(y)] = (surface_height - y) * v_ratio;

                                        break;
                                    }
                                }
                                current_scene->handle (event);
                            }
                            current_scene->update (time);

                            Graphics_Context::Accessor graphics_context = window->lock_graphics_context ();

                            if (graphics_context)
                            {
                                renderer->set_view_size (current_scene->get_view_size());

                                renderer->clear ();

                                current_scene->render (*renderer);

                                graphics_context->flush_and_display ();
                            }
                        }
                    }
                }
            }

            time = std::chrono::duration_cast< std::chrono::seconds >
            (
                high_resolution_clock::now () - start_time
            )
            .count ();
        }
        while (!kernel.exit && current_scene);

        kernel.running = false;
    }

}
