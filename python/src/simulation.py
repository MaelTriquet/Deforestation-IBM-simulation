import random as rd
from predator import Predator
from prey import Prey

class Simulation :
    
    def __init__(self, app_) :
        self.app = app_
        self.pred_list = [Predator(100, [rd.randint(0, self.app.width), rd.randint(0, self.app.height)], rd.randint(3, 8), 10) for _ in range(20)]
        self.prey_list = [Prey(100, [rd.randint(0, self.app.width), rd.randint(0, self.app.height)], rd.randint(1, 5), 10) for _ in range(20)]
        self.pop = self.pred_list + self.prey_list

    def update(self) :
        for animal in self.pop :
            animal.move(animal.velocity, self.app.width, self.app.height)

    def show(self) :
        self.app.noStroke()
        for animal in self.pop :
            self.app.fill(*animal.color)
            self.app.circle(animal.position[0], animal.position[1], animal.radius)