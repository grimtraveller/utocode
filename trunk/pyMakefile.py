import os
import sys

def main():
    for dirpath, dirnames, filenames in os.walk('.'):
        for f in filenames:
            full_name = os.path.join(dirpath, f)
            name, ext = os.path.splitext(full_name)
            path = os.path.split(full_name)[0]
            if '.c' == ext and '.' == path:
                if 1 == len(sys.argv):
                    cmd = 'gcc ' + full_name + ' -D"MACOSX" -ggdb -o ' + name
                    print cmd
                    os.system(cmd)
                else:
                    if 'clean' == sys.argv[1]:
                        try:
                            os.remove(name)
                        except:
                            pass
                        try:
                            os.rmdir(name+'.dSYM')
                        except:
                            pass
                        cmd = 'rm -rf ' + name + '.dSYM'
                        print cmd
                        os.system(cmd)
                    else:
                        cmd = 'gcc ' + full_name + ' -D"MACOSX" -ggdb -o ' + name
                        print cmd
                        os.system(cmd)


if __name__ == '__main__':
    main()
