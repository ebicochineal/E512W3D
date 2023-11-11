#include "E512W3D.hpp"
#include "E512W3DTileMap.hpp"
#include "tile80x80.hpp"

E512W3DWindow w;
Object3D camera;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    M5.MPU6886.Init();

    e512w3d.width = 160*4;
    e512w3d.height = 80*4;
    w.width = e512w3d.width;
    w.height = e512w3d.height;
    w.setCamera(camera);
    camera.position.x += 16*8;
    camera.position.y += 16*8;
    
    e512w3d.add(w);
    e512w3d.begin();
    w.bgcolor = color565(80, 80, 80);
}

void game () {
    static bool initf = true;
    static E512W3DTileMap tilemap(32, 32);
    static E512W3DGameObject2D m(&tile80x80_texture);
    static int tileindex = 0;
    
    if (initf) {
        tilemap.texture = &tile80x80_texture;
        tilemap.tex_h = 16;
        tilemap.tex_w = 16;
        tilemap.tile.emplace_back();
        tilemap.tile.emplace_back(1, 0, 0, 1);
        tilemap.tile.emplace_back(2, 0, 0, 0);
        tilemap.tile.emplace_back(0, 4, 4, 0);
        tilemap.tile.emplace_back(2, 0, 0, 4, 2, 0, 2, 0, 1);
        tilemap.tile.emplace_back(2, 0, 0, 0, 2, 4, 1, 0, 0);
        
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                tilemap.tile.emplace_back(x, y, 0, 0);
                tilemap.setTileIndex(tilemap.width/2+1+x, 4-y, tilemap.tile.size()-1);
            }
        }
        for (int x = 0; x < 6; ++x) { tilemap.setTileIndex(tilemap.width/2+1+x, 5, x); }
        
        for (int y = 0; y < tilemap.height/2; ++y) {
            for (int x = 0; x < tilemap.width/2 ; ++x) {
                bool a = (y > 2 && xrnd()%12==0);
                bool b = (x == 0 || y == 0 || x == tilemap.width/2-1 || y == tilemap.height/2-1);
                tilemap.setTileIndex(x, y, (a || b ? 1 : 2));
            }
        }
        tilemap.setTileIndex(13, 4, 5);// coin
        
        // character
        m.collision_layer = 1;
        m.position.x = 16;
        m.position.y = 16;
        m.tex_w = 16;
        m.tex_h = 16;
        m.tex_x = 0;
        m.tex_y = 0;
        m.collisionFitTexture();
        
        initf = false;
    }
    
    // drag camera move
    camera.position.x -= E512W3DInput::delta1.x;
    camera.position.y += E512W3DInput::delta1.y;
    
    // character action
    bool ground = m.isGround(tilemap);
    if (ground && E512W3DInput::getKey('W')) { m.velocity.y = 10; }
    if (E512W3DInput::getKey('A')) { m.velocity.x = max(m.velocity.x-2.0f, -8.0f); }
    if (E512W3DInput::getKey('D')) { m.velocity.x = min(m.velocity.x+2.0f,  8.0f); }
    if (!ground) { m.velocity.y -= 0.75; }
    if (m.moveX(tilemap)) { m.velocity.x = 0; }
    if (m.moveY(tilemap)) { m.velocity.y = 0; }
    m.velocity.x *= 0.6;
    for (auto&& i : m.adjacentTileList(tilemap)) {
        if (tilemap.getTileIndex(i.x, i.y) == 5) { tilemap.setTileIndex(i.x, i.y, 2); }
    }
    
    draw2d(w, tilemap);
    draw2d(w, m);
    
    E512Point t = tilemapPosition(w, tilemap);
    
    if (tilemap.isInside(t.x, t.y)) {
        // draw
        if (E512W3DInput::getButton(0) && t.x < tilemap.width/2 && t.y < tilemap.height/2) { tilemap.setTileIndex(t.x, t.y, tileindex); }
        // select
        if (E512W3DInput::getButton(2)) {
            tilemap.setTileIndex(tilemap.width/2+1, 6, tilemap.getTileIndex(t.x, t.y));
            tileindex = tilemap.getTileIndex(t.x, t.y);
        }
    }
    w.print("tileindex = ");
    w.print(tileindex);
    w.print(" : (");
    w.print(tilemap.tile[tileindex].tex_x);
    w.print(", ");
    w.print(tilemap.tile[tileindex].tex_y);
    w.print(")");
}

void loop() {
    if (e512w3d.isFixedTime()) {
        E512W3DInput::update();
        e512w3d.clear();
        w.begin();
        game();
        e512w3d.pushScreen();
    }
}
