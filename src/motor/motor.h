// ========== DEFINICION DE PARAMETROS INICIALES ==========
#define LARGO_ARREGLO_VELOCIDADES 10
#define LARGO_ARREGLO_VELOCIDADES_REGRESION 12
// ========================================================


// ========== DECLARACION FUNCIONES PUBLICAS ==============
void motorInicio();
void motorActualizacion();

void menuInicio();
void menuActualizacion();
// ========================================================


// ========== DEFINICION DE ENUM ==========================

// ========================================================


// ========== DEFINICION DE CLASES ========================
class Motor {
private:
  int rpmSeteado;
  unsigned long rpmLeido;
  unsigned long ultimaLectura;
  unsigned long nuevaLectura;
  unsigned long velocidades[LARGO_ARREGLO_VELOCIDADES];

  int entradasRegresion[LARGO_ARREGLO_VELOCIDADES_REGRESION] = {35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90};
  unsigned long velocidadesRegresion[LARGO_ARREGLO_VELOCIDADES_REGRESION];
  float mRegresion;
  float bRegresion;

  void setearPinMotor();
  void procesarDatos();
  void calcularRegresion();
  void obtenerConstantesRegresion();
  void llenarArregloVelocidades();


public:
  Motor();
  bool setRpm();
  int getRpmSeteado();
  void getRpmLeido();
  void calcularRpm();
  void ajustarParametros();
};

// ========================================================