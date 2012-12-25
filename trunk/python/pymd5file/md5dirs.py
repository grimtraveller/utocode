import os
import hashlib
def main():
    d = dict()
    for dirpath, dirnames, filenames in os.walk('e:\\'):
        for i in xrange(len(filenames)):
            full_name = os.path.join(dirpath, filenames[i])
            print full_name
            try:
                c = open(full_name).read()
            except:
                continue
            m = hashlib.md5()
            m.update(c)
            key = str(m.hexdigest())
            if d.has_key(key) == False:
                d[str(m.hexdigest())] = list()
            d[str(m.hexdigest())].append(full_name)
    for key in d.keys():
        if len(d[key]) > 1:
            print d[key]

if __name__ == '__main__':
    main()
