#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// WiFi AP SSID
#define WIFI_SSID "realme C65"
// WiFi password
#define WIFI_PASSWORD "badhon65"

#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "17QnxuVIIDsEVvJjbZcmbZu_hus3AB-mJ4tb4diCiRGIZu7Fr0OlNiGHLozLQU0sgcTEPBLGui1CnBuwA7kM1w=="
#define INFLUXDB_ORG "ac168798e497963f"
#define INFLUXDB_BUCKET "ESP32_Solar"

// Time zone info
#define TZ_INFO "UTC6"

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensorReadings("measurements");

// LDR is connected to GPIO 34 (Analog ADC1_CH6)
const int LDR_Pin = 34;
// PV is connected to GPIO 35 (Analog ADC1_CH7)
const int PV_Pin = 35;
// variable for storing the LDR value
int LDR_Value;
// variable for storing the LDR value
float PV_Value;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  // Get latest sensor readings
  LDR_Value = analogRead(LDR_Pin);
  PV_Value = ((analogRead(PV_Pin)) / 4096.0) * 3.3 * 13.33;

  lcd.setCursor(0, 0);
  lcd.print("L/I:");
  lcd.setCursor(4, 0);
  lcd.print(LDR_Value);
  lcd.setCursor(0, 1);
  lcd.print("PV:");
  lcd.setCursor(4, 1);
  lcd.print(PV_Value);

  // Add readings as fields to point
  sensorReadings.addField("LDR_Value", LDR_Value);
  sensorReadings.addField("PV_Value", PV_Value);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensorReadings));

  // Write point into buffer
  client.writePoint(sensorReadings);

  // Clear fields for next usage. Tags remain the same.
  sensorReadings.clearFields();

  // If no Wifi signal, try to reconnect it
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }

  // Wait 10s
  Serial.println("Wait 1s");
  delay(1000);
  lcd.clear();
}
