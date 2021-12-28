#pragma once
#include "E512W3DUtils.hpp"

class E512W3DWindow {
public:
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
    
    
    void draw () {
        this->dsy = max(this->sy, (int16_t)0);
        this->dsx = max(this->sx, (int16_t)0);
        this->dey = min(this->sy + this->height, this->screen_height);
        this->dex = min(this->sx + this->width, this->screen_width);
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        this->clear();
        this->updateViewMatrix();
        this->updateLightVector();
        if (this->isortho) {
            this->projescreen = Matrix4x4::orthoscreenMatrix(this->width, this->height, this->ortho_size);
        } else {
            this->projescreen = Matrix4x4::projscreenMatrix(this->width, this->height);
        }
        this->drawChild(this->child, Matrix4x4::identity());
    }
    
    void setDirectionalLight (float x, float y, float z) { this->setDirectionalLight(Vector3(x, y, z)); }
    void setDirectionalLight (Vector3 d) { this->light = Vector3::normalize(Vector3() - d); }
    
    void addChild (Object3D& o) {
        o.parent = NULL;
        this->child.emplace_back(&o);
    }
    
    void setCamera (Object3D& o) { this->camera = &o; }
private:
    Object3D* camera = NULL;
    E512Array<Object3D*> child;
    Vector3 light;
    Vector3 light_vector;
    Matrix4x4 view;
    Matrix4x4 projescreen;
    int16_t dsy, dsx, dey, dex;
    
    void drawChild (E512Array<Object3D*>& child, Matrix4x4 pmat) {
        for (auto&& c : child) {
            if (c->render_type == RenderType::Hide) { continue; }
            
            Matrix4x4 mat = this->worldMatrix(c, pmat);
            if (c->mesh != NULL) {
                E512Array<Vector3> v = c->mesh->vertexs;
                if (c->render_type == RenderType::WireFrame) {
                    this->worldviewTransform(c, mat, v);
                    this->projscreenTransform(c, v);
                    this->drawWire(c, v);
                }
                if (c->render_type == RenderType::PolygonColor) {
                    E512Array<uint16_t> colors(c->mesh->faces.size());
                    this->worldviewTransform(c, mat, v);
                    this->polygon(c, v, colors);
                    this->projscreenTransform(c, v);
                    this->drawPolygon(c, v, colors);
                }
                if (c->render_type == RenderType::PolygonNormal) {
                    E512Array<uint16_t> colors(c->mesh->faces.size());
                    this->worldviewTransform(c, mat, v);
                    this->polygonNormal(c, v, colors);
                    this->projscreenTransform(c, v);
                    this->drawPolygon(c, v, colors);
                }
                if (c->render_type == RenderType::PolygonTexture) {
                    E512Array<float> lights(c->mesh->faces.size());
                    this->worldviewTransform(c, mat, v);
                    this->polygonTexture(c, v, lights);
                    this->projscreenTransform(c, v);
                    this->drawPolygonTexture(c, v, lights);
                }
            }
            this->drawChild(c->child, mat);
        }
    }
    
    void polygonTexture (Object3D* o, E512Array<Vector3>& v, E512Array<float>& lights) {
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v0 = v[f.a];
            const Vector3& v1 = v[f.b];
            const Vector3& v2 = v[f.c];
            const Vector3 n = Vector3::normalize(Vector3::cross(v1-v0, v2-v0));
            lights[i] = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
        }
    }
    
    void polygon (Object3D* o, E512Array<Vector3>& v, E512Array<uint16_t>& colors) {
        float r = (o->color >> 11) << 3;
        float g = ((o->color >> 5) & 0b111111) << 2;
        float b = (o->color & 0b11111) << 3;
        
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v0 = v[f.a];
            const Vector3& v1 = v[f.b];
            const Vector3& v2 = v[f.c];
            const Vector3 n = Vector3::normalize(Vector3::cross(v1-v0, v2-v0));
            const float d = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            colors[i] = color565(min(r*d, 255.0f), min(g*d, 255.0f), min(b*d, 255.0f));
        }
    }
    
    void polygonNormal (Object3D* o, E512Array<Vector3>& v, E512Array<uint16_t>& colors) {
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v0 = v[f.a];
            const Vector3& v1 = v[f.b];
            const Vector3& v2 = v[f.c];
            const Vector3 n = (Vector3::normalize(Vector3::cross(v1-v0, v2-v0)) * 0.5f + 0.5f) * 255.0f;
            colors[i] = color565(n.x, n.y, n.z);
        }
    }
    
    void projscreenTransform (Object3D* o, E512Array<Vector3>& v) {
        for (int i = 0; i < v.size(); ++i) {
            v[i] = Matrix4x4::mul(v[i], this->projescreen);
        }
    }
    
    Matrix4x4 worldMatrix (Object3D* o, Matrix4x4 pmat) {
        Matrix4x4 mat = Matrix4x4::identity();
        mat = Matrix4x4::mul(mat, Matrix4x4::scaleMatrix(o->scale));
        mat = Matrix4x4::mul(mat, Matrix4x4::rotMatrix(o->rotation));
        mat = Matrix4x4::mul(mat, Matrix4x4::moveMatrix(o->position));
        mat = Matrix4x4::mul(mat, pmat);
        return mat;
    }
    
    void updateViewMatrix () {
        Matrix4x4 mat = Matrix4x4::identity();
        
        if (this->camera != NULL) {
            Object3D* obj = this->camera;
            while (obj != NULL) {
                mat = Matrix4x4::mul(Matrix4x4::rotMatrix(Vector3() - obj->rotation), mat);
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
                mat = Matrix4x4::mul(Matrix4x4::rotMatrix(Vector3() - obj->rotation), mat);
                obj = obj->parent;
            }
        }
        this->light_vector = Matrix4x4::mul(this->light, mat);
    }
    
    void worldviewTransform (Object3D* o, Matrix4x4 mat, E512Array<Vector3>& v) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < v.size(); ++i) {
            v[i] = Matrix4x4::mul(v[i], mat);
        }
    }
    
    void clear () {
        for (int y = this->dsy; y < this->dey; ++y) {
            for (int x = this->dsx; x < this->dex; ++x) {
                this->buff->drawPixel(x, y, this->bgcolor);
                this->zbuff->drawPixel(x, y, 0);
                // this->buff[y*this->width+x] = this->bgcolor;
                // this->zbuff[y*this->width+x] = 0;
            }
        }
    }
    
    void drawWire (Object3D* o, E512Array<Vector3>& v) {
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v1 = v[f.a];
            const Vector3& v2 = v[f.b];
            const Vector3& v3 = v[f.c];
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            this->drawBuffLine(v1.x, v1.y, v2.x, v2.y, o->color);
            this->drawBuffLine(v2.x, v2.y, v3.x, v3.y, o->color);
            this->drawBuffLine(v3.x, v3.y, v1.x, v1.y, o->color);
        }
    }
    
    void drawPolygon (Object3D* o, E512Array<Vector3>& v, E512Array<uint16_t>& colors) {
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v1 = v[f.a];
            const Vector3& v2 = v[f.b];
            const Vector3& v3 = v[f.c];
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
            this->fillTriangle(o, v, i, z, colors[i]);
        }
    }
    
    void drawPolygonTexture (Object3D* o, E512Array<Vector3>& v, E512Array<float>& lights) {
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Vector3& v1 = v[f.a];
            const Vector3& v2 = v[f.b];
            const Vector3& v3 = v[f.c];
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            this->fillTriangleTX(o, v, i, lights[i]);
        }
    }
    
    inline bool inSide (const int& x, const int& y) { return !(x < 0 || x >= this->width || y < 0 || y >= this->height); }
    
    inline bool inSide2 (int x, int y) {////
        x += this->sx;
        y += this->sy;
        return !(x < this->dsx || x >= this->dex || y < this->dsy || y >= this->dey);
    }
    inline void swap (int16_t& a, int16_t& b) { int16_t c = a; a = b; b = c; }
    
    inline void getUV (float x, float y, float x1, float y1, float x2, float y2, float x3, float y3, float& u, float& v) {
        float t = abs((x2-x1) * (y3-y1) - (y2-y1) * (x3-x1));
        u = 0;
        v = 0;
        if (t > 0) {
            u = abs((x1-x) * (y2-y) - (y1-y) * (x2-x)) / t;
            v = abs((x1-x) * (y3-y) - (y1-y) * (x3-x)) / t;
        }
    }
    
    
/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).
Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
Copyright (c) 2013 Adafruit Industries.  All rights reserved.
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
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangle (Object3D* o, E512Array<Vector3>& v, int16_t index, uint16_t z, uint16_t color) {
        const Face& f = o->mesh->faces[index];
        
        int16_t x1 = v[f.a].x;
        int16_t y1 = v[f.a].y;
        int16_t x2 = v[f.b].x;
        int16_t y2 = v[f.b].y;
        int16_t x3 = v[f.c].x;
        int16_t y3 = v[f.c].y;
        
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
    
    // GFXcanvas8::writeFastHLine modification
    inline void drawBuffLineHTX (int16_t sx, int16_t y, int16_t w, int16_t za,int16_t zba, int16_t zca, Object3D* o, E512Array<Vector3>& v, int16_t index, float light) {
        if (sx >= this->width || y < 0 || y >= this->height) { return; }
        int16_t ex = sx + w - 1;
        if (ex < 0) { return; }
        sx = max(sx, (int16_t)0);
        ex = min(ex, (int16_t)(this->width - 1));
        
        
        const Face& f = o->mesh->faces[index];
        const int16_t v1x = v[f.a].x;
        const int16_t v1y = v[f.a].y;
        const int16_t v2x = v[f.b].x;
        const int16_t v2y = v[f.b].y;
        const int16_t v3x = v[f.c].x;
        const int16_t v3y = v[f.c].y;
        
        
        const Face& fuv = o->mesh->uv_faces[index];
        float ax = o->mesh->uv_vertexs[fuv.a].x;
        float ay = o->mesh->uv_vertexs[fuv.a].y;
        float bx = o->mesh->uv_vertexs[fuv.b].x;
        float by = o->mesh->uv_vertexs[fuv.b].y;
        float cx = o->mesh->uv_vertexs[fuv.c].x;
        float cy = o->mesh->uv_vertexs[fuv.c].y;
        float dax = bx-ax;
        float day = by-ay;
        float dbx = cx-ax;
        float dby = cy-ay;
        
        for (int16_t x = sx; x < ex; ++x) {
            if (!this->inSide2(x, y)) { continue; }
            
            float u=0;
            float v=0;
            this->getUV(x, y, v1x, v1y, v2x, v2y, v3x, v3y, u, v);
            
            float tu = dax*v+dbx*u+ax;
            float tv = day*v+dby*u+ay;
            
            uint16_t z = zba*v+zca*u+za;
            
            if (z > this->zbuff->readPixel(x+this->sx, y+this->sy)) {
                this->zbuff->drawPixel(x+this->sx, y+this->sy, z);
                
                uint16_t color = o->texture->getColor(tu, tv);
                float r = (color >> 11) << 3;
                float g = ((color >> 5) & 0b111111) << 2;
                float b = (color & 0b11111) << 3;
                
                color = color565(min(r*light, 255.0f), min(g*light, 255.0f), min(b*light, 255.0f));
                
                this->buff->drawPixel(x+this->sx, y+this->sy, color);
            }
        }
    }
    // Adafruit_GFX::fillTriangle modification
    inline void fillTriangleTX (Object3D* o, E512Array<Vector3>& v, int16_t index, float light) {
        const Face& f = o->mesh->faces[index];
        
        int16_t x1 = v[f.a].x;
        int16_t y1 = v[f.a].y;
        int16_t x2 = v[f.b].x;
        int16_t y2 = v[f.b].y;
        int16_t x3 = v[f.c].x;
        int16_t y3 = v[f.c].y;
        
        int16_t za = (1.0f-v[f.a].z) * 32767;
        int16_t zb = (1.0f-v[f.b].z) * 32767;
        int16_t zc = (1.0f-v[f.c].z) * 32767;
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
            this->drawBuffLineHTX(a, y1, b-a+1, za, zba, zca, o, v, index, light);
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
            this->drawBuffLineHTX(a, y, b-a+1, za, zba, zca, o, v, index, light);
        }
        
        sa = dx32 * (y - y2);
        sb = dx31 * (y - y1);
        for(; y<=y3; ++y) {
            a = x2 + sa / dy32;
            b = x1 + sb / dy31;
            sa += dx32;
            sb += dx31;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineHTX(a, y, b-a+1, za, zba, zca, o, v, index, light);
        }
    }
};

class E512W3DWindowManager {
public:
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
        }
    }
    
    void add (E512W3DWindow& w) {
        if (this->wsize < 32) {
            this->ws[this->wsize] = &w;
            this->wsize += 1;
        }
    }
    
    void fixedDrawWait () {
        while (millis() - this->prev_time < this->fixed_milli_time) { delay(1); }
        this->prev_time = millis();
        this->buffUpdate();
        this->screenDraw();
    }
    
    void fixedDraw () {
        uint64_t t = millis();
        if (t - this->prev_time >= this->fixed_milli_time) {
            this->buffUpdate();
            this->screenDraw();
            this->prev_time = t;
        }
    }
    
    void draw () {
        this->prev_time = millis();
        this->buffUpdate();
        this->screenDraw();
    }
    
    bool isFixedTime () {
        return millis() - this->prev_time >= this->fixed_milli_time;
    }
    
    
    uint64_t prev_time = 0;
private:
    void screenDraw () {
        this->tft_es_buff->pushSprite(0, 0);
    }
    
    void buffUpdate () {
        this->tft_es_buff->fillSprite(0);
        for (int i = 0; i < this->wsize; ++i) {
            E512W3DWindow& w = *this->ws[i];
            w.draw();
        }
    }
    
};

