# Controlador de velocidad

## Introducción General

Las funciones del sistema se pueden separar en dos principales tareas. La primera es la de calibrar el sistema, es decir obtener los parámetros necesarios para relacionar la señal de entrada con la tensión de salida. La segunda es la de permitir al usuario ingresar un valor deseado y transformarlo en una tensión que alimente al motor.

El funcionamiento del sistema después de la calibración se puede simplificar en la siguiente figura

![](https://github.com/jrania/Proyecto_Final/blob/main/imagenes/3.2%20Diagrama%20de%20flujo%20basico.png)


## Introducción al código

El programa principal corre en el programa proyecto.ino. Este incluye el módulo motor y llama a dos funciones. La funcion motorInicio() se utiliza una sola vez, mientras que motorActualizacion() se llama en loop hasta que se desconecte el programa.

Una vez hecho esto se presenta la función motorInicio(). Esta realiza distintas tareas previas al funcionamiento estable del programa. La función comunicacionSerieInicio(), importada del módulo comunicacionSerie, inicia la comunicación serie y configura el baudrate a 9600. Esto permite que el Arduino se comunique con el ESP8266, reciba la velocidad deseada y le envíe la velocidad leída.

 pinesInicio() se encuentra en el módulo pines y configura dos pines de entrada y salida. El primero es la entrada del sensor infrarrojo, que enviará una señal cada vez que el motor curse el sensor. El otro es el que se conectará al motor y será necesario que sea un pin analógico, para poder cambiar el duty cycle del PWM asociado. La siguiente función, displayInit() se encuentra en el módulo display y realiza el protocolo necesario para iniciar el display.
 
Por último llama a la función parametrosInicio() que invoca a ajustarParametros() y que luego llama a Motor::calcularRegresion(). Esta función corresponde a un método de la clase Motor.

### Calibracion

La función velocidadesRegresion() llena el arreglo velocidadesRegresion[]. Para eso realiza un barrido de los valores guardados en entradasRegresion[] y se los envía al pin conectado al motor llamando a la función setearPinMotor(). Luego, durante 10 segundos toma los valores de la entrada del sensor con la funcion motor.calcularRpm(); y guarda el valor de velocidad calculado en la rpmLeido.
Luego de que void Motor::calcularRegresion() llame a llenarArregloVelocidades() se utiliza la función obtenerConstantesRegresion(). Su objetivo es leer los valores de velocidadesRegresion[] y obtener los valores de los atributos mRegresion y bRegresion de la clase Motor.
Estos valores son los que relacionan el valor de PWM que se le ingresa al pin conectado al motor y la velocidad en RPM del motor. Este es el fin de la calibración.


### Después de la calibración
Una vez obtenidos los atributos mRegresion y bRegresion, se llama a la función motorActualizacion(), la cual se ejecuta cada 500microsegundos. Esta función tiene tres subfunciones:

Al llamar a setRpm se revisa el buffer de entrada de la comunicación serie entre el Arduino y el ESP8266. Los valores se envían luego de dividirse por 10, poor lo que es necesario reescalarlos. Una vez hechco, se lo transforma linealmente con los valores de mRegresion y bRegresion calculados.

