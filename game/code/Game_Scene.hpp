/*
 * GAME SCENE
 * Copyright © 2018+ Nicolás Tapia Sanz
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * nic.tap95@gmail.com
 */


#ifndef GAME_SCENE_HEADER
#define GAME_SCENE_HEADER

    #include <memory>
    #include <basics/Canvas>
    #include <basics/Scene>
    #include <basics/Atlas>
    #include <basics/Timer>

    #include "Sprite.hpp"

    namespace example
    {

        using basics::Canvas;
        using basics::Atlas;
        using basics::Timer;

        class Game_Scene : public basics::Scene {

            typedef basics::Graphics_Context::Accessor Context;
            std::unique_ptr< Atlas > atlas;
            std::unique_ptr< Atlas > menuAtlas;

            enum State
            {
                LOADING,
                RUNNING,
                PAUSED,
                OVER,
                ERROR
            };

            enum buttonsID{
                RIGHT,
                LEFT,
                PAUSE,
            };

            enum spritesID{
                BACKGROUND,
                PLATFORM,
                CHARACTER,
                DOWN,
                TOP
            };

            enum menusID{
                BACKGROUND_PAUSE,
                TITLE
            };

            enum buttonsMenusID{
                PLAY,
                MENU
            };

        private:

            State          state;
            bool           suspended;

            unsigned       canvas_width;
            unsigned       canvas_height;

            Timer timer;

        public :

            struct Button{
                const Atlas::Slice * slice;
                Point2f position;
                bool isPressed;
            };

            static const unsigned nButtons = 3;
            static const unsigned nSprites = 5;
            static const unsigned nPlatforms = 10;
            static const unsigned nMenu = 2;
            static const unsigned nButtonsMenu = 2;

            int speedY = 0;
            bool iSRight = true;
           // bool isPaused = false;

            Button buttons[nButtons];
            Button menuButtons[nButtonsMenu];

            class Element{
            public:
                const Atlas::Slice * slice;
                Point2f position;

            public:
                bool intersects (Element & other){

                    float this_left    = this->position[0];
                    float this_bottom  = this->position[1];
                    float this_right   = this_left   + this->slice->width;
                    float this_top     = this_bottom + this->slice->height;

                    float other_left   = other.position[0];
                    float other_bottom = other.position[1];
                    float other_right  = other_left   + other.slice->width;
                    float other_top    = other_bottom + other.slice->height;


                    return !(other_left >= this_right || other_right <= this_left || other_bottom >= this_top || other_top <= this_bottom);

                }
            };

            Element sprites[nSprites];
            Element platforms[nPlatforms];
            Element pause[nMenu];

        public:

            Game_Scene();

            basics::Size2u get_view_size () override {
                return { canvas_width, canvas_height };
            }

            bool initialize () override;

            void suspend () override;

            int optionAt(const Point2f &point);

            void resume () override;

            void handle (basics::Event & event) override;

            void update (float time) override;

            void render (Context & context) override;

        private:

            void load_textures ();

            void create_sprites ();

            void run_simulation (float time);

            void update_user (float time);

            void render_loading (Canvas & canvas);

            void render_playfield (Canvas & canvas);

            void generate_platforms();

            bool check_collisions();

            void move_platforms(float time);

            void refresh_platforms(Element & platform);

            void render_pause(Canvas & canvas);

            void update_menu();

            void render_over(Canvas & canvas);

        };
    }

#endif
