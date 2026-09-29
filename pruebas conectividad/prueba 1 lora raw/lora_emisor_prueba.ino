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

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  SPI.setPins(RADIO_MISO_PIN, RADIO_SCK_PIN, RADIO_MOSI_PIN);
  SPI.begin();
  
  radio.tcxoVoltage = 1.6;
  int state = radio.begin(915.0); 

  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("Fallo inicio"));
    while (true);
  }
  radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table);
}
//unicamente enviando datos crudos
void loop() {
  Serial.print(F("[LR1110] Enviando... "));
  
  // Forzamos un arreglo de exactamente 4 letras
  uint8_t mensaje[4] = {'H', 'O', 'L', 'A'};
  
  // Transmitimos obligando al radio a enviar exactamente 4 bytes
  int state = radio.transmit(mensaje, 4);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("Enviado."));
  }
  delay(3000);
}