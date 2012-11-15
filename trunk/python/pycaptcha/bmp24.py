import cStringIO
#http://davidf.sjsoft.com/files/pyjpeg/BmpFormat.py

def dw2c(word):
    """double word(4 bytes, 32bits)convert to four char"""
    return chr(word & 0xff) + chr((word >> 8) & 0xff) + chr((word >> 16) & 0xff) + chr((word >> 24) & 0xff)

def w2c(word):
    """world(2 bytes, 16bits)convert to two chars"""
    return chr(word & 0xff) + chr((word >> 8) & 0xff)

def c2dw(c):
    """four chars convert to double world(4 bytes, 32bits)"""
    return (ord(c[3])<<24)+(ord(c[2])<<16)+(ord(c[1])<<8)+(ord(c[0]))

class Raster(object):
    x = 0
    y = 1
    def __init__(self):
        self.width = 0
        self.height = 0
        self.data = str()
    def __str__(self):
        return '%s\n%d\n%d\n%d\n' % (repr(self.data), self.width, self.height, len(self.data))
    def initWithWidthAndData(self, width, data):
        self.width = width
        self.data = data
        self.height = size/height
    def initWithWidthAndHeight(self, height, data):
        self.height = height
        self.data = data
        self.width = size/width
    def copy(self, raster):
        self.width = raster.width
        self.height = raster.height
        self.data = raster.data
    def initWithBmp24File(self, filename):
        pass
    def cut(self, left, top, right, bottom):
        data = str()
        for h in range(top, bottom):
            data += self.data[(h*self.width+left)*3:(h*self.width+right)*3]
        raster = Raster()
        raster.width = right - left
        raster.height = bottom - top
        raster.data = data
        return raster

    def combine(self, raster, where = 3):
        """left:1 top:2 right:3 bottom:4"""
        if where in (1, 3):
            if where == 1:
                self, raster = raster, self
            width = self.width + raster.width
            height = max(self.height, raster.height)
            data = str()
            for h in range(height):
                if h < self.height:
                    data += self.data[self.width*3*h:self.width*3*(h+1)]
                else:
                    data += chr(0x00)*3*self.width
                if h < raster.height:
                    data += raster.data[raster.width*3*h:raster.width*3*(h+1)]
                else:
                    data += chr(0x00)*3*raster.width
            self.width, self.height, self.data = width, height, data
        else:
            pass

    def separate(self):
        pass
    def zoom(self, percent):
        pass
    def rotate(self, degree):
        pass

class Bmp24File(object):
    def __init__(self,raster = None):
        self.raster = raster
        self.count_of_row = 0
        if raster:
            self.count_of_row = (self.raster.width*24+31)/32*4

    def read(self, fl):
        f = open(fl, 'rb')
        header = f.read(14)
        data_offset = c2dw(header[10:14])
        info_header = f.read(40)
        self.raster = Raster()
        self.raster.width = c2dw(info_header[4:8])
        self.raster.height = c2dw(info_header[8:12])
        self.raster.data = str()
        self.count_of_row = (self.raster.width*24+31)/32*4
        offset = data_offset
        for y in range(self.raster.height):
            f.seek(offset)
            self.raster.data = f.read(self.raster.width*3) + self.raster.data
            offset += self.count_of_row
        f.close()

    def fileheader(self):
        return "BM" + dw2c(self.filesize()) + dw2c(0) + dw2c(self.dataoffset())
    
    def filesize(self):
        return self.dataoffset() + self.datasize()
    
    def dataoffset(self):
        headerlen = 14
        infoheaderlen = 40
        colortablelen = 0
        return headerlen + infoheaderlen + colortablelen
    
    def datasize(self):
        return self.count_of_row * self.raster.height
    
    def infoheader(self):
        planes = 1
        bitcount = 24
        compression = 0
        xpixelsperm = 1
        ypixelsperm = 1
        colorsused = 0
        colorsimportant = 0 # all
        return dw2c(40) + dw2c(self.raster.width) + dw2c(self.raster.height) + w2c(planes) + w2c(bitcount) + dw2c(compression) + dw2c(self.datasize()) + dw2c(xpixelsperm) + dw2c(ypixelsperm) + dw2c(colorsused) + dw2c(colorsimportant)

    def write(self, fl):
        f = open(fl, 'wb')
        f.write(str(self))
        f.close()
    def __str__(self):
        header_info = self.fileheader() + self.infoheader()
        data = ''
        for y in range(self.raster.height):
            data = self.raster.data[y*self.raster.width*3:(y+1)*self.raster.width*3] + chr(0x00)*(self.count_of_row - 3*self.raster.width) + data
        return header_info + data
        

if __name__ == '__main__':
    letters_idx = 'abcdefghijklmnopqrstuvwxyz0123456789'
    letters = {}
    for y in range(2):
        for x in range(18):
            letters[letters_idx[y*18+x]] = (x, y, x+1, y+1)
    
    bmp = Bmp24File()
    bmp.read('letters.bmp')

    letter_width = bmp.raster.width/18
    letter_height = bmp.raster.height/2
    for k in letters.keys():
        byte = ''
        left = letters[k][0] * letter_width
        top = letters[k][1] * letter_height
        right = letters[k][2] * letter_width
        bottom = letters[k][3] * letter_height
        letters[k] = bmp.raster.cut(left, top, right, bottom)
#    for k in letters.keys():
#        Bmp24File(letters[k]).write(k+'.bmp')
    import random
    captcha_num = 4
    captcha = random.sample(letters_idx, captcha_num)
    raster = Raster()
    for c in captcha:
        raster.combine(letters[c])
#        Bmp24File(letters[c]).write(c+'.bmp')
    Bmp24File(raster).write('captcha.bmp')

