#include <Adafruit_TinyUSB.h>
#include <bluefruit.h>

// Cambia al pin digital donde tengas conectado tu buzzer activo
const int pinBuzzer = 15; //o led

// Crea el servicio UART (Serial por Bluetooth)
BLEUart bleuart;

void setup() {
  Serial.begin(115200);
  
  // Configura el buzzer
  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);

  Serial.println("Iniciando BLE UART...");
  
  // Inicializa el radio BLE
  Bluefruit.begin();
  Bluefruit.setTxPower(4); 
  Bluefruit.setName("Wio_Buzzer");

  // Inicializa el servicio UART
  bleuart.begin();

  // Configura la emisión (Advertising) para que la app nRF Connect reconozca el servicio UART
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart); // Anuncia que acepta datos
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.start(0); 

  Serial.println("Listo. Conéctate desde nRF Connect y envía el número 1");
}

void loop() {
  // Verifica si llegaron datos desde el celular
  while (bleuart.available()) {
    char datoRecibido = (char) bleuart.read();
    Serial.print("Recibí: ");
    Serial.println(datoRecibido);

    // Si el celular envía un '1', hace sonar el buzzer
    if (datoRecibido == '1') {
      digitalWrite(pinBuzzer, HIGH);
      delay(300); // Pitido de 300ms
      digitalWrite(pinBuzzer, LOW);
    }
  }
}