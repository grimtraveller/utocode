import os
import hashlib
import sys
def main():
    c = open(sys.argv[1]).read()
    m = hashlib.md5()
    m.update(c)
    print m.hexdigest()
if __name__ == '__main__':
    main()

