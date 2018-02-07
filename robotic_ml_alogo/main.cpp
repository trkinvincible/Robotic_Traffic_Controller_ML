#include <iostream>
#include <TrafficController.h>
#include "Renderer/RendererSFML.h"

using namespace std;

int main(int argc, char *argv[])
{
    TrafficController rk_controller("../Graph_Square.txt");

    RendererSFML	my_RendererSFML(rk_controller);

    my_RendererSFML.run([&]()
    {
        rk_controller.update(0.25f);
    });
}
