/*
 * INTRO SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef MENU_SCENE_HEADER
#define MENU_SCENE_HEADER

#include <memory>
#include <basics/Scene>
#include <basics/Atlas>
#include <basics/Point>
#include <basics/Timer>

    namespace example {

        using basics::Atlas;
        using basics::Point2f;
        using basics::Timer;
        using basics::Graphics_Context;

        class Menu_Scene : public basics::Scene{

            enum State{
                LOADING,
                READY,
                FINISHED,
                ERROR
            };

            enum optionID{
                PLAY,
                HELP,
                CREDITS
            };

            struct Option{
                const Atlas::Slice * slice;
                Point2f position;
                float isPressed;
            };

            static const unsigned nOptions = 4;

        private:

            State state;
            bool suspended;

            unsigned canvasWidth;
            unsigned canvasHeight;

            Timer timer;

            Option options[nOptions];

            std::unique_ptr< Atlas > atlas;


        public:

            Menu_Scene();

            basics::Size2u get_view_size () override {
                return {canvasWidth, canvasHeight};
            };

            bool initialize() override;

            void suspend() override{
                suspended = true;
            }

            void resume() override{
                suspended = false;
            }

            void handle(basics::Event & event) override;

            void update (float time) override;

            void render (Graphics_Context::Accessor & context) override;

        private:

            void configureOptions();

            int optionAt (const Point2f & point);
        };
       

    }

#endif
