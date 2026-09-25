//prueba de comunicacion

#include <Adafruit_TinyUSB.h>

void setup() {

  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Serial.println("Iniciando prueba del módulo GPS SIM28...");

  Serial1.begin(9600);
}

void loop() {

  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}
