'''Daniel Craig'''
'''I PlEdGe My HoNoR tHaT i HaVe AbIdEd By ThE sTeVeNs HoNoR sYsTeM'''

from tkinter import *
import time
import random
import speedometer

#Tkinter, initialize all variables
root=Tk()
screen=Canvas(root,height=576,width=1024)
speedtext = Label(text="0")
rpmtext=Label(text='0',fg="black")
speedtext.config(font=("Arial",42))
rpmtext.config(font=("Arial",60))
speedtext.place(rely=0.43,relx=0.132,anchor='w')
rpmtext.place(rely=0.9,relx=1.0,anchor='e')
screen.pack()
screen.create_oval(30,30,300,300,tag="oval")
screen.create_oval(1000,30,720,300,tag="tachoval")
A=speedometer.Speedometer(screen,"oval",Range=(40,0)) #speedometer
#B=speedometer.Speedometer(screen,"tachoval",Range=(7,0))  #tachometer
#for next week, look at speedometer
#If that is easy, look into open webcam using tkinter for webcam feed

'''
Camera in the center.
speedometer on the left
below speedometer -> Tachometer
Right side - digital number readout
left side - engine rpm. lower left - number. upper left - dial
right side - miles per hour. Lower right - number. Upper right - dial'''

'''
Try to get webcam feed
Or work on tachometer
Only show every other number to make it easier to read
'''

testSpeed=1
#testSpeed = int(input())
#print('speed:' +str(testSpeed))
def getSpeed(increment):
    return int(20+increment)


#def updateLabel(speedInput):      #Input for speed goes here
while(True):
    time.sleep(0.9)
    speedtext.config(text=str(testSpeed))
    rpmtext.config(text=str(random.randint(0,75))+" rpm")
    A.moveto(testSpeed*1.05,"oval")
    #B.moveto(testSpeed*1.05,"tachoval")
    screen.update()
    testSpeed+=1



#updateLabel(44)
#screen.mainloop()
