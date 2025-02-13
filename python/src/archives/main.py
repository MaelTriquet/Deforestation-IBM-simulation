from processing_py import App #type: ignore
from simulation import *

def main():
    app = App(256, 256)
    simulation = Simulation(app)
    while True:
        simulation.update()
        simulation.show()
        draw(app)

def draw(app):
    app.redraw()
    app.background(30)


if __name__ == "__main__":
    main()
