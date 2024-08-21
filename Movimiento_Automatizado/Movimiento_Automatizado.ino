#include <Stepper.h>

// Definir el número de pasos por revolución del motor (ajusta según tu motor)
const int stepsPerRevolution = 2048;

// Crear una instancia de la clase Stepper
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

// Pines del joystick
const int joyXPin = A0;
const int joyYPin = A1;

// Valores de umbral para detectar movimiento
const int threshold = 50;

void setup() {
  // Establecer la velocidad del motor (ajusta según tus necesidades)
  myStepper.setSpeed(10); // Velocidad en RPM
  Serial.begin(9600);
}

void loop() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);

  // Mover el motor según la posición del joystick en el eje X
  if (joyXValue > (512 + threshold)) {
    // Mover en sentido horario
    myStepper.step(stepsPerRevolution / 100);
  } else if (joyXValue < (512 - threshold)) {
    // Mover en sentido antihorario
    myStepper.step(-stepsPerRevolution / 100);
  }

  // (Opcional) También podrías usar el eje Y para controlar otro motor o función
  // Aquí solo movemos el motor en función del eje X
}
