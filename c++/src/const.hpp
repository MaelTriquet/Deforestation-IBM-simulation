#include <cmath>
#pragma once

<<<<<<< HEAD
<<<<<<< HEAD
#define MAX_POP_PRED 330
#define MAX_POP_PRED_PERCENT 0.8669624893577113
#define MAX_POP_PREY 583
#define MAX_POP_PREY_PERCENT 0.6268111979773885
#define TREE_START 555
#define FRUIT_ENERGY 98
#define TIME_TREE_GROWTH 247.85950734658906
#define MAX_ENERGY 2568
#define ROT_TIME 381
#define REPRODUCTION_TIMEOUT 102
#define PRED_GAIN_ENERGY_EATING 92
#define PRED_LOST_ENERGY_FIGHT_BY_PREY 968
#define PRED_PASSIVE_ENERGY_LOSS 1
#define PRED_N_MIN_CHILDREN 1
#define PRED_N_MAX_CHILDREN 5
#define PREY_LOST_ENERGY_FIGHT_BY_PRED 733
#define PREY_PASSIVE_ENERGY_LOSS 0
#define PREY_N_MIN_CHILDREN 3
#define PREY_N_MAX_CHILDREN 7
#define MAX_HEALTH 757
#define PRED_START MAX_POP_PRED * MAX_POP_PRED_PERCENT
#define PREY_START MAX_POP_PREY * MAX_POP_PREY_PERCENT
#define WINDOW_HEIGHT 800
=======
// simulation
>>>>>>> a3aee00de817078f60b0f0fb4cbf0909e7f55ba9
=======
// simulation
>>>>>>> 2fdc633a70f808a914e9e92fe18571e7df03a12f
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// initialisation
#define MAX_POP_PRED 564
#define MAX_POP_PRED_PERCENT 0.6289917054021859
#define PRED_START MAX_POP_PRED * MAX_POP_PRED_PERCENT
#define MAX_POP_PREY 589
#define MAX_POP_PREY_PERCENT 0.7526646964917078
#define PREY_START MAX_POP_PREY * MAX_POP_PREY_PERCENT
#define TREE_START 307

// trees
#define MIN_TREES_RADIUS 2
#define MAX_TREES_RADIUS 12
#define PROB_TREE_RANDOM_SPAWN 0.2
#define TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT 0.8
#define FRUIT_ENERGY 53
#define TIME_TREE_GROWTH 553

// animals
#define ANIMALS_RADIUS 9
#define MAX_HEALTH 1533
#define MAX_ENERGY 4893
#define INITIAL_ENERGY MAX_ENERGY * 0.8
#define NB_RAY 8
#define RAY_LENGTH 70
#define INVISIBILITY_TIME 1000
#define REPRODUCTION_TIMEOUT 190
#define LOST_ENERGY_REPRODUCTION 10
#define MUTATION_RATE 0.75
#define PROB_ADD_CONNECTION 0.15
#define PROB_ADD_NEURON 0.15
#define ROT_TIME 821
#define RADIUS_PERCENT_ROTTING 0.8
#define ROTTING_PER_FRAME 1

// preys
#define PREY_MAX_VELOCITY 5
#define PREY_MAX_RAY_ANGLE M_PI_2
#define PREY_LOST_ENERGY_FIGHT_BY_PRED 142
#define PREY_PASSIVE_ENERGY_LOSS 2
#define PREY_N_MIN_CHILDREN 2
#define PREY_N_MAX_CHILDREN 7

// predators
#define PRED_MAX_VELOCITY 7
#define PRED_MAX_RAY_ANGLE M_PI_2
#define PRED_LOST_ENERGY_FIGHT 0
#define PRED_GAIN_ENERGY_EATING 97
#define PRED_LOST_ENERGY_FIGHT_BY_PREY 857
#define PRED_PASSIVE_ENERGY_LOSS 2
#define PRED_N_MIN_CHILDREN 1
#define PRED_N_MAX_CHILDREN 4

