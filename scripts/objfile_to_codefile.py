#! /usr/bin/env python3
import os

path = input('obj_file_path = ')

vertex = []
face = []
uvvertex = []
uvface = []

def addface(a, b, c, face, uvface):
    *a, = map(int, a.split('/'))
    *b, = map(int, b.split('/'))
    *c, = map(int, c.split('/'))
    if len(a) == 1:
        face += [[a[0]-1, b[0]-1, c[0]-1]]
        
        
    if len(a) == 2:
        face += [[a[0]-1, b[0]-1, c[0]-1]]
        uvface += [[a[1]-1, b[1]-1, c[1]-1]]
    
    

with open(path) as f:
    toint = lambda x : int(x.split('/')[0])
    for i in f.readlines():
        if str(i).startswith('v '):
            _, x, y, z = i.split(' ')
            vertex += [[float(x), float(y), float(z)]]
        if str(i).startswith('vt '):
            _, x, y = i.split(' ')
            uvvertex += [[float(x), float(y)]]
        if i.startswith('f '):
            *l, = i.split(' ')
            if len(l) == 4:
                _, a, b, c = l
                addface(a,b,c, face, uvface)
                
            else:
                _, a, b, c, d = l
                addface(a,b,d, face, uvface)
                addface(b,c,d, face, uvface)
                face += [[toint(a)-1, toint(b)-1, toint(d)-1]]
                face += [[toint(b)-1, toint(c)-1, toint(d)-1]]

# (this script dirpath)/obj_file_name.hpp
obj_file_name = os.path.basename(path).split('.')[0]
p = os.path.dirname(__file__).replace('\\', '/') + '/'
p = '' if p == '/' else p
with open(p+obj_file_name+'.hpp', 'w') as f:
    f.write('#pragma once\n')
    f.write('#include "E512W3D.hpp"\nMesh %s;\nvoid %sInit () {\n'%(obj_file_name, obj_file_name))
    
    f.write('    // %s.vertexs '%obj_file_name + str(len(vertex)) + '\n')
    f.write('    // %s.faces '%obj_file_name + str(len(face)) + '\n')
    f.write('    // %s.uv_vertexs '%obj_file_name + str(len(uvvertex)) + '\n')
    f.write('    // %s.uv_faces '%obj_file_name + str(len(uvface)) + '\n')
    
    f.write('#if !defined(ARDUINO)\n')
    
    f.write('    {\n')
    f.write('        %s.vertexs.reserve('%obj_file_name + str(len(vertex)) + ');\n')
    t = '        float va[] = {'
    for x, y, z in vertex:
        t += str(x) + ',' + str(y) + ',' + str(z) + ','
    t += '};\n'
    f.write(t)
    f.write('        for (int i = 0; i < %d; i += 3) {\n'%(len(vertex)*3))
    f.write('            %s.addVertex(va[i], va[i+1], va[i+2]);\n'%obj_file_name)
    f.write('        }\n')
    f.write('    }\n')
    
    f.write('    {\n')
    f.write('        %s.faces.reserve('%obj_file_name + str(len(face)) + ');\n')
    t = '        int fa[] = {'
    for a, b, c in face:
        t += str(a) + ',' + str(b) + ',' + str(c) + ','
    t += '};\n'
    f.write(t)
    f.write('        for (int i = 0; i < %d; i += 3) {\n'%(len(face)*3))
    f.write('            %s.addFace(fa[i], fa[i+1], fa[i+2]);\n'%obj_file_name)
    f.write('        }\n')
    f.write('    }\n')
    
    if len(uvface) > 0:
        f.write('    {\n')
        f.write('        %s.uv_vertexs.reserve('%obj_file_name + str(len(uvvertex)) + ');\n')
        t = '        float uva[] = {'
        for x, y in uvvertex:
            t += str(x) + ',' + str(y) + ','
        t += '};\n'
        f.write(t)
        f.write('        for (int i = 0; i < %d; i += 2) {\n'%(len(uvvertex)*2))
        f.write('            %s.addVertexUV(uva[i], uva[i+1]);\n'%obj_file_name)
        f.write('        }\n')
        f.write('    }\n')
        
        
        f.write('    {\n')
        f.write('        %s.uv_faces.reserve('%obj_file_name + str(len(uvface)) + ');\n')
        t = '        int ufa[] = {'
        for a, b, c in uvface:
            t += str(a) + ',' + str(b) + ',' + str(c) + ','
        t += '};\n'
        f.write(t)
        f.write('        for (int i = 0; i < %d; i += 3) {\n'%(len(uvface)*3))
        f.write('            %s.addFaceUV(ufa[i], ufa[i+1], ufa[i+2]);\n'%obj_file_name)
        f.write('        }\n')
        f.write('    }\n')
    
    f.write('#else\n')
    
    
    # f.write('    %s.vertexs.reserve('%obj_file_name + str(len(vertex)) + ');\n')
    # f.write('    %s.faces.reserve('%obj_file_name + str(len(face)) + ');\n')
    # for i in vertex:
    #     f.write('    %s.addVertex('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    # for i in face:
    #     f.write('    %s.addFace('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    # if len(uvface) > 0:
    #     f.write('    %s.uv_vertexs.reserve('%obj_file_name + str(len(uvvertex)) + ');\n')
    #     f.write('    %s.uv_faces.reserve('%obj_file_name + str(len(uvface)) + ');\n')
    #     for i in uvvertex:
    #         f.write('    %s.addVertexUV('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    #     for i in uvface:
    #         f.write('    %s.addFaceUV('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    
    f.write('    %s.vertexs.reserve('%obj_file_name + str(len(vertex)) + ');\n')
    f.write('    %s.faces.reserve('%obj_file_name + str(len(face)) + ');\n')
    for i in vertex:
        f.write('    %s.vertexs.emplace_back('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    for i in face:
        f.write('    %s.faces.emplace_back('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    if len(uvface) > 0:
        f.write('    %s.uv_vertexs.reserve('%obj_file_name + str(len(uvvertex)) + ');\n')
        f.write('    %s.uv_faces.reserve('%obj_file_name + str(len(uvface)) + ');\n')
        for i in uvvertex:
            f.write('    %s.uv_vertexs.emplace_back('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
        for i in uvface:
            f.write('    %s.uv_faces.emplace_back('%obj_file_name + ', '.join(str(x) for x in i) + ');\n')
    
    
    f.write('#endif\n')
    f.write('}\n')
    








