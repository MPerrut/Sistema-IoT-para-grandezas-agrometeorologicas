#include "FirebaseComponent.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include "WiFi.h"

//Objetos Firebase
FirebaseAuth auth;
FirebaseConfig config;
String uid;
String estadoPath;

//Função de inicialização da conexão com o Firebase
String initFirebase() {
  //Inicialização da conexão com o Firebase
  config.api_key = "chave da API";
  auth.user.email = "email do usuário";
  auth.user.password = "senha do usuário";
  config.database_url = "url do banco de dados";

  //Configuração do WiFi
  Firebase.reconnectWiFi(true);

  //Configuração do token
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);

  //Verificação da conexão com o Firebase
  while ((auth.token.uid) == "") {
    delay(1000);
  }

  //Retorno do UID do usuário
  uid = auth.token.uid.c_str();
  return uid;
}

//Função de verificação do estado do usuário
bool estadoUser(){
  estadoPath = "/UsersData/" + String(uid) + "/Estado/ativo";
  Firebase.RTDB.getBool(&fbdo, estadoPath);
  return fbdo.boolData();
}