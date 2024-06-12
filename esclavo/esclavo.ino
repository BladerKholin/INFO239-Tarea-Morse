#include <Wire.h>

int buttonState; // Variable para almacenar el estado del botón recibido
int pointTime = 200;
int timeDelta = 50;

bool presionado = false;
int startPressed, endPressed, startSpace, endSpace = 0;

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
    buttonState = Wire.read(); // Leer el estado del botón
    Serial.print(buttonState);
    if (buttonState == 1){
      startSpace, endSpace = 0;
      startPressed = millis();
      while (buttonState == 1){buttonState = Wire.read();}
      endPressed = millis();
      int time = endPressed - startPressed;
      if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
        Serial.print(".");
      }
      else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
        Serial.print("_");
      }
    }else{
      startPressed, endPressed = 0;
      startSpace = millis();
      while (buttonState == 0){buttonState = Wire.read();}
      endSpace = millis();
      int time = endSpace - startSpace;
      if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
        Serial.print("");
      }
      else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
        Serial.print(" ");
      }
      
    }
  }

  
}

