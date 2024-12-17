from cell import Cell
class Grid:
    cells : list[list[Cell]]
    radius : float
    width : float
    height : float

    def __init__(self, win_width, win_height, radius_):
        self.radius = radius_
        self.width = int(win_width - 1e-5) // radius_ + 1
        self.height = int(win_height - 1e-5) // radius_ + 1
        self.cells = [[Cell(j + i * self.width) for j in range(self.width)] for i in range(self.height)]

    def update(self, pop : list):
        """updates the content of the cells (clear then refill)
        
        Keyword arguments:
        pop -- list of objects (Animal or Tree)
        """
        
        for cell in self.cells:
            cell.clear()
        for element in pop:
            x = int(element.position[0])
            y = int(element.position[1])
            self.cells[x // self.radius + y // self.radius * self.width].add(element) # maybe element.index??

    def get_neighbours(self, index : int) -> list[Cell]:
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
    
def test():

    #test init 1
    grid = Grid(800, 800, 100)
    if (len(grid.cells) != 8 or len(grid.cells[0]) != 8):
        print("Error : size 1 fail")
    else:
        print("Success : size 1 correct")

    #test init 2
    grid = Grid(901, 901, 100)
    if (len(grid.cells) != 10 or len(grid.cells[0]) != 10):
        print("Error : size 2 fail")
    else:
        print("Success : size 2 correct")

    #test neighbours
    neighbours = grid.get_neighbours(11) # 1, 1
    neighbours_idx = [n.index for n in neighbours]
    if not 0 in neighbours_idx:
        print("Error : top left")
    elif not 1 in neighbours_idx:
        print("Error : top")
    elif not 2 in neighbours_idx:
        print("Error : top right")
    elif not 10 in neighbours_idx:
        print("Error : left")
    elif not 12 in neighbours_idx:
        print("Error : right")
    elif not 20 in neighbours_idx:
        print("Error : bot left")
    elif not 21 in neighbours_idx:
        print("Error : bot")
    elif not 22 in neighbours_idx:
        print("Error : bot right")
    elif not 11 in neighbours_idx:
        print("Error : self")
    else:
        print("Success : neighbours correct")
if __name__ == "__main__":
    test()