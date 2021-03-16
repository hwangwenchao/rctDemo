# _*_ coding: utf-8 _*_

"""
from PIL import Image
import argparse

parser = argparse.ArgumentParser()

# input arguments
parser.add_argument("file")
parser.add_argument("-o", "--outfile")
parser.add_argument("--width", type = int, default = 80)
parser.add_argument("--height", type = int, default = 80)

# get argments
args = parser.parse_args()

IMG = args.file
WIDTH = args.width
HEIGHT = args.height
OUTPUT = args.outfile


ascii_char = list("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ")

def get_char(r, g, b, alpha = 256):
    if alpha == 0:
        return ' '
    
    length = len(ascii_char)
    
    gray = int(0.2126*r + 0.7152*g + 0.0722*b)
    
    uint = (256.0 + 1) / length
    
    return ascii_char[int(gray/uint)]
    
if __name__ == '__main__':
    
    im = Image.open(IMG)
    
    im = im.resize((WIDTH, HEIGHT), Image.NEAREST)
    
    txt = ""
    
    for i in range(HEIGHT):
        for j in range(WIDTH):
            txt += get_char(*im.getpixel((j, i)))
        txt += '\n'
    
    print(txt)
    
    if OUTPUT:
        with open(OUTPUT, "w") as f:
            f.write(txt)
    else:
        with open("outfile.txt", "w") as f:
            f.write(txt)
