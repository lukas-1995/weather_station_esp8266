#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"
#define BMP280_I2C_ADDRESS 0x76
#define DHTTYPE DHT22 
Adafruit_BMP280 bmp280;
DHT dht;
const char* ssid     = "WLAN Kunterbunt";//Replace with Network SSID
const char* password = "e@L7r6cmda##Mj9NX^L4Gi5Q#8HhhS";//Replace with Network Password
unsigned long ch_no = 2019428;//Replace with Thingspeak Channel no.
const char * write_api = "CJZHPF5QT3S9ZIYM";
WiFiClient  client;
void setup(void) 
{
  Serial.begin(9600);
  delay(1000);
  Wire.begin(4, 0);
  dht.setup(D1);
  if ( bmp280.begin(BMP280_I2C_ADDRESS) == 0 )
  {
    Serial.println("error");
    while (1)
      delay(1000);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}
void loop()
{
  float temp     = bmp280.readTemperature();
  float temp2     = dht.getTemperature();
  float pressure = bmp280.readPressure();
  float hpa = pressure / 100;
  delay(dht.getMinimumSamplingPeriod());
  int humidity = dht.getHumidity();
  int rain = analogRead(A0);
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hpa);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, rain);
  ThingSpeak.setField(5, temp2);
  ThingSpeak.writeFields(ch_no, write_api);
  delay(10000);
}