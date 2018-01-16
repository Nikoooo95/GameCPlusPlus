/*
 * ANDROID OPENGL ES CONTEXT
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712202240
 */

#ifndef BASICS_ANDROID_OPENGL_CONTEXT_HEADER
#define BASICS_ANDROID_OPENGL_CONTEXT_HEADER

    #include <atomic>
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <basics/OpenGLES_Context>

    namespace basics { namespace internal
    {

        using std::atomic;

        class Native_Window;

        class Android_OpenGLES_Context final : public OpenGLES_Context
        {

            ANativeWindow * native_window;
            EGLDisplay      display;
            EGLSurface      surface;
            EGLContext      context;
            EGLConfig       config;

            atomic< bool >  initialized;
            atomic< bool >  available;
			
			EGLint   		surface_width;
			EGLint  		surface_height;

        public:

            Android_OpenGLES_Context(Native_Window & window);

           ~Android_OpenGLES_Context()
            {
                finalize ();
            }

        public:

            bool is_available () const override
            {
                return available;
            }

            void invalidate () override
            {
                available = false;
            }

            void suspend () override;
            bool resume () override;
            void finalize () override;

            bool is_current () const override;
            bool make_current () override;

            bool set_sync_swap (bool activated) override;
            bool flush_and_display () override;

			unsigned get_surface_width () override
			{
				return unsigned(surface_width);
			}
			
			unsigned get_surface_height () override
			{
				return unsigned(surface_height);
			}
			
        private:

            bool initialize_display ();
            bool initialize_surface ();
            bool initialize_context ();

            void finalize_display ();
            void finalize_surface ();
            void finalize_context ();

        };

    }}

#endif
