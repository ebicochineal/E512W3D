#include "E512W3D.hpp"

#include "ebi.hpp"
#include "ebi_64_32.hpp"

E512W3DWindow w;
Object3D a;
Object3D camera;

void setup () {
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
    static int cnt = 0;
    
    // if (e512w3d.isFixedTime()) {
        a.rotation *= Quaternion::angleAxis(5.0, Vector3(0, 1, 0));
        if ((cnt / 60) % 2 == 0) {
            // e512w3d.clear();
            // w.draw();// window all object draw
            
            e512w3d.clear();
            w.begin();
            w.draw(a);// window object draw
            
            w.print("core 1");
        } else {
            // e512w3d.clear();
            // multiCoreDraw(w);// window all object draw multicore
            
            e512w3d.clear();
            w.begin();
            multiCoreDraw(w, a);// window object draw multicore
            
            w.print("core 2");
        }
        e512w3d.pushScreen();

        cnt += 1;
    // }
}
