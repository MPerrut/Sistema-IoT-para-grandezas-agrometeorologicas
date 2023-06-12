#ifndef BH1750_H
#define BH1750_H

#include "FirebaseComponent/FirebaseComponent.h"

void initBH1750(String uid);
bool Request_BH1750();
void Resposta_BH1750();

extern FirebaseData fbdo;

#endif