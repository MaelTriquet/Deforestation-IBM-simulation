import subprocess
import re
import sigopt #type: ignore

# constants
N_ITER = 10000
CONST_PARAMETERS = {
    "WINDOW_HEIGHT" : "800",
    "WINDOW_WIDTH" : "800",
    "ANIMALS_RADIUS" : "9",
    "MAX_TREES_RADIUS" : "12",
    "MIN_TREES_RADIUS" : "2",
    "RAY_LENGTH" : "101",
}

SIGOPT_PARAMETERS = [
    {"name": "MAX_POP_PRED",                     "type": "int",    "bounds": {"min": 135, "max": 165}},
    {"name": "MAX_POP_PREY",                     "type": "int",    "bounds": {"min": 243, "max": 297}},
    {"name": "TREE_START",                       "type": "int",    "bounds": {"min": 369, "max": 451}},
    {"name": "FRUIT_ENERGY",                     "type": "int",    "bounds": {"min": 162, "max": 198}},
    {"name": "TIME_TREE_GROWTH",                 "type": "double",  "bounds": {"min": 341.14, "max": 416.95}},
    {"name": "MAX_ENERGY",                       "type": "int",    "bounds": {"min": 2880, "max": 3520}},
    {"name": "ROT_TIME",                         "type": "int",    "bounds": {"min": 585, "max": 715}},
    {"name": "LOST_ENERGY_REPRODUCTION",         "type": "int",    "bounds": {"min": 72, "max": 88}},
    {"name": "REPRODUCTION_TIMEOUT",             "type": "int",    "bounds": {"min": 63, "max": 77}},
    {"name": "PRED_GAIN_ENERGY_EATING",          "type": "int",    "bounds": {"min": 171, "max": 209}},
    {"name": "PRED_LOST_ENERGY_FIGHT_BY_PREY",   "type": "int",    "bounds": {"min": 180, "max": 220}},
    {"name": "PRED_PASSIVE_ENERGY_LOSS",         "type": "int",    "bounds": {"min": 5, "max": 7}},
    {"name": "PRED_N_MIN_CHILDREN",              "type": "int",    "bounds": {"min": 1, "max": 2}},  
    {"name": "PRED_N_MAX_CHILDREN",              "type": "int",    "bounds": {"min": 3, "max": 6}},  
    {"name": "INVISIBILITY_TIME",                "type": "int",    "bounds": {"min": 1710, "max": 2090}},
    {"name": "PREY_LOST_ENERGY_FIGHT_BY_PRED",   "type": "int",    "bounds": {"min": 90, "max": 110}},
    {"name": "PREY_PASSIVE_ENERGY_LOSS",         "type": "int",    "bounds": {"min": 2, "max": 4}},  
    {"name": "PREY_N_MIN_CHILDREN",              "type": "int",    "bounds": {"min": 1, "max": 3}},  
    {"name": "PREY_N_MAX_CHILDREN",              "type": "int",    "bounds": {"min": 3, "max": 7}},  
    {"name": "PRED_START",                       "type": "int",    "bounds": {"min": 67, "max": 83}},
    {"name": "PREY_START",                       "type": "int",    "bounds": {"min": 121, "max": 149}},
    {"name": "PROB_TREE_RANDOM_SPAWN",           "type": "double",  "bounds": {"min": 0.18, "max": 0.22}},
    {"name": "INITIAL_ENERGY",                   "type": "double",  "bounds": {"min": 2304, "max": 2816}},
    {"name": "MAX_HEALTH",                       "type": "int",    "bounds": {"min": 2880, "max": 3520}},
    {"name": "MUTATION_RATE",                    "type": "double",  "bounds": {"min": 0.72, "max": 0.88}},
    {"name": "PROB_ADD_CONNECTION",              "type": "double",  "bounds": {"min": 0.225, "max": 0.275}},
    {"name": "PROB_ADD_NEURON",                  "type": "double",  "bounds": {"min": 0.072, "max": 0.088}},
    {"name": "NB_RAY",                           "type": "int",    "bounds": {"min": 7, "max": 9}},
    {"name": "PRED_LOST_ENERGY_FIGHT",           "type": "int",    "bounds": {"min": 0, "max": 2}},  
    {"name": "PRED_MAX_VELOCITY",                "type": "int",    "bounds": {"min": 6, "max": 8}},  
    {"name": "PRED_MAX_RAY_ANGLE",               "type": "double",  "bounds": {"min": 1.41, "max": 1.73}},  
    {"name": "PREY_MAX_VELOCITY",                "type": "int",    "bounds": {"min": 4, "max": 6}},  
    {"name": "PREY_MAX_RAY_ANGLE",               "type": "double",  "bounds": {"min": 1.41, "max": 1.73}},  
    {"name": "ROTTING_PER_FRAME",                "type": "double",  "bounds": {"min": 0.45, "max": 0.55}},  
    {"name": "RADIUS_PERCENT_ROTTING",           "type": "double",  "bounds": {"min": 0.63, "max": 0.77}},  
    {"name": "TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT", "type": "double", "bounds": {"min": 0.72, "max": 0.88}}
]


# transform the constants from a .hpp file to a python dictionary
def hpp_to_dict(file_path="../../c++/src/const.hpp") :

    # initialisation
    const_dict = {}
    
    # read the file
    with open(file_path, "r") as file :
        lines_list = file.readlines()

    # iterate on the lines
    for line in lines_list :
        if re.match(r"#define", line) :
            const_name, const_value = line.split()[1], line.split()[2]
            const_dict[const_name] = const_value

    # keep only the good lines (with numbers, mainly for tests)
    const_dict = {k: v for k, v in const_dict.items() if re.match(r"^[0-9]", v)}

    # convert the values to their types
    for const_name, const_value in const_dict.items() :
        if "." in const_value :
            const_dict[const_name] = float(const_value)
        else :
            const_dict[const_name] = int(const_value)

    return const_dict


# transform the python dictionary to a .hpp file
def dict_to_hpp(const_dict, file_path="../../c++/src/const2.hpp") :

    # open the file
    with open(file_path, "w") as file :

        # imports
        file.write("#include <cmath>\n")
        file.write("#pragma once\n\n")

        # write the non constants constants
        for const_name, const_value in const_dict.items() :
            file.write(f"#define {const_name} {const_value}\n")
        
        # write the (real) constants
        for const_name, const_value in CONST_PARAMETERS.items() :
            file.write(f"#define {const_name} {const_value}\n")

    return


# main
def main() :

    # initialisation
    conn = sigopt.Connection(driver="lite")
    n_iter = N_ITER
    best_max_score = 0
    current_params = {}
    best_params = {}

    # first experiment
    experiment = conn.experiments().create(
        name = "Forest life simulation",
        parameters = SIGOPT_PARAMETERS,
        metrics = [{
            "name"      : "score",
            "objective" : "maximize",
            "strategy"  : "optimize"
        }],
        observation_budget = n_iter
    )

    # iterate to find the best parameters
    for i in range(n_iter) :

        # new SigOpt suggestion, given by Bayesian optimization (or random method)
        # the exploration probability decreases with the number of iterations
        if i % 5 == 0 or i % 5 == 1 :
            suggestion = conn.experiments(experiment.id).suggestions().create(type="random")
        else :
            suggestion = conn.experiments(experiment.id).suggestions().create()
        current_params = suggestion.assignments

        # apply the new constants to the .hpp file
        dict_to_hpp(current_params, file_path="../../c++/src/const.hpp")

        # run the bash script : the three simulations run and the results are written in the corresponding folder
        subprocess.run(["./run.sh", str(i)], check=True, text=True, capture_output=True)

        # read the results of the three simulations and get their average score
        max_score = 0
        for j in range(1, 4) :
            with open(f"../../res/bayes/settings_{i}/run_{j}/results.txt", "r") as file :
                lines_list = file.readlines()
            if max_score < float(lines_list[0].strip()) :
                max_score = float(lines_list[0].strip())

        # we give back the score to SigOpt to help it find the next point to evaluate
        conn.experiments(experiment.id).observations().create(
            suggestion = suggestion.id,
            values=[{"name" : "score", "value" : max_score}],
        )

        # end of the iteration
        # only the best accuracy is saved and new random parameters are generated
        if max_score > best_max_score :
            best_max_score = max_score
            best_params = current_params.copy()

        # message
        print(f"Itération / score / random exploration : {i} / {best_max_score} / {i % 5 in [0, 1]}")

    return best_params, best_max_score


# test of the program
if __name__ == "__main__" :
    main()
