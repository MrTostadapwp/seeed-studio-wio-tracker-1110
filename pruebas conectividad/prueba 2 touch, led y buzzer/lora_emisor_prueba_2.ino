#include <RadioLib.h>
#include <SPI.h> 

#define RADIO_CS_PIN    44
#define RADIO_DIO1_PIN  2
#define RADIO_RST_PIN   42
#define RADIO_BUSY_PIN  43
#define RADIO_MISO_PIN  47
#define RADIO_SCK_PIN   45
#define RADIO_MOSI_PIN  46

LR1110 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN, SPI);

static const uint32_t rfswitch_dio_pins[] = { RADIOLIB_LR11X0_DIO5, RADIOLIB_LR11X0_DIO6, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC };
static const Module::RfSwitchMode_t rfswitch_table[] = {
  { LR11x0::MODE_STBY,   { LOW,  LOW  } },
  { LR11x0::MODE_RX,     { HIGH, LOW  } },
  { LR11x0::MODE_TX,     { HIGH, HIGH } },
  { LR11x0::MODE_TX_HP,  { LOW,  HIGH } },
  { LR11x0::MODE_TX_HF,  { LOW,  LOW  } },
  { LR11x0::MODE_GNSS,   { LOW,  LOW  } },
  { LR11x0::MODE_WIFI,   { LOW,  LOW  } },
  END_OF_MODE_TABLE,
};

// --- PIN DEL SENSOR ---
const int pinSensor = 15; // Corresponde al puerto D2 del Wio Tracker
uint8_t estadoAnterior = 255; // Variable para detectar cambios

void setup() {
  Serial.begin(115200);
  pinMode(pinSensor, INPUT); // Configuramos el pin como entrada

  SPI.setPins(RADIO_MISO_PIN, RADIO_SCK_PIN, RADIO_MOSI_PIN);
  SPI.begin();
  
  radio.tcxoVoltage = 1.6;
  int state = radio.begin(915.0); 
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("Fallo inicio"));
    while (true);
  }
  radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table);
  Serial.println(F("Emisor listo. Toca el sensor para enviar..."));
}

void loop() {
  // Leemos si el sensor está siendo tocado (1) o no (0)
  uint8_t estadoActual = digitalRead(pinSensor);
  
  // Solo transmitimos si el estado cambió (presionado o soltado)
  if (estadoActual != estadoAnterior) {
    Serial.print(F("Transmitiendo estado: "));
    Serial.println(estadoActual);
    
    // Enviamos el byte exacto (tamaño 1)
    radio.transmit(&estadoActual, 1);
    
    estadoAnterior = estadoActual; // Actualizamos la memoria
  }
  
  delay(50); // Pequeña pausa de estabilidad
}