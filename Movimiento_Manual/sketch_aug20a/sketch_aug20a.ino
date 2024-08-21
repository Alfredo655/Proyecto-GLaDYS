#include <Stepper.h>

// Definir el número de pasos por revolución del motor (ajusta según tu motor)
const int stepsPerRevolution = 2048;

// Crear una instancia de la clase Stepper
Stepper XStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper YStepper(stepsPerRevolution, 4, 5, 6, 7);
// Pines del joystick
const int joyXPin = A0;
const int joyYPin = A1;

// Valores de umbral para detectar movimiento
const int Umbral = 50;

void setup() {
  // Establecer la velocidad del motor (ajusta según tus necesidades)
  XStepper.setSpeed(10); // Velocidad en RPM
  YStepper.setSpeed(10); // Velocidad en RPM
  Serial.begin(9600);
}

void loop() {
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  
  // Movimiento del motor Y
  if (joyYValue > (512 + Umbral)){
    XStepper.step(stepsPerRevolution / 100);
  }
  else if (joyYValue < (512 + Umbral)){
    YStepper.step(-stepsPerRevolution / 100);
  }
  // Movimiento del motor X
  if (joyXValue > (512 + Umbral)) {
    XStepper.step(stepsPerRevolution / 100);
  }
  else if (joyXValue < (512 - Umbral)) {
    XStepper.step(-stepsPerRevolution / 100);
  }

}
