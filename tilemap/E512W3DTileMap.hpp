#pragma once

#include "E512W3D.hpp"


bool aabb (int asx, int asy, int aex, int aey, int bsx, int bsy, int bex, int bey) {
    return min(aex, bex) > max(asx, bsx) && min(aey, bey) > max(asy, bsy);
}

struct E512W3DTileLayer {
    int16_t tex_x = 0;
    int16_t tex_y = 0;
    uint8_t anim = 0;
    uint8_t anim_position = 0;
    uint16_t anim_cnt = 0;
    uint16_t anim_wait = 8;
    
    int16_t tex_h = 1;
    int16_t start_h = 0;
    
    bool use = false;
    
    bool autotile = false;
    uint16_t autotile_plug = 0;
    uint16_t autotile_jack = 0;
    
    E512W3DTileLayer () {}
    E512W3DTileLayer (int16_t tex_x, int16_t tex_y) {
        this->tex_x = tex_x;
        this->tex_y = tex_y;
        this->use = true;
    }
    E512W3DTileLayer (int16_t tex_x, int16_t tex_y, uint8_t anim) {
        this->tex_x = tex_x;
        this->tex_y = tex_y;
        this->anim = anim;
        this->use = true;
    }
    E512W3DTileLayer (int16_t tex_x, int16_t tex_y, uint8_t anim, int16_t tex_h, int16_t start_h) {
        this->tex_x = tex_x;
        this->tex_y = tex_y;
        this->anim = anim;
        this->tex_h = tex_h;
        this->start_h = start_h;
        this->use = true;
    }
};

struct E512W3DTile {
    uint16_t collision_layer = 0;
    
    E512W3DTileLayer a;
    E512W3DTileLayer b;
    
    E512W3DTile () {}
    
    E512W3DTile (E512W3DTileLayer a, uint16_t collision_layer) {
        this->collision_layer = collision_layer;
        this->a = a;
    }
    
    E512W3DTile (E512W3DTileLayer a, E512W3DTileLayer b, uint16_t collision_layer) {
        this->collision_layer = collision_layer;
        this->a = a;
        this->b = b;
    }
    
    E512W3DTile (int16_t tex_x, int16_t tex_y, uint8_t anim, uint16_t collision_layer) {
        this->collision_layer = collision_layer;
        this->a.tex_x = tex_x;
        this->a.tex_y = tex_y;
        this->a.anim = anim;
        this->a.use = true;
    }
    
    E512W3DTile (int16_t tex_x, int16_t tex_y, uint8_t anim, int16_t btex_x, int16_t btex_y, uint8_t banim, int16_t btex_h, int16_t bstart_h, uint16_t collision_layer) {
        this->collision_layer = collision_layer;
        this->a.tex_x = tex_x;
        this->a.tex_y = tex_y;
        this->a.anim = anim;
        this->a.use = true;
        
        this->b.tex_x = btex_x;
        this->b.tex_y = btex_y;
        this->b.anim = banim;
        this->b.tex_h = btex_h;
        this->b.start_h = bstart_h;
        this->b.use = true;
    }
};

class E512W3DTileMap {
public:
    E512Array<uint16_t> dat;
    int width = 0;
    int height = 0;
    int z = 0;
    E512Array<E512W3DTile> tile;// 0 blanktile, 1-1023 user tile
    Texture* texture = NULL;
    int tex_w = 0;
    int tex_h = 0;
    
    int max_tex_h = 0;
    
    E512W3DTileMap () {}
    E512W3DTileMap (int width, int height) {
        this->width = width;
        this->height = height;
        int wh = width * height;
        this->dat.reserve(wh);
        for (int i = 0; i < wh; ++i) { this->dat[i] = 0; }
    }
    
    bool isInside (int x, int y) {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height) { return false; }
        return true;
    }
    
    void update () {
        this->max_tex_h = 0;
        for (auto&& i : this->tile) {
            this->max_tex_h = max(i.b.tex_h-1, this->max_tex_h);
            if (i.a.anim > 0) {
                i.a.anim_wait = max((int)i.a.anim_wait, 1);
                if (i.a.anim_cnt % i.a.anim_wait == 0) {
                    i.a.anim_position = (i.a.anim_position + 1) % i.a.anim;
                }
                i.a.anim_cnt += 1;
                if (i.a.anim_cnt >= i.a.anim * i.a.anim_wait) { i.a.anim_cnt = 0; }
            }
            if (i.b.anim > 0) {
                i.b.anim_wait = max((int)i.b.anim_wait, 1);
                if (i.b.anim_cnt % i.b.anim_wait == 0) {
                    i.b.anim_position = (i.b.anim_position + 1) % i.b.anim;
                }
                i.b.anim_cnt += 1;
                if (i.b.anim_cnt >= i.b.anim * i.b.anim_wait) { i.b.anim_cnt = 0; }
            }
        }
    }
    
    uint16_t getTileIndex (int x, int y) { return this->dat[y*this->width+x] & 0b1111111111; }
    void setTileIndex (int x, int y, uint16_t t) { this->dat[y*this->width+x] = (t & 0b1111111111) | (this->dat[y*this->width+x] & 0b1111110000000000); }
    E512W3DTile getTile (int x, int y) { return this->tile[this->getTileIndex(x, y)]; }
    uint16_t getTileValue (int x, int y) { return (this->dat[y*this->width+x] >> 10) & 0b111111; }
    void setTileValue (int x, int y, uint16_t t) { this->dat[y*this->width+x] = (this->dat[y*this->width+x] & 0b1111111111) | ((t & 0b111111) << 10); }
    
    uint16_t getTileIndexS (int x, int y) {
        if (!this->isInside(x, y)) { return 0; }
        return this->dat[y*this->width+x] & 0b1111111111;
    }
    void setTileIndexS (int x, int y, uint16_t t) {
        if (!this->isInside(x, y)) { return; }
        this->dat[y*this->width+x] = (t & 0b1111111111) | (this->dat[y*this->width+x] & 0b1111110000000000);
    }
    E512W3DTile getTileS (int x, int y) {
        if (!this->isInside(x, y)) { return this->tile[0]; }
        return this->tile[this->getTileIndex(x, y)];
    }
    uint16_t getTileValueS (int x, int y) {
        if (!this->isInside(x, y)) { return 0; }
        return (this->dat[y*this->width+x] >> 10) & 0b111111;
    }
    void setTileValueS (int x, int y, uint16_t t) {
        if (!this->isInside(x, y)) { return; }
        this->dat[y*this->width+x] = (this->dat[y*this->width+x] & 0b1111111111) | ((t & 0b111111) << 10);
    }
};

struct Object2D {
    Texture* texture = NULL;
    int16_t tex_x = 0;
    int16_t tex_y = 0;
    int16_t tex_w = 0;
    int16_t tex_h = 0;
    
    Vector3 position;
    
    Object2D () {}
    Object2D (Texture* texture) {
        this->texture = texture;
        this->tex_w = texture->width;
        this->tex_h = texture->height;
    }
    Object2D (Vector2 position, Texture* texture) {
        this->position = Vector3(position.x, position.y, 0);
        this->texture = texture;
        this->tex_w = texture->width;
        this->tex_h = texture->height;
    }
};

struct E512W3DGameObject2D : Object2D {
private:
    struct MoveStruct {
        bool ret = false;
        int p;
        float l, r, d, u;
        int tl, tr, td, tu;
        float px, py, v;
        MoveStruct (E512W3DTileMap& tm, E512W3DGameObject2D& o, float& v) {
            this->v = v;
            this->px = o.position.x;
            this->py = o.position.y;
            this->l = o.cl+o.position.x;
            this->r = o.cr+o.position.x;
            this->d = o.cd+o.position.y;
            this->u = o.cu+o.position.y;
            this->tl = max((int)(l/tm.tex_w), 0);
            this->tr = min((int)((r-1)/tm.tex_w), tm.width-1);
            this->td = max((int)(d/tm.tex_h), 0);
            this->tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        }
    };
    
    void moveRight (E512W3DTileMap& tm, MoveStruct& s) {
        s.tr = min((int)((s.r+s.v)/tm.tex_w), tm.width-1);
        s.p = s.r+s.v;
        for (int y = s.td; y <= s.tu; ++y) {
            for (int x = s.tl; x <= s.tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                s.ret = true;
                s.p = min(s.p, x*tm.tex_w);
            }
        }
    }
    void moveRightAfter (MoveStruct& s) {
        if (s.ret) { s.v -= (s.px+this->cr-1+s.v) - s.p; }
        s.v = max(s.v, 0.0f);
        this->position.x += s.v;
        if (s.ret) { this->position.x -= 0.5f; }
    }
    void moveLeft (E512W3DTileMap& tm, MoveStruct& s) {
        s.tl = max((int)((s.l+s.v)/tm.tex_w), 0);
        s.p = s.l+s.v;
        for (int y = s.td; y <= s.tu; ++y) {
            for (int x = s.tl; x <= s.tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                s.ret = true;
                s.p = max(s.p, x*tm.tex_w+tm.tex_w);
            }
        }
    }
    void moveLeftAfter (MoveStruct& s) {
        if (s.ret) { s.v += s.p - (s.px+s.v+this->cl); }
        s.v = min(s.v, 0.0f);
        this->position.x += s.v;
        if (s.ret) { this->position.x += 0.5f; }
    }
    
    void moveUp (E512W3DTileMap& tm, MoveStruct& s) {
        s.tu = min((int)((s.u+s.v)/tm.tex_h), tm.height-1);
        s.p = s.u+s.v;
        for (int y = s.td; y <= s.tu; ++y) {
            for (int x = s.tl; x <= s.tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                s.ret = true;
                s.p = min(s.p, y*tm.tex_h);
            }
        }
    }
    void moveUpAfter (MoveStruct& s) {
        if (s.ret) { s.v -= (s.py+this->cu-1+s.v) - s.p; }
        s.v = max(s.v, 0.0f);
        this->position.y += s.v;
        if (s.ret) { this->position.y -= 0.5f; }
    }
    void moveDown (E512W3DTileMap& tm, MoveStruct& s) {
        s.td = max((int)((s.d+s.v)/tm.tex_h), 0);
        s.p = s.d+s.v;
        for (int y = s.td; y <= s.tu; ++y) {
            for (int x = s.tl; x <= s.tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                s.ret = true;
                s.p = max(s.p, y*tm.tex_h+tm.tex_h);
            }
        }
    }
    void moveDownAfter (MoveStruct& s) {
        if (s.ret) { s.v += s.p - (s.py+s.v+this->cd); }
        s.v = min(s.v, 0.0f);
        this->position.y += s.v;
        if (s.ret) { this->position.y += 0.5f; }
    }
public:
    int16_t cl = 0;
    int16_t cr = 0;
    int16_t cu = 0;
    int16_t cd = 0;
    Vector2 velocity;
    uint16_t collision_layer = 1;
    E512W3DGameObject2D () {}
    E512W3DGameObject2D (Texture* texture) : Object2D (texture) {}
    E512W3DGameObject2D (Vector2 position, Texture* texture) : Object2D (position, texture) {}
    void collisionFitTexture () {
        this->cl = 0;
        this->cu = this->tex_h;
        this->cr = this->tex_w;
        this->cd = 0;
    }
    
    bool moveX (E512W3DTileMap& tm) {
        if (this->velocity.x == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.x);
        if (this->velocity.x > 0) {
            this->moveRight(tm, s);
            this->moveRightAfter(s);
        }
        if (this->velocity.x < 0) {
            this->moveLeft(tm, s);
            this->moveLeftAfter(s);
        }
        return s.ret;
    }
    bool moveY (E512W3DTileMap& tm) {
        if (this->velocity.y == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.y);
        if (this->velocity.y > 0) {
            this->moveUp(tm, s);
            this->moveUpAfter(s);
        }
        if (this->velocity.y < 0) {
            this->moveDown(tm, s);
            this->moveDownAfter(s);
        }
        return s.ret;
    }
    
    bool moveX (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) {
        if (this->velocity.x == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.x);
        if (this->velocity.x > 0) {
            this->moveRight(tm, s);
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d, s.r+s.v, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = min(s.p, (int)gx);
                }
            }
            this->moveRightAfter(s);
        }
        if (this->velocity.x < 0) {
            this->moveLeft(tm, s);
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l+s.v, s.d, s.r, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = max(s.p, (int)gx+i.cr);
                }
            }
            this->moveLeftAfter(s);
        }
        return s.ret;
    }
    bool moveY (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) {
        if (this->velocity.y == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.y);
        if (this->velocity.y > 0) {
            this->moveUp(tm, s);
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d, s.r, s.u+s.v, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = min(s.p, (int)gy);
                }
            }
            this->moveUpAfter(s);
        }
        if (this->velocity.y < 0) {
            this->moveDown(tm, s);
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d+s.v, s.r, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = max(s.p, (int)gy+i.cu);
                }
            }
            this->moveDownAfter(s);
        }
        return s.ret;
    }
    
    bool moveX (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) {
        if (this->velocity.x == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.x);
        if (this->velocity.x > 0) {
            this->moveRight(tm, s);
            for (auto&& j : o) {
                E512W3DGameObject2D& i = *j;
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d, s.r+s.v, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = min(s.p, (int)gx);
                }
            }
            this->moveRightAfter(s);
        }
        if (this->velocity.x < 0) {
            this->moveLeft(tm, s);
            for (auto&& j : o) {
                E512W3DGameObject2D& i = *j;
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l+s.v, s.d, s.r, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = max(s.p, (int)gx+i.cr);
                }
            }
            this->moveLeftAfter(s);
        }
        return s.ret;
    }
    bool moveY (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) {
        if (this->velocity.y == 0) { return false; }
        MoveStruct s(tm, *this, this->velocity.y);
        if (this->velocity.y > 0) {
            this->moveUp(tm, s);
            for (auto&& j : o) {
                E512W3DGameObject2D& i = *j;
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d, s.r, s.u+s.v, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = min(s.p, (int)gy);
                }
            }
            this->moveUpAfter(s);
        }
        if (this->velocity.y < 0) {
            this->moveDown(tm, s);
            for (auto&& j : o) {
                E512W3DGameObject2D& i = *j;
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(s.l, s.d+s.v, s.r, s.u, gx, gy, gx+i.cr, gy+i.cu)) {
                    s.ret = true;
                    s.p = max(s.p, (int)gy+i.cu);
                }
            }
            this->moveDownAfter(s);
        }
        return s.ret;
    }
    
    bool isGround (E512W3DTileMap& tm) { return this->isCollision(tm, 0, 0, 0, 1); }
    bool isGround (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) { return this->isCollision(tm, o, 0, 0, 0, 1); }
    bool isGround (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) { return this->isCollision(tm, o, 0, 0, 0, 1); }
    
    bool objectAABB (E512W3DGameObject2D& o, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        const int al = this->cl+this->position.x-extend_l;
        const int ar = this->cr+this->position.x+extend_r;
        const int ad = this->cd+this->position.y-extend_d;
        const int au = this->cu+this->position.y+extend_u;
        const int bl = o.cl+o.position.x;
        const int br = o.cr+o.position.x;
        const int bd = o.cd+o.position.y;
        const int bu = o.cu+o.position.y;
        return aabb(al, ad, ar, au, bl, bd, br, bu);
    }
    
    bool isCollision (E512W3DGameObject2D& o, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        if ((this->collision_layer & o.collision_layer) == 0) { return false; }
        return this->objectAABB(o, extend_l, extend_u, extend_r, extend_d);
    }
    bool isCollision (E512W3DTileMap& tm, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        float l = this->cl+this->position.x-extend_l;
        float r = this->cr+this->position.x+extend_r;
        float d = this->cd+this->position.y-extend_d;
        float u = this->cu+this->position.y+extend_u;
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        for (int y = td; y <= tu; ++y) {
            for (int x = tl; x <= tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                return true;
            }
        }
        return false;
    }
    
    bool isCollision (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        if (this->isCollision(tm, extend_l, extend_u, extend_r, extend_d)) { return true; }
        for (auto&& i : o) {
            if (this->isCollision(i, extend_l, extend_u, extend_r, extend_d)) { return true; }
        }
        return false;
    }
    
    bool isCollision (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        if (this->isCollision(tm, extend_l, extend_u, extend_r, extend_d)) { return true; }
        for (auto&& i : o) {
            if (this->isCollision(*i, extend_l, extend_u, extend_r, extend_d)) { return true; }
        }
        return false;
    }
    
    E512Array<E512Point> collisionTilePositionList (E512W3DTileMap& tm, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        int l = this->cl+this->position.x-extend_l;
        int r = this->cr+this->position.x+extend_r;
        int d = this->cd+this->position.y-extend_d;
        int u = this->cu+this->position.y+extend_u;
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        E512Array<E512Point> ret;
        for (int y = td; y <= tu; ++y) {
            for (int x = tl; x <= tr; ++x) {
                if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                ret.emplace_back(x, y);
            }
        }
        return ret;
    }
    E512Array<E512Point> adjacentTilePositionList (E512W3DTileMap& tm, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
        int l = this->cl+this->position.x-extend_l;
        int r = this->cr+this->position.x+extend_r;
        int d = this->cd+this->position.y-extend_d;
        int u = this->cu+this->position.y+extend_u;
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        E512Array<E512Point> ret;
        for (int y = td; y <= tu; ++y) {
            for (int x = tl; x <= tr; ++x) {
                ret.emplace_back(x, y);
            }
        }
        return ret;
    }
    
    
};

void draw2dSprite (E512W3DWindow& w, Object2D& o, int z = -1) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    int tx = o.tex_x;
    int ty = o.tex_y;
    int th = o.tex_h;
    int tw = o.tex_w;
    
    if (z < 0) { z = (cy-o.position.y) + 1; }
    if (cx+o.position.x > w.width || cx+o.position.x+tw < 0) { return; }
    if (cy-o.position.y-th > w.height || cy-o.position.y-th+th < 0) { return; }
    w.drawTextureTXYWHZ(cx+(int)o.position.x, cy-(int)o.position.y-th, tx, ty, tw, th, z, *o.texture);
}

void draw2d (E512W3DWindow& w, Object2D& o, int z = -1) { draw2dSprite(w, o, z); }

void draw2dTileMap (E512W3DWindow& w, E512W3DTileMap& m) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    const int th = m.tex_h;
    const int tw = m.tex_w;
    const int hth = m.tex_h / 2;
    const int htw = m.tex_w / 2;
    
    int l = -cx;
    int r = l + w.width;
    int u = cy;
    int d = u - w.height;
    
    int tl = l/tw;
    int tr = r/tw;
    int tu = u/th;
    int td = d/th;
    
    m.update();
    
    int ti9[9];
    // 678
    // 345
    // 012
    
    for (int y = tu; y >= td - m.max_tex_h; --y) {
        for (int x = tl; x <= tr; ++x) {
            if (!m.isInside(x, y)) { continue; }
            int ti = m.getTileIndex(x, y);
            const E512W3DTile& t = m.getTile(x, y);
            if (ti == 0) { continue; }
            
            if ((t.a.use && t.a.autotile) || (t.b.use && t.b.autotile)) {
                for (int dy = 0; dy < 3; ++dy) {
                    for (int dx = 0; dx < 3; ++dx) {
                        ti9[dy*3+dx] = m.getTileIndexS(x-1+dx, y-1+dy);
                    }
                }
            }
            
            if (t.a.use && !t.a.autotile) {
                int tx = t.a.tex_x * tw;
                int ty = t.a.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.a.anim > 0) { tx += tw * t.a.anim_position; }
                w.drawTextureTXYWHZ(cx+px, cy-py-th, tx, ty, tw, th, m.z, *m.texture);
            }
            if (t.a.use && t.a.autotile) {
                int tx = t.a.tex_x * tw;
                int ty = t.a.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.a.anim > 0) { tx += tw * t.a.anim_position; }
                
                int t1 = 0;
                int t2 = 0;
                int t3 = 0;
                int t4 = 0;
                if (ti9[7] == ti || m.tile[ti9[7]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t1 = 1;
                    t2 = 1;
                }
                if (ti9[1] == ti || m.tile[ti9[1]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t3 = 1;
                    t4 = 1;
                }
                if (ti9[3] == ti || m.tile[ti9[3]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t1 = t1 == 1 ? 3 : 2;
                    t3 = t3 == 1 ? 3 : 2;
                }
                if (ti9[5] == ti || m.tile[ti9[5]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t2 = t2 == 1 ? 3 : 2;
                    t4 = t4 == 1 ? 3 : 2;
                }
                if (t1 == 3 && (ti9[6] == ti || m.tile[ti9[6]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t1 = 4; }
                if (t2 == 3 && (ti9[8] == ti || m.tile[ti9[8]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t2 = 4; }
                if (t3 == 3 && (ti9[0] == ti || m.tile[ti9[0]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t3 = 4; }
                if (t4 == 3 && (ti9[2] == ti || m.tile[ti9[2]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t4 = 4; }
                
                int tx1 = tx;
                int ty1 = ty + t1 * th;
                w.drawTextureTXYWHZ(cx+px, cy-py-th, tx1, ty1, htw, hth, m.z, *m.texture);
                int tx2 = tx + htw;
                int ty2 = ty + t2 * th;
                w.drawTextureTXYWHZ(cx+px+htw, cy-py-th, tx2, ty2, htw, hth, m.z, *m.texture);
                int tx3 = tx;
                int ty3 = ty + hth + t3 * th;
                w.drawTextureTXYWHZ(cx+px, cy-py-th+hth, tx3, ty3, htw, hth, m.z, *m.texture);
                int tx4 = tx + htw;
                int ty4 = ty + hth + t4 * th;
                w.drawTextureTXYWHZ(cx+px+htw, cy-py-th+hth, tx4, ty4, htw, hth, m.z, *m.texture);
            }
            if (t.b.use && !t.b.autotile) {
                int tx = t.b.tex_x * tw;
                int ty = t.b.tex_y * th - th * (t.b.tex_h-1);
                int bth = t.b.tex_h * th;
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.b.start_h+1;
                if (t.b.anim > 0) { tx += tw * t.b.anim_position; }
                w.drawTextureTXYWHZ(cx+px, cy-py-bth, tx, ty, tw, bth, z, *m.texture);
            }
            if (t.b.use && t.b.autotile && t.b.tex_h == 1) {
                int tx = t.b.tex_x * tw;
                int ty = t.b.tex_y * th;
                // int bth = t.b.tex_h * th;
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.b.start_h+1;
                if (t.b.anim > 0) { tx += tw * t.b.anim_position; }
                
                int t1 = 0;
                int t2 = 0;
                int t3 = 0;
                int t4 = 0;
                if (ti9[7] == ti || m.tile[ti9[7]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t1 = 1;
                    t2 = 1;
                }
                if (ti9[1] == ti || m.tile[ti9[1]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t3 = 1;
                    t4 = 1;
                }
                if (ti9[3] == ti || m.tile[ti9[3]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t1 = t1 == 1 ? 3 : 2;
                    t3 = t3 == 1 ? 3 : 2;
                }
                if (ti9[5] == ti || m.tile[ti9[5]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t2 = t2 == 1 ? 3 : 2;
                    t4 = t4 == 1 ? 3 : 2;
                }
                if (t1 == 3 && (ti9[6] == ti || m.tile[ti9[6]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t1 = 4; }
                if (t2 == 3 && (ti9[8] == ti || m.tile[ti9[8]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t2 = 4; }
                if (t3 == 3 && (ti9[0] == ti || m.tile[ti9[0]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t3 = 4; }
                if (t4 == 3 && (ti9[2] == ti || m.tile[ti9[2]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t4 = 4; }
                
                int tx1 = tx;
                int ty1 = ty + t1 * th;
                w.drawTextureTXYWHZ(cx+px, cy-py-th, tx1, ty1, htw, hth, z, *m.texture);
                int tx2 = tx + htw;
                int ty2 = ty + t2 * th;
                w.drawTextureTXYWHZ(cx+px+htw, cy-py-th, tx2, ty2, htw, hth, z, *m.texture);
                int tx3 = tx;
                int ty3 = ty + hth + t3 * th;
                w.drawTextureTXYWHZ(cx+px, cy-py-th+hth, tx3, ty3, htw, hth, z, *m.texture);
                int tx4 = tx + htw;
                int ty4 = ty + hth + t4 * th;
                w.drawTextureTXYWHZ(cx+px+htw, cy-py-th+hth, tx4, ty4, htw, hth, z, *m.texture);
            }
        }
    }
}

void draw2d (E512W3DWindow& w, E512W3DTileMap& m) { draw2dTileMap(w, m); }




void draw2dbTileMap (E512W3DWindow& w, E512W3DTileMap& m) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    const int th = m.tex_h;
    const int tw = m.tex_w;
    const int hth = m.tex_h / 2;
    const int htw = m.tex_w / 2;
    
    int l = -cx;
    int r = l + w.width;
    int u = cy;
    int d = u - w.height;
    
    int tl = l/tw;
    int tr = r/tw;
    int tu = u/th;
    int td = d/th;
    
    m.update();
    
    int ti9[9];
    // 678
    // 345
    // 012
    
    for (int y = tu; y >= td - m.max_tex_h; --y) {
        for (int x = tl; x <= tr; ++x) {
            if (!m.isInside(x, y)) { continue; }
            int ti = m.getTileIndex(x, y);
            const E512W3DTile& t = m.getTile(x, y);
            if (ti == 0) { continue; }
            
            if ((t.a.use && t.a.autotile) || (t.b.use && t.b.autotile)) {
                for (int dy = 0; dy < 3; ++dy) {
                    for (int dx = 0; dx < 3; ++dx) {
                        ti9[dy*3+dx] = m.getTileIndexS(x-1+dx, y-1+dy);
                    }
                }
            }
            
            float b = (15 - min((int)m.getTileValue(x, y), 15)) / 15.0f ;
            if (t.a.use && !t.a.autotile) {
                int tx = t.a.tex_x * tw;
                int ty = t.a.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.a.anim > 0) { tx += tw * t.a.anim_position; }
                w.drawTextureTXYWHZB(cx+px, cy-py-th, tx, ty, tw, th, m.z, b, *m.texture);
            }
            
            if (t.a.use && t.a.autotile) {
                int tx = t.a.tex_x * tw;
                int ty = t.a.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.a.anim > 0) { tx += tw * t.a.anim_position; }
                
                int t1 = 0;
                int t2 = 0;
                int t3 = 0;
                int t4 = 0;
                if (ti9[7] == ti || m.tile[ti9[7]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t1 = 1;
                    t2 = 1;
                }
                if (ti9[1] == ti || m.tile[ti9[1]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t3 = 1;
                    t4 = 1;
                }
                if (ti9[3] == ti || m.tile[ti9[3]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t1 = t1 == 1 ? 3 : 2;
                    t3 = t3 == 1 ? 3 : 2;
                }
                if (ti9[5] == ti || m.tile[ti9[5]].a.autotile_jack & m.tile[ti].a.autotile_plug) {
                    t2 = t2 == 1 ? 3 : 2;
                    t4 = t4 == 1 ? 3 : 2;
                }
                if (t1 == 3 && (ti9[6] == ti || m.tile[ti9[6]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t1 = 4; }
                if (t2 == 3 && (ti9[8] == ti || m.tile[ti9[8]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t2 = 4; }
                if (t3 == 3 && (ti9[0] == ti || m.tile[ti9[0]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t3 = 4; }
                if (t4 == 3 && (ti9[2] == ti || m.tile[ti9[2]].a.autotile_jack & m.tile[ti].a.autotile_plug)) { t4 = 4; }
                
                int tx1 = tx;
                int ty1 = ty + t1 * th;
                w.drawTextureTXYWHZB(cx+px, cy-py-th, tx1, ty1, htw, hth, m.z, b, *m.texture);
                int tx2 = tx + htw;
                int ty2 = ty + t2 * th;
                w.drawTextureTXYWHZB(cx+px+htw, cy-py-th, tx2, ty2, htw, hth, m.z, b, *m.texture);
                int tx3 = tx;
                int ty3 = ty + hth + t3 * th;
                w.drawTextureTXYWHZB(cx+px, cy-py-th+hth, tx3, ty3, htw, hth, m.z, b, *m.texture);
                int tx4 = tx + htw;
                int ty4 = ty + hth + t4 * th;
                w.drawTextureTXYWHZB(cx+px+htw, cy-py-th+hth, tx4, ty4, htw, hth, m.z, b, *m.texture);
                
            }
            
            if (t.b.use && !t.b.autotile) {
                int tx = t.b.tex_x * tw;
                int ty = t.b.tex_y * th - th * (t.b.tex_h-1);
                int bth = t.b.tex_h * th;
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.b.start_h+1;
                if (t.b.anim > 0) { tx += tw * t.b.anim_position; }
                w.drawTextureTXYWHZB(cx+px, cy-py-bth, tx, ty, tw, bth, z, b, *m.texture);
            }
            if (t.b.use && t.b.autotile && t.b.tex_h == 1) {
                int tx = t.b.tex_x * tw;
                int ty = t.b.tex_y * th;
                // int bth = t.b.tex_h * th;
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.b.start_h+1;
                if (t.b.anim > 0) { tx += tw * t.b.anim_position; }
                
                int t1 = 0;
                int t2 = 0;
                int t3 = 0;
                int t4 = 0;
                if (ti9[7] == ti || m.tile[ti9[7]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t1 = 1;
                    t2 = 1;
                }
                if (ti9[1] == ti || m.tile[ti9[1]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t3 = 1;
                    t4 = 1;
                }
                if (ti9[3] == ti || m.tile[ti9[3]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t1 = t1 == 1 ? 3 : 2;
                    t3 = t3 == 1 ? 3 : 2;
                }
                if (ti9[5] == ti || m.tile[ti9[5]].b.autotile_jack & m.tile[ti].b.autotile_plug) {
                    t2 = t2 == 1 ? 3 : 2;
                    t4 = t4 == 1 ? 3 : 2;
                }
                if (t1 == 3 && (ti9[6] == ti || m.tile[ti9[6]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t1 = 4; }
                if (t2 == 3 && (ti9[8] == ti || m.tile[ti9[8]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t2 = 4; }
                if (t3 == 3 && (ti9[0] == ti || m.tile[ti9[0]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t3 = 4; }
                if (t4 == 3 && (ti9[2] == ti || m.tile[ti9[2]].b.autotile_jack & m.tile[ti].b.autotile_plug)) { t4 = 4; }
                
                int tx1 = tx;
                int ty1 = ty + t1 * th;
                w.drawTextureTXYWHZB(cx+px, cy-py-th, tx1, ty1, htw, hth, z, b, *m.texture);
                int tx2 = tx + htw;
                int ty2 = ty + t2 * th;
                w.drawTextureTXYWHZB(cx+px+htw, cy-py-th, tx2, ty2, htw, hth, z, b, *m.texture);
                int tx3 = tx;
                int ty3 = ty + hth + t3 * th;
                w.drawTextureTXYWHZB(cx+px, cy-py-th+hth, tx3, ty3, htw, hth, z, b, *m.texture);
                int tx4 = tx + htw;
                int ty4 = ty + hth + t4 * th;
                w.drawTextureTXYWHZB(cx+px+htw, cy-py-th+hth, tx4, ty4, htw, hth, z, b, *m.texture);
                
            }
        }
    }
}

void draw2db (E512W3DWindow& w, E512W3DTileMap& m) { draw2dbTileMap(w, m); }

void draw2dSpriteScreenPosition (E512W3DWindow& w, Object2D& o, int x, int y, int z = -1) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    int tx = o.tex_x;
    int ty = o.tex_y;
    int th = o.tex_h;
    int tw = o.tex_w;
    
    if (z < 0) { z = (cy-o.position.y) + 1; }
    if (x > w.width || x+tw < 0) { return; }
    if (y-th > w.height || y-th+th < 0) { return; }
    w.drawTextureTXYWHZ(x, y, tx, ty, tw, th, z, *o.texture);
}


Vector3 screenPosition (E512W3DWindow& w, E512W3DTileMap& m, int x, int y) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    const int th = m.tex_h;
    const int tw = m.tex_w;
    int px = x * tw;
    int py = y * th;
    return Vector3(cx+px, cy-py-th-1, cy-py-1);
}

Vector3 screenPosition (E512W3DWindow& w, Object2D& o) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    return Vector3(cx+(int)o.position.x, cy-(int)o.position.y-o.tex_h-1, cy-o.position.y-1);
}

Vector2 screenPosition (E512W3DWindow& w, int x, int y) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    // if (cx+x > w.width  || cx+x < 0) { return Vector2(-1.0f, -1.0f); }
    // if (cy-y > w.height || cy-y < 0) { return Vector2(-1.0f, -1.0f); }
    return Vector2(cx+x, cy-y-1);
}

E512Point screenPositionTo2DWorldPosition (E512W3DWindow& w, int x, int y) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    int tx = x-cx;
    int ty = cy-y-1;
    return E512Point(tx, ty);
}
E512Point tilemapPosition (E512W3DWindow& w, E512W3DTileMap& m, int x, int y) {
    E512Point p = screenPositionTo2DWorldPosition(w, x, y);
    int tx = p.x / m.tex_w;
    int ty = p.y / m.tex_h;
    if (p.x < 0) { tx -= 1; }
    if (p.y < 0) { ty -= 1; }
    return E512Point(tx, ty);
}
E512Point tilemapPosition (E512W3DWindow& w, E512W3DTileMap& m) {
    Vector2 c = E512W3DInput::cursorPosition();
    return tilemapPosition(w, m, c.x-w.sx, c.y-w.sy);
}


