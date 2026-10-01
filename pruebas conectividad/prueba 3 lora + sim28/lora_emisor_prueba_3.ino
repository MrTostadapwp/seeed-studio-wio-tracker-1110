#include <RadioLib.h>
#include <SPI.h>
#include <TinyGPS++.h>

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

TinyGPSPlus gps;
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600); // El módulo SIM28 opera a 9600 baudios por defecto
  
  SPI.setPins(RADIO_MISO_PIN, RADIO_SCK_PIN, RADIO_MOSI_PIN);
  SPI.begin();
  
  radio.tcxoVoltage = 1.6;
  if (radio.begin(915.0) != RADIOLIB_ERR_NONE) {
    while (true);
  }
  radio.setRfSwitchTable(rfswitch_dio_pins, rfswitch_table);
  Serial.println(F("Tracker IoT Iniciado. Sal al exterior para obtener señal GPS..."));
}

void loop() {
  // Leemos continuamente el puerto UART del GPS
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  if (millis() - ultimoEnvio > 5000) {
    float latitud;
    float longitud;

    // Verificamos si el GPS ya tiene señal real
    if (gps.location.isValid()) {
      latitud = gps.location.lat();
      longitud = gps.location.lng();
      Serial.println(F("\n[+] --- GPS FIJADO ---"));
    } else {
      // Coordenadas falsas de relleno mientras busca satélites
      latitud = 19.043210;
      longitud = -98.201234;
      Serial.println(F("\n[-] --- BUSCANDO SATÉLITES (Enviando datos falsos de relleno) ---"));
    }

    // Mostramos la lectura exacta en el Monitor Serie local
    Serial.print(F("Latitud:  "));
    Serial.println(latitud, 6);
    Serial.print(F("Longitud: "));
    Serial.println(longitud, 6);

    // Comprimimos para enviar por LoRa
    int32_t lat_entero = latitud * 1000000;
    int32_t lon_entero = longitud * 1000000;

    uint8_t payload[8];
    payload[0] = (lat_entero >> 24) & 0xFF;
    payload[1] = (lat_entero >> 16) & 0xFF;
    payload[2] = (lat_entero >> 8) & 0xFF;
    payload[3] = lat_entero & 0xFF;

    payload[4] = (lon_entero >> 24) & 0xFF;
    payload[5] = (lon_entero >> 16) & 0xFF;
    payload[6] = (lon_entero >> 8) & 0xFF;
    payload[7] = lon_entero & 0xFF;

    Serial.print(F("Transmitiendo payload por LoRa... "));
    int state = radio.transmit(payload, 8);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("Enviado."));
    } else {
      Serial.println(F("Error de radio."));
    }
    
    ultimoEnvio = millis();
  }
}