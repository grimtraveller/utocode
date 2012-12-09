#!/usr/bin/python
# -*- coding: utf-8 -*-
from Tkinter import *
import tkFileDialog
class TkView(object):
    def __init__(self):
        self._top = Tk()
        self._filename = StringVar()
        self._value = StringVar()
        frame = Frame(self._top)
        frame.pack(fill='both')
        edit = Entry(frame, textvariable=self._filename)
        edit.pack(side='left', fill='x', expand=1)
        translate_button = Button(frame, text='select', command=self.open)
        translate_button.pack(side='left')
        vscrollbar = Scrollbar(self._top)
        vscrollbar.pack(side=RIGHT, pady=15, fill=Y)
        self._valuetext = Text(self._top, yscrollcommand=vscrollbar.set)
        self._valuetext.pack(fill='both', pady=15, expand=1)
        vscrollbar.config(command=self._valuetext.yview)
        self._top.geometry('500x255+150+150')

    def open(self, event=None):
        self._filename = tkFileDialog.askopenfilename(title = 'Open Excel File', 
                filetypes=[('Excel', '*.xls'), ('All files', '*')])
        print self._filename

    def searching(self):
        pass
    def search(self):
        pass
    def save(self):
        pass
if '__main__' == __name__:
    view = TkView()
    mainloop()

#    tkctrl = TkCtrl()
#    tkctrl.mainloop()
