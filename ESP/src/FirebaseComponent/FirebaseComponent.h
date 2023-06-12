#ifndef FIREBASE_H
#define FIREBASE_H

#include <Firebase_ESP_Client.h>

//extern FirebaseAuth auth;
//extern FirebaseConfig config;

String initFirebase();
bool estadoUser();

extern FirebaseData fbdo;

#endif
