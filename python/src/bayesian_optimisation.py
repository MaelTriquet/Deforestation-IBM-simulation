import subprocess
import re
import glob
import sigopt #type: ignore

# constants
N_ITER = 10000
CONST_PARAMETERS = {
    "PRED_START" : "MAX_POP_PRED * MAX_POP_PRED_PERCENT",
    "PREY_START" : "MAX_POP_PREY * MAX_POP_PREY_PERCENT",
    "WINDOW_HEIGHT" : "800",
    "WINDOW_WIDTH" : "800",
    "ANIMALS_RADIUS" : "9",
    "MAX_TREES_RADIUS" : "12",
    "MIN_TREES_RADIUS" : "2",
    "INITIAL_ENERGY" : "MAX_ENERGY * 0.8",
    "NB_RAY" : "8",
    "RAY_LENGTH" : "70",
    "PRED_LOST_ENERGY_FIGHT" : "0",
    "PRED_MAX_VELOCITY" : "7",
    "PRED_MAX_RAY_ANGLE" : "M_PI_2",
    "PREY_MAX_VELOCITY" : "5",
    "LOST_ENERGY_REPRODUCTION" : "10",
    "PREY_MAX_RAY_ANGLE" : "M_PI_2",
    "INVISIBILITY_TIME" : "1000",
    "MUTATION_RATE" : "0.75",
    "PROB_ADD_CONNECTION" : "0.15",
    "PROB_ADD_NEURON" : "0.15",
    "PROB_TREE_RANDOM_SPAWN" : "0.2",
    "TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT" : "0.8",
    "RADIUS_PERCENT_ROTTING" : "0.8", 
    "ROTTING_PER_FRAME" : "1"
}
SIGOPT_PARAMETERS = [
    {"name": "MAX_POP_PRED",                   "type": "int",    "bounds": {"min": 200, "max": 700}},
    {"name": "MAX_POP_PRED_PERCENT",           "type": "double", "bounds": {"min": 0.5, "max": 1}},
    {"name": "MAX_POP_PREY",                   "type": "int",    "bounds": {"min": 200, "max": 700}},
    {"name": "MAX_POP_PREY_PERCENT",           "type": "double", "bounds": {"min": 0.5, "max": 1}},
    {"name": "TREE_START",                     "type": "int",    "bounds": {"min": 100, "max": 700}},
    {"name": "FRUIT_ENERGY",                   "type": "int",    "bounds": {"min": 10, "max": 200}},
    {"name": "TIME_TREE_GROWTH",               "type": "int",    "bounds": {"min": 120, "max": 600}},
    {"name": "MAX_ENERGY",                     "type": "int",    "bounds": {"min": 500, "max": 5000}},
    {"name": "ROT_TIME",                       "type": "int",    "bounds": {"min": 50, "max": 1000}},
    {"name": "REPRODUCTION_TIMEOUT",           "type": "int",    "bounds": {"min": 50, "max": 300}},
    {"name": "PRED_GAIN_ENERGY_EATING",        "type": "int",    "bounds": {"min": 50, "max": 300}},
    {"name": "PRED_LOST_ENERGY_FIGHT_BY_PREY", "type": "int",    "bounds": {"min": 0, "max": 1000}},
    {"name": "PRED_PASSIVE_ENERGY_LOSS",       "type": "int",    "bounds": {"min": 0, "max": 3}},
    {"name": "PRED_N_MIN_CHILDREN",            "type": "int",    "bounds": {"min": 1, "max": 2}},
    {"name": "PRED_N_MAX_CHILDREN",            "type": "int",    "bounds": {"min": 2, "max": 5}},
    {"name": "PREY_LOST_ENERGY_FIGHT_BY_PRED", "type": "int",    "bounds": {"min": 0, "max": 2000}},
    {"name": "PREY_PASSIVE_ENERGY_LOSS",       "type": "int",    "bounds": {"min": 0, "max": 3}},
    {"name": "PREY_N_MIN_CHILDREN",            "type": "int",    "bounds": {"min": 1, "max": 3}},
    {"name": "PREY_N_MAX_CHILDREN",            "type": "int",    "bounds": {"min": 3, "max": 8}},
    {"name": "MAX_HEALTH",                     "type": "int",    "bounds": {"min": 300, "max": 2000}},
]
CONSTANTS_BY_ORDER = [

    # simulation part
    ["WINDOW_WIDTH", "WINDOW_HEIGHT"], 

    # initialisation part
    ["MAX_POP_PRED", "MAX_POP_PRED_PERCENT", "PRED_START", "MAX_POP_PREY", "MAX_POP_PREY_PERCENT", "PREY_START", "TREE_START"],

    # trees part
    ["MIN_TREES_RADIUS", "MAX_TREES_RADIUS", "PROB_TREE_RANDOM_SPAWN", "TREE_RADIUS_BEFORE_REPRODUCTION_PERCENT", "FRUIT_ENERGY", "TIME_TREE_GROWTH"], 

    # animals part
    ["ANIMALS_RADIUS", "MAX_HEALTH", "MAX_ENERGY", "INITIAL_ENERGY", "NB_RAY", "RAY_LENGTH", "INVISIBILITY_TIME", "REPRODUCTION_TIMEOUT", "LOST_ENERGY_REPRODUCTION", "MUTATION_RATE", "PROB_ADD_CONNECTION", "PROB_ADD_NEURON", "ROT_TIME", "RADIUS_PERCENT_ROTTING", "ROTTING_PER_FRAME"], 

    # preys part
    ["PREY_MAX_VELOCITY", "PREY_MAX_RAY_ANGLE", "PREY_LOST_ENERGY_FIGHT_BY_PRED", "PREY_PASSIVE_ENERGY_LOSS", "PREY_N_MIN_CHILDREN", "PREY_N_MAX_CHILDREN"],

    # predators part 
    ["PRED_MAX_VELOCITY", "PRED_MAX_RAY_ANGLE", "PRED_LOST_ENERGY_FIGHT", "PRED_GAIN_ENERGY_EATING", "PRED_LOST_ENERGY_FIGHT_BY_PREY", "PRED_PASSIVE_ENERGY_LOSS", "PRED_N_MIN_CHILDREN", "PRED_N_MAX_CHILDREN"], 

]
CONSTANTS_CATEGORIES_NAMES = ["simulation", "initialisation", "trees", "animals", "preys", "predators"]

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
    const_dict = {k : v for k, v in const_dict.items() if re.match(r"^[0-9]", v) and k not in CONST_PARAMETERS.keys()}

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

        # write the categories
        for i, category in enumerate(CONSTANTS_BY_ORDER) :
            file.write(f"// {CONSTANTS_CATEGORIES_NAMES[i]}\n")
            for const_name in category :
                if const_name in const_dict.keys() :
                    file.write(f"#define {const_name} {const_dict[const_name]}\n")
                else :
                    file.write(f"#define {const_name} {CONST_PARAMETERS[const_name]}\n")
            file.write("\n")

    return


# get all the previous scores and associate them with the previous number of iterations
def get_previous_scores() :

    # initialisation
    scores_dict = {}
    path_list = glob.glob("../../res/bayes/settings_*/run_1/results.txt")

    # get all the files "results.txt"
    for file_path in path_list[:-1] :
        with open(file_path, "r") as f :
            content = f.readlines()
        score = float(content[0].strip())
        iter = int(file_path.split("/")[4].split("_")[1])
        scores_dict[iter] = score
    
    return scores_dict


# get all constants from the .hpp files for each iteration
def get_previous_constants() :

    # initialisation
    constants_dict = {}
    path_list = glob.glob("../../res/bayes/settings_*/const.hpp")
    
    # get all the files "const.hpp"
    for file_path in path_list :
        iter = int(file_path.split("/")[4].split("_")[1])
        constants_dict[iter] = hpp_to_dict(file_path)

    return constants_dict


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

    # reuse of all previous attempts
    dict_prev_scores = get_previous_scores()
    dict_prev_constants = get_previous_constants()
    for iter, constants in dict_prev_constants.items() :
        conn.experiments(experiment.id).observations().create(
            assignments = constants,
            values=[{"name" : "score", "value" : dict_prev_scores[iter]}],
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
        subprocess.run(["./run_once.sh", str(i)], check=True, text=True, capture_output=True)

        # read the results of the three simulations and get their average score
        max_score = 0
        for j in range(1, 2) :
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
