from cell import Cell

class Grid:
    cells : list[list[Cell]]
    radius : float
    width : float
    height : float

    def __init__(self, win_width, win_height, radius_):
        self.radius = radius_
        self.width = int(win_width) // radius_ + 1
        self.height = int(win_height) // radius_ + 1
        self.cells = [[Cell(j + i * self.width) for j in range(self.width)] for i in range(self.height)]

    def update(self, pop : list):
        """updates the content of the cells (clear then refill)
        
        Keyword arguments:
        pop -- list of objects (Animal or Tree)
        """
        
        for cell in self.cells:
            cell.clear()
        for element in pop:
            x = int(element.pos[0])
            y = int(element.pos[1])
            self.cells[x // self.radius + y // self.radius * self.width].add(element) # maybe element.index??

    def get_neighbours(self, index) -> list[Cell]:
        """returns the list of cells neighbouring the index cell

        Keyword arguments:
        index -- index of the center cell
        """
        i, j = index // self.width, index % self.width
        res = [self.cells[i][j]]                                                                       # center
        res.append(self.cells[(self.height + i - 1) % self.height][(self.width + j - 1) % self.width]) # top left
        res.append(self.cells[(self.height + i - 1) % self.height][j])                                 # top
        res.append(self.cells[(self.height + i - 1) % self.height][(j + 1) % self.width])              # top right
        res.append(self.cells[i][(self.width + j - 1) % self.width])                                   # left
        res.append(self.cells[i][(j + 1) % self.width])                                                # right
        res.append(self.cells[(i + 1) % self.height][(self.width + j - 1) % self.width])               # bottom left
        res.append(self.cells[(i + 1) % self.height][j])                                               # bottom
        res.append(self.cells[(i + 1) % self.height][(j + 1) % self.width])                            # bottom right

        return res