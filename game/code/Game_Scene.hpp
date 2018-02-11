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

namespace example {

    using basics::Canvas;
    using basics::Atlas;
    using basics::Point2f;
    using basics::Timer;

    class Game_Scene : public basics::Scene {

        typedef basics::Graphics_Context::Accessor Context; ///< Contexto de la escena
        std::unique_ptr< Atlas > atlas;                     ///< Atlas de sprites del juego
        std::unique_ptr< Atlas > menuAtlas;                 ///< Atlas de botones de los menus

        /**
        * Representa el estado de la escena en su conjunto.
        */
        enum State {
            LOADING,
            RUNNING,
            PAUSED,
            OVER,
            ERROR
        };

        /**
         * Representan los ID de los sprites del juego en sí mismo.
         */
        enum spritesID{
            BACKGROUND,
            PLATFORM,
            CHARACTER,
            DOWN,
            TOP
        };

        /**
         * Representan los ID de los sprites de los menus.
         */
        enum menusID{
            BACKGROUND_PAUSE,
            TITLE
        };

        /**
         * Representa los ID  de los Sprites de botones del canvas de juego
         */
        enum buttonsID{
            RIGHT,
            LEFT,
            PAUSE,
        };

        /**
         * Representa los ID  de los Sprites de botones de los menus (PAUSA y GAME OVER)
         */
        enum buttonsMenusID{
            PLAY,
            MENU
        };

    private:

        State          state;           ///< Estado de la escena
        bool           suspended;       ///< true cuando la escena está en segundo plano y viceversa.

        unsigned       canvas_width;    ///< Ancho de la resolución virtual usada para dibujar.
        unsigned       canvas_height;   ///< Alto  de la resolución virtual usada para dibujar.

        //Timer timer;                  ///< Time utilizado para controlar el tiempo del Loading

    public :

        /**
         * Estructura de un botón de la interfaz de juego o de los menus
         */
        struct Button{
            const Atlas::Slice * slice;
            Point2f position;
            bool isPressed;
        };

        /**
         * Clase elemento. Son todos aquellos sprites que aparecen en el juego y que no
         * son botones.
         */
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

        static const unsigned nSprites = 5;     ///< Numero de Sprites en el juego
        static const unsigned nPlatforms = 10;  ///< Cantidad de plataformas en el juego
        static const unsigned nMenu = 2;        ///< Numero de sprites en el menu Game-Over/Pausa

        static const unsigned nButtons = 3;     ///< Numero de botones en el juego
        static const unsigned nButtonsMenu = 2; ///< Numero de botones en el menú Game-Over/Pausa
        static const unsigned speedX = 500;     ///< Velocidad en el eje X del personaje
        const float gravity = -9.8f;             ///< Gravedad sobre el personaje

        int speedY = 0;                         ///< Velocidad vertical del personaje
        bool iSRight = true;                    ///< Dirección del personaje

        Button buttons[nButtons];               ///< Array de botones del canvas del juego
        Button menuButtons[nButtonsMenu];       ///< Array de botones en el menu Game-Over/Pausa

        Element sprites[nSprites];              ///< Array de sprites del juego
        Element platforms[nPlatforms];          ///< Array de plataformas
        Element menuSprites[nMenu];             ///< Array de sprites en el menu Game-Over/Pausa

    public:

        // -------------------------------------------------------------------------------------
        /**
         * Constructor de la escena
         */
        Game_Scene(){
            state = LOADING;
            suspended = true;
            canvas_width  = 720;
            canvas_height =  1280;
            speedY = 500;
            iSRight = true;
            srand (unsigned(time(nullptr)));
        };

        // -------------------------------------------------------------------------------------
        /**
         * Este método lo llama Director para conocer la resolución virtual con la que está
         * trabajando la escena.
         * @return Tamaño en coordenadas virtuales que está usando la escena.
         */
        basics::Size2u get_view_size () override {
            return { canvas_width, canvas_height };
        }

        // -------------------------------------------------------------------------------------
        /**
         * Este método lo invoca Director automáticamente cuando el juego pasa a segundo plano.
         */
        void suspend() override{
            suspended = true;
        }

        // -------------------------------------------------------------------------------------
        /**
         * Este método lo invoca Director automáticamente cuando el juego pasa a primer plano.
         */
        void resume() override{
            suspended = false;
        }

        // -------------------------------------------------------------------------------------
        /**
         * Aquí se inicializan los atributos que deben restablecerse cada vez que se inicia la escena.
         * @return
         */
        bool initialize () override;

        // -------------------------------------------------------------------------------------
        /**
         * Este método se encarga de controlar la entrada de datos de pulsación sobre la pantalla
         */
        void handle (basics::Event & event) override;

        // -------------------------------------------------------------------------------------
        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * actualize su estado.
         */
        void update (float time) override;

        // -------------------------------------------------------------------------------------
        /**
         * Este método se invoca automáticamente una vez por fotograma para que la escena
         * dibuje su contenido.
         */
        void render (Context & context) override;

    private:

        // -------------------------------------------------------------------------------------
        int optionAt(const Point2f &point);
        // -------------------------------------------------------------------------------------
        void load_textures ();
        // -------------------------------------------------------------------------------------
        void create_sprites ();
        // -------------------------------------------------------------------------------------
        void run_simulation (float time);
        // -------------------------------------------------------------------------------------
        void update_user (float time);
        // -------------------------------------------------------------------------------------
        void update_menu();
        // -------------------------------------------------------------------------------------
        void render_loading (Canvas & canvas);
        // -------------------------------------------------------------------------------------
        void render_playfield (Canvas & canvas);
        // -------------------------------------------------------------------------------------
        void render_pause(Canvas & canvas);
        // -------------------------------------------------------------------------------------
        void render_over(Canvas & canvas);
        // -------------------------------------------------------------------------------------
        void generate_platforms();
        // -------------------------------------------------------------------------------------
        bool check_collisions();
        // -------------------------------------------------------------------------------------
        void move_platforms(float time);
        // -------------------------------------------------------------------------------------
        void refresh_platforms(Element & platform);
    };
}

#endif