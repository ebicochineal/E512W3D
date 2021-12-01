#! /usr/bin/env python3
import os
import tkinter

def color565(r, g, b) : return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3)
path = input('png_file_path = ')


root = tkinter.Tk()
img = tkinter.PhotoImage(file=path)
pixels = []
w = img.width()
h = img.height()
for y in range(h):
    for x in range(w):
        r, g, b = img.get(x, y)
        pixels += [color565(r, g, b)]


# (this script dirpath)/file_name.h
file_name = os.path.basename(path).split('.')[0]
p = os.path.dirname(__file__).replace('\\', '/') + '/'
p = '' if p == '/' else p
if w < 1 : exit()
with open(p+file_name+'.h', 'w') as f:
    f.write('#include "E512W3D.h"\n')
    f.write('uint16_t %s_pixels[] = {'%(file_name))
    for i in pixels: f.write(str(i) + ',')
    f.write('};\n')
    f.write('Texture %s_texture(%d, %d, %s_pixels);'%(file_name, w, h, file_name))
    




