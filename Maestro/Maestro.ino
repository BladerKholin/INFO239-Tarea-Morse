#include <Wire.h>

const int buttonPin = 9; // Pin donde está conectado el botón
int buttonState = 0;     // Variable para almacenar el estado del botón

void setup() {
  Wire.begin();           // Iniciar como maestro
  pinMode(buttonPin, INPUT); // Configurar el pin del botón como entrada
  Serial.begin(9600);     // Iniciar comunicación serial para depuración
}

void loop() {
  buttonState = digitalRead(buttonPin); // Leer el estado del botón
  Serial.println(buttonState); // Imprimir el estado del botón para depuración
  Wire.beginTransmission(8); // Iniciar transmisión al esclavo con dirección 8
  Wire.write(buttonState);   // Enviar el estado del botón
  Wire.endTransmission();    // Finalizar transmisión

 
}
