import cv2
import serial
import time

# Configurar el puerto serial (ajusta el puerto según tu configuración)
arduino = serial.Serial('/dev/ttyUSB0', 9600)
time.sleep(2)

# Cargar el clasificador de rostro
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# Iniciar la cámara
cap = cv2.VideoCapture(0)

# Definir el centro de la imagen (asumiendo una resolución de 640x480)
centerX_frame = 640 // 2
centerY_frame = 480 // 2

while True:
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Detección de rostro
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    if len(faces) == 0:
        # No se detectó ninguna cara, enviar mensaje especial
        arduino.write("NO_FACE\n".encode())
    else:
        for (x, y, w, h) in faces:
            # Calcular las coordenadas del centro del rostro
            face_centerX = x + w // 2
            face_centerY = y + h // 2

            # Calcular la diferencia del centro del rostro con respecto al centro del marco
            deltaX = face_centerX - centerX_frame
            deltaY = face_centerY - centerY_frame

            # Enviar las diferencias (delta) al Arduino
            arduino.write(f"{deltaX},{deltaY}\n".encode())

            # Dibujar un rectángulo alrededor del rostro
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
    
    # Mostrar el vídeo
    cv2.imshow('Face Tracking', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar la cámara y cerrar la ventana
cap.release()
cv2.destroyAllWindows()
