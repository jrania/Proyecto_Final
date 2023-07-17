// ========== LIBRERIAS ===================================
#include "Arduino.h"
#include "motor.h"
#include "../comunicacionSerie/comunicacionSerie.h"
#include "../pines/pines.h"
#include "../sensor/sensor.h"
#include "../regresion/regresion.h"
#include "../display/display.h"
// ========================================================


// ========== DEFINICION DE PINES =========================
#define PIN_MOTOR_SALIDA 9
#define PIN_MOTOR_LECTURA 2
// ========================================================


// ========== DEFINICION DE PARAMETROS INICIALES ==========
#define RPM_INICIO 0

#define INICIO_REGRESION 35
#define SALTO_REGRESION 5
#define FIN_REGRESION 90

// ========================================================


// ========== DEFINICION MENSAJES =========================
#define MENSAJE_INICIO_CALCULO_REGRESION "Calculando regresion"
#define MENSAJE_ESPERA_CALCULO_REGRESION ". "
#define MENSAJE_FIN_CALCULO_REGRESION "Regresion calculada"

#define MENSAJE_GET_RPM_LEIDO "La velocidad actual es: "
#define MENSAJE_NUEVA_LINEA "\r\n"
// ========================================================


// ========== DECLARACION DE FUNCIONES PRIVADAS ===========
void rpmSeteadoActualizacion();
void leerSensor();
void parametrosInicio();
// ========================================================


// ========== DECLARACION DE VARIABLES PUBLICAS ===========
Motor motor;
char mensaje[10];
// ========================================================


// ========== DEFINICION DE FUNCIONES PUBLICAS ============
void motorInicio() {
  comunicacionSerieInicio();
  pinesInicio(PIN_MOTOR_SALIDA, PIN_MOTOR_LECTURA);
  sensorInicio(PIN_MOTOR_LECTURA);
  displayInit(DISPLAY_CONNECTION_GPIO_4BITS);
  delay(1500);
  parametrosInicio();
  delay(1000);
  displayCharPositionWrite(0, 0);
  displayStringWrite("RPM: ");
}


void motorActualizacion() {
  static int i = 0;
  motor.setRpm();
  motor.calcularRpm();
  delayMicroseconds(500);
  if (i > 1000) {
       motor.getRpmLeido();
    i = 0;
  }
  i = i + 1;
}

void menuInicio() {
  enviarMensaje("Presione 'r' para obtener la velocidad medida\r\n");
  enviarMensaje("Presione 's' para modificar la velocidad\r\n");
  enviarMensaje("Presione 't' para recalibrar el sistema\r\n");
}

void menuActualizacion() {
  int opcion = 'a';
  if (Serial.available()) {
    opcion = leerComunicacionSerie();
  }

  switch (opcion) {
    case 'r':
      motor.getRpmLeido();
      menuInicio();
      break;
    case 't':
      motor.ajustarParametros();
      menuInicio();
      break;
    case 'a':
      break;
    default:
      motor.setRpm();
      break;
  }
}


Motor::Motor() {
  rpmSeteado = RPM_INICIO;
  setearPinMotor();
}

bool Motor::setRpm() {
  bool lectura = true;

  if (Serial.available()) {
    rpmSeteado = Serial.read();
    rpmSeteado = (rpmSeteado * 10 - bRegresion) / mRegresion;
    setearPinMotor();
    lectura = true;
  } else {
    lectura = false;
  }
  return lectura;
}

void Motor::getRpmLeido() {
  Serial.write(rpmLeido/10);
//  enviarComunicacionSerie(rpmLeido);
}

void Motor::ajustarParametros() {
  displayCharPositionWrite(0, 0);
  displayStringWrite("Calibrando");
  calcularRegresion();
}


// ========================================================


// ========== DEFINICION DE FUNCIONES PRIVADAS ============

void Motor::setearPinMotor() {
  escribirPin(PIN_MOTOR_SALIDA, rpmSeteado);
}

void Motor::calcularRpm() {
  static int i;

  nuevaLectura = sensorActualizacion();
  if (nuevaLectura) {
    procesarDatos();
    i = 0;
  }
  if(i>1000){
    rpmLeido = 0;
    i = 0;
  }
  i = i + 1;
}

void parametrosInicio() {
  motor.ajustarParametros();
}

//En esta funcion hay que trabajar
void Motor::procesarDatos() {
  unsigned long velocidadTemp;
  unsigned long rpmLeidoTemp = 0;
  int i;

  velocidadTemp = 6e7 / (nuevaLectura - ultimaLectura);
  ultimaLectura = nuevaLectura;

  for (i = 0; i < LARGO_ARREGLO_VELOCIDADES - 1; i++) {
    velocidades[i] = velocidades[i + 1];
    rpmLeidoTemp = rpmLeidoTemp + velocidades[i];
  }
  velocidades[LARGO_ARREGLO_VELOCIDADES - 1] = velocidadTemp;
  rpmLeido = (rpmLeidoTemp + velocidadTemp) / LARGO_ARREGLO_VELOCIDADES;
}

void Motor::calcularRegresion() {
  llenarArregloVelocidades();
  obtenerConstantesRegresion();
}

void Motor::obtenerConstantesRegresion() {
  constantesRegresion(&mRegresion, &bRegresion, entradasRegresion, sizeof(entradasRegresion) / sizeof(int), velocidadesRegresion,
                      sizeof(velocidadesRegresion) / sizeof(unsigned long));
}

void Motor::llenarArregloVelocidades() {
  int i, j;

  for (i = 0; i < LARGO_ARREGLO_VELOCIDADES_REGRESION; i++) {
    rpmSeteado = entradasRegresion[i];
    setearPinMotor();
    for (j = 0; j < 20000; j++) {
      motor.calcularRpm();
      delayMicroseconds(500);
    }
    sprintf(mensaje, "%d", i);
    displayCharPositionWrite(11, 0);
    displayStringWrite(mensaje);
    velocidadesRegresion[i] = rpmLeido;
  }
}
// ========================================================
