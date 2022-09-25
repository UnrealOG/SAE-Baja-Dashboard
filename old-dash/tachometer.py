from tkinter import *
import math as m
class RadiusError(Exception):
    pass

class Tachometer():
    def changerange(self,Range=(-10,190),rfont=("Verdana",34)):
        for i in range(-10,195,10):
            self.canvas.itemconfig(self.range_marks[int((i+10)/10)],text=str((i+10)*(Range[1]-Range[0])/Range[0]),font=rfont)                        
    def moveto(self,value,tag):
        '''if(value>self.range[1]):
            value=self.range[1]
            self.canvas.itemconfig(self.needle,fill="#000000")
        elif(value<self.range[0]):
            value=self.range[0]
            self.canvas.itemconfig(self.needle,fill="#000000")'''
        #else:
        self.canvas.itemconfig(self.needle,fill=self.needlecolor)
        #Where the needle points
        self.canvas.coords(self.needle,self.radius+self.temp[0],self.radius+self.temp[1],self.radius+self.radius*m.cos((value*self.slope+self.intercept)*m.pi/180)+self.temp[0],self.radius-self.radius*m.sin((value*self.slope+self.intercept)*m.pi/180)+self.temp[1])

        #x: self.radius+self.radius*m.cos((value*self.slope+self.intercept)*m.pi/180)+30
        #y: self.radius-self.radius*m.sin((value*self.slope+self.intercept)*m.pi/180)+30

    #needle        
    def __init__(self,canvas,tag,bg="#222222",needlecolor="#ff0000",markscolor="#000000",Range=(0,30),digitscolor="#000033"):
        self.range=Range
        self.needlecolor=needlecolor
        self.canvas=canvas
        self.tag=tag
        self.temp=self.canvas.coords(tag)
        #if((temp[0]-temp[2])!=(temp[1]-temp[3])):
            #raise RadiusError
        #else:
        self.radius=(self.temp[2]-self.temp[0])/2
        self.slope=200.0/(self.range[1]-self.range[0])
        self.intercept=190-self.range[1]*self.slope     #was 190 before
        self.range_marks=[]
        q=10
        u=0
        #Speed markings
        for i in range(-10,195,20): #-10, 195, 20
            if(i%10==0):
                #Main speed markings
                x1,y1=self.radius+(self.radius-20)*m.cos(i*m.pi/180)+self.temp[0],self.radius-(self.radius-20)*m.sin(i*m.pi/180)+self.temp[1]
                canvas.create_line(x1,y1,self.radius+self.radius*m.cos(i*m.pi/180)+self.temp[0],self.radius-self.radius*m.sin(i*m.pi/180)+self.temp[1],fill=markscolor,width=2) 
                self.range_marks.append(canvas.create_text(x1+u,y1+q, text=int((i+10)*(self.range[1]-self.range[0]-20)/200.8+self.range[0]) ,font=("Courier",int(self.radius/10)),fill=digitscolor))
            else:                                                                                                     #200.6
                #In between markings
                x1,y1=self.radius+(self.radius-10)*m.cos(i*m.pi/180),self.radius-(self.radius-10)*m.sin(i*m.pi/180)
                canvas.create_line(x1,y1,self.radius+self.radius*m.cos(i*m.pi/180)+self.temp[1],self.radius-self.radius*m.sin(i*m.pi/180)+30,fill="#acace6",width=2)
            if(i>=90):
                q=+10
                u=q
            else:
                q=10
                u=0

        #Fucking magic or some shit i dont know
        self.needle=canvas.create_line(self.radius+self.temp[0],self.radius+self.temp[1],self.radius+self.temp[0],self.radius+self.temp[1]+20,fill=needlecolor,width=2)
        canvas.create_oval(self.radius+self.temp[0]-10,self.radius+self.temp[1]-10,self.temp[2]-self.radius+10,self.temp[3]-self.radius+10,fill="#ffff00")
