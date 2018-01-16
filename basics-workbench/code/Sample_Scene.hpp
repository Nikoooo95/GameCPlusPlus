/*
 * SAMPLE SCENE
 * Copyright © 2018+ Ángel Rodríguez Ballesteros
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * angel.rodriguez@esne.edu
 */

#include <basics/Scene>

namespace example
{

    class Sample_Scene : public basics::Scene
    {
        struct Ball{
            float size;
            float x, y;
            float speedX, speedY;
        };

        struct Bar{
            float width, height;
            float x, y;
        };

        Bar bar1;
        Bar bar2;
        Ball ball;

        float fingerPosition;
        float speedBar;

        basics::Size2u size;

    public:

        Sample_Scene(basics::Size2u size);

        basics::Size2u get_view_size () override
        {
            return { 1280, 720 };
        }

        bool initialize () override;
        void handle     (basics::Event & event) override;
        void update     (float time) override;
        void render     (basics::Renderer & renderer) override;
        void collisions(Ball&, const Bar&);

    };

}
