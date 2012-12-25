import sys
import os
import hashlib
def walk(path):
    fl = os.listdir(path)
    for f in fl:
        if os.path.isdir(os.path.join(path,f)):
            walk(os.path.join(path,f))
        else:
            c = open(os.path.join(path,f)).read()
            md5file = open(os.path.join(path,f) + ".md5", "w")
            m = hashlib.md5()
            m.update(c)
	    md5s = "%s:%s\n" % (os.path.join(path,f),str(m.hexdigest()))
	    md5file.write(md5s)
	    md5.close()
if __name__ == "__main__": 
	walk(sys.argv[1])
