#include <Wire.h>

String letras[] = {"A","B","C","D","E","F","G","H","I","J","K",
                    "L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};

String morse[] = {".-", "-...", "-.-.",
                        "-.." ,".","..-.",
                        "--.","....","..",
                        ".---", "-.-", ".-..",
                        "--", "-.", "---",
                        ".--.", "--.-", ".-." ,
                        "..." , "-" , "..-",
                        "...-", ".--", "-..-",
                        "-.--" , "--.." 
};

String receivedString = "";

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
    decodificar();
  }

  Serial.println(buttonState); // Imprimir el estado del botón para depuración
}


void decodificar (){
  
    for(int i = 0; i < 26;i++){
      if(receivedString == morse[i]){
        Serial.print(letras[i]);
        break;
      }
    }
      delay(200);
    
  receivedString = "";
}
