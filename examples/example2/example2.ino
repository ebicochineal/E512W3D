#include "E512W3D.hpp"
#include "cube.hpp"

E512W3DWindow w;
E512Array<Object3D> origins;
E512Array<Object3D> objs;
Object3D a;
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
    
    for (int y = 0; y < gsize; ++y) {
        for (int x = 0; x < gsize; ++x) {
            Object3D o;
            o.mesh = &cube;
            o.render_type = RenderType::PolygonColor;
            o.position.y = 1;
            o.color = color565(32, 64, 255);
            objs.emplace_back(o);
        }
    }
    
    for (int y = 0; y < gsize; ++y) {
        for (int x = 0; x < gsize; ++x) {
            Object3D o;
            o.position.x = -gsize + x * 2 + 1;
            o.position.z = gsize + -y * 2 - 1;
            origins.emplace_back(o);
        }
    }
    
    for (int i = 0; i < gsize * gsize; ++i) {;
        origins[i].addChild(objs[i]);
    }
    
    
    for (auto&& i : origins) {
        a.addChild(i);
    }
    
    w.addChild(a);
    
    camera.position.z = 32;
    camera.position.y = 16;
    camera.rotation.x = -30;
    w.setCamera(camera);
    
    w.setDirectionalLight(0, -1, -1);
    w.ambient = 0.7;
    e512w3d.add(w);
    
    e512w3d.begin();
}

void loop() {
    static float v = 0;
    if (e512w3d.isFixedTime()) {
        for (int i = 0; i < gsize * gsize; ++i) {
            int x = i % gsize;
            int y = i / gsize;
            origins[i].scale.y = sin((x+y) + v) * 0.5 + cos(y + v) * 0.5 + 1.0;
            float t = origins[i].scale.y;
            objs[i].color = color565(min(32.0 * t, 255.0), min(64.0 * t, 255.0), min(255.0 * t, 255.0));
        }
        a.rotation.y = v * 4;
        v += 0.2;
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