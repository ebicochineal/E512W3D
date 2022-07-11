#include "E512W3D.hpp"

#include "ebi.hpp"
#include "ebi_64_32.hpp"

E512W3DWindow w;
Object3D a;
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
    
    ebiInit();
    a.mesh = &ebi;
    a.texture = &ebi_64_32_texture;
    a.render_type = RenderType::PolygonTexture;
    a.color = M5.Lcd.color565(255, 255, 255);
    a.rotation.x = 0;
    
    camera.position.z = 15;
    camera.position.y = 1;
    w.setCamera(camera);
    w.setDirectionalLight(-1, -1, -1);
    w.ambient = 0.8;
    w.addChild(a);
    
    e512w3d.add(w);
    e512w3d.begin();
}

void loop () {
    if (e512w3d.isFixedTime()) {
        a.rotation.y += 5.0f;
        e512w3d.draw();
        
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
