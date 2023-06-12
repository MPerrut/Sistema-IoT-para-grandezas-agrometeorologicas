#include "bh1750.h"
#include "Epoch/Epoch.h"
#include <Wire.h>
#include <BH1750.h>

/*Variaveis globais BH1750*/
String BHT1750_Path;
String BH1750_LuxPath = "/Lux";
String BH1750_timePath = "/timestamp";
String Request_BH1750_Path;
String parentPathBH1750;

/*Objetos BH1750*/
BH1750 lightMeter;
TwoWire I2CBH1750 = TwoWire(0);

/*Objetos Firebase*/
FirebaseJson jsonBH;

/*Variaveis auxiliares*/
int timestampBH1750;

/*Função de inicialização do BH1750*/
void initBH1750(String uid){

  const int I2C_SDA = 18;
  const int I2C_SCL = 19;
    
  I2CBH1750.begin(I2C_SDA, I2C_SCL);
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &I2CBH1750);

  BHT1750_Path = "/UsersData/" + uid + "/BHT1750";
  Request_BH1750_Path = "/UsersData/" + uid + "/Request/BH1750_request/BH1750_request";
   
}

/*Função de requisição do BH1750*/
bool Request_BH1750(){
  Firebase .RTDB.getBool(&fbdo, Request_BH1750_Path);
  bool aux = fbdo.boolData();
  Firebase.RTDB.setBool(&fbdo, Request_BH1750_Path, false);
  return aux;
}

/*Função de resposta do BH1750*/
void Resposta_BH1750(){
  timestampBH1750 = getTime();
  parentPathBH1750= BHT1750_Path + "/" + String(timestampBH1750);

  jsonBH.set(BH1750_LuxPath.c_str(), String(lightMeter.readLightLevel()));
  jsonBH.set(BH1750_timePath.c_str(), String(timestampBH1750));
  Serial.printf("envia json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPathBH1750.c_str(), &jsonBH) ? "ok" : fbdo.errorReason().c_str());
}