## 3d-software-renderer m5stickc/m5stack/console-app/windows-app/ncurses  
- 複数のウィンドウ  
- ワイヤーフレーム、ポリゴン、法線カラー、半透明、テクスチャ  
- テクスチャパース補正  
- Directional Light  
- Ambient  
- テクスチャなしなら8000ポリゴンくらいまで  
- コンソール、ウィンドウアプリ化できる  

![Gif](./img/anim.gif)  
![Gif](./img/animc.gif)  
  
example1  
![Image](./img/1.jpg)  
example2  
![Image](./img/2.jpg)  
example3  
![Image](./img/3.jpg)  
example4  
![Image](./img/4.jpg)  
vert:1183, face:2208, vertuv:1183, faceuv:2208,texture:64x32  

---

Blender 3Dファイルエクスポート設定 
```
objファイル
[+] Triangulate Faces
[-] Write Normals
[-] Write Materials
```
objファイルをスクリプトでhppに変換  
pngファイルをスクリプトでhppに変換  

---
## M5StickC
M5StickC以外ではテストしていませんが他のボードでも動くかもしれません  
- M5StickCPlus
- M5Stack
- M5Stack-Fire
- M5Stack-Core2

Arduino IDE設定  
```
ファイル/環境設定/追加のボードマネージャのURL
https://dl.espressif.com/dl/package_esp32_index.json

ツール/ボード/ボードマネージャ
esp32 by Espressif System インストール

ツール/ライブラリを管理
M5StickC インストール
// M5StickCPlus, M5Stack インストール
```

Arduino IDEで書き込み  

---
## console-app
.ino -> .cpp  

mingw, gcc  
windowsのコマンドプロンプトだとカラー表示できないためlinuxかwsl  

```
g++ -std=c++1z ***.cpp -o ***.out
```
---
## ncurses
.ino -> .cpp  

mingw, gcc  
ncursesインストール  
```
sudo apt-get install ncurses-dev
```

```
g++ -std=c++1z ***.cpp -o ***.out -lncurses
```
Escで終了  
Escで終了しないと色がおかしくなる  
color_content, pair_contentで色情報保存して最後に戻しているつもり  

#### キー入力  
bool keydown (char c)  

---
## windows-app
.ino -> .cpp  
windows mingw  
```
g++ -std=c++1z -static-libstdc++ -static -lstdc++ -mwindows ***.cpp -o ***.exe
```

#### キー入力  
bool keydown (char c)  

---
E512Array<uint8_t> numtostr (int v)  
E512Array<uint8_t> numtostr (float v, uint8_t n = 4)  
uint16_t color565 (uint16_t r, uint16_t g, uint16_t b)  


#### E512Array
uint32_t size ()  
uint32_t capacity ()  
void shrink_to_fit ()  
void reserve (uint32_t sz)  
void resize (uint32_t sz, T c = T())  
void emplace_back (T t)  
void push_back (T t)  
void pop_back (T t)  
void erase_index (int index)  
void erase_value (T t)  
void clear ()  
T& front ()  
T& back ()  

#### E512W3DWindowManager
E512W3DWindowManager ()  
void begin ()  
void add (E512W3DWindow& w)  
void draw ()  
void fixedDraw ()  
void fixedDrawWait ()  
bool isFixedTime ()  
void pushScreen ()  
uint16_t fixed_milli_time = 33  
void clear ()  
#### E512W3DWindow  
E512W3DWindow ()  
E512W3DWindow (uint16_t bgcolor)  
E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height)  
E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height, uint16_t bgcolor)  
E512W3DWindow (int16_t sx, int16_t sy, uint16_t width, uint16_t height, uint16_t bgcolor, Vector3 light)  
void resize (uint16_t width, uint16_t height)  
void setDirectionalLight (float x, float y, float z)  
void setDirectionalLight(Vector3(x, y, z))  
void addChild (Object3D& o)  
void setCamera (Object3D& o)  
int16_t sx = 0  
int16_t sy = 0  
uint16_t width  
uint16_t height  
uint16_t bgcolor  
float ambient = 0 // 0f - 1f  
float light_strength = 1.0f;  
bool isortho = false
float ortho_size = 0.1f
void draw ()  
void draw (Object3D& obj, bool child = false)  
void drawObjestAxis (Object3D& obj)  
void drawLine (Object3D& start, Object3D& end, uint16_t color = 0xFFFF)  
void drawLine (Vector3 start, Vector3 end, uint16_t color = 0xFFFF)  
void drawLine (uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color = 0xFFFF)  
void drawPoint (Object3D& obj, uint16_t size = 1, uint16_t color = 0xFFFF)  
void drawPoint (Vector3 p, uint16_t size = 1, uint16_t color = 0xFFFF)  
void drawPoint (int16_t px, int16_t py, uint16_t size = 1, uint16_t color = 0xFFFF)  
void begin (bool color_buffer_clear = true, bool z_buffer_clear = true)  
Vector3 screenPosition (Object3D& obj)  
Vector3 screenPosition (Vector3 p)  
  
uint16_t text_color;  
uint16_t text_bgcolor;  
bool text_use_bgcolor;  
void setTextCursor (int16_t x, int16_t y)  
void setTextSize (uint16_t n)  
void print (E512Array<uint8_t> v, bool wordwrap = true)  
void println (E512Array<uint8_t> v, bool wordwrap = true)  
void print (const char* cp, bool wordwrap = true)  
void println (const char* cp, bool wordwrap = true)  
void print (const uint8_t c, bool wordwrap = true)  
void println (const uint8_t c, bool wordwrap = true)  

  
#### Object3D  
Vector3 position  
Quaternion rotation  
Vector3 scale  
Mesh* mesh  
Object3D* parent  
uint16_t render_type = RenderType::WireFrame  
E512Array<Object3D*> child  
void setParent (Object3D& o)  
void addChild (Object3D& o)  
Matrix4x4 worldMatrix ()  
Vector3 worldPosition ()  
Vector3 forward ()  
Vector3 back ()  
Vector3 up ()  
Vector3 down ()  
Vector3 right ()  
Vector3 left ()  


    
#### Mesh
void addVertex (float x, float y, float z)  
void addVertex (Vector3 v)  
void addFace (uint16_t a, uint16_t b, uint16_t c)  
void addFace (Face f)  
void addVertexUV (float x, float y)  
void addVertexUV (Vector2 v)  
void addFaceUV (uint16_t a, uint16_t b, uint16_t c)  
void addFaceUV (Face f)  
#### Face
uint16_t a, b, c  

#### Quaternion
static Quaternion angleAxis (float w, float x, float y, float z)  
static Quaternion angleAxis (float w, Vector3 v)  

#### enum RenderType
WireFrame  
PolygonColor  
PolygonNormal  
PolygonTranslucent  
PolygonTexture  
PolygonTextureDoubleFace  
PolygonTexturePerspectiveCorrect  
PolygonTexturePerspectiveCorrectDoubleFace  
Hide  
None  


