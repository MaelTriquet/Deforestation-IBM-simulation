from predator import Predator
from prey import Prey

class Population:
    pop : list # Animal or Tree
    start_nb_pred : int
    start_nb_prey : int

    def __init__(self, start_nb_pred_, start_nb_prey_):
        self.start_nb_pred = start_nb_pred_
        self.start_nb_prey = start_nb_prey_
        pred = [Predator() for _ in range(self.start_nb_pred)]
        prey = [Prey() for _ in range(self.start_nb_prey)]
        self.pop = pred + prey
    
    def update(self):
        for i in range(len(self.pop)-1, -1, -1): # going down because potentially altering the size of the list throughout the loop
            if not self.pop[i].is_dead:
                self.pop[i].update()
                continue
            self.pop[i].rotting -= 1
            if self.pop[i].rotting <= 0:
                self.pop.remove(i)
    
    def show(self):
        for animal in self.pop:
            animal.show()