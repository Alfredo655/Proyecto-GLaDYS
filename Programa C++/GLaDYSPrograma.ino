#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Servo.h>

// -------- Pines y Definiciones -------- //
// LEDs
#define ledR 2
#define ledM 3
#define ledV 4

// Botón de cambio de modo
#define BotonC 6

// Botón de disparo
#define BotonD 5
#define Rele 1
// Joystick
int joyX = A0;
int joyY = A1;

// Servos
Servo servoX, servoY, servoDis;

// Audio
SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// -------- Variables Globales -------- //
int estado = 1;  // 1 = Traqueo facial, 0 = Control manual
int B_estado = 0, B = 0;
int posX = 90, posY = 90, posS = 90;  // Posiciones iniciales de los servos
unsigned long previousMillis = 0;
const unsigned long interval = 10000;
int Disparador = 1;
int cooldown = 0;
int deteccion = 1;
int centrado = 1;
int torroto = 3;
int sentinela = 3;
void setup() {


  // Serial
  Serial.begin(4800);
  mySoftwareSerial.begin(9600);

  // LEDs
  pinMode(ledR, OUTPUT);
  pinMode(ledM, OUTPUT);
  pinMode(ledV, OUTPUT);

  //Botón Disparo
  pinMode(BotonD, INPUT);
  // Botón de cambio de modo
  pinMode(BotonC, INPUT);
  pinMode(Rele, OUTPUT);
  digitalWrite(Rele, HIGH);
  // Servos
  servoDis.attach(7);
  servoX.attach(9);
  servoY.attach(8);
  servoDis.write(90);
  servoX.write(90);
  servoY.write(100);

  // Audio
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true)
      ;
  }
  myDFPlayer.volume(30);  // Volumen entre 0 y 30
  myDFPlayer.play(1);
}


// -------- Bucle Principal -------- //
void loop() {
  // Leer el estado del botón
  B_estado = digitalRead(BotonC);
  if (B_estado == HIGH && B == LOW) {
    estado = !estado;  // Alternar entre modos
    if (torroto == 0 && estado == 0) {
      int randomTrack = random(26, 33);  // Seleccionar archivo aleatorio
      myDFPlayer.play(randomTrack);
      delay(2500);
      torroto = 0;
    } else if (sentinela == 0 && estado == 1) {
      int randomTrack = random(4, 14);  // Seleccionar archivo aleatorio
      myDFPlayer.play(randomTrack);
      delay(2500);
      sentinela = 0;
    }
  }
  B = B_estado;

  // Modo según el estado
  if (estado == 0) {
    traqueoFacial();
    torroto = 1;
    sentinela = 0;
  } else {
    controlManual();
    sentinela = 1;
    torroto = 0;
  }
}

void traqueoFacial() {

  unsigned long currentMillis = millis();
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');

    // Manejo de LEDs
    if (command.endsWith("R")) {
      if (currentMillis - previousMillis >= 10000) {
        previousMillis = currentMillis;
        int randomTrack = random(76, 84);  // Seleccionar archivo aleatorio
        myDFPlayer.play(randomTrack);
      }
      digitalWrite(ledR, HIGH);
      digitalWrite(ledM, LOW);
      digitalWrite(ledV, LOW);
      deteccion = 1;
      centrado = 0;
      cooldown = 0;
    } else if (command.endsWith("Y")) {
      if (deteccion == 1) {
        int randomTrack = random(33, 42);  // Seleccionar archivo aleatorio
        myDFPlayer.play(randomTrack);
        deteccion = 0;
        centrado = 0;
        cooldown = 0;
      }
      digitalWrite(ledR, LOW);
      digitalWrite(ledM, HIGH);
      digitalWrite(ledV, LOW);
    } else if (command.endsWith("G")) {
      digitalWrite(ledR, LOW);
      digitalWrite(ledM, LOW);
      digitalWrite(ledV, HIGH);
      centrado = 1;
      if (currentMillis - previousMillis > 1000 && centrado == 1) {
        previousMillis = currentMillis;
        cooldown += 1;
        if (cooldown >= 5) {
          int randomTrack = random(65, 67);  // Seleccionar archivo aleatorio
          servoDis.write(45);
          if(deteccion == 1){
          myDFPlayer.play(randomTrack);
          }
          delay(500);
          servoDis.write(120);
          deteccion = 0;
        } else if (currentMillis - previousMillis > 1000 && centrado == 1 && cooldown > 5) {
          servoDis.write(45);
          delay(500);
          servoDis.write(120);
        }
      }
    }

    // Control de servos
    int deltaX = command.substring(0, 2).toInt();
    int deltaY = command.substring(3, 5).toInt();
    posX = constrain(posX + deltaX, 0, 180);
    posY = constrain(posY + deltaY, 90, 180);

    servoX.write(posX);
    servoY.write(posY);
  }
}

void controlManual() {
  unsigned long currentMillis = millis();
  // Leer joystick
  int valX = analogRead(joyX);
  int valY = analogRead(joyY);

  // Mapear valores del joystick
  valX = map(valX, 0, 1023, 0, 180);
  valY = map(valY, 0, 1023, 0, 180);

  //Disparo
  if (digitalRead(BotonD) == HIGH) {
    servoDis.write(120);
  } else {
    servoDis.write(45);
  }
  // Control de servos
  if (valY >= 158) {
    valY = 156;
  }
  if (valY <= 75) {
    valY = 77;
  }
  if (valX >= 176) {
    valX = 170;
  }
  if (valX <= 12) {
    valX = 15;
  }
  servoX.write(valX);
  servoY.write(valY);


  // Audio aleatorio cada 5 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int randomTrack = random(42, 65);  // Seleccionar archivo aleatorio
    myDFPlayer.play(randomTrack);
  }
}