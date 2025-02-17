import sys
import os
import numpy as np #type: ignore
import matplotlib.pyplot as plt #type: ignore
import pandas as pd #type: ignore


# remove the zeros at the end of a data file
def remove_zeros(df) :
    df = df.loc[(df != 0).any(axis=1)]
    return df


# plot the two curves of population over times (preys and predators) 
def plot_pop(preys_list, predators_list, trees_list, save_path="../../data/graphics/temp.png", figsize=(15, 6)) :

    # initialisation
    frames_list = np.arange(1, len(preys_list) + 1, 1)
    plt.rcParams.update({"font.size": 12})

    # plot the graphic
    plt.figure(figsize=figsize)
    plt.plot(frames_list, preys_list, label="Proies", color="blue")
    plt.plot(frames_list, predators_list, label="Prédateurs", color="red")
    plt.plot(frames_list, trees_list, label="Arbres", color="Green")

    # add the labels and title
    plt.xlabel("Frame")
    plt.ylabel("Population")
    plt.title("Évolution des populations des arbres, proies et prédateurs au cours du temps")
    plt.legend()
    plt.grid(True, linestyle="--", alpha=0.7)

    # save the figure
    plt.savefig(save_path, dpi=300, bbox_inches="tight")

    return None


# plot the graphic for the sum of preys and predators
def plot_pop_sum(preys_list, predators_list, save_path="../../data/graphics/temp_bis.png", figsize=(15, 6)) :

    # initialisation
    frames_list = np.arange(1, len(preys_list) + 1, 1)
    total_pop = preys_list + predators_list
    plt.rcParams.update({"font.size": 12})

    # plot the graphic
    _, ax = plt.subplots(figsize=figsize)
    ax.plot(frames_list, total_pop, "k-", linewidth=2, label="Population totale")

    # fill the areas (proportionally)
    ax.fill_between(frames_list, 0, preys_list, color="blue", alpha=0.6, label="Proies")
    ax.fill_between(frames_list, preys_list, total_pop, color="red", alpha=0.6, label="Prédateurs")

    # display settings
    ax.set_xlabel("Frame")
    ax.set_ylabel("Population")
    ax.set_title("Évolution de la population totale au cours du temps")
    ax.legend()

    # save the figure
    plt.savefig(save_path, dpi=300, bbox_inches="tight")

    return None


# execution of the program
if __name__ == "__main__" :

    # get the inline arguments
    if len(sys.argv) == 1 : 
        print("\n/!\\ Merci de mettre des arguments lors de l'exécution du programme /!\\ :")
        print("\t- nom du fichier CSV à traiter, sans l'extension")
        print("\t- 0 pour enregistrer proprement les graphes, 1 pour des fichiers temporaires")
        print("\t- les bornes des frames (en préciser deux)\n")
        quit()
    file_name = sys.argv[1]
    temp = bool(int(sys.argv[2])) if len(sys.argv) >= 3 else True
    a, b = (int(sys.argv[3]), int(sys.argv[4])) if len(sys.argv) >= 5 else (0, 100000)

    # manage the files and folders
    file_path = os.path.join("..", "..", "res" if file_name == "plot_info" else "data", f"{file_name}.csv")
    if temp :
        save_path_1 = os.path.join("..", "..", "data", "graphics", "temp1.png")
        save_path_2 = os.path.join("..", "..", "data", "graphics", "temp2.png")
    else :
        results_path = os.path.join("..", "..", "data", "graphics", file_name)
        if not os.path.exists(results_path) :
            os.makedirs(results_path)
        save_path_1 = os.path.join(results_path, "fig1.png")
        save_path_2 = os.path.join(results_path, "fig2.png")

    # load the data
    data = pd.read_csv(file_path)
    data = remove_zeros(data)
    data.columns = ["Predators", "Preys", "Corpses", "Trees"]

    # prepare the data
    preys_list = data["Preys"].to_numpy()[a:b]
    predators_list = data["Predators"].to_numpy()[a:b]
    trees_list = data["Trees"].to_numpy()[a:b]

    # plot the curves
    plot_pop(preys_list, predators_list, trees_list, save_path=save_path_1)
    plot_pop_sum(preys_list, predators_list, save_path=save_path_2)
