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


String msg = "holaholachaochaocomocomocasacasatrestresprogramagobiernohistoriapersonasadelante";
//String msg = "";
String destino = "4";
bool stringComplete = false;


struct Packet {
    unsigned short origen = 04; // 2 bytes
    unsigned short destino; // 2 bytes
    unsigned short crc= 0x00; // 2 bytes
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];  
};


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
    
    packet.total = paquetesTotales();
    
    packet.secuencia = (char)(num_paquete);
    
    packet.destino = (unsigned short)(destino.toInt());
    // ENvia paquete
    char charArray[sizeof(Packet)];
    memcpy(charArray, &packet, sizeof(Packet));
    for(int i = 0;i < 8;i++)
      Serial.print(packet.message[i]);
    Serial.println();  
    
    vw_send((uint8_t *)charArray, sizeof(charArray)); 

    Serial.println(packet.secuencia);
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
    
  delay(6000);
}
}


