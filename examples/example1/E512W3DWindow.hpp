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
        this->begin();
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        this->drawChild(this->child, Matrix4x4::identity());
        this->drawChildT(this->child, Matrix4x4::identity());
    }
    
    
    
    void draw (Object3D& obj, bool child = false) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        
        Matrix4x4 mat = obj.parent->worldMatrix();
        
        if (child) {
            E512Array<Object3D*> objs;
            objs.emplace_back(&obj);
            this->drawChild(objs, mat);
            this->drawChildT(objs, mat);
        } else {
            mat = this->worldMatrix(&obj, mat);
            if (obj.mesh == NULL) { return; }
            if (obj.render_type == RenderType::WireFrame) { this->drawWireFrame(&obj, mat); }
            if (obj.render_type == RenderType::PolygonColor) { this->drawPolygonColor(&obj, mat); }
            if (obj.render_type == RenderType::PolygonNormal) { this->drawPolygonNormal(&obj, mat); }
            if (obj.render_type == RenderType::PolygonTexture) { this->drawPolygonTexture(&obj, mat); }
            if (obj.render_type == RenderType::PolygonTextureDoubleFace) { this->drawPolygonTextureDoubleFace(&obj, mat); }
            if (obj.render_type == RenderType::PolygonTexturePerspectiveCorrect) { this->PolygonTexturePerspectiveCorrect(&obj, mat); }
            if (obj.render_type == RenderType::PolygonTexturePerspectiveCorrectDoubleFace) { this->PolygonTexturePerspectiveCorrectDoubleFace(&obj, mat); }
            if (obj.render_type == RenderType::PolygonTranslucent) { this->drawPolygonTranslucent(&obj, mat); }
        }
    }
    
    void drawObjestAxis (Object3D& obj) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Matrix4x4 mat = this->objectWorldViewMatrix(obj);
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(Vector3(), mat), this->projescreen);
        Vector3 x = Matrix4x4::muld(Matrix4x4::mul(Vector3(2, 0, 0), mat), this->projescreen);
        Vector3 y = Matrix4x4::muld(Matrix4x4::mul(Vector3(0, 2, 0), mat), this->projescreen);
        Vector3 z = Matrix4x4::muld(Matrix4x4::mul(Vector3(0, 0, 2), mat), this->projescreen);
        this->drawBuffLine(s.x, s.y, x.x, x.y, color565(255, 0, 0));
        this->drawBuffLine(s.x, s.y, y.x, y.y, color565(0, 255, 0));
        this->drawBuffLine(s.x, s.y, z.x, z.y, color565(0, 0, 255));
    }
    
    
    void drawLine (Object3D& start, Object3D& end, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(start)), this->projescreen);
        Vector3 e = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(end)), this->projescreen);
        if (s.z < 0 || s.z > 1 || e.z < 0 || e.z > 1) { return; }
        this->drawBuffLine(s.x, s.y, e.x, e.y, color);
    }
    
    void drawLine (Vector3 start, Vector3 end, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 s = Matrix4x4::muld(Matrix4x4::mul(start, this->view), this->projescreen);
        Vector3 e = Matrix4x4::muld(Matrix4x4::mul(end, this->view), this->projescreen);
        if (s.z < 0 || s.z > 1 || e.z < 0 || e.z > 1) { return; }
        this->drawBuffLine(s.x, s.y, e.x, e.y, color);
    }
    void drawLine (uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        this->drawBuffLine(sx, sy, ex, ey, color);
    }
    
    void drawPoint (Object3D& obj, uint16_t size = 1, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        Vector3 p = Matrix4x4::muld(Matrix4x4::mul(Vector3(), this->objectWorldViewMatrix(obj)), this->projescreen);
        if (p.z < 0 || p.z > 1) { return; }
        this->drawPoint(p.x, p.y, size, color);
    }
    void drawPoint (Vector3 p, uint16_t size = 1, uint16_t color = 0xFFFF) {
        if (this->dsy == this->dey || this->dsx == this->dex) { return; }
        p = Matrix4x4::muld(Matrix4x4::mul(p, this->view), this->projescreen);
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
            this->projescreen = Matrix4x4::orthoscreenMatrix(this->width, this->height, this->ortho_size);
        } else {
            this->projescreen = Matrix4x4::projscreenMatrix(this->width, this->height);
        }
        
        if (color_buffer_clear) { this->clearCbuff(); }
        if (z_buffer_clear) { this->clearZbuff(); }
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
    
    // void updateViewMatrix () {
    //     Matrix4x4 mat = Matrix4x4::identity();
        
    //     if (this->camera != NULL) {
    //         Object3D* obj = this->camera;
    //         while (obj != NULL) {
    //             mat = Matrix4x4::mul(Matrix4x4::rotMatrix(Vector3() - obj->rotation), mat);
    //             mat = Matrix4x4::mul(Matrix4x4::moveMatrix(Vector3() - obj->position), mat);
    //             obj = obj->parent;
    //         }
    //     }
        
    //     this->view = mat;
    // }
    
    // void updateLightVector () {
    //     Matrix4x4 mat = Matrix4x4::identity();
        
    //     if (this->camera != NULL) {
    //         Object3D* obj = this->camera;
    //         while (obj != NULL) {
    //             mat = Matrix4x4::mul(Matrix4x4::rotMatrix(Vector3() - obj->rotation), mat);
    //             obj = obj->parent;
    //         }
    //     }
    //     this->light_vector = Matrix4x4::mul(this->light, mat);
    // }
    
    
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
                
                if (c->render_type == RenderType::WireFrame) { this->drawWireFrame(c, mat); }
                if (c->render_type == RenderType::PolygonColor) { this->drawPolygonColor(c, mat); }
                if (c->render_type == RenderType::PolygonNormal) { this->drawPolygonNormal(c, mat); }
                if (c->render_type == RenderType::PolygonTexture) { this->drawPolygonTexture(c, mat); }
                if (c->render_type == RenderType::PolygonTextureDoubleFace) { this->drawPolygonTextureDoubleFace(c, mat); }
                if (c->render_type == RenderType::PolygonTexturePerspectiveCorrect) { this->PolygonTexturePerspectiveCorrect(c, mat); }
                if (c->render_type == RenderType::PolygonTexturePerspectiveCorrectDoubleFace) { this->PolygonTexturePerspectiveCorrectDoubleFace(c, mat); }
            }
            this->drawChild(c->child, mat);
        }
    }
    
    void drawChildT (E512Array<Object3D*>& child, Matrix4x4 pmat) {
        for (auto&& c : child) {
            if (c->render_type == RenderType::Hide) { continue; }
            
            Matrix4x4 mat = this->worldMatrix(c, pmat);
            if (c->mesh != NULL) {
                if (c->render_type == RenderType::PolygonTranslucent) { this->drawPolygonTranslucent(c, mat); }
            }
            this->drawChildT(c->child, mat);
        }
    }
    
    
    void drawWireFrame (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::muld(Matrix4x4::mul(v1, mat), this->projescreen);
            v2 = Matrix4x4::muld(Matrix4x4::mul(v2, mat), this->projescreen);
            v3 = Matrix4x4::muld(Matrix4x4::mul(v3, mat), this->projescreen);
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            this->drawBuffLine(v1.x, v1.y, v2.x, v2.y, o->color);
            this->drawBuffLine(v2.x, v2.y, v3.x, v3.y, o->color);
            this->drawBuffLine(v3.x, v3.y, v1.x, v1.y, o->color);
        }
    }
    
    
    void drawPolygonColor (Object3D* o, Matrix4x4 mat) {
        float r = (o->color >> 11) << 3;
        float g = ((o->color >> 5) & 0b111111) << 2;
        float b = (o->color & 0b11111) << 3;
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            const float d = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            
            uint16_t color = color565(min(r*d, 255.0f), min(g*d, 255.0f), min(b*d, 255.0f));
            uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
            this->fillTriangleColor(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, z, color);
        }
    }
    
    
    void drawPolygonNormal (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
            
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            Vector3 hn = (n * 0.5f + 0.5f) * 255.0f;
            uint16_t color = color565(hn.x, hn.y, hn.z);
            uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
            this->fillTriangleColor(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, z, color);
        }
    }
    
    
    void drawPolygonTranslucent (Object3D* o, Matrix4x4 mat) {
        float r = (o->color >> 11) << 3;
        float g = ((o->color >> 5) & 0b111111) << 2;
        float b = (o->color & 0b11111) << 3;
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (Vector3::cross(v2 - v1, v3 - v2).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            const float d = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            
            uint16_t color = color565(min(r*d, 255.0f), min(g*d, 255.0f), min(b*d, 255.0f));
            uint16_t z = (1.0f-(v1.z+v2.z+v3.z)*0.333f) * 32767;
            this->fillTriangleTranslucent(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, z, color);
        }
    }
    
    
    void drawPolygonTexture (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (Vector3::cross(v2-v1, v3-v1).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            this->fillTriangleTexture(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
            
            
        }
    }
    
    void drawPolygonTextureDoubleFace (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector3 v1 = o->mesh->vertexs[f.a];
            Vector3 v2 = o->mesh->vertexs[f.b];
            Vector3 v3 = o->mesh->vertexs[f.c];
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2-v1, v3-v1));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a];
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b];
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c];
            
            float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            this->fillTriangleTexture(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
    }
    
    
    void PolygonTexturePerspectiveCorrect (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector4 v1 = Vector4(o->mesh->vertexs[f.a]);
            Vector4 v2 = Vector4(o->mesh->vertexs[f.b]);
            Vector4 v3 = Vector4(o->mesh->vertexs[f.c]);
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()).z > 0) { continue; }
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a] / v1.w;
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b] / v2.w;
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c] / v3.w;
            
            v1.w = 1.0 / v1.w;
            v2.w = 1.0 / v2.w;
            v3.w = 1.0 / v3.w;
            
            float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            this->fillTriangleTexturePerspectiveCorrect(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
        }
    }
    
    
    void PolygonTexturePerspectiveCorrectDoubleFace (Object3D* o, Matrix4x4 mat) {
        mat = Matrix4x4::mul(mat, this->view);
        for (int i = 0; i < o->mesh->faces.size(); ++i) {
            const Face& f = o->mesh->faces[i];
            const Face& fuv = o->mesh->uv_faces[i];
            
            Vector4 v1 = Vector4(o->mesh->vertexs[f.a]);
            Vector4 v2 = Vector4(o->mesh->vertexs[f.b]);
            Vector4 v3 = Vector4(o->mesh->vertexs[f.c]);
            
            v1 = Matrix4x4::mul(v1, mat);
            v2 = Matrix4x4::mul(v2, mat);
            v3 = Matrix4x4::mul(v3, mat);
            
            const Vector3 n = Vector3::normalize(Vector3::cross(v2.xyz()-v1.xyz(), v3.xyz()-v1.xyz()));
            
            v1 = Matrix4x4::muld(v1, this->projescreen);
            v2 = Matrix4x4::muld(v2, this->projescreen);
            v3 = Matrix4x4::muld(v3, this->projescreen);
            
            if (!((v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1) || (v3.z > 0 && v3.z < 1))) { continue; }
            if (!((v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width) || (v3.x >= 0 && v3.x < this->width))) { continue; }
            if (!((v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height) || (v3.y >= 0 && v3.y < this->height))) { continue; }
            
            Vector2 v1uv = o->mesh->uv_vertexs[fuv.a] / v1.w;
            Vector2 v2uv = o->mesh->uv_vertexs[fuv.b] / v2.w;
            Vector2 v3uv = o->mesh->uv_vertexs[fuv.c] / v3.w;
            
            v1.w = 1.0 / v1.w;
            v2.w = 1.0 / v2.w;
            v3.w = 1.0 / v3.w;
            
            float light = max(max(Vector3::dot(this->light_vector, n) * this->light_strength, this->ambient), 0.0f);
            this->fillTriangleTexturePerspectiveCorrect(v1, v2, v3, v1uv, v2uv, v3uv, light, o);
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
    
    inline bool inSide (const int& x, const int& y) { return !(x < 0 || x >= this->width || y < 0 || y >= this->height); }
    
    inline bool inSide2 (int x, int y) {////
        x += this->sx;
        y += this->sy;
        return !(x < this->dsx || x >= this->dex || y < this->dsy || y >= this->dey);
    }
    inline void swap (int16_t& a, int16_t& b) { int16_t c = a; a = b; b = c; }
    
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
        this->colorBufferClear();
        this->allWindowDraw();
        this->pushScreen();
    }
    
    void fixedDraw () {
        uint64_t t = millis();
        if (t - this->prev_time >= this->fixed_milli_time) {
            this->colorBufferClear();
            this->allWindowDraw();
            this->pushScreen();
            this->prev_time = t;
        }
    }
    
    void draw () {
        this->prev_time = millis();
        this->colorBufferClear();
        this->allWindowDraw();
        this->pushScreen();
    }
    
    bool isFixedTime () {
        return millis() - this->prev_time >= this->fixed_milli_time;
    }
    void clear () {
        this->prev_time = millis();
        this->colorBufferClear();
    }
    void pushScreen () {
        this->tft_es_buff->pushSprite(0, 0);
    }
    uint64_t prev_time = 0;
private:
    void colorBufferClear () { this->tft_es_buff->fillSprite(0); }
    
    void allWindowDraw () {
        for (int i = 0; i < this->wsize; ++i) {
            E512W3DWindow& w = *this->ws[i];
            w.draw();
        }
    }
    
};

