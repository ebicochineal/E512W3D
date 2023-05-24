#pragma once

#include "E512W3DUtils.hpp"
#include "E512W3DWindow.hpp"
E512W3DWindowManager e512w3d;


#if !(defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2) || defined(ARDUINO_ARCH_RP2040))
    void setup();
    void loop();
#endif


#if defined(ARDUINO_M5Stick_C) || defined(ARDUINO_ARCH_RP2040)
    inline void delay1 () { delay(1); }
#else
    inline void delay1 () { usleep(10); }
#endif

E512W3DWindow* rendertargetwindow;
Object3D* rendertargetobject;
bool rendertargetchild = false;
bool render1 = false;
bool isrunning = true;

void loop1 () {
    if (render1 && rendertargetwindow != NULL) {
        if (rendertargetobject == NULL) {
            for (auto&& i : rendertargetwindow->child) { rendertargetwindow->draw(*i, 1, 2, rendertargetchild); }
        } else {
            rendertargetwindow->draw(*rendertargetobject, 1, 2, rendertargetchild);
        }
        render1 = false;
    }
}

#if defined(_WIN32)
    #include <thread>
    bool rnf0 = false;
    void threadLoop0 () {
        while (isrunning) {
            if (rnf0) {
                loop();
                rnf0 = false;
            }
            delay1();
        }
    }

    bool rnf1 = false;
    void threadLoop1 () {
        while (isrunning) {
            if (rnf1) {
                loop1();
                rnf1 = false;
            }
            delay1();
        }
    }
#endif

#if defined(ARDUINO_M5Stick_C)
    bool multicorebegin = true;
    bool rnf1 = false;
    void threadLoop1 (void* param) {
        while (isrunning) {
            loop1();
            delay1();
        }
    }
    void multiCoreDraw (E512W3DWindow& w) {
        if (multicorebegin) {
            xTaskCreatePinnedToCore(threadLoop1, "Core0", 4096, NULL, 1, NULL, 0);
            multicorebegin = false;
        }
        rendertargetwindow = &w;
        rendertargetobject = NULL;
        rendertargetchild = true;
        w.begin();
        render1 = true;
        for (auto&& i : w.child) { w.draw(*i, 0, 2, rendertargetchild); }
        while (render1 && isrunning) { delay1(); }
    }
    void multiCoreDraw (E512W3DWindow& w, Object3D& o, bool child = false) {
        if (multicorebegin) {
            xTaskCreatePinnedToCore(threadLoop1, "Core0", 4096, NULL, 1, NULL, 0);
            multicorebegin = false;
        }
        rendertargetwindow = &w;
        rendertargetobject = &o;
        rendertargetchild = child;
        render1 = true;
        w.draw(o, 0, 2, child);
        while (render1 && isrunning) { delay1(); }
    }
#else
    void multiCoreDraw (E512W3DWindow& w) {
        rendertargetwindow = &w;
        rendertargetobject = NULL;
        rendertargetchild = true;
        w.begin();
        render1 = true;
        for (auto&& i : w.child) { w.draw(*i, 0, 2, rendertargetchild); }
        while (render1 && isrunning) { delay1(); }
    }
    void multiCoreDraw (E512W3DWindow& w, Object3D& o, bool child = false) {
        rendertargetwindow = &w;
        rendertargetobject = &o;
        rendertargetchild = child;
        render1 = true;
        w.draw(o, 0, 2, child);
        while (render1 && isrunning) { delay1(); }
    }
#endif




#if defined(_WIN32)
    #define WC_NAME TEXT("e512w3d")
    #define WT_NAME TEXT("e512w3d")
    LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_PAINT:
            {
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
            case WM_KEYDOWN:
            {
                if (wp >= 0 && wp < 128) { keys[wp] = true; }
                return 0;
            }
            case WM_KEYUP:
            {
                if (wp >= 0 && wp < 128) { keys[wp] = false; }
                return 0;
            }
            case WM_KILLFOCUS:
            {
                for (auto&& i : keys) { i = false; }
                return 0;
            }
        }
        return DefWindowProc(hwnd , msg , wp , lp);
    }
    int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
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
        
        setup();
        
        hwnd = CreateWindow(
            WC_NAME, WT_NAME, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
            100, 100, e512w3d.width+6, e512w3d.height+28,
            NULL, NULL, hInstance, NULL
        );
        if (hwnd == NULL) { return 0; }
        M5.window_app(hwnd, e512w3d.width, e512w3d.height);
        POINT pos;
        
        std::thread th0(threadLoop0);
        std::thread th1(threadLoop1);
        while (true) {
            GetCursorPos(&pos);
            ScreenToClient(hwnd, &pos);
            cursor_x = pos.x;
            cursor_y = pos.y;
            cursor_l = GetKeyState(VK_LBUTTON) & 0x80;
            cursor_m = GetKeyState(VK_MBUTTON) & 0x80;
            cursor_r = GetKeyState(VK_RBUTTON) & 0x80;
            
            
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if(msg.message == WM_QUIT) { break; }
                DispatchMessage(&msg);
            } else {
                rnf0 = true;
                rnf1 = true;
                delay1();
            }
        }
        isrunning = false;
        th0.join();
        th1.join();
        return msg.wParam;
    }
#elif defined(__EMSCRIPTEN__)
    int main () {
        setup();
        
        EM_ASM(
            e512w3d_mouse_l = false;
            e512w3d_mouse_m = false;
            e512w3d_mouse_r = false;
            e512w3d_mouse_position_x = 0;
            e512w3d_mouse_position_y = 0;
            e512w3d_buff = null;
            e512w3d_canvas = null;
            e512w3d_ctx = null;
            e512w3d_keys = Array(128);
            
            window.addEventListener("mousemove", (e) => {
                var rect = e512w3d_canvas.getBoundingClientRect();
                e512w3d_mouse_position_x = ((e.clientX - rect.left) / parseFloat(e512w3d_canvas.style.width)) * e512w3d_canvas.width;
                e512w3d_mouse_position_y = ((e.clientY - rect.top) / parseFloat(e512w3d_canvas.style.height)) * e512w3d_canvas.height;
            });
            
            window.addEventListener("mousedown", (e) => {
                if (e.button == 0) { e512w3d_mouse_l = true; }
                if (e.button == 1) { e512w3d_mouse_m = true; }
                if (e.button == 2) { e512w3d_mouse_r = true; }
            });
            
            window.addEventListener("mouseup", (e) => {
                if (e.button == 0) { e512w3d_mouse_l = false; }
                if (e.button == 1) { e512w3d_mouse_m = false; }
                if (e.button == 2) { e512w3d_mouse_r = false; }
            });
            
            window.addEventListener("mouseleave", (e) => {
                if (e.button == 0) { e512w3d_mouse_l = false; }
                if (e.button == 1) { e512w3d_mouse_m = false; }
                if (e.button == 2) { e512w3d_mouse_r = false; }
            });
            
            window.addEventListener("touchstart", (e) => {
                e512w3d_mouse_l = true;
                var x = 0;
                var y = 0;
                if (e.touches && e.touches[0]) {
                    x = e.touches[0].clientX;
                    y = e.touches[0].clientY;
                } else if (e.clientX && e.clientY) {
                    x = e.clientX;
                    y = e.clientY;
                } else {
                    return;
                }
                var rect = e512w3d_canvas.getBoundingClientRect();
                e512w3d_mouse_position_x = ((x - rect.left) / parseFloat(e512w3d_canvas.style.width)) * e512w3d_canvas.width;
                e512w3d_mouse_position_y = ((y - rect.top) / parseFloat(e512w3d_canvas.style.height)) * e512w3d_canvas.height;
            });
            window.addEventListener("touchmove", (e) => {
                e512w3d_mouse_l = true;
                var x = 0;
                var y = 0;
                if (e.touches && e.touches[0]) {
                    x = e.touches[0].clientX;
                    y = e.touches[0].clientY;
                } else if (e.clientX && e.clientY) {
                    x = e.clientX;
                    y = e.clientY;
                } else {
                    return;
                }
                var rect = e512w3d_canvas.getBoundingClientRect();
                e512w3d_mouse_position_x = ((x - rect.left) / parseFloat(e512w3d_canvas.style.width)) * e512w3d_canvas.width;
                e512w3d_mouse_position_y = ((y - rect.top) / parseFloat(e512w3d_canvas.style.height)) * e512w3d_canvas.height;
            });
            window.addEventListener("touchend", (e) => {
                e512w3d_mouse_l = false;
            });
            
            
            window.addEventListener("keydown", (e) => {
                if (e.isComposing || e.keyCode >= 128) { return; }
                e512w3d_keys[e.keyCode] = true;
            });
            
            window.addEventListener("keyup", (e) => {
                if (e.isComposing || e.keyCode >= 128) { return; }
                e512w3d_keys[e.keyCode] = false;
            });
        );
        
        CanvasSetup(e512w3d.width, e512w3d.height);
        while (true) {
            cursor_x = EM_ASM_INT(return e512w3d_mouse_position_x);
            cursor_y = EM_ASM_INT(return e512w3d_mouse_position_y);
            cursor_l = EM_ASM_INT(return e512w3d_mouse_l);
            cursor_m = EM_ASM_INT(return e512w3d_mouse_m);
            cursor_r = EM_ASM_INT(return e512w3d_mouse_r);
            for (int i=0; i < 128; ++i) { keys[i] = GetKey(i); }
            loop();
            emscripten_sleep(1);
        }
        return 0;
    }
#elif !(defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2) || defined(ARDUINO_ARCH_RP2040))
    #if __has_include(<curses.h>) && defined(USENCURSES)
        #include <csignal>
        bool floop = true;
        void signal_handler(int sig) { floop = false; }
        
        int main () {
            initscr();
            start_color();
            noecho();
            curs_set(0);
            cbreak();
            timeout(0);
            
            ncc.init_colors();
            setup();
            
            signal(SIGINT, signal_handler);
            
            char c = -1;
            while (floop) {
                loop();
                
                clearkeyarray();
                while (true) {
                    c = getch();
                    if (c == 8 || c == 127) { keys[8] = true; }
                    if (c >= 'a' && c <= 'z') { keys['A'+c-'a'] = true; }
                    if (c >= 'A' && c <= 'Z') { keys[16] = true; }// shift
                    if (c >= 0 && c < 128) {
                        keys[c] = true;
                    } else {
                        c = -1;
                        flushinp();
                        break;
                    }
                }
                if (keys[0x1b]) { break; }
                delay(1);
            }
            
            ncc.end_colors();
            endwin();
            return 0;
        }
    #else
        int main () {
            setup();
            while (true) {
                loop();
                delay(16);
            }
            return 0;
        }
    #endif
#endif






