#include <AccelStepper.h>

// Definir los pines para el motor paso a paso
#define STEP_PIN 2
#define DIR_PIN 3

// Crear una instancia de AccelStepper para el motor paso a paso
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

int posX = 90; // Posición inicial del motor paso a paso
int stepSpeed = 100; // Velocidad de paso del motor

void setup() {
  stepper.setMaxSpeed(1000); // Ajusta la velocidad máxima
  stepper.setAcceleration(500); // Ajusta la aceleración
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int commaIndex = data.indexOf(',');

    if (commaIndex > 0) {
      int cx = data.substring(0, commaIndex).toInt();
      int cy = data.substring(commaIndex + 1).toInt();

      // Mapeo de coordenadas de la cámara a los pasos del motor
      // Ajusta el rango de mapeo según la resolución y el rango del motor
      int targetPosX = map(cx, 0, 640, 0, 200); // Ajusta 640 y 200 según tu configuración

      // Mover el motor paso a paso a la posición calculada
      stepper.moveTo(targetPosX);
      stepper.run();
    }
  }
}
