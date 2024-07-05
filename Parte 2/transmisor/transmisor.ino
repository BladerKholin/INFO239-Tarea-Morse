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
const char *origen = "04"; char *destino = "04";
int secuencia = 1;
int CRC = 5;
int total = 1;
char *msg="Paskibe"; char package[17]; char temp[9];
int ledPin = 9;


void setup(){
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_tx_pin(2);
  pinMode(ledPin, INPUT);
  Serial.begin(9600);
}
void loop(){
   packageCodificator();
   Serial.print(package);
   vw_send((uint8_t *)package, strlen(package));
   vw_wait_tx();
}

void packageCodificator(){
  sprintf(temp, "%08s",msg); strcat(package, temp);
  sprintf(temp, "%1d",total); strcat(package, temp);
  sprintf(temp, "%1d",secuencia); strcat(package, temp);
  sprintf(temp, "%02d", CRC); strcat(package, temp);
  strcat(package, destino);
  strcat(package, origen);
};
