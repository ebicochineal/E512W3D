### M5StickCで3Dオブジェクトを表示できるライブラリ  
- 複数のウィンドウ
- ワイヤーフレーム
- ポリゴン表示（Zソート法　Zバッファだけどポリゴン単位なので）
- 法線カラー表示
- Directional Light
- Ambient

#### todo
- オブジェクト　：　削除、検索
- ウィンドウ　：　削除、非表示、描画順変更
- カメラ　：　平行投影


----

#### E512Array
uint16_t size ()  
void resize (uint16_t s)  
void emplace_back (T t)  
void push_back (T t)  
void erase_index (int index)  
void erase_value (T t)  
T& front ()  
T& back ()  

#### E512WindowManager
E512WindowManager (uint16_t width, uint16_t height)  
add (E512W3D)  
reDraw ()  
fixedDraw ()  
#### E512W3D  
E512W3D (int16_t sx, int16_t sy, uint8_t width, uint8_t height)  
E512W3D (int16_t sx, int16_t sy, uint8_t width, uint8_t height,uint16_t bgcolor)  
E512W3D (int16_t sx, int16_t sy, uint8_t width, uint8_t height,uint16_t bgcolor, Vector3 ligtht)  
void resize (uint8_t width, uint8_t height)  
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
  
#### Object3D  
Vector3 position  
Vector3 rotation  
Vector3 scale  
Mesh* mesh  
Object3D* parent  
uint8_t render_type = RenderType::WireFrame  
E512Array<Object3D*> child  
void setParent (Object3D& o)  
void addChild (Object3D& o)  
    
#### Mesh
void addVertex (float x, float y, float z)  
void addVertex (Vector3 v)  
void addFace (uint16_t x, uint16_t y, uint16_t z)  
void addFace (Face f)  

#### Face
uint16_t x, y, z  

#### enum RenderType
WireFrame  
Polygon  
PolygonNormal  
Hide  


