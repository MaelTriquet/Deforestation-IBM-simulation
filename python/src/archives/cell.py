class Cell:
    content : list
    index : int

    def __init__(self, index_):
        self.index = index_
        self.content = []

    def clear(self):
        """clears the content of the cell
        """
        
        self.content = []
    
    def add(self, object):
        """adds the object to the content of the cell
        
        Keyword arguments:
        object -- object to be added (Animal or Tree)
        """
        
        self.content.append(object)