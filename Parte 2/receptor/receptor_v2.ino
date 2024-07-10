#include <VirtualWire.h>

unsigned short id_arduino = 0x04;
unsigned short broadcast = 0x00;
String msg = "";
int num_secuencia = 0;

struct Packet {
    unsigned short origen; // 2 bytes
    unsigned short destino; // 2 bytes
    unsigned short crc; // 2 bytes
    unsigned char secuencia; // 1 byte
    unsigned char total;  // 1 byte
    char message[8];
};

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

    if (vw_get_message(buf, &buflen)) {
        Packet packet;
        if (buflen == sizeof(Packet)) {
            memcpy(&packet, buf, sizeof(Packet));

            if (packet.destino == id_arduino || packet.destino == broadcast) {
                Serial.print("Origen: ");
                Serial.println(packet.origen);

                Serial.print("Total : ");
                Serial.println(packet.total);

                if (packet.secuencia == num_secuencia + 1) {
                    num_secuencia = packet.secuencia;
                    for (int i = 0; i < 8; i++) {
                        msg += packet.message[i];
                    }
                    Serial.print("Secuencia: ");
                    Serial.println(packet.secuencia);
                } else {
                    Serial.println("Paquete perdido");
                    Serial.print("Esperando secuencia: ");
                    Serial.println(num_secuencia + 1);
                    Serial.print("Recibido secuencia: ");
                    Serial.println(packet.secuencia);
                    
                    msg += "##" + String(num_secuencia + 1) + "##";
                    num_secuencia = packet.secuencia;
                    for (int i = 0; i < 8; i++) {
                        msg += packet.message[i];
                    }
                }

                Serial.println("Progreso del mensaje:");
                Serial.println(msg);
                if (num_secuencia == packet.total) {
                    Serial.println("Mensaje llegó al final");
                    num_secuencia = 0;
                    msg = "";
                }
            }

        } else {
            Serial.println("Error: Tamaño del paquete incorrecto");
        }
    }
}
