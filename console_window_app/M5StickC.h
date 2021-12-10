#pragma once
#include <cmath>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#endif

class DammyMPU6886 {
public:
    void Init () {};
};

class DammyLcd {
public:
    uint16_t height = 320;
    uint16_t width = 320;
    uint16_t x = 0;
    uint16_t y = 0;
    void setRotation (int x) {};
    
    uint16_t color565 (uint16_t r, uint16_t g, uint16_t b) { return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3); }
    
    void setCursor (int x, int y) { this->x = x; this->y = y; }
    
    
#ifdef _WIN32
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
    void print (std::string x) {
        std::cout << x << std::endl;
    }
#endif
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
    
#ifdef _WIN32
    HWND hwnd;
    uint16_t width;
    uint16_t height;
    uint32_t* pixels;
    bool draw = false;
    void begin () {}
    
    void window_app (HWND hwnd, uint16_t width, uint16_t height) {
        this->hwnd = hwnd;
        this->width = width;
        this->height = height;
        this->pixels = new uint32_t[width*height];
        this->Lcd.hwnd = hwnd;
    }
    
#else
    void begin () {}
#endif
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
    
#ifdef _WIN32
    void pushSprite (int x, int y) {
        for (int y = 0; y < M5.height; y += 1) {
            for (int x = 0; x < M5.width; x += 1) {
                uint32_t c1 = this->buff[y*M5.width+x];
                uint16_t r = (((c1 & 0xF800) >> 11) << 3);
                uint16_t g = (((c1 & 0x07E0) >>  5) << 2);
                uint16_t b = (((c1 & 0x001F)      ) << 3);
                
                M5.pixels[y*M5.width+x] = (r << 16) | (g << 8) | (b);
                // SetPixel(hdc, x, y, r | (g << 8) | (b << 16));
            }
        }
        InvalidateRect(M5.hwnd, NULL, false);
        UpdateWindow(M5.hwnd);
    }
#else
    void pushSprite (int x, int y) {
        std::string s = "\x1B[2J\x1B[H";
        for (int y = 0; y < this->height; y += 2) {
            for (int x = 0; x < this->width; x += 1) {
                uint32_t c1 = this->buff[y*this->width+x];
                uint32_t c2 = this->buff[(y+1)%this->height*this->width+x];
                // uint16_t r = (((c1 & 0xF800) >> 11) << 3);
                // uint16_t g = (((c1 & 0x07E0) >>  5) << 2);
                // uint16_t b = (((c1 & 0x001F)      ) << 3);
                
                uint16_t r = ((((c1 & 0xF800) >> 11) << 3) + (((c2 & 0xF800) >> 11) << 3)) >> 1;
                uint16_t g = ((((c1 & 0x07E0) >>  5) << 2) + (((c2 & 0x07E0) >>  5) << 2)) >> 1;
                uint16_t b = ((((c1 & 0x001F)      ) << 3) + (((c2 & 0x001F)      ) << 3)) >> 1;
                // s += "\033[38;2;"+std::to_string(r)+";"+std::to_string(g)+";" + std::to_string(b) + "m@";
                s += "\033[48;2;"+std::to_string(r)+";"+std::to_string(g)+";" + std::to_string(b) + "m ";
            }
            s += "\033[0m\n";
        }
        std::cout << s;
    }
#endif
    void fillSprite (uint16_t color) {
        for (int i = 0; i < this->height*this->width; ++i) { this->buff[i] = color; }
    }
    
    void drawPixel (uint16_t x, uint16_t y, uint16_t color) {
        this->buff[y*this->width+x] = color;
    }
    uint16_t readPixel (uint16_t x, uint16_t y) { return this->buff[y*this->width+x]; }
};

void delay (int v) { usleep(v * 1000);  }

int64_t map(int64_t x, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#include <chrono>
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
StopWatchChrono gsw;

uint64_t millis () { return gsw.get_milli_time(); }

#ifdef _WIN32
#define WC_NAME TEXT("e512w3d")
#define WT_NAME TEXT("e512w3d")

int gwidth;
int gheight;
LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HBITMAP hBitmap = CreateBitmap(M5.width, M5.height, 1, 32, M5.pixels);
        HDC hBuffer = CreateCompatibleDC(hdc);
        SelectObject(hBuffer, hBitmap);
        BitBlt(hdc, 0, 0, M5.width, M5.height, hBuffer, 0, 0, SRCCOPY);
        DeleteDC(hBuffer);
        DeleteObject(hBitmap);
        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefWindowProc(hwnd , msg , wp , lp);
}
WPARAM mainloop (void (*setup)(), void (*loop)(), int width, int height, HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASS wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL , IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL , IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WC_NAME;
    if (!RegisterClass(&wc)) { return 0; }
    hwnd = CreateWindow(
        WC_NAME, WT_NAME, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        100, 100, width+6, height+28,
        NULL, NULL, hInstance, NULL
    );
    if (hwnd == NULL) { return 0; }
    uint64_t prev = gsw.get_milli_time();
    M5.window_app(hwnd, width, height);
    setup();
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if(msg.message == WM_QUIT) { break; }
            DispatchMessage(&msg);
        } else {
            uint64_t t = gsw.get_milli_time();
            if (t - prev >= 16) {
                loop();
                prev = t;
            }
            Sleep(1);
        }
    }
    return msg.wParam;
}
#define VMAINLOOP(wm) int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) { return mainloop(&setup, &loop, wm.width, wm.height, hInstance, hPrevInstance, lpCmdLine, nCmdShow); }

#else

int mainloop (void (*setup)(), void (*loop)()) {
    setup();
    while (true) {
        loop();
        delay(66);
    }
}
#define VMAINLOOP(wm) int main () { return mainloop(&setup, &loop); }

#endif



