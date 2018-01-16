/*
 * GRAPHICS CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712202208
 */

#ifndef BASICS_GRAPHICS_CONTEXT_HEADER
#define BASICS_GRAPHICS_CONTEXT_HEADER

    #include <memory>
    #include <mutex>
    #include <utility>

    #include <basics/declarations>
    #include <basics/types>

    namespace basics
    {

        class Graphics_Context
        {
        public:

            class Accessor
            {

                // El contexto puede ser eliminado en cualquier momento que no esté lockeado.
                // El mutex no puede ser eliminado mientras tenga algún lock pendiente en algún hilo.
                // Como el mutex pertenece a la ventana, este perdurará más que el contexto.
                // Cuando el contexto se elimine, quien haya iniciado un lock tiene una copia del
                // mutex, pero el weak_ptr dirá que el contexto se eliminó.
                // Mientras algún hilo tenga el lock, el contexto no se puede eliminar, pero sí
                // mientras esté esperando a un lock.
                // Mientras el accesor exista, este mantiene el lock del mutex.

                std::shared_ptr < Graphics_Context > context;
                std::unique_lock< std::mutex       > lock;

            public:

                Accessor() = default;
                Accessor(Accessor && ) = default;
                Accessor(const Accessor & ) = delete;

                // 1. Se crea un observer (weak_ptr) del shared_ptr del contexto.
                // 2. Se intenta bloquear el mutex, lo cual puede detener la ejecución.
                // 3. Cuando se consigue el lock del mutex, se pregunta al observer si el contexto existe todavía.

                Accessor(const std::weak_ptr< Graphics_Context > & context_observer, std::mutex & mutex)
                :
                    lock(mutex)
                {
                    context = context_observer.lock ();
                }

                Accessor
                (
                    const std::weak_ptr< Graphics_Context > & context_observer,
                    std::mutex & mutex,
                    const std::try_to_lock_t &
                )
                :
                    lock(mutex, std::try_to_lock)
                {
                    if (lock.owns_lock ())
                    {
                        context = context_observer.lock ();
                    }
                }

                virtual ~Accessor()
                {
                    // Es necesario eliminar la referencia al contexto antes de que se suelte el lock
                    // The reference to the context must be released before the lock is released:

                    if (context) context.reset ();
                }

            public:

                bool has_context () const
                {
                    return context.get ();
                }

                bool owns_lock () const
                {
                    return lock.owns_lock ();
                }

            public:

                Graphics_Context * operator -> ()
                {
                    return context.get ();
                }

                const Graphics_Context * operator -> () const
                {
                    return context.get ();
                }

            public:

                operator bool () const
                {
                    return has_context () && context->is_available () && owns_lock ();
                }

            };

        protected:

            Window & window;

        protected:

            Graphics_Context(Window & window) : window(window)
            {
            }

            virtual ~Graphics_Context() = default;

        public:

            virtual void invalidate () = 0;
            virtual void suspend () = 0;
            virtual bool resume () = 0;
            virtual void finalize () = 0;

            virtual bool is_available () const = 0;
            virtual bool is_current () const = 0;
            virtual bool make_current () = 0;
            virtual bool set_sync_swap (bool activated) = 0;
            virtual bool flush_and_display () = 0;
			
			virtual unsigned get_surface_width () = 0;
			virtual unsigned get_surface_height () = 0;

        };

    }

#endif
