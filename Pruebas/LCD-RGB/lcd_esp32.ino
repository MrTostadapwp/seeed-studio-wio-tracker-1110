#include <Wire.h>
#include "rgb_lcd.h"

// Crea el objeto para controlar la pantalla
rgb_lcd lcd;

// Definición de pines para el ESP32-S3
const int SDA_PIN = 17;
const int SCL_PIN = 18;

void setup() {
  Serial.begin(115200);

  // Inicializa el bus I2C indicando los pines exactos
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Inicializa la pantalla de 16 columnas y 2 filas
  lcd.begin(16, 2);
  
  // Configura un color de fondo (por ejemplo, verde)
  lcd.setRGB(0, 255, 0); 
  
  // Imprime el texto de prueba
  lcd.setCursor(0, 0);
  lcd.print("Hola ESP32-S3!");
  
  lcd.setCursor(0, 1);
  lcd.print("Prueba superada");
}

void loop() {
  // El texto se queda estático
}