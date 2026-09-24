#include <Adafruit_TinyUSB.h>
const int pinHumedad = 30; 

void setup() {

  Serial.begin(115200);

  while (!Serial) {
    delay(10);
  }

  Serial.println("Iniciando prueba del Moisture Sensor v1.4...");
}

void loop() {
  int valorHumedad = analogRead(pinHumedad);

  Serial.print("Lectura del sensor: ");
  Serial.println(valorHumedad);

  delay(1000);
}