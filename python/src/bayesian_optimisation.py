import subprocess
import re
import sigopt #type: ignore

# constants
N_ITER = 10
CONST_PARAMETERS = {
    "PRED_START" : "MAX_POP_PRED / 2",
    "PREY_START" : "MAX_POP_PREY / 2",
    "MAX_TREES_RADIUS" : "12",
    "MIN_TREES_RADIUS" : "2",
    "PROB_TREE_RANDOM_SPAWN" : "0.2",
    "ANIMALS_RADIUS" : "9",
    "INITIAL_ENERGY" : "MAX_ENERGY * 0.8",
    "MAX_HEALTH" : "MAX_ENERGY",
    "MUTATION_RATE" : "0.8",
    "PROB_ADD_CONNECTION" : "0.25",
    "PROB_ADD_NEURON" : "0.08",
    "NB_RAY" : "8",
    "RAY_LENGTH" : "101",
    "PRED_LOST_ENERGY_FIGHT" : "0",
    "PRED_MAX_VELOCITY" : "7",
    "PRED_MAX_RAY_ANGLE" : "M_PI_2",
    "PREY_MAX_VELOCITY" : "5",
    "PREY_MAX_RAY_ANGLE" : "M_PI_2"
}
SIGOPT_PARAMETERS = [
    {"name": "MAX_POP_PRED",                   "type": "int",    "bounds": {"min": 100, "max": 300}},
    {"name": "MAX_POP_PREY",                   "type": "int",    "bounds": {"min": 100, "max": 300}},
    {"name": "TREE_START",                     "type": "int",    "bounds": {"min": 200, "max": 700}},
    {"name": "FRUIT_ENERGY",                   "type": "int",    "bounds": {"min": 10, "max": 200}},
    {"name": "TIME_TREE_GROWTH",               "type": "double", "bounds": {"min": 60, "max": 600}},
    {"name": "MAX_ENERGY",                     "type": "int",    "bounds": {"min": 500, "max": 5000}},
    {"name": "ROT_TIME",                       "type": "int",    "bounds": {"min": 50, "max": 1000}},
    {"name": "LOST_ENERGY_REPRODUCTION",       "type": "int",    "bounds": {"min": 0, "max": 100}},
    {"name": "REPRODUCTION_TIMEOUT",           "type": "int",    "bounds": {"min": 50, "max": 300}},
    {"name": "PRED_GAIN_ENERGY_EATING",        "type": "int",    "bounds": {"min": 50, "max": 300}},
    {"name": "PRED_LOST_ENERGY_FIGHT_BY_PREY", "type": "int",    "bounds": {"min": 0, "max": 1000}},
    {"name": "PRED_PASSIVE_ENERGY_LOSS",       "type": "int",    "bounds": {"min": 0, "max": 6}},
    {"name": "PRED_N_MIN_CHILDREN",            "type": "int",    "bounds": {"min": 1, "max": 2}},
    {"name": "PRED_N_MAX_CHILDREN",            "type": "int",    "bounds": {"min": 2, "max": 5}},
    {"name": "INVISIBILITY_TIME",              "type": "int",    "bounds": {"min": 100, "max": 3000}},
    {"name": "PREY_LOST_ENERGY_FIGHT_BY_PRED", "type": "int",    "bounds": {"min": 0, "max": 2000}},
    {"name": "PREY_PASSIVE_ENERGY_LOSS",       "type": "int",    "bounds": {"min": 0, "max": 3}},
    {"name": "PREY_N_MIN_CHILDREN",            "type": "int",    "bounds": {"min": 1, "max": 3}},
    {"name": "PREY_N_MAX_CHILDREN",            "type": "int",    "bounds": {"min": 3, "max": 7}},
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
    score = 0
    best_avg_score = 0
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

        # message
        print(f"----- Itération : {i} -----")
        print(f"--> Best score : {best_avg_score}")
        print(f"--> Best parameters : {best_params}")
        print("="*100)

        # new SigOpt suggestion, given by Bayesian optimization
        suggestion = conn.experiments(experiment.id).suggestions().create()
        current_params = suggestion.assignments

        # apply the new constants to the .hpp file
        dict_to_hpp(current_params, file_path="../../c++/src/const.hpp")

        # run the bash script : the three simulations run and the results are written in the corresponding folder
        subprocess.run(["bash", "./run.sh"])

        # read the results of the three simulations and get their average score
        avg_score = 0
        for j in range(3) :
            with open(f"../../res/bayes/settings_{i}/run{j}/results.txt", "r") as file :
                lines_list = file.readlines()
            avg_score += float(lines_list[0].strip())
        avg_score /= 3

        # we give back the score to SigOpt to help it find the next point to evaluate
        conn.experiments(experiment.id).observations().create(
            suggestion = suggestion.id,
            values=[{"name" : "score", "value" : avg_score}],
        )

        # end of the iteration
        # only the best accuracy is saved and new random parameters are generated
        if avg_score > best_avg_score :
            best_avg_score = score
            best_params = current_params.copy()

    return best_params, best_avg_score

# test of the program
if __name__ == "__main__" :
    const_dict = {"salut": 5, "coucou": 6, "hello": 7}
    dict_to_hpp(const_dict)




"""
Python : 
- lancer le bash
- lire les résultats de la simulation
- les envoyer à SigOpt
- récupérer les nouveaux paramètres
- relancer le bash
en boucle --> n fois
"""