import random as rd

class Animal :

    radius = 10

    def __init__(self, energy_, position_, velocity_, max_velocity_) :
        self.energy = energy_
        self.position = position_
        self.velocity = velocity_
        self.max_velocity = max_velocity_
        self.id_dead = False

    def move(self, velocity, window_width, window_height) :
        new_x = (self.position[0] + velocity) % window_width
        new_y = (self.position[1] + velocity) % window_height
        self.position = [new_x, new_y]

    def eat() :
        pass

    def reproduce() :
        pass

    def die() :
        pass