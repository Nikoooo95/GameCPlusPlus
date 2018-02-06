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
        load_textures();
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
                    Point2f touchLocation = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int optionTouched = optionAt(touchLocation);
                    for(int index=0; index < nButtons; ++index){
                        buttons[index].isPressed = index == optionTouched;
                    }
                    break;
                }
                case ID(touch-ended):{
                    for(auto & button : buttons) button.isPressed = false;
                    break;
                }
            }
        }
    }

    int Game_Scene::optionAt(const Point2f &point) {
        for(int index = 0; index < nButtons; ++index){
            const Button & tempButton = buttons[index];

            if(
                    point[0] > tempButton.position[0] - tempButton.slice->width &&
                    point[0] < tempButton.position[0] + tempButton.slice->width  &&
                    point[1] > tempButton.position[1] - tempButton.slice->height &&
                    point[1] < tempButton.position[1] + tempButton.slice->height
                    ){
                return index;
            }
        }

        return -1;
    }

    void Game_Scene::update (float time) {
        if (!suspended) switch (state) {
            case LOADING: break;
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
            create_sprites ();
            state=RUNNING;
        }

    }

    void Game_Scene::create_sprites () {

        sprites[BACKGROUND].slice   = atlas->get_slice(ID(background));
        sprites[TOP].slice          = atlas->get_slice(ID(top));
        sprites[DOWN].slice         = atlas->get_slice(ID(down));
        sprites[PLATFORM].slice     = atlas->get_slice(ID(platform));
        sprites[CHARACTER].slice    = atlas->get_slice(ID(character));

        buttons[LEFT].slice         = atlas->get_slice(ID(left));
        buttons[RIGHT].slice        = atlas->get_slice(ID(right));
        buttons[PAUSE].slice        = atlas->get_slice(ID(pause));


        sprites[TOP].position           = Point2f(sprites[TOP].slice->width, canvas_height*2-sprites[TOP].slice->height);
        sprites[DOWN].position          = Point2f(sprites[DOWN].slice->width, sprites[DOWN].slice->height);
        sprites[BACKGROUND].position    = Point2f(canvas_width, canvas_height);
        sprites[PLATFORM].position      = Point2f(300, 300);
        sprites[CHARACTER].position     = Point2f(500, 500);

        buttons[LEFT].position      = Point2f(buttons[LEFT].slice->width, sprites[DOWN].slice->height/2);
        buttons[RIGHT].position     = Point2f(canvas_width-buttons[RIGHT].slice->width, sprites[DOWN].slice->height/2);
        buttons[PAUSE].position     = Point2f(canvas_width*2-buttons[RIGHT].slice->width*1.5f, canvas_height*2-buttons[PAUSE].slice->height*1.15f);

        initialize();
    }

    void Game_Scene::run_simulation (float time) {
        update_user ();
    }

    void Game_Scene::update_user () {
       /* if(character.intersects(sprites[DOWN])){
            //speed
        }*/
        if(buttons[LEFT].isPressed){
            sprites[CHARACTER].position[0] -= 20.f;

        }
        if(buttons[RIGHT].isPressed)
            sprites[CHARACTER].position[0] += 20.f;
    }

    void Game_Scene::render_playfield (Canvas & canvas) {
        canvas.clear();
        if(state == RUNNING){

            for(auto & element : sprites){
                if(element.slice){
                    canvas.fill_rectangle ({ element.position[0]*.5f, element.position[1]*.5f },
                                           { element.slice->width, element.slice->height },
                                           element.slice);
                }


            }

            for(auto & button : buttons){
                canvas.fill_rectangle ({ button.position[0], button.position[1]},
                                       { button.slice->width, button.slice->height },
                                       button.slice);
            }

            canvas.set_transform(Transformation2f());
        }
    }

    void Game_Scene::render_loading (Canvas & canvas) {
        const Atlas::Slice * slice = atlas->get_slice(ID(loading));
        if (slice) {
            canvas.fill_rectangle
                    (
                            { canvas_width * .5f, canvas_height * .5f },
                            { slice->width, slice->height},
                            slice
                    );
        }
    }

}
