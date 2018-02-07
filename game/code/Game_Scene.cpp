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
        speedY = 500.f;

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
        sprites[CHARACTER].position     = Point2f(500, 800);

        buttons[LEFT].position      = Point2f(buttons[LEFT].slice->width, buttons[LEFT].slice->height/1.5f);
        buttons[RIGHT].position     = Point2f(canvas_width-buttons[RIGHT].slice->width, buttons[RIGHT].slice->height/1.5f);
        buttons[PAUSE].position     = Point2f(canvas_width-buttons[RIGHT].slice->width, sprites[TOP].position[1]/2-sprites[TOP].slice->height*0.2);

        initialize();
        generate_platforms();
    }

    void Game_Scene::run_simulation (float time) {
        update_user (time);
    }

    void Game_Scene::update_user (float time) {
        float gravity = -9.8f;


        sprites[CHARACTER].position[1] += speedY*time + 0.5f*gravity*time*time;
        speedY += gravity;
        if(speedY<0){
            if(check_collisions()){
            speedY = 500.f;
            }
        }

        if(buttons[LEFT].isPressed){
            sprites[CHARACTER].position[0] -= 500.f * time;

        }
        if(buttons[RIGHT].isPressed)
            sprites[CHARACTER].position[0] += 500.f * time;
    }

    void Game_Scene::render_playfield (Canvas & canvas) {
        canvas.clear();
        if(state == RUNNING){

            for(auto & element : sprites){
                if(element.slice && element.slice!=sprites[PLATFORM].slice && element.slice!=sprites[CHARACTER].slice){
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

            for(auto & platform : platforms){
                if(platform.position[1]<sprites[TOP].position[1]*.5f){
                    canvas.fill_rectangle ({ platform.position[0], platform.position[1] },
                                           { platform.slice->width, platform.slice->height },
                                           platform.slice);
                }

            }
            canvas.fill_rectangle ({ sprites[CHARACTER].position[0], sprites[CHARACTER].position[1] },
                                   { sprites[CHARACTER].slice->width, sprites[CHARACTER].slice->height },
                                   sprites[CHARACTER].slice);
            //canvas.set_transform(Transformation2f());
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

    void Game_Scene::generate_platforms() {
        const Atlas::Slice * tempSlice = sprites[PLATFORM].slice;
        int tempN =(int) canvas_height-sprites[DOWN].slice->height;
        for(size_t index = 0; index<nPlatforms; ++index){
            platforms[index].slice = tempSlice;
            platforms[index].position[0] = 3 + rand()%(canvas_width - 3);
            platforms[index].position[1] = sprites[DOWN].slice->height + rand()%(tempN);
        }
    }

    bool Game_Scene::check_collisions() {
        for(auto & platform : platforms){
            if(sprites[CHARACTER].intersects(platform)) return true;
        }
        return false;
    }

}
