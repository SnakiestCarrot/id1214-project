#pragma once

#include "NeuralNetwork.hpp"
#include <vector>

class Population {
public:
    /**
     * @brief Creates a new population.
     * @param popSize The number of neural networks in this population.
     * @param topology The shape of each brain (e.g., {6, 10, 4}).
     */
    Population(size_t popSize, const std::vector<size_t>& topology);

    /**
     * @brief Gets a specific brain from the population for evaluation.
     */
    NeuralNetwork& getBrain(size_t index);

    /**
     * @brief Sets the fitness for a specific brain after its game is over.
     * @param index The index of the brain.
     * @param score The score that brain achieved (e.g., snake length).
     */
    void setFitness(size_t index, double score);

    /**
     * @brief Runs the GA (selection, crossover, mutation)
     * to create the next generation.
     */
    void evolve();

    /** @brief Gets the current generation number. */
    int getGeneration() const { return generation; }

    /** @brief Gets the highest fitness from the last generation. */
    double getBestFitness() const { return bestFitness; }

    /** @brief Returns the total size of the population. */
    size_t size() const { return population.size(); }

    /** @brief Returns the average fitness of the population. */
    double getAverageFitness() const;

private:
    std::vector<NeuralNetwork> population;
    std::vector<double> fitness; // One fitness score for each brain
    std::vector<size_t> topology;
    int generation;
    double bestFitness;

    // --- Private GA Helper Methods ---

    /** @brief Selects a parent for reproduction (e.g., tournament). */
    NeuralNetwork& selectParent();

    /** @brief Creates a "child" from two "parent" brains. */
    NeuralNetwork crossover(NeuralNetwork& parentA, NeuralNetwork& parentB);

    /** @brief Randomly modifies the genes (weights/biases) of a brain. */
    void mutate(NeuralNetwork& brain);
};