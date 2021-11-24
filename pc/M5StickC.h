#ifndef M5STICKC
#define M5STICKC
#include <cmath>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>

class DammyMPU6886 {
public:
    void Init () {};
};

class DammyLcd {
public:
    uint16_t height = 320;
    uint16_t width = 320;
    
    void setRotation (int x) {};
    
    uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }
    
    void setCursor (int x, int y) {}
    void print (std::string x) {}
};
class DammyAxp {
public:
    void ScreenBreath (int x) {};
    double GetVapsData () { return 0; }
};

class M5StickC {
public:
    DammyLcd Lcd;
    DammyAxp Axp;
    DammyMPU6886 MPU6886;
    
    void begin () {};
};

M5StickC M5;


template<class T> std::string String (T t) { return std::to_string(t); }
int max (int a, int b) { return std::max(a, b); }
int min (int a, int b) { return std::min(a, b); }
double max (double a, double b) { return std::max(a, b); }
double min (double a, double b) { return std::min(a, b); }

class TFT_eSprite {
public:
    int colordepth = 24;
    int width = 160;
    int height = 80;
    uint16_t* buff;
    DammyLcd* lcd;
    TFT_eSprite (DammyLcd* lcd) { this->lcd = lcd; }
    void setColorDepth (int v) {}
    void createSprite (int width, int height) {
        this->width = width;
        this->height = height;
        this->buff = new uint16_t[height * width];
    }
    void pushSprite (int x, int y) {
        std::string s = "\x1B[2J\x1B[H";
        for (int y = 0; y < this->height; y += 2) {
            
            for (int x = 0; x < this->width; x += 1) {
                uint16_t r = ((this->buff[y*this->width+x] & 0b1111100000000000) >> 11) << 3;
                uint16_t g = ((this->buff[y*this->width+x] & 0b0000011111100000) >>  5) << 2;
                uint16_t b = ((this->buff[y*this->width+x] & 0b0000000000011111)      ) << 3;
                
                s += "\033[38;2;"+std::to_string(r)+";"+std::to_string(g)+";" + std::to_string(b) + "m@";
            }
            s += "\033[0m\n";
        }
        std::cout << s;
    }
    void fillSprite (uint16_t color) {
        for (int i = 0; i < this->height*this->width; ++i) { this->buff[i] = color; }
    }
    
    void drawPixel (uint16_t x, uint16_t y, uint16_t color) {
        this->buff[y*this->width+x] = color;
    }
    uint16_t readPixel (uint16_t x, uint16_t y) { return this->buff[y*this->width+x]; }
};


void delay (int v) { usleep(v * 1000);  }

class StopWatch {
public:
    int starts;
    int startm;
    int tstarts = 0;
    int tstartm = 0;
    struct timeb timebuffer;
    StopWatch () {
        ftime(&this->timebuffer);
        this->starts = this->timebuffer.time;
        this->startm = this->timebuffer.millitm;
    }
    inline void stop () {
        ftime(&this->timebuffer);
        this->tstarts = this->timebuffer.time;
        this->tstartm = this->timebuffer.millitm;
    }
    inline void resume () {
        ftime(&this->timebuffer);
        this->starts += this->timebuffer.time - this->tstarts;
        this->startm += this->timebuffer.millitm - this->tstartm;
    }
    inline int get_milli_time () {
        ftime(&this->timebuffer);
        return (this->timebuffer.time - this->starts) * 1000 + (this->timebuffer.millitm - this->startm);
    }
};

StopWatch gsw;
int millis () { return gsw.get_milli_time(); }

#endif

