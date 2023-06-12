#include "gps.h"
#include <TinyGPSPlus.h>
#include "SoftwareSerial.h"

// Variavéis GPS
String GPS_Path;
String GPS_latPath = "/LAT";
String GPS_lngPath = "/LNG";
String GPS_timePath = "/Date";
String Request_GPS_Path;
TinyGPSPlus gps;
static const int RXPin = 16, TXPin =17;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);

// Variavéis Firebase
FirebaseJson jsonGPS;
String parentPath;

/*Função de inicialização do GPS*/
void initGPS(String uid){
  ss.begin(GPSBaud);
  GPS_Path = "/UsersData/" + uid + "/GPS";
  Request_GPS_Path = "/UsersData/" + uid + "/Request/GPS_request/Requisito";
}

/*Função de requisição do GPS*/
bool Request_GPS(){
  Firebase .RTDB.getBool(&fbdo, Request_GPS_Path);
  bool aux = fbdo.boolData();
  Firebase.RTDB.setBool(&fbdo, Request_GPS_Path, false);
  return aux;
}

int controle = 0;

/*Função de resposta do GPS*/
void Resposta_GPS() {

  parentPath= GPS_Path + "/GPS_Data";

  Serial.println(ss.read());

   while (ss.available() > 0){
    gps.encode(ss.read());
    if (true){
      Serial.println("Latitude= ");
      Serial.println(gps.location.lat(), 6);
      Serial.println(" Longitude= ");
      Serial.println(gps.location.lng(), 6);

      jsonGPS.set(GPS_timePath.c_str(), String(gps.date.value()) + " " + String(gps.time.value())),
      jsonGPS.set(GPS_latPath.c_str(), String(gps.location.lat(), 6));
      jsonGPS.set(GPS_lngPath.c_str(), String(gps.location.lng(), 6));
      Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &jsonGPS) ? "ok" : fbdo.errorReason().c_str());
      
    }
    controle++;
    if(controle > 20){
        break;
    }
  }

  Serial.println("Envia GPS");
}

