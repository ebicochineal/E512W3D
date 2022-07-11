#pragma once

#include "E512W3DUtils.hpp"
#include "E512W3DWindow.hpp"
E512W3DWindowManager e512w3d;

void setup();
void loop();

#ifdef _WIN32
    #define WC_NAME TEXT("e512w3d")
    #define WT_NAME TEXT("e512w3d")
    
    E512Array<bool> keys = E512Array<bool>(128, false);
    
    bool keydown (char c) { return keys[c]; }
    
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
        
        while (true) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if(msg.message == WM_QUIT) { break; }
                DispatchMessage(&msg);
            } else {
                loop();
                Sleep(1);
            }
        }
        return msg.wParam;
    }
#else
    #if __has_include(<curses.h>) && defined(USENCURSES)
        E512Array<bool> keys = E512Array<bool>(128, false);
        bool readkey = false;
        bool keydown (char c) {
            readkey = true;
            return keys[c];
        }
        void clearkeyarray () {
            if (!readkey) { return; }
            readkey = false;
            for (int i=0; i < 128; ++i) { keys[i] = false; }
        }
        
        int main () {
            initscr();
            start_color();
            noecho();
            curs_set(0);
            cbreak();
            timeout(0);
            
            ncc.init_colors();
            setup();
            
            while (true) {
                char c = getch();
                if (c >= 'a' && c <= 'z') { keys['A'+c-'a'] = true; }
                if (c >= 0 && c < 128) { keys[c] = true; }
                if (keys[0x1b]) { break; }
                loop();
                clearkeyarray();
                flushinp();
                delay(16);
            }
            
            // while (true) {
            //     char c = getch();
            //     if (c == 0x1b) { break; }
            //     loop();
            //     delay(16);
            // }
            
            ncc.end_colors();
            endwin();
            return 0;
        }
    #else
        bool keydown (char c) { return false; }
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






