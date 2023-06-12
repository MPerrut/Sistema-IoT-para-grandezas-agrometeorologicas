/*Includes de bibliotecas*/
/*Bibliotecas Arduino.h/ ESP32/ Wifi.h / Firebase/ Sensores*/
#include <Arduino.h>
#include "WiFi/WiFi.h"
#include "FirebaseComponent/FirebaseComponent.h"
#include "Epoch/Epoch.h"
#include "SHT75/sht75.h"
#include "BH1750/bh1750.h"
#include "GPS/gps.h"

/*Objetos Firebase*/
FirebaseData fbdo;
String guarda;

void setup(){
  /*Inicialização do Serial*/
  Serial.begin(115200);
  /*Inicialização do WiFi*/
  initWiFi();

  /*Inicialização do firebase*/
  String user = initFirebase();
  guarda = user;

  /*Inicialização dos sensores*/
  initSHT75(user);
  initBH1750(user);
  initGPS(user);

  /*Inicialização do Epoch*/
  epoch_init();

  /*Inicialização do Deep Sleep caso o usário não esteja ativo na aplicação Web*/
  if(!estadoUser()){
    /*Envio dos dados dos sensores*/
    Resposta_SHT75();
    Resposta_BH1750();
    Resposta_GPS();
    /*Configuração do Deep Sleep*/
    esp_sleep_enable_timer_wakeup( 5 * 58 * 1000000);
    /*Início do Deep Sleep*/
    esp_deep_sleep_start();
  } 
}
void loop(){
  /*Verificação de requisição dos sensores*/
  if(Request_SHT75()){
    Resposta_SHT75();
  }

  if(Request_BH1750()){
    Resposta_BH1750();
  }

  if(Request_GPS()){
    initGPS(guarda);
    Resposta_GPS();
  }

  /*Verificação de requisição do Deep Sleep, caso o usuário tenha saído da aplicação Web*/
  if(!estadoUser()){
    esp_sleep_enable_timer_wakeup(5 * 58 * 1000000);
    esp_deep_sleep_start();
  } 
}