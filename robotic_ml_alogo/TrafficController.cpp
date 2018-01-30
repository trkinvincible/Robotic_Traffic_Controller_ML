#include "TrafficController.h"

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

    mTragetVechicals.resize(mGenAlgo.getGenomes().size()/*50*/);

    for (unsigned int i = 0; i < mTragetVechicals.size(); ++i)
    {
        Robots& V = mTragetVechicals[i];
        V.reset(mCircuit);
    }
}

void TrafficController::update(float step)
{
    bool	someone_is_alive = false;

#pragma omp parallel for //num_threads(3)
    {
        // This code will be executed by three threads.

        // Chunks of this loop will be divided amongst
        // the (three) threads of the current team.
        //        #pragma omp for
        for (unsigned int i = 0; i < mTragetVechicals.size(); ++i)
        {
            if (!mTragetVechicals[i].isAlive())
                continue;

            someone_is_alive = true;

            mTragetVechicals[i].update(step, mCircuit, mGenAlgo.getNNetworks()[i]);
        }
    }

    if (someone_is_alive)
        return;

    // rate genomes
    for (unsigned int i = 0; i < mTragetVechicals.size(); ++i)
        mGenAlgo.rateGenome(i, mTragetVechicals[i].getFitness());

    mGenAlgo.BreedPopulation();

    if (mGenAlgo.isAGreatGeneration())
    {
        Robots& car = mTragetVechicals[ mGenAlgo.getAlpha().m_index ];
    }

    for (Robots& car : mTragetVechicals)
        car.reset(mCircuit);
}
