#include "M5StickC.h"
#include "E512W3D.h"

#include "body.h"
#include "eye.h"
E512WindowManager wm(160, 80);
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(0, 0, 0));
Object3D o;
Object3D a, b, t1,t2;
Object3D camera;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    
    bodyInit();
    eyeInit();
    
    a.mesh = &body;
    a.render_type = RenderType::PolygonNormal;
    o.addChild(a);
    
    b.mesh = &eye;
    b.render_type = RenderType::PolygonNormal;
    o.addChild(b);
    
    camera.position.z = 10;
    camera.position.y = 0;
    w.setCamera(camera);
    w.setDirectionalLight(-1, -1, -1);
    w.ambient = 0.4;
    w.addChild(o);
    
    o.rotation.y = 270;
    
    wm.add(w);
    
    M5.MPU6886.Init();
}

void loop() {
    o.rotation.y += 5;
    wm.fixedDraw();
}

int main () {
    setup();
    // while (1) {
    for (int i = 0; i < 100; ++i) {
        loop();
        delay(100);
    }
    return 0;
}