#include "Arduino.h"
#include "sensor.h"
#include "../comunicacionSerie/comunicacionSerie.h"

// ========== DEFINICION TIEMPOS ==========================
#define INCREMENTO_TIEMPO_MICROS 500
#define TIEMPO_DEBOUNCE_SENSOR 1500
// ========================================================

int pinInterrupcionSensor;
irSensorEstado_t irSensorEstado = IR_SENSOR_ESCANEANDO;
unsigned long lecturaTiempo;
bool nuevaLectura;

void sensorInicio(int pinInterrupcion) {
  attachInterrupt(digitalPinToInterrupt(pinInterrupcion), interrupcion, RISING);
  pinInterrupcionSensor = pinInterrupcion;
}

unsigned long sensorActualizacion() {
  static int tiempoDebounceAcumuladoSensorIr;
  static int incrementoTiempo_ms = TIEMPO_DEBOUNCE_SENSOR;

  lecturaTiempo = 0;
  switch (irSensorEstado) {
    case IR_SENSOR_ESCANEANDO:
      break;

    case IR_SENSOR_DEBOUNCE:
      if (tiempoDebounceAcumuladoSensorIr >= TIEMPO_DEBOUNCE_SENSOR) {
        if (digitalRead(pinInterrupcionSensor) == true) {
          irSensorEstado = IR_SENSOR_DETECTADO;
          lecturaTiempo = micros();
        } else {
          irSensorEstado = IR_SENSOR_ESCANEANDO;
        }
      }
      tiempoDebounceAcumuladoSensorIr = tiempoDebounceAcumuladoSensorIr + incrementoTiempo_ms;
      break;

    case IR_SENSOR_DETECTADO:
      if (digitalRead(pinInterrupcionSensor) == false) {
        irSensorEstado = IR_SENSOR_ESCANEANDO;
      }
      break;
  }
  return lecturaTiempo;
}


void interrupcion() {
    irSensorEstado = IR_SENSOR_DEBOUNCE;
}