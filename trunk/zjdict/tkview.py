# -*- coding: utf-8 -*-
import Tkinter as Tk
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
        Tk.mainloop()
    def tip(self, event=None):
        word = self.view.word.get()
#        key_pattern = r'('+word+'.*)'
#        matched_keys = re.findall(key_pattern, keys) 
#        self.view.addtips(matched_keys)


    

class TkView(object):
    def __init__(self):
        self._top = Tk.Tk()
        bar = Tk.Menu(self._top)
        self._top.configure(menu=bar)
        main_menu = Tk.Menu(bar)
        bar.add_cascade(label='Menu',menu=main_menu)
        main_menu.add_command(label='Save')
        main_menu.add_command(label='Quit', command=self._top.quit)

        self._key = Tk.StringVar()
        frame = Tk.Frame(self._top)
        frame.pack(fill='both')
        edit = Tk.Entry(frame, textvariable=self._key)
        edit.pack(side='left', fill='x', expand=1)
        translate_button = Tk.Button(frame, text='translate', command=self.translate)
        translate_button.pack(side='left')
        search_button = Tk.Button(frame, text='search', command=self.search)
        search_button.pack(side='right')
        vscrollbar = Tk.Scrollbar(self._top)
        vscrollbar.pack(side=Tk.RIGHT, pady=15, fill=Tk.Y)
        
        font_style = "ËÎÌו"
        self._valuelist = Tk.Listbox(self._top,yscrollcommand=vscrollbar.set, font=(font_style, 9))
        self._valuelist.pack(fill='both', pady=15, expand=1)
        vscrollbar.config(command=self._valuelist.yview)
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
        lines = value.split('\x0a')
        self._valuelist.delete(0, Tk.END)
        for line in lines:
            self._valuelist.insert(Tk.END, line)
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
        self._valuelist.delete(0, Tk.END)
        for value in values:
            self._valuelist.insert(Tk.END, value)
        self._load.finish()
if '__main__' == __name__:
    view = TkView()
    mod = zjdict.zjdictmod()
    view.loading(mod)
    Tk.mainloop()

#    tkctrl = TkCtrl()
#    tkctrl.mainloop()
