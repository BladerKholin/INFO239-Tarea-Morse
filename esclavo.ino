#include <Wire.h>

String buttonState; // Variable para almacenar el estado del botón recibido

void setup() {
  Wire.begin(8);            // Iniciar como esclavo con dirección 8
  Wire.onReceive(receiveEvent); // Registrar función de recepción
  Serial.begin(9600);       // Iniciar comunicación serial para depuración
}

void loop() {
  // Nada que hacer aquí, todo el trabajo se hace en receiveEvent
}

void receiveEvent(int howMany) {
  while (Wire.available()) { // Mientras haya datos disponibles
    buttonState = Wire.readString(); // Leer el estado del botón
  }

  Serial.println(buttonState); // Imprimir el estado del botón para depuración
}

