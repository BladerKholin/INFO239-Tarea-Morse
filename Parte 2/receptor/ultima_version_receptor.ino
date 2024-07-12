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
int origen_recibido = -1;  // recibir de un solo origen

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
                
                imprimeDatos(packet);
                Serial.print("Mensaje: " + String(packet.message));
                if( grupos[packet.origen] == false && origen_recibido == -1){
                    num_secuencia++;
                    Serial.print("no está\n");
                    grupos[packet.origen] = true;
                    tiempos[packet.origen].origen = packet.origen;
                    tiempos[packet.origen].tiempo_fin = millis() + (tiempo_paquete * packet.total);
                      
                    for (int i = 0; i < 8; i++) {
                        msg += packet.message[i];
                    }
                    Serial.println("Llegó un paquete: ");
                      
                    origen_recibido = (int)packet.origen;  // guarda origen
                       
                    // Serial.print("Grupo: " + String(tiempos[packet.origen].origen) + " tiempo fin " + String(tiempos[packet.origen].tiempo_fin) + " \n" );     
                }
                else{
                    num_secuencia++;
                    if((int)packet.origen == origen_recibido){
                          for (int i = 0; i < 8; i++) {
                            msg += packet.message[i];
                        }
                    }

                } 
                //Serial.println(msg); 
                imprimeDatos(packet);
                
                if(num_secuencia != packet.secuencia && origen_recibido == packet.origen) {
                    Serial.println("Paquete perdido");
                    //Serial.println(num_secuencia);
                    // Añadir marcador de paquete perdido y reiniciar secuencia
                    msg += "####";
                }

                if(num_secuencia == packet.total && origen_recibido == (int)packet.origen ){
                  Serial.print("\n# MENSAJE CORRECTO del grupo " + String(packet.origen) + " ! \n Mensaje completo: ");
                  Serial.println(msg);
                  //Serial.print("Cantidad de paquetes recibidos: " + String(num_secuencia) + " \n");
                  Serial.print("\n\n\n");
                  msg = "";
                  grupos[packet.origen] = false;

                    origen_recibido = -1;
                  num_secuencia = 0;
                  
                }

                }
                else if (packet.destino == broadcast){
                  imprimeDatos(packet);
                  Serial.print("Mensaje: " + String(packet.message));
                }
                
    }

      
    for(const auto& tuple : tiempos){
        if(grupos[tuple.origen] == true && tuple.tiempo_fin   <= (unsigned long)millis() ){
          if(origen_recibido == (int)tuple.origen){  // quizas cambiar
          Serial.print("\n# TIEMPO EXPIRADO! \nMensaje del grupo " + String(tuple.origen) + " : " + msg + "\n");
          //Serial.println(millis());
          msg = "";
          grupos[tuple.origen] = false;
          Serial.print("Cantidad de paquetes recibidos: " + String(num_secuencia) + " \n");
          Serial.print("\n\n\n");
          num_secuencia = 0;
              origen_recibido = -1;
          }
      }
    }
    
}
