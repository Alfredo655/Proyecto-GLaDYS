#include <Servo.h>

// Declaración de los objetos Servo
Servo servoX;
Servo servoY;

// Pines del joystick
const int pinJoystickX = A0;
const int pinJoystickY = A1;

// Pines de los servos
const int pinServoX = 9;
const int pinServoY = 10;

// Pin del botón y láser
const int pinBoton = 7;
const int pinLaser = 6;

// Variables para los valores del joystick
int valorJoystickX;
int valorJoystickY;

// Variables para los servos (0-180 grados)
int posicionServoX;
int posicionServoY;

void setup() {
  // Iniciar la comunicación serial (opcional para depuración)
  Serial.begin(9600);
  
  // Adjuntar los servos a sus pines correspondientes
  servoX.attach(pinServoX);
  servoY.attach(pinServoY);
  
  // Configurar el pin del láser como salida
  pinMode(pinLaser, OUTPUT);
  
  // Configurar el pin del botón como entrada (con resistencia pull-up interna)
  pinMode(pinBoton, INPUT_PULLUP);
  
  // Posicionar los servos en el centro
  servoX.write(90);
  servoY.write(90);
  
  // Apagar el láser al inicio
  digitalWrite(pinLaser, LOW);
}

void loop() {
  // Leer los valores del joystick
  valorJoystickX = analogRead(pinJoystickX);
  valorJoystickY = analogRead(pinJoystickY);
  
  // Convertir los valores del joystick (0-1023) a ángulos de los servos (0-180)
  posicionServoX = map(valorJoystickX, 0, 1023, 0, 180);
  posicionServoY = map(valorJoystickY, 0, 1023, 0, 180);
  
  // Mover los servos a las posiciones correspondientes
  servoX.write(posicionServoX);
  servoY.write(posicionServoY);
  
  // Leer el estado del botón (LOW significa que está presionado)
  int estadoBoton = digitalRead(pinBoton);
  
  // Si el botón está presionado, encender el láser
  if (estadoBoton == LOW) {
    digitalWrite(pinLaser, HIGH);  // Encender láser
  } else {
    digitalWrite(pinLaser, LOW);   // Apagar láser
  }
  
  // (Opcional) Imprimir los valores leídos en el monitor serial
  Serial.print("Joystick X: ");
  Serial.print(valorJoystickX);
  Serial.print(" -> Servo X: ");
  Serial.println(posicionServoX);
  
  Serial.print("Joystick Y: ");
  Serial.print(valorJoystickY);
  Serial.print(" -> Servo Y: ");
  Serial.println(posicionServoY);
  
  Serial.print("Estado del botón: ");
  Serial.println(estadoBoton == LOW ? "Presionado" : "No presionado");
  
  // Pequeña demora para evitar lecturas demasiado rápidas
  delay(15);
}
