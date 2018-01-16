/*
 *  DIRECTOR
 *  Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 *  Distributed under the Boost Software License, version  1.0
 *  See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 *  angel.rodriguez@esne.edu
 *
 *  C1801072242
 */

#ifndef BASICS_DIRECTOR_HEADER
#define BASICS_DIRECTOR_HEADER

    #include <memory>
    #include <basics/Event_Queue>

    namespace basics
    {

        class Scene;
        class Renderer;

        class Director final
        {
        public:

            static Director & get_instance ()
            {
                static Director director;
                return director;
            }

        private:

            struct
            {
                bool running;
                bool exit;
            }
            kernel;

            struct State
            {
                bool active   = false;
                bool focused  = false;
                bool graphics = false;

                operator bool () const
                {
                    return active && focused && graphics;
                }
            }
            state;

            std::shared_ptr< Scene    > current_scene;
            std::shared_ptr< Renderer > renderer;

            Event_Queue event_queue;

            float surface_width;
            float surface_height;

        private:

            Director();

        public:

            void run_scene (const std::shared_ptr< Scene > & new_scene);

            void stop ()
            {
                kernel.exit = kernel.running;
            }

            void handle_event (const Event & event)
            {
                event_queue.push (event);
            }

        private:

            void run_kernel ();

        };

        extern Director & director;

    }

#endif
