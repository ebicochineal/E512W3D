#pragma once

#include "E512W3DUtilsX.hpp"

uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }



template <class T>
class E512Array {
private:
    uint16_t array_size = 0;
    uint16_t array_capacity = 1;
public:
    T* a;
    
    E512Array () { this->a = new T[1]; }
    E512Array (uint16_t sz) {
        this->a = new T[sz];
        this->array_capacity = sz;
    }
    E512Array (uint16_t sz, T t) {
        this->a = new T[sz];
        this->array_size = sz;
        this->array_capacity = sz;
        for (int i = 0; i < sz; ++i) { this->a[i] = t; }
    }
    
    ~E512Array () { delete[] this->a; }
    
    uint16_t size () { return this->array_size; }
    uint16_t capacity () { return this->array_capacity; }
    
    void resize (uint16_t sz, T c = T()) {
        while (sz < this->array_size) { this->pop_back(); }
        while (sz > this->array_size) { this->push_back(c); }
    }
    
    void reserve (uint16_t sz) {
        if (sz < this->array_capacity) { return; }
        this->array_capacity = sz;
        T* a = new T[this->array_capacity];
        for (int i = 0; i < min(this->array_size, this->array_capacity); ++i) { a[i] = this->a[i]; }
        delete[] this->a;
        this->a = a;
        this->array_size = min(this->array_size, this->array_capacity);
    }
    
    void shrink_to_fit () {
        this->array_capacity = this->array_size;
        T* a = new T[this->array_capacity];
        for (int i = 0; i < this->array_size; ++i) { a[i] = this->a[i]; }
        delete[] this->a;
        this->a = a;
    }
    
    template <class... Args>
    void emplace_back (Args... args) {
        if (this->array_size + 1 > this->array_capacity) {
            this->array_capacity *= 2;
            T* a = new T[this->array_capacity];
            for (int i = 0; i < this->array_size; ++i) { a[i] = this->a[i]; }
            a[this->array_size] = T(args...);
            delete[] this->a;
            this->a = a;
            this->array_size += 1;
        } else {
            this->a[this->array_size] = T(args...);
            this->array_size += 1;
        }
    }
    void push_back (T t) { this->emplace_back(t); }
    
    void pop_back () {
        if (this->array_size > 0) { this->array_size -= 1; }
    }
    
    void erase_index (int index) {
        uint16_t tcnt = 0;
        for (uint16_t i = 0; i < this->array_size; ++i) {
            if (i != index) {
                this->a[tcnt] = this->a[i];
                tcnt += 1;
            }
        }
        this->array_size = tcnt;
    }
    
    void erase_value (T t) {
        uint16_t tcnt = 0;
        for (uint16_t i = 0; i < this->array_size; ++i) {
            if (this->a[i] != t) {
                this->a[tcnt] = this->a[i];
                tcnt += 1;
            }
        }
        this->array_size = tcnt;
    }
    
    T& front () { return this->a[0]; }
    T& back () { return this->a[this->array_size - 1]; }
    
    E512Array (const E512Array& t) {
        T* a = new T[t.array_capacity];
        for (int i = 0; i < t.array_size; ++i) { a[i] = t.a[i]; }
        this->array_size = t.array_size;
        this->array_capacity = t.array_capacity;
        this->a = a;
    }
    
    E512Array& operator = (const E512Array& t) {
        T* a = new T[t.array_capacity];
        for (int i = 0; i < t.array_size; ++i) { a[i] = t.a[i]; }
        this->array_size = t.array_size;
        this->array_capacity = t.array_capacity;
        delete[] this->a;
        this->a = a;
        return *this;
    }
    
    // indexer
    T& operator [] (uint16_t i) { return this->a[i]; }
    
    // range based for
    T* begin () { return &this->a[0]; }
    T* end () { return &this->a[this->array_size]; }
};

struct Vector2 {
public:
    float x, y;
    Vector2 () { this->x = 0; this->y = 0; }
    Vector2 (float t) { this->x = t; this->y = t; }
    Vector2 (float x, float y) { this->x = x; this->y = y; }
    //Vector2 (Vector3 v) { this->x = v.x; this->y = v.y; }
    Vector2 operator + (const Vector2& t) const { return Vector2(this->x + t.x, this->y + t.y); }
    Vector2 operator - (const Vector2& t) const { return Vector2(this->x - t.x, this->y - t.y); }
    Vector2 operator * (const Vector2& t) const { return Vector2(this->x * t.x, this->y * t.y); }
    Vector2 operator / (const Vector2& t) const { return Vector2(this->x / t.x, this->y / t.y); }
    Vector2 operator + (const float& t) const { return Vector2(this->x + t, this->y + t); }
    Vector2 operator - (const float& t) const { return Vector2(this->x - t, this->y - t); }
    Vector2 operator * (const float& t) const { return Vector2(this->x * t, this->y * t); }
    Vector2 operator / (const float& t) const { return Vector2(this->x / t, this->y / t); }
    bool operator == (const Vector2& t) const { return this->x == t.x && this->y == t.y; }
};
struct Vector3 {
public:
    float x, y, z;
    Vector3 () { this->x = 0; this->y = 0; this->z = 0; }
    Vector3 (float t) { this->x = t; this->y = t; this->z = t; }
    Vector3 (Vector2 t, float z) { this->x = t.x; this->y = t.y; this->z = z; }
    Vector3 (float x, Vector2 t) { this->x = x; this->y = t.x; this->z = t.y; }
    Vector3 (float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
    static Vector3 normalize (Vector3 v) {
        float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (d == 0) { return Vector3(0.0f, 0.0f, 0.0f); }
        return v / d;
    }
    static Vector3 cross (const Vector3 a, const Vector3 b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
    static float dot (Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    Vector3 operator + (const Vector3& t) const { return Vector3(this->x + t.x, this->y + t.y, this->z + t.z); }
    Vector3 operator - (const Vector3& t) const { return Vector3(this->x - t.x, this->y - t.y, this->z - t.z); }
    Vector3 operator * (const Vector3& t) const { return Vector3(this->x * t.x, this->y * t.y, this->z * t.z); }
    Vector3 operator / (const Vector3& t) const { return Vector3(this->x / t.x, this->y / t.y, this->z / t.z); }
    Vector3 operator + (const float& t) const { return Vector3(this->x + t, this->y + t, this->z + t); }
    Vector3 operator - (const float& t) const { return Vector3(this->x - t, this->y - t, this->z - t); }
    Vector3 operator * (const float& t) const { return Vector3(this->x * t, this->y * t, this->z * t); }
    Vector3 operator / (const float& t) const { return Vector3(this->x / t, this->y / t, this->z / t); }
    bool operator == (const Vector3& t) const { return this->x == t.x && this->y == t.y && this->z == t.z; }
};
struct Matrix4x4 {
public:
    float m[4][4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    Matrix4x4 () {}
    Matrix4x4 (float a[]) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                this->m[i][j] = a[i*4+j];
            }
        }
    }
    static float radian (float d) { return d * 0.01745329251f; }
    static Matrix4x4 identity () {
        float a[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        return Matrix4x4(a);
    }
    
    static Matrix4x4 mul (Matrix4x4 a, Matrix4x4 b) {
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    r.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return r;
    }
    
    static Vector3 mul (Vector3 v, Matrix4x4 m) {
        float d = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];
        Vector3 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2],
        };
        
        if (d > 0) {
            r.x /= d;
            r.y /= d;
            r.z /= d;
        }
        return r;
    }
    
    static Matrix4x4 projectionMatrix (float w, float h) {
        Matrix4x4 r;
        float aspect =  w / h;
        float cfov = Matrix4x4::radian(45.0);
        float cnear = 4.0f;
        float cfar = 1000.0f;
        float y = 1.0f / tan(cfov * 0.5f);
        float x = y / aspect;
        float z = cfar / (cnear - cfar);
        r.m[0][0] = x;
        r.m[1][1] = y;
        r.m[2][2] = z;
        r.m[2][3] = -1.0f;
        r.m[3][2] = z * cnear;
        return r;
    }
    
    static Matrix4x4 orthoMatrix (float w, float h, float size) {
        Matrix4x4 r;
        float left = -w * size;
        float right = w * size;
        
        float top = h * size;
        float bottom = -h * size;
        
        float cnear = 4.0f;
        float cfar = 1000.0f;
        r.m[0][0] = 2.0f / (right - left);
        r.m[1][1] = 2.0f / (top - bottom);
        r.m[2][2] = -2.0f / (cnear - cfar);
        r.m[3][0] = -((right+left)/(right-left));
        r.m[3][1] = -((top+bottom)/(top-bottom));
        r.m[3][2] = ((cfar+cnear)/(cfar-cnear));
        
        r.m[3][3] = 1.0f;
        
        return r;
    }
    static Matrix4x4 screenMatrix (float w, float h) {
        Matrix4x4 r;
        float hcx = w / 2;
        float hcy = h / 2;
        r.m[0][0] = hcx;
        r.m[1][1] = -hcy;
        r.m[2][2] = 1.0f;
        r.m[3][0] = hcx; r.m[3][1] = hcy; r.m[3][3] = 1.0f;
        return r;
    }
    
    static Matrix4x4 projscreenMatrix (float w, float h) {
        Matrix4x4 r;
        r.m[0][0] = 1.0f; r.m[1][1] = 1.0f; r.m[2][2] = 1.0f; r.m[3][3] = 1.0f;
        r = Matrix4x4::mul(r, Matrix4x4::projectionMatrix(w, h));
        r = Matrix4x4::mul(r, Matrix4x4::screenMatrix(w, h));
        return r;
    }
    static Matrix4x4 orthoscreenMatrix (float w, float h, float size) {
        Matrix4x4 r;
        r.m[0][0] = 1.0f; r.m[1][1] = 1.0f; r.m[2][2] = 1.0f; r.m[3][3] = 1.0f;
        r = Matrix4x4::mul(r, Matrix4x4::orthoMatrix(w, h, size));
        r = Matrix4x4::mul(r, Matrix4x4::screenMatrix(w, h));
        return r;
    }
    
    static Matrix4x4 moveMatrix (Vector3 v) {
        Matrix4x4 r = Matrix4x4::identity();
        r.m[3][0] = v.x;
        r.m[3][1] = v.y;
        r.m[3][2] = v.z;
        return r;
    }
    static Matrix4x4 rotMatrix (Vector3 v) {
        Matrix4x4 r = Matrix4x4::identity();
        Matrix4x4 t;
        float x = Matrix4x4::radian(v.x);
        float y = Matrix4x4::radian(v.y);
        float z = Matrix4x4::radian(v.z);
        
        t = Matrix4x4::identity();
        t.m[1][1] =  cos(x);
        t.m[1][2] =  sin(x);
        t.m[2][1] = -sin(x);
        t.m[2][2] =  cos(x);
        r = Matrix4x4::mul(r, t);
        
        t = Matrix4x4::identity();
        t.m[0][0] =  cos(y);
        t.m[0][2] = -sin(y);
        t.m[2][0] =  sin(y);
        t.m[2][2] =  cos(y);
        r = Matrix4x4::mul(r, t);
        
        t = Matrix4x4::identity();
        t.m[0][0] =  cos(z);
        t.m[0][1] =  sin(z);
        t.m[1][0] = -sin(z);
        t.m[1][1] =  cos(z);
        r = Matrix4x4::mul(r, t);
        
        return r;
    }
    
    static Matrix4x4 scaleMatrix (Vector3 v) {
        Matrix4x4 r = Matrix4x4::identity();
        r.m[0][0] = v.x;
        r.m[1][1] = v.y;
        r.m[2][2] = v.z;
        return r;
    }
};

struct Face {
    uint16_t a, b, c;
    Face () {};
    Face (uint16_t a, uint16_t b, uint16_t c) { this->a = a; this->b = b; this->c = c; }
};

struct Mesh {
public:
    E512Array<Vector3> vertexs;
    E512Array<Face> faces;
    
    E512Array<Vector2> uv_vertexs;
    E512Array<Face> uv_faces;
    
    Mesh () {};
    ~Mesh () {};
    
    void addVertex (float x, float y, float z) { this->addVertex(Vector3(x, y, z)); }
    void addVertex (Vector3 v) {
        this->vertexs.emplace_back(v);
        // this->tvertexs.emplace_back(v);
    }
    
    void addFace (uint16_t a, uint16_t b, uint16_t c) { this->addFace(Face(a, b, c)); }
    void addFace (Face f) {
        this->faces.emplace_back(f);
    }
    
    void addVertexUV (float x, float y) { this->addVertexUV(Vector2(x, y)); }
    void addVertexUV (Vector2 v) {
        this->uv_vertexs.emplace_back(v);
    }
    void addFaceUV (uint16_t a, uint16_t b, uint16_t c) { this->addFaceUV(Face(a, b, c)); }
    void addFaceUV (Face f) {
        this->uv_faces.emplace_back(f);
    }
    
    // void addFaceUV (float a, float b, float c) { this->addFaceUV(FaceUV(a, b, c)); }
    // void addFaceUV (FaceUV fuv) {
    //     this->faceuvs.emplace_back(fuv);
    // }
    
};

struct Texture {
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t* pixels;
    Texture () {};
    Texture (uint16_t width, uint16_t height, uint16_t* pixels) {
        this->width = width;
        this->height = height;
        this->pixels = pixels;
    }
    uint16_t getColor (float u, float v) {
        const uint16_t u16i = (uint16_t)(this->width * u) % this->width;
        const uint16_t v16i = (uint16_t)(this->height * (1.0-v)) % this->height;
        return this->pixels[v16i*this->width+u16i];
    }
};

enum RenderType {
    WireFrame,
    PolygonColor,
    PolygonNormal,
    PolygonTexture,
    Hide,
    None,
};

struct Object3D {
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
    Mesh* mesh = NULL;
    uint16_t color = 65535;
    uint16_t render_type = RenderType::WireFrame;
    Object3D* parent;
    E512Array<Object3D*> child;
    Texture* texture;
    
    Object3D () {};
    
    void setParent (Object3D& o) {
        this->parent = &o;
        o.child.emplace_back(this);
    }
    void addChild (Object3D& o) {
        o.parent = this;
        this->child.emplace_back(&o);
    }
};
