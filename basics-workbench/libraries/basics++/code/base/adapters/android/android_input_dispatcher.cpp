/*
 * ANDROID INPUT DISPATCHER
 * Copyright © 2017+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 *
 * C1712291616
 */

#include <basics/macros>
#include <basics/Director>

#if defined(BASICS_ANDROID_OS)

    #include <android/input.h>

#include <sstream>
#include <basics/Log>

    namespace basics { namespace internal
    {

        int handle_motion_event (AInputEvent * android_event)
        {
            switch (AInputEvent_getSource (android_event))
            {
                case AINPUT_SOURCE_TOUCHSCREEN:
                {
                    float x = AMotionEvent_getX (android_event, 0);
                    float y = AMotionEvent_getY (android_event, 0);

                    switch (AKeyEvent_getAction (android_event))
                    {
                        case AMOTION_EVENT_ACTION_DOWN:
                        {
                            Event event(FNV(touch-started));

                            event.properties[FNV(x)] = x;
                            event.properties[FNV(y)] = y;

                            director.handle_event (event);

                            break;
                        }

                        case AMOTION_EVENT_ACTION_MOVE:
                        {
                            Event event(FNV(touch-moved));

                            event.properties[FNV(x)] = x;
                            event.properties[FNV(y)] = y;

                            director.handle_event (event);

                            break;
                        }

                        case AMOTION_EVENT_ACTION_UP:
                        {
                            Event event(FNV(touch-ended));

                            event.properties[FNV(x)] = x;
                            event.properties[FNV(y)] = y;

                            director.handle_event (event);

                            break;
                        }
                    }
                    /*std::ostringstream o;

                    o << "MOTION EVENT AT " << AMotionEvent_getX( event, 0 ) << ", " << AMotionEvent_getY( event, 0 );

                    log.d (o.str ());*/
                }
            }

            return true;
        }

        int handle_key_event (AInputEvent * event)
        {
            return false;
        }

        void android_input_dispatcher (AInputQueue  * input_queue)
        {
            if (input_queue)
            {
                AInputEvent * event = nullptr;

                while (AInputQueue_hasEvents (input_queue) == 1)
                {
                    if (AInputQueue_getEvent (input_queue, &event) >= 0 && AInputQueue_preDispatchEvent (input_queue, event) == 0)
                    {
                        int handled;

                        switch (AInputEvent_getType (event))
                        {
                            case AINPUT_EVENT_TYPE_MOTION: handled = handle_motion_event (event); break;
                            case AINPUT_EVENT_TYPE_KEY:    handled = handle_key_event    (event); break;
                            default:
                            {
                                handled = false;
                            }
                        }

                        AInputQueue_finishEvent (input_queue, event, handled);
                    }
                }
            }
        }

    }}

#endif
