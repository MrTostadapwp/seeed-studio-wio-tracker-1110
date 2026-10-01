import serial
import tkinter as tk
import tkintermapview

# --- CONFIGURACIÓN ---
PUERTO_COM = 'COM8'  # <--- CAMBIA ESTO AL PUERTO DE TU RECEPTOR
BAUDIOS = 115200

# --- CREAR LA VENTANA ---
ventana = tk.Tk()
ventana.geometry("800x600")
ventana.title("Rastreador LoRa - Wio Tracker 1110")

# --- CREAR EL MAPA ---
mapa = tkintermapview.TkinterMapView(ventana, width=800, height=600, corner_radius=0)
mapa.pack(fill="both", expand=True)
mapa.set_zoom(16) # Nivel de zoom inicial

marcador = None

# --- CONECTAR AL ARDUINO ---
try:
    puerto_serie = serial.Serial(PUERTO_COM, BAUDIOS, timeout=1)
    print(f"Escuchando en {PUERTO_COM}...")
except Exception as e:
    print(f"Error conectando al puerto: {e}")
    puerto_serie = None

# --- FUNCIÓN DE LECTURA ---
def leer_datos_lora():
    global marcador
    if puerto_serie and puerto_serie.in_waiting > 0:
        try:
            # Leemos la línea que llega del Arduino
            linea = puerto_serie.readline().decode('utf-8').strip()
            
            # Si la línea empieza con "DATA:", extraemos las coordenadas
            if linea.startswith("DATA:"):
                datos = linea.replace("DATA:", "").split(",")
                if len(datos) == 2:
                    lat = float(datos[0])
                    lon = float(datos[1])
                    print(f"Ubicación recibida -> Lat: {lat}, Lon: {lon}")
                    
                    # Centrar el mapa en la nueva coordenada
                    mapa.set_position(lat, lon)
                    
                    # Poner o mover el marcador
                    if marcador is None:
                        marcador = mapa.set_marker(lat, lon, text="Wio Tracker GPS")
                    else:
                        marcador.set_position(lat, lon)
        except Exception as e:
            pass # Ignoramos errores de lectura si llegan datos incompletos

    # Esta línea hace que la función se vuelva a ejecutar a sí misma cada 100 milisegundos
    ventana.after(100, leer_datos_lora)

# --- INICIAR EL PROGRAMA ---
# Asegúrate de CERRAR el Monitor Serie de Arduino IDE antes de correr esto
ventana.after(100, leer_datos_lora)
ventana.mainloop()
