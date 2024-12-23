#! /usr/bin/env python3
import os
import tkinter

def color565(r, g, b) : return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3)
def color1555(abit, r, g, b) : return (abit << 15) | ((r>>3)<<10) | ((g>>3)<<5) | (b>>3)

op = input('Texture ReadOnly y/n = ')
path = input('png_file_path = ')

root = tkinter.Tk()
img = tkinter.PhotoImage(file=path)
pixels = []
w = img.width()
h = img.height()

for y in range(h):
    for x in range(w):
        r, g, b = img.get(x, y)
        # pixels += [color565(r, g, b)]
        abit = img.transparency_get(x, y)
        pixels += [color1555(abit, r, g, b)]


# (this script dirpath)/file_name.hpp
file_name = os.path.basename(path).split('.')[0]
p = os.path.dirname(__file__).replace('\\', '/') + '/'
p = '' if p == '/' else p
if w < 1 : exit()
with open(p+file_name+'.hpp', 'w') as f:
    f.write('#pragma once\n')
    f.write('#include "E512W3D.hpp"\n')
    f.write(['', 'const ']['y' in op.lower()] + 'uint16_t %s_pixels[] = {'%(file_name))
    for i in pixels: f.write(str(i) + ',')
    f.write('};\n')
    f.write(['', 'C']['y' in op.lower()] + 'Texture %s_texture(%d, %d, %s_pixels);'%(file_name, w, h, file_name))
    




