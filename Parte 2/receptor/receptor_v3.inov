#include <VirtualWire.h>

String mensaje_recibido = "";
struct Packet {
    unsigned short origen; // 2 bytes
    unsigned short destino; // 2 bytes
    unsigned short crc; // 2 bytes
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];  
};
void setup(){
    Serial.begin(9600);
    Serial.println("Configurando Recepcion");
    vw_set_ptt_inverted(true); 
    vw_setup(2000);
    vw_set_rx_pin(2);
    vw_rx_start();
}

void loop(){
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)){
        
        Serial.println("\nOrigen: ");
        for (int i = 0; i < 2; i++){  // buflen es la cantidad de bytes que llegan, en este caso 16
            Serial.print((int)buf[i]);  
        }
        Serial.println("\nDestino: ");
        for (int i = 2; i < 4; i++){  // buflen es la cantidad de bytes que llegan, en este caso 16
            Serial.print((int)buf[i]);
        }
        Serial.println("\nCRC:");
        for (int i = 4; i < 6; i++){  // buflen es la cantidad de bytes que llegan, en este caso 16
            Serial.print((int)buf[i]);
        }
        Serial.println("\nSecuencia: ");
        Serial.print((int)buf[6]);

        Serial.println("\nTotal: ");
        Serial.print((int)buf[7]);
        
        //Serial.println("\nMensaje: ");
        for (int i = 8; i < 16; i++){  // buflen es la cantidad de bytes que llegan, en este caso 16
            //Serial.print((char)buf[i]);
            if(buf[i] >= 0)
              mensaje_recibido += (char)buf[i];
        }
      if((int)buf[6] == (int)buf[7]){
            Serial.println("\n\nMensaje: " + mensaje_recibido);
            mensaje_recibido = "";
        }

    }

   memset(buf,"\0",sizeof(buf)); 
}
