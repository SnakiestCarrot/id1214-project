#include "Trainer.hpp"
#include "NeuralNetwork.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <cmath>

// worker for multi-threaded fitness evaluation
double evaluate_brain_fitness(const std::vector<size_t>& topology, const std::vector<double>& genes, int max_steps) {
    Snake snake;
    Food food(10, 10);
    World world(snake, food);
    NeuralNetwork brain(topology, ActivationType::RELU); 
    brain.setGenes(genes);
    int steps = 0;
    int score_at_last_food = 0;
    int steps_since_last_food = 0;
    while (!world.snake_hit_wall() && !snake.hit_itself() && steps < max_steps) {
        world.handle_ai_input(brain);
        world.update();
        steps++;
        steps_since_last_food++;
        if (world.getScore() > score_at_last_food) {
            score_at_last_food = world.getScore();
            steps_since_last_food = 0;
        }
        if (steps_since_last_food > 150) break;
    }
    return (double)steps + (double)(world.getScore() * 800);
}

Trainer::Trainer()
    : POPULATION_SIZE(500),
      topology{11, 20, 12, 3},
      game(), 
      population(POPULATION_SIZE, topology),
      visualize_mode(false)
{
    if (!game.init()) {
        std::cerr << "Game failed to initialize!" << std::endl;
    }
    std::cout << "Starting in Training Mode. Press 'V' to visualize." << std::endl;
    unsigned int num_threads = std::thread::hardware_concurrency();
    std::cout << "Running on " << num_threads << " threads." << std::endl;
}

Trainer::~Trainer() {
}

void Trainer::run() {
    while (!game.shouldQuit()) {
        
        game.pollEvents();

        if (game.visualizeToggled()) {
            visualize_mode = !visualize_mode;
            std::cout << "Mode set to: " 
                      << (visualize_mode ? "Visualize" : "Training") 
                      << std::endl;
            
            if (visualize_mode) {
                start_visualization();
            } else {
                end_visualization();
            }
        }

        if (visualize_mode) {
            run_visualization();
        } else {
            run_training_generation();
        }
    }
}

void Trainer::start_visualization() {
    vis_snake = std::make_unique<Snake>();
    vis_food  = std::make_unique<Food>(10, 10);
    vis_world = std::make_unique<World>(*vis_snake, *vis_food);
}

void Trainer::end_visualization() {
    vis_world.reset();
    vis_food.reset();
    vis_snake.reset();
}

void Trainer::run_visualization() {
    if (vis_world->snake_hit_wall() || vis_snake->hit_itself()) {
        std::cout << "Champion died. Restarting visualization." << std::endl;
        start_visualization();
    }

    NeuralNetwork& bestBrain = population.getBrain(0);

    vis_world->handle_ai_input(bestBrain);
    vis_world->update();

    game.prepareScene();
    vis_world->draw(game.getRenderer()); // Pass the renderer
    game.presentScene();

    SDL_Delay(1000 / FPS);
}

void Trainer::run_training_generation() {
    std::vector<std::future<double>> fitness_futures;

    for (size_t i = 0; i < population.size(); ++i) {
        std::vector<double> genes = population.getBrain(i).getGenes();
        fitness_futures.push_back(
            std::async(std::launch::async, evaluate_brain_fitness, topology, genes, MAX_STEPS_PER_GAME)
        );
    }

    for (size_t i = 0; i < population.size(); ++i) {
        double fitness = fitness_futures[i].get();
        population.setFitness(i, fitness);
    }

    std::cout << "Gen: " << population.getGeneration() 
              << " | Best: " << population.getBestFitness()
              << " | Avg: " << population.getAverageFitness()
              << std::endl;

    population.evolve();
}