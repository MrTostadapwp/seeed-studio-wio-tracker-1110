//se uso un modulo de buzzer incluido en el kit


#include <Adafruit_TinyUSB.h>
#include <Wire.h>
#include <LIS3DHTR.h>

#define WIRE Wire
LIS3DHTR<TwoWire> LIS; 
const int pinBuzzer = 30; 

void setup() 
{
    delay(100); 

    Serial.begin(115200);
    while (!Serial) delay(100);   
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, LOW);

    LIS.begin(WIRE, 0x19); 

    if (!LIS) {
        Serial.println("LIS3DHTR no conectó.");
        while (1);
    }
    
    Serial.println("Sistema iniciado. Esperando detectar caída...");
}

void loop()
{
    float x = LIS.getAccelerationX();
    float y = LIS.getAccelerationY();
    float z = LIS.getAccelerationZ();

    // Imprimir para monitoreo
    Serial.print("x:"); Serial.print(x); Serial.print("  ");
    Serial.print("y:"); Serial.print(y); Serial.print("  ");
    Serial.print("z:"); Serial.println(z);
//filtro
    if (abs(x) < 0.2 && abs(y) < 0.2 && abs(z) < 0.2) {
        Serial.println("¡ALERTA: CAÍDA LIBRE DETECTADA!");
        
//patron de la alarma
        for (int i = 0; i < 6; i++) {
            digitalWrite(pinBuzzer, HIGH);
            delay(50);
            digitalWrite(pinBuzzer, LOW);
            delay(75); // Apagado por 100 milisegundos
        }
        
        Serial.println("Alarma finalizada, reanudando monitoreo...");
    }
    delay(50);
}
