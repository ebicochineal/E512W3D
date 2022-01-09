## 3d-software-renderer m5stickc/console-app/windows-app/ncurses  
- 複数のウィンドウ  
- ワイヤーフレーム  
- ポリゴン表示、テクスチャ  
- 法線カラー表示  
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

---
## M5StickC
```
objエクスポート Blender設定
[+] Triangulate Faces
[-] Write Normals
[-] Write Materials
obj形式のファイルをスクリプトでhppに変換  
Arduino IDEで書き込み  
```

---
## CUI
.ino -> .cpp  
windowsのコマンドプロンプトだとカラー表示できないためlinuxかwsl  
__has_includeを使っているのでC++17の方が良いかも  
```
g++ -std=c++1z ***.cpp -o ***.out
```
---
## TUI
.ino -> .cpp  
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


---
## GUI
.ino -> .cpp  
windows mingw  
```
g++ -std=c++1z -static-libstdc++ -mwindows ***.cpp -o ***.exe
```


---
---

#### E512Array
uint16_t size ()  
uint16_t capacity ()  
void shrink_to_fit ()  
void reserve (uint16_t sz)  
void resize (uint16_t sz, T c = T())  
void emplace_back (T t)  
void push_back (T t)  
void pop_back (T t)  
void erase_index (int index)  
void erase_value (T t)  
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
uint16_t fixed_milli_time = 33  
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
  
#### Object3D  
Vector3 position  
Vector3 rotation  
Vector3 scale  
Mesh* mesh  
Object3D* parent  
uint16_t render_type = RenderType::WireFrame  
E512Array<Object3D*> child  
void setParent (Object3D& o)  
void addChild (Object3D& o)  
    
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

#### enum RenderType
WireFrame  
PolygonColor  
PolygonNormal  
PolygonTexture  
Hide  
None  


