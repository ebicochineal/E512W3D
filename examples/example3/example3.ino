#include "E512W3D.hpp"
#include "zou.hpp"

E512W3DWindow w0, w1, w2, w3;

Object3D a0, a;
Object3D o, c0, c1, c2, c3;

void setup () {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    M5.MPU6886.Init();
    
    e512w3d.width = 160;
    e512w3d.height = 80;
    w0.sx = e512w3d.width/2;
    w0.sy = 0;
    w0.width = e512w3d.width/2;
    w0.height = e512w3d.height/2;
    
    w1.sx = 0;
    w1.sy = e512w3d.height/2;
    w1.width = e512w3d.width/2;
    w1.height = e512w3d.height/2;
    
    w2.sx = e512w3d.width/2;
    w2.sy = e512w3d.height/2;
    w2.width = e512w3d.width/2;
    w2.height = e512w3d.height/2;
    
    w3.sx = 0;
    w3.sy = 0;
    w3.width = e512w3d.width/2;
    w3.height = e512w3d.height/2;
    
    
    zouInit();
    
    a.mesh = &zou;
    a.position.y = -2;
    a.position.z = -1;
    a.render_type = RenderType::WireFrame;
    a.color = color565(255, 255, 255);
    a0.mesh = &zou;
    a0.position.y = -2;
    a0.position.z = -1;
    a0.render_type = RenderType::PolygonNormal;
    
    w0.addChild(a0);
    w1.addChild(a);
    w2.addChild(a);
    w3.addChild(a);
    
    c0.position.z = 8;
    c0.position.y = 3;
    c0.rotation *= Quaternion::angleAxis(-20, Vector3(1, 0, 0));
    
    o.addChild(c0);
    // w0.setDirectionalLight(0, -1, -1);
    w0.setCamera(c0);
    
    c1.position.z = 8;
    w1.ortho_size = 0.05;
    w1.setCamera(c1);
    w1.isortho = true;
    
    c2.position.x = 8;
    c2.rotation *= Quaternion::angleAxis(90, Vector3(0, 1, 0));
    w2.ortho_size = 0.05;
    w2.setCamera(c2);
    w2.isortho = true;
    
    c3.position.y = 8;
    c3.rotation *= Quaternion::angleAxis(-90, Vector3(1, 0, 0));
    w3.ortho_size = 0.05;
    w3.setCamera(c3);
    w3.isortho = true;
    
    // 3 0
    // 1 2
    
    e512w3d.add(w0);
    e512w3d.add(w1);
    e512w3d.add(w2);
    e512w3d.add(w3);
    
    e512w3d.begin();
}

void loop () {
    if (e512w3d.isFixedTime()) {
        o.rotation *= Quaternion::angleAxis(1, Vector3(0, 1, 0));
        e512w3d.draw();
    }
}