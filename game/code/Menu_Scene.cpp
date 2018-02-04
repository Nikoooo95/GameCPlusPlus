/*
 * INTRO SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"

#include <basics/Director>
#include <basics/Canvas>

using namespace basics;


namespace example
{

    Menu_Scene::Menu_Scene() {
        state = LOADING;
        suspended = true;
        canvasWidth = 720;
        canvasHeight = 1280;
    }

    bool Menu_Scene::initialize() {
        for(auto & option : options){
            option.isPressed = false;
        }
        return true;
    }

    void Menu_Scene::handle (basics::Event & event){
        if(state == READY){
            switch (event.id){
                case ID(touch-started):
                case ID(touch-moved):{
                    Point2f touchLocation = {*event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int optionTouched = optionAt(touchLocation);
                    for(size_t index = 0; index < nOptions; ++index){
                        options[index].isPressed = index == optionTouched;
                    }

                    break;
                }

                case ID(touch_ended):{
                    for(auto & option : options) option.isPressed = false;
                    Point2f touchLocation = {*event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    if(optionAt(touchLocation) == PLAY){
                        director.run_scene(std::shared_ptr< Scene >(new Game_Scene));
                    }
                }

            }
        }
    }

    int Menu_Scene::optionAt(const Point2f & point){
        for(size_t index = 0; index < nOptions; ++index){
            const Option & option = options[index];
            if(
                    point[0] > option.position[0] - option.slice->width  &&
                    point[0] < option.position[0] + option.slice->width  &&
                    point[1] > option.position[1] - option.slice->height &&
                    point[1] < option.position[1] + option.slice->height
            ){
                return index;
            }
        }

        return -1;
    }

    void Menu_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                canvas = Canvas::create (ID(canvas), context, {{ canvasWidth, canvasHeight }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();

                if (state == READY)
                {
                    // Se dibuja el slice de cada una de las opciones del menú:

                    for (auto & option : options)
                    {
                        canvas->set_transform
                                (
                                        scale_then_translate_2d
                                                (
                                                        option.isPressed ? 0.75f : 1.f,              // Escala de la opción
                                                        { option.position[0], option.position[1] }      // Traslación
                                                )
                                );

                        canvas->fill_rectangle ({ 0.f, 0.f }, { option.slice->width, option.slice->height }, option.slice, CENTER | TOP);
                    }

                    // Se restablece la transformación aplicada a las opciones para que no afecte a
                    // dibujos posteriores realizados con el mismo canvas:

                    canvas->set_transform (Transformation2f());
                }
            }
        }
    }


}
