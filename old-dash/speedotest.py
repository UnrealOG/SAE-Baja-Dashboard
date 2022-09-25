import speedometer
from tkinter import *
import time
root=Tk()
canvas=Canvas(root,height=500,width=500)
canvas.pack()
canvas.create_oval(0,0,500,500,tag="oval")
A=speedometer.Speedometer(canvas,"oval",Range=(0,40))
for i in range(1000):
    root.update()
    time.sleep(0.1)
    i+=1
    A.moveto(i,"oval")
A.changerange(Range=(0,40),rfont=("Verdana",22))

root.mainloop()
