
#include "NeuralNetwork.hpp"


NeuralNetwork::NeuralNetwork(int input_size, int hidden_size, int output_size) {
    // Initialize weights and biases with random values
    weights_input_hidden.resize(hidden_size, std::vector<double>(input_size));
    weights_output_hidden.resize(output_size, std::vector<double>(hidden_size));
    biases_hidden.resize(hidden_size);
    biases_output.resize(output_size);

    for (auto& row : weights_input_hidden)
        for (auto& weight : row)
            weight = ((double) rand() / RAND_MAX) * 2 - 1; // Random values between -1 and 1

    for (auto& row : weights_output_hidden)
        for (auto& weight : row)
            weight = ((double) rand() / RAND_MAX) * 2 - 1;

    for (auto& bias : biases_hidden)
        bias = ((double) rand() / RAND_MAX) * 2 - 1;

    for (auto& bias : biases_output)
        bias = ((double) rand() / RAND_MAX) * 2 - 1;
}



