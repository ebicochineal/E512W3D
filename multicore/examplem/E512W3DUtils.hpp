#pragma once
#include "E512W3DUtilsX.hpp"

const float DEGREE_TO_RADIAN_F = 0.01745329251f;
inline float toRadianF (float d) { return d * DEGREE_TO_RADIAN_F; } 
uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }
uint16_t color1555 (uint16_t a, uint16_t r, uint16_t g, uint16_t b) { return a << 15 | ((r>>3)<<10) | ((g>>3)<<5) | (b>>3); }



// max size uint16_t
// template <class T>
// class E512Array {
// private:
//     uint16_t array_size = 0;
//     uint16_t array_capacity = 1;
// public:
//     T* a;
    
//     E512Array () { this->a = new T[1]; }
//     E512Array (uint16_t sz) {
//         this->a = new T[sz];
//         this->array_capacity = sz;
//     }
//     E512Array (uint16_t sz, T t) {
//         this->a = new T[sz];
//         this->array_size = sz;
//         this->array_capacity = sz;
//         for (int i = 0; i < sz; ++i) { this->a[i] = t; }
//     }
    
//     ~E512Array () { delete[] this->a; }
    
//     uint16_t size () { return this->array_size; }
//     uint16_t capacity () { return this->array_capacity; }
    
//     void clear () { this->array_size = 0; }
    
//     void resize (uint16_t sz, T c = T()) {
//         while (sz < this->array_size) { this->pop_back(); }
//         while (sz > this->array_size) { this->push_back(c); }
//     }
    
//     void reserve (uint16_t sz) {
//         if (sz < this->array_capacity) { return; }
//         this->array_capacity = sz;
//         T* a = new T[this->array_capacity];
//         for (int i = 0; i < min(this->array_size, this->array_capacity); ++i) { a[i] = this->a[i]; }
//         delete[] this->a;
//         this->a = a;
//         this->array_size = min(this->array_size, this->array_capacity);
//     }
    
//     void shrink_to_fit () {
//         this->array_capacity = this->array_size;
//         T* a = new T[this->array_capacity];
//         for (int i = 0; i < this->array_size; ++i) { a[i] = this->a[i]; }
//         delete[] this->a;
//         this->a = a;
//     }
    
//     template <class... Args>
//     void emplace_back (Args... args) {
//         if (this->array_size + 1 > this->array_capacity) {
//             this->array_capacity *= 2;
//             T* a = new T[this->array_capacity];
//             for (int i = 0; i < this->array_size; ++i) { a[i] = this->a[i]; }
//             a[this->array_size] = T(args...);
//             delete[] this->a;
//             this->a = a;
//             this->array_size += 1;
//         } else {
//             this->a[this->array_size] = T(args...);
//             this->array_size += 1;
//         }
//     }
//     void push_back (T t) { this->emplace_back(t); }
    
//     void pop_back () {
//         if (this->array_size > 0) { this->array_size -= 1; }
//     }
    
//     void erase_index (int index) {
//         uint16_t tcnt = 0;
//         for (uint16_t i = 0; i < this->array_size; ++i) {
//             if (i != index) {
//                 this->a[tcnt] = this->a[i];
//                 tcnt += 1;
//             }
//         }
//         this->array_size = tcnt;
//     }
    
//     void erase_value (T t) {
//         uint16_t tcnt = 0;
//         for (uint16_t i = 0; i < this->array_size; ++i) {
//             if (this->a[i] != t) {
//                 this->a[tcnt] = this->a[i];
//                 tcnt += 1;
//             }
//         }
//         this->array_size = tcnt;
//     }
    
//     T& front () { return this->a[0]; }
//     T& back () { return this->a[this->array_size - 1]; }
    
//     E512Array (const E512Array& t) {
//         T* a = new T[t.array_capacity];
//         for (int i = 0; i < t.array_size; ++i) { a[i] = t.a[i]; }
//         this->array_size = t.array_size;
//         this->array_capacity = t.array_capacity;
//         this->a = a;
//     }
    
//     E512Array& operator = (const E512Array& t) {
//         T* a = new T[t.array_capacity];
//         for (int i = 0; i < t.array_size; ++i) { a[i] = t.a[i]; }
//         this->array_size = t.array_size;
//         this->array_capacity = t.array_capacity;
//         delete[] this->a;
//         this->a = a;
//         return *this;
//     }
    
//     // indexer
//     T& operator [] (uint16_t i) { return this->a[i]; }
    
//     // range based for
//     T* begin () { return &this->a[0]; }
//     T* end () { return &this->a[this->array_size]; }
// };

// max size uint32_t
template <class T>
class E512Array {
private:
    uint32_t array_size = 0;
    uint32_t array_capacity = 1;
public:
    T* a;
    
    E512Array () { this->a = new T[1]; }
    E512Array (uint32_t sz) {
        this->a = new T[sz];
        this->array_capacity = sz;
    }
    E512Array (uint32_t sz, T t) {
        this->a = new T[sz];
        this->array_size = sz;
        this->array_capacity = sz;
        for (int i = 0; i < sz; ++i) { this->a[i] = t; }
    }
    
    ~E512Array () { delete[] this->a; }
    
    uint32_t size () { return this->array_size; }
    uint32_t capacity () { return this->array_capacity; }
    
    void clear () { this->array_size = 0; }
    
    void resize (uint32_t sz, T c = T()) {
        while (sz < this->array_size) { this->pop_back(); }
        while (sz > this->array_size) { this->push_back(c); }
    }
    
    void reserve (uint32_t sz) {
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
        uint32_t tcnt = 0;
        for (uint32_t i = 0; i < this->array_size; ++i) {
            if (i != index) {
                this->a[tcnt] = this->a[i];
                tcnt += 1;
            }
        }
        this->array_size = tcnt;
    }
    
    void erase_value (T t) {
        uint32_t tcnt = 0;
        for (uint32_t i = 0; i < this->array_size; ++i) {
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
    T& operator [] (uint32_t i) { return this->a[i]; }
    
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
    static float distance (const Vector2 a, const Vector2 b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }
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
    static float distance (const Vector3 a, const Vector3 b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)); }
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
struct Vector4 {
public:
    float x, y, z, w;
    Vector4 () { this->x = 0; this->y = 0; this->z = 0; this->w = 0; }
    Vector4 (float t) { this->x = t; this->y = t; this->z = t; this->w = t; }
    Vector4 (Vector3 v) { this->x = v.x; this->y = v.y; this->z = v.z; this->w = 1; }
    Vector4 (float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
    Vector3 xyz() { return Vector3(this->x, this->y, this->z); }
    Vector4 operator + (const Vector4& t) const { return Vector4(this->x + t.x, this->y + t.y, this->z + t.z, this->w + t.w); }
    
    
    Vector4 operator * (const float& t) const { return Vector4(this->x * t, this->y * t, this->z * t, this->w * t); }
};

struct Quaternion {
    float w = 1.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    Quaternion () {}
    Quaternion (float w, float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    
    static Quaternion angleAxis (float w, float x, float y, float z) {
        Quaternion q;
        w *= DEGREE_TO_RADIAN_F;
        q.w = cos(w/2);
        w = sin(-w/2);
        q.x = x * w;
        q.y = y * w;
        q.z = z * w;
        return q;
    }
    
    static Quaternion angleAxis (float w, Vector3 v) {
        return Quaternion::angleAxis(w, v.x, v.y, v.z);
    }
    
    Quaternion mul (Quaternion q, Quaternion p) {
        return Quaternion(
            -q.x*p.x - q.y*p.y - q.z*p.z + q.w*p.w,
            q.w*p.x - q.z*p.y + q.y*p.z + q.x*p.w,
            q.z*p.x + q.w*p.y - q.x*p.z + q.y*p.w,
            -q.y*p.x + q.x*p.y + q.w*p.z + q.z*p.w
        );
    }
    
    void mul (Quaternion q) { *this = Quaternion::mul(*this, q); }
    
    Quaternion operator * (const Quaternion& t) const {
        return Quaternion(
            -this->x*t.x - this->y*t.y - this->z*t.z + this->w*t.w,
             this->w*t.x - this->z*t.y + this->y*t.z + this->x*t.w,
             this->z*t.x + this->w*t.y - this->x*t.z + this->y*t.w,
            -this->y*t.x + this->x*t.y + this->w*t.z + this->z*t.w
        );
    }
    
    Quaternion &operator *= (const Quaternion& t) {
        this->mul(t);
        return *this;
    }
    
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
        Vector3 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2],
        };
        
        return r;
    }
    
    static Vector3 muld (Vector3 v, Matrix4x4 m) {
        float d = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
        Vector3 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2],
        };
        
        if (d > 0) {
            r.x /= d;
            r.y /= d;
            r.z /= d;
        }
        return r;
    }
    
    static Vector4 muld (Vector4 v, Matrix4x4 m) {
        float d = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
        Vector4 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2],
            v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3],
        };
        
        if (d > 0) {
            r.x /= d;
            r.y /= d;
            r.z /= d;
        }
        return r;
    }
    
    static Vector4 mul (Vector4 v, Matrix4x4 m) {
        Vector4 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2],
            v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3],
        };
        
        return r;
    }
    
    
    static Matrix4x4 projectionMatrix (float w, float h) {
        Matrix4x4 r;
        float aspect =  w / h;
        float cfov = 45.0 * DEGREE_TO_RADIAN_F;
        float cnear = 0.5f;
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
        
        float cnear = 0.5f;
        float cfar = 1000.0f;
        r.m[0][0] = 4.0f / (right - left);
        r.m[1][1] = 4.0f / (top - bottom);
        r.m[2][2] = -4.0f / (cfar - cnear);
        r.m[3][0] = -((right+left)/(right-left));
        r.m[3][1] = -((top+bottom)/(top-bottom));
        r.m[3][2] = ((cfar+cnear)/(cfar-cnear));
        
        r.m[3][3] = 2.0f;
        
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
        float x = v.x * DEGREE_TO_RADIAN_F;
        float y = v.y * DEGREE_TO_RADIAN_F;
        float z = v.z * DEGREE_TO_RADIAN_F;
        
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
    
    static Matrix4x4 rotMatrix (Quaternion q) {
        Matrix4x4 r = Matrix4x4::identity();
        r.m[0][0] = 2*q.w*q.w + 2*q.x*q.x - 1.0;
        r.m[0][1] = 2*q.x*q.y - 2*q.z*q.w;
        r.m[0][2] = 2*q.x*q.z + 2*q.y*q.w;
        
        r.m[1][0] = 2*q.x*q.y + 2*q.z*q.w;
        r.m[1][1] = 2*q.w*q.w + 2*q.y*q.y - 1.0;
        r.m[1][2] = 2*q.y*q.z - 2*q.x*q.w;
        
        r.m[2][0] = 2*q.x*q.z - 2*q.y*q.w;
        r.m[2][1] = 2*q.y*q.z + 2*q.x*q.w;
        r.m[2][2] = 2*q.w*q.w + 2*q.z*q.z - 1.0;
        
        return r;
    }
    static Matrix4x4 rotMatrixR (Quaternion q) {
        q.w = -q.w;
        Matrix4x4 r = Matrix4x4::identity();
        r.m[0][0] = 2*q.w*q.w + 2*q.x*q.x - 1.0;
        r.m[0][1] = 2*q.x*q.y - 2*q.z*q.w;
        r.m[0][2] = 2*q.x*q.z + 2*q.y*q.w;
        
        r.m[1][0] = 2*q.x*q.y + 2*q.z*q.w;
        r.m[1][1] = 2*q.w*q.w + 2*q.y*q.y - 1.0;
        r.m[1][2] = 2*q.y*q.z - 2*q.x*q.w;
        
        r.m[2][0] = 2*q.x*q.z - 2*q.y*q.w;
        r.m[2][1] = 2*q.y*q.z + 2*q.x*q.w;
        r.m[2][2] = 2*q.w*q.w + 2*q.z*q.z - 1.0;
        
        return r;
    }
    
    static Matrix4x4 inverse (Matrix4x4 a) {
        Matrix4x4 b;
        float d = a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2]
                + a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2] + a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3] + a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0]
                + a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1]
                + a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2] + a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0]
                - a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2] - a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1]
                - a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3] - a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0] - a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2]
                - a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1] - a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0]
                - a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2] - a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0] - a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1];
        if (d == 0.0f) { return a; }
        
        b.m[0][0] = a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[1][3] * a.m[2][1] * a.m[3][2] - a.m[1][1] * a.m[2][3] * a.m[3][2] - a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[1][3] * a.m[2][2] * a.m[3][1];
        b.m[0][1] = a.m[0][1] * a.m[2][3] * a.m[3][2] + a.m[0][2] * a.m[2][1] * a.m[3][3] + a.m[0][3] * a.m[2][2] * a.m[3][1] - a.m[0][1] * a.m[2][2] * a.m[3][3] - a.m[0][2] * a.m[2][3] * a.m[3][1] - a.m[0][3] * a.m[2][1] * a.m[3][2];
        b.m[0][2] = a.m[0][1] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[3][2] - a.m[0][1] * a.m[1][3] * a.m[3][2] - a.m[0][2] * a.m[1][1] * a.m[3][3] - a.m[0][3] * a.m[1][2] * a.m[3][1];
        b.m[0][3] = a.m[0][1] * a.m[1][3] * a.m[2][2] + a.m[0][2] * a.m[1][1] * a.m[2][3] + a.m[0][3] * a.m[1][2] * a.m[2][1] - a.m[0][1] * a.m[1][2] * a.m[2][3] - a.m[0][2] * a.m[1][3] * a.m[2][1] - a.m[0][3] * a.m[1][1] * a.m[2][2];

        b.m[1][0] = a.m[1][0] * a.m[2][3] * a.m[3][2] + a.m[1][2] * a.m[2][0] * a.m[3][3] + a.m[1][3] * a.m[2][2] * a.m[3][0] - a.m[1][0] * a.m[2][2] * a.m[3][3] - a.m[1][2] * a.m[2][3] * a.m[3][0] - a.m[1][3] * a.m[2][0] * a.m[3][2];
        b.m[1][1] = a.m[0][0] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][0] + a.m[0][3] * a.m[2][0] * a.m[3][2] - a.m[0][0] * a.m[2][3] * a.m[3][2] - a.m[0][2] * a.m[2][0] * a.m[3][3] - a.m[0][3] * a.m[2][2] * a.m[3][0];
        b.m[1][2] = a.m[0][0] * a.m[1][3] * a.m[3][2] + a.m[0][2] * a.m[1][0] * a.m[3][3] + a.m[0][3] * a.m[1][2] * a.m[3][0] - a.m[0][0] * a.m[1][2] * a.m[3][3] - a.m[0][2] * a.m[1][3] * a.m[3][0] - a.m[0][3] * a.m[1][0] * a.m[3][2];
        b.m[1][3] = a.m[0][0] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] + a.m[0][3] * a.m[1][0] * a.m[2][2] - a.m[0][0] * a.m[1][3] * a.m[2][2] - a.m[0][2] * a.m[1][0] * a.m[2][3] - a.m[0][3] * a.m[1][2] * a.m[2][0];

        b.m[2][0] = a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[1][3] * a.m[2][0] * a.m[3][1] - a.m[1][0] * a.m[2][3] * a.m[3][1] - a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[1][3] * a.m[2][1] * a.m[3][0];
        b.m[2][1] = a.m[0][0] * a.m[2][3] * a.m[3][1] + a.m[0][1] * a.m[2][0] * a.m[3][3] + a.m[0][3] * a.m[2][1] * a.m[3][0] - a.m[0][0] * a.m[2][1] * a.m[3][3] - a.m[0][1] * a.m[2][3] * a.m[3][0] - a.m[0][3] * a.m[2][0] * a.m[3][1];
        b.m[2][2] = a.m[0][0] * a.m[1][1] * a.m[3][3] + a.m[0][1] * a.m[1][3] * a.m[3][0] + a.m[0][3] * a.m[1][0] * a.m[3][1] - a.m[0][0] * a.m[1][3] * a.m[3][1] - a.m[0][1] * a.m[1][0] * a.m[3][3] - a.m[0][3] * a.m[1][1] * a.m[3][0];
        b.m[2][3] = a.m[0][0] * a.m[1][3] * a.m[2][1] + a.m[0][1] * a.m[1][0] * a.m[2][3] + a.m[0][3] * a.m[1][1] * a.m[2][0] - a.m[0][0] * a.m[1][1] * a.m[2][3] - a.m[0][1] * a.m[1][3] * a.m[2][0] - a.m[0][3] * a.m[1][0] * a.m[2][1];

        b.m[3][0] = a.m[1][0] * a.m[2][2] * a.m[3][1] + a.m[1][1] * a.m[2][0] * a.m[3][2] + a.m[1][2] * a.m[2][1] * a.m[3][0] - a.m[1][0] * a.m[2][1] * a.m[3][2] - a.m[1][1] * a.m[2][2] * a.m[3][0] - a.m[1][2] * a.m[2][0] * a.m[3][1];
        b.m[3][1] = a.m[0][0] * a.m[2][1] * a.m[3][2] + a.m[0][1] * a.m[2][2] * a.m[3][0] + a.m[0][2] * a.m[2][0] * a.m[3][1] - a.m[0][0] * a.m[2][2] * a.m[3][1] - a.m[0][1] * a.m[2][0] * a.m[3][2] - a.m[0][2] * a.m[2][1] * a.m[3][0];
        b.m[3][2] = a.m[0][0] * a.m[1][2] * a.m[3][1] + a.m[0][1] * a.m[1][0] * a.m[3][2] + a.m[0][2] * a.m[1][1] * a.m[3][0] - a.m[0][0] * a.m[1][1] * a.m[3][2] - a.m[0][1] * a.m[1][2] * a.m[3][0] - a.m[0][2] * a.m[1][0] * a.m[3][1];
        b.m[3][3] = a.m[0][0] * a.m[1][1] * a.m[2][2] + a.m[0][1] * a.m[1][2] * a.m[2][0] + a.m[0][2] * a.m[1][0] * a.m[2][1] - a.m[0][0] * a.m[1][2] * a.m[2][1] - a.m[0][1] * a.m[1][0] * a.m[2][2] - a.m[0][2] * a.m[1][1] * a.m[2][0];
        float t[] = {b.m[0][0] / d, b.m[0][1] / d, b.m[0][2] / d, b.m[0][3] / d,
                     b.m[1][0] / d, b.m[1][1] / d, b.m[1][2] / d, b.m[1][3] / d,
                     b.m[2][0] / d, b.m[2][1] / d, b.m[2][2] / d, b.m[2][3] / d,
                     b.m[3][0] / d, b.m[3][1] / d, b.m[3][2] / d, b.m[3][3] / d};
        return Matrix4x4(t);
    }
};

struct Ray {
public:
    Vector3 position = Vector3(0, 0, 0);
    Vector3 direction = Vector3(0, -1, 0);
    float distance = 0.0f;
    Ray () {}
    Ray (Vector3 s, Vector3 e) {
        this->position = s;
        this->direction = Vector3::normalize(e-s);
        this->distance = Vector3::distance(s, e);
    }
    
    Ray (int x, int y, Matrix4x4 view, Matrix4x4 proj) {
        Vector4 s(x, y, 0, 1);
        Vector4 e(x, y, 1, 1);
        Matrix4x4 invm = Matrix4x4::inverse(Matrix4x4::mul(view, proj));
        s = Matrix4x4::muld(s, invm);
        e = Matrix4x4::muld(e, invm);
        this->position = Vector3(s.x, s.y, s.z);
        this->direction = Vector3::normalize(Vector3(e.x-s.x, e.y-s.y, e.z-s.z));
        this->distance = 1000.0f;
    }
    
    float raytriangle (Vector3 v1, Vector3 v2, Vector3 v3) {
        float dist = -1.0f;
        Vector3 e1 = v2 - v1;
        Vector3 e2 = v3 - v1;
        Vector3 p = Vector3::cross(this->direction, e2);
        float d = Vector3::dot(e1, p);
        if (d < 0.0001f) { return dist; }
        Vector3 t = this->position - v1;
        float u = Vector3::dot(t, p);
        if (u < 0.0f || u > d) { return dist; }
        Vector3 q = Vector3::cross(t, e1);
        float v = Vector3::dot(this->direction, q);
        if (v < 0.0f || u + v > d) { return dist; }
        dist = Vector3::dot(e2, q);
        float f = 1.0f / d;
        dist *= f;
        u *= f;
        v *= f;
        dist = dist <= this->distance ? dist : -1.0f;
        return dist;
    }
    
    float raytriangle (Vector3 v1, Vector3 v2, Vector3 v3, float& u, float& v) {
        float dist = -1.0f;
        Vector3 e1 = v2 - v1;
        Vector3 e2 = v3 - v1;
        Vector3 p = Vector3::cross(this->direction, e2);
        float d = Vector3::dot(e1, p);
        if (d < 0.0001f) { return dist; }
        Vector3 t = this->position - v1;
        u = Vector3::dot(t, p);
        if (u < 0.0f || u > d) { return dist; }
        Vector3 q = Vector3::cross(t, e1);
        v = Vector3::dot(this->direction, q);
        if (v < 0.0f || u + v > d) { return dist; }
        dist = Vector3::dot(e2, q);
        float f = 1.0f / d;
        dist *= f;
        u *= f;
        v *= f;
        dist = dist <= this->distance ? dist : -1.0f;
        return dist;
    }
};

struct RaycastHit {
    Vector3 point;
    float u, v, distance;
    uint32_t triangleindex;
    operator bool () const { return this->distance > -1; }
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
        const uint16_t v16i = (uint16_t)(this->height * (1.0f-v)) % this->height;
        return this->pixels[v16i*this->width+u16i];
    }
    void setColor (float u, float v, uint16_t color) {
        const uint16_t u16i = (uint16_t)(this->width * u) % this->width;
        const uint16_t v16i = (uint16_t)(this->height * (1.0f-v)) % this->height;
        this->pixels[v16i*this->width+u16i] = color;
    }
};

namespace RenderType {
    enum Type {
        WireFrame,
        PolygonColor,
        PolygonNormal,
        PolygonTranslucent,
        PolygonTexture,
        PolygonTextureDoubleFace,
        PolygonTexturePerspectiveCorrect,
        PolygonTexturePerspectiveCorrectDoubleFace,
        Hide,
        None,
        // Line,
    };
}

struct Object3D {
public:
    Vector3 position;
    // Vector3 rotation;
    Quaternion rotation;
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
    
    Vector3 worldPosition () {
        return Matrix4x4::mul(Vector3(), this->worldMatrix());
    }
    
    Vector3 forward () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(mat.m[2][0], mat.m[2][1], mat.m[2][2]));
    }
    Vector3 back () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(-mat.m[2][0], -mat.m[2][1], -mat.m[2][2]));
    }
    Vector3 up () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(mat.m[1][0], mat.m[1][1], mat.m[1][2]));
    }
    Vector3 down () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(-mat.m[1][0], -mat.m[1][1], -mat.m[1][2]));
    }
    Vector3 right () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(-mat.m[0][0], -mat.m[0][1], -mat.m[0][2]));
    }
    Vector3 left () {
        const Matrix4x4 mat = this->worldMatrix();
        return Vector3::normalize(Vector3(mat.m[0][0], mat.m[0][1], mat.m[0][2]));
    }
    
    Matrix4x4 worldMatrix () {
        Matrix4x4 mat = Matrix4x4::identity();
        Object3D* obj = this;
        while (obj != NULL) {
            mat = Matrix4x4::mul(mat, Matrix4x4::scaleMatrix(obj->scale));
            mat = Matrix4x4::mul(mat, Matrix4x4::rotMatrix(obj->rotation));
            mat = Matrix4x4::mul(mat, Matrix4x4::moveMatrix(obj->position));
            obj = obj->parent;
        }
        return mat;
    }
    
    RaycastHit raycast (Ray r) {
        RaycastHit hit;
        Matrix4x4 mat = this->worldMatrix();
        hit.distance = -1.0f;
        hit.triangleindex = 0;
        
        float tu, tv;
        float u, v, dist;
        float bestdist = r.distance;
        for (int i = 0; i < this->mesh->faces.size(); ++i) {
            
            Vector3 v1 = Matrix4x4::mul(this->mesh->vertexs[this->mesh->faces[i].a], mat);
            Vector3 v2 = Matrix4x4::mul(this->mesh->vertexs[this->mesh->faces[i].b], mat);
            Vector3 v3 = Matrix4x4::mul(this->mesh->vertexs[this->mesh->faces[i].c], mat);
            
            dist = r.raytriangle(v1, v2, v3, u, v);
            if (dist > -1 && dist <= bestdist) {
                hit.triangleindex = i;
                bestdist = dist;
                hit.distance = dist;
                tu = u;
                tv = v;
            }
        }
        
        if (hit.distance > -1) {
            Vector2 uv1 = this->mesh->uv_vertexs[this->mesh->uv_faces[hit.triangleindex].a];
            Vector2 uv2 = this->mesh->uv_vertexs[this->mesh->uv_faces[hit.triangleindex].b];
            Vector2 uv3 = this->mesh->uv_vertexs[this->mesh->uv_faces[hit.triangleindex].c];
            float dbx = uv2.x-uv1.x;
            float dby = uv2.y-uv1.y;
            float dcx = uv3.x-uv1.x;
            float dcy = uv3.y-uv1.y;
            hit.u = dbx*tu+dcx*tv+uv1.x;
            hit.v = dby*tu+dcy*tv+uv1.y;
            hit.point = r.position + Vector3::normalize(r.direction) * hit.distance;
        }
        return hit;
    }
};


class E512Font {
public:
    virtual uint16_t getHeight (const uint8_t c) { return 12; }
    virtual uint16_t getWidth (const uint8_t c) { return 6; }
    virtual bool getPixel (const uint8_t c, uint16_t y, uint16_t x) { return true; }
};

class DefaultFont12x6 : public E512Font {
public:
    uint16_t getHeight (const uint8_t c) override { return this->font_height; }
    uint16_t getWidth (const uint8_t c) override { return this->font_width; }
    bool getPixel (const uint8_t c, uint16_t y, uint16_t x) override {
        return (data[(uint16_t)c * this->font_height + y] << x) & 0b00100000;
    }
private:
    uint8_t data[3072] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 15, 48, 0, 63, 31, 61, 10, 21, 16, 0, 0,
        0, 0, 48, 0, 32, 56, 60, 44, 24, 48, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 15, 3, 0, 1, 2, 2, 0, 0,
        0, 0, 0, 1, 15, 3, 0, 1, 2, 2, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 48, 0, 32, 56, 60, 44, 24, 48, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 4, 4, 4, 4, 4, 0, 4, 0, 0,
        0, 0, 20, 20, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 10, 31, 10, 10, 10, 10, 31, 10, 0, 0,
        0, 4, 14, 21, 20, 12, 6, 5, 21, 14, 4, 0,
        0, 0, 0, 0, 57, 42, 60, 15, 21, 39, 0, 0,
        0, 12, 18, 18, 12, 25, 37, 34, 35, 29, 0, 0,
        0, 0, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 4, 4, 8, 8, 8, 4, 4, 2, 0,
        0, 0, 8, 4, 4, 2, 2, 2, 4, 4, 8, 0,
        0, 0, 0, 0, 0, 0, 4, 21, 14, 21, 4, 0,
        0, 0, 0, 0, 0, 0, 4, 4, 31, 4, 4, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 4,
        0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
        0, 0, 2, 2, 4, 4, 4, 8, 8, 8, 16, 16,
        0, 0, 0, 30, 18, 22, 30, 26, 18, 30, 0, 0,
        0, 0, 0, 4, 12, 4, 4, 4, 4, 14, 0, 0,
        0, 0, 0, 30, 2, 2, 30, 16, 16, 30, 0, 0,
        0, 0, 0, 30, 2, 2, 30, 2, 2, 30, 0, 0,
        0, 0, 0, 18, 18, 18, 30, 2, 2, 2, 0, 0,
        0, 0, 0, 30, 16, 16, 30, 2, 2, 30, 0, 0,
        0, 0, 0, 30, 16, 16, 30, 18, 18, 30, 0, 0,
        0, 0, 0, 30, 2, 2, 2, 2, 2, 2, 0, 0,
        0, 0, 0, 30, 18, 18, 30, 18, 18, 30, 0, 0,
        0, 0, 0, 30, 18, 18, 30, 2, 2, 30, 0, 0,
        0, 0, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0,
        0, 0, 0, 0, 8, 0, 0, 12, 8, 8, 0, 0,
        0, 0, 0, 2, 4, 8, 16, 8, 4, 2, 0, 0,
        0, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0,
        0, 0, 0, 8, 4, 2, 1, 2, 4, 8, 0, 0,
        0, 0, 14, 17, 17, 1, 2, 4, 4, 0, 4, 0,
        0, 0, 14, 17, 29, 19, 23, 27, 27, 23, 14, 0,
        0, 0, 0, 4, 4, 10, 10, 14, 17, 17, 0, 0,
        0, 0, 0, 30, 17, 17, 30, 17, 17, 31, 0, 0,
        0, 0, 0, 14, 17, 16, 16, 16, 17, 14, 0, 0,
        0, 0, 0, 30, 17, 17, 17, 17, 17, 30, 0, 0,
        0, 0, 0, 31, 16, 16, 31, 16, 16, 31, 0, 0,
        0, 0, 0, 31, 16, 16, 31, 16, 16, 16, 0, 0,
        0, 0, 0, 14, 17, 16, 16, 19, 17, 15, 0, 0,
        0, 0, 0, 17, 17, 17, 31, 17, 17, 17, 0, 0,
        0, 0, 0, 14, 4, 4, 4, 4, 4, 14, 0, 0,
        0, 0, 0, 15, 2, 2, 2, 2, 2, 28, 0, 0,
        0, 0, 0, 18, 20, 24, 24, 20, 18, 17, 0, 0,
        0, 0, 0, 16, 16, 16, 16, 16, 16, 30, 0, 0,
        0, 0, 0, 17, 27, 27, 21, 21, 21, 21, 0, 0,
        0, 0, 0, 17, 25, 21, 21, 21, 19, 17, 0, 0,
        0, 0, 0, 14, 17, 17, 17, 17, 17, 14, 0, 0,
        0, 0, 0, 30, 17, 17, 30, 16, 16, 16, 0, 0,
        0, 0, 0, 14, 17, 17, 17, 21, 19, 15, 0, 0,
        0, 0, 0, 30, 17, 17, 30, 20, 18, 17, 0, 0,
        0, 0, 0, 14, 17, 16, 14, 1, 17, 14, 0, 0,
        0, 0, 0, 31, 4, 4, 4, 4, 4, 4, 0, 0,
        0, 0, 0, 17, 17, 17, 17, 17, 17, 14, 0, 0,
        0, 0, 0, 17, 17, 17, 17, 10, 14, 4, 0, 0,
        0, 0, 0, 21, 21, 21, 21, 21, 10, 10, 0, 0,
        0, 0, 0, 17, 17, 10, 4, 10, 17, 17, 0, 0,
        0, 0, 0, 17, 17, 27, 14, 4, 4, 4, 0, 0,
        0, 0, 0, 31, 1, 2, 4, 8, 16, 31, 0, 0,
        0, 0, 14, 8, 8, 8, 8, 8, 8, 8, 14, 0,
        0, 0, 17, 10, 4, 31, 4, 31, 4, 4, 4, 0,
        0, 0, 14, 2, 2, 2, 2, 2, 2, 2, 14, 0,
        0, 4, 10, 17, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0,
        0, 0, 16, 8, 4, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 28, 2, 14, 18, 18, 29, 0, 0,
        0, 0, 16, 16, 16, 28, 18, 18, 18, 28, 0, 0,
        0, 0, 0, 0, 0, 14, 16, 16, 16, 14, 0, 0,
        0, 0, 2, 2, 2, 14, 18, 18, 18, 14, 0, 0,
        0, 0, 0, 0, 0, 30, 18, 30, 16, 30, 0, 0,
        0, 0, 3, 4, 4, 15, 4, 4, 4, 4, 0, 0,
        0, 0, 0, 0, 0, 14, 18, 18, 18, 14, 18, 12,
        0, 0, 16, 16, 16, 28, 18, 18, 18, 18, 0, 0,
        0, 0, 0, 4, 0, 4, 4, 4, 4, 4, 0, 0,
        0, 0, 0, 4, 0, 4, 4, 4, 4, 4, 24, 0,
        0, 0, 16, 16, 16, 18, 20, 28, 18, 17, 0, 0,
        0, 0, 12, 4, 4, 4, 4, 4, 4, 14, 0, 0,
        0, 0, 0, 0, 0, 30, 21, 21, 21, 21, 0, 0,
        0, 0, 0, 0, 0, 28, 18, 18, 18, 18, 0, 0,
        0, 0, 0, 0, 0, 14, 17, 17, 17, 14, 0, 0,
        0, 0, 0, 0, 0, 30, 18, 18, 18, 28, 16, 16,
        0, 0, 0, 0, 0, 30, 18, 18, 18, 14, 2, 2,
        0, 0, 0, 0, 0, 22, 24, 16, 16, 16, 0, 0,
        0, 0, 0, 0, 12, 18, 8, 4, 18, 12, 0, 0,
        0, 0, 0, 0, 8, 30, 8, 8, 8, 6, 0, 0,
        0, 0, 0, 0, 0, 18, 18, 18, 18, 14, 0, 0,
        0, 0, 0, 0, 0, 17, 17, 10, 10, 4, 0, 0,
        0, 0, 0, 0, 0, 17, 21, 21, 31, 10, 0, 0,
        0, 0, 0, 0, 0, 17, 10, 4, 10, 17, 0, 0,
        0, 0, 0, 0, 0, 17, 17, 10, 4, 4, 8, 0,
        0, 0, 0, 0, 0, 31, 2, 4, 8, 31, 0, 0,
        0, 0, 6, 8, 8, 8, 24, 8, 8, 8, 6, 0,
        0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0,
        0, 0, 24, 4, 4, 4, 6, 4, 4, 4, 24, 0,
        0, 0, 0, 0, 8, 21, 2, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 8, 20, 8, 0, 0,
        0, 28, 16, 16, 16, 16, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 14, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0,
        0, 0, 0, 0, 0, 12, 12, 0, 0, 0, 0, 0,
        0, 0, 0, 31, 1, 1, 31, 1, 2, 28, 0, 0,
        0, 0, 0, 0, 0, 31, 1, 5, 4, 8, 0, 0,
        0, 0, 0, 0, 2, 4, 28, 4, 4, 4, 0, 0,
        0, 0, 0, 0, 8, 30, 18, 4, 4, 8, 0, 0,
        0, 0, 0, 0, 0, 31, 4, 4, 4, 31, 0, 0,
        0, 0, 0, 0, 2, 31, 2, 6, 10, 18, 0, 0,
        0, 0, 0, 0, 8, 15, 21, 6, 4, 2, 0, 0,
        0, 0, 0, 0, 0, 30, 2, 2, 2, 31, 0, 0,
        0, 0, 0, 0, 0, 15, 1, 15, 1, 15, 0, 0,
        0, 0, 0, 0, 0, 20, 21, 1, 2, 28, 0, 0,
        0, 0, 0, 0, 0, 0, 31, 0, 0, 0, 0, 0,
        0, 0, 0, 63, 1, 5, 4, 8, 8, 16, 0, 0,
        0, 0, 1, 6, 28, 36, 4, 4, 4, 4, 0, 0,
        0, 0, 4, 31, 17, 17, 1, 1, 2, 12, 0, 0,
        0, 0, 0, 31, 4, 4, 4, 4, 4, 31, 0, 0,
        0, 0, 2, 31, 2, 6, 10, 18, 38, 2, 0, 0,
        0, 0, 4, 31, 5, 5, 9, 9, 18, 18, 0, 0,
        0, 0, 4, 30, 4, 31, 2, 2, 2, 2, 0, 0,
        0, 0, 6, 9, 9, 17, 1, 2, 4, 24, 0, 0,
        0, 0, 8, 15, 18, 18, 2, 4, 4, 8, 0, 0,
        0, 0, 0, 31, 1, 1, 1, 1, 1, 31, 0, 0,
        0, 0, 18, 63, 18, 18, 18, 2, 4, 4, 0, 0,
        0, 0, 0, 28, 1, 29, 1, 1, 2, 28, 0, 0,
        0, 0, 0, 63, 1, 2, 4, 12, 18, 33, 0, 0,
        0, 0, 16, 63, 17, 17, 18, 16, 16, 15, 0, 0,
        0, 0, 0, 17, 9, 1, 2, 2, 4, 8, 0, 0,
        0, 0, 6, 9, 9, 23, 1, 2, 4, 24, 0, 0,
        0, 0, 7, 10, 7, 26, 2, 2, 4, 24, 0, 0,
        0, 0, 0, 20, 21, 21, 1, 1, 2, 28, 0, 0,
        0, 0, 15, 0, 31, 2, 2, 2, 4, 8, 0, 0,
        0, 0, 8, 8, 8, 12, 10, 8, 8, 8, 0, 0,
        0, 0, 2, 31, 2, 2, 2, 4, 4, 8, 0, 0,
        0, 0, 0, 31, 0, 0, 0, 0, 0, 31, 0, 0,
        0, 0, 0, 31, 1, 1, 14, 6, 9, 16, 0, 0,
        0, 0, 4, 30, 6, 12, 23, 4, 4, 4, 0, 0,
        0, 0, 0, 3, 3, 2, 4, 4, 8, 16, 0, 0,
        0, 0, 0, 18, 18, 18, 18, 17, 17, 33, 0, 0,
        0, 0, 0, 16, 19, 28, 16, 16, 16, 31, 0, 0,
        0, 0, 0, 31, 1, 1, 1, 2, 4, 24, 0, 0,
        0, 0, 0, 8, 24, 36, 4, 2, 1, 1, 0, 0,
        0, 0, 4, 31, 4, 22, 21, 37, 12, 4, 0, 0,
        0, 0, 0, 7, 29, 1, 18, 12, 6, 2, 0, 0,
        0, 0, 24, 7, 0, 24, 7, 0, 24, 7, 0, 0,
        0, 0, 4, 4, 8, 8, 16, 18, 63, 1, 0, 0,
        0, 0, 2, 18, 12, 6, 5, 8, 8, 16, 0, 0,
        0, 0, 0, 28, 8, 8, 30, 8, 8, 7, 0, 0,
        0, 0, 0, 22, 25, 50, 16, 8, 8, 4, 0, 0,
        0, 0, 0, 30, 2, 2, 2, 2, 2, 31, 0, 0,
        0, 0, 0, 31, 1, 1, 31, 1, 1, 31, 0, 0,
        0, 0, 0, 31, 0, 31, 1, 1, 2, 28, 0, 0,
        0, 0, 0, 18, 18, 18, 18, 4, 4, 8, 0, 0,
        0, 0, 0, 20, 20, 20, 21, 37, 38, 36, 0, 0,
        0, 0, 0, 16, 16, 16, 17, 18, 20, 24, 0, 0,
        0, 0, 0, 31, 17, 17, 17, 17, 17, 14, 0, 0,
        0, 0, 0, 31, 17, 17, 1, 1, 2, 12, 0, 0,
        0, 0, 0, 16, 9, 1, 1, 1, 2, 28, 0, 0,
        0, 0, 40, 20, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 16, 40, 16, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    uint16_t font_height = 12;
    uint16_t font_width = 6;
};

void atokInit (E512Array< E512Array<uint8_t> >& a, E512Array< E512Array<uint8_t> >& b) {
    uint8_t da[396] = {46,0,0,44,0,0,119,111,0,120,97,0,120,105,0,120,117,0,120,101,0,120,111,0,120,121,97,120,121,117,120,121,111,120,116,117,107,107,0,115,115,0,116,116,0,104,104,0,109,109,0,121,121,0,114,114,0,119,119,0,103,103,0,122,122,0,100,100,0,98,98,0,112,112,0,107,121,97,107,121,117,107,121,111,115,121,97,115,121,117,115,121,111,116,121,97,116,121,117,116,121,111,104,121,97,104,121,117,104,121,111,109,121,97,109,121,117,109,121,111,114,121,97,114,121,117,114,121,111,103,121,97,103,121,117,103,121,111,122,121,97,122,121,117,122,121,111,106,121,97,106,121,117,106,121,111,106,97,0,106,117,0,106,111,0,98,121,97,98,121,117,98,121,111,112,121,97,112,121,117,112,121,111,107,97,0,107,105,0,107,117,0,107,101,0,107,111,0,115,97,0,115,105,0,115,117,0,115,101,0,115,111,0,116,97,0,116,105,0,116,117,0,116,101,0,116,111,0,110,97,0,110,105,0,110,117,0,110,101,0,110,111,0,104,97,0,104,105,0,104,117,0,104,101,0,104,111,0,109,97,0,109,105,0,109,117,0,109,101,0,109,111,0,121,97,0,121,117,0,121,111,0,114,97,0,114,105,0,114,117,0,114,101,0,114,111,0,119,97,0,103,97,0,103,105,0,103,117,0,103,101,0,103,111,0,122,97,0,122,105,0,122,117,0,122,101,0,122,111,0,100,97,0,100,105,0,100,117,0,100,101,0,100,111,0,98,97,0,98,105,0,98,117,0,98,101,0,98,111,0,112,97,0,112,105,0,112,117,0,112,101,0,112,111,0,97,0,0,105,0,0,117,0,0,101,0,0,111,0,0,110,110,0,45,0,0,};
    uint8_t db[396] = {161,0,0,164,0,0,166,0,0,167,0,0,168,0,0,169,0,0,170,0,0,171,0,0,172,0,0,173,0,0,174,0,0,175,0,0,175,107,0,175,115,0,175,116,0,175,104,0,175,109,0,175,121,0,175,114,0,175,119,0,175,103,0,175,122,0,175,100,0,175,98,0,175,112,0,183,172,0,183,173,0,183,174,0,188,172,0,188,173,0,188,174,0,193,172,0,193,173,0,193,174,0,203,172,0,203,173,0,203,174,0,208,172,0,208,173,0,208,174,0,216,172,0,216,173,0,216,174,0,183,222,172,183,222,173,183,222,174,188,222,172,188,222,173,188,222,174,188,222,172,188,222,173,188,222,174,188,222,172,188,222,173,188,222,174,203,222,172,203,222,173,203,222,174,203,223,172,203,223,173,203,223,174,182,0,0,183,0,0,184,0,0,185,0,0,186,0,0,187,0,0,188,0,0,189,0,0,190,0,0,191,0,0,192,0,0,193,0,0,194,0,0,195,0,0,196,0,0,197,0,0,198,0,0,199,0,0,200,0,0,201,0,0,202,0,0,203,0,0,204,0,0,205,0,0,206,0,0,207,0,0,208,0,0,209,0,0,210,0,0,211,0,0,212,0,0,213,0,0,214,0,0,215,0,0,216,0,0,217,0,0,218,0,0,219,0,0,220,0,0,182,222,0,183,222,0,184,222,0,185,222,0,186,222,0,187,222,0,188,222,0,189,222,0,190,222,0,191,222,0,192,222,0,193,222,0,194,222,0,195,222,0,196,222,0,202,222,0,203,222,0,204,222,0,205,222,0,206,222,0,202,223,0,203,223,0,204,223,0,205,223,0,206,223,0,177,0,0,178,0,0,179,0,0,180,0,0,181,0,0,221,0,0,176,0,0,};
    E512Array<uint8_t> t;
    for (int i = 0; i < 132; ++i) {
        t.clear();
        for (int j = 0; j < 3; ++j) {
            uint8_t c = da[i*3+j];
            if (c > 0) { t.emplace_back(c); }
        }
        a.emplace_back(t);
    }
    for (int i = 0; i < 132; ++i) {
        t.clear();
        for (int j = 0; j < 3; ++j) {
            uint8_t c = db[i*3+j];
            if (c > 0) { t.emplace_back(c); }
        }
        b.emplace_back(t);
    }
}
void endReplace (E512Array<uint8_t>& t, E512Array<uint8_t>& a, E512Array<uint8_t>& b) {
    if (t.size() < a.size()) { return; }
    for (int i = 0; i < a.size(); ++i) {
        if (t[t.size()-a.size()+i] != a[i]) { return; }
    }
    for (int i = 0; i < a.size(); ++i) { t.pop_back(); }
    for (auto&& i : b) { t.emplace_back(i); }
}
E512Array<uint8_t> atok (const char* cp) {
    static bool initf = true;
    static E512Array< E512Array<uint8_t> > a, b;
    E512Array<uint8_t> r;
    if (initf) {
        initf = false;
        atokInit(a, b);
    }
    
    int idx = 0;
    while (true) {
        char c = cp[idx];
        idx += 1;
        if (c == '\0') { break; }
        r.emplace_back(c);
        for (int i = 0; i < a.size(); ++i) { endReplace(r, a[i], b[i]); }
    }
    for (auto&& i : r) { if (i == 'n') { i = 221; } }
    
    return r;
}
E512Array<uint8_t> atok (E512Array<uint8_t>& u8a) {
    static bool initf = true;
    static E512Array< E512Array<uint8_t> > a, b;
    E512Array<uint8_t> r;
    if (initf) {
        initf = false;
        atokInit(a, b);
    }
    
    for (auto&& c : u8a) {
        r.emplace_back(c);
        for (int i = 0; i < a.size(); ++i) { endReplace(r, a[i], b[i]); }
    }
    for (auto&& i : r) { if (i == 'n') { i = 221; } }
    
    return r;
}

E512Array<uint8_t> cptou8a (const char* cp) {
    E512Array<uint8_t> r;
    int i = 0;
    while (true) {
        char c = cp[i];
        i += 1;
        if (c == '\0') { break; }
        r.emplace_back(c);
    }
    return r;
}

E512Array<uint8_t> cptoe512array (char* cp) {
    E512Array<uint8_t> r;
    for (int i = 0; i < 32; ++i)  {
        uint8_t c = cp[i];
        if (c == '\0') { break; }
        r.emplace_back(c);
    }
    return r;
}
E512Array<uint8_t> cptoe512array (char* cp, uint8_t n) {
    E512Array<uint8_t> r;
    int cnt = 0;
    bool dot = false;
    for (int i = 0; i < 32; ++i)  {
        uint8_t c = cp[i];
        if (c == '\0') { break; }
        if (dot) { cnt += 1; }
        if (c == '.') { dot = true; }
        r.emplace_back(c);
        if (cnt >= n) { break; }
    }
    while (cnt < n) {
        r.emplace_back('0');
        cnt += 1;
    }
    return r;
}
E512Array<uint8_t> numtostr (int v) {
    char cp[128];
    sprintf(cp, "%d", v);
    return cptoe512array(cp);
}
#if defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2)
    E512Array<uint8_t> numtostr (float v, uint8_t n = 4) {
        char cp[128];
        n = n > 64 ? 64 : n;
        dtostrf(v, 0, n, cp);
        return cptoe512array(cp, n);
    }
#elif defined(ARDUINO_ARCH_RP2040)
    #include <avr/dtostrf.h>
    E512Array<uint8_t> numtostr (float v, uint8_t n = 4) {
        char cp[128];
        n = n > 64 ? 64 : n;
        dtostrf(v, 0, n, cp);
        return cptoe512array(cp, n);
    }
#else
    E512Array<uint8_t> numtostr (float v, uint8_t n = 4) {
        char cp[128];
        sprintf(cp, "%f", v);
        return cptoe512array(cp, n);
    }
#endif
