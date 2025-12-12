#pragma once

#include <vector>
#include "NeuralNetwork.hpp"

class Population {
public:
    Population(size_t popSize, const std::vector<size_t>& topology);

    void update();
    void evolve();

    void reset(size_t popSize, const std::vector<size_t>& newTopology);

    NeuralNetwork& getBrain(size_t index);
    void setFitness(size_t index, double score);
    
    double getBestFitness() const { return bestFitness; }
    double getAverageFitness() const;
    size_t getGeneration() const { return generation; }
    size_t size() const { return population.size(); }

private:
    std::vector<NeuralNetwork> population;
    std::vector<double> fitness;
    
    // Topology is no longer const because we might change it on reset
    std::vector<size_t> topology; 
    
    size_t generation;
    double bestFitness;

    NeuralNetwork& selectParent();
    NeuralNetwork crossover(NeuralNetwork& parentA, NeuralNetwork& parentB);
    void mutate(NeuralNetwork& brain);
};