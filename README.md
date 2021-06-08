# **M5Stack**
M5Stackについてまとめたパッケージ
M5Stackの入門書である[みんなのM5Stack入門](https://www.switch-science.com/catalog/5933/)と一緒に進めると理解しやすい．

入門書は，フレイザー図書館にあります．  
本パッケージの[pdfフォルダ](pdf)には，入門書の補足資料有り．  

## **M5Stack setup**
1. **ArduinoIDEのインストール**  
    1. [ArduinoIDE](https://www.arduino.cc/en/main/software)から最新版をダウンロード
    2. ファイルを解凍
        ```bash
        $   cd Downloads
        $   tar -xJf arduino-○.○.○-linux64.tar.xz
        ```
    3. インストール
        ```bash
        $   cd arduino-○.○.○
        $   sudo install.sh
        ```  
        ※古いバージョンが既に入っている場合は，アンインストール([詳しくはこちら](https://asukiaaa.blogspot.com/2016/11/ubuntuarduino-ide.html))

2. **espressifとM5stackライブラリのインストール**    
[リンク](https://raspberrypi.mongonta.com/howto-start-m5stack-arduinoide/#ArduinoIDE%E3%81%AE%E8%A8%AD%E5%AE%9A)のArduinoIDEの設定の①〜⑤を行う

3. **デバイスファイルの設定**  
PCとM5StackをUSBで接続すると、/dev/ttyUSB*のバイスファイルが生成される．デバイスファイルに書き込み権限をつける．  
    M5Stackを接続し、シリアルポート名を確認
    ```bash
    $   ls /dev/ttyUSB*
    ```
    ttyACM0の場合
    ```bash
    $   sudo chmod 666 /dev/ttyUSB0
    ```

4. **動作確認**    
[リンク](https://raspberrypi.mongonta.com/howto-start-m5stack-arduinoide/#HelloWorld%E3%82%92%E5%AE%9F%E8%A1%8C%E3%81%99%E3%82%8B%E3%80%82)のHelloWorldを実行する。を行う．