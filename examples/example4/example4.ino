#include <M5StickC.h>
#include "E512W3D.h"
#include "body.h"
#include "eye.h"
E512WindowManager wm(160, 80);
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(20, 20, 20));
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
    // a.render_type = RenderType::PolygonNormal;
    a.render_type = RenderType::Polygon;
    a.color = M5.Lcd.color565(200, 44, 85);
    o.addChild(a);
    
    b.mesh = &eye;
    // b.render_type = RenderType::PolygonNormal;
    b.render_type = RenderType::Polygon;
    b.color = M5.Lcd.color565(0, 0, 0);
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
    // battery
    int16_t batv = (int16_t)(M5.Axp.GetVapsData() * 1.4f);
    batv = max(min(batv, 4100), 3300);
    int16_t bati = (int16_t)map(batv, 3300, 4100, 0, 100);
    float g = bati * 0.01f;
    
    o.rotation.y += 5;
    wm.fixedDraw();
    
    // text
    M5.Lcd.setCursor(120, 0);
    M5.Lcd.print(String(bati)+"%");
    M5.Lcd.setCursor(120, 16);
    M5.Lcd.print(String(batv*0.001f)+"V");
}