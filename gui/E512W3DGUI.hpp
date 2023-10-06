#pragma once

#include "E512W3D.hpp"


class E512W3DWindowTextCursor {
public:
    int16_t text_cursor_x;
    int16_t text_cursor_y;
    int16_t text_sub_cursor_x;
    int16_t text_sub_cursor_y;
    uint16_t text_bgcolor;
    uint16_t text_color;
    int16_t text_size;
    bool text_use_bgcolor;
    E512W3DWindowTextCursor (E512W3DWindow& w) {
        this->text_cursor_x     = w.text_cursor_x;
        this->text_cursor_y     = w.text_cursor_y;
        this->text_sub_cursor_x = w.text_sub_cursor_x;
        this->text_sub_cursor_y = w.text_sub_cursor_y;
        this->text_bgcolor      = w.text_bgcolor;
        this->text_color        = w.text_color;
        this->text_size         = w.text_size;
        this->text_use_bgcolor  = w.text_use_bgcolor;
    }
    
    void load (E512W3DWindow& w) {
        w.text_cursor_x     = this->text_cursor_x;
        w.text_cursor_y     = this->text_cursor_y;
        w.text_sub_cursor_x = this->text_sub_cursor_x;
        w.text_sub_cursor_y = this->text_sub_cursor_y;
        w.text_bgcolor      = this->text_bgcolor;
        w.text_color        = this->text_color;
        w.text_size         = this->text_size;
        w.text_use_bgcolor  = this->text_use_bgcolor;
    }
};

class E512W3DGUISlider {
public:
    int16_t x, y;
    uint16_t hv = 0;// h v
    uint16_t type = 0;// slider, seek
    uint16_t color;
    uint16_t bgcolor;
    int m = 3;
    int n = 2;
    
    bool ishover = false;
    bool isdrag = false;
    
    E512W3DGUISlider () {}
    E512W3DGUISlider (int16_t x, int16_t y, uint16_t size, uint16_t hv = 0, uint16_t type = 0, int maxvalue = 1, int value = 0, uint16_t color = 42260, uint16_t bgcolor = 21130, uint16_t knobsize = 3, uint16_t linewidth = 2) {
        this->m = knobsize;
        this->n = linewidth;
        this->x = x;
        this->y = y;
        this->size = max((int)size, this->m*2+this->n);
        this->color = color;
        this->bgcolor = bgcolor;
        this->hv = hv;
        this->type = type;
        this->maxvalue = max(maxvalue, 1);
        this->setValue(value);
    }
    
    void setValue (int value) {
        this->value = min(max(value, 0), this->maxvalue);
    }
    int getValue () { return this->value; }
    void setValue01 (float value) {
        this->value = this->maxvalue * min(max(value, 0.0f), 1.0f);
    }
    float getValue01 () { return (float)this->value / (float)this->maxvalue; }
    
    void setMaxValue (int value) {
        this->maxvalue = max(value, 1);
    }
    int getMaxValue () { return this->maxvalue; }
    
    void setSize (int size) {
        this->size = max(size, this->m*2+this->n);
    }
    int getSize () { return this->size; }
    
    
    void guiUpdate (E512W3DWindow& w) {
        int px = w.getCursorX();
        int py = w.getCursorY();
        
        int sx, sy, ex, ey;
        
        if (this->type == 0) {
            if (this->hv == 0) {
                sx = this->x;
                sy = this->y;
                ex = this->x+this->size;
                ey = this->y+this->n+this->m*2;
            } else {
                sx = this->x;
                sy = this->y;
                ex = this->x+this->n+this->m*2;
                ey = this->y+size;
            }
        } else {
            if (this->hv == 0) {
                sx = this->x;
                sy = this->y;
                ex = this->x+this->size;
                ey = this->y+this->n+this->m*2;
            } else {
                sx = this->x;
                sy = this->y;
                ex = this->x+this->n+this->m*2;
                ey = this->y+size;
            }
        }
        
        this->ishover = this->inside(w, sx, sy, ex, ey);
        
        if (this->ishover && E512W3DInput::getButtonDown(0)) {
            this->isdrag = true;
        } else if (this->isdrag && E512W3DInput::getButton(0)) {
            this->isdrag = true;
        } else {
            this->isdrag = false;
        }
        
        if (this->isdrag == false) { return; }
        
        float md = 1.0f / (float)(this->maxvalue + 1);
        
        if (this->type == 0) {
            int s = this->size - this->m*2 - this->n;
            if (this->hv == 0) {
                int l = this->x+this->m;
                int r = this->x+this->n+s+this->m;
                float t = min(max(px, l), r) - l;
                // this->setValue01(t / (r-l));
                this->setValue01(min(t / (r-l)+md, 1.0f));
            } else {
                int u = this->y+this->m;
                int d = this->y+this->n+s+this->m;
                float t = min(max(py, u), d) - u;
                // this->setValue01(t / (d-u));
                this->setValue01(min(t / (d-u)+md, 1.0f));
            }
        } else {
            if (this->hv == 0) {
                int l = this->x;
                int r = this->x+this->size;
                float t = min(max(px, l), r);
                // this->setValue01(t / (r-l));
                this->setValue01(min(t / (r-l)+md, 1.0f));
            } else {
                int u = this->y;
                int d = this->y+this->size;
                float t = min(max(py, u), d);
                // this->setValue01(t / (d-u));
                this->setValue01(min(t / (d-u)+md, 1.0f));
            }
        }
    }
    void guiDraw (E512W3DWindow& w) {
        int s = this->size - this->m*2 - this->n;
        
        if (this->type == 0) {
            int t = (this->size - this->m*2 - this->n) * this->getValue01();
            if (this->hv == 0) {
                w.drawRect(this->x+this->m, this->y+this->m, this->x+this->n+s+this->m, this->y+this->n+this->m, this->bgcolor);
                w.drawRect(this->x+t, this->y, this->x+this->n+this->m*2+t, this->y+this->n+this->m*2, this->color);
            } else {
                w.drawRect(this->x+this->m, this->y+this->m, this->x+this->n+this->m, this->y+this->n+s+this->m, this->bgcolor);
                w.drawRect(this->x, this->y+t, this->x+this->n+this->m*2, this->y+this->n+this->m*2+t, this->color);
            }
        } else {
            int t = this->size * this->getValue01();
            if (this->hv == 0) {
                w.drawRect(this->x, this->y, this->x+this->size, this->y+this->n+this->m*2, this->bgcolor);
                w.drawRect(this->x, this->y, this->x+t, this->y+this->n+this->m*2, this->color);
            } else {
                w.drawRect(this->x, this->y, this->x+this->n+this->m*2, this->y+this->size, this->bgcolor);
                w.drawRect(this->x, this->y, this->x+this->n+this->m*2, this->y+t, this->color);
            }
        }
    }
    void popupDraw (E512W3DWindow& w) {
        if (!(this->ishover || this->isdrag)) { return; }
        if (this->ishover && !this->isdrag && E512W3DInput::getButton(0)) { return; }
        int px = w.getCursorX();
        int py = w.getCursorY();
        E512W3DWindowTextCursor c(w);
        w.text_size = 1;
        w.text_bgcolor = 0xFFFF;
        w.text_color = 0x0000;
        w.text_use_bgcolor = true;
        E512Array<uint8_t> str = numtostr(this->value);
        px = min(max(px, 0), w.width);
        py = min(max(py, 0), w.height);
        
        int sx, sy, ex, ey;
        if (this->hv == 0) {
            sx = this->x;
            sy = this->y;
            ex = this->x+this->size;
            ey = this->y+this->n+this->m*2;
        } else {
            sx = this->x;
            sy = this->y;
            ex = this->x+this->n+this->m*2;
            ey = this->y+size;
        }
        
        px = min(max(px, sx), ex);
        py = min(max(py, sy), ey);
        int fw = w.font->getWidth('a');
        if (this->hv == 0) {
            w.setTextCursor(px + (px < w.width / 2 ? 12 : -fw*str.size()-12), y + (y < w.height / 2 ? 12 : -12));
        } else {
            w.setTextCursor(x + (x < w.width / 2 ? 12 : -fw*str.size()-12), py + (py < w.height / 2 ? 12 : -12));
        }
        w.print(this->value, false);
        c.load(w);
    }
private:
    uint16_t size;
    int value;
    int maxvalue;
    bool inside (E512W3DWindow& w, int16_t sx, int16_t sy, int16_t ex, int16_t ey) {
        int px = w.getCursorX();
        int py = w.getCursorY();
        return px >= sx && px < ex && py >= sy && py < ey;
    }
};

namespace ButtonHoverAnim {
    enum Type {
        None,
        Light,
        Dark,
    };
}
namespace ButtonDownAnim {
    enum Type {
        None,
        Right,
        Left,
        Down,
        Up,
    };
}

class E512W3DGUIButton {
public:
    int16_t x, y;
    uint16_t width, height;
    uint16_t color;
    uint16_t text_color;
    E512Array<uint8_t> text;
    uint16_t text_size = 1;
    bool wordwrap = true;
    int m = 0;
    
    int16_t hoveranimframe = 0;
    
    bool ishover = false;
    bool isdown = false;
    bool isup = false;
    bool isdrag = false;
    
    uint16_t hoveranim = ButtonHoverAnim::Light;
    uint16_t downanim = ButtonDownAnim::Right;
    
    E512W3DGUIButton () {}
    E512W3DGUIButton (int16_t x, int16_t y, uint16_t width, uint16_t height, E512Array<uint8_t> text, uint16_t color = 42260, uint16_t text_color = 0xFFFF) {
        this->x = x;
        this->y = y;
        this->width = max((int)width, 2);
        this->height = max((int)height, 2);
        this->text = text;
        this->color = color;
        this->text_color = text_color;
    }
    E512W3DGUIButton (int16_t x, int16_t y, uint16_t width, uint16_t height, const char* text, uint16_t color = 42260, uint16_t text_color = 0xFFFF) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->text = cptou8a(text);
        this->color = color;
        this->text_color = text_color;
    }
    
    void guiUpdate (E512W3DWindow& w) {
        this->isdown = false;
        this->isup = false;
        this->ishover = false;
        if (this->inside(w, this->x, this->y, this->x+this->width, this->y+this->height)) {
            this->ishover = true;
            if (E512W3DInput::getButtonDown(0)) {
                this->isdown = true;
                this->isdrag = true;
            }
            if (E512W3DInput::getButtonUp(0) && this->isdrag) { this->isup = true; }
        }
        this->isdrag = E512W3DInput::getButton(0) && this->isdrag;
        
        if (this->ishover) {
            this->hoveranimframe = 8;
        } else {
            this->hoveranimframe = max(this->hoveranimframe-1, 0);
        }
    }
    
    void guiDraw (E512W3DWindow& w) {
        uint16_t color = this->color;
        if (this->hoveranim != ButtonHoverAnim::None) {
            float f = this->hoveranim == ButtonHoverAnim::Light ? 1.0f : -1.0f;
            float r = ((this->color  >> 11 & 0b11111) << 3) * (1.0f + this->hoveranimframe * 0.0125f * f);
            float g = ((this->color  >>  6 & 0b11111) << 3) * (1.0f + this->hoveranimframe * 0.0125f * f);
            float b = ((this->color  >>  0 & 0b11111) << 3) * (1.0f + this->hoveranimframe * 0.0125f * f);
            r = max(min(r, 255.0f), 0.0f);
            g = max(min(g, 255.0f), 0.0f);
            b = max(min(b, 255.0f), 0.0f);
            color = color565(r, g, b);
        }
        
        int16_t l = this->x;
        int16_t u = this->y;
        if (this->downanim != ButtonDownAnim::None && (this->isdown || this->isdrag)) {
            if (this->downanim == ButtonDownAnim::Right) { l += 1; }
            if (this->downanim == ButtonDownAnim::Left) { l -= 1; }
            if (this->downanim == ButtonDownAnim::Down) { u += 1; }
            if (this->downanim == ButtonDownAnim::Up) { u -= 1; }
        }
        w.drawRect(l, u, l+this->width, u+this->height, color);
        
        E512W3DWindowTextCursor c(w);
        w.text_size = this->text_size;
        w.text_color = this->text_color;
        
        int16_t tl = l + this->m;
        int16_t tu = u + this->m;
        int16_t tr = l + this->width - this->m;
        int16_t td = u + this->height - this->m;
        int16_t cx = tl;
        int16_t cy = tu;
        for (auto&& i : this->text) {
            uint16_t tw = w.font->getWidth(i) * w.text_size;
            uint16_t th = w.font->getHeight(i) * w.text_size;
            if (i == '\n') {
                cx = tl;
                cy += th;
                continue;
            }
            if (wordwrap && cx + tw > tr) {
                cx = tl;
                cy += th;
            }
            if (cx + tw <= tr && cy + th <= td) {
                w.drawChar(i, cx, cy);
                cx += tw;
            }
        }
        
        c.load(w);
    }
    
    
private:
    bool inside (E512W3DWindow& w, int16_t sx, int16_t sy, int16_t ex, int16_t ey) {
        int px = w.getCursorX();
        int py = w.getCursorY();
        return px >= sx && px < ex && py >= sy && py < ey;
    }
};

