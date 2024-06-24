#include <Wire.h>

int buttonState; // Variable para almacenar el estado del botón recibido
int pointTime = 200;
int timeDelta = 50;
bool pressed = false; bool isAChar = false;
char palabra[5] = "";

int startPressed, endPressed, startSpace, endSpace = 0;

void setup() {
  Wire.begin(8);            // Iniciar como esclavo con dirección 8
  Wire.onReceive(receiveEvent); // Registrar función de recepción
  Wire.onRequest(requestEvent);
  Serial.begin(9600);       // Iniciar comunicación serial para depuración
}

void loop() {
  // Nada que hacer aquí, todo el trabajo se hace en receiveEvent
}

void receiveEvent(int howMany) {
  while(Wire.available()){
    buttonState = Wire.read(); // Leer el estado del botón
    if(!isAChar){
      if (buttonState == 1){
        if(!pressed){
          pressed = true;
          startPressed = millis(); endPressed = 0;
          endSpace = millis();
          
          int time = endSpace - startSpace;
          if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
          }
          else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
            isAChar = true;
            delay(100);
          }
        }
      }else{
        if(pressed){
          pressed = false;
          endPressed=millis(); 
          startSpace=millis(); endSpace = 0;
          int time = endPressed - startPressed;
          if (time >= pointTime - timeDelta && time<=pointTime+ timeDelta){
            strcat(palabra,".");
            Serial.print(".");
          }
          else if(time >= 3*(pointTime - timeDelta) && time <= 3*(pointTime + timeDelta)){
            strcat(palabra,"-");
            Serial.print("-");
          }
        }
      }
    }
  }
}

void requestEvent(){
    if (isAChar){
      Serial.println();
      Serial.print(palabra);
      Serial.println();
      Wire.write(palabra, sizeof(palabra));
      isAChar = false;
      strcpy(palabra, "");
    /*if (!cola.isEmpty()){ 
      char *c;
      c = cola.dequeue();
      Serial.println();
      Serial.print(c);
      Serial.println();
      Wire.write(c, sizeof(c));*/
    }else{
      Wire.write("EMPTY", sizeof("EMPTY"));
    }
}
