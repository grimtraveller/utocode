#!/usr/bin/python
# -*- coding: utf-8 -*-
from Tkinter import *
import zjdict as zjdict
import re
import sys
import threading
import zjtkload as zjtkload
images = ['loading (1).gif', 'loading (2).gif', 'loading (3).gif', 'loading (4).gif', 'loading (5).gif', 'loading (6).gif']

class TkCtrl(object):
    def __init__(self):
        self.view = TkView()
        self.mod = zjdict.zjdictmod()
        self.view.loading(self.mod.appendDicts)
    def mainloop(self):
        mainloop()
    def tip(self, event=None):
        word = self.view.word.get()
#        key_pattern = r'('+word+'.*)'
#        matched_keys = re.findall(key_pattern, keys) 
#        self.view.addtips(matched_keys)


    

class TkView(object):
    def __init__(self):
        self._top = Tk()
        bar = Menu(self._top)
        self._top.configure(menu=bar)
        main_menu = Menu(bar)
        bar.add_cascade(label='Menu',menu=main_menu)
        main_menu.add_command(label='Save', command=self.save)
        main_menu.add_command(label='Search', command=self.search)
        main_menu.add_command(label='Quit', command=self._top.quit)

        self._key = StringVar()
        self._value = StringVar()
        frame = Frame(self._top)
        frame.pack(fill='both')
        edit = Entry(frame, textvariable=self._key)
        edit.pack(side='left', fill='x', expand=1)
        translate_button = Button(frame, text='translate', command=self.translate)
        translate_button.pack(side='left')
        save_button = Button(frame, text='save', command=self.save)
        save_button.pack(side='right')
        vscrollbar = Scrollbar(self._top)
        vscrollbar.pack(side=RIGHT, pady=15, fill=Y)
#        font_style = "ËÎÌו"
#        self._valuelist = Listbox(self._top,yscrollcommand=vscrollbar.set, font=(font_style, 9))
#        self._valuelist = Listbox(self._top,yscrollcommand=vscrollbar.set)
#        self._valuelist.pack(fill='both', pady=15, expand=1)
#        vscrollbar.config(command=self._valuelist.yview)
        self._valuetext = Text(self._top, yscrollcommand=vscrollbar.set)
        self._valuetext.pack(fill='both', pady=15, expand=1)
        vscrollbar.config(command=self._valuetext.yview)
        self._top.geometry('500x255+150+150')
    def loadThread(self, mod):
        self._mod = mod
        mod.appendDicts()
        self._load.finish()

    def loading(self, mod):
        self._load = zjtkload.Load(self._top, images)
        t = threading.Thread(target=self.loadThread, args=(mod,))
        t.start()
        self._load.run()
    def translate(self, event=None):
        value = self._mod.translate(self._key.get())
        self._valuetext.config(state=NORMAL)
        self._valuetext.delete(1.0, END)
        self._valuetext.insert(END, value)
        self._valuetext.config(state=DISABLED)
#        lines = value.split('\x0a')
#        self._valuelist.delete(0, END)
#        for line in lines:
#            self._valuelist.insert(END, line)
    def searching(self):
        self._load = zjtkload.Load(self._top, images)
        t = threading.Thread(target=self.searchThread)
        t.start()
        self._load.run()
    def search(self):
        self.searching()
    def searchThread(self):
        print self._mod.translate(self._key.get())
        values = self._mod.search(self._key.get())
        self._valuelist.delete(0, END)
        for value in values:
            self._valuelist.insert(END, value)
        self._load.finish()
    def save(self):
        self._mod.save('newword.txt')
if '__main__' == __name__:
    view = TkView()
    mod = zjdict.zjdictmod()
    view.loading(mod)
    mainloop()

#    tkctrl = TkCtrl()
#    tkctrl.mainloop()
