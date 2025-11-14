
#pragma once
#include <vector>

class NeuralNetwork {
public:
    NeuralNetwork(int input_size, int hidden_size, int output_size);
    std::vector<double> feedforward(const std::vector<double>& inputs);

private:
    std::vector<std::vector<double>> weights_input_hidden;
    std::vector<std::vector<double>> weights_output_hidden;
    std::vector<double> biases_hidden;
    std::vector<double> biases_output;
    double activate(double x);
};











