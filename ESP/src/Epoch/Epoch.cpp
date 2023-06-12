#include "epoch.h"

/*Configurando o servidor de timer*/
void epoch_init() {
  const char* ntpServer = "pool.ntp.org";
  configTime(0, 0, ntpServer);
}

/*Função de obtenção do tempo*/
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return now;
}