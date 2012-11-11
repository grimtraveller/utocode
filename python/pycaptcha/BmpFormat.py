# done from http://www.daubnet.com/formats/BMP.html

def dw2c(word):
  return chr(word & 0xff) + chr((word >> 8) & 0xff) + chr((word >> 16) & 0xff) + chr((word >> 24) & 0xff)

def w2c(word):
  return chr(word & 0xff) + chr((word >> 8) & 0xff)

class BMPFile:
  def __init__(self, width, height, rgbstr):
    self.data = rgbstr
    self.width = width
    self.height = height

  def __str__(self):
    return self.getheader() + self.getinfoheader() + self.getcolortable() + self.data

  def getheader(self):
    return "BM" + dw2c(self.filesize()) + dw2c(0) + dw2c(self.dataoffset())

  def filesize(self):
    return self.dataoffset() + self.imagesize()

  def dataoffset(self):
    headerlen = 14
    infoheaderlen = 40
    colortablelen = 0
    return headerlen + infoheaderlen + colortablelen

  def imagesize(self):
    """compressed size of image"""
    return len(self.data)

  def getinfoheader(self):
    planes = 1
    bitcount = 24
    compression = 0
    xpixelsperm = 1
    ypixelsperm = 1
    colorsused = 0
    colorsimportant = 0 # all
    return dw2c(40) + dw2c(self.width) + dw2c(self.height) + w2c(planes) + w2c(bitcount) + dw2c(compression) + dw2c(self.imagesize()) + dw2c(xpixelsperm) + dw2c(ypixelsperm) + dw2c(colorsused) + dw2c(colorsimportant)

  def getcolortable(self):
    # blank for 24bit color
    return ""



