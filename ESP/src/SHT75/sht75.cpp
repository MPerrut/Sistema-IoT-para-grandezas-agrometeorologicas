#include "SHT75.h"
#include "Epoch/Epoch.h"
#include <SHT1x-ESP.h>

/*SHT Variáveis*/
String SHT_Path;
String SHT_tempcPath = "/TemperaturaC";
String SHT_tempfPath = "/TemperaturaF";
String SHT_humidityPath = "/umidade";
String SHT_timePath = "/timestamp";
String Request_SHT_Path;
String parentePathSHT;

FirebaseJson jsonSHT;

int timestampSHT;

/*Defenindo pinos do SHT*/
#define dataPin  21
#define clockPin 22
SHT1x sht1x(dataPin, clockPin, SHT1x::Voltage::DC_3_3v);

/*Função de inicialização do SHT*/
void initSHT75(String uid){
  SHT_Path = "/UsersData/" + String(uid) + "/SHT";
  Request_SHT_Path = "/UsersData/" + String(uid) + "/Request/SHT_request/SHT_request";
}

/*Função de requisição do SHT*/
bool Request_SHT75(){
  Firebase.RTDB.getBool(&fbdo, Request_SHT_Path);
  bool aux = fbdo.boolData();
  Firebase.RTDB.setBool(&fbdo, Request_SHT_Path, false);
  return aux;
}

/*Função de resposta do SHT*/
void Resposta_SHT75(){
  timestampSHT = getTime();
  parentePathSHT= SHT_Path + "/" + String(timestampSHT);

  jsonSHT.set(SHT_tempcPath.c_str(), String(sht1x.readTemperatureC()));
  jsonSHT.set(SHT_tempfPath.c_str(), String(sht1x.readTemperatureF()));
  jsonSHT.set(SHT_humidityPath.c_str(), String(sht1x.readHumidity()));
  jsonSHT.set(SHT_timePath.c_str(), String(timestampSHT));
  Serial.printf("Envia json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentePathSHT.c_str(), &jsonSHT) ? "ok" : fbdo.errorReason().c_str());
}
