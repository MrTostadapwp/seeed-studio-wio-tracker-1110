//el modulo usado es basico, asi que necesitamos un espacio libre de todo el ruido posible


#include <Adafruit_TinyUSB.h> 
#include <TinyGPS++.h>

TinyGPSPlus gps;
unsigned long ultimoMensaje = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Serial.println("Iniciando decodificador GPS...");
  
  Serial1.begin(9600); 
}

void loop() {
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());
  }

  if (millis() - ultimoMensaje > 2000) {
    
    if (gps.location.isValid()) {
      Serial.print("Latitud: ");
      Serial.print(gps.location.lat(), 6); // 6 decimales de precisión
      Serial.print(" | Longitud: ");
      Serial.println(gps.location.lng(), 6);
    } 
    else {
      Serial.println("Buscando satélites... (Lleva el módulo al exterior o a una ventana)");
    }
    
    ultimoMensaje = millis();
  }
}
