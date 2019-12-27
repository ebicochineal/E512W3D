#! /usr/bin/env python3
import os

path = input('obj_file_path = ')

vertex = []
face = []
with open(path) as f:
    toint = lambda x : int(x.split('/')[0])
    for i in f.readlines():
        if str(i).startswith('v '):
            _, x, y, z = i.split(' ')
            vertex += [[float(x), float(y), float(z)]]
        if i.startswith('f '):
            *l, = i.split(' ')
            if len(l) == 4:
                _, a, b, c = l
                face += [[toint(a)-1, toint(b)-1, toint(b)-1]]
            else:
                _, a, b, c, d = l
                face += [[toint(a)-1, toint(b)-1, toint(d)-1]]
                face += [[toint(b)-1, toint(c)-1, toint(d)-1]]

# (this script dirpath)/obj_file_name.h
obj_file_name = os.path.basename(path).split('.')[0]
p = os.path.dirname(__file__).replace('\\', '/') + '/'
p = '' if p == '/' else p
with open(p+obj_file_name+'.h', 'w') as f:
    
    f.write('#include "E512W3D.h"\nMesh %s;\nvoid %sInit () {\n'%(obj_file_name, obj_file_name))
    for i in vertex:
        f.write('    %s.addVertex('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    for i in face:
        f.write('    %s.addFace('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    f.write('}\n')









