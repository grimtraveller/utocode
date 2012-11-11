import BmpFormat
import random
letters_idx = 'abcdefghijklmnopqrstuvwxyz0123456789'
letters = {}
for y in range(2):
    for x in range(18):
        letters[letters_idx[y*18+x]] = (x, y, x+1, y+1)

def c2dw(c):
    return (ord(c[3])<<24)+(ord(c[2])<<16)+(ord(c[1])<<8)+(ord(c[0]))
src = open('letters.bmp', 'rb')
header = src.read(14)
data_offset = c2dw(header[10:14])
info_header = src.read(40)
src.seek(data_offset)
raster_data = src.read()
src_width = c2dw(info_header[4:8])
src_height = c2dw(info_header[8:12])
src_rgb_per_row = (src_width*24+31)/32*4
letter_width = src_width/18
letter_height = src_height/2
letter_rgb_per_row = (letter_width*24+31)/32*4
for k in letters.keys():
    byte = ''
    left = letters[k][0] * letter_width
    top = src_height - letters[k][1] * letter_height
    right = letters[k][2] * letter_width
    bottom = src_height - letters[k][3] * letter_height
    top, bottom = bottom, top
    width = right - left
    height = bottom - top
    rgb_per_row = (width*24+31)/32*4
    for h in range(top, bottom):
        begin = h * src_rgb_per_row + left * 3
        end = begin + 3 * width 
        row_data = raster_data[begin:end]+chr(0x00)*(rgb_per_row - 3*width)
        byte += row_data
    bmp = BmpFormat.BMPFile(width, height, byte)
    o = open(k+'.bmp', 'wb')
    o.write(str(bmp))
    o.close()

captcha_num = 4
captcha = random.sample(letters_idx, captcha_num)
captcha_bin = []
for c in captcha:
    byte = ''
    left = letters[c][0] * letter_width
    top = src_height - letters[c][1] * letter_height
    right = letters[c][2] * letter_width
    bottom = src_height - letters[c][3] * letter_height
    top, bottom = bottom, top
    width = right - left
    height = bottom - top
    rgb_per_row = (width*24+31)/32*4
    for h in range(top, bottom):
        begin = h * src_rgb_per_row + left * 3
        end = begin + 3 * width 
        row_data = raster_data[begin:end]
        byte += row_data
    bmp = BmpFormat.BMPFile(letter_width, letter_height, byte)
    captcha_bin.append(byte)

captcha_width = letter_width * captcha_num
captcha_height = letter_height
captcha_rgb_per_row = (captcha_width*24+31)/32*4
byte = ''
for h in range(letter_height):
    for c in captcha_bin:
        byte += c[h*letter_width*3:(h+1)*letter_width*3]
    byte += chr(0x00)*(captcha_rgb_per_row - 3*captcha_width)
bmp = BmpFormat.BMPFile(captcha_width, captcha_height, byte)
o = open(str(captcha)+'.bmp', 'wb')
o.write(str(bmp))
o.close()
