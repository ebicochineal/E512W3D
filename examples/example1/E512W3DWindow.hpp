#pragma once
#include "E512W3DUtils.hpp"

int cursor_x = 0;
int cursor_y = 0;
bool cursor_l = false;
bool cursor_m = false;
bool cursor_r = false;

E512Array<bool> keys = E512Array<bool>(128, false);

bool readkey = false;// ncurses
bool keydown (char c) {
    readkey = true;// ncurses
    return keys[c];
}
void clearkeyarray () {// ncurses
    if (!readkey) { return; }
    readkey = false;
    for (int i=0; i < 128; ++i) { keys[i] = false; }
}
class E512W3DInput {
private:
    static E512Array<bool> tmp, prev;
    static E512Array<bool> mtmp, mprev;
    static Vector2 prev0, prev1, prev2;
public:
    static Vector2 delta0, delta1, delta2;
    static int16_t width, height;
    static void update () {
#if defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2)
        float ax, ay, az;
        M5.update();
        M5.MPU6886.getAccelData(&ax, &ay, &az);
        
        E512W3DInput::mprev[0] = E512W3DInput::mtmp[0];
        E512W3DInput::mprev[1] = E512W3DInput::mtmp[1];
        // E512W3DInput::mprev[2] = E512W3DInput::mtmp[2];
        E512W3DInput::mtmp[0] = M5.BtnA.isPressed();
        E512W3DInput::mtmp[1] = M5.BtnB.isPressed();
        // E512W3DInput::mtmp[2] = M5.BtnC.isPressed();
        
        if (abs(ay) > 0.4) { cursor_x += ay * 5; }
        if (abs(ax) > 0.4) { cursor_y += ax * 5; }
        
        cursor_x = max(min(cursor_x, E512W3DInput::width-1), 0);
        cursor_y = max(min(cursor_y, E512W3DInput::height-1), 0);
#else
        for (int i = 0; i < 128; ++i) {
            E512W3DInput::prev[i] = E512W3DInput::tmp[i];
            E512W3DInput::tmp[i] = keydown(i);
        }
        E512W3DInput::mprev[0] = E512W3DInput::mtmp[0];
        E512W3DInput::mprev[1] = E512W3DInput::mtmp[1];
        E512W3DInput::mprev[2] = E512W3DInput::mtmp[2];
        E512W3DInput::mtmp[0] = cursor_l;
        E512W3DInput::mtmp[1] = cursor_m;
        E512W3DInput::mtmp[2] = cursor_r;
#endif
        if (E512W3DInput::getButtonDown(0)) { E512W3DInput::prev0 = E512W3DInput::cursorPosition(); }
        if (E512W3DInput::getButtonDown(1)) { E512W3DInput::prev1 = E512W3DInput::cursorPosition(); }
        if (E512W3DInput::getButtonDown(2)) { E512W3DInput::prev2 = E512W3DInput::cursorPosition(); }
        
        
        
        E512W3DInput::delta0 = Vector2(0);
        E512W3DInput::delta1 = Vector2(0);
        E512W3DInput::delta2 = Vector2(0);
        if (E512W3DInput::getButton(0)) {
            E512W3DInput::delta0 = E512W3DInput::cursorPosition() - E512W3DInput::prev0;
            E512W3DInput::prev0 = E512W3DInput::cursorPosition();
        }
        if (E512W3DInput::getButton(1)) {
            E512W3DInput::delta1 = E512W3DInput::cursorPosition() - E512W3DInput::prev1;
            E512W3DInput::prev1 = E512W3DInput::cursorPosition();
        }
        if (E512W3DInput::getButton(2)) {
            E512W3DInput::delta2 = E512W3DInput::cursorPosition() - E512W3DInput::prev2;
            E512W3DInput::prev2 = E512W3DInput::cursorPosition();
        }
        
        
        
        
    }
    static bool getKey (uint8_t c) { return E512W3DInput::tmp[c]; }
    
    // doesn't work with ncurses
    static bool getKeyUp (uint8_t c) { return !E512W3DInput::tmp[c] && E512W3DInput::tmp[c] != E512W3DInput::prev[c]; }
    
    // doesn't work with ncurses
    static bool getKeyDown (uint8_t c) { return E512W3DInput::tmp[c] && E512W3DInput::tmp[c] != E512W3DInput::prev[c]; }
    
    static bool getButton (uint8_t c) {
        if (c < 3) { return E512W3DInput::mtmp[c]; }
        return false;
    }
    static bool getButtonUp (uint8_t c) {
        if (c < 3) { return !E512W3DInput::mtmp[c] && E512W3DInput::mtmp[c] != E512W3DInput::mprev[c]; }
        return false;
    }
    static bool getButtonDown (uint8_t c) {
        if (c < 3) { return E512W3DInput::mtmp[c] && E512W3DInput::mtmp[c] != E512W3DInput::mprev[c]; }
        return false;
    }
    static Vector2 cursorPosition () { return Vector2(cursor_x, cursor_y); }
    
};
E512Array<bool> E512W3DInput::tmp = E512Array<bool>(128, false);
E512Array<bool> E512W3DInput::prev = E512Array<bool>(128, false);
E512Array<bool> E512W3DInput::mtmp = E512Array<bool>(3, false);
E512Array<bool> E512W3DInput::mprev = E512Array<bool>(3, false);
int16_t E512W3DInput::width = 160;
int16_t E512W3DInput::height = 80;
Vector2 E512W3DInput::prev0, E512W3DInput::prev1, E512W3DInput::prev2;
Vector2 E512W3DInput::delta0, E512W3DInput::delta1, E512W3DInput::delta2;

class E512W3DWindow {
public:
    Object3D* camera = NULL;
    E512Font* font;
    TFT_eSprite* buff;
    TFT_eSprite* zbuff;
    uint16_t screen_width, screen_height;
    
    int16_t sx = 0;
    int16_t sy = 0;
    uint16_t width, height;
    uint16_t bgcolor = 0;
    bool isortho = false;
    float ortho_size = 0.1f;
    float ambient = 0;// 0f - 1f
    float light_strength = 1.0f;
    int16_t maxdiv = 4;
    E512Array<Object3D*> child;
    
    E512W3DWindow () {
        this->init(0, 0, 160, 80, 0, Vector3(0, -1, 0));
    }
    E512W3DWindow (uint16_t bgcolor) {
        this->init(0, 0, 160, 80, bgcolor, Vector3(0, -1, 0));
    }
    E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height) {
        this->init(sx, sy, width, height, 0, Vector3(0, -1, 0));
    }
    E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height, uint16_t bgcolor) {
        this->init(sx, sy, width, height, bgcolor, Vector3(0, -1, 0));
    }
    E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height, uint16_t bgcolor, Vector3 light) {
        this->init(sx, sy, width, height, bgcolor, light);
    }
    
    void init (int16_t sx, int16_t sy, uint16_t width, uint16_t height, uint16_t bgcolor, Vector3 light) {
        this->sx = sx;
        this->sy = sy;
        this->width = width;
        this->height = height;
        this->bgcolor = bgcolor;
        this->setDirectionalLight(light);
    }
    
    ~E512W3DWindow () {}
    
    void resize (uint16_t width, uint16_t height) {
        this->width = width;
        this->height = height;
    }
    
    void draw () { this->drawmt(0, 1); }
    
    void draw (Object3D& obj, bool child = false) { this->drawmt(obj, 0, 1, child); }
    
    void drawmt (int sindex, int nthread) {
        this->begin();
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        this->drawChild(this->child, Matrix4x4::identity(), sindex, nthread);
        this->drawChildT(this->child, Matrix4x4::identity(), sindex, nthread);
    }
    
    void drawmt (Object3D& obj, int sindex, int  nthread, bool child = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        Matrix4x4 mat = Matrix4x4::identity();
        if (obj.parent != NULL) { mat = obj.parent->worldMatrix(); }
        
        if (child) {
            E512Array<Object3D*> objs;
            objs.emplace_back(&obj);
            this->drawChild(objs, mat, sindex, nthread);
            this->drawChildT(objs, mat, sindex, nthread);
        } else {
            mat = this->worldMatrix(&obj, mat);
            if (obj.mesh == NULL) { return; }
            if (obj.render_type == RenderType::WireFrame) { this->drawWireFrameObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonColor) { this->drawPolygonColorObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonNormal) { this->drawPolygonNormalObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonTexture) { this->drawPolygonTextureObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonTextureDoubleFace) { this->drawPolygonTextureDoubleFaceObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonTexturePerspectiveCorrect) { this->drawPolygonTexturePerspectiveCorrectObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonTexturePerspectiveCorrectDoubleFace) { this->drawPolygonTexturePerspectiveCorrectDoubleFaceObject(&obj, mat, sindex, nthread); }
            if (obj.render_type == RenderType::PolygonTranslucent) { this->drawPolygonTranslucentObject(&obj, mat, sindex, nthread); }
        }
    }
    
    void drawObjestAxis (Object3D& obj) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Matrix4x4 mat = this->objectWorldViewMatrix(obj);
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(Vector3(), mat), this->projscreen);
        Vector3 x = Matrix4x4::muld(Matrix4x4::mul(Vector3(2, 0, 0), mat), this->projscreen);
        Vector3 y = Matrix4x4::muld(Matrix4x4::mul(Vector3(0, 2, 0), mat), this->projscreen);
        Vector3 z = Matrix4x4::muld(Matrix4x4::mul(Vector3(0, 0, 2), mat), this->projscreen);
        this->drawBuffLine(s.x, s.y, x.x, x.y, color565(255, 0, 0));
        this->drawBuffLine(s.x, s.y, y.x, y.y, color565(0, 255, 0));
        this->drawBuffLine(s.x, s.y, z.x, z.y, color565(0, 0, 255));
    }
    
    
    void drawLine (Object3D& start, Object3D& end, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(start)), this->projscreen);
        Vector3 e = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(end)), this->projscreen);
        if (s.z < 0 || s.z > 1 || e.z < 0 || e.z > 1) { return; }
        this->drawBuffLine(s.x, s.y, e.x, e.y, color);
    }
    
    void drawLine (Vector3 start, Vector3 end, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(start, this->view), this->projscreen);
        Vector3 e = Matrix4x4::muld(Matrix4x4::mul(end, this->view), this->projscreen);
        if (s.z < 0 || s.z > 1 || e.z < 0 || e.z > 1) { return; }
        this->drawBuffLine(s.x, s.y, e.x, e.y, color);
    }
    void drawLine (uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        this->drawBuffLine(sx, sy, ex, ey, color);
    }
    
    void drawPoint (Object3D& obj, uint16_t size = 1, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 p = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(obj)), this->projscreen);
        if (p.z < 0 || p.z > 1) { return; }
        this->drawPoint(p.x, p.y, size, color);
    }
    void drawPoint (Vector3 p, uint16_t size = 1, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        p = Matrix4x4::muld(Matrix4x4::mul(p, this->view), this->projscreen);
        if (p.z < 0 || p.z > 1) { return; }
        this->drawPoint(p.x, p.y, size, color);
    }
    
    void drawPoint (int16_t px, int16_t py, uint16_t size = 1, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        int16_t sx = px - ((size-1) >> 1);
        int16_t sy = py - ((size-1) >> 1);
        int16_t ex = min((int16_t)(sx + size), (int16_t)(this->width-1));
        int16_t ey = min((int16_t)(sy + size), (int16_t)(this->height-1));
        sx = max(sx, (int16_t)0);
        sy = max(sy, (int16_t)0);
        for (int16_t y = sy; y < ey; ++y) {
            for (int16_t x = sx; x < ex; ++x) {
                if (this->inSide2(x, y)) { this->buff->drawPixel(x + this->sx, y + this->sy, color); }
            }
        }
    }
    
    
    void drawCircle (int16_t sx, int16_t sy, int16_t ex, int16_t ey, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        if (sx > ex) { this->swap(sx, ex); }
        if (sy > ey) { this->swap(sy, ey); }
        
        sx += this->sx;
        sy += this->sy;
        ex += this->sx;
        ey += this->sy;
        
        float width = (ex - sx);
        float height = (ey - sy);
        float aspectratio = width / height;
        
        float dw = (ex - sx) % 2 ? 0 : 0.5;
        float dh = (ey - sy) % 2 ? 0 : 0.5;
        float px = ((sx + ex) >> 1) - dw;
        float py = ((sy + ey) >> 1) - dh;
        float size = (ex-sx) * 0.5;
        ex = min(ex, this->dex);
        ey = min(ey, this->dey);
        sx = max(sx, this->dsx);
        sy = max(sy, this->dsy);
        float ss = size * size;
        for (int16_t y = sy; y < ey; ++y) {
            for (int16_t x = sx; x < ex; ++x) {
                float d = (x - px) * (x - px) + (y - py) * aspectratio * (y - py) * aspectratio; 
                if (d <= ss) { this->buff->drawPixel(x, y, color); }
            }
        }
    }
    void drawRect (int16_t sx, int16_t sy, int16_t ex, int16_t ey, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        if (sx > ex) { this->swap(sx, ex); }
        if (sy > ey) { this->swap(sy, ey); }
        
        sx += this->sx;
        sy += this->sy;
        ex += this->sx;
        ey += this->sy;
        
        ex = min(ex, this->dex);
        ey = min(ey, this->dey);
        sx = max(sx, this->dsx);
        sy = max(sy, this->dsy);
        for (int16_t y = sy; y < ey; ++y) {
            for (int16_t x = sx; x < ex; ++x) {
                this->buff->drawPixel(x, y, color);
            }
        }
    }
    
    
    void drawTexture (int16_t sx, int16_t sy, int16_t ex, int16_t ey, Texture& tex, bool flipx = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        if (sx > ex) { this->swap(sx, ex); }
        if (sy > ey) { this->swap(sy, ey); }
        
        sx += this->sx;
        sy += this->sy;
        ex += this->sx;
        ey += this->sy;
        
        const float w = ex-sx;
        const float h = ey-sy;
        if (w*h < 1) { return; }
        
        int16_t isx = max(sx, this->dsx);
        int16_t isy = max(sy, this->dsy);
        int16_t iex = min(ex, this->dex);
        int16_t iey = min(ey, this->dey);
        
        float fx = flipx ? -1.0f : 1.0f;
        
        static E512Array<int> ua, va;
        ua.clear();
        va.clear();
        for (int y=isy; y < iey; ++y) {
            const float v = float(y-sy)/h;
            const int t = (int)(tex.height * v)*tex.width;
            va.emplace_back(t);
        }
        if (flipx) {
            for (int x=isx; x < iex; ++x) {
                const float u = 0.999999f-float(x-sx)/w;
                const int t = (int)(tex.width * u);
                ua.emplace_back(t);
            }
        } else {
            for (int x=isx; x < iex; ++x) {
                const float u = float(x-sx)/w;
                const int t = (int)(tex.width * u);
                ua.emplace_back(t);
            }
        }
        
        for (int y=isy, vi=0; y < iey; ++y, ++vi) {
            for (int x=isx, ui=0; x < iex; ++x, ++ui) {
                const uint16_t c1555 = tex.pixels[va[vi]+ua[ui]];
                const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                const uint16_t b = ((c1555      ) & 0b11111)      ;
                if ((c1555 >> 15 & 1) == 0) { this->buff->drawPixel(x, y, r|g|b); }
            }
        }
    }
    
    void drawTexture (int16_t sx, int16_t sy, Texture& tex, bool flipx = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        sx += this->sx;
        sy += this->sy;
        int16_t ex = sx + tex.width;
        int16_t ey = sy + tex.height;
        
        ex = min(ex, this->dex);
        ey = min(ey, this->dey);
        int16_t isx = max(sx, this->dsx);
        int16_t isy = max(sy, this->dsy);
        int16_t itx = sx < this->dsx ? this->dsx - sx : 0;
        int16_t ity = sy < this->dsy ? this->dsy - sy : 0;
        if (flipx) {
            for (int y = isy, ty = ity; y < ey; ++y, ++ty) {
                const int tyy = ty * tex.width;
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    const uint16_t c1555 = tex.pixels[tyy + tex.width-tx-1];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) { this->buff->drawPixel(x, y, r|g|b); }
                }
            }
        } else {
            for (int y = isy, ty = ity; y < ey; ++y, ++ty) {
                const int tyy = ty * tex.width;
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    const uint16_t c1555 = tex.pixels[tyy + tx];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) { this->buff->drawPixel(x, y, r|g|b); }
                }
            }
        }
    }
    
    
    void drawChar (uint8_t c, int16_t px, int16_t py) {
        if (px >= this->width || px + this->font->getWidth(c) * this->text_size < 0) { return; }
        if (py >= this->height || py + this->font->getHeight(c) * this->text_size < 0) { return; }
        
        for (int y = 0; y < this->font->getHeight(c); ++y) {
            for (int x = 0; x < this->font->getWidth(c); ++x) {
                if (this->font->getPixel(c, y, x)) {
                    for (int my = 0; my < this->text_size; ++my) {
                        for (int mx = 0; mx < this->text_size; ++mx) {
                            int wx = px + x * this->text_size + mx;
                            int wy = py + y * this->text_size + my;
                            if (this->inSide2(wx, wy)) { this->buff->drawPixel(wx + this->sx, wy + this->sy, this->text_color); }
                        }
                    }
                }
            }
        }
    }
    void drawCharBG (uint8_t c, int16_t px, int16_t py) {
        if (px >= this->width || px + this->font->getWidth(c) * this->text_size < 0) { return; }
        if (py >= this->height || py + this->font->getHeight(c) * this->text_size < 0) { return; }
        
        for (int y = 0; y < this->font->getHeight(c); ++y) {
            for (int x = 0; x < this->font->getWidth(c); ++x) {
                if (this->font->getPixel(c, y, x)) {
                    for (int my = 0; my < this->text_size; ++my) {
                        for (int mx = 0; mx < this->text_size; ++mx) {
                            int wx = px + x * this->text_size + mx;
                            int wy = py + y * this->text_size + my;
                            if (wx >= this->width || wx < 0) { continue; }
                            if (wy >= this->height || wy < 0) { continue; }
                            this->buff->drawPixel(this->sx + wx, this->sy + wy, this->text_color);
                        }
                    }
                } else {
                    for (int my = 0; my < this->text_size; ++my) {
                        for (int mx = 0; mx < this->text_size; ++mx) {
                            int wx = px + x * this->text_size + mx;
                            int wy = py + y * this->text_size + my;
                            if (wx >= this->width || wx < 0) { continue; }
                            if (wy >= this->height || wy < 0) { continue; }
                            this->buff->drawPixel(this->sx + wx, this->sy + wy, this->text_bgcolor);
                        }
                    }
                }
            }
        }
    }
    
    Vector3 screenPosition (Object3D& obj) {
        return Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(obj)), this->projscreen);
    }
    Vector3 screenPosition (Vector3 p) {
        return Matrix4x4::muld(Matrix4x4::mul(p, this->view), this->projscreen);
    }
    
    
    int16_t text_cursor_x = 0;
    int16_t text_cursor_y = 0;
    int16_t text_sub_cursor_x = 0;
    int16_t text_sub_cursor_y = 0;
    int16_t text_size = 1;
    uint16_t text_color = 0xFFFF;
    uint16_t text_bgcolor = 0x0000;
    bool text_use_bgcolor = false;
    
    void setTextCursor (int16_t x, int16_t y) {
        this->text_cursor_x = x;
        this->text_cursor_y = y;
        this->text_sub_cursor_x = 0;
        this->text_sub_cursor_y = 0;
    }
    
    void setTextSize (uint16_t n) {
        this->text_size = n > 0 ? n : 1;
        this->text_size = this->text_size <= 32 ? this->text_size : 32;
    }
    
    
    
    void print (E512Array<uint8_t> v, bool wordwrap = true) {
        for (auto&& c : v) {
            if (c == '\n') {
                this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
                this->text_sub_cursor_x = 0;
                continue;
            }
            if (wordwrap && this->text_sub_cursor_x > 0 && this->text_cursor_x + this->text_sub_cursor_x + this->font->getWidth(c) * this->text_size > this->width) {
                this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
                this->text_sub_cursor_x = 0;
            }
            if (this->text_use_bgcolor) {
                this->drawCharBG(c, this->text_cursor_x + this->text_sub_cursor_x, this->text_cursor_y + this->text_sub_cursor_y);
            } else {
                this->drawChar(c, this->text_cursor_x + this->text_sub_cursor_x, this->text_cursor_y + this->text_sub_cursor_y);
            }
            this->text_sub_cursor_x += this->font->getWidth(c) * this->text_size;
        }
    }
    void println (E512Array<uint8_t> v, bool wordwrap = true) {
        this->print(v, wordwrap);
        this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
        this->text_sub_cursor_x = 0;
    }
    
    void print (const char* cp, bool wordwrap = true) {
        uint16_t index = 0;
        while (true) {
            uint8_t c = cp[index];
            index += 1;
            if (c == '\0') { break; }
            if (c == '\n') {
                this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
                this->text_sub_cursor_x = 0;
                continue;
            }
            if (wordwrap && this->text_sub_cursor_x > 0 && this->text_cursor_x + this->text_sub_cursor_x + this->font->getWidth(c) * this->text_size > this->width) {
                this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
                this->text_sub_cursor_x = 0;
            }
            if (this->text_use_bgcolor) {
                this->drawCharBG(c, this->text_cursor_x + this->text_sub_cursor_x, this->text_cursor_y + this->text_sub_cursor_y);
            } else {
                this->drawChar(c, this->text_cursor_x + this->text_sub_cursor_x, this->text_cursor_y + this->text_sub_cursor_y);
            }
            this->text_sub_cursor_x += this->font->getWidth(c) * this->text_size;
        }
    }
    void println (const char* cp, bool wordwrap = true) {
        this->print(cp, wordwrap);
        this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
        this->text_sub_cursor_x = 0;
    }
    
    void print (const uint8_t c, bool wordwrap = true) {
        E512Array<uint8_t> v;
        v.emplace_back(c);
        this->print(v, wordwrap);
    }
    void println (const uint8_t c, bool wordwrap = true) {
        E512Array<uint8_t> v;
        v.emplace_back(c);
        this->print(v, wordwrap);
        this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
        this->text_sub_cursor_x = 0;
    }
    
    void print (const char c, bool wordwrap = true) {
        E512Array<uint8_t> v;
        v.emplace_back(c);
        this->print(v, wordwrap);
    }
    void println (const char c, bool wordwrap = true) {
        E512Array<uint8_t> v;
        v.emplace_back(c);
        this->print(v, wordwrap);
        this->text_sub_cursor_y += this->font->getHeight(0) * this->text_size;
        this->text_sub_cursor_x = 0;
    }
    
    void print (const int n, bool wordwrap = true) { this->print(numtostr(n), wordwrap); }
    void println (const int n, bool wordwrap = true) { this->println(numtostr(n), wordwrap); }
    void print (const size_t n, bool wordwrap = true) { this->print(numtostr((int)n), wordwrap); }
    void println (const size_t n, bool wordwrap = true) { this->println(numtostr((int)n), wordwrap); }
    void print (const double n, bool wordwrap = true) { this->print(numtostr((float)n), wordwrap); }
    void println (const double n, bool wordwrap = true) { this->println(numtostr((float)n), wordwrap); }
    void print (const float n, bool wordwrap = true) { this->print(numtostr(n), wordwrap); }
    void println (const float n, bool wordwrap = true) { this->println(numtostr(n), wordwrap); }
    
    
    Matrix4x4 objectWorldViewMatrix (Object3D& obj) {
        return Matrix4x4::mul(obj.worldMatrix(), this->view);
    }
    
    void begin (bool color_buffer_clear = true, bool z_buffer_clear = true) {
        this->dsy = min(max(this->sy, (int16_t)0), (int16_t)this->screen_height);
        this->dsx = min(max(this->sx, (int16_t)0), (int16_t)this->screen_width );
        this->dey = min(max((int16_t)(this->sy + this->height), (int16_t)0), (int16_t)this->screen_height);
        this->dex = min(max((int16_t)(this->sx + this->width ), (int16_t)0), (int16_t)this->screen_width );
        
        this->updateViewMatrix();
        this->updateLightVector();
        if (this->isortho) {
            this->projscreen = Matrix4x4::orthoscreenMatrix(this->width, this->height, this->ortho_size);
        } else {
            this->projscreen = Matrix4x4::projscreenMatrix(this->width, this->height);
        }
        
        if (color_buffer_clear) { this->clearCbuff(); }
        if (z_buffer_clear) { this->clearZbuff(); }
        
        this->text_sub_cursor_x = 0;
        this->text_sub_cursor_y = 0;
        this->text_cursor_x = 0;
        this->text_cursor_y = 0;
    }
    
    void setDirectionalLight (float x, float y, float z) { this->setDirectionalLight(Vector3(x, y, z)); }
    void setDirectionalLight (Vector3 d) { this->light = Vector3::normalize(Vector3() - d); }
    
    void addChild (Object3D& o) {
        o.parent = NULL;
        this->child.emplace_back(&o);
    }
    
    void setCamera (Object3D& o) { this->camera = &o; }
    
    void clearCbuff () {
        for (int y = this->dsy; y < this->dey; ++y) {
            for (int x = this->dsx; x < this->dex; ++x) {
                this->buff->drawPixel(x, y, this->bgcolor);
            }
        }
    }
    void clearZbuff () {
        for (int y = this->dsy; y < this->dey; ++y) {
            for (int x = this->dsx; x < this->dex; ++x) {
                this->zbuff->drawPixel(x, y, 0);
            }
        }
    }
    
    void updateViewMatrix () {
        Matrix4x4 mat = Matrix4x4::identity();
        
        if (this->camera != NULL) {
            Object3D* obj = this->camera;
            while (obj != NULL) {
                mat = Matrix4x4::mul(Matrix4x4::rotMatrixR(obj->rotation), mat);
                mat = Matrix4x4::mul(Matrix4x4::moveMatrix(Vector3() - obj->position), mat);
                obj = obj->parent;
            }
        }
        
        this->view = mat;
    }
    
    void updateLightVector () {
        Matrix4x4 mat = Matrix4x4::identity();
        
        if (this->camera != NULL) {
            Object3D* obj = this->camera;
            while (obj != NULL) {
                mat = Matrix4x4::mul(Matrix4x4::rotMatrixR(obj->rotation), mat);
                obj = obj->parent;
            }
        }
        this->light_vector = Matrix4x4::mul(this->light, mat);
    }
    
    int16_t getCursorX () { return cursor_x - this->sx; }
    int16_t getCursorY () { return cursor_y - this->sy; }
    
    
    void drawTextureTXYWH (int16_t sx, int16_t sy, int16_t tex_x, int16_t tex_y, int16_t tex_w, int16_t tex_h, Texture& tex, bool flipx = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        sx += this->sx;
        sy += this->sy;
        int16_t ex = sx + tex_w;
        int16_t ey = sy + tex_h;
        
        ex = min(ex, this->dex);
        ey = min(ey, this->dey);
        int16_t isx = max(sx, this->dsx);
        int16_t isy = max(sy, this->dsy);
        int16_t itx = tex_x + (sx < this->dsx ? this->dsx - sx : 0);
        int16_t ity = tex_y + (sy < this->dsy ? this->dsy - sy : 0);
        if (flipx) {
            for (int y = isy, ty = ity * tex.width; y < ey; ++y, ty+=tex.width) {
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    const uint16_t c1555 = tex.pixels[ty + tex_x+tex_w-tx-1];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) { this->buff->drawPixel(x, y, r|g|b); }
                }
            }
        } else {
            for (int y = isy, ty = ity * tex.width; y < ey; ++y, ty+=tex.width) {
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    const uint16_t c1555 = tex.pixels[ty + tx];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) { this->buff->drawPixel(x, y, r|g|b); }
                }
            }
        }
    }
    void drawTextureTXYWHZ (int16_t sx, int16_t sy, int16_t tex_x, int16_t tex_y, int16_t tex_w, int16_t tex_h, uint16_t z, Texture& tex, bool flipx = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        sx += this->sx;
        sy += this->sy;
        int16_t ex = sx + tex_w;
        int16_t ey = sy + tex_h;
        
        ex = min(ex, this->dex);
        ey = min(ey, this->dey);
        int16_t isx = max(sx, this->dsx);
        int16_t isy = max(sy, this->dsy);
        int16_t itx = tex_x + (sx < this->dsx ? this->dsx - sx : 0);
        int16_t ity = tex_y + (sy < this->dsy ? this->dsy - sy : 0);
        if (flipx) {
            for (int y = isy, ty = ity * tex.width; y < ey; ++y, ty+=tex.width) {
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    if (this->zbuff->readPixel(x, y) > z) { continue; }
                    const uint16_t c1555 = tex.pixels[ty + tex_x+tex_w-tx-1];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) {
                        this->buff->drawPixel(x, y, r|g|b);
                        this->zbuff->drawPixel(x, y, z);
                    }
                }
            }
        } else {
            for (int y = isy, ty = ity * tex.width; y < ey; ++y, ty+=tex.width) {
                for (int x = isx, tx = itx; x < ex; ++x, ++tx) {
                    if (this->zbuff->readPixel(x, y) > z) { continue; }
                    const uint16_t c1555 = tex.pixels[ty + tx];
                    const uint16_t r = ((c1555 >> 10) & 0b11111) << 11;
                    const uint16_t g = ((c1555 >>  5) & 0b11111) <<  6;
                    const uint16_t b = ((c1555      ) & 0b11111)      ;
                    if ((c1555 >> 15 & 1) == 0) {
                        this->buff->drawPixel(x, y, r|g|b);
                        this->zbuff->drawPixel(x, y, z);
                    }
                }
            }
        }
    }
    
    
    Matrix4x4 view;
    Matrix4x4 projscreen;
    
private:
    Vector3 light;
    Vector3 light_vector;
    int16_t dsy, dsx, dey, dex;
    
    void drawChild (E512Array<Object3D*>& child, Matrix4x4 pmat, int sindex = 0, int nthread = 1) {
        for (auto&& c : child) {
            if (c->render_type == RenderType::Hide) { continue; }
            
            Matrix4x4 mat = this->worldMatrix(c, pmat);
            
            if (c->mesh != NULL) {
                if (c->render_type == RenderType::WireFrame) { this->drawWireFrameObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonColor) { this->drawPolygonColorObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonNormal) { this->drawPolygonNormalObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonTexture) { this->drawPolygonTextureObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonTextureDoubleFace) { this->drawPolygonTextureDoubleFaceObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonTexturePerspectiveCorrect) { this->drawPolygonTexturePerspectiveCorrectObject(c, mat, sindex, nthread); }
                if (c->render_type == RenderType::PolygonTexturePerspectiveCorrectDoubleFace) { this->drawPolygonTexturePerspectiveCorrectDoubleFaceObject(c, mat, sindex, nthread); }
            }
            this->drawChild(c->child, mat, sindex, nthread);
        }
    }
    
    void drawChildT (E512Array<Object3D*>& child, Matrix4x4 pmat, int sindex = 0, int nthread = 1) {
        for (auto&& c : child) {
            if (c->render_type == RenderType::Hide) { continue; }
            
            Matrix4x4 mat = this->worldMatrix(c, pmat);
            if (c->mesh != NULL) {
                if (c->render_type == RenderType::PolygonTranslucent) { this->drawPolygonTranslucentObject(c, mat, sindex, nthread); }
            }
            this->drawChildT(c->child, mat, sindex, nthread);
        }
    }
    
    bool notDraw (Vector3& v1, Vector3& v2, Vector3& v3) {
        if (v1.z < 0 && v2.z < 0 && v3.z < 0) { return true; }
        if (v1.z > 1 && v2.z > 1 && v3.z > 1) { return true; }
        if (v1.x < 0 && v2.x < 0 && v3.x < 0) { return true; }
        if (v1.x >= this->width && v2.x >= this->width && v3.x >= this->width) { return true; }
        if (v1.y < 0 && v2.y < 0 && v3.y < 0) { return true; }
        if (v1.y >= this->height && v2.y >= this->height && v3.y >= this->height) { return true; }
        return false;
    }
    bool notDraw (Vector4& v1, Vector4& v2, Vector4& v3) {
        if (v1.z <= 0 && v2.z <= 0 && v3.z <= 0) { return true; }
        if (v1.z > 1 && v2.z > 1 && v3.z > 1) { return true; }
        if (v1.x < 0 && v2.x < 0 && v3.x < 0) { return true; }
        if (v1.x >= this->width && v2.x >= this->width && v3.x >= this->width) { return true; }
        if (v1.y < 0 && v2.y < 0 && v3.y < 0) { return true; }
        if (v1.y >= this->height && v2.y >= this->height && v3.y >= this->height) { return true; }
        return false;
    }
    
    
    
    bool isBackFace (Vector3& v1, Vector3& v2, Vector3& v3) {
        return Vector3::cross(v2-v1, v3-v1).z > 0;
    }
    bool isBackFace (Vector4& v1, Vector4& v2, Vector4& v3) {
        return Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()).z > 0;
    }
    
    inline bool isDivide (Vector3& v1, Vector3& v2, Vector3& v3) {
        return v1.z < 0 || v2.z < 0 || v3.z < 0;
    }
    inline bool isDivide (Vector4& v1, Vector4& v2, Vector4& v3) {
        return v1.z < 0 || v2.z < 0 || v3.z < 0;
    }
    
    // inline bool isDivide (Vector3& v1, Vector3& v2, Vector3& v3) {
    //     return v1.z < 0 || v2.z < 0 || v3.z < 0 || v1.x < 0 || v2.x < 0 || v3.x < 0 || v1.y < 0 || v2.y < 0 || v3.y < 0 || v1.x > this->width || v2.x > this->width || v3.x > this->width || v1.y > this->height || v2.y > this->height || v3.y > this->height;
    // }
    // inline bool isDivide (Vector4& v1, Vector4& v2, Vector4& v3) {
    //     return v1.z < 0 || v2.z < 0 || v3.z < 0 || v1.x < 0 || v2.x < 0 || v3.x < 0 || v1.y < 0 || v2.y < 0 || v3.y < 0 || v1.x > this->width || v2.x > this->width || v3.x > this->width || v1.y > this->height || v2.y > this->height || v3.y > this->height;
    // }
    // inline bool notDrawZ (Vector3& v1, Vector3& v2, Vector3& v3) { return true; }
    // inline bool notDrawZ (Vector4& v1, Vector4& v2, Vector4& v3) { return true; }
    
    inline bool notDrawZ (Vector3& v1, Vector3& v2, Vector3& v3) { return v1.z < 0 || v2.z < 0 || v3.z < 0; }
    inline bool notDrawZ (Vector4& v1, Vector4& v2, Vector4& v3) { return v1.z < 0 || v2.z < 0 || v3.z < 0; }
    
    void drawWireFrameObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            this->drawWireFrame(o, Vector3(o->mesh->vertexs[f.a]), Vector3(o->mesh->vertexs[f.b]), Vector3(o->mesh->vertexs[f.c]), mat, this->maxdiv);
        }
    }
    void drawPolygonColorObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            this->drawPolygonColor(o, Vector3(o->mesh->vertexs[f.a]), Vector3(o->mesh->vertexs[f.b]), Vector3(o->mesh->vertexs[f.c]), mat, this->maxdiv);
        }
    }
    void drawPolygonNormalObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            this->drawPolygonNormal(o, Vector3(o->mesh->vertexs[f.a]), Vector3(o->mesh->vertexs[f.b]), Vector3(o->mesh->vertexs[f.c]), mat, this->maxdiv);
        }
    }
    void drawPolygonTranslucentObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            this->drawPolygonTranslucent(o, Vector3(o->mesh->vertexs[f.a]), Vector3(o->mesh->vertexs[f.b]), Vector3(o->mesh->vertexs[f.c]), mat, this->maxdiv);
        }
    }
    
    void drawWireFrame (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            this->drawWireFrame(o, ov1, v12, v31, mat, d-1);
            this->drawWireFrame(o, ov2, v23, v12, mat, d-1);
            this->drawWireFrame(o, ov3, v31, v23, mat, d-1);
            this->drawWireFrame(o, v12, v23, v31, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        
        this->drawBuffLine(v1, v2, v1.x, v1.y, v2.x, v2.y, o->color);
        this->drawBuffLine(v2, v3, v2.x, v2.y, v3.x, v3.y, o->color);
        this->drawBuffLine(v3, v1, v3.x, v3.y, v1.x, v1.y, o->color);
    }
    void drawPolygonColor (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            this->drawPolygonColor(o, ov1, v12, v31, mat, d-1);
            this->drawPolygonColor(o, ov2, v23, v12, mat, d-1);
            this->drawPolygonColor(o, ov3, v31, v23, mat, d-1);
            this->drawPolygonColor(o, v12, v23, v31, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        
        float r = (o->color >> 11) << 3;
        float g = ((o->color >> 5) & 0b111111) << 2;
        float b = (o->color & 0b11111) << 3;
        const float l = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        uint16_t color = color565(min(r*l, 255.0f), min(g*l, 255.0f), min(b*l, 255.0f));
        //uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
        this->fillTriangleColor(v1, v2, v3, color);
    }
    void drawPolygonNormal (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            this->drawPolygonNormal(o, ov1, v12, v31, mat, d-1);
            this->drawPolygonNormal(o, ov2, v23, v12, mat, d-1);
            this->drawPolygonNormal(o, ov3, v31, v23, mat, d-1);
            this->drawPolygonNormal(o, v12, v23, v31, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        
        Vector3 hn = (n * 0.5f + 0.5f) * 255.0f;
        uint16_t color = color565(hn.x, hn.y, hn.z);
        //uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
        this->fillTriangleColor(v1, v2, v3, color);
    }
    void drawPolygonTranslucent (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            this->drawPolygonTranslucent(o, ov1, v12, v31, mat, d-1);
            this->drawPolygonTranslucent(o, ov2, v23, v12, mat, d-1);
            this->drawPolygonTranslucent(o, ov3, v31, v23, mat, d-1);
            this->drawPolygonTranslucent(o, v12, v23, v31, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        
        float r = (o->color >> 11) << 3;
        float g = ((o->color >> 5) & 0b111111) << 2;
        float b = (o->color & 0b11111) << 3;
        const float l = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        uint16_t color = color565(min(r*l, 255.0f), min(g*l, 255.0f), min(b*l, 255.0f));
        uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
        this->fillTriangleTranslucent(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, z, color);
    }
    
    
    
    void drawPolygonTextureObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = Vector3(o->mesh->vertexs[f.a]);
            Vector3 v2 = Vector3(o->mesh->vertexs[f.b]);
            Vector3 v3 = Vector3(o->mesh->vertexs[f.c]);
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            this->drawPolygonTexture(o, v1, v2, v3, v1uv, v2uv, v3uv, mat, this->maxdiv);
        }
    }
    void drawPolygonTextureDoubleFaceObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = Vector3(o->mesh->vertexs[f.a]);
            Vector3 v2 = Vector3(o->mesh->vertexs[f.b]);
            Vector3 v3 = Vector3(o->mesh->vertexs[f.c]);
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            this->drawPolygonTextureDoubleFace(o, v1, v2, v3, v1uv, v2uv, v3uv, mat, this->maxdiv);
        }
    }
    
    void drawPolygonTexture (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Vector2 v1uv, Vector2 v2uv, Vector2 v3uv, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            Vector2 v12uv = (v1uv + v2uv) * 0.5;
            Vector2 v23uv = (v2uv + v3uv) * 0.5;
            Vector2 v31uv = (v3uv + v1uv) * 0.5;
            this->drawPolygonTexture(o, ov1, v12, v31, v1uv, v12uv, v31uv, mat, d-1);
            this->drawPolygonTexture(o, ov2, v23, v12, v2uv, v23uv, v12uv, mat, d-1);
            this->drawPolygonTexture(o, ov3, v31, v23, v3uv, v31uv, v23uv, mat, d-1);
            this->drawPolygonTexture(o, v12, v23, v31, v12uv, v23uv, v31uv, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        
        float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        this->fillTriangleTexture(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
    }
    void drawPolygonTextureDoubleFace (Object3D* o, Vector3 ov1, Vector3 ov2, Vector3 ov3, Vector2 v1uv, Vector2 v2uv, Vector2 v3uv, Matrix4x4 mat, int d) {
        Vector3 v1 = Matrix4x4::mul(ov1, mat);
        Vector3 v2 = Matrix4x4::mul(ov2, mat);
        Vector3 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector3 v12 = (ov1 + ov2) * 0.5;
            Vector3 v23 = (ov2 + ov3) * 0.5;
            Vector3 v31 = (ov3 + ov1) * 0.5;
            Vector2 v12uv = (v1uv + v2uv) * 0.5;
            Vector2 v23uv = (v2uv + v3uv) * 0.5;
            Vector2 v31uv = (v3uv + v1uv) * 0.5;
            this->drawPolygonTextureDoubleFace(o, ov1, v12, v31, v1uv, v12uv, v31uv, mat, d-1);
            this->drawPolygonTextureDoubleFace(o, ov2, v23, v12, v2uv, v23uv, v12uv, mat, d-1);
            this->drawPolygonTextureDoubleFace(o, ov3, v31, v23, v3uv, v31uv, v23uv, mat, d-1);
            this->drawPolygonTextureDoubleFace(o, v12, v23, v31, v12uv, v23uv, v31uv, mat, d-1);
            return;
        }
        
        float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        this->fillTriangleTexture(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
    }
    
    
    
    
    void drawPolygonTexturePerspectiveCorrectObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector4 v1 = Vector4(o->mesh->vertexs[f.a]);
            Vector4 v2 = Vector4(o->mesh->vertexs[f.b]);
            Vector4 v3 = Vector4(o->mesh->vertexs[f.c]);
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            this->drawPolygonTexturePerspectiveCorrect(o, v1, v2, v3, v1uv, v2uv, v3uv, mat, this->maxdiv);
        }
    }
    void drawPolygonTexturePerspectiveCorrectDoubleFaceObject (Object3D* o, Matrix4x4 mat, int sindex = 0, int nthread = 1) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = sindex; i < o->mesh->faces.size(); i += nthread) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector4 v1 = Vector4(o->mesh->vertexs[f.a]);
            Vector4 v2 = Vector4(o->mesh->vertexs[f.b]);
            Vector4 v3 = Vector4(o->mesh->vertexs[f.c]);
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            this->drawPolygonTexturePerspectiveCorrectDoubleFace(o, v1, v2, v3, v1uv, v2uv, v3uv, mat, this->maxdiv);
        }
    }
    
    void drawPolygonTexturePerspectiveCorrect (Object3D* o, Vector4 ov1, Vector4 ov2, Vector4 ov3, Vector2 v1uv, Vector2 v2uv, Vector2 v3uv, Matrix4x4 mat, int d) {
        Vector4 v1 = Matrix4x4::mul(ov1, mat);
        Vector4 v2 = Matrix4x4::mul(ov2, mat);
        Vector4 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector4 v12 = (ov1 + ov2) * 0.5;
            Vector4 v23 = (ov2 + ov3) * 0.5;
            Vector4 v31 = (ov3 + ov1) * 0.5;
            Vector2 v12uv = (v1uv + v2uv) * 0.5;
            Vector2 v23uv = (v2uv + v3uv) * 0.5;
            Vector2 v31uv = (v3uv + v1uv) * 0.5;
            this->drawPolygonTexturePerspectiveCorrect(o, ov1, v12, v31, v1uv, v12uv, v31uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrect(o, ov2, v23, v12, v2uv, v23uv, v12uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrect(o, ov3, v31, v23, v3uv, v31uv, v23uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrect(o, v12, v23, v31, v12uv, v23uv, v31uv, mat, d-1);
            return;
        }
        if (this->isBackFace(v1, v2, v3)) { return; }
        v1uv = v1uv / v1.w;
        v2uv = v2uv / v2.w;
        v3uv = v3uv / v3.w;
        v1.w = 1.0 / v1.w;
        v2.w = 1.0 / v2.w;
        v3.w = 1.0 / v3.w;
        float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        this->fillTriangleTexturePerspectiveCorrect(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
    }
    void drawPolygonTexturePerspectiveCorrectDoubleFace (Object3D* o, Vector4 ov1, Vector4 ov2, Vector4 ov3, Vector2 v1uv, Vector2 v2uv, Vector2 v3uv, Matrix4x4 mat, int d) {
        Vector4 v1 = Matrix4x4::mul(ov1, mat);
        Vector4 v2 = Matrix4x4::mul(ov2, mat);
        Vector4 v3 = Matrix4x4::mul(ov3, mat);
        const Vector3 n = Vector3::normalize(Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()));
        v1 = Matrix4x4::muld(v1, this->projscreen);
        v2 = Matrix4x4::muld(v2, this->projscreen);
        v3 = Matrix4x4::muld(v3, this->projscreen);
        if (this->notDraw(v1, v2, v3)) { return; }
        if (this->isDivide(v1, v2, v3)) {
            if (d < 1) { return; }
            Vector4 v12 = (ov1 + ov2) * 0.5;
            Vector4 v23 = (ov2 + ov3) * 0.5;
            Vector4 v31 = (ov3 + ov1) * 0.5;
            Vector2 v12uv = (v1uv + v2uv) * 0.5;
            Vector2 v23uv = (v2uv + v3uv) * 0.5;
            Vector2 v31uv = (v3uv + v1uv) * 0.5;
            this->drawPolygonTexturePerspectiveCorrectDoubleFace(o, ov1, v12, v31, v1uv, v12uv, v31uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrectDoubleFace(o, ov2, v23, v12, v2uv, v23uv, v12uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrectDoubleFace(o, ov3, v31, v23, v3uv, v31uv, v23uv, mat, d-1);
            this->drawPolygonTexturePerspectiveCorrectDoubleFace(o, v12, v23, v31, v12uv, v23uv, v31uv, mat, d-1);
            return;
        }
        v1uv = v1uv / v1.w;
        v2uv = v2uv / v2.w;
        v3uv = v3uv / v3.w;
        v1.w = 1.0 / v1.w;
        v2.w = 1.0 / v2.w;
        v3.w = 1.0 / v3.w;
        float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        this->fillTriangleTexturePerspectiveCorrect(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
    }
    
    
    Matrix4x4 worldMatrix (Object3D* o, Matrix4x4 pmat) {
        Matrix4x4 mat = Matrix4x4::identity();
        mat = Matrix4x4::mul(mat, Matrix4x4::scaleMatrix(o->scale));
        mat = Matrix4x4::mul(mat, Matrix4x4::rotMatrix(o->rotation));
        mat = Matrix4x4::mul(mat, Matrix4x4::moveMatrix(o->position));
        mat = Matrix4x4::mul(mat, pmat);
        return mat;
    }
    
    inline bool inSide (const int& x, const int& y) { return !(x < 0 || x >= this->width || y < 0 || y >= this->height); }
    
    inline bool inSide2 (int x, int y) {////
        x += this->sx;
        y += this->sy;
        return !(x < this->dsx || x >= this->dex || y < this->dsy || y >= this->dey);
    }
    inline void swap (int16_t& a, int16_t& b) { int16_t c = a; a = b; b = c; }
    inline float distance (const float& ax, const float& ay, const float& bx, const float& by) {
        return sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    }
    
/*
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
    // Adafruit_GFX::drawLine modification
    inline void drawBuffLine (int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
        if (abs(y2 - y1) > abs(x2 - x1)) {
            this->swap(x1, y1); this->swap(x2, y2);
            if (x1 > x2) { this->swap(x1, x2); this->swap(y1, y2); }
            int16_t dx = x2 - x1;
            int16_t dy = abs(y2 - y1);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y1 < y2 ? 1 : -1;
            for (; x1<=x2; ++x1) {
                if (this->inSide2(y1, x1)) { this->buff->drawPixel(y1+this->sx, x1+this->sy, color); }
                err -= dy;
                if (err < 0) {
                    y1 += ystep;
                    err += dx;
                }
            }
        } else {
            if (x1 > x2) { this->swap(x1, x2); this->swap(y1, y2); }
            int16_t dx = x2 - x1;
            int16_t dy = abs(y2 - y1);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y1 < y2 ? 1 : -1;
            for (; x1<=x2; ++x1) {
                if (this->inSide2(x1, y1)) { this->buff->drawPixel(x1+this->sx, y1+this->sy, color); }
                err -= dy;
                if (err < 0) {
                    y1 += ystep;
                    err += dx;
                }
            }
        }
    }
    
    // Adafruit_GFX::drawLine modification
    inline void drawBuffLine (Vector3& v1, Vector3& v2, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
        
        float d12 = max(this->distance(v1.x, v1.y, v2.x, v2.y), 1.0f);
        
        if (abs(y2 - y1) > abs(x2 - x1)) {
            this->swap(x1, y1); this->swap(x2, y2);
            if (x1 > x2) { this->swap(x1, x2); this->swap(y1, y2); }
            int16_t dx = x2 - x1;
            int16_t dy = abs(y2 - y1);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y1 < y2 ? 1 : -1;
            for (; x1<=x2; ++x1) {
                float d = this->distance(v2.x, v2.y, y1, x1) / d12;
                float fz = v1.z * d + v2.z * (1.0f-d);
                uint16_t z = (1.0f-fz) * 32767;
                if (this->inSide2(y1, x1) && z >= this->zbuff->readPixel(y1+this->sx, x1+this->sy)) {
                    this->buff->drawPixel(y1+this->sx, x1+this->sy, color);
                    this->zbuff->drawPixel(y1+this->sx, x1+this->sy, z);
                }
                err -= dy;
                if (err < 0) {
                    y1 += ystep;
                    err += dx;
                }
            }
        } else {
            if (x1 > x2) { this->swap(x1, x2); this->swap(y1, y2); }
            int16_t dx = x2 - x1;
            int16_t dy = abs(y2 - y1);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y1 < y2 ? 1 : -1;
            for (; x1<=x2; ++x1) {
                float d = this->distance(v2.x, v2.y, x1, y1) / d12;
                float fz = v1.z * d + v2.z * (1.0f-d);
                uint16_t z = (1.0f-fz) * 32767;
                if (this->inSide2(x1, y1) && z >= this->zbuff->readPixel(x1+this->sx, y1+this->sy)) {
                    this->buff->drawPixel(x1+this->sx, y1+this->sy, color);
                    this->zbuff->drawPixel(x1+this->sx, y1+this->sy, z);
                }
                err -= dy;
                if (err < 0) {
                    y1 += ystep;
                    err += dx;
                }
            }
        }
    }
    
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineH (int16_t sx, int16_t y, int16_t w, int16_t z, uint16_t color) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                this->zbuff->drawPixel(x+this->sx, y+this->sy, z);
                this->buff->drawPixel(x+this->sx, y+this->sy, color);
            }
            
        }
    }
    
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineHTranslucent (int16_t sx, int16_t y, int16_t w, int16_t z, uint16_t color) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                uint16_t color2 = this->buff->readPixel(x+this->sx, y+this->sy);
                
                uint16_t r = ((((color >> 11) &  0b11111) << 3) + (((color2 >> 11) &  0b11111) << 3)) >> 1;
                uint16_t g = ((((color >> 5)  & 0b111111) << 2) + (((color2 >> 5)  & 0b111111) << 2)) >> 1;
                uint16_t b = ((( color        &  0b11111) << 3) + (( color2        &  0b11111) << 3)) >> 1;
                
                this->buff->drawPixel(x+this->sx, y+this->sy, color565(r, g, b));
            }
        }
    }
    
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineHTexture (int16_t sx, int16_t y, int16_t w, int16_t za, int16_t zba, int16_t zca, Vector3& v1, Vector3& v2, Vector3& v3, Vector2& v1uv, Vector2& v2uv, Vector2& v3uv, float light, Object3D* o) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        
        const int16_t v1x = v1.x;
        const int16_t v1y = v1.y;
        const int16_t v2x = v2.x;
        const int16_t v2y = v2.y;
        const int16_t v3x = v3.x;
        const int16_t v3y = v3.y;
        
        float dbx = v2uv.x-v1uv.x;
        float dby = v2uv.y-v1uv.y;
        float dcx = v3uv.x-v1uv.x;
        float dcy = v3uv.y-v1uv.y;
        
        float ab = 0;
        float ac = 0;
        float t = abs((v2x-v1x) * (v3y-v1y) - (v2y-v1y) * (v3x-v1x));
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            
            if (t > 0) {
                ab = abs((v1x-x) * (v2y-y) - (v1y-y) * (v2x-x)) / t;
                ac = abs((v1x-x) * (v3y-y) - (v1y-y) * (v3x-x)) / t;
            }
            
            float tu = dbx*ac+dcx*ab+v1uv.x;
            float tv = dby*ac+dcy*ab+v1uv.y;
            uint16_t z = zba*ac+zca*ab+za;
            
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                uint16_t color = o->texture->getColor(tu, tv);
                if ((color >> 15) & 1) { continue; }
                float r = ((color >> 10) & 0b11111) << 3;
                float g = ((color >> 5)  & 0b11111) << 3;
                float b = ( color        & 0b11111) << 3;
                
                color = color565(min(r*light, 255.0f), min(g*light, 255.0f), min(b*light, 255.0f));
                this->zbuff->drawPixel(x+this->sx, y+this->sy, z);
                this->buff->drawPixel(x+this->sx, y+this->sy, color);
            }
        }
    }
    
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineHTexturePerspectiveCorrect (int16_t sx, int16_t y, int16_t w, int16_t za, int16_t zba, int16_t zca, Vector4& v1, Vector4& v2, Vector4& v3, Vector2& v1uv, Vector2& v2uv, Vector2& v3uv, float light, Object3D* o) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        const int16_t v1x = v1.x;
        const int16_t v1y = v1.y;
        const int16_t v2x = v2.x;
        const int16_t v2y = v2.y;
        const int16_t v3x = v3.x;
        const int16_t v3y = v3.y;
        
        float dbx = v2uv.x-v1uv.x;
        float dby = v2uv.y-v1uv.y;
        float dbz = v2.w-v1.w;
        float dcx = v3uv.x-v1uv.x;
        float dcy = v3uv.y-v1uv.y;
        float dcz = v3.w-v1.w;
        
        float ab = 0;
        float ac = 0;
        float t = abs((v2x-v1x) * (v3y-v1y) - (v2y-v1y) * (v3x-v1x));
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            
            if (t > 0) {
                ab = abs((v1x-x) * (v2y-y) - (v1y-y) * (v2x-x)) / t;
                ac = abs((v1x-x) * (v3y-y) - (v1y-y) * (v3x-x)) / t;
            }
            
            float tu = dbx*ac+dcx*ab+v1uv.x;
            float tv = dby*ac+dcy*ab+v1uv.y;
            float tw = dbz*ac+dcz*ab+v1.w;
            
            uint16_t z = zba*ac+zca*ab+za;
            
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                uint16_t color = o->texture->getColor(tu/tw, tv/tw);
                
                if ((color >> 15) & 1) { continue; }
                float r = ((color >> 10) & 0b11111) << 3;
                float g = ((color >> 5)  & 0b11111) << 3;
                float b = ( color        & 0b11111) << 3;
                
                color = color565(min(r*light, 255.0f), min(g*light, 255.0f), min(b*light, 255.0f));
                this->zbuff->drawPixel(x+this->sx, y+this->sy, z);
                this->buff->drawPixel(x+this->sx, y+this->sy, color);
            }
        }
    }
    
    
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleColor (int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t z, uint16_t color) {
        
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        if (y2 > y3) { this->swap(y3, y2); this->swap(x3, x2); }
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        
        int16_t a, b, y;
        if (y1 == y3) {
            a = b = x1;
            if (x2 < a) {
                a = x2;
            } else if(x2 > b) {
                b = x2;
            }
            if (x3 < a) {
                a = x3;
            } else if (x3 > b) {
                b = x3;
            }
            this->drawBuffLineH(a, y1, b-a+1, z, color);
            return;
        }
        
        int32_t dx21 = x2 - x1;
        int32_t dy21 = y2 - y1;
        int32_t dx31 = x3 - x1;
        int32_t dy31 = y3 - y1;
        int32_t dx32 = x3 - x2;
        int32_t dy32 = y3 - y2;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y2 == y3 ? y2 : y2-1;
        
        for(y=y1; y<=last; ++y) {
            a = x1 + sa / dy21;
            b = x1 + sb / dy31;
            sa += dx21;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineH(a, y, b-a+1, z, color);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineH(a, y, b-a+1, z, color);
        }
    }
    
    
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleColor (Vector3& v1, Vector3& v2, Vector3& v3, uint16_t color) {
        int16_t x1 = v1.x;
        int16_t y1 = v1.y;
        int16_t x2 = v2.x;
        int16_t y2 = v2.y;
        int16_t x3 = v3.x;
        int16_t y3 = v3.y;
        
        int16_t za = (1.0f-v1.z) * 32767;
        int16_t zb = (1.0f-v2.z) * 32767;
        int16_t zc = (1.0f-v3.z) * 32767;
        int16_t zba = zb - za;
        int16_t zca = zc - za;
        
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        if (y2 > y3) { this->swap(y3, y2); this->swap(x3, x2); }
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        
        int16_t a, b, y;
        if (y1 == y3) {
            a = b = x1;
            if (x2 < a) {
                a = x2;
            } else if(x2 > b) {
                b = x2;
            }
            if (x3 < a) {
                a = x3;
            } else if (x3 > b) {
                b = x3;
            }
            this->drawBuffLineHColor(a, y1, b-a+1, za, zba, zca, v1, v2, v3, color);
            return;
        }
        
        int32_t dx21 = x2 - x1;
        int32_t dy21 = y2 - y1;
        int32_t dx31 = x3 - x1;
        int32_t dy31 = y3 - y1;
        int32_t dx32 = x3 - x2;
        int32_t dy32 = y3 - y2;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y2 == y3 ? y2 : y2-1;
        
        for(y=y1; y<=last; ++y) {
            a = x1 + sa / dy21;
            b = x1 + sb / dy31;
            sa += dx21;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHColor(a, y, b-a+1, za, zba, zca, v1, v2, v3, color);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHColor(a, y, b-a+1, za, zba, zca, v1, v2, v3, color);
        }
    }
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineHColor (int16_t sx, int16_t y, int16_t w, int16_t za, int16_t zba, int16_t zca, Vector3& v1, Vector3& v2, Vector3& v3, uint16_t color) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        
        const int16_t v1x = v1.x;
        const int16_t v1y = v1.y;
        const int16_t v2x = v2.x;
        const int16_t v2y = v2.y;
        const int16_t v3x = v3.x;
        const int16_t v3y = v3.y;
        
        float ab = 0;
        float ac = 0;
        float t = abs((v2x-v1x) * (v3y-v1y) - (v2y-v1y) * (v3x-v1x));
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            
            if (t > 0) {
                ab = abs((v1x-x) * (v2y-y) - (v1y-y) * (v2x-x)) / t;
                ac = abs((v1x-x) * (v3y-y) - (v1y-y) * (v3x-x)) / t;
            }
            
            uint16_t z = zba*ac+zca*ab+za;
            
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                this->zbuff->drawPixel(x+this->sx, y+this->sy, z);
                this->buff->drawPixel(x+this->sx, y+this->sy, color);
            }
        }
    }
    
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleTranslucent (int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t z, uint16_t color) {
        
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        if (y2 > y3) { this->swap(y3, y2); this->swap(x3, x2); }
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        
        int16_t a, b, y;
        if (y1 == y3) {
            a = b = x1;
            if (x2 < a) {
                a = x2;
            } else if(x2 > b) {
                b = x2;
            }
            if (x3 < a) {
                a = x3;
            } else if (x3 > b) {
                b = x3;
            }
            this->drawBuffLineHTranslucent(a, y1, b-a+1, z, color);
            return;
        }
        
        int32_t dx21 = x2 - x1;
        int32_t dy21 = y2 - y1;
        int32_t dx31 = x3 - x1;
        int32_t dy31 = y3 - y1;
        int32_t dx32 = x3 - x2;
        int32_t dy32 = y3 - y2;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y2 == y3 ? y2 : y2-1;
        
        for(y=y1; y<=last; ++y) {
            a = x1 + sa / dy21;
            b = x1 + sb / dy31;
            sa += dx21;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTranslucent(a, y, b-a+1, z, color);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTranslucent(a, y, b-a+1, z, color);
        }
    }
    
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleTexture (Vector3& v1, Vector3& v2, Vector3& v3, Vector2& v1uv, Vector2& v2uv, Vector2& v3uv, float light, Object3D* o) {
        int16_t x1 = v1.x;
        int16_t y1 = v1.y;
        int16_t x2 = v2.x;
        int16_t y2 = v2.y;
        int16_t x3 = v3.x;
        int16_t y3 = v3.y;
        
        int16_t za = (1.0f-v1.z) * 32767;
        int16_t zb = (1.0f-v2.z) * 32767;
        int16_t zc = (1.0f-v3.z) * 32767;
        int16_t zba = zb - za;
        int16_t zca = zc - za;
        
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        if (y2 > y3) { this->swap(y3, y2); this->swap(x3, x2); }
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        
        int16_t a, b, y;
        if (y1 == y3) {
            a = b = x1;
            if (x2 < a) {
                a = x2;
            } else if(x2 > b) {
                b = x2;
            }
            if (x3 < a) {
                a = x3;
            } else if (x3 > b) {
                b = x3;
            }
            this->drawBuffLineHTexture(a, y1, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
            return;
        }
        
        int32_t dx21 = x2 - x1;
        int32_t dy21 = y2 - y1;
        int32_t dx31 = x3 - x1;
        int32_t dy31 = y3 - y1;
        int32_t dx32 = x3 - x2;
        int32_t dy32 = y3 - y2;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y2 == y3 ? y2 : y2-1;
        
        for(y=y1; y<=last; ++y) {
            a = x1 + sa / dy21;
            b = x1 + sb / dy31;
            sa += dx21;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTexture(a, y, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTexture(a, y, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
    }
    
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleTexturePerspectiveCorrect (Vector4& v1, Vector4& v2, Vector4& v3, Vector2& v1uv, Vector2& v2uv, Vector2& v3uv, float light, Object3D* o) {
        int16_t x1 = v1.x;
        int16_t y1 = v1.y;
        int16_t x2 = v2.x;
        int16_t y2 = v2.y;
        int16_t x3 = v3.x;
        int16_t y3 = v3.y;
        
        int16_t za = (1.0f-v1.z) * 32767;
        int16_t zb = (1.0f-v2.z) * 32767;
        int16_t zc = (1.0f-v3.z) * 32767;
        int16_t zba = zb - za;
        int16_t zca = zc - za;
        
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        if (y2 > y3) { this->swap(y3, y2); this->swap(x3, x2); }
        if (y1 > y2) { this->swap(y1, y2); this->swap(x1, x2); }
        
        int16_t a, b, y;
        if (y1 == y3) {
            a = b = x1;
            if (x2 < a) {
                a = x2;
            } else if(x2 > b) {
                b = x2;
            }
            if (x3 < a) {
                a = x3;
            } else if (x3 > b) {
                b = x3;
            }
            this->drawBuffLineHTexturePerspectiveCorrect(a, y1, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
            return;
        }
        
        int32_t dx21 = x2 - x1;
        int32_t dy21 = y2 - y1;
        int32_t dx31 = x3 - x1;
        int32_t dy31 = y3 - y1;
        int32_t dx32 = x3 - x2;
        int32_t dy32 = y3 - y2;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y2 == y3 ? y2 : y2-1;
        
        for(y=y1; y<=last; ++y) {
            a = x1 + sa / dy21;
            b = x1 + sb / dy31;
            sa += dx21;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTexturePerspectiveCorrect(a, y, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTexturePerspectiveCorrect(a, y, b-a+1, za, zba, zca, v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
    }
    
    
    
};

class E512W3DWindowManager {
public:
    E512Font* font = new DefaultFont12x6();
    E512W3DWindow* ws[32];
    uint16_t wsize = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    
    TFT_eSprite* tft_es_buff;
    TFT_eSprite* zbuff;
    
    uint16_t fixed_milli_time = 33;
    E512W3DWindowManager () {}
    
    void begin () {
        this->tft_es_buff = new TFT_eSprite(&M5.Lcd);
        this->tft_es_buff->setColorDepth(16);
        this->tft_es_buff->createSprite(this->width, this->height);
        this->zbuff = new TFT_eSprite(&M5.Lcd);
        this->zbuff->setColorDepth(16);
        this->zbuff->createSprite(this->width, this->height);
        for (int i = 0; i < this->wsize; ++i) {
            this->ws[i]->buff = this->tft_es_buff;
            this->ws[i]->zbuff = this->zbuff;
            this->ws[i]->screen_width = this->width;
            this->ws[i]->screen_height = this->height;
            this->ws[i]->font = this->font;
        }
        
        E512W3DInput::width = this->width;
        E512W3DInput::height = this->height;
    }
    
    void setFont (E512Font* font) {
        this->font = font;
        for (int i = 0; i < this->wsize; ++i) {
            this->ws[i]->font = this->font;
        }
    }
    
    void add (E512W3DWindow& w) {
        if (this->wsize < 32) {
            this->ws[this->wsize] = &w;
            this->wsize += 1;
        }
    }
    
    void fixedWait () {
        while (!this->isFixedTime()) { delay(1); }
    }
    
    void fixedDrawWait () {
        this->fixedWait();
        this->draw();
    }
    
    void fixedDraw () {
        if (this->isFixedTime()) { this->draw(); }
    }
    
    void draw () {
        this->clear();
        this->allWindowDraw();
        this->pushScreen();
    }
    
    bool isFixedTime () {
        return millis() - this->prev_time >= this->fixed_milli_time - this->over_time;
    }
    void clear (uint16_t color = 0) {
        this->over_time = (millis() - this->prev_time) - (this->fixed_milli_time - this->over_time);
        this->over_time = min(this->over_time, this->fixed_milli_time);
        this->prev_time = millis();
        this->colorBufferClear(color);
    }
    void pushScreen () {
        this->tft_es_buff->pushSprite(0, 0);
    }
    uint64_t prev_time = 0;
private:
    uint16_t over_time = 0;
    void colorBufferClear (uint16_t color = 0) { this->tft_es_buff->fillSprite(color); }
    
    void allWindowDraw () {
        for (int i = 0; i < this->wsize; ++i) {
            E512W3DWindow& w = *this->ws[i];
            w.draw();
        }
    }
    
};







