#include <Adafruit_TinyUSB.h>
#include <bluefruit.h>

// Define el pin del puerto Grove digital que estés usando (ej. D1 = pin 1)
const int pinTouch = 15; 

// Crea el servicio UART para Bluetooth
BLEUart bleuart;

void setup() {
  Serial.begin(115200);
  
  // Configura el pin del sensor táctil como entrada
  pinMode(pinTouch, INPUT);

  Serial.println("Iniciando BLE UART para Sensor Touch...");
  
  // Inicializa el radio BLE
  Bluefruit.begin();
  Bluefruit.setTxPower(4); 
  Bluefruit.setName("Wio_Touch");

  // Inicializa el servicio UART
  bleuart.begin();

  // Configura la emisión (Advertising) para que la app nRF Connect reconozca el servicio
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.start(0); 

  Serial.println("Listo. Conéctate desde nRF Connect al dispositivo 'Wio_Touch'.");
}

void loop() {
  // Lee el estado del sensor táctil
  int estadoTouch = digitalRead(pinTouch);

  // Si detecta el dedo (Active High)
  if (estadoTouch == HIGH) {
    Serial.println("Toque detectado de forma local.");
    
    // Envía el mensaje por Bluetooth a la app
    bleuart.println("¡Sensor Tocado!");
    
    // Pausa de medio segundo para evitar saturar la conexión con miles de mensajes por un solo toque
    delay(500); 
  }
}