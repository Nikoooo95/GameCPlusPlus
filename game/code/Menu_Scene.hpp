/*
 * MENU SCENE
 * Copyright © 2018+ Nicolás Tapia Sanz
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * nic.tap95@gmail.com
 */


#ifndef MENU_SCENE_HEADER
#define MENU_SCENE_HEADER

#include <memory>
#include <basics/Scene>
#include <basics/Atlas>
#include <basics/Canvas>
#include <basics/Point>
#include <basics/Graphics_Context>

namespace example {

        using basics::Canvas;
        using basics::Atlas;
        using basics::Point2f;
        using basics::Graphics_Context;

        class Menu_Scene : public basics::Scene{

            enum State{
                LOADING,
                RUNNING,
                HELPING,
                ERROR
            };

            enum spritesID{
                BACKGROUND,
                TITLE,
                TEXT_HELP
            };

            enum optionID{
                PLAY,
                HELP,
                EXIT,
                BACK
            };

            struct Element{
                const Atlas::Slice * slice;
                Point2f position;
            };

            struct Option{
                const Atlas::Slice * slice;
                Point2f position;
                float isPressed;
            };

            static const unsigned nOptions = 4;
            static const unsigned nSprites = 3;

        private:

            State state;
            bool suspended;

            unsigned canvas_width;
            unsigned canvas_height;

            Option options[nOptions];
            Element sprites[nSprites];

            typedef basics::Graphics_Context::Accessor Context;
            std::unique_ptr< Atlas > atlas;


        public:

            Menu_Scene();

            basics::Size2u get_view_size () override {
                return {canvas_width, canvas_height};
            };

            void suspend() override{
                suspended = true;
            }

            void resume() override{
                suspended = false;
            }

            bool initialize() override;

             void handle(basics::Event & event) override;

            void update (float time) override;

            void render (Graphics_Context::Accessor & context) override;

        private:

            int optionAt (const Point2f & point);

            void load_textures();

            void create_sprites();

            void update_helping();

            void update_menu();

            void render_menu(Canvas & canvas);

            void render_help(Canvas & canvas);
        };
       

    }

#endif
