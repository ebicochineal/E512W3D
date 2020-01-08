#include <M5StickC.h>
#include "E512W3D.h"
#include "cube.h"

E512WindowManager wm(160, 80);
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(0, 0, 0));
E512Array<Object3D> origins;
E512Array<Object3D> objs;
Object3D a;
void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    cubeInit();
    
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Object3D o;
            o.mesh = &cube;
            o.render_type = RenderType::Polygon;
            o.position.y = 1;
            o.color = M5.Lcd.color565(32, 64, 255);
            objs.emplace_back(o);
        }
    }
    
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Object3D o;
            o.position.x = -7 + x * 2;
            o.position.z = 7 + -y * 2;
            o.child.emplace_back(&objs[origins.size()]);
            origins.emplace_back(o);
        }
    }
    
    for (auto&& i : origins) {
        a.child.emplace_back(&i);
    }
    
    w.child.emplace_back(&a);
    
    
    w.camera.position.z = 16;
    w.camera.position.y = 10;
    w.camera.rotation.x = -30;
    w.setDirectionalLight(0, -1, -1);
    w.ambient = 0.7;
    wm.add(w);
    
    
    
    M5.MPU6886.Init();
}

float accX = 0;
float accY = 0;
float accZ = 0;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float temp = 0;

float v = 0;


void loop() {
    M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.MPU6886.getAccelData(&accX, &accY, &accZ);
    M5.MPU6886.getTempData(&temp);
    // rot 0
    // int x = accY*20;
    // int y = -accX*20;
    
    // rot 1
    // int x = accY*20;
    // int y = accX*20;
    
    // battery
    int16_t batv = (int16_t)(M5.Axp.GetVapsData() * 1.4f);
    batv = max(min(batv, 4100), 3300);
    int16_t bati = (int16_t)map(batv, 3300, 4100, 0, 100);
    float g = bati * 0.01f;
    
    for (int i = 0; i < 64; ++i) {
        int x = i % 8;
        int y = i / 8;
        
        origins[i].scale.y = sin((x+y) + v) * 0.5 + cos(y + v) * 0.5 + 1.0;
        float t = sin((x+y) + v) * 0.5  + cos(y + v) * 0.5  + 1.0;
        t = map(t * 100, 0, 200, 50, 100) * 0.01f;
        
        objs[i].color = M5.Lcd.color565(32.0 * t, 64.0 * t, 255.0 * t);
    }
    
    a.rotation.y = v * 2;
    v += 0.2;
    
    wm.fixedDraw();
    
    // text
    M5.Lcd.setCursor(120, 0);
    M5.Lcd.print(String(bati)+"%");
    M5.Lcd.setCursor(120, 16);
    M5.Lcd.print(String(batv*0.001f)+"V");
}