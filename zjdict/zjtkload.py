import Tkinter as Tk
import time
class Load():
    def __init__(self, top, images=list(), interval=1):
        self._interval = interval
        self._finish = False
        self._parent = top
        self._top = Tk.Toplevel(top)
        self._top.overrideredirect(1)
        ws = top.winfo_screenwidth()
        hs = top.winfo_screenheight()
        top.lift()
        self._top.lift(top)
        self._images = list()
        self._k = 0
        self._canvas_width = 0
        self._canvas_height = 0
        for image in images:
            pi = Tk.PhotoImage(file=image)
            self._canvas_width = max(self._canvas_width, pi.width())
            self._canvas_height = max(self._canvas_height, pi.height())
            self._images.append([pi.width(), pi.height(), image])
        self._canvas = Tk.Canvas(self._top, width=self._canvas_width, height=self._canvas_height)
        self._canvas.pack()
    def finish(self, finish=True):
        self._finish = finish
    def run(self):
        self._parent.lower(self._top)
        ps = self._parent.winfo_geometry()
        ps = ps.replace(str(self._parent.winfo_width()), str(self._canvas_width))
        ps = ps.replace(str(self._parent.winfo_height()), str(self._canvas_height))
        a = list(ps)
        self._top.geometry(ps)
        pi = Tk.PhotoImage(file=self._images[self._k%len(self._images)][2])
        width = self._images[self._k%len(self._images)][0]/2.0
        height = self._images[self._k%len(self._images)][1]/2.0
        self._canvas.create_image(width, height, image=pi)
        self._canvas.update()
        self._k += 1
        if self._finish:
            self._top.quit()
            return
        else:
            self._top.after(self._interval, self.run)
        time.sleep(0.1)

if '__main__' == __name__:
    import zjtkload
    top = Tk.Tk()
    images = ['loading (1).gif', 'loading (2).gif', 'loading (3).gif', 'loading (4).gif', 'loading (5).gif', 'loading (6).gif']
    load = zjtkload.Load(top, images)
    load.run()
    Tk.mainloop()

