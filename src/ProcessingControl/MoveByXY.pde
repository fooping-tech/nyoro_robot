// Processingスケッチ: シリアル通信でx, y座標を送信してロボットを動かす
// ボーレート: 9600

import processing.serial.*;

Serial myPort;

// 送信する座標
int x = 250; // 初期値（中央）
int y = 250; // 初期値（中央）
boolean dragging = false;
String[] portList;
int selectedPort = -1;
boolean portSelected = false;

void setup() {
  size(500, 500);
  portList = Serial.list();
  background(255);
}

void draw() {
  background(255);
  if (!portSelected) {
    fill(0);
    textAlign(LEFT, TOP);
    text("シリアルポートを選択してください:", 20, 20);
    for (int i = 0; i < portList.length; i++) {
      if (i == selectedPort) fill(0, 100, 255); else fill(0);
      text(i + ": " + portList[i], 40, 50 + i * 30);
    }
    return;
  }
  // 範囲ガイド
  stroke(200);
  noFill();
  rect(0, 0, 500, 500);
  // ポイント
  fill(0, 100, 255);
  stroke(0);
  ellipse(x, y, 30, 30);
  // テキスト（変換後の値を表示）
  int dispX = int(map(constrain(x, 0, 500), 0, 500, 0, 180));
  int dispY = int(map(constrain(y, 0, 500), 0, 500, 0, 180));
  fill(0);
  textAlign(CENTER, CENTER);
  text("x: " + dispX + ", y: " + dispY, width/2, height - 20);
}

void mousePressed() {
  if (!portSelected) {
    // ポート選択
    for (int i = 0; i < portList.length; i++) {
      if (mouseY > 50 + i * 30 && mouseY < 50 + (i + 1) * 30) {
        selectedPort = i;
        myPort = new Serial(this, portList[selectedPort], 9600);
        portSelected = true;
        break;
      }
    }
    return;
  }
  // ポイントの上を押したらドラッグ開始
  float d = dist(mouseX, mouseY, x, y);
  if (d < 20) {
    dragging = true;
  }
}

void mouseDragged() {
  if (portSelected && dragging) {
    x = constrain(mouseX, 0, 500);
    y = constrain(mouseY, 0, 500);
    sendPosition();
  }
}

void mouseReleased() {
  dragging = false;
}

void sendPosition() {
  int sendX = int(map(constrain(x, 0, 500), 0, 500, 0, 180));
  int sendY = int(map(constrain(y, 0, 500), 0, 500, 0, 180));
  String xCmd = "set 1 " + sendX;
  String yCmd = "set 3 " + sendY;
  myPort.write(xCmd + "\n");
  delay(10);
  myPort.write(yCmd + "\n");
  println("送信: " + xCmd + ", " + yCmd);
}
