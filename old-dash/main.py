#!/usr/bin/env python3
'''Daniel Craig'''
'''I PlEdGe My HoNoR tHaT i HaVe AbIdEd By ThE sTeVeNs HoNoR sYsTeM'''

import tkinter as tk
import time
import random
import speedometer
import tachometer
import serial
import imutils
import cv2
from PIL import Image, ImageTk

'''Tkinter, initialize all variables'''

root=tk.Tk()
root.attributes("-zoomed",True)

#Video capture
video = cv2.VideoCapture(0)
video.set(3, 640/2);
video.set(4, 480/2);
video.set(cv2.CAP_PROP_FPS, 40)
video.set(cv2.CAP_PROP_BUFFERSIZE, 1);

#Screen
screen=tk.Canvas(root)

#Labels
speedtext = tk.Label(text="0")
rpmtext = tk.Label(text='0',fg="black")
fourwheeldrive = tk.Label(text="Off",fg="red")
speedtext.config(font=("Arial",40))
rpmtext.config(font=("Arial",40))
speedtext.place(rely=0.90,relx=0.02,anchor='w')
rpmtext.place(rely=0.90,relx=.98,anchor='e')
fourwheeldrive.config(font=("Arial",35))
fourwheeldrive.place(relx=0.5, rely=0.85, anchor='s')

#Screen final
screen.pack(fill=tk.BOTH, expand=True)
screen.create_oval(30,30,200,200,tag="oval")
screen.create_oval(640-200,30,640-30,200,tag="tachoval")

#Speedometer/tachometer ovals
A=speedometer.Speedometer(screen,"oval",Range=(40,0))
B=tachometer.Tachometer(screen,"tachoval",Range=(5000,0))

#Label for camera spot
lmain = tk.Label(root)
lmain.place(relx=0.5,rely=0.5,anchor='c')

releasedVideo=False
testSpeed=0
tachSpeed=1000
def getSpeed(increment):
    return int(20+increment)

while(True):
    _, frame = video.read()                           #Read Video
    frame = cv2.flip(frame,1)
    cv2image = cv2.cvtColor(frame,cv2.COLOR_BGR2RGBA)
    img = Image.fromarray(cv2image)
    img = img.resize((200,200),Image.LANCZOS)
    imgtk = ImageTk.PhotoImage(image=img)
    lmain.imgtk = imgtk
    lmain.configure(image=imgtk)
    value=20
    randSpeed=int(random.randint(0,41))
    speedtext.config(text=str(randSpeed)+' mph')
    rpmtext.config(text=str(tachSpeed)+" rpm")
    A.moveto(randSpeed,"oval")
    B.moveto(tachSpeed,"tachoval")
    fourwheeldrive.config(text="4WD: On")
    fourwheeldrive.config(fg="green")
    screen.update()

video.release()
screen.mainloop()
