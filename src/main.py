from processing_py import * #type: ignore

def main():
    app = App(800, 800)
    while True:
        draw(app)

    
def draw(app):
    app.background(0)


if __name__ == "__main__":
    main()