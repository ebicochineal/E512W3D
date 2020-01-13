#include <M5StickC.h>
#include "E512W3D.h"
#include "cube.h"

E512WindowManager wm(160, 80);
// E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(200, 44, 85));
E512W3D w( 0, 0, 160, 80, M5.Lcd.color565(20, 20, 20));

Object3D a, b, c, d, e, f;
Object3D camera;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    cubeInit();
    
    a.mesh = &cube;
    a.render_type = RenderType::PolygonNormal;
    a.addChild(b);
    a.addChild(c);
    a.addChild(d);
    a.addChild(e);
    
    b.mesh = &cube;
    b.position.x = 12;
    b.rotation.y = 180;
    b.render_type = RenderType::Polygon;
    b.color = M5.Lcd.color565(255, 0, 0);
    
    c.mesh = &cube;
    c.position.x = -12;
    c.render_type = RenderType::Polygon;
    c.color = M5.Lcd.color565(0, 255, 0);
    
    d.mesh = &cube;
    d.position.z = 12;
    d.rotation.y = 90;
    d.render_type = RenderType::Polygon;
    d.color = M5.Lcd.color565(0, 0, 255);
    
    e.mesh = &cube;
    e.position.z = -12;
    e.rotation.y = 270;
    e.render_type = RenderType::WireFrame;
    e.color = M5.Lcd.color565(255, 255, 255);
    
    w.addChild(a);
    camera.position.z = 32;
    camera.position.y = 16;
    camera.rotation.x = -30;
    w.setCamera(camera);
    w.setDirectionalLight(-1, -1, -1);
    wm.add(w);
    
    // w.ambient = 1;
    
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
    
    // move
    a.rotation.y += 6 * g;
    v += 0.5 * g;
    b.position.y = abs(sin(v)) * 3 * g;
    c.position.y = abs(sin(v+0.25)) * 3 * g;
    d.position.y = abs(sin(v+0.5)) * 3 * g;
    e.position.y = abs(sin(v+1)) * 3 * g;
    
    
    wm.fixedDraw();
    
    // text
    M5.Lcd.setCursor(120, 0);
    M5.Lcd.print(String(bati)+"%");
    M5.Lcd.setCursor(120, 16);
    M5.Lcd.print(String(batv*0.001f)+"V");
}