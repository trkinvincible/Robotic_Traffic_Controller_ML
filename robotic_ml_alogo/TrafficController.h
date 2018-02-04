#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include <circuit.h>
#include <robots.h>
#include <ai/NeuralNetworkTopology.h>
#include <ai/GeneticAlgorithm.h>

class TrafficController
{
private:
    typedef std::vector<Robots> vec_robots;
    typedef std::vector<std::vector<Robots>> vec_robots_l;

    Circuit mCircuit;
    vec_robots_l mTargetVehicals;
    std::vector<NeuralNetworkTopology> mNNTopology;
    std::vector<GeneticAlgorithm> mGenAlgo;

public:
    TrafficController(const std::string &filename);
public: // methods
    int success_rate;
    inline int getSuccessRate() const { return success_rate; }
    bool someone_is_alive;
    void	update(float step);
public: // setter/getter
    inline const Circuit& getCircuit() const { return mCircuit; }

    inline const vec_robots_l& getTragetVechicals(){return mTargetVehicals;}
    inline unsigned int getCurrentGeneration(int count) const { return mGenAlgo.at(count).getCurrentGeneration(); }
    inline unsigned int	getBestFitness(int count) const { return mGenAlgo.at(count).getBestFitness(); }

    inline const std::vector<GeneticAlgorithm::t_genome>&	getGenomes(int count) const { return mGenAlgo.at(count).getGenomes(); }
};

#endif // TRAFFIC_CONTROLLER_H
