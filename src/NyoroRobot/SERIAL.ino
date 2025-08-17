int SERIAL_values[4][2] = {
  {0, 0},  // id 0: angle, yobi
  {0, 0},  // id 1
  {0, 0},  // id 2
  {0, 0}   // id 3
};

// idに基づいてangleを取得する関数
int SERIAL_checkAngle(int id) {
  if (id >= 0 && id < 4) {
    return SERIAL_values[id][0];
  }
  return -1;  // 無効なidの場合
}

// idに基づいてyobiを取得する関数
int SERIAL_checkYobi(int id) {
  if (id >= 0 && id < 4) {
    return SERIAL_values[id][1];
  }
  return -1;  // 無効なidの場合
}

void SERIAL_beign() {
  //SERIAL INITIAL
  Serial.begin(9600);  //Serial begin
  delay(50);             //delay 50ms.  延迟50ms
  Serial.println("SERIAL setup was completed.");
}


/**
 * 指定された文字列から指定された位置の単語を取得する関数。
 *
 * @param inputString 取得対象の文字列
 * @param wordIndex 取得する単語の位置
 * @return 指定された位置の単語。見つからない場合は空の文字列を返します。
 */
String SERIAL_getWordFromIndex(String inputString, int wordIndex) {
  // 単語の個数をカウントする変数
  int wordCount = 0;
  // 単語の開始位置
  int wordStartIndex = 0;
  // 単語の終了位置
  int wordEndIndex = -1;
  // 文字列の最大インデックス
  int maxIndex = inputString.length() - 1;

  // 文字列をスキャンして単語を探索
  for (int i = 0; i <= maxIndex && wordCount <= wordIndex; i++) {
    // 現在の文字がスペースか、最後の文字である場合、単語が見つかったと判断
    if (inputString.charAt(i) == ' ' || i == maxIndex) {
      // 単語の個数を増加させる
      wordCount++;
      // 単語の開始位置を設定
      wordStartIndex = wordEndIndex + 1;

      // 単語の終了位置を設定。最後の文字の場合は終了位置を文字列の末尾に設定
      if (i == maxIndex) {
        wordEndIndex = i + 1;
      } else {
        wordEndIndex = i;
      }
    }
  }

  // 指定された位置の単語を返す。見つからない場合は空の文字列を返す
  if (wordCount > wordIndex) {
    return inputString.substring(wordStartIndex, wordEndIndex);
  } else {
    return "";
  }
}


void SERIAL_InputCheck() {
  //シリアルが利用可能か確認
  if (Serial.available()) {
    // シリアル読み取り
    String input = Serial.readStringUntil('\n');
    // 大文字へ
    input.toUpperCase();
    //コマンドを取得
    String command = SERIAL_getWordFromIndex(input, 0);

    if (command == F("MODE")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt();//mode_number 0:stop,1:SerialControlMode
      //value2 = SERIAL_getWordFromIndex(input, 2);
      //value3 = SERIAL_getWordFromIndex(input, 3);
      //value4 = SERIAL_getWordFromIndex(input, 4);
      switch(value1){
          case 0:
            break;
          case 1:
            break;
      }
      Serial.println("Mode_OK");
    }

    if (command == F("SET")) {
      int value1 = SERIAL_getWordFromIndex(input, 1).toInt(); // id
      int value2 = SERIAL_getWordFromIndex(input, 2).toInt(); // angle

      if (1 <= value1 && value1 <= 4) {
        if (value2 > 180) value2 = 180;
        if (value2 < 0) value2 = 0;

        SERIAL_values[value1 - 1][0] = value2;  // angleを設定
        Serial.println("Set_OK");
      } else {
        Serial.println("無効なIDです");
      }
    }


  } else {
      //Serial通信切断時の処理
  }
}