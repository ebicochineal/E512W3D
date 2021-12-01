#include "M5StickC.h"
#include "E512W3D.h"

#include "ebi.h"
#include "ebi_64_29.h"

E512WindowManager wm(160, 80);
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(0, 0, 0));
Object3D a;
Object3D camera;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    
    ebiInit();
    a.mesh = &ebi;
    a.texture = &ebi_64_29_texture;
    a.render_type = RenderType::PolygonTexture;
    a.color = M5.Lcd.color565(255, 255, 255);
    a.rotation.x = 0;
    
    camera.position.z = 15;
    camera.position.y = 1;
    w.setCamera(camera);
    w.setDirectionalLight(-1, -1, -1);
    w.ambient = 0.8;
    w.addChild(a);
    
    wm.add(w);
    M5.MPU6886.Init();
}


void loop () {
    a.rotation.y += 5;
    wm.fixedDraw();
    
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

int main () {
    setup();
    while (1) {
        loop();
        delay(100);
    }
}