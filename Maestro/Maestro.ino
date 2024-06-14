#include <Wire.h>

const int buttonPin = 9; // Pin donde está conectado el botón
int buttonState = 0;     // Variable para almacenar el estado del botón
char c;

void setup() {
  Wire.begin();           // Iniciar como maestro
  pinMode(buttonPin, INPUT); // Configurar el pin del botón como entrada
  Serial.begin(9600);     // Iniciar comunicación serial para depuración
}

void loop() {
  buttonState = digitalRead(buttonPin); // Leer el estado del botón
  
  Wire.beginTransmission(8); // Iniciar transmisión al esclavo con dirección 8
  Wire.write(buttonState);   // Enviar el estado del botón
  Wire.endTransmission();    // Finalizar transmisión
  
  Wire.requestFrom(8, 20); // Solicita hasta 20 bytes del esclavo con dirección 8
  if( (Wire.available()) and (c = Wire.read())!=" ") {
    //c = Wire.read(); // Lee cada byte enviado por el esclavo
    receivedString += c; // Añade el byte al string
  }
  else if( (Wire.available()) and (c = Wire.read())==" ") {
    //c = Wire.read(); // Lee cada byte enviado por el esclavo
    
    Serial.print(receiveString);
  }

  Serial.println(buttonState); // Imprimir el estado del botón para depuración
}
