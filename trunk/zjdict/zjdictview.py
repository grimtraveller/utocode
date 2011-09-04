import zjdict
import sys
def input_word(mode):
    source = raw_input('>>>>')
    return translate(source)
def translate(source):
    if 'win32' == sys.platform:
        return mode.translate(source.decode('gbk').encode('utf-8'))
    else:
        return mode.translate(source)
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
    mode = zjdict.zjdictmod()
    if 1 == len(sys.argv):
        while True:
           show_record(input_word(mode))
    else:
        show_record(translate(sys.argv[1]))


