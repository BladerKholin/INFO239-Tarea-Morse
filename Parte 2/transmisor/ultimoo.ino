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




//String msg = "holaholachaochaoholaholachaochaoholaholachaochaoholaholachaochaoholaholachaochao\n";
String msg = "";
String destino = "";
bool stringComplete = false;


struct Packet {
    unsigned short origen; // 2 bytes
    unsigned short destino; // 2 bytes
    uint16_t crc; // 2 bytes // falta
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];  
};

uint16_t calcularCRC(String msg2, int length) {
    uint16_t crc = 0x1F;  // Valor inicial del CRC-5-USB
    uint16_t poly = 0x15;  // Polinomio x^5 + x^2 + 1

    for (uint8_t i = 0; i < length; i++) {
        crc ^= msg2[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x10) {  // Si el bit más significativo (bit 4) es 1
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc & 0x1F;  // El CRC-5-USB es de 5 bits
}

uint16_t toBigEndian(uint16_t value) {
  return (value >> 8) | (value << 8);
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
    
    
    packet.crc = calcularCRC(msg2, (int)msg2.length());
    packet.crc = toBigEndian(packet.crc);
    packet.total = paquetesTotales();
    
    packet.secuencia = (char)(num_paquete);
    Serial.println(packet.message);
    packet.destino = (unsigned short)(destino.toInt());
    packet.destino = toBigEndian(packet.destino);
    packet.origen = 04;
    packet.origen = toBigEndian(packet.origen);
    // ENvia paquete
    char charArray[sizeof(Packet)];
    memcpy(charArray, &packet, sizeof(Packet));
    //for(int i = 0;i < 8;i++)
    //  Serial.print(packet.message[i]);
    //Serial.println();  
    
    vw_send((uint8_t *)charArray, sizeof(charArray)); 
    
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
  if (destino.length() == 0) {
    destino = ingresaTexto("# Ingrese destino: ");
    Serial.print("Destino: " + destino + "\n");
  }

  if (msg.length() == 0 && destino.length() > 0) {
    msg = ingresaTexto("# Ingrese mensaje:");
  }

  if (msg.length() > 0 && destino.length() > 0) {
    String msg2 = "";
    int num_paquete = 0;
    for (int i = 0; i < msg.length()-1; i++) {
      msg2 += msg[i];
      if (msg2.length() == 8) {
        num_paquete++;
        creaPaquete(msg2,num_paquete);
        msg2 = "";
      } else if ((msg.length() - i) <= 7 && i == msg.length() - 2) {
        num_paquete++;
        creaPaquete(msg2,num_paquete);
        msg2 = "";
        
      }
    }
    Serial.print("Mensaje con destino a arduino " + destino + " ¡ Ha sido enviado!\n\n");
    msg = ""; 
    destino = ""; 
    
  delay(6000);
}
}
