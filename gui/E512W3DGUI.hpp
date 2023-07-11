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
    
    bool ismouseover = false;
    bool isdrag = false;
    
    E512W3DGUISlider (int16_t x, int16_t y, uint16_t size, uint16_t hv = 0, uint16_t type = 0, int maxvalue = 1, int value = 0, uint16_t color = 42260, uint16_t bgcolor = 21130) {
        this->x = x;
        this->y = y;
        this->size = max(size, this->m*2+this->n);
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
    
    
    void updateDrag (E512W3DWindow& w) {
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
        
        this->ismouseover = this->inside(w, sx, sy, ex, ey);
        
        if (this->ismouseover && E512W3DInput::getButtonDown(0)) {
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
        if (!(this->ismouseover || this->isdrag)) { return; }
        if (this->ismouseover && !this->isdrag && E512W3DInput::getButton(0)) { return; }
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
