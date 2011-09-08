#/usr/bin/python
#coding=utf-8
import os
import pystardict
import sys
class zjdictmod():
    def __init__(self):
        self._key = ''
        self._values = ''
        self._dicts_dir = os.path.join(os.path.dirname(__file__))
        self._dicts_dir += os.sep + 'dictionary' + os.sep
        self._dicts = list()
        self._dicts.append(\
                pystardict.Dictionary(os.path.join(self._dicts_dir, \
                    'stardict-cedict-gb-2.4.2', \
                    'cedict-gb')))
        self._dicts.append(
                pystardict.Dictionary(os.path.join(self._dicts_dir, \
                        'stardict-kdic-computer-gb-2.4.2', \
                        'kdic-computer-gb')))
        if 'linux2' != sys.platform:
            self._dicts.append(
                    pystardict.Dictionary(os.path.join(self._dicts_dir, \
                            'stardict-langdao-ce-gb-2.4.2', \
                            'langdao-ce-gb')))
            self._dicts.append(\
                    pystardict.Dictionary(os.path.join(self._dicts_dir, \
                            'stardict-langdao-ec-gb-2.4.2', \
                            'langdao-ec-gb')))
    def lsdict(self):
        pass
    def words(self):
        pass
    def translate(self,key):
        self._key, self._values = key, ''
        for d in self._dicts:
            if d.has_key(key):
                value = d.dict[key]
                self._values += value + '\n'
        return self._values
    def save(self, fname):
        print(self._key, self._values)
        if ('' != self._key) and ('' != self._values):
            print(fname)
            f = open(fname, 'a+')
            f.write(self._key)
            f.write(self._values)
            f.close()
if '__main__' == __name__:
    pass
