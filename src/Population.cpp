#include "Population.hpp"
#include <random>
#include <algorithm>
#include <iostream>

static std::mt19937 ga_randomEngine(std::random_device{}());

Population::Population(size_t popSize, const std::vector<size_t>& topology) 
    : topology(topology), generation(0), bestFitness(0.0) {
    
    reset(popSize, topology);
}

void Population::reset(size_t popSize, const std::vector<size_t>& newTopology) {
    this->topology = newTopology;
    this->generation = 0;
    this->bestFitness = 0.0;

    population.clear();
    fitness.clear();

    population.reserve(popSize);
    fitness.resize(popSize, 0.0);

    for (size_t i = 0; i < popSize; ++i) {
        population.emplace_back(newTopology, ActivationType::RELU);
    }
    
    std::cout << "Population Reset! Topology: { ";
    for(auto n : newTopology) std::cout << n << " ";
    std::cout << "}" << std::endl;
}

double Population::getAverageFitness() const {
    double totalFitness = 0.0;
    for (double fit : fitness) {
        totalFitness += fit;
    }
    return totalFitness / population.size();
}

NeuralNetwork& Population::getBrain(size_t index) {
    return population[index];
}

void Population::setFitness(size_t index, double score) {
    fitness[index] = score;
}

void Population::evolve() {
    auto bestIt = std::max_element(fitness.begin(), fitness.end());
    bestFitness = *bestIt;
    size_t bestBrainIndex = std::distance(fitness.begin(), bestIt);

    std::vector<NeuralNetwork> newGeneration;
    newGeneration.reserve(population.size());

    std::vector<double> championGenes = population[bestBrainIndex].getGenes();
    NeuralNetwork champion(topology, ActivationType::RELU);
    champion.setGenes(championGenes);
    newGeneration.push_back(champion);

    for (size_t i = 1; i < population.size(); ++i) {
        NeuralNetwork& parentA = selectParent();
        NeuralNetwork& parentB = selectParent();
        NeuralNetwork child = crossover(parentA, parentB);
        mutate(child);
        newGeneration.push_back(child);
    }

    population = newGeneration;
    std::fill(fitness.begin(), fitness.end(), 0.0);
    generation++;
}

NeuralNetwork& Population::selectParent() {
    const int TOURNAMENT_SIZE = 5; 
    NeuralNetwork* winner = nullptr;
    double best_fit = -1.0;
    std::uniform_int_distribution<size_t> dist(0, population.size() - 1);

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        size_t index = dist(ga_randomEngine);
        if (fitness[index] > best_fit) {
            best_fit = fitness[index];
            winner = &population[index];
        }
    }
    return *winner;
}

NeuralNetwork Population::crossover(NeuralNetwork& parentA, NeuralNetwork& parentB) {
    std::vector<double> genesA = parentA.getGenes();
    std::vector<double> genesB = parentB.getGenes();
    size_t geneCount = genesA.size();
    std::vector<double> childGenes;
    childGenes.reserve(geneCount);
    std::uniform_real_distribution<double> coinFlip(0.0, 1.0);

    for (size_t i = 0; i < geneCount; ++i) {
        if (coinFlip(ga_randomEngine) < 0.5) childGenes.push_back(genesA[i]);
        else childGenes.push_back(genesB[i]);
    }
    
    NeuralNetwork child(topology, ActivationType::RELU);
    child.setGenes(childGenes);
    return child;
}

void Population::mutate(NeuralNetwork& brain) {
    const double MUTATION_RATE = 0.05; 
    const double MUTATION_STRENGTH = 0.2; 
    std::vector<double> genes = brain.getGenes();
    std::uniform_real_distribution<double> rateDist(0.0, 1.0);
    std::normal_distribution<double> amountDist(0.0, MUTATION_STRENGTH);

    for (double& gene : genes) {
        if (rateDist(ga_randomEngine) < MUTATION_RATE) {
            gene += amountDist(ga_randomEngine);
            if (gene > 1.0) gene = 1.0;
            if (gene < -1.0) gene = -1.0;
        }
    }
    brain.setGenes(genes);
}
