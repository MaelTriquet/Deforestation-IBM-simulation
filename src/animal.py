import random as rd

class Animal :

    def __init__(self, energy_, position_, velocity_, max_velocity_, color_) :
        self.energy = energy_
        self.position = position_
        self.velocity = velocity_
        self.max_velocity = max_velocity_

    def move(self, next_position = None) :
        if next_position is None :
            next_position = [coord + rd.randint() for coord in self.position]
        self.position = next_position

    def eat() :
        pass

    def reproduce() :
        pass

    def die() :
        pass