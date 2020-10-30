'''Daniel Craig'''

from tkinter import *
import time
import random

screen=Tk()
screen.title("My first screen")
screen.geometry("1024x576")

speedtext = Label(text="0")
rpmtext=Label(text='0',fg="black")
speedtext.config(font=("Arial",130))
rpmtext.config(font=("Arial",60))
speedtext.place(relx=0.1,anchor='nw')
rpmtext.place(rely=0.9,anchor='w')

'''
Camera in the center.
speedometer on the left
below speedometer -> Tachometer
Right side - digital number readout
left side - engine rpm. lower left - number. upper left - dial
right side - miles per hour. Lower right - number. Upper right - dial'''

testSpeed=1
#testSpeed = int(input())
#print('speed:' +str(testSpeed))
def getSpeed(increment):
    return int(20+increment)

#def updateLabel(speedInput):      #Input for speed goes here
while(True):
    testSpeed+=1
    time.sleep(0.1)
    speedtext.config(text=str(testSpeed+5)+" mph")
    rpmtext.config(text=str(random.randint(0,75))+" other")
    screen.update()







#updateLabel(44)
screen.mainloop()