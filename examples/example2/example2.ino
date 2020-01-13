#include <M5StickC.h>
#include "E512W3D.h"
#include "cube.h"

E512WindowManager wm(160, 80);
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(0, 0, 0));
E512Array<Object3D> origins;
E512Array<Object3D> objs;
Object3D a;
Object3D camera;

const uint8_t gsize = 16;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    cubeInit();
    
    for (int y = 0; y < gsize; ++y) {
        for (int x = 0; x < gsize; ++x) {
            Object3D o;
            o.mesh = &cube;
            o.render_type = RenderType::Polygon;
            o.position.y = 1;
            o.color = M5.Lcd.color565(32, 64, 255);
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
    
    for (int i = 0; i < gsize * gsize; ++i) {
        // objs[i].setParent(origins[i]);
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
    
    for (int i = 0; i < gsize * gsize; ++i) {
        int x = i % gsize;
        int y = i / gsize;
        
        origins[i].scale.y = sin((x+y) + v) * 0.5 + cos(y + v) * 0.5 + 1.0;
        float t = sin((x+y) + v) * 0.5  + cos(y + v) * 0.5  + 1.0;
        t = map(t * 100, 0, 200, 50, 100) * 0.01f;
        
        objs[i].color = M5.Lcd.color565(32.0 * t, 64.0 * t, 255.0 * t);
    }
    
    a.rotation.y = v * 4;
    v += 0.2;
    
    wm.fixedDraw();
    
    // text
    M5.Lcd.setCursor(120, 0);
    M5.Lcd.print(String(bati)+"%");
    M5.Lcd.setCursor(120, 16);
    M5.Lcd.print(String(batv*0.001f)+"V");
}