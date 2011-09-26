import Tkinter as Tk
import zjdict as zjdict
import re
import sys
import threading
import zjtkload as zjtkload
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

        self._word = Tk.StringVar(self._top)
        frame = Tk.Frame(self._top)
        frame.pack(fill='both')
        edit = Tk.Entry(frame, textvariable=self._word)
        edit.pack(side='left', fill='x', expand=1)
        edit.bind('<KeyRelease>',self.showTip)
        if 'win32' == sys.platform:
            clean_button = Tk.Button(frame, text='clear', command=self.clean)
        else:
            clean_button = Tk.Button(frame, text='clear', command=self.clean)
        clean_button.pack()
        self.tiplist = Tk.Listbox(self._top,borderwidth=0)
        self.tiplist.pack(fill='both', pady=15, expand=1)
        self._top.geometry('400x255+150+150')
    def loading(self, fun):
        t = threading.Thread(target=loadThread, args=(self,))
        t.start()
        zjtkload.Load.run()
    def loadThread(loading=None):
        if loading is not None:
            laoding.run()
    def clean(self, event=None):
        self.word.set('')
    def addTips(self, matched_keys):
        self.tiplist.delete(0, Tk.END)
        for key in matched_keys:
            self.tiplist.insert(Tk.END, key)
    def showTip(self):
        pass

if '__main__' == __name__:
    view = TkView()
    mod = zjdict.zjdictmod()
    view.loading(self.mod.appendDicts)
    Tk.mainloop()

#    tkctrl = TkCtrl()
#    tkctrl.mainloop()
