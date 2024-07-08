#include <VirtualWire.h>

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
        Packet packet;
        if (buflen == sizeof(Packet)) {  // Verifica que el tamaño del mensaje recibido sea el esperado
            memcpy(&packet, buf, sizeof(Packet));
            
            
            // Imprimir la cabecera
            Serial.print("Origen: ");
            if(packet.origen == 8)
            Serial.println(packet.origen);
            
            
            
            // Imprimir el mensaje
            Serial.print("Mensaje Recibido: ");
            Serial.println(packet.message);

            Serial.print("Secuencia: ");
            Serial.print(packet.secuencia);
            Serial.print(" de : ");
            Serial.println(packet.total);

            
            
        } else {
            Serial.println("Error: Tamaño del paquete incorrecto");
        }
    }
}
