#include "TrafficController.h"
#include <unistd.h>
#include <list>
#include <vector>

TrafficController::TrafficController(const std::string &filename)
{
    mCircuit.LoadPath(filename);

    std::vector<unsigned int> tmp_hidden;
    tmp_hidden.push_back(3);
    /*
        (N)
        (NE)
        (E)
        (SE)
        (S)
        (SW)
        (W)
        (NW)
    */
    mNNTopology.init(4/*sensors-if next node present*/, tmp_hidden, 2/*speed,angle*/);

    mGenAlgo.init(mNNTopology);

    std::vector<std::vector<t_vec2f>> start_stop_pos = mCircuit.getStartStopPositions();

    mTargetVehicals.resize(start_stop_pos.size());

    int count=0;
    for(vec_robots &vec_robs : mTargetVehicals){

        vec_robs.resize(mGenAlgo.getGenomes().size()/*30*/);

        std::vector<t_vec2f> pos = start_stop_pos.at(count);
        for(auto &robo : vec_robs)
            robo.reset(pos.at(0),pos.at(1));
        count++;
    }

    success_rate = 0;
}

void TrafficController::update(float step)
{
    int vehicle_count = 0;
    for(vec_robots &vec_robs : mTargetVehicals){

        someone_is_alive = false;

#pragma omp parallel for //num_threads(3)
        {
            // This code will be executed by three threads.

            // Chunks of this loop will be divided amongst
            // the (three) threads of the current team.
            //        #pragma omp for
            for (unsigned int i = 0; i < vec_robs.size(); ++i)
            {
                if (!vec_robs[i].isAlive())
                    continue;

                someone_is_alive = true;

                vec_robs[i].update(step, mCircuit, mGenAlgo.getNNetworks()[i]);
                success_rate+=vec_robs[i].getsuccess_rate();
            }
        }

        if (someone_is_alive)
            continue;

        //sleep(3);

        // rate genomes
        for (unsigned int i = 0; i < vec_robs.size(); ++i)
            mGenAlgo.rateGenome(i, vec_robs[i].getFitness());

        mGenAlgo.BreedPopulation();


        std::vector<std::vector<t_vec2f>> start_stop_pos = mCircuit.getStartStopPositions();

        std::vector<t_vec2f> pos = start_stop_pos.at(vehicle_count);
        for(auto &robo : vec_robs){

            robo.reset(pos.at(0),pos.at(1));
        }
        vehicle_count++;
    }
}
