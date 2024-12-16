import cv2
import time
import serial

# Configuración de conexión serial
arduino = serial.Serial('COM3', 4800)  # Cambia '/dev/ttyUSB0' según tu configuración
time.sleep(2)  # Esperar que Arduino esté listo

# Configuración de cámara
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# Variables
frame_width = 640
frame_height = 480
center_tolerance_x = frame_width * 0.1
center_tolerance_y = frame_height * 0.1
last_command = "0,0,R"
last_face_time = 0
last_move_time = 0
face_detected_stable = False
face_centered_time = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.1, 5, minSize=(70, 70))

    if len(faces) > 0:
        x, y, w, h = faces[0]  # Procesar solo la primera cara detectada
        center_x = x + w // 2
        center_y = y + h // 2

        # Determinar la sección
        command = ""
        if center_x < frame_width // 3 and center_y < frame_height // 3:
            command = "+2,+2,Y"  # Superior Izquierda
        elif center_x > 2 * frame_width // 3 and center_y < frame_height // 3:
            command = "-2,+2,Y"  # Superior Derecha
        elif center_x < frame_width // 3 and center_y > 2 * frame_height // 3:
            command = "+2,-2,Y"  # Inferior Izquierda
        elif center_x > 2 * frame_width // 3 and center_y > 2 * frame_height // 3:
            command = "-2,-2,Y"  # Inferior Derecha
        elif center_x < frame_width // 3:
            command = "+2,+0,Y"  # Izquierda
        elif center_x > 2 * frame_width // 3:
            command = "-2,+0,Y"  # Derecha
        elif center_y < frame_height // 3:
            command = "+0,+2,Y"  # Arriba
        elif center_y > 2 * frame_height // 3:
            command = "+0,-2,Y"  # Abajo
        else:
            command = "+0,+0,G"  # Centro (Cara centrada)
        print(command)
        # Confirmar estabilidad del rostro
        current_time = time.time()
        if command != last_command:
            last_face_time = current_time
            face_detected_stable = False
            face_centered_time = 0

        if current_time - last_face_time >= 0.1:
            face_detected_stable = True

        if current_time - last_move_time >= 0.1:
            arduino.write((command + "\n").encode('utf-8'))
            last_command = command
            last_move_time = current_time

        # Manejo de tiempo centrado
        if command == "+0,+0,G":
            if face_centered_time == 0:
                face_centered_time = current_time
            elif current_time - face_centered_time >= 1.5:
                arduino.write("DISPARO\n".encode('utf-8'))
        else:
            face_centered_time = 0
    else:
        # No se detecta rostro
        arduino.write("+0,+0,R\n".encode('utf-8'))
        last_command = "+0,+0,R"
        face_centered_time = 0

    # Dibujar rectángulo en la cara detectada
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Mostrar imagen
    cv2.imshow("Frame", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
