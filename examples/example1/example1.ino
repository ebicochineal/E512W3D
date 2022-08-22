#include "E512W3D.hpp"
#include "cube.hpp"

E512W3DWindow w(color565(20, 20, 20));

Object3D a, b, c, d, e, f;
Object3D camera;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    M5.MPU6886.Init();
    
    e512w3d.width = 160;
    e512w3d.height = 80;
    w.width = e512w3d.width;
    w.height = e512w3d.height;
    
    cubeInit();
    
    a.mesh = &cube;
    a.render_type = RenderType::PolygonNormal;
    a.addChild(b);
    a.addChild(c);
    a.addChild(d);
    a.addChild(e);
    
    b.mesh = &cube;
    b.position.x = 12;
    b.rotation = Quaternion::angleAxis(180 , Vector3(0, 1, 0));
    b.render_type = RenderType::PolygonColor;
    b.color = color565(255, 0, 0);
    
    c.mesh = &cube;
    c.position.x = -12;
    c.render_type = RenderType::PolygonColor;
    c.color = color565(0, 255, 0);
    
    d.mesh = &cube;
    d.position.z = 12;
    d.rotation = Quaternion::angleAxis(90 , Vector3(0, 1, 0));
    d.render_type = RenderType::PolygonColor;
    d.color = color565(0, 0, 255);
    
    e.mesh = &cube;
    e.position.z = -12;
    e.rotation = Quaternion::angleAxis(270 , Vector3(0, 1, 0));
    e.render_type = RenderType::WireFrame;
    e.color = color565(255, 255, 255);
    
    w.addChild(a);
    camera.position.z = 32;
    camera.position.y = 16;
    camera.rotation = Quaternion::angleAxis(-30 , Vector3(1, 0, 0));
    w.setCamera(camera);
    w.setDirectionalLight(-1, -1, -1);
    e512w3d.add(w);
    
    // w.ambient = 1;
    
    e512w3d.begin();
}

void loop() {
    static float v = 0;
    if (e512w3d.isFixedTime()) {
        a.rotation *= Quaternion::angleAxis(6 , Vector3(0, 1, 0));
        b.position.y = abs(sin(v)) * 3;
        c.position.y = abs(sin(v+0.25f)) * 3;
        d.position.y = abs(sin(v+0.5f)) * 3;
        e.position.y = abs(sin(v+1.0f)) * 3;
        v += 0.5f;
        e512w3d.draw();
    }
}