from processing_py import App #type: ignore
from simulation import *

def main():
    app = App(800, 800)
    simulation = Simulation(app)
    while True:
        simulation.update()
        simulation.show()
        draw(app)

# app.fill(r, g, b)
# app.circle(pos_x, pos_y, rayon)

    
def draw(app):
    app.background(120)
    app.redraw()


if __name__ == "__main__":
    main()
