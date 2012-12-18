import sys
import os
def createBigCFile(name, lineno):
    f = open(name, 'wb')
    i = 0
    f.write('#include <stdio.h>\r\n')
    f.write('int main(int argc, char* argv[])\r\n')
    f.write('{\r\n')
    while True:
        if i >= lineno:
            break;
        f.write('\tprintf("%d\\n", __LINE__);\r\n')
        i += 1
    f.write('\treturn 0;\r\n')
    f.write('}\r\n')
    f.close()

if __name__ == '__main__':
    if len(sys.argv) < 1:
        lineno = 2 ** 20
    else:
        lineno = 2 ** sys.argv[1]
    createCFile('t__LINE__MACRO.c', lineno)
