
### コンソール、ウィンドウアプリとして動かす

#### 追加する
M5StickC.h  

#### inoファイルの書き換え
***.ino -> ***.cpp  
```cpp
.
.
.
VMAINLOOP(wm)
```

![Gif](../img/animc.gif) 

----
#### コンソールアプリ
```
g++ -std=c++11 ***.cpp -o ***.out
```
windowsのコマンドプロンプトでも動くけどカラー表示できないためwsl用  


#### ウィンドウアプリ
MinGW  
```
g++ -std=c++11 -static-libstdc++ -mwindows ***.cpp -o ***.exe
```


