/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include "Sample_Scene.hpp"
#include <basics/Log>
#include <cstdlib>

using namespace basics;

namespace example
{

    Sample_Scene::Sample_Scene(Size2u size) : size(size)
    {

    }

    bool Sample_Scene::initialize ()
    {

        ball.x = 590;
        ball.y = 310;
        ball.size = 100;
        ball.speedX = 0;
        ball.speedY = 0;

        bar1.height = 200;
        bar1.width = 50;
        bar1.x = 20;
        bar1.y = 310;

        bar2.height = 200;
        bar2.width = 50;
        bar2.x = 1210;
        bar2.y = 260;

        fingerPosition = 0;
        speedBar = 0.05;
        return true;
    }

    void Sample_Scene::handle (Event & event)
    {
        switch (event.id)
        {
            case FNV(touch-started):
            {
                //VELOCIDAD ALEATORIA DE SALIDA
                if(ball.speedY==0 && ball.speedX==0){
                    ball.speedX = rand()%5+10;
                    ball.speedY = rand()%5+10;
                }

                //SEGUIMIENTO DEL DEDO DEL USUARIO
                fingerPosition = (*event.properties[FNV(y)].as< var::Float > ()) - bar2.height/2;
            }
            case FNV(touch-moved):
            {
                break;
            }
        }
    }

    void Sample_Scene::update (float time)
    {

        //MOVIMIENTO DE LA IA
        bar1.y = ball.y+(ball.size/2)-(bar1.height/2);

        //ACTUALIZACION DE MOVIMIENTO DE LA PELOTA
        if(ball.speedX!=0 || ball.speedY!=0){
            ball.x = ball.x + ball.speedX*0.75;
            ball.y = ball.y + ball.speedY*0.75;
        }


        //ACTUALIZACION DE MOVIMIENTO DE BARRA DE USUARIO
        if(fingerPosition!=0 && bar2.y != fingerPosition){
            if(fingerPosition<=360){
                if(bar2.y <= 0){
                    bar2.y = 0;
                }else{
                    bar2.y = bar2.y - (bar2.y - fingerPosition)*speedBar;
                }
            }else{
                if(bar2.y+bar2.height>=720){
                    bar2.y = 720-bar2.height;
                }else{
                    bar2.y = bar2.y + (fingerPosition - bar2.y)*speedBar;
                }

            }

        }

        //SI SE SALE POR LA DERECHA O POR LA IZQUIERDA
        if(ball.x > 1280 - ball.size ||
              ball.x <= 0){
            ball.x = 590;
            ball.y = 310;
            ball.speedY = 0;
            ball.speedX = 0;
            bar2.y = 260;
            fingerPosition=0;
        }

        //REBOTE SUPERIOR O INFERIOR
        if(ball.y >= 720 - ball.size ||
                ball.y <= 0 ){
            ball.speedY = -ball.speedY;
        }

        //DETECCION DE COLISIONES
        Sample_Scene::collisions(ball, bar2);
        //"COLISION" CON LA BARRA DE LA IA
        if(ball.x <= bar1.x+bar1.width){
            ball.speedX = -ball.speedX;
        }

    }

    //RENDERIZADO DE OBJETOS DE LA ESCENA
    void Sample_Scene::render (basics::Renderer & renderer) {
        renderer.draw_rectangle(ball.x, ball.y, ball.size, ball.size);
        renderer.draw_rectangle(bar1.x, bar1.y, bar1.width, bar1.height);
        renderer.draw_rectangle(bar2.x, bar2.y, bar2.width, bar2.height);

    }

    //COLISION CON LA BARRA DEL JUGADOR
    void Sample_Scene::collisions(Ball & ball_, const Bar & bar_) {
        if(ball_.x+ball_.size >= bar_.x){
            if((ball_.y >= bar_.y && ball_.y <= bar_.y+bar_.height) ||
                    (ball_.y <= bar_.y && ball_.y+ball_.size >= bar_.y)){
                ball_.speedX = -ball_.speedX;
            }

        }

    }


}
