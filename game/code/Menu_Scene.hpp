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

            /**
           * Representa el estado de la escena en su conjunto.
           */
            enum State{
                LOADING,
                RUNNING,
                HELPING,
                ERROR
            };

            /**
           * Representa los ID de los Sprites estaticos
           */
            enum spritesID{
                BACKGROUND,
                TITLE,
                TEXT_HELP
            };

            /**
           * Representa los ID  de los Sprites de botones
           */
            enum optionID{
                PLAY,
                HELP,
                EXIT,
                BACK
            };

            /**
             * Todos los Sprites que se mostraran pertenecen a esta estructura
             */
            struct Element{
                const Atlas::Slice * slice;
                Point2f position;
            };

            /**
             * Todos los botones que se mostraran pertenecen a esta estructura
             */
            struct Option{
                const Atlas::Slice * slice;
                Point2f position;
                float isPressed;
            };

            static const unsigned nOptions = 4; //Numero de botones
            static const unsigned nSprites = 3; //Numero de sprites simples

        private:

            State state;                            ///< Estado de la escena
            bool suspended;                         ///< true cuando la escena está en segundo plano y viceversa.

            unsigned canvas_width;                  ///< Ancho de la resolución virtual usada para dibujar.
            unsigned canvas_height;                 ///< Alto  de la resolución virtual usada para dibujar.

            Option options[nOptions];               ///< Array de botones que se mostrarán en el menú.
            Element sprites[nSprites];              ///< Array de Sprites simples

            //typedef basics::Graphics_Context::Accessor Context;
            std::unique_ptr< Atlas > atlas;         ///< Atlas de sprites


        public:

            // -------------------------------------------------------------------------------------
            /**
             * Constructor de la escena
             */
            Menu_Scene() {
                state = LOADING;
                suspended = true;
                canvas_width = 720;
                canvas_height = 1280;
            }

            // -------------------------------------------------------------------------------------
            /**
             * Este método lo llama Director para conocer la resolución virtual con la que está
             * trabajando la escena.
             * @return Tamaño en coordenadas virtuales que está usando la escena.
             */
            basics::Size2u get_view_size () override {
                return {canvas_width, canvas_height};
            };

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
            bool initialize() override;

            // -------------------------------------------------------------------------------------
            /**
             * Este método se encarga de controlar la entrada de datos de pulsación sobre la pantalla
             */
            void handle(basics::Event & event) override;

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
            void render (Graphics_Context::Accessor & context) override;

        private:

            // -------------------------------------------------------------------------------------
            int optionAt (const Point2f & point);
            // -------------------------------------------------------------------------------------
            void load_textures();
            // -------------------------------------------------------------------------------------
            void create_sprites();
            // -------------------------------------------------------------------------------------
            void update_menu();
            // -------------------------------------------------------------------------------------
            void update_helping();
            // -------------------------------------------------------------------------------------
            void render_menu(Canvas & canvas);
            // -------------------------------------------------------------------------------------
            void render_helping(Canvas & canvas);
        };
}
#endif
