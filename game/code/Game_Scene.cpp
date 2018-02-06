/*
 * GAME SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Game_Scene.hpp"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example {

    Game_Scene::Game_Scene(){
        state = LOADING;
        suspended = true;
        canvas_width  = 720;
        canvas_height =  1280;

        srand (unsigned(time(nullptr)));
        initialize ();
    }

    bool Game_Scene::initialize () {
        for(auto & button : buttons){
            button.isPressed = false;
        }
        return true;
    }

    void Game_Scene::suspend () {
        suspended = true;
    }

    void Game_Scene::resume () {
        suspended = false;
    }

    void Game_Scene::handle (Event & event) {
        if (state == RUNNING){
            switch (event.id){
                case ID(touch-started):
                case ID(touch-moved): {
                    break;
                }
                case ID(touch_ended):{
                    break;
                }
            }
        }
    }

    int Game_Scene::optionAt(const Point2f &point) {

    }

    void Game_Scene::update (float time) {
        if (!suspended) switch (state) {
            case LOADING: load_textures(); break;
            case RUNNING: run_simulation (time); break;
            case ERROR:   break;
        }
    }

    void Game_Scene::render (Context & context) {
        if (!suspended) {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));
            if (!canvas) {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas) {
                canvas->clear ();
                switch (state) {
                    case LOADING: render_loading   (*canvas); break;
                    case RUNNING: render_playfield (*canvas); break;
                    case ERROR:   break;
                }
            }
        }


    }

    void Game_Scene::load_textures () {
        Graphics_Context::Accessor context = director.lock_graphics_context ();
        if (context) {
               atlas.reset(new Atlas("game-scene/game.sprites", context));
                state = atlas->good() ? RUNNING : ERROR;
            if(state == RUNNING){
                create_sprites();
            }

        }else if (timer.get_elapsed_seconds () > 1.f){
            create_sprites ();                          // la carga antes de pasar al juego para que
           load_textures();
        }

    }

    void Game_Scene::create_sprites () {
        buttons[LEFT].slice     =   atlas->get_slice(ID(left));
        buttons[RIGHT].slice    =   atlas->get_slice(ID(right));
        buttons[PAUSE].slice    =   atlas->get_slice(ID(pause));

        buttons[LEFT].position  =   Point2f(buttons[LEFT].slice->width/2, buttons[LEFT].slice->width/2);
        buttons[RIGHT].position =   Point2f(canvas_width-buttons[LEFT].slice->width*1.5, buttons[LEFT].slice->width/2);
        buttons[PAUSE].position =   Point2f(300, 300);

        initialize();
    }

    void Game_Scene::run_simulation (float time) {
        update_user ();
    }

    void Game_Scene::update_user () {
    }

    void Game_Scene::render_playfield (Canvas & canvas) {
        canvas.clear();
        if(state == RUNNING){
            for(auto & button : buttons){
                canvas.set_transform(scale_then_translate_2d
                                             (
                                                     button.isPressed ? 0.75f : 1.f,              // Escala de la opción
                                                     { button.position[0], button.position[1] }      // Traslación
                                             ));
                canvas.fill_rectangle ({ 0.f, 0.f }, { button.slice->width, button.slice->height }, button.slice, CENTER | TOP);
            }

            canvas.set_transform(Transformation2f());
        }
    }

    void Game_Scene::render_loading (Canvas & canvas) {
        Texture_2D * loading_texture = textures[ID(loading)].get ();
        if (loading_texture) {
            canvas.fill_rectangle
                    (
                            { canvas_width * .5f, canvas_height * .5f },
                            { loading_texture->get_width (), loading_texture->get_height () },
                            loading_texture
                    );
        }
    }

}
