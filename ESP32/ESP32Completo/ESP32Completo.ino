#include "config.h"
#include "Arduino.h"
#include <HardwareSerial.h>
#define SERIAL_PORT Serial2
#define IO_LOOP_DELAY 5000
int count = 0;
uint8_t Distancia_byte1;
uint8_t Distancia_byte2;
uint8_t Peso_byte1;
uint8_t Peso_byte2;
uint8_t bandera=0, Tirar=0;
uint16_t DistanciaTotal, PesoTotal;
unsigned long lastUpdate = 0;

// set up the 'counter' feed
AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *Distancia = io.feed("Sensor_Distancia");
AdafruitIO_Feed *Peso = io.feed("Sensor_Peso");
AdafruitIO_Feed *Lanzamiento = io.feed("Lanzamiento");
void setup() {

  // Inicializar la comunicación serial de depuración
  Serial.begin(115200);
  // Inicializar el puerto serial a 9600 baudios
  SERIAL_PORT.begin(9600);
  // wait for serial monitor to open
  while(!Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  Lanzamiento->onMessage(handleMessage);
  //Tirar=Lanzamiento;
  //Tirar = Lanzamiento->getValue().toInt();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText()); 
  Lanzamiento->get();

}

void loop() {
  io.run(); //De toda la vida
  if (bandera==0 && SERIAL_PORT.available() >= 4) {
    // Leer los 4 bytes recibidos del PIC16F887
    Distancia_byte1 = SERIAL_PORT.read();
    Distancia_byte2 = SERIAL_PORT.read();
    Peso_byte1 = SERIAL_PORT.read();
    Peso_byte2 = SERIAL_PORT.read();
    PesoTotal = combineBytes(Peso_byte1, Peso_byte2);
    DistanciaTotal = combineBytes(Distancia_byte1, Distancia_byte2);
    Serial.print("Distancia: ");
    Serial.println(DistanciaTotal);
    Serial.print("Peso: ");
    Serial.println(PesoTotal);
    bandera=1;
  }
  if (bandera==1) {
    
    //Mensaje que se envía a Adafruit
    Serial.print("sending -> Distancia");
    Serial.println(DistanciaTotal);
    Distancia->save(DistanciaTotal);
    Serial.print("sending -> Peso");
    Serial.println(PesoTotal);
    Peso->save(PesoTotal);
    bandera=2;  
    //Lanzamiento->get();
  }
  if (bandera==2 && Tirar==0){
    
    if (SERIAL_PORT.available() == 1) {
      bandera=SERIAL_PORT.read();
    }
    else{
    while (SERIAL_PORT.available() > 0) {
      SERIAL_PORT.read(); // Leer y descartar el byte del búfer
    }
    }
    Serial.print("PIC -> Bandera: ");
    Serial.println(bandera);
    Serial.print("ADAFRUIT -> Tirar: ");
    Serial.println(Tirar);
    
    if (Tirar==1){
      SERIAL_PORT.write(1);
      
      bandera=0;
      
    }
    delay(1250);
  }
  delay(500);

}
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());
  Tirar = data->toInt();
}

uint16_t combineBytes(uint8_t highByte, uint8_t lowByte) {
  return (highByte << 8) | lowByte;
}
