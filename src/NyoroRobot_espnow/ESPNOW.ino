#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
esp_now_peer_info_t slave;
// MACアドレスを設定
uint8_t specificMacAddress[] = {0x90, 0x15, 0x06, 0xFD, 0x91, 0xE8};//受信機(jetson側)
//uint8_t specificMacAddress[] = {0x14, 0x2B, 0x2F, 0xA1, 0x70, 0x88};//送信機(jullie側)
#define ESP_MAC_WIFI_STA     0
#define ESPNOW_SEND_WAIT_TIME 20
int ESPNOW_mode =0;
int ESPNOW_level =0;
int ESPNOW_hue =0;
int ESPNOW_brightness=25;
int ESPNOW_speak=0;

int ESPNOW_CheckMode(){
  return ESPNOW_mode;
}

int ESPNOW_CheckLevel(){
//  DUMP(ESPNOW_duty);
  return ESPNOW_level;
}
int ESPNOW_CheckHue(){
//  DUMP(ESPNOW_hue);
  return ESPNOW_hue;
}
int ESPNOW_CheckBrightness(){
  return ESPNOW_brightness;
}

void ESPNOW_checkAddress(){
    
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);   // IDF5でもOKな呼び出し
    Serial.print("ESP-NOW MAC address: ");
    for (int i = 0; i < 6; i++) {
      if (i > 0) Serial.print(":");
      if (mac[i] < 16) Serial.print("0");  // 1桁の場合にゼロを追加
      Serial.print(mac[i], HEX);
    }
    Serial.println();
}
void ESPNOW_setup(){
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
      Serial.println("ESPNOW setup was completed.");
      //M5.Lcd.print("ESPNow Init Success\n");
    } else {
      Serial.println("ESPNow Init Failed");
      //M5.Lcd.print("ESPNow Init Failed\n");
      ESP.restart();
    }
 
  // 特定のMACアドレス（specificMacAddress）をpeerInfoにコピー
  memcpy(slave.peer_addr, specificMacAddress, sizeof(specificMacAddress));
  //for (int i = 0; i < 6; ++i) {
  //  slave.peer_addr[i] = (uint8_t)0xff;
  //}
    esp_err_t addStatus = esp_now_add_peer(&slave);
    if (addStatus == ESP_OK) {
      // Pair success
      Serial.println("ESPNOW Pair success");
    }
    // ESP-NOWコールバック登録
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
}

// 送信コールバック
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }

// 受信コールバック
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *data, int data_len) {
  const uint8_t* mac_addr = recv_info->src_addr;
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.printf("Last Packet Recv from: %s\n", macStr);
  //Serial.printf("Last Packet Recv Data(%d): ", data_len);

  bool flag =true;
  for(int i=0;i<5;i++){
    if(mac_addr[i] != specificMacAddress[i])flag = false;
  }
  if(flag){
    int mode=data[0];//mode 0 or 1(ESP_NOWのvalue有効)
    int value1=data[1];//mode
    int value2=data[2];//hue
    int value3=data[3];//bright
    int value4=data[4];//level
    int value5=data[5];//speak
  /*
    for ( int i = 0 ; i < data_len ; i++ ) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println("");
  */
  //mode=0のとき
    if(mode==1){
      ESPNOW_hue = value2;
      ESPNOW_level = value3;
      ESPNOW_brightness = value4;
      ESPNOW_speak = value5;
    }
  }
}
void send_data(int mode,int value1,int value2,int value3,int value4,int value5){
    uint8_t data[6] = {mode,value1,value2,value3,value4,value5};

    esp_err_t result = esp_now_send(slave.peer_addr, data, sizeof(data));

    delay(ESPNOW_SEND_WAIT_TIME);

}