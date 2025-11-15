#include "NeuralNetwork.hpp"
#include <stdexcept> 
#include <cmath>     

// Initialize the static random number generator
std::mt19937 NeuralNetwork::randomEngine(std::random_device{}());

// --- Constructor ---
// CHANGED: std::vector<int> to std::vector<size_t>
NeuralNetwork::NeuralNetwork(const std::vector<size_t>& topology, ActivationType funcType) {
    if (topology.size() < 2) {
        throw std::invalid_argument("Topology must have at least 2 layers (input and output).");
    }
    this->topology = topology;
    setActivation(funcType);

    for (size_t i = 1; i < topology.size(); ++i) {
        // CHANGED: int to size_t
        size_t numNeurons = topology[i];
        size_t numPrevLayerNeurons = topology[i - 1];

        Layer newLayer;
        
        newLayer.biases.resize(numNeurons);
        newLayer.weights.resize(numNeurons, std::vector<double>(numPrevLayerNeurons));

        // Initialize all weights and biases with random values
        // CHANGED: int n/p to size_t n/p
        for (size_t n = 0; n < numNeurons; ++n) {
            newLayer.biases[n] = getRandomDouble();
            for (size_t p = 0; p < numPrevLayerNeurons; ++p) {
                newLayer.weights[n][p] = getRandomDouble();
            }
        }
        layers.push_back(newLayer); // Add the newly created layer
    }
}

// --- Public Methods ---

std::vector<double> NeuralNetwork::feedForward(const std::vector<double>& inputs) {
    // This warning is now fixed because inputs.size() (size_t)
    // is compared with topology[0] (size_t)
    if (inputs.size() != topology[0]) {
        throw std::invalid_argument("Input vector size does not match input layer topology.");
    }

    std::vector<double> currentOutputs = inputs;

    for (const auto& layer : layers) {
        // CHANGED: int to size_t
        size_t numNeurons = layer.biases.size();
        size_t numPrevLayerNeurons = layer.weights[0].size();
        
        std::vector<double> nextOutputs(numNeurons);

        // Calculate the value for each neuron in this layer
        // CHANGED: int n/p to size_t n/p
        for (size_t n = 0; n < numNeurons; ++n) {
            double sum = layer.biases[n];

            for (size_t p = 0; p < numPrevLayerNeurons; ++p) {
                sum += currentOutputs[p] * layer.weights[n][p];
            }
            
            nextOutputs[n] = activation(sum);
        }
        
        currentOutputs = nextOutputs;
    }

    return currentOutputs;
}

std::vector<double> NeuralNetwork::getGenes() const {
    std::vector<double> genes;
    
    for (const auto& layer : layers) {
        genes.insert(genes.end(), layer.biases.begin(), layer.biases.end());
        for (const auto& neuronWeights : layer.weights) {
            genes.insert(genes.end(), neuronWeights.begin(), neuronWeights.end());
        }
    }
    return genes;
}

void NeuralNetwork::setGenes(const std::vector<double>& genes) {
    // CHANGED: int to size_t
    // This fixes all warnings in this function
    size_t geneIndex = 0; 
    
    for (auto& layer : this->layers) {
        // Set biases
        for (double& bias : layer.biases) {
            if (geneIndex >= genes.size()) throw std::out_of_range("Gene vector is too small.");
            bias = genes[geneIndex++];
        }
        // Set weights
        for (auto& neuronWeights : layer.weights) {
            for (double& weight : neuronWeights) {
                if (geneIndex >= genes.size()) throw std::out_of_range("Gene vector is too small.");
                weight = genes[geneIndex++];
            }
        }
    }

    if (geneIndex != genes.size()) {
        throw std::runtime_error("Gene vector size did not match the network's structure.");
    }
}

// --- Private Helper Methods ---

void NeuralNetwork::setActivation(ActivationType funcType) {
    switch (funcType) {
        case ActivationType::SIGMOID:
            activation = sigmoid;
            break;
        case ActivationType::RELU:
            activation = relu;
            break;
        case ActivationType::TANH:
            activation = tanh;
            break;
    }
}

double NeuralNetwork::getRandomDouble() {
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    return dist(randomEngine);
}