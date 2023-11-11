#pragma once

#include "E512W3D.hpp"


bool aabb (int asx, int asy, int aex, int aey, int bsx, int bsy, int bex, int bey) {
    return min(aex, bex) > max(asx, bsx) && min(aey, bey) > max(asy, bsy);
}

struct E512Point {
    int x, y;
    E512Point () { this->x = 0; this->y = 0; };
    E512Point (int x, int y) { this->x = x; this->y = y; }
    
    E512Point operator + (const E512Point& t) const { return E512Point(this->x + t.x, this->y + t.y); }
    E512Point operator - (const E512Point& t) const { return E512Point(this->x - t.x, this->y - t.y); }
    bool operator == (const E512Point& t) const { return this->x == t.x && this->y == t.y; }
    bool operator != (const E512Point& t) const { return this->x != t.x || this->y != t.y; }
};

namespace TileType {
    enum Type {
        Normal,
        Object,
    };
}

struct E512W3DTile {
    int16_t tex_x = 0;
    int16_t tex_y = 0;
    uint16_t collision_layer = 0;
    
    uint8_t anim = 0;
    uint8_t anim_position = 0;
    uint16_t anim_cnt = 0;
    uint16_t anim_wait = 8;
    
    uint16_t child = 0;
    
    int type = 0;
    
    // Object
    int16_t btex_x = 0;
    int16_t btex_y = 0;
    uint8_t banim = 0;
    uint8_t banim_position = 0;
    uint16_t banim_cnt = 0;
    uint16_t banim_wait = 8;
    int16_t btex_h = 1;
    int16_t bstart_h = 0;
    
    
    E512W3DTile () {}
    E512W3DTile (int16_t tex_x, int16_t tex_y, uint8_t anim, uint16_t collision_layer) {
        this->tex_x = tex_x;
        this->tex_y = tex_y;
        this->collision_layer = collision_layer;
        this->anim = anim;
        this->type = TileType::Normal;
    }
    
    E512W3DTile (int16_t tex_x, int16_t tex_y, uint8_t anim, int16_t btex_x, int16_t btex_y, uint8_t banim, int16_t btex_h, int16_t bstart_h, uint16_t collision_layer) {
        this->tex_x = tex_x;
        this->tex_y = tex_y;
        this->anim = anim;
        this->btex_x = btex_x;
        this->btex_y = btex_y;
        this->banim = banim;
        this->btex_h = btex_h;
        this->bstart_h = bstart_h;
        this->collision_layer = collision_layer;
        this->type = TileType::Object;
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
    
    int max_btex_h = 0;
    
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
        this->max_btex_h = 0;
        for (auto&& i : this->tile) {
            this->max_btex_h = max(i.btex_h-1, this->max_btex_h);
            if (i.anim > 0) {
                i.anim_wait = max((int)i.anim_wait, 1);
                if (i.anim_cnt % i.anim_wait == 0) {
                    i.anim_position = (i.anim_position + 1) % i.anim;
                }
                
                i.anim_cnt += 1;
                if (i.anim_cnt >= i.anim * i.anim_wait) { i.anim_cnt = 0; }
            }
            if (i.banim > 0) {
                i.banim_wait = max((int)i.banim_wait, 1);
                if (i.banim_cnt % i.banim_wait == 0) {
                    i.banim_position = (i.banim_position + 1) % i.banim;
                }
                
                i.banim_cnt += 1;
                if (i.banim_cnt >= i.banim * i.banim_wait) { i.banim_cnt = 0; }
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
    
    bool isGround (E512W3DTileMap& tm) { return this->isCollision(tm, 0, 0, 0, 1); }
    
    bool moveX (E512W3DTileMap& tm) {
        float v = this->velocity.x;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.x > 0) {
            tr = min((int)((r+v)/tm.tex_h), tm.width-1);
            int p = r+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, x*tm.tex_w);
                }
            }
            if (ret) { v -= (px+this->cr-1+v) - p; }
            v = max(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x -= 0.5f; }
        }
        if (this->velocity.x < 0) {
            tl = max((int)((l+v)/tm.tex_w), 0);
            int p = l+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, x*tm.tex_w+tm.tex_w);
                }
            }
            if (ret) { v += p - (px+v+this->cl); }
            v = min(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x += 0.5f; }
        }
        return ret;
    }
    bool moveY (E512W3DTileMap& tm) {
        float v = this->velocity.y;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.y > 0) {
            tu = min((int)((u+v)/tm.tex_h), tm.height-1);
            int p = u+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, y*tm.tex_h);
                }
            }
            if (ret) { v -= (py+this->cu-1+v) - p; }
            v = max(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y -= 0.5f; }
        }
        if (this->velocity.y < 0) {
            td = max((int)((d+v)/tm.tex_h), 0);
            int p = d+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, y*tm.tex_h+tm.tex_h);
                }
            }
            if (ret) { v += p - (py+v+this->cd); }
            v = min(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y += 0.5f; }
        }
        return ret;
    }
    
    bool isGround (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) { return this->isCollision(tm, o, 0, 0, 0, 1); }
    
    bool moveX (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) {
        float v = this->velocity.x;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.x > 0) {
            tr = min((int)((r+v)/tm.tex_h), tm.width-1);
            int p = r+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, x*tm.tex_w);
                }
            }
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(l, d, r+v, u, gx, gy, gx+i.cr, gy+i.cu)) {
                    ret = true;
                    p = min(p, (int)gx);
                }
            }
            if (ret) { v -= (px+this->cr-1+v) - p; }
            v = max(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x -= 0.5f; }
        }
        if (this->velocity.x < 0) {
            tl = max((int)((l+v)/tm.tex_w), 0);
            int p = l+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, x*tm.tex_w+tm.tex_w);
                }
            }
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(l+v, d, r, u, gx, gy, gx+i.cr, gy+i.cu)) {
                    ret = true;
                    p = max(p, (int)gx+i.cr);
                }
            }
            if (ret) { v += p - (px+v+this->cl); }
            v = min(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x += 0.5f; }
        }
        return ret;
    }
    bool moveY (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D>& o) {
        float v = this->velocity.y;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.y > 0) {
            tu = min((int)((u+v)/tm.tex_h), tm.height-1);
            int p = u+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, y*tm.tex_h);
                }
            }
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(l, d, r, u+v, gx, gy, gx+i.cr, gy+i.cu)) {
                    ret = true;
                    p = min(p, (int)gy);
                }
            }
            if (ret) { v -= (py+this->cu-1+v) - p; }
            v = max(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y -= 0.5f; }
        }
        if (this->velocity.y < 0) {
            td = max((int)((d+v)/tm.tex_h), 0);
            int p = d+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, y*tm.tex_h+tm.tex_h);
                }
            }
            for (auto&& i : o) {
                if ((i.collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i.position.x;
                float gy = i.position.y;
                if (aabb(l, d+v, r, u, gx, gy, gx+i.cr, gy+i.cu)) {
                    ret = true;
                    p = max(p, (int)gy+i.cu);
                }
            }
            if (ret) { v += p - (py+v+this->cd); }
            v = min(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y += 0.5f; }
        }
        return ret;
    }
    
    bool isGround (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) { return this->isCollision(tm, o, 0, 0, 0, 1); }
    
    bool moveX (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) {
        float v = this->velocity.x;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.x > 0) {
            tr = min((int)((r+v)/tm.tex_h), tm.width-1);
            int p = r+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, x*tm.tex_w);
                }
            }
            for (auto&& i : o) {
                if ((i->collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i->position.x;
                float gy = i->position.y;
                if (aabb(l, d, r+v, u, gx, gy, gx+i->cr, gy+i->cu)) {
                    ret = true;
                    p = min(p, (int)gx);
                }
            }
            if (ret) { v -= (px+this->cr-1+v) - p; }
            v = max(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x -= 0.5f; }
        }
        if (this->velocity.x < 0) {
            tl = max((int)((l+v)/tm.tex_w), 0);
            int p = l+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, x*tm.tex_w+tm.tex_w);
                }
            }
            for (auto&& i : o) {
                if ((i->collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i->position.x;
                float gy = i->position.y;
                if (aabb(l+v, d, r, u, gx, gy, gx+i->cr, gy+i->cu)) {
                    ret = true;
                    p = max(p, (int)gx+i->cr);
                }
            }
            if (ret) { v += p - (px+v+this->cl); }
            v = min(v, 0.0f);
            this->position.x += v;
            if (ret) { this->position.x += 0.5f; }
        }
        return ret;
    }
    bool moveY (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o) {
        float v = this->velocity.y;
        if (v == 0) { return false; }
        float px = this->position.x;
        float py = this->position.y;
        
        float l = this->cl+this->position.x;
        float r = this->cr+this->position.x;
        float d = this->cd+this->position.y;
        float u = this->cu+this->position.y;
        
        int tl = max((int)(l/tm.tex_w), 0);
        int tr = min((int)((r-1)/tm.tex_w), tm.width-1);
        int td = max((int)(d/tm.tex_h), 0);
        int tu = min((int)((u-1)/tm.tex_h), tm.height-1);
        
        bool ret = false;
        if (this->velocity.y > 0) {
            tu = min((int)((u+v)/tm.tex_h), tm.height-1);
            int p = u+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = min(p, y*tm.tex_h);
                }
            }
            for (auto&& i : o) {
                if ((i->collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i->position.x;
                float gy = i->position.y;
                if (aabb(l, d, r, u+v, gx, gy, gx+i->cr, gy+i->cu)) {
                    ret = true;
                    p = min(p, (int)gy);
                }
            }
            if (ret) { v -= (py+this->cu-1+v) - p; }
            v = max(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y -= 0.5f; }
        }
        if (this->velocity.y < 0) {
            td = max((int)((d+v)/tm.tex_h), 0);
            int p = d+v;
            for (int y = td; y <= tu; ++y) {
                for (int x = tl; x <= tr; ++x) {
                    if ((tm.getTile(x, y).collision_layer & this->collision_layer) == 0) { continue; }
                    ret = true;
                    p = max(p, y*tm.tex_h+tm.tex_h);
                }
            }
            for (auto&& i : o) {
                if ((i->collision_layer & this->collision_layer) == 0) { continue; }
                float gx = i->position.x;
                float gy = i->position.y;
                if (aabb(l, d+v, r, u, gx, gy, gx+i->cr, gy+i->cu)) {
                    ret = true;
                    p = max(p, (int)gy+i->cu);
                }
            }
            if (ret) { v += p - (py+v+this->cd); }
            v = min(v, 0.0f);
            this->position.y += v;
            if (ret) { this->position.y += 0.5f; }
        }
        return ret;
    }
    
    
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
        for (auto&& i : o) {
            if (this->isCollision(i, extend_l, extend_u, extend_r, extend_d)) { return true; }
        }
        return false;
    }
    
    bool isCollision (E512W3DTileMap& tm, E512Array<E512W3DGameObject2D*>& o, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
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
        for (auto&& i : o) {
            if (this->isCollision(*i, extend_l, extend_u, extend_r, extend_d)) { return true; }
        }
        return false;
    }
    
    E512Array<E512Point> isCollisionList (E512W3DTileMap& tm, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
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
    E512Array<E512Point> adjacentTileList (E512W3DTileMap& tm, int extend_l=0, int extend_u=0, int extend_r=0, int extend_d=0) {
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
    
    int l = -cx;
    int r = l + w.width;
    int u = cy;
    int d = u - w.height;
    
    int tl = l/tw;
    int tr = r/tw;
    int tu = u/th;
    int td = d/th;
    
    m.update();
    
    for (int y = tu; y >= td - m.max_btex_h; --y) {
        for (int x = tl; x <= tr; ++x) {
            if (!m.isInside(x, y)) { continue; }
            int ti = m.getTileIndex(x, y);
            const E512W3DTile& t = m.getTile(x, y);
            if (ti == 0) { continue; }
            
            if (t.type == TileType::Normal || t.type == TileType::Object) {
                int tx = t.tex_x * tw;
                int ty = t.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.anim > 0) { tx += tw * t.anim_position; }
                w.drawTextureTXYWHZ(cx+px, cy-py-th, tx, ty, tw, th, m.z, *m.texture);
            }
            if (t.type == TileType::Object) {
                int btx = t.btex_x * tw;
                int bty = t.btex_y * th - th * (t.btex_h-1);
                int bth = t.btex_h*th;
                int btw = tw;
                if (t.banim > 0) { btx += tw * t.banim_position; }
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.bstart_h+1;
                w.drawTextureTXYWHZ(cx+px, cy-py-bth, btx, bty, btw, bth, z, *m.texture);
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
    
    int l = -cx;
    int r = l + w.width;
    int u = cy;
    int d = u - w.height;
    
    int tl = l/tw;
    int tr = r/tw;
    int tu = u/th;
    int td = d/th;
    
    m.update();
    
    for (int y = tu; y >= td - m.max_btex_h; --y) {
        for (int x = tl; x <= tr; ++x) {
            if (!m.isInside(x, y)) { continue; }
            int ti = m.getTileIndex(x, y);
            const E512W3DTile& t = m.getTile(x, y);
            if (ti == 0) { continue; }
            float b = (15 - min((int)m.getTileValue(x, y), 15)) / 15.0f ;
            if (t.type == TileType::Normal || t.type == TileType::Object) {
                int tx = t.tex_x * tw;
                int ty = t.tex_y * th;
                int px = x * tw;
                int py = y * th;
                if (t.anim > 0) { tx += tw * t.anim_position; }
                w.drawTextureTXYWHZB(cx+px, cy-py-th, tx, ty, tw, th, m.z, b, *m.texture);
            }
            if (t.type == TileType::Object) {
                int btx = t.btex_x * tw;
                int bty = t.btex_y * th - th * (t.btex_h-1);
                int bth = t.btex_h*th;
                int btw = tw;
                if (t.banim > 0) { btx += tw * t.banim_position; }
                int px = x * tw;
                int py = y * th;
                int z = m.z+(cy-py)-t.bstart_h+1;
                
                w.drawTextureTXYWHZB(cx+px, cy-py-bth, btx, bty, btw, bth, z, b, *m.texture);
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
    return Vector3(cx+px, cy-py-th, cy-py);
}

Vector3 screenPosition (E512W3DWindow& w, Object2D& o) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    return Vector3(cx+(int)o.position.x, cy-(int)o.position.y-o.tex_h, cy-o.position.y);
}

Vector2 screenPosition (E512W3DWindow& w, int x, int y) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    if (cx+x > w.width  || cx+x < 0) { Vector2(-1.0f, -1.0f); }
    if (cy-y > w.height || cy-y < 0) { Vector2(-1.0f, -1.0f); }
    return Vector2(cx+x, cy-y);
}

E512Point screenPositionTo2DWorldPosition (E512W3DWindow& w, int x, int y) {
    const int cx = w.width/2 - w.camera->position.x;
    const int cy = w.height/2 + w.camera->position.y;
    int tx = (x-cx);
    int ty = (cy-y);
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


