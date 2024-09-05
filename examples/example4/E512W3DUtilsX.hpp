#pragma once

#define USENCURSES

#if defined(ARDUINO_M5Stick_C)
    #include "M5StickC.h"
#elif defined(ARDUINO_M5Stick_C_PLUS)
    #include "M5StickCPlus.h"
#elif defined(ARDUINO_M5Stack_Core_ESP32)
    #include "M5Stack.h"
#elif defined(ARDUINO_M5STACK_FIRE)
    #include "M5Stack.h"
#elif defined(ARDUINO_M5STACK_Core2)
    #include "M5Core2.h"
#elif defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350)
    #include <Adafruit_GFX.h>
    #include <Adafruit_ST7735.h>
    #include <SPI.h>
    
    #define TFT_CS   17
    #define TFT_RST  22
    #define TFT_DC   28
    Adafruit_ST7735 tft = Adafruit_ST7735(&SPI, TFT_CS, TFT_DC, TFT_RST);
    #define TFT_MOSI 19
    #define TFT_SCLK 18
    
    class DummyMPU6886 {
    public:
        void Init () {}
        void getAccelData(float* ax, float* ay, float* az) {}
        void getGyroData(float* gx, float* gy, float* gz) {}
    };

    class DummyLcd {
    public:
        uint16_t height = 80;
        uint16_t width = 160;
        uint16_t x = 0;
        uint16_t y = 0;
        void setRotation (int x) {}
        uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }
        void setCursor (int x, int y) { this->x = x; this->y = y; }
    };
    class DummyAxp {
    public:
        void ScreenBreath (int x) {}
        double GetVapsData () { return 0; }
    };

    class M5StickC {
    public:
        DummyLcd Lcd;
        DummyAxp Axp;
        DummyMPU6886 MPU6886;
        
        void begin () {
            tft.initR(INITR_MINI160x80);
            tft.invertDisplay(true);
            tft.fillScreen(ST77XX_BLACK);
            
            // Screen edge noise countermeasures
            delay(100);
            tft.setRotation(0);
            tft.fillScreen(ST77XX_BLACK);
            tft.setRotation(1);
            tft.fillScreen(ST77XX_BLACK);
            tft.setRotation(2);
            tft.fillScreen(ST77XX_BLACK);
            tft.setRotation(3);
            tft.fillScreen(ST77XX_BLACK);
            tft.setRotation(0);
            tft.fillScreen(ST77XX_BLACK);
            tft.setRotation(3);
            tft.fillScreen(ST77XX_BLACK);
        }
        void update () {}
    };

    M5StickC M5;

    class TFT_eSprite {
    public:
        int colordepth = 24;
        int width = 160;
        int height = 80;
        uint16_t* buff;
        DummyLcd* lcd;
        TFT_eSprite (DummyLcd* lcd) { this->lcd = lcd; }
        void setColorDepth (int v) {}
        void createSprite (int width, int height) {
            this->width = width;
            this->height = height;
            this->buff = new uint16_t[height * width];
        }
        void pushSprite (int px, int py) {
            // RGB -> BGR
            for (int y = 0; y < this->height; y += 1) {
                for (int x = 0; x < this->width; x += 1) {
                    uint32_t c = this->buff[y*this->width+x];
                    uint16_t r = c & 0xF800;
                    uint16_t g = c & 0x07E0;
                    uint16_t b = c & 0x001F;
                    c = (b<<11) | g | (r>>11);
                    this->buff[y*this->width+x] = c;
                }
            }
            tft.drawRGBBitmap(0, 0, this->buff, this->width, this->height);
        }
        
        void fillSprite (uint16_t color) {
            for (int i = 0; i < this->height*this->width; ++i) { this->buff[i] = color; }
        }
        void drawPixel (uint16_t x, uint16_t y, uint16_t color) { this->buff[y*this->width+x] = color; }
        uint16_t readPixel (uint16_t x, uint16_t y) { return this->buff[y*this->width+x]; }
    };
#else
    #include <cmath>
    #include <chrono>
    #include <cstdlib>
    #include <iostream>
    #include <string>
    #include <sstream>
    #include <unistd.h>
    
    #define abs(x) std::abs(x)
    
    template<class T> std::string String (T t) { return std::to_string(t); }
    int max (uint32_t a, uint32_t b) { return std::max(a, b); }
    int min (uint32_t a, uint32_t b) { return std::min(a, b); }
    int max (int a, int b) { return std::max(a, b); }
    int min (int a, int b) { return std::min(a, b); }
    double max (double a, double b) { return std::max(a, b); }
    double min (double a, double b) { return std::min(a, b); }
    float max (float a, float b) { return std::max(a, b); }
    float min (float a, float b) { return std::min(a, b); }
    
    
    void delay (int v) { usleep(v * 1000); }
    void delayMicroseconds (int v) { usleep(v); }
    
    int64_t map(int64_t x, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    class StopWatchChrono {
    public:
        std::chrono::system_clock::time_point start, tstart, end;
        StopWatchChrono () {
            this->start = std::chrono::system_clock::now();
        }
        inline void stop () {
            this->tstart = std::chrono::system_clock::now();
        }
        inline void resume () {
            this->start += std::chrono::system_clock::now() - this->tstart;
        }
        inline uint64_t get_milli_time () {
            this->end = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        }
    };
    
    uint64_t millis () {
        static StopWatchChrono swc;
        return swc.get_milli_time();
    }
    #if defined(_WIN32)
        #include <windows.h>
    #elif defined(__EMSCRIPTEN__)
        #include <emscripten.h>
        
        EM_JS(void, CanvasSetup, (int width, int height), {
            e512w3d_canvas = document.getElementById('e512w3d-canvas');
            e512w3d_ctx = e512w3d_canvas.getContext('2d');
            e512w3d_buff = e512w3d_ctx.createImageData(width, height);
            e512w3d_canvas.width = width;
            e512w3d_canvas.height = height;
            e512w3d_canvas.addEventListener("mousemove", (e) => {
                var rect = e.target.getBoundingClientRect();
                e512w3d_mouse_position_x = ((e.clientX - rect.left) / parseFloat(e512w3d_canvas.style.width)) * e512w3d_canvas.width;
                e512w3d_mouse_position_y = ((e.clientY - rect.top) / parseFloat(e512w3d_canvas.style.height)) * e512w3d_canvas.height;
            });
        });
        EM_JS(void, CanvasPut, (int x, int y, int r, int g, int b), {
            var p = (y*e512w3d_canvas.width+x)*4;
            e512w3d_buff.data[p] = r;
            e512w3d_buff.data[p+1] = g;
            e512w3d_buff.data[p+2] = b;
            e512w3d_buff.data[p+3] = 255; });
        EM_JS(void, CanvasPush, (), { e512w3d_ctx.putImageData(e512w3d_buff, 0, 0); });
        EM_JS(bool, GetKey, (int n), { return e512w3d_keys[n]; });
    #elif defined(__ANDROID_API__)
        
    #else
        // float abs (double v) { return std::abs(v); }
        // float abs (float v) { return std::abs(v); }
        #if __has_include(<curses.h>) && defined(USENCURSES)
            #include <curses.h>
            
            class NCColor {
            public:
                int16_t rs[256];
                int16_t gs[256];
                int16_t bs[256];
                
                int16_t pas[256];
                int16_t pbs[256];
                
                NCColor () {}
                
                void init_colors () {
                    int16_t r, g, b;
                    for (int i = 0; i < 216; ++i) {
                        color_content(1+i, &r, &g, &b);
                        this->rs[i] = r;
                        this->gs[i] = g;
                        this->bs[i] = b;
                    }
                    
                    int16_t pa, pb;
                    for (int i = 0; i < 216; ++i) {
                        pair_content(1+i, &pa, &pb);
                        this->pas[i] = pa;
                        this->pbs[i] = pb;
                    }
                    
                    for (int r = 0; r < 6; ++r) {
                        for (int g = 0; g < 6; ++g) {
                            for (int b = 0; b < 6; ++b) {
                                int n = r*36+g*6+b;
                                init_color(1+n, 200*r, 200*g, 200*b);
                                init_pair(1+n, 1+n, 1+n);
                            }
                        }
                    }
                    
                    init_pair(255, 216, 1);
                }
                void end_colors () {
                    // for (int r = 0; r < 6; ++r) {
                    //     for (int g = 0; g < 6; ++g) {
                    //         for (int b = 0; b < 6; ++b) {
                    //             int n = r*36+g*6+b;
                    //             init_color(1+n, this->rs[n], this->gs[n], this->bs[n]);
                    //             init_pair(1+n, 1+n, 1+n);
                    //         }
                    //     }
                    // }
                    for (int i = 0; i < 216; ++i) {
                        init_color(1+i, this->rs[i], this->gs[i], this->bs[i]);
                    }
                    
                    for (int i = 0; i < 216; ++i) {
                        init_pair(1+i, this->pas[i], this->pbs[i]);
                    }
                    
                    
                }
                
                int16_t index (int16_t r, int16_t g, int16_t b) {
                    r /= 51;
                    g /= 51;
                    b /= 51;
                    return r * 36 + g * 6 + b + 1;
                }
            };
            NCColor ncc;
            
        #endif
    #endif
    
    class DummyMPU6886 {
    public:
        void Init () {}
        void getAccelData(float* ax, float* ay, float* az) {}
        void getGyroData(float* gx, float* gy, float* gz) {}
    };

    class DummyLcd {
    public:
        uint16_t height = 320;
        uint16_t width = 320;
        uint16_t x = 0;
        uint16_t y = 0;
        void setRotation (int x) {}
        uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }
        void setCursor (int x, int y) { this->x = x; this->y = y; }
        
        
    #if defined(_WIN32)
        HWND hwnd;
        void print (std::string s) {
            HDC hdc = GetDC(hwnd);
            HFONT hFont = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, FF_SWISS, TEXT("Arial"));
            SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, 0xFFFFFF);
            TextOut(hdc, this->x, this->y, TEXT(s.c_str()), s.size());
            DeleteObject(hFont);
            ReleaseDC(this->hwnd, hdc);
        }
    #else
        #if __has_include(<curses.h>) && defined(USENCURSES)
            void print (std::string x) {
                attron(COLOR_PAIR(255));
                move(this->y, this->x);
                addstr(x.c_str());
                refresh();
            }
        #else
            void print (std::string x) { std::cout << x << std::endl; }
        #endif
        
    #endif
    };
    class DummyAxp {
    public:
        void ScreenBreath (int x) {}
        double GetVapsData () { return 0; }
    };

    class M5StickC {
    public:
        DummyLcd Lcd;
        DummyAxp Axp;
        DummyMPU6886 MPU6886;
        void begin () {}
        void update () {}
    #if defined(_WIN32)
        HWND hwnd;
        uint16_t width;
        uint16_t height;
        uint32_t* pixels;
        bool draw = false;
        
        void window_app (HWND hwnd, uint16_t width, uint16_t height) {
            this->hwnd = hwnd;
            this->width = width;
            this->height = height;
            this->pixels = new uint32_t[width*height];
            this->Lcd.hwnd = hwnd;
        }
    #elif defined(__ANDROID_API__)
        int width;
        int height;
        int tw = 1;
        int th = 1;
        uint8_t* pixels = new uint8_t[1];
        size_t buffer_size = 1;
        void resize (int width, int height) {
            this->width = width;
            this->height = height;
            this->tw = 1;
            this->th = 1;
            while (this->tw < width) { this->tw *= 2; }
            while (this->th < height) { this->th *= 2; }
            while (this->buffer_size < this->tw*this->th * 3) {
                this->buffer_size *= 2;
                delete[] this->pixels;
                this->pixels = new uint8_t[this->buffer_size];
            }
        }
        int textureSize () { return this->tw * this->th; }
    #endif
    };

    M5StickC M5;

    class TFT_eSprite {
    public:
        int colordepth = 24;
        int width = 160;
        int height = 80;
        uint16_t* buff;
        DummyLcd* lcd;
        TFT_eSprite (DummyLcd* lcd) { this->lcd = lcd; }
        void setColorDepth (int v) {}
        void createSprite (int width, int height) {
            this->width = width;
            this->height = height;
            this->buff = new uint16_t[height * width];
        }
    #if defined(_WIN32)
        void pushSprite (int px, int py) {
            for (int y = 0; y < this->height; y += 1) {
                for (int x = 0; x < this->width; x += 1) {
                    uint32_t c1 = this->buff[y*this->width+x];
                    
                    // color565
                    uint16_t r = (((c1 & 0xF800) >> 11) << 3);
                    uint16_t g = (((c1 & 0x07E0) >>  5) << 2);
                    uint16_t b = (((c1 & 0x001F)      ) << 3);
                    M5.pixels[y*M5.width+x] = (r << 16) | (g << 8) | (b);
                    
                    // color555
                    // uint16_t r = ((c1 >> 10) & 0b11111) << 3;
                    // uint16_t g = ((c1 >>  5) & 0b11111) << 3;
                    // uint16_t b = ( c1        & 0b11111) << 3;
                    // M5.pixels[y*M5.width+x] = (r << 16) | (g << 8) | (b);
                    
                    // SetPixel(hdc, x, y, r | (g << 8) | (b << 16));
                }
            }
            InvalidateRect(M5.hwnd, NULL, false);
            UpdateWindow(M5.hwnd);
        }
    #elif defined(__ANDROID_API__)
        void pushSprite (int px, int py) {
            if (M5.textureSize() < this->width*this->height) { M5.resize(this->width, this->height); }
            for (int y = 0; y < this->height; y += 1) {
                for (int x = 0; x < this->width; x += 1) {
                    uint32_t c = this->buff[y*this->width+x];
                    M5.pixels[(y * M5.tw + x) * 3 + 0] = (((c & 0xF800) >> 11) << 3);
                    M5.pixels[(y * M5.tw + x) * 3 + 1] = (((c & 0x07E0) >> 5) << 2);
                    M5.pixels[(y * M5.tw + x) * 3 + 2] = (((c & 0x001F)) << 3);
                    //M5.pixels[(y * M5.tw + x) * 4 + 3] = 255;
                }
            }
        }
    #elif defined(__EMSCRIPTEN__)
        void pushSprite (int px, int py) {
            for (int y = 0; y < this->height; y += 1) {
                for (int x = 0; x < this->width; x += 1) {
                    uint32_t c1 = this->buff[y*this->width+x];
                    int16_t r = (((c1 >> 11) & 0b11111) << 3);
                    int16_t g = (((c1 >>  5) & 0b111111) << 2);
                    int16_t b = (( c1        & 0b11111) << 3);
                    CanvasPut(x, y, r, g, b);
                }
            }
            CanvasPush();
        }
    #else
        #if __has_include(<curses.h>) && defined(USENCURSES)
            void pushSprite (int px, int py) {
                for (int y = 0; y < this->height; y += 2) {
                    move(y/2, 0);
                    for (int x = 0; x < this->width; x += 1) {
                        uint32_t c1 = this->buff[y*this->width+x];
                        uint32_t c2 = this->buff[(y+1)%this->height*this->width+x];
                        
                        // color565s
                        // int16_t r = ((((c1 >> 11) & 0b11111) << 3);
                        // int16_t g = ((((c1 >>  5) & 0b111111) << 2);
                        // int16_t b = ((( c1        & 0b11111) << 3);
                        // color565d
                        int16_t r = ((((c1 >> 11) &  0b11111) << 3) + (((c2 >> 11) & 0b11111) << 3)) >> 1;
                        int16_t g = ((((c1 >>  5) & 0b111111) << 2) + (((c2 >>  5) & 0b111111) << 2)) >> 1;
                        int16_t b = ((( c1        &  0b11111) << 3) + (( c2        & 0b11111) << 3)) >> 1;
                        
                        // color555
                        // int16_t r = ((((c1 >> 10) & 0b11111) << 3) + (((c2 >> 10) & 0b11111) << 3)) >> 1;
                        // int16_t g = ((((c1 >>  5) & 0b11111) << 3) + (((c2 >>  5) & 0b11111) << 3)) >> 1;
                        // int16_t b = ((( c1        & 0b11111) << 3) + (( c2        & 0b11111) << 3)) >> 1;
                        
                        attron(COLOR_PAIR(ncc.index(r, g, b)));
                        addstr(" ");
                    }
                    
                }
                refresh();
            }
        #else
            void pushSprite (int px, int py) {
                std::string s = "\x1B[2J\x1B[H";
                for (int y = 0; y < this->height; y += 2) {
                    for (int x = 0; x < this->width; x += 1) {
                        uint32_t c1 = this->buff[y*this->width+x];
                        uint32_t c2 = this->buff[(y+1)%this->height*this->width+x];
                        
                        // color565s
                        // int16_t r = ((((c1 >> 11) & 0b11111) << 3);
                        // int16_t g = ((((c1 >>  5) & 0b111111) << 2);
                        // int16_t b = ((( c1        & 0b11111) << 3);
                        // color565d
                        int16_t r = ((((c1 >> 11) &  0b11111) << 3) + (((c2 >> 11) & 0b11111) << 3)) >> 1;
                        int16_t g = ((((c1 >>  5) & 0b111111) << 2) + (((c2 >>  5) & 0b111111) << 2)) >> 1;
                        int16_t b = ((( c1        &  0b11111) << 3) + (( c2        & 0b11111) << 3)) >> 1;
                        
                        // color555
                        // int16_t r = ((((c1 >> 10) & 0b11111) << 3) + (((c2 >> 10) & 0b11111) << 3)) >> 1;
                        // int16_t g = ((((c1 >>  5) & 0b11111) << 3) + (((c2 >>  5) & 0b11111) << 3)) >> 1;
                        // int16_t b = ((( c1        & 0b11111) << 3) + (( c2        & 0b11111) << 3)) >> 1;
                        
                        // s += "\033[38;2;"+std::to_string(r)+";"+std::to_string(g)+";" + std::to_string(b) + "m@";
                        s += "\033[48;2;"+std::to_string(r)+";"+std::to_string(g)+";" + std::to_string(b) + "m ";
                    }
                    s += "\033[0m\n";
                }
                std::cout << s;
            }
        #endif
    #endif
        void fillSprite (uint16_t color) {
            for (int i = 0; i < this->height*this->width; ++i) { this->buff[i] = color; }
        }
        void drawPixel (uint16_t x, uint16_t y, uint16_t color) { this->buff[y*this->width+x] = color; }
        uint16_t readPixel (uint16_t x, uint16_t y) { return this->buff[y*this->width+x]; }
    };


#endif




