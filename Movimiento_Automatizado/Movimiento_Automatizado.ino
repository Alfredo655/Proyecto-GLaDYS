#include <Servo.h>

Servo servoX;  // Servo para el eje X
Servo servoY;  // Servo para el eje Y

int posX = 90;  // Posiciones iniciales de los servos (centrado)
int posY = 90;

const int ledRed = 11;     // Pin del LED rojo (ninguna cara detectada)
const int ledYellow = 12;  // Pin del LED amarillo (cara detectada pero no centrada)
const int ledGreen = 13;   // Pin del LED verde (cara detectada y centrada)

const int threshold = 20;  // Umbral para considerar el rostro "centrado"

void setup() {
  // Inicializar los servos en los pines correspondientes
  servoX.attach(9);  // Cambia el pin si es necesario
  servoY.attach(10); // Cambia el pin si es necesario
  
  // Inicializar los pines de los LEDs
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
  // Asegurarse de que todos los LEDs están apagados al inicio
  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, LOW);

  // Inicializar comunicación serial
  Serial.begin(9600);
}

void loop() {
  // Si se han recibido datos por el puerto serial
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    // Comprobar si se ha recibido alguna cara
    if (data == "NO_FACE") {
      // No hay cara detectada: encender LED rojo y apagar los otros
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledGreen, LOW);
    } else {
      // Parsear las coordenadas recibidas
      int commaIndex = data.indexOf(',');
      if (commaIndex > 0) {
        int deltaX = data.substring(0, commaIndex).toInt();
        int deltaY = data.substring(commaIndex + 1).toInt();
        
        // Ajustar las posiciones de los servos en función del deltaX y deltaY
        posX = constrain(posX + deltaX / 20, 0, 180);  // Ajusta la sensibilidad
        posY = constrain(posY + deltaY / 20, 0, 180);

        // Mover los servos a las posiciones calculadas
        servoX.write(posX);
        servoY.write(posY);

        // Verificar si el rostro está lo suficientemente centrado
        if (abs(deltaX) < threshold && abs(deltaY) < threshold) {
          // Cara centrada: encender LED verde y apagar los otros
          digitalWrite(ledGreen, HIGH);
          digitalWrite(ledRed, LOW);
          digitalWrite(ledYellow, LOW);
        } else {
          // Cara detectada pero no centrada: encender LED amarillo y apagar los otros
          digitalWrite(ledYellow, HIGH);
          digitalWrite(ledRed, LOW);
          digitalWrite(ledGreen, LOW);
        }
      }
    }
  }
}
