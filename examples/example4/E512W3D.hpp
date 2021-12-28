#pragma once

#include "E512W3DUtils.hpp"
#include "E512W3DWindow.hpp"
E512W3DWindowManager e512w3d;

void setup();
void loop();

#ifdef _WIN32
    #define WC_NAME TEXT("e512w3d")
    #define WT_NAME TEXT("e512w3d")

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
                if (c == 0x1b) { break; }
                loop();
                delay(16);
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






