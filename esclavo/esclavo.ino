#include <Wire.h>

const int slaveDir = 8;
const int LEDPin = 9;
const int LEDsend = 8;

int buttonState; // Variable para almacenar el estado del botón recibido
const int pointTime = 200;
const int timeDelta = 50;
bool pressed = false; bool isAChar = false;
char palabra[5] = "";

int startPressed, endPressed, startSpace, endSpace = 0;

void setup() {
  Wire.begin(slaveDir);            // Iniciar como esclavo con dirección 8
  Wire.onReceive(receiveEvent); // Registrar función de recepción
  Wire.onRequest(requestEvent);
  Serial.begin(9600);       // Iniciar comunicación serial para depuración
  pinMode(LEDPin, OUTPUT);
  pinMode(LEDsend, OUTPUT);
}

void loop() {
  // Nada que hacer aquí, todo el trabajo se hace en receiveEvent
}

void receiveEvent(int howMany) {
  while(Wire.available()){
    buttonState = Wire.read(); // Leer el estado del botón
    if(!isAChar){
      if (buttonState == 1){
        digitalWrite(LEDPin, HIGH);
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
        digitalWrite(LEDPin, LOW);
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
      digitalWrite(LEDsend, HIGH);
      delay(200);
      Serial.println();
      Serial.print("Morse enviado: ");
      Serial.print(palabra);
      Serial.println();
      Wire.write(palabra, sizeof(palabra));
      isAChar = false;
      strcpy(palabra, "");
      //digitalWrite(LEDsend,LOW);
    }else{
      digitalWrite(LEDsend, LOW);
      Wire.write("EMPTY", sizeof("EMPTY"));
    }
}
