#include <Adafruit_TinyUSB.h>
#include <bluefruit.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Iniciando radio BLE...");
  Bluefruit.begin();

  // Arreglo para almacenar los 6 bytes de la dirección MAC
  uint8_t mac[6];
  Bluefruit.getAddr(mac);

  Serial.print("La dirección MAC de este Wio Tracker es: ");
  
  // Imprime la dirección en formato estándar (XX:XX:XX:XX:XX:XX) en mayúsculas
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 0x10) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0) Serial.print(":");
  }
  Serial.println();

  // Emite una señal básica para que la app lo mantenga en la lista
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.start(0);
}

void loop() {
  delay(1000);
}