#include <Wire.h>
char letras[] = {'A','B','C','D','E','F','G','H','I','J','K',
                    'L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

char *morse[] = {".-", "-...", "-.-.",
                        "-.." ,".","..-.",
                        "--.","....","..",
                        ".---", "-.-", ".-..",
                        "--", "-.", "---",
                        ".--.", "--.-", ".-." ,
                        "..." , "-" , "..-",
                        "...-", ".--", "-..-",
                        "-.--" , "--.." 
};



const int buttonPin = 9; // Pin donde está conectado el botón
const int slaveDir = 8;
const int recievePin = 10;
int buttonState = 0;     // Variable para almacenar el estado del botón
char c;

void setup() {
  Wire.begin();           // Iniciar como maestro
  pinMode(buttonPin, INPUT); // Configurar el pin del botón como entrada
  pinMode(recievePin, OUTPUT);
  Serial.begin(9600);     // Iniciar comunicación serial para depuración
}

void loop() {
  digitalWrite(recievePin,LOW);
  buttonState = digitalRead(buttonPin); // Leer el estado del botón
  
  Wire.beginTransmission(slaveDir); // Iniciar transmisión al esclavo con dirección 8
  Wire.write(buttonState);   // Enviar el estado del botón
  Wire.endTransmission();    // Finalizar transmisión
  char receivedData[6] = "";
  int index =0;
  Wire.requestFrom(slaveDir, sizeof(receivedData)-1); // Solicita hasta 20 bytes del esclavo con dirección 8
  while( (Wire.available())) {
    char buffer = Wire.read();
    if (buffer >=0){
      receivedData[index++] = buffer; // Lee cada byte enviado por el esclavo
    }
  }

  
  receivedData[index]='\0';
  if(strcmp(receivedData, "EMPTY")!=0){
    digitalWrite(recievePin,HIGH);
    delay(100);
    Serial.print(receivedData);
    Serial.print(" : ");
    decodificar(receivedData);
  }
}


void decodificar (char *palabra){
  int i = 0;
  bool esMorse = false;
  while(i < 26 && !esMorse ){
    if(strcmp(palabra,morse[i])==0){
        Serial.println(letras[i]);
        esMorse = true;
    }
    i++;
  }
  if(!esMorse){
    Serial.println("No es morse");
  }
}
