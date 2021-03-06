# UART_Monitor
  - M5Stackを使った、ラインモニタ（UART）です。
  - 機能概要
    - UART（データ2本、制御信号5本）信号をモニタできます。
    - モニタしたデータを閲覧できます。（1画面に収まらない場合は、ページを切り替えます。ページあたりのデータ数は、36データ。）
    - 下記のConfig値を設定できます。
      - ビットレート（bps）
        - 300／600／1200／2400／4800／9600／19200／38400／57600／115200
      - ビット長
        - 5bit／6bit／7bit／8bit
      - ストップビット
        - 1bit／2bit
      - パリティ
        - none（なし）／odd（奇数）／even（偶数）

# 必要なもの
  - M5Stack
  - Arduino IDE
    - バージョン1.8.5での動作を確認しています。

# 使用しているOSSライブラリ
  - M5Stack（ https://github.com/m5stack/M5Stack ）

# ビルド
  1. PCにM5StackをUSB接続します。
  1. Arduino IDE（別途ダウンロードしてください）より、M5Stack/AngleMeter/AngleMeter.inoを開きます。
  1. 同IDE上で、「マイコンボードに書き込む」を実行します。
      - あらかじめ、下記の設定を行ってください。
        1. ボードは、「M5Stack-Core-ESP32」を指定。
        1. シリアルポートは、M5StackをUSB接続して追加されるCOMポートを指定。
            - COMポートがない場合、ドライバ（ https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers ）のインストールが必要になります。

# ピン（GPIO）接続
  - 下表の通りに接続されることを想定しています。（ラインモニタなので、バッファ制御以外の信号は入力です）

    |項番|信号名|GPIO番号|DSUBコネクタ（9ピン）|
    |:-:|:-:|:-:|:-:|
    |1|DCD|0|1|
    |2|RxD|16|2|
    |3|TxD|17|3|
    |4|DTR|2|4|
    |5|DSR|5|6|
    |6|RTS|13|7|
    |7|CTS|15|8|
    |8|バッファ制御|23|LOWでEnable|
  - RS232Cレベル→3.3Vレベル（M5Stack）へのレベル変換回路が必要です。
    - くれぐれも、RS-232C信号をM5Stackに直接接続しないようにしてください。（当然ですが）

# 操作方法
  - 4つの状態があり、各状態でできる操作があります。
    - 停止状態
      - [START]ボタンで、モニタ状態となる。
      - [CONFIG]ボタンで、Config状態になる
    - モニタ状態
      - [STOP]ボタンで、モニタ停止
        - 取り込んだデータが1つもなければ、停止状態となる。
        - 1データでもあれば、閲覧状態となる。
      - [CHARACTOR]ボタンで、キャラクタ表示となる。
      - [BINARY]ボタンで、文字コード表示となる。
    - 閲覧状態
      - [CLEAR]ボタンで、取り込んだデータを消去して停止状態となる。
      - [REVERSE]ボタンで、ひとつ前のページを表示する。
      - [FORWARD]ボタンで、ひとつ次のページを表示する。
    - Config状態
      - [SELECT]ボタンで、Config種別（ビットレート／ビット長／ストップビット）の切換え。
      - [VALUE]ボタンで、Config値の切換え。
      - [CLOSE]ボタンで、停止状態となる。
        - Config値が確定で、画面上部の値表示に反映されます。
