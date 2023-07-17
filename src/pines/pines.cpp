#include "Arduino.h"
#include "pines.h"

void pinesInicio(int pinMotorSalida, int pinMotorLectura) {
  pinMode(pinMotorSalida, OUTPUT);
  pinMode(pinMotorLectura, INPUT_PULLUP);
}

void escribirPin(int pinMotorSalida, int rpmSeteado){
  analogWrite(pinMotorSalida, rpmSeteado);
}