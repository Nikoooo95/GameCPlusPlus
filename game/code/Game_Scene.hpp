/*
 * GAME SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#ifndef GAME_SCENE_HEADER
#define GAME_SCENE_HEADER

    #include <map>
    #include <list>
    #include <memory>

    #include <basics/Canvas>
    #include <basics/Id>
    #include <basics/Scene>
    #include <basics/Texture_2D>
    #include <basics/Timer>
#include <basics/Atlas>

    #include "Sprite.hpp"

    namespace example
    {

        using basics::Id;
        using basics::Timer;
        using basics::Canvas;
        using basics::Texture_2D;
        using basics::Atlas;

        class Game_Scene : public basics::Scene {

            typedef std::shared_ptr< Texture_2D  >     Texture_Handle;
            typedef std::map< Id, Texture_Handle >     Texture_Map;
            typedef basics::Graphics_Context::Accessor Context;
            std::unique_ptr< Atlas > atlas;

            enum State
            {
                LOADING,
                RUNNING,
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

        private:

            State          state;                               ///< Estado de la escena.
            bool           suspended;                           ///< true cuando la escena está en segundo plano y viceversa.

            unsigned       canvas_width;                        ///< Ancho de la resolución virtual usada para dibujar.
            unsigned       canvas_height;     ///< Alto  de la resolución virtual usada para dibujar.



        public :

            Texture_Map    textures;                            ///< Mapa  en el que se guardan shared_ptr a las texturas cargadas

            Timer          timer;                               ///< Cronómetro usado para medir intervalos de tiempo

            struct Button{
                const Atlas::Slice * slice;
                Point2f position;
                bool isPressed;
            };

            static const unsigned nButtons = 3;
            static const unsigned nSprites = 5;



            Button buttons[nButtons];

            class Element{
            public:
                const Atlas::Slice * slice;
                Point2f position;

            public:
                bool intersects (Element & other){
                    if(this->position[1] + other.slice->height < other.position[1] || this->position[1] > other.position[1]+other.slice->height) return false;
                    if(this->position[0] + other.slice->height < other.position[0] || this->position[0] > other.position[0]+other.slice->height) return false;
                    return false;
                }
            };

            Element sprites[nSprites];


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

            void update_user ();

            void render_loading (Canvas & canvas);

            void render_playfield (Canvas & canvas);


        };
    }

#endif
