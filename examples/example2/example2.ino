#include "E512W3D.hpp"
#include "cube.hpp"

E512W3DWindow w;
Object3D a, b, c;
Object3D camera;

const uint8_t gsize = 16;

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
    a.render_type = RenderType::PolygonColor;
    a.position.y = 1;
    b.addChild(a);
    c.addChild(b);
    w.addChild(c);
    
    camera.position.z = 32;
    camera.position.y = 16;
    camera.rotation = Quaternion::angleAxis(-30, Vector3(1, 0, 0));
    
    w.setCamera(camera);
    
    w.setDirectionalLight(0, -1, -1);
    w.ambient = 0.7;
    e512w3d.add(w);
    
    e512w3d.begin();
}

void loop() {
    static float v = 0;
    if (e512w3d.isFixedTime()) {
        v += 0.2;
        c.rotation = Quaternion::angleAxis(v*4 , Vector3(0, 1, 0));
        
        e512w3d.clear();
        w.begin();
        for (int y = 0; y < gsize; ++y) {
            for (int x = 0; x < gsize; ++x) {
                b.position.x = -gsize + x*2 + 1;
                b.position.z = gsize + -y*2 - 1;
                b.scale.y = sin((x+y) + v)*0.5 + cos(y+v)*0.5 + 1.0;
                float t = b.scale.y;
                a.color = color565(min(32.0* t, 255.0), min(64.0*t, 255.0), min(255.0*t, 255.0));
                w.draw(a);
            }
        }
        e512w3d.pushScreen();
        
        // battery
        // int16_t batv = (int16_t)(M5.Axp.GetVapsData() * 1.4f);
        // batv = max(min(batv, 4100), 3300);
        // int16_t bati = (int16_t)map(batv, 3300, 4100, 0, 100);
        // M5.Lcd.setCursor(120, 0);
        // M5.Lcd.print(String(bati)+"%");
        // M5.Lcd.setCursor(120, 16);
        // M5.Lcd.print(String(batv*0.001f)+"V");
    }
}