import cv2
import serial
import time

# Configuración de la conexión serial
ser = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)  # Espera para que se establezca la conexión

# Iniciar la captura de video
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # Convertir a escala de grises
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Cargar el clasificador de rostros de OpenCV
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

    # Detectar rostros
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    for (x, y, w, h) in faces:
        # Dibujar un rectángulo alrededor del rostro detectado (opcional)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

        # Calcular el centro del rostro
        cx = x + w // 2
        cy = y + h // 2

        # Enviar las coordenadas al Arduino
        ser.write(f"{cx},{cy}\n".encode('utf-8'))

    # Mostrar el video con los rostros detectados (opcional)
    cv2.imshow('Face Tracking', frame)

    # Salir si se presiona 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar recursos
cap.release()
cv2.destroyAllWindows()
ser.close()
