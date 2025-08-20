# nyoro_robot

## 概要

nyoro_robotは、Arduinoプラットフォーム上で動作するロボット制御プログラムです。`NyoroRobot`クラスや`AtomMotion`クラスを用いて、ロボットの各種動作を制御します。

## ディレクトリ構成

```
src/
  NyoroRobot/
    AtomMotion.cpp
    AtomMotion.h
    NyoroRobot.ino
    SERIAL.ino
```

- `NyoroRobot.ino` : メインのArduinoスケッチ
- `SERIAL.ino` : シリアル通信関連の処理
- `AtomMotion.cpp` / `AtomMotion.h` : モーション制御クラス

## 必要な環境

- Arduino IDE または PlatformIO
- 対応するArduinoボード（例：M5Atomなど）

## ビルド・書き込み方法

1. Arduino IDEで`src/NyoroRobot/NyoroRobot.ino`を開きます。
2. ボードとポートを選択します。
3. 必要なライブラリをインストールします（例：M5Atom用ライブラリなど）。
4. 「書き込み」ボタンでボードに書き込みます。

## ライセンス

MIT License
