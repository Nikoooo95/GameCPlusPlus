/*
 * MENU SCENE
 * Copyright © 2018+ Nicolás Tapia Sanz
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * nic.tap95@gmail.com
 */


#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"

#include <basics/Director>
#include <basics/Canvas>
#include <basics/Transformation>

using namespace basics;
using namespace std;

namespace example {

    // ---------------------------------------------------------------------------------------------
    bool Menu_Scene::initialize () {
        state = LOADING;
        for (auto & option : options) {
            option.isPressed = false;
        }
        return true;
    }

    // ---------------------------------------------------------------------------------------------
    void Menu_Scene::handle (basics::Event & event){
        switch (event.id){
            case ID(touch-started):{
                Point2f touchLocation = {*event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                int optionTouched = optionAt(touchLocation);
                if(state == RUNNING){
                    for(int index = 0; index < nOptions; ++index){
                        if(options[index].slice != options[BACK].slice){
                            options[index].isPressed = index == optionTouched;
                        }
                    }
                }
                else
                if(state == HELPING){
                    options[BACK].isPressed = true;
                }
                break;
            }

            case ID(touch_ended):{
                for(auto & option : options) option.isPressed = false;
                break;
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    void Menu_Scene::update (float time) {
        if (!suspended) switch (state) {
                case LOADING: load_textures(); break;
                case RUNNING: update_menu(); break;
                case HELPING: update_helping(); break;
                case ERROR: break;
            }
    }

    // ---------------------------------------------------------------------------------------------
    void Menu_Scene::render (Graphics_Context::Accessor & context) {
        if(!suspended){
            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));
            if (!canvas) {
                canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            if (canvas) {
                canvas->clear ();
                switch (state) {
                    case LOADING: break;
                    case RUNNING: render_menu (*canvas); break;
                    case HELPING: render_helping (*canvas); break;
                    case ERROR:   break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de comprobar sobre que opción ha pulsado el jugador
     * @return el valor int sobre el que se ha pulsado
     */
    int Menu_Scene::optionAt(const Point2f & point){
        for(int index = 0; index < nOptions; ++index){
            const Option & option = options[index];
            if(
                    point[0] > (option.position[0]-option.slice->width/2) - option.slice->width  &&
                    point[0] < option.position[0] + (option.slice->width/2)  &&
                    point[1] > (option.position[1]-option.slice->height/2) - option.slice->height &&
                    point[1] < option.position[1] + (option.slice->height/2)
                    ){
                return index;
            }
        }
        return -1;
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método carga las texturas del atlas del menú
     */
    void Menu_Scene::load_textures() {
        if(state == LOADING){
            Graphics_Context::Accessor context = director.lock_graphics_context();
            if (context) {
                atlas.reset(new Atlas("menu-scene/menu.sprites", context));
                state = atlas->good() ? RUNNING : ERROR;
                if(state == RUNNING){
                    create_sprites();
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de crear los sprites y botones y asignarles una posicion en el canvas
     */
    void Menu_Scene::create_sprites() {
        sprites[TITLE].slice            = atlas->get_slice(ID(title));
        sprites[BACKGROUND].slice       = atlas->get_slice(ID(background));
        sprites[TEXT_HELP].slice        = atlas->get_slice(ID(text_help));

        options[PLAY].slice             = atlas->get_slice(ID(play));
        options[HELP].slice             = atlas->get_slice(ID(help));
        options[EXIT].slice             = atlas->get_slice(ID(exit));
        options[BACK].slice             = atlas->get_slice(ID(back));

        sprites[TITLE].position         = Point2f(canvas_width/2, canvas_height-sprites[TITLE].slice->height*2.f);
        sprites[BACKGROUND].position    = Point2f(canvas_width/2, canvas_height/2);
        sprites[TEXT_HELP].position     = Point2f(canvas_width/2, canvas_height/2);

        options[PLAY].position          = Point2f(canvas_width/2, canvas_height/2);
        options[HELP].position          = Point2f(canvas_width/2, options[PLAY].position[1]-options[HELP].slice->height*2);
        options[EXIT].position          = Point2f(canvas_width/2, options[HELP].position[1]-options[EXIT].slice->height*2);
        options[BACK].position          = Point2f(canvas_width/2, options[BACK].slice->height*3);

    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga del update en cada frame durante el estado de Running y de la
     * pulsación de botones durante este tiempo
     */
    void Menu_Scene::update_menu() {
        if(state == RUNNING){
            if(options[PLAY].isPressed){
                director.run_scene(std::shared_ptr< Scene >(new Game_Scene));
                reset_options ();
            }
            if(options[HELP].isPressed){
                state = HELPING;
                reset_options ();
            }
            if(options[EXIT].isPressed){
                director.run_scene (shared_ptr< Scene >());
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga del update en cada frame durante el estado de Helping
     */
    void Menu_Scene::update_helping() {
        if (options[BACK].isPressed) {
            state = RUNNING;
            reset_options ();
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de renderizar la escena durante el estado de Running
     */
    void Menu_Scene::render_menu(Canvas &canvas) {
        canvas.clear();
        for(auto & element :  sprites){
            if(element.slice != sprites[TEXT_HELP].slice){
                canvas.fill_rectangle ({ element.position[0], element.position[1] },
                                       { element.slice->width, element.slice->height },
                                       element.slice);
            }
        }
        for(auto & button : options){
            if(button.slice != options[BACK].slice){
                canvas.fill_rectangle ({ button.position[0], button.position[1] },
                                       { button.slice->width, button.slice->height },
                                       button.slice);
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    /**
     * Este método se encarga de renderizar la escena durante el estado de Helping
     */
    void Menu_Scene::render_helping(Canvas &canvas) {
        canvas.clear();
        canvas.fill_rectangle ({ sprites[TEXT_HELP].position[0], sprites[TEXT_HELP].position[1] },
                               { sprites[TEXT_HELP].slice->width, sprites[TEXT_HELP].slice->height },
                               sprites[TEXT_HELP].slice);

        canvas.fill_rectangle ({ options[BACK].position[0], options[BACK].position[1] },
                               { options[BACK].slice->width, options[BACK].slice->height },
                               options[BACK].slice);

    }
    // ---------------------------------------------------------------------------------------------
    /**
     * Este método resetea todos los botones a false.
     */
    void Menu_Scene::reset_options()
    {
        for (auto & option : options)
        {
            option.isPressed = false;
        }
    }
}
