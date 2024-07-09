#include <VirtualWire.h>


String msg = "holachao?juan";
int num_paquete = 1;

struct Packet {
    unsigned short origen = 0x08; // 2 bytes
    unsigned short destino = 0x04; // 2 bytes
    unsigned short crc = 0x00; // 2 bytes
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];  
};

void creaPaquete(String msg2){
    Packet packet;
    strncpy(packet.message, msg2.c_str(), sizeof(packet.message) );
    packet.secuencia = (char)(num_paquete++);
    
    
    int total = (((int)msg.length()) / 8);
    if(msg.length() == 8){
      num_paquete = 1;
      packet.total = (char)total;
    }
      
    else
      packet.total = (char)(total +1);


    // ENvia paquete
    char charArray[sizeof(Packet)];
    memcpy(charArray, &packet, sizeof(Packet));
    Serial.println(msg2);  
    vw_send((uint8_t *)charArray, sizeof(charArray)); 
}

void setup(){
    vw_set_ptt_inverted(true);
    vw_setup(2000);
    vw_set_tx_pin(2);    
    Serial.begin(9600);
}

void loop(){
    
    String msg2 = "";
      for(int i = 0; i < msg.length(); i++){
          msg2 += msg[i]; 
          if(msg2.length() == 8){
              creaPaquete(msg2);
              msg2 = "";
            
          }
          
          else if((msg.length() - i) <= 7 && i == msg.length() -1){
            
              creaPaquete(msg2);
              msg2 = "";
              num_paquete = 1;
          }
      }

    
    
    Serial.println("Enviado!");
    vw_wait_tx();
    delay(5000);
}
