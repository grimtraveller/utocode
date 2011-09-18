import Tkinter as Tk
import zjdict as zjdict
import re
import sys
import threading
import zjtkload as zjtkload
class TkCtrl(object):
    def __init__(self):
        self.view = TkView(self.tip)
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
    def __init__(self, tip):
        self.tip = tip
        self.top = Tk.Tk()
        bar = Tk.Menu(self.top)
        self.top.configure(menu=bar)
        main_menu = Tk.Menu(bar)
        bar.add_cascade(label='Menu',menu=main_menu)
        main_menu.add_command(label='Save')
        main_menu.add_command(label='Quit', command=self.top.quit)

        self.tiplist = Tk.Listbox(self.top,borderwidth=0)
        self.tiplist.pack(fill='both', padx=5, pady=5, expand=1)

        self.word = Tk.StringVar(self.top)
        edit = Tk.Entry(self.top, textvariable=self.word)
        edit.pack(side='left', fill='x', expand=1, padx=5)
        edit.bind('<KeyRelease>',self.tip)
        if 'win32' == sys.platform:
            clean_button = Tk.Button(self.top, text='clear', command=self.clean)
        else:
            clean_button = Tk.Button(self.top, text='clear',pady=15, command=self.clean)
        clean_button.pack()
        self.top.geometry('400x255+150+150')
    def loading(self, fun):
        fun()
        pass
#        t = threading.Thread(target=loadThread, args=(self,))
#        t.start()
#        zjtkload.Load.run()
    def loadThread(loading=None):
        if loading is not None:
            laoding.run()
    def clean(self, event=None):
        self.word.set('')
    def addTips(self, matched_keys):
        self.tiplist.delete(0, Tk.END)
        for key in matched_keys:
            self.tiplist.insert(Tk.END, key)

if '__main__' == __name__:
    tkctrl = TkCtrl()
    tkctrl.mainloop()
