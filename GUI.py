
from Tkinter import Tk, Canvas, BOTH, W, N, E, S
from ttk import Frame, Button, Label, Style

import sys

import AStar
import Navigation as nav

class Example(Frame):

    def __init__(self, parent, dim, start, goal, valid_points, paths):
        Frame.__init__(self, parent)
        self.dim = dim
        self.start = start
        self.goal = goal
        self.valid_points = valid_points
        self.paths = paths
        self.parent = parent
        self.p_num = 0

        self.initUI()

    def initUI(self):

        self.parent.title("Windows")
        self.style = Style()
        self.style.theme_use("default")
        self.pack(fill=BOTH, expand=1)

        self.columnconfigure(1, weight=1)
        self.columnconfigure(3, pad=7)
        self.rowconfigure(3, weight=1)
        self.rowconfigure(5, pad=7)

        lbl = Label(self, text="Windows")
        lbl.grid(sticky=W, pady=4, padx=5)

        self.createCanvas()

        abtn = Button(self, text="Next path", command=self.paintPath)
        abtn.grid(row=1, column=3)

        cbtn = Button(self, text="Close")
        cbtn.grid(row=2, column=3, pady=4)

        hbtn = Button(self, text="Help")
        hbtn.grid(row=5, column=0, padx=5)

        obtn = Button(self, text="OK")
        obtn.grid(row=5, column=3)

    def createCanvas(self):
        self.canvas = Canvas(self)
        self.canvas.create_rectangle(1, 1, 500, 500,
            outline="#000", fill="#000")
        self.canvas.grid(row=1, column=0, columnspan=2, rowspan=4,
            padx=5, sticky=E+W+S+N)
        self.r = 500.0 / self.dim[0]
        self.c = 500.0 / self.dim[1]
        #print self.r, self.c
        for x in range(1,self.dim[0]):
            self.canvas.create_line(1+self.r*x, 1, 1+self.r*x, 500)
        for x in range(1,self.dim[1]):
            self.canvas.create_line(1, 1+self.c*x, 500, 1+self.c*x)
        for p in self.valid_points:
            #print p
            self.canvas.create_rectangle(1+self.r*p[0], 501-self.c*p[1], 1+self.r*p[0]+self.r, 501-self.c*p[1]-self.c,
                outline="#000", fill="#fff")

    def paintPath(self):
        if self.p_num == len(self.paths):
            self.p_num = 0
            self.createCanvas()
        r, c = self.r, self.c
        r_ = r/6
        c_ = c/6
        if self.p_num:
            for p in self.paths[self.p_num-1]:
                self.canvas.create_oval(1+r*p[0]+r_, 501-c*p[1]-c_, 1+r*p[0]+r-r_, 501-c*p[1]-c+c_,
                    fill="#a0a")
        for p in self.paths[self.p_num]:
            self.canvas.create_oval(1+r*p[0]+r_, 501-c*p[1]-c_, 1+r*p[0]+r-r_, 501-c*p[1]-c+c_,
                fill="#ff0")
        self.p_num += 1

def main():
    
    mode, dim, start, goal, obstacles = nav.get_input()

    valid_points = nav.get_valid_points(dim, obstacles)

    #print nav.succ((0,0), valid_points)

    paths = AStar.a_star(lambda p:nav.succ(p, valid_points),
                         lambda p:nav.heur(p, goal),
                         lambda p,q:nav.heur(p,q),
                         start,
                         lambda p:p==goal,
                         mode)

    root = Tk()
    root.geometry("640x600+200+100")
    app = Example(root, dim, start, goal, valid_points, paths)
    root.mainloop()


if __name__ == '__main__':
    main()
