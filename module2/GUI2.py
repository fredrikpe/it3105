
from Tkinter import Tk, Canvas, BOTH, W, N, E, S
from ttk import Frame, Button, Label, Style

import sys

import AStar
import VC

class Example(Frame):

    def __init__(self, parent, start, paths):
        Frame.__init__(self, parent)
        self.paths = paths
        self.start = start
        self.parent = parent
        self.p_num = 0

        self.color_list = ["blue", "green", "red", "yellow",
            "orange", "purple", "pink", "brown", "magenta", "black"]

        self.x_max = max(n.p[0] for n in start.nodes)
        self.y_max = max(n.p[1] for n in start.nodes)

        self.dim = 500.0

        self.xs = self.dim/(self.x_max + 1)
        self.ys = self.dim/(self.y_max + 1)

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

        abtn = Button(self, text="Next path", command=self.paintState)
        abtn.grid(row=1, column=3)

        cbtn = Button(self, text="Close")
        cbtn.grid(row=2, column=3, pady=4)

        hbtn = Button(self, text="Help")
        hbtn.grid(row=5, column=0, padx=5)

        obtn = Button(self, text="OK")
        obtn.grid(row=5, column=3)

    def createCanvas(self):
        xs, ys = self.xs, self.ys
        self.canvas = Canvas(self)
        self.canvas.create_rectangle(1, 1, self.dim, self.dim,
            outline="#fff", fill="#fff")
        self.canvas.grid(row=1, column=0, columnspan=2, rowspan=4,
            padx=5, sticky=E+W+S+N)

        print (self.x_max, self.y_max)
        for n in self.start.nodes:
            self.canvas.create_oval(4+n.p[0]*xs, 4+n.p[1]*ys,  n.p[0]*xs+16, n.p[1]*ys+16,
                fill="#000")
        for c in self.start.constraints:
            self.canvas.create_line(10+c.vars[0].p[0]*xs, 10+c.vars[0].p[1]*ys,
                10+c.vars[1].p[0]*xs, 10+c.vars[1].p[1]*ys)

    def paintState(self):
        xs, ys = self.xs, self.ys
        if self.p_num == len(self.paths):
            self.p_num = 0
        for n in self.paths[self.p_num][-1].nodes:
            if len(n.domain) == 1:
                c = n.domain[0]
            else: c = -1
            self.canvas.create_oval(4+n.p[0]*xs, 4+n.p[1]*ys,  n.p[0]*xs+16, n.p[1]*ys+16,
                fill=self.color_list[c])
        self.p_num += 1


def main():
    #print all(map(lambda v:v==1, [1, 1, 1]));return
    NP = VC.VCProblem()

    AS = AStar.AStar(NP)

    paths = AS.solve()
    #print paths[-1][-1]

    root = Tk()
    root.geometry("640x600+200+100")
    app = Example(root, NP.start, paths)
    root.mainloop()


if __name__ == '__main__':
    main()
