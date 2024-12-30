E512W3Dで使用できるGUIです  
#### スライダー E512W3DGUISlider  
- スライダーの幅は8ピクセルです  
- つまみを操作するにはE512W3DInput::updateの後にguiUpdateをしてください
- 表示するにはguiDrawしてください
- マウスオーバー中ポップアップで値を表示するにはguiDrawの後にpopupDrawをしてください
- getValue01で値を0.0~1.0の範囲で取得できます
- drawLineやdrawCharなどと同じくスクリーン全体のdrawには対応していません
----
```cpp 
#include "E512W3D.hpp"
#include "E512W3DGUI.hpp"

E512W3DWindow w;
Object3D camera;

void setup () {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(9);
    M5.IMU.Init();
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
        r.guiUpdate(w);
        g.guiUpdate(w);
        b.guiUpdate(w);
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
----
#### ボタン E512W3DGUIButton 
- E512W3DInput::updateの後にguiUpdateをしてください  
- ishover ボタン上にカーソルがある  
- isdrag ボタンを押したまま（ボタンを押したまま移動中でボタン上にカーソルが無くてもtrue）  
- isdown ボタンを押したフレームにtrue  
- isup ボタンを押してボタン上で離された（移動した後にボタン上に戻って離した場合もtrue、移動後ボタン上にカーソルが無い場合はfalse）  

----
#### E512W3DGUISlider
E512W3DGUISlider (int16_t x, int16_t y, uint16_t size, uint16_t hv = 0, uint16_t type = 0, int maxvalue = 1, int value = 0, uint16_t color = 42260, uint16_t bgcolor = 21130)  
void setValue (int value)  
int getValue ()  
void setValue01 (float value)  
float getValue01 ()  
void setMaxValue (int value)  
int getMaxValue ()  
void setSize (uint16_t size)  
int getSize ()  
void guiUpdate (E512W3DWindow& w)  
void guiDraw (E512W3DWindow& w)  
void popupDraw (E512W3DWindow& w)  

#### E512W3DGUIButton
bool ishover  
bool isdown  
bool isup  
bool isdrag  
uint16_t hoveranim = ButtonHoverAnim::Light  
uint16_t downanim = ButtonDownAnim::Right  
E512W3DGUIButton (int16_t x, int16_t y, uint16_t width, uint16_t height, E512Array<uint8_t> text, uint16_t color = 42260, uint16_t text_color = 0xFFFF)  
E512W3DGUIButton (int16_t x, int16_t y, uint16_t width, uint16_t height, const char* text, uint16_t color = 42260, uint16_t text_color = 0xFFFF)  
void guiUpdate (E512W3DWindow& w)  
void guiDraw (E512W3DWindow& w)  

#### enum ButtonHoverAnim
None  
Light  
Dark  

#### enum ButtonDownAnim
None  
Right  
Left  
Down  
Up  