import zjdict
import sys
import threading
import time
import getch
class CUIView:
	pass

def loadDicts(mod):
    mod.appendDicts()
def loading(target, args):
    interval = 0.1
    t = threading.Thread(target=target, args=args)
    t.start()
    while True:
        sys.stdout.write('\b-')
        sys.stdout.flush()
        time.sleep(interval)
        sys.stdout.write('\b\\')
        sys.stdout.flush()
        time.sleep(interval)
        sys.stdout.write('\b|')
        sys.stdout.flush()
        time.sleep(interval)
        sys.stdout.write('\b/')
        sys.stdout.flush()
        if t.isAlive():
            t.join(interval)
        else:
            break;
    return 
def translate(source):
    if 'win32' == sys.platform:
        return mod.translate(source.decode('gbk').encode('utf-8'))
    else:
        return mod.translate(source)
def show_record(destination):
    if 'win32' == sys.platform:
        try:
            print(destination.decode('utf-8').encode('gbk'))
        except:
            #todo: replate some letter in symbol that can't show in gbk code
            for s in destination.split('\n'):
                try:
                    print(s.decode('utf-8').encode('gbk'))
                except:
                    continue

    else:
        print(destination)

if '__main__' == __name__:
    print('')
    fname = 'newword.txt'
    mod = zjdict.zjdictmod()
    loading(loadDicts, (mod,))
    if 1 == len(sys.argv):
        while True:
            source = raw_input('\b>>>>')
            if 0 == len(source):
				continue
            if ',' == source[0]:
                if ',quit' == source or ',exit' == source:
                    break
                elif ',save' == source:
                    mod.save(fname)
                elif ',search' == source:
                    grep = raw_input('?>')
                    tips = mod.search(grep)
                    count = 0
                    for tip in tips:
                        if 'win32' == sys.platform:
                            print(tip.decode('utf-8').encode('gbk'))
                        else:
                            print(tip)
                        count += 1
                        if count > 10:
                            k = getch.getch()
                            if '\x1b' == k:
                                if 0 == len(raw_input('ESC')):
                                    break
                            count = 0
            else:
                destination = translate(source)
                show_record(destination)
    else:
        show_record(translate(sys.argv[1]))
