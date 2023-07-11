E512W3Dで使用できるGUIです  
現在スライダーのみです  
- スライダーの幅は8ピクセルです  
- つまみを操作するにはE512W3DInput::updateの後にdragUpdateをしてください
- 表示するにはguiDrawしてください
- マウスオーバー中ポップアップで値を表示するにはguiDrawの後にpopupDrawをしてください
- getValue01で値を0.0~1.0の範囲で取得できます
- drawLineやdrawCharなどと同じくスクリーン全体のdrawには対応していません

```cpp 
#include "E512W3D.hpp"
#include "E512W3DGUI.hpp"

E512W3DWindow w;
Object3D camera;

void setup () {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    M5.MPU6886.Init();
    e512w3d.width = 160*2;
    e512w3d.height = 80*2;
    w.width = e512w3d.width;
    w.height = e512w3d.height;
    w.setCamera(camera);
    e512w3d.add(w);
    e512w3d.begin();
}

E512W3DGUISlider r( 0,  0, 160, 0, 0, 255,   0);
E512W3DGUISlider g( 0,  8, 160, 0, 0, 255,   0);
E512W3DGUISlider b( 0, 16, 160, 0, 0, 255,   0);
void loop () {
    if (e512w3d.isFixedTime()) {
        E512W3DInput::update();
        r.updateDrag(w);
        g.updateDrag(w);
        b.updateDrag(w);
        w.bgcolor = color565(r.getValue(), g.getValue(), b.getValue());
        e512w3d.clear();
        w.begin();
        r.guiDraw(w);
        g.guiDraw(w);
        b.guiDraw(w);
        r.popupDraw(w);
        g.popupDraw(w);
        b.popupDraw(w);
        e512w3d.pushScreen();
    }
}
```



#### E512W3DGUISlider
E512W3DGUISlider (int16_t x, int16_t y, uint16_t size, uint16_t hv = 0, uint16_t type = 0, int maxvalue = 1, int value = 0, uint16_t color = 42260, uint16_t bgcolor = 21130)  
void setValue (int value)  
int getValue ()  
void setValue01 (float value)  
float getValue01 ()  
void setMaxValue (int value)  
int getMaxValue ()  
void setSize (int size)  
int getSize ()  
void updateDrag (E512W3DWindow& w)  
void guiDraw (E512W3DWindow& w)  
void popupDraw (E512W3DWindow& w)  