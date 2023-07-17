// ========== DEFINICION DE ENUM ==========================
typedef enum {
  IR_SENSOR_ESCANEANDO,
  IR_SENSOR_DEBOUNCE,
  IR_SENSOR_DETECTADO,
} irSensorEstado_t;
// ========================================================

void sensorInicio(int);
unsigned long sensorActualizacion();
void interrupcion();
void actualizarArreglo();
