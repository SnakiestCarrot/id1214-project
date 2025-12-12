#pragma once

#include <vector>
#include <functional> 
#include <string>
#include <random>     

enum class ActivationType {
    SIGMOID,
    RELU,
    TANH
};

class NeuralNetwork {
public:
    /**
     * @brief Creates a new neural network.
     * @param topology A vector of unsigned integers defining the number 
     * of neurons in each layer. (e.g., {5, 8, 3})
     * @param funcType The activation function to use (e.g., RELU)
     */
    NeuralNetwork(const std::vector<size_t>& topology, ActivationType funcType);

    /**
     * @brief Performs the feed-forward calculation.
     * @param inputs A vector of inputs matching the size of the first layer.
     * @return A vector of outputs from the final layer.
     */
    std::vector<double> feedForward(const std::vector<double>& inputs);

    /**
     * @brief Gets all weights and biases as a single "gene" vector.
     */
    std::vector<double> getGenes() const;

    /**
     * @brief Sets all weights and biases from a "gene" vector.
     */
    void setGenes(const std::vector<double>& genes);

private:
    /**
     * @brief A simple struct to represent a layer.
     */
    struct Layer {
        std::vector<std::vector<double>> weights;
        std::vector<double> biases;
    };

    std::vector<size_t> topology;
    std::vector<Layer> layers; 
    std::function<double(double)> activation; 

    static std::mt19937 randomEngine;

    // Helpers
    void setActivation(ActivationType funcType);
    static double getRandomDouble();

    // Activation functions
    static double sigmoid(double x) { return 1.0 / (1.0 + std::exp(-x)); }
    static double relu(double x) { return std::max(0.0, x); }
    static double tanh(double x) { return std::tanh(x); }
};