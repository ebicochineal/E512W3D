
### コンソールアプリとして動かす

#### 追加する
M5StickC.h  

#### inoファイルの書き換え
.ino -> .cpp  
```cpp
.
.
.
int main () {
    setup();
    while (1) {
        loop();
        delay(100);
    }
}
```

![Gif](../img/animc.gif) 

----
 
windowsのコマンドプロンプトでも動くけどカラー表示できないためwsl用  


