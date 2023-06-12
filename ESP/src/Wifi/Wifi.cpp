#include "WiFi.h"

//Função de inicialização do WiFi
void initWiFi() {
  WiFi.begin("SSD", "SENHA");
  int timeout_counter = 0;
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    timeout_counter++;
    if(timeout_counter >= 10){
    ESP.restart();
    }
  }
  Serial.println(WiFi.localIP());
  delay(1000);
}