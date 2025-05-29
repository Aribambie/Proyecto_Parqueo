
#include <WiFi.h>              // Manejo de WiFi
#include <Wire.h>              // Comunicación I2C
#include <ArduinoJson.h>      // Manejo de JSON para respuesta HTTP

// === CONFIGURACIÓN WI-FI ===
const char* ssid = "Galaxy A33 5G0F37";
const char* password = "rcgc1744";

WiFiServer server(80);        // Servidor HTTP puerto 80

// === CONFIGURACIÓN I2C ===
#define I2CSlaveAddress1 0x55 // Dirección del STM32
#define I2C_SDA 21
#define I2C_SCL 22

unsigned long lastI2CRead = 0;
const unsigned long i2cInterval = 5000; // Tiempo entre lecturas

int estadoParqueos[8]; // Array con estado de cada parqueo (0: libre, 1: ocupado)

// === SETUP INICIAL ===
void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL); // Inicializar I2C con pines definidos

  for (int i = 0; i < 8; i++) estadoParqueos[i] = 0;

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  Serial.println(WiFi.localIP());
  server.begin();
}

// === LECTURA DE STM32 POR I2C ===
void leerDatosDesdeSTM32() {
  uint8_t sensor = random(99); // Valor aleatorio para prueba
  Wire.beginTransmission(I2CSlaveAddress1);
  Wire.write('S');             // Comando para STM32
  Wire.write('>');             // Separador o instrucción extendida
  Wire.write(sensor);          // Valor de prueba
  uint8_t error = Wire.endTransmission(true);

  Serial.print("endTransmission: ");
  Serial.println(error);

  uint8_t bytesReceived = Wire.requestFrom(I2CSlaveAddress1, 4);
  Serial.print("requestFrom: ");
  Serial.println(bytesReceived);

  if (bytesReceived == 4) {
    for (int i = 0; i < 4; i++) {
      estadoParqueos[i] = Wire.read();
      Serial.printf("P%d = %d\n", i + 1, estadoParqueos[i]);
    }
  } else {
    Serial.println("No data received");
  }
}

// === ENVÍO DE JSON POR HTTP ===
void enviarJSONEstado(WiFiClient& client) {
  StaticJsonDocument<256> doc;
  for (int i = 0; i < 8; i++) {
    doc["P" + String(i + 1)] = estadoParqueos[i];
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Connection: close");
  client.println();
  serializeJson(doc, client);
}

// === LOOP PRINCIPAL ===
void loop() {
  if (millis() - lastI2CRead >= i2cInterval) {
    lastI2CRead = millis();
    leerDatosDesdeSTM32();
  }

  WiFiClient client = server.available();
  if (client) {
    Serial.println("\n[Cliente conectado]");
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    if (req.indexOf("/estado") >= 0) {
      leerDatosDesdeSTM32();  // Refrescar lectura antes de responder
      enviarJSONEstado(client);
    } else {
      enviarPaginaHTML(client); // Servir página web
    }

    delay(10);
    client.stop();
    Serial.println("[Cliente desconectado]");
  }
}

// === PÁGINA HTML COMPLETA ===
void enviarPaginaHTML(WiFiClient& client){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    client.println(R"rawliteral(
<!DOCTYPE html>
<html lang=\"es\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Estado Parkeomatic</title>
    <!-- ESTILOS CSS ATRACTIVOS Y RESPONSIVOS -->
    <style>
        /* Estilos en bloque... */
    </style>
</head>
<body>
    <div class=\"container\">
        <div class=\"header\">
            <h1>Estado Parkeomatic <span class=\"car-icon\">🚗</span></h1>
        </div>

        <div class=\"controls\">
            <button class=\"update-btn\" onclick=\"updateStatus()\">🔄 Actualizar Estado</button>
            <div class=\"auto-update\">⏱️ Actualización automática cada 5 segundos</div>
        </div>

        <div class=\"content\">
            <div class=\"section\">
                <h2 class=\"section-title\">📊 Espacios Disponibles</h2>
                <div class=\"parking-grid\" id=\"available-spots\"></div>
            </div>
            <div class=\"section\">
                <h2 class=\"section-title\">🚙 Estado Completo</h2>
                <div class=\"parking-grid\" id=\"all-spots\"></div>
            </div>
        </div>

        <div class=\"stats\">
            <div class=\"stat-item available-stat\"><div class=\"stat-number\" id=\"available-count\">0</div><div class=\"stat-label\">Disponibles</div></div>
            <div class=\"stat-item occupied-stat\"><div class=\"stat-number\" id=\"occupied-count\">0</div><div class=\"stat-label\">Ocupados</div></div>
            <div class=\"stat-item total-stat\"><div class=\"stat-number\" id=\"total-count\">8</div><div class=\"stat-label\">Total</div></div>
        </div>
    </div>

    <!-- JAVASCRIPT: INTERACCIÓN DINÁMICA -->
    <script>
        let parkingData = {};

        function fetchParkingStatus() {
            fetch('/estado')
                .then(response => response.json())
                .then(data => {
                    parkingData = data;
                    updateInterface();
                })
                .catch(error => console.error('Error fetching data:', error));
        }

        function updateInterface() {
            const availableGrid = document.getElementById('available-spots');
            const allGrid = document.getElementById('all-spots');
            let availableSpots = '', allSpots = '';
            let availableCount = 0, occupiedCount = 0;

            for (let i = 1; i <= 8; i++) {
                const spotKey = 'P' + i;
                const isOccupied = parkingData[spotKey] === 1;
                const status = isOccupied ? 'occupied' : 'available';
                const statusText = isOccupied ? 'Ocupado 🚫' : 'Disponible 🚗';

                if (!isOccupied) {
                    availableCount++;
                    availableSpots += `<div class="parking-spot available"><span class="spot-number">Parqueo ${i}</span><div class="spot-status"><span class="status-indicator"></span>Disponible</div></div>`;
                } else {
                    occupiedCount++;
                }

                allSpots += `<div class="parking-spot ${status}"><span class="spot-number">#${i}</span><div class="spot-status"><span class="status-indicator"></span>${statusText}</div></div>`;
            }

            availableGrid.innerHTML = availableSpots;
            allGrid.innerHTML = allSpots;
            document.getElementById('available-count').textContent = availableCount;
            document.getElementById('occupied-count').textContent = occupiedCount;
        }

        function updateStatus() {
            const btn = document.querySelector('.update-btn');
            const originalText = btn.innerHTML;
            btn.innerHTML = '⏳ Actualizando...';
            btn.disabled = true;

            fetchParkingStatus();

            setTimeout(() => {
                btn.innerHTML = '✅ Actualizado';
                setTimeout(() => {
                    btn.innerHTML = originalText;
                    btn.disabled = false;
                }, 1000);
            }, 1000);
        }

        document.addEventListener('DOMContentLoaded', function() {
            fetchParkingStatus();
            setInterval(fetchParkingStatus, 5000);
        });
    </script>
</body>
</html>
)rawliteral");
}
