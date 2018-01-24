/*
 * MENU SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Scene>
#include <basics/Texture_2D>
#include <basics/Director>


using basics::Texture_2D;

class Menu_Scene : public basics::Scene{

    std::shared_ptr< basics::Texture_2D > button_play;
    std::shared_ptr< basics::Texture_2D > button_help;
public:

    basics::Size2u get_view_size () override {
        return{1280, 720};
    }

    bool initialize(){
        return true;
    }

    void suspend(){
    }

    void finalize(){
    }

    void handle(basics::Event & event){
        bool playPressed = false;
        bool helpPressed = false;
        //Comprobar que se ha acertado
        if(playPressed){
           //Se cargaria la escena
            //basics::director.run_scene(shared_ptr< Scene >(new Scena));
        }
    }

    void update(float time){
    }

    Menu_Scene();

    void render (basics::Graphics_Context::Accessor & context) override;

private:

    void load();
    void run(float time);


};