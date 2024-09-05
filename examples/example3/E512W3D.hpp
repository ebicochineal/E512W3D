#pragma once

#include "E512W3DUtils.hpp"
#include "E512W3DWindow.hpp"
E512W3DWindowManager e512w3d;



#if !(defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350))
    void setup();
    void loop();
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
                loop();
                usleep(100);
            }
        }
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
#elif defined(__ANDROID_API__)
    /*
    Copyright (C) 2010 The Android オープンソース プロジェクト
    
    Apache License Version 2.0 (「本ライセンス」) に基づいてライセンスされます。;
    本ライセンスに準拠しない場合はこのファイルを使用できません。
    本ライセンスのコピーは、以下の場所から入手できます。
    
        http://www.apache.org/licenses/LICENSE-2.0
    
    適用される法令または書面での同意によって命じられない限り、本ライセンスに基づいて頒布されるソフトウェアは、
    明示黙示を問わず、いかなる保証も条件もなしに現状のまま
    頒布されます。
    本ライセンスでの権利と制限を規定した文言ついては、
    本ライセンスを参照してください。
    */
    /* android native-activity template modification */
    #include <malloc.h>

    struct saved_state { float angle; int32_t x; int32_t y; };

    struct engine {
        struct android_app* app;
        ASensorManager* sensorManager;
        const ASensor* accelerometerSensor;
        ASensorEventQueue* sensorEventQueue;
        int animating;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;
        int32_t width;
        int32_t height;
        struct saved_state state;
    };

    static int engine_init_display(struct engine* engine) {
        const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
        };
        EGLint w, h, format;
        EGLint numConfigs;
        EGLConfig config;
        EGLSurface surface;
        EGLContext context;

        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(display, 0, 0);
        
        eglChooseConfig(display, attribs, &config, 1, &numConfigs);
        
        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

        ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
        
        surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
        context = eglCreateContext(display, config, NULL, NULL);
        
        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) { return -1; }
        
        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
        
        engine->display = display;
        engine->context = context;
        engine->surface = surface;
        engine->width = w;
        engine->height = h;
        engine->state.angle = 0;
        
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glDisable(GL_DEPTH_TEST);
        
        return 0;
    }


    uint32_t android_texture_id = 0;
    static void androidDrawTexture (engine* engine) {
        if (android_texture_id < 1) { glGenTextures(1, &android_texture_id); }
        if (android_texture_id < 1) { return; }
        
        if (e512w3d.width * e512w3d.height < 1) { return; }
        if (engine->width * engine->height < 1) { return; }
        
        // if (M5.textureSize() < e512w3d.width*e512w3d.height) { M5.resize(e512w3d.width, e512w3d.height); }
        
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, android_texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, M5.tw, M5.th, 0, GL_RGB, GL_UNSIGNED_BYTE, M5.pixels);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 3);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindTexture(GL_TEXTURE_2D, android_texture_id);
        
        float l = 0.0f;
        float u = 0.0f;
        float r = (float)e512w3d.width / (float)M5.tw;
        float d = (float)e512w3d.height / (float)M5.th;
        float uv[8] = {l, d, r, d, l, u, r, u};
        glTexCoordPointer(2, GL_FLOAT, 0, uv);
        
        float awh = (float)e512w3d.width / (float)e512w3d.height;
        float bwh = (float)engine->width / (float)engine->height;
        
        const bool vf = awh / bwh <= 1.0f;
        float vx = vf ? awh / bwh : 1.0f;
        float vy = vf ? 1.0f : bwh / awh;
        
        GLfloat vs[12] = {-vx, -vy, 0.0f, vx, -vy, 0.0f, -vx, vy, 0.0f, vx, vy, 0.0f};
        glVertexPointer(3, GL_FLOAT, 0, vs);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        // glDisable(GL_BLEND);
    }

    static void engine_draw_frame(struct engine* engine) {
        static float r = 200.0f / 255.0f;
        static float g =  43.0f / 255.0f;
        static float b =  85.0f / 255.0f;
        if (engine->display == NULL) { return; }
        glClearColor(r, g, b, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        androidDrawTexture(engine);
        eglSwapBuffers(engine->display, engine->surface);
    }

    static void engine_term_display(struct engine* engine) {
        if (engine->display != EGL_NO_DISPLAY) {
            eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (engine->context != EGL_NO_CONTEXT) {
                eglDestroyContext(engine->display, engine->context);
            }
            if (engine->surface != EGL_NO_SURFACE) {
                eglDestroySurface(engine->display, engine->surface);
            }
            eglTerminate(engine->display);
        }
        engine->animating = 0;
        engine->display = EGL_NO_DISPLAY;
        engine->context = EGL_NO_CONTEXT;
        engine->surface = EGL_NO_SURFACE;
    }

    static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
        struct engine* engine = (struct engine*)app->userData;
        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
            engine->state.x = AMotionEvent_getX(event, 0);
            engine->state.y = AMotionEvent_getY(event, 0);
            
            if (e512w3d.width * e512w3d.height < 1) { return 1; }
            if (engine->width * engine->height < 1) { return 1; }
            float ax = (float)engine->state.x / (float)engine->width;
            float ay = (float)engine->state.y / (float)engine->height;
            float awh = (float)e512w3d.width / (float)e512w3d.height;
            float bwh = (float)engine->width / (float)engine->height;
            
            const bool vf = awh / bwh <= 1.0f;
            float w = vf ? awh / bwh : 1.0f;
            float h = vf ? 1.0f : bwh / awh;
            
            cursor_x = (((ax-0.5f) / w) + 0.5f) * (float)e512w3d.width ;
            cursor_y = (((ay-0.5f) / h) + 0.5f) * (float)e512w3d.height;
            
            cursor_l = !(AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_BUTTON_RELEASE > 0);
            return 1;
        }
        return 0;
    }

    static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
        struct engine* engine = (struct engine*)app->userData;
        switch (cmd) {
        case APP_CMD_SAVE_STATE:
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
                ASensorEventQueue_setEventRate(engine->sensorEventQueue, engine->accelerometerSensor, (1000L / 60) * 1000);
            }
            engine->animating = 1;
            break;
        case APP_CMD_LOST_FOCUS:
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue, engine->accelerometerSensor);
            }
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
        }
    }

    void android_main(struct android_app* state) {
        struct engine engine;
        
        memset(&engine, 0, sizeof(engine));
        state->userData = &engine;
        state->onAppCmd = engine_handle_cmd;
        state->onInputEvent = engine_handle_input;
        engine.app = state;
        
        engine.sensorManager = ASensorManager_getInstance();
        engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
        engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);

        if (state->savedState != NULL) { engine.state = *(struct saved_state*)state->savedState; }
        engine.animating = 1;

        setup();
        
        while (1) {
            int ident, events;
            struct android_poll_source* source;
            while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
                if (source != NULL) { source->process(state, source); }
                
                if (ident == LOOPER_ID_USER) {
                    if (engine.accelerometerSensor != NULL) {
                        ASensorEvent event;
                        while (ASensorEventQueue_getEvents(engine.sensorEventQueue, &event, 1) > 0) {}
                    }
                }
                
                if (state->destroyRequested != 0) {
                    engine_term_display(&engine);
                    return;
                }
            }
            loop();
            if (engine.animating) {
                engine.state.angle += 0.01f;
                if (engine.state.angle > 1) { engine.state.angle = 0; }
                engine_draw_frame(&engine);
            }
        }
    }
    /* android native-activity template modification */
#elif !(defined(ARDUINO_M5Stick_C) || defined(ARDUINO_M5Stick_C_PLUS) || defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_RP2350))
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















