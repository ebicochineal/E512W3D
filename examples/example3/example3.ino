#include "M5StickC.h"
#include "E512W3D.hpp"
#include "zou.hpp"

E512WindowManager wm(160, 80);
E512W3D w0(wm.width/2, 0, wm.width/2, wm.height/2, M5.Lcd.color565(0, 0, 0));
E512W3D w1(0, wm.height/2, wm.width/2, wm.height/2, M5.Lcd.color565(0, 0, 0));
E512W3D w2(wm.width/2, wm.height/2, wm.width/2, wm.height/2, M5.Lcd.color565(0, 0, 0));
E512W3D w3(0, 0, wm.width/2, wm.height/2, M5.Lcd.color565(0, 0, 0));
Object3D a0, a;
Object3D o, c0, c1, c2, c3;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    zouInit();
    
    a.mesh = &zou;
    a.position.y = -2;
    a.position.z = -1;
    a.render_type = RenderType::WireFrame;
    a.color = M5.Lcd.color565(255, 255, 255);
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
    c0.rotation.x = -20;
    o.addChild(c0);
    // w0.setDirectionalLight(0, -1, -1);
    w0.setCamera(c0);
    
    c1.position.z = 8;
    w1.ortho_size = 0.05;
    w1.setCamera(c1);
    w1.isortho = true;
    
    c2.position.x = 8;
    c2.rotation.y = 90;
    w2.ortho_size = 0.05;
    w2.setCamera(c2);
    w2.isortho = true;
    
    c3.position.y = 8;
    c3.rotation.x = -90;
    w3.ortho_size = 0.05;
    w3.setCamera(c3);
    w3.isortho = true;
    
    // 3 0
    // 1 2
    
    wm.add(w0);
    wm.add(w1);
    wm.add(w2);
    wm.add(w3);
}

void loop() {
    if (wm.isFixedTime()) {
        o.rotation.y += 1.0f;
        wm.draw();
        
        // battery
        // int16_t batv = (int16_t)(M5.Axp.GetVapsData() * 1.4f);
        // batv = max(min(batv, 4100), 3300);
        // int16_t bati = (int16_t)map(batv, 3300, 4100, 0, 100);
        // float g = bati * 0.01f;
        // M5.Lcd.setCursor(120, 0);
        // M5.Lcd.print(String(bati)+"%");
        // M5.Lcd.setCursor(120, 16);
        // M5.Lcd.print(String(batv*0.001f)+"V");
    }
}