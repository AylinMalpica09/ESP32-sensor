#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4; 
const char *ssid = "Totalplay-11A5";
const char *password = "11A507FD7cXD5uvf";
String datos;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  // mac custom
  Serial.println();
  WiFi.mode(WIFI_STA);
  /*Serial.print("[OLD] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);
  Serial.print("[NEW] ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());*/

  
  //Conectar wifi 
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  HTTPClient http;
  http.begin("");
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  delay(10000);
   if (WiFi.status() == WL_CONNECTED){
    
    HTTPClient http;

    String code = "1";
    
    datos = String(temperatureC) + "ºC";
    String json;
    StaticJsonDocument<300> doc;
    doc["temperature"]=datos;
    doc["id"]=code;
    serializeJson(doc, json);
    Serial.println(json);
    //Indicamos el destino
    http.begin("http://192.168.100.195:3001/temperature");
    http.addHeader("Content-Type", "application/json");
    
    int status_response = http.POST(json);
    
    if (status_response > 0){
      Serial.println("Código HTTP ► " + String(status_response)); // Print return code

      if (status_response == 200){
        String data_response= http.getString();
        Serial.println("El servidor respondió ▼ ");
        Serial.println(data_response);
      }
    }else{
      String data_response= http.getString();
      Serial.print(" Error enviando POST, código: ");
      Serial.println(status_response);
      Serial.println(data_response);
    }
    http.end(); 
  }
}

void loop() {
  
}
