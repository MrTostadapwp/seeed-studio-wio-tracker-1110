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
  { LR11x0::MODE_STBY,   { LOW,  LOW  } }, { LR11x0::MODE_RX,     { HIGH, LOW  } },
  { LR11x0::MODE_TX,     { HIGH, HIGH } }, { LR11x0::MODE_TX_HP,  { LOW,  HIGH } },
  { LR11x0::MODE_TX_HF,  { LOW,  LOW  } }, { LR11x0::MODE_GNSS,   { LOW,  LOW  } },
  { LR11x0::MODE_WIFI,   { LOW,  LOW  } }, END_OF_MODE_TABLE,
};

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  SPI.setPins(RADIO_MISO_PIN, RADIO_SCK_PIN, RADIO_MOSI_PIN);
  SPI.begin();
  
  radio.tcxoVoltage = 1.6;
  if (radio.begin(915.0) != RADIOLIB_ERR_NONE) {
    while (true);
  }
  radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table);
  Serial.println(F("Estación Base lista para recibir telemetría..."));
}

void loop() {
  uint8_t payload[8]; 
  int state = radio.receive(payload, 8);

  if (state == RADIOLIB_ERR_NONE) {
    int32_t lat_entero = (payload[0] << 24) | (payload[1] << 16) | (payload[2] << 8) | payload[3];
    int32_t lon_entero = (payload[4] << 24) | (payload[5] << 16) | (payload[6] << 8) | payload[7];

    float latitud = lat_entero / 1000000.0;
    float longitud = lon_entero / 1000000.0;
    
    // Imprimimos un formato especial para que Python lo lea fácilmente
    Serial.print("DATA:");
    Serial.print(latitud, 6);
    Serial.print(",");
    Serial.println(longitud, 6);
  } 
}