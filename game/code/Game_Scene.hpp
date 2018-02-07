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
            static const unsigned nPlatforms = 10;

            int speedY = 0;


            Button buttons[nButtons];

            class Element{
            public:
                const Atlas::Slice * slice;
                Point2f position;

            public:
                bool intersects (Element & other){

                    // Se determinan las coordenadas de la esquina inferior izquierda y de la superior derecha
                    // de este sprite:

                    float this_left    = this->position[0];
                    float this_bottom  = this->position[1];
                    float this_right   = this_left   + this->slice->width;
                    float this_top     = this_bottom + this->slice->height;

                    // Se determinan las coordenadas de la esquina inferior izquierda y de la superior derecha
                    // del otro sprite:

                    float other_left   = other.position[0];
                    float other_bottom = other.position[1];
                    float other_right  = other_left   + other.slice->width;
                    float other_top    = other_bottom + other.slice->height;

                    // Se determina si los rectángulos envolventes de ambos sprites se solapan:

                    return !(other_left >= this_right || other_right <= this_left || other_bottom >= this_top || other_top <= this_bottom);

                    /*if(this->position[1] + other.slice->height < other.position[1] || this->position[1] > other.position[1]+other.slice->height) return false;
                    if(this->position[0] + other.slice->height < other.position[0] || this->position[0] > other.position[0]+other.slice->height) return false;
                    return false;*/

                    return  !(other.position[0] >= this->position[0]+this->slice->width ||
                            other.position[0]+other.slice->width <= this->position[0] ||
                            other.position[1] >= this->position[1]+this->slice->height ||
                            other.position[1] + other.slice->height <= this->position[1]);
                }
            };

            Element sprites[nSprites];
            Element platforms[nPlatforms];


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

        };
    }

#endif
