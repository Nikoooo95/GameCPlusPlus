/*
 * GAME SCENE
 * Copyright © 2018+ Nicolás Tapia Sanz
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * nic.tap95@gmail.com
 */

#include "Game_Scene.hpp"
#include "Menu_Scene.hpp"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example {

    // ---------------------------------------------------------------------------------------------
    bool Game_Scene::initialize () {
        //state = LOADING;
        for(auto & button : buttons){
            button.isPressed = false;
        }
        return true;
    }

    // ---------------------------------------------------------------------------------------------
    void Game_Scene::handle (Event & event) {
        if (state == RUNNING || state == PAUSED || state == OVER){
            switch (event.id){
                case ID(touch-started):
                case ID(touch-moved): {
                    Point2f touchLocation = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int optionTouched = optionAt(touchLocation);
                    if(state == RUNNING){
                        for(int index=0; index < nButtons; ++index){
                            buttons[index].isPressed = index == optionTouched;
                        }
                    }
                    if(state == PAUSED || state == OVER){
                        for(int index=0; index < nButtonsMenu; ++index){
                            menuButtons[index].isPressed = index == optionTouched;
                        }
                    }
                    break;
                }
                case ID(touch-ended):{
                    for(auto & button : buttons) button.isPressed = false;
                    for(auto & button : menuButtons) button.isPressed = false;
                    break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    void Game_Scene::update (float time) {
        if (!suspended) switch (state) {
                case LOADING:       load_textures();        break;
                case RUNNING:
                case PAUSED:
                case OVER :         run_simulation(time);   break;
                case ERROR:                                 break;
            }
    }

    // ---------------------------------------------------------------------------------------------
    void Game_Scene::render (Context & context) {
        if (!suspended) {
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));
            if (!canvas) {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas) {
                canvas->clear ();
                switch (state) {
                    case LOADING:   render_loading (*canvas);   break;
                    case RUNNING:   render_playfield (*canvas); break;
                    case PAUSED:    render_pause (*canvas);     break;
                    case OVER:      render_over (*canvas);      break;
                    case ERROR:                                 break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de comprobar sobre que opción ha pulsado el jugador
     * @return el valor int sobre el que se ha pulsado
     */
    int Game_Scene::optionAt(const Point2f &point) {
        if(state == RUNNING){
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
        }
        if(state == PAUSED || state == OVER){
            for(int index = 0; index < nButtonsMenu; ++index){
                const Button & tempButton = menuButtons[index];
                if(
                        point[0] > tempButton.position[0] - tempButton.slice->width &&
                        point[0] < tempButton.position[0] + tempButton.slice->width  &&
                        point[1] > tempButton.position[1] - tempButton.slice->height &&
                        point[1] < tempButton.position[1] + tempButton.slice->height
                        ){
                    return index;
                }
            }
        }


        return -1;
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método carga las texturas del atlas del menú
     */
    void Game_Scene::load_textures () {
        if(state == LOADING) {
            Graphics_Context::Accessor context = director.lock_graphics_context();
            if (context) {
                atlas.reset(new Atlas("game-scene/game.sprites", context));
                menuAtlas.reset(new Atlas("game-scene/pause.sprites", context));
                state = atlas->good() ? (menuAtlas->good() ? RUNNING : ERROR) : ERROR;
                if (state == RUNNING) {
                    create_sprites();

                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de crear los sprites y botones y asignarles una posicion en el canvas
     */
    void Game_Scene::create_sprites () {

        sprites[BACKGROUND].slice       = atlas->get_slice(ID(background));
        sprites[TOP].slice              = atlas->get_slice(ID(top));
        sprites[DOWN].slice             = atlas->get_slice(ID(down));
        sprites[PLATFORM].slice         = atlas->get_slice(ID(platform));
        sprites[CHARACTER].slice        = atlas->get_slice(ID(character));

        buttons[LEFT].slice             = atlas->get_slice(ID(left));
        buttons[RIGHT].slice            = atlas->get_slice(ID(right));
        buttons[PAUSE].slice            = atlas->get_slice(ID(pause));


        sprites[TOP].position           = Point2f(sprites[TOP].slice->width/2, canvas_height-sprites[TOP].slice->height/2);
        sprites[DOWN].position          = Point2f(sprites[DOWN].slice->width/2, sprites[DOWN].slice->height/2);
        sprites[BACKGROUND].position    = Point2f(canvas_width/2, canvas_height/2);
        sprites[PLATFORM].position      = Point2f(0, 0);
        sprites[CHARACTER].position     = Point2f(500, 800);

        buttons[LEFT].position          = Point2f(buttons[LEFT].slice->width, buttons[LEFT].slice->height/1.5f);
        buttons[RIGHT].position         = Point2f(canvas_width-buttons[RIGHT].slice->width, buttons[RIGHT].slice->height/1.5f);
        buttons[PAUSE].position         = Point2f(canvas_width-buttons[RIGHT].slice->width, sprites[TOP].position[1]);

        menuSprites[BACKGROUND_PAUSE].slice     = menuAtlas->get_slice(ID(background));
        menuButtons[PLAY].slice                 = menuAtlas->get_slice(ID(resume));
        menuButtons[MENU].slice                 = menuAtlas->get_slice(ID(menu));
        menuSprites[TITLE].slice                = menuAtlas->get_slice(ID(title_pause));

        menuSprites[BACKGROUND_PAUSE].position  = Point2f(canvas_width/2, canvas_height/2);
        menuButtons [PLAY].position             = Point2f(canvas_width/2, canvas_height/2);
        menuButtons[MENU].position              = Point2f(canvas_width/2, menuButtons[PLAY].position[1]-menuButtons[MENU].slice->height*2);
        menuSprites[TITLE].position             = Point2f(canvas_width/2, menuButtons[PLAY].position[1] + menuSprites[TITLE].slice->height);

        initialize();
        generate_platforms();
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga del update en cada frame. Dependiendo del estado del juego, llama a un
     * método u otro. Tan solo será llamado si el juego se está ejecutando como tal. No puede darse
     * el caso de que entre en Loading, aunque igualmente lo dejo puesto para el control de errores.
     * @param time
     */
    void Game_Scene::run_simulation (float time) {
        switch(state){
            case LOADING:                       break;
            case RUNNING:   update_user(time);  break;
            case PAUSED:
            case OVER:      update_menu();      break;
            case ERROR:                         break;
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Update que controla cada frame el trascurso del juego. Se encarga de controlar el movimiento
     * del personaje vertical y horizontal, el movimiento de las plataformas, el game over y la pausa
     * @param time
     */
    void Game_Scene::update_user (float time) {
        sprites[CHARACTER].position[1] += speedY*time + 0.5f*gravity*time*time;
        speedY += gravity;
        if(speedY<0 &&
                sprites[CHARACTER].position[1]<canvas_height-sprites[TOP].slice->height-sprites[CHARACTER].slice->height){
            if(check_collisions()) {
                speedY = 500;
            }
        }
        if( sprites[CHARACTER].position[1] > canvas_height/2){
            move_platforms(time);
        }

        if(buttons[LEFT].isPressed){
            sprites[CHARACTER].position[0] -= speedX * time;
            iSRight = false;
        }
        if(buttons[RIGHT].isPressed){
            sprites[CHARACTER].position[0] += speedX * time;
            iSRight = true;
        }

        if(sprites[CHARACTER].position[0] < 0){
            sprites[CHARACTER].position[0] = canvas_width;
        }
        if(sprites[CHARACTER].position[0] > canvas_width){
            sprites[CHARACTER].position[0] = 0.f;
        }

        if(sprites[CHARACTER].position[1] < sprites[DOWN].position[1]+sprites[DOWN].slice->height/2){
            state = OVER;
        }

        if(buttons[PAUSE].isPressed){
            state = PAUSED;
        }

    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Update que se llama en los menus Game-Over / Pausa. El menú es el mismo, pero dependiendo del
     * estado realiza una función u otra.
     */
    void Game_Scene::update_menu() {
        if(menuButtons[MENU].isPressed){
            director.run_scene(std::shared_ptr< Scene >(new Menu_Scene));
        }

        if(menuButtons[PLAY].isPressed){
            if(state == PAUSED){
                state = RUNNING;
            }
            if(state == OVER){
                director.run_scene(std::shared_ptr< Scene >(new Game_Scene));
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método es el encargado de renderizar el Loading durante la carga de los sprites.
     * @param canvas
     */
    void Game_Scene::render_loading (Canvas & canvas) {
        if(state == LOADING){
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

    // ---------------------------------------------------------------------------------------------
    /**
     * Es el encargado de renderizar el campo de juego. Renderiza todos los elementos que se deben
     * mostrar mientras se está jugando.
     * @param canvas
     */
    void Game_Scene::render_playfield (Canvas & canvas) {
        canvas.clear();
        if(state == RUNNING){
            canvas.fill_rectangle ({ sprites[BACKGROUND].position[0], sprites[BACKGROUND].position[1] },
                                   { sprites[BACKGROUND].slice->width, sprites[BACKGROUND].slice->height },
                                   sprites[BACKGROUND].slice);
            for(auto & platform : platforms){
                if(platform.position[1]<sprites[TOP].position[1]){
                    canvas.fill_rectangle ({ platform.position[0], platform.position[1] },
                                           { platform.slice->width, platform.slice->height },
                                           platform.slice);
                }

            }
            for(auto & element : sprites){
                if(element.slice && element.slice!=sprites[PLATFORM].slice && element.slice!=sprites[BACKGROUND].slice){
                    if(element.slice != sprites[CHARACTER].slice || (element.slice == sprites[CHARACTER].slice && iSRight)){
                        canvas.fill_rectangle ({ element.position[0], element.position[1] },
                                               { element.slice->width, element.slice->height },
                                               element.slice);
                    }else if(element.slice == sprites[CHARACTER].slice && !iSRight){
                        canvas.fill_rectangle ({ element.position[0], element.position[1] },
                                               { element.slice->width, element.slice->height },
                                               element.slice, FLIP_HORIZONTAL);
                    }
                }
            }

            for(auto & button : buttons){
                canvas.fill_rectangle ({ button.position[0], button.position[1]},
                                       { button.slice->width, button.slice->height },
                                       button.slice);
            }
            //canvas.set_transform(Transformation2f());
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método es el encargado de renderizar el menu de pausa.
     * @param canvas
     */
    void Game_Scene::render_pause(Canvas & canvas){
        canvas.clear();
            for(auto & element : menuSprites) {
                canvas.fill_rectangle({element.position[0], element.position[1]},
                                      {element.slice->width, element.slice->height},
                                      element.slice);
            }
            for(auto & element : menuButtons) {
                canvas.fill_rectangle({element.position[0], element.position[1]},
                                      {element.slice->width, element.slice->height},
                                      element.slice);
            }

    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método es el encargado de renderizar la pantalla de Game Over.
     * @param canvas
     */
    void Game_Scene::render_over(Canvas & canvas){
        menuButtons[PLAY].slice     = menuAtlas->get_slice(ID(play_again));
        menuSprites[TITLE].slice    = menuAtlas->get_slice(ID(title_over));
        render_pause(canvas);
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de rellenar el array de plataformas al iniciar la partida
     */
    void Game_Scene::generate_platforms() {
        const Atlas::Slice * tempSlice = sprites[PLATFORM].slice;
        int tempN =(int) canvas_height+canvas_height/4;
        for(size_t index = 0; index<nPlatforms; ++index){
            platforms[index].slice = tempSlice;
            platforms[index].position[0] = 3 + rand()%(canvas_width - 3);
            platforms[index].position[1] = sprites[DOWN].slice->height + rand()%(tempN - (int)sprites[DOWN].slice->height);
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de comprobar si el personaje ha colisionado con alguna plataforma
     * @return true si ha colisionado con una, false si no es así.
     */
    bool Game_Scene::check_collisions() {
        for(auto & platform : platforms){
            if(sprites[CHARACTER].intersects(platform)) return true;
        }
        return false;
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Move_Platforms se encarga de mover las plataformas hacia la parte inferior de la pantalla
     * @param time
     */
    void Game_Scene::move_platforms(float time) {
        float temp = sprites[CHARACTER].position[1] - canvas_height/2;
        for(auto & platform : platforms){
                platform.position[1] -= temp*time;
            if(platform.position[1]<sprites[DOWN].position[1]+sprites[DOWN].slice->height/2){
                refresh_platforms(platform);
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Se encarga de actualizar las plataformas que han salido por la parte inferior en una nueva
     * parte de la zona superior para re-utilizarlas.
     * @param platform
     */
    void Game_Scene::refresh_platforms(Element & platform) {
        platform.position[0] = 3 + rand()%(canvas_width - 3);
        platform.position[1] = sprites[TOP].position[1] + rand()%(canvas_height+1-(int)sprites[TOP].position[1]);
    }

}
