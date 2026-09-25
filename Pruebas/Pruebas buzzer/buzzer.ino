#include <Adafruit_TinyUSB.h> // Para comunicación Serial
#include <Wire.h>
#include <LIS3DHTR.h>

#define WIRE Wire
LIS3DHTR<TwoWire> LIS; 

// Define el pin donde conectaste el buzzer activo
const int pinBuzzer = 1; // <-- Cambia esto si usaste otro puerto D

void setup() 
{
    delay(100); 

    Serial.begin(115200);
    while (!Serial) delay(100);   

    // Configurar el pin del buzzer como salida y asegurar que inicie apagado
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, LOW);

    // Inicializa LIS3DHTR
    LIS.begin(WIRE, 0x19); 

    if (!LIS) {
        Serial.println("LIS3DHTR no conectó.");
        while (1);
    }
    
    Serial.println("Sistema iniciado. Esperando detectar caída...");
}

void loop()
{
    // Leer los valores de los 3 ejes
    float x = LIS.getAccelerationX();
    float y = LIS.getAccelerationY();
    float z = LIS.getAccelerationZ();

    // Imprimir para monitoreo
    Serial.print("x:"); Serial.print(x); Serial.print("  ");
    Serial.print("y:"); Serial.print(y); Serial.print("  ");
    Serial.print("z:"); Serial.println(z);

    // Lógica de Caída Libre:
    // Si los 3 ejes están muy cerca de 0g al mismo tiempo, el módulo está cayendo.
    // Usamos un umbral de 0.3g para evitar falsos positivos por el ruido del sensor.
    if (abs(x) < 0.3 && abs(y) < 0.3 && abs(z) < 0.3) {
        Serial.println("¡ALERTA: CAÍDA LIBRE DETECTADA!");
        
        // Activar la alarma (Buzzer activo)
        digitalWrite(pinBuzzer, HIGH);
        
        // Mantener la alarma encendida por 2 segundos
        delay(2000); 
        
        // Apagar la alarma
        digitalWrite(pinBuzzer, LOW);
        Serial.println("Alarma apagada, reanudando monitoreo...");
    }

    delay(100); // Pausa de 100ms entre lecturas
}