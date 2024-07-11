/*
/*
ver ::cl 20120520
Configuracion basica para modulo transmisor RT 11
Utiliza libreria VirtualWire.h
pin 01 entrada desde Arduino pin digital 2
pin 02 Tierra
pin 07 tierra
pin 08 antena externa
pin 09 tierra
pin 10 5v
*/

#include <VirtualWire.h>
#include<stdio.h>  
#include<string.h>  
#define N strlen(gen_poly)
char gen_poly[7] = "100101";
int data_length, i, j;


String msg = "holaholachaochao\n";
//String msg = "";
String destino = "4";
bool stringComplete = false;


struct Packet {
    unsigned short origen = 04; // 2 bytes
    unsigned short destino; // 2 bytes
    unsigned short crc; // 2 bytes // falta
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];  
};

void XOR(char *check_value) {  
    for (int j = 1; j < N; j++)  
        check_value[j] = ((check_value[j] == gen_poly[j]) ? '0' : '1');  
}

void charToBinary(char c, char *binary) {
    for (int i = 0; i < 8; ++i) {
        binary[7 - i] = ((c >> i) & 1) + '0'; // Convertir bit a '0' o '1'
    }
    binary[8] = '\0'; // Añadir el terminador nulo
}

void crc(char *data,char *check_value) {  
    // initializing check_value  
    for (int i = 0; i < N; i++)  
        check_value[i] = data[i];  

    do {  
        // check if the first bit is 1 and calls XOR function  
        if (check_value[0] == '1')  
            XOR(check_value);  

        // Move the bits by 1 position for the next computation  
        for (int j = 0; j < N - 1; j++)  
            check_value[j] = check_value[j + 1];  

        // appending a bit from data  
        check_value[j] = data[i++];  
    } while (i <= data_length + N - 1);  

    check_value[N - 1] = '\0'; // Asegurar terminador nulo al final
}  

char paquetesTotales(){
  int total = (((int)msg.length()-1) / 8);
    if((msg.length()-1) % 8 == 0){
      return (char)total;
    }
    else
      return (char)(total +1);
}


void creaPaquete(String msg2, int num_paquete){
    Packet packet;
    strncpy(packet.message, msg2.c_str(), sizeof(packet.message) );
    int len = msg2.length();
    char binaryStr[8 * len + N]; // Espacio para los bits y el terminador nulo

    binaryStr[0] = '\0'; // Iniciar la cadena vacía
    char check_value[8];

    for (int i = 0; i < len; ++i) {
        char binary[9]; // Para almacenar la representación binaria de un carácter
        charToBinary(packet.message[i], binary);
        strcat(binaryStr, binary); // Concatenar la representación binaria
    }
    
    data_length = strlen(binaryStr);
    
    for (int i = data_length; i < data_length + N - 1; i++) {
        binaryStr[i] = '0';  
    }
    binaryStr[data_length + N - 1] = '\0';

    crc(binaryStr, check_value);

    unsigned short crc_value = 0;
    for (int i = 0; i < N - 1; i++) {
        if (check_value[i] == '1') {
            crc_value |= (1 << ((N - 2) - i));
        }
    }
    
    
    packet.crc= (unsigned short)crc_value;
    
    packet.total = paquetesTotales();
    
    packet.secuencia = (char)(num_paquete);
    Serial.println(packet.message);
    packet.destino = (unsigned short)(destino.toInt());
    // ENvia paquete
    char charArray[sizeof(Packet)];
    memcpy(charArray, &packet, sizeof(Packet));
    //for(int i = 0;i < 8;i++)
    //  Serial.print(packet.message[i]);
    //Serial.println();  
    
    vw_send((uint8_t *)charArray, sizeof(charArray)); 
    memset(binaryStr,"\0",sizeof(binaryStr));
    memset(check_value,"\0",sizeof(check_value));
    Serial.println(packet.secuencia);
    Serial.print("Crc del paquete : " + String(packet.crc) + "\n");
    
}

String ingresaTexto(const char* prompt) {
  String texto = "";
  Serial.println(prompt);
  while (!stringComplete) {
    while (Serial.available() > 0) {
      char receivedChar = (char)Serial.read();
      texto += receivedChar;

      if (receivedChar == '\n') {
        stringComplete = true;
      }
    }
  }
  stringComplete = false;
  return texto;
}

void setup(){
    vw_set_ptt_inverted(true);
    vw_setup(2000);
    vw_set_tx_pin(2);    
    Serial.begin(9600);
   
    

}

void loop() {
  /*if (destino.length() == 0) {
    destino = ingresaTexto("# Ingrese destino: ");
    Serial.print("Destino: " + destino + "\n");
  }

  if (msg.length() == 0 && destino.length() > 0) {
    msg = ingresaTexto("# Ingrese mensaje:");
  }*/

  if (msg.length() > 0 && destino.length() > 0) {
    String msg2 = "";
    int num_paquete = 0;
    for (int i = 0; i < msg.length(); i++) {
      msg2 += msg[i];
      if (msg2.length() == 8) {
        num_paquete++;
        creaPaquete(msg2,num_paquete);
        msg2 = "";
      } else if ((msg.length() - i) <= 7 && i == msg.length() - 1) {
        num_paquete++;
        creaPaquete(msg2,num_paquete);
        msg2 = "";
        
      }
    }
    Serial.print("Mensaje con destino a arduino " + destino + " ¡ Ha sido enviado!\n\n");
    //msg = ""; 
    //destino = ""; 
    
  delay(10000);
}
}



