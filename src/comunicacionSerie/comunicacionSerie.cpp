#include "Arduino.h"
#include "comunicacionSerie.h"

#define MENSAJE_INICIO_SISTEMA "Bienvenido al sistema de control de velocidad\r\n"


void comunicacionSerieInicio() {
  Serial.begin(9600);
}

int leerComunicacionSerie() {
  return Serial.read();
}

char leerComunicacionSerieChar(){
  return char(Serial.read());
}

int enviarComunicacionSerie(unsigned long rpmLeido) {
  Serial.write(int(rpmLeido/10));
}

void enviarMensaje(char *mensaje) {
  Serial.print(mensaje);
}

