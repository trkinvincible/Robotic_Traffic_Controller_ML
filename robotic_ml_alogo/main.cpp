#include <iostream>
#include <TrafficController.h>
#include "Renderer/RendererSFML.h"

using namespace std;

int main(int argc, char *argv[])
{
    TrafficController rk_controller("c:\\Personal\\ML\\Learning_car_source_code\\geneticAlgorithm_experiment-master\\carDemo\\src_cpp\\res\\Map_test.txt");

    RendererSFML	my_RendererSFML(rk_controller);

    my_RendererSFML.run([&]()
    {
        rk_controller.update(0.25f);
    });
}
