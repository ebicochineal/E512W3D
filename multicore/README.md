2つのコアを使っての描画を可能にしたバージョンです  
E512W3D.hppだけ差し替えて使用します  
mingwはスレッドモデルがposixの方のmingwを使用してください  
LoopがUIと別スレッドになっているので数秒処理が止まる場合はこちらを使うと落ちません  

| m5stickc | console | ncurses | windows | emscripten | android | pico |
| -        | -       | -       | -       | -          | -       | -    |
| yes      | no      | no      | yes     | no         | no      | yes  |

