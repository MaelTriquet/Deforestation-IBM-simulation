#pragma once
#include <cmath>

// simulation
#define MAX_POP 20
#define PRED_START 40
#define PREY_START 70
#define TREE_START 400

// trees
#define TREES_RADIUS 10
#define FRUIT_ENERGY 300
#define FRUIT_TIME 5

// animals
#define ANIMALS_RADIUS 10
#define INITIAL_ENERGY 1000
#define MUTATION_RATE .08
#define NB_RAY 8
#define RAY_LENGTH 120
#define ROT_TIME 150
#define LOST_ENERGY_REPRODUCTION 50
#define REPRODUCTION_TIMEOUT 60

// predators
#define PRED_GAIN_ENERGY_EATING 250
#define PRED_LOST_ENERGY_FIGHT 10
#define PRED_MAX_VELOCITY 7
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PRED_PERCENT_DAMAGE .12f

// preys
#define INVISIBILITY_TIME 200
#define PREY_LOST_ENERGY_FIGHT 50
#define PREY_MAX_VELOCITY 6
#define PREY_MAX_RAY_ANGLE M_PI*2*.8
