
from Tkinter import Tk, Canvas, BOTH, W, N, E, S
from ttk import Frame, Button, Label, Style

import sys

import AStar
import VC

class Example(Frame):
    def __init__(self, parent, problem, AS):
        Frame.__init__(self, parent)
        self.AS = AS
        self.paths = AS.paths
        self.P = problem
        self.parent = parent
        self.p_num = 0
        self.color_list = ["blue", "yellow", "red", "green",
            "cyan", "magenta", "pink", "brown", "magenta", "black"]
        self.dim = 600.0
        self.findEq()
        self.initUI()

    def findEq(self):
        xs, ys = zip(*self.P.points.itervalues())
        self.x_x2 = max(xs)
        self.y_x2 = max(ys)
        self.x_x1 = min(xs)
        self.y_x1 = min(ys)
        self.x_y1 = self.y_y1 = 10.0
        self.x_y2 = self.y_y2 = self.dim - 20.0
        self.mx = (self.x_y2 - self.x_y1) / (self.x_x2 - self.x_x1)
        self.my = (self.y_y2 - self.y_y1) / (self.y_x2 - self.y_x1)

    def x_eq(self, x):
        return self.mx*(x-self.x_x1) + self.x_y1

    def y_eq(self, y):
        return self.my*(y-self.y_x1) + self.y_y1

    def initUI(self):
        self.parent.title("Windows")
        self.style = Style()
        self.style.theme_use("default")
        self.pack(fill=BOTH, expand=1)

        self.columnconfigure(1, weight=1)
        self.columnconfigure(3, pad=7)
        self.rowconfigure(3, weight=1)

        uc = self.paths[-1][-1].unsat_cs()
        uv = self.paths[-1][-1].uncolor_vs()
        ns = len(self.AS.closed)
        pn = len(self.paths)
        sl = len(self.paths[-1])
        s = "Unsatisfied consts: " + str(uc)
        s += ". Uncolored verts: " + str(uv)
        s += ". Explored nodes: " + str(ns)
        s += ". Popped nodes: " + str(pn)
        s += ". Length of solution: " + str(sl)
        lbl2 = Label(self, text=s)
        lbl2.grid(sticky=W, pady=4, padx=5)

        self.createCanvas()

        abtn = Button(self, text="Next path", command=self.paintState)
        abtn.grid(row=1, column=3)

        cbtn = Button(self, text="Close")
        cbtn.grid(row=2, column=3, pady=4)

    def createCanvas(self):
        x_eq, y_eq = self.x_eq, self.y_eq
        points = self.P.points
        self.canvas = Canvas(self)
        self.canvas.create_rectangle(1, 1, self.dim, self.dim,
            outline="#fff", fill="#fff")
        self.canvas.grid(row=1, column=0, columnspan=2, rowspan=4,
            padx=5, sticky=E+W+S+N)

        for p in points.itervalues():
            self.createOval(p)
        for c in self.P.constraints:
            self.canvas.create_line(5+x_eq(points[c.vars[0]][0]), 5+y_eq(points[c.vars[0]][1]),
                5+x_eq(points[c.vars[1]][0]), 5+y_eq(points[c.vars[1]][1]))

    def paintState(self):
        x_eq, y_eq = self.x_eq, self.y_eq
        if self.p_num == len(self.paths):
            self.p_num = 0
        for i, d in self.paths[self.p_num][-1].nodes.iteritems():
            if len(d) == 1:
                c = d[0]
            else: c = -1
            self.createOval(self.P.points[i], color=self.color_list[c])
        self.p_num += 1

    def createOval(self, p, color="black"):
        self.canvas.create_oval(self.x_eq(p[0]), self.y_eq(p[1]),
            self.x_eq(p[0])+10, self.y_eq(p[1])+10,
            fill=color)

import time

def main():

    #print dict(['a', 'b']); return
    s = time.time()
    NP = VC.VCProblem()
    s2 = time.time()
    AS = AStar.AStar(NP)
    s3 = time.time()
    AS.solve()
    s4 = time.time()
    print s2-s
    print s3-s2
    print s4-s3
    root = Tk()
    root.geometry("730x650+75+50")
    app = Example(root, NP, AS)
    root.mainloop()

if __name__ == '__main__':
    main()
