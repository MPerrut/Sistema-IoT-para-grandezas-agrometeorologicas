#ifndef GPS_H
#define GPS_H

#include "FirebaseComponent/FirebaseComponent.h"

void initGPS(String uid);
bool Request_GPS();
void Resposta_GPS();
void Teste();

extern FirebaseData fbdo;

#endif