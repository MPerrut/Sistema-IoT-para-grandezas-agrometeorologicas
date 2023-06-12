#ifndef SHT75_H
#define SHT75_H


#include "FirebaseComponent/FirebaseComponent.h"

void initSHT75(String uid);
bool Request_SHT75();
void Resposta_SHT75();

extern FirebaseData fbdo;

#endif
