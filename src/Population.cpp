#include "Population.hpp"
#include <random>       // For all GA-related randomness
#include <algorithm>    // For std::max_element, std::fill
#include <stdexcept>    // For error handling

// Mersenne Twister random engine for GA operations
static std::mt19937 ga_randomEngine(std::random_device{}());


Population::Population(size_t popSize, const std::vector<size_t>& topology) 
    : topology(topology), generation(0), bestFitness(0.0) {
    
    if (popSize == 0) {
        throw std::invalid_argument("Population size must be greater than 0.");
    }
    
    // pre-allocate
    population.reserve(popSize);
    fitness.resize(popSize, 0.0);

    // Create the initial population of brains
    for (size_t i = 0; i < popSize; ++i) {
        population.emplace_back(topology, ActivationType::RELU);
    }
}

// public methods

double Population::getAverageFitness() const {
    double totalFitness = 0.0;
    for (double fit : fitness) {
        totalFitness += fit;
    }
    return totalFitness / population.size();
}

NeuralNetwork& Population::getBrain(size_t index) {
    if (index >= population.size()) {
        throw std::out_of_range("Brain index out of range.");
    }
    return population[index];
}

void Population::setFitness(size_t index, double score) {
    if (index >= fitness.size()) {
        throw std::out_of_range("Fitness index out of range.");
    }
    fitness[index] = score;
}

/**
 * @brief This is the core GA function.
 * It creates a new generation of brains from the old one.
 */
void Population::evolve() {
    // Find the best brain from the current generation
    auto bestIt = std::max_element(fitness.begin(), fitness.end());
    bestFitness = *bestIt;
    size_t bestBrainIndex = std::distance(fitness.begin(), bestIt);

    std::vector<NeuralNetwork> newGeneration;
    newGeneration.reserve(population.size());

    // Elitism: Add the best brain to the new
    //      generation, unchanged. This guarantees we never "lose" progress.
    //      We get its genes and create a new brain with those genes.
    std::vector<double> championGenes = population[bestBrainIndex].getGenes();
    NeuralNetwork champion(topology, ActivationType::RELU);
    champion.setGenes(championGenes);
    newGeneration.push_back(champion);

    // fill the rest of the new generation
    for (size_t i = 1; i < population.size(); ++i) {
        // select two parents
        NeuralNetwork& parentA = selectParent();
        NeuralNetwork& parentB = selectParent();

        // create a child by crossing them over
        NeuralNetwork child = crossover(parentA, parentB);

        // mutate the child
        mutate(child);

        // add the new child to the generation
        newGeneration.push_back(child);
    }

    population = newGeneration;

    std::fill(fitness.begin(), fitness.end(), 0.0);

    generation++;
}


// private methods

/**
 * @brief Selects a parent using "Tournament Selection".
 * We pick N random brains and the one with the highest fitness "wins".
 */
NeuralNetwork& Population::selectParent() {
    const int TOURNAMENT_SIZE = 5; 
    
    NeuralNetwork* winner = nullptr;
    double best_fitness = -1.0;

    // pick random indexes for the tournament
    std::uniform_int_distribution<size_t> dist(0, population.size() - 1);

    for (int i = 0; i < TOURNAMENT_SIZE; ++i) {
        size_t index = dist(ga_randomEngine);
        if (fitness[index] > best_fitness) {
            best_fitness = fitness[index];
            winner = &population[index];
        }
    }
    // Return a reference to the winning brain
    return *winner;
}

/**
 * @brief Creates a new child brain by combining the genes of two parents.
 * This implementation uses Uniform Crossover.
 */
NeuralNetwork Population::crossover(NeuralNetwork& parentA, NeuralNetwork& parentB) {
    std::vector<double> genesA = parentA.getGenes();
    std::vector<double> genesB = parentB.getGenes();
    size_t geneCount = genesA.size();

    std::vector<double> childGenes;
    childGenes.reserve(geneCount);

    // "coin flip" distribution
    std::uniform_real_distribution<double> coinFlip(0.0, 1.0);

    // for each gene, flip a coin to decide
    // which parent it comes from.
    for (size_t i = 0; i < geneCount; ++i) {
        if (coinFlip(ga_randomEngine) < 0.5) {
            childGenes.push_back(genesA[i]);
        } else {
            childGenes.push_back(genesB[i]);
        }
    }
    
    // create the new child brain and set its combined genes
    NeuralNetwork child(topology, ActivationType::RELU);
    child.setGenes(childGenes);
    return child;
}

/**
 * @brief Randomly alters the genes of a brain.
 */
void Population::mutate(NeuralNetwork& brain) {
    const double MUTATION_RATE = 0.08;
    const double MUTATION_STRENGTH = 0.32; 

    std::vector<double> genes = brain.getGenes();
    
    // distribution for if a mutation happens
    std::uniform_real_distribution<double> rateDist(0.0, 1.0);
    
    // distribution for the strength of a mutation
    std::normal_distribution<double> amountDist(0.0, MUTATION_STRENGTH);

    for (double& gene : genes) {
        if (rateDist(ga_randomEngine) < MUTATION_RATE) {
            // Add a small random value +-N(0, MUTATION_STRENGTH)
            gene += amountDist(ga_randomEngine);
            
            // restrict gene to between -1.0 and 1.0
            if (gene > 1.0) gene = 1.0;
            if (gene < -1.0) gene = -1.0;
        }
    }

    brain.setGenes(genes);
}