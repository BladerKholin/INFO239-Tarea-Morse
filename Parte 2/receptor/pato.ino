#include <VirtualWire.h>


struct Tuple {
    unsigned short origen;
    unsigned long tiempo_fin;
};

unsigned short id_arduino = 0x04;
unsigned short broadcast = 0x00;
String msg = "";
int num_secuencia = 0;
int tiempo_paquete = 500;
bool grupos[12] = {false,false,false,false,false,false,false,false,false,false,false,false};
Tuple tiempos[12]; 

uint16_t comprueba_crc;



struct Packet {
    unsigned short origen; // 2 bytes
    unsigned short destino; // 2 bytes
    uint16_t crc; // 2 bytes
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];
};

void imprimeDatos(Packet &packet){
  Serial.print("\nOrigen: ");
  Serial.println(packet.origen);
  Serial.print("Secuencia: ");
  Serial.println(packet.secuencia);  
  Serial.print("Crc: ");
  Serial.println(packet.crc);
  Serial.print("Total: ");
  Serial.println(packet.total);
}

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

void setup() {
    Serial.begin(9600);
    Serial.println("Configurando Recepcion");
    vw_set_ptt_inverted(true);
    vw_setup(2000);
    vw_set_rx_pin(2);
    vw_rx_start();
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    unsigned long tiempo_recepcion = millis();
    if (vw_get_message(buf, &buflen)) {
        
        Packet packet;
        //if (buflen == sizeof(Packet)) {
            memcpy(&packet, buf, sizeof(Packet));
            //int tiempo_mensaje = tiempo_paquete*packet.total;
            if (packet.destino == id_arduino) { 
                num_secuencia++;
                imprimeDatos(packet);
                Serial.print("Mensaje: " + String(packet.message));
                if( grupos[packet.origen] == false){
                    
                    Serial.print("no está\n");
                    grupos[packet.origen] = true;
                    tiempos[packet.origen].origen = packet.origen;
                    tiempos[packet.origen].tiempo_fin = millis() + (tiempo_paquete * packet.total);
                      
                    for (int i = 0; i < 8; i++) {
                        msg += packet.message[i];
                    }
                    Serial.println("Llegó un paquete: ");
                      
                    comprueba_crc = calcularCRC(String(packet.message), (int)(String(packet.message).length()));
                       
                    // Serial.print("Grupo: " + String(tiempos[packet.origen].origen) + " tiempo fin " + String(tiempos[packet.origen].tiempo_fin) + " \n" );     
                }
                else{
                  for (int i = 0; i < 8; i++) {
                        msg += packet.message[i];
                    }
                    
                    comprueba_crc = calcularCRC(String(packet.message), (int)(String(packet.message).length()));
                } 
                //Serial.println(msg); 
                imprimeDatos(packet);

                if(comprueba_crc != packet.crc){
                  Serial.println("CRC distinto, mensaje CON error!");
                }
                else{
                  Serial.println("CRC iguales, mensaje SIN error!");
                }

                Serial.print("Crc calculado: ");
                Serial.println(comprueba_crc);
                if(num_secuencia != packet.secuencia) {
                    Serial.println("Paquete perdido");
                    //Serial.println(num_secuencia);
                    // Añadir marcador de paquete perdido y reiniciar secuencia
                    msg += "####";
                }

                if(num_secuencia == packet.total ){
                  Serial.print("\n# MENSAJE CORRECTO del grupo " + String(packet.origen) );
                  Serial.print( " ! \n Mensaje completo: ");
                  Serial.println(msg);
                  //Serial.print("Cantidad de paquetes recibidos: " + String(num_secuencia) + " \n");
                  Serial.print("\n\n\n");
                  msg = "";
                  grupos[packet.origen] = false;

                  num_secuencia = 0;
                  
                }

                }
                else if (packet.destino == broadcast){
                  imprimeDatos(packet);
                  if(comprueba_crc != packet.crc){
                  Serial.println("CRC distinto, mensaje CON error!");
                }
                else{
                  Serial.println("CRC iguales, mensaje SIN error!");
                }
                  Serial.print("Mensaje: ");
                  Serial.println(String(packet.message));
                  comprueba_crc = calcularCRC(String(packet.message), (int)(String(packet.message).length()));
                  
                }
                
    }

    // MODIFICAR grupos[en la posicion] ,se guardar el valor origen del paquete, en este caso el msg es del arduino 4
    // por eso le puse 4 y no una variable. 
    for(const auto& tuple : tiempos){
        if(grupos[tuple.origen] == true && tuple.tiempo_fin   <= (unsigned long)millis()){
          
          Serial.print("\n# TIEMPO EXPIRADO! \nMensaje del grupo ");
          Serial.print(String(tuple.origen));
          Serial.println(" : " + msg + "\n");
          msg = "";
          grupos[tuple.origen] = false;
          Serial.print("Cantidad de paquetes recibidos: ");
          Serial.print(num_secuencia);
          Serial.print("\n\n\n");
          num_secuencia = 0;
      }
    }
    
}
