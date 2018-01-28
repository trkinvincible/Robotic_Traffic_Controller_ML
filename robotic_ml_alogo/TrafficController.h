#ifndef TRAFFIC_CONTROLLER_H
#define TRAFFIC_CONTROLLER_H

#include <circuit.h>
#include <robots.h>
#include <ai/NeuralNetworkTopology.h>
#include <ai/GeneticAlgorithm.h>

class TrafficController
{
private:
    Circuit mCircuit;
    std::vector<Robots> mTragetVechicals;
    NeuralNetworkTopology mNNTopology;
    GeneticAlgorithm mGenAlgo;
public:
    TrafficController(const std::string &filename);
public: // methods
    void	update(float step);
public: // setter/getter
    inline const Circuit& getCircuit() const { return mCircuit; }
    inline const std::vector<Robots>& getTragetVechicals() const { return mTragetVechicals; }

    inline unsigned int getCurrentGeneration() const { return mGenAlgo.getCurrentGeneration(); }
    inline unsigned int	getBestFitness() const { return mGenAlgo.getBestFitness(); }

    inline const std::vector<GeneticAlgorithm::t_genome>&	getGenomes() const { return mGenAlgo.getGenomes(); }
    inline const NeuralNetworkTopology& getNNTopology() const { return mNNTopology; }
};

#endif // TRAFFIC_CONTROLLER_H
