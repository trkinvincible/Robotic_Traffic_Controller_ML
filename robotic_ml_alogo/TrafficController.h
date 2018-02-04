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
    NeuralNetworkTopology mNNTopology;
    GeneticAlgorithm mGenAlgo;

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
    inline unsigned int getCurrentGeneration() const { return mGenAlgo.getCurrentGeneration(); }
    inline unsigned int	getBestFitness() const { return mGenAlgo.getBestFitness(); }

    inline const std::vector<GeneticAlgorithm::t_genome>&	getGenomes() const { return mGenAlgo.getGenomes(); }
    inline const NeuralNetworkTopology& getNNTopology() const { return mNNTopology; }
};

#endif // TRAFFIC_CONTROLLER_H
