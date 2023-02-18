#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <BH1750.h>
#include <SimpleDHT.h>
#define BMP280_I2C_ADDRESS 0x76
Adafruit_BMP280 bmp280;
int pinDHT22 = 5;
SimpleDHT22 dht22(pinDHT22);
BH1750 lightMeter;
const int rainDigitalPin = 14;
const int rainAnalogPin = A0;
const int rainSensorPowerPin = 12;
const int soundSensor = 13; 
const char* ssid     = "WLAN Kunterbunt";//Replace with Network SSID
const char* password = "e@L7r6cmda##Mj9NX^L4Gi5Q#8HhhS";//Replace with Network Password
unsigned long ch_no = 2019428;//Replace with Thingspeak Channel no.
const char * write_api = "CJZHPF5QT3S9ZIYM";
WiFiClient  client;
void setup(void) 
{
  Serial.begin(9600);
  delay(1000);
  Wire.begin(4,0);
  Serial.println(bmp280.begin(BMP280_I2C_ADDRESS));
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
  lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  pinMode(rainDigitalPin, INPUT);
  pinMode(rainAnalogPin, INPUT);
  pinMode(rainSensorPowerPin, OUTPUT);
  pinMode(soundSensor,INPUT);
  bmp280.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_OFF,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_4000); /* Standby time. */
}
void loop()
{
  digitalWrite(rainSensorPowerPin, HIGH); // Schaltet den Strom für den Sensor ein
  delay(100);
  int rainDigital = digitalRead(rainDigitalPin);
  int rainAnalog = analogRead(rainAnalogPin);
  
  Serial.print("Analog Value: ");
  Serial.print(rainAnalog);
  Serial.print("\t Digital Value: "); 
  Serial.println(rainDigital);            // 0 = Regen / 1 = kein Regen

  digitalWrite(rainSensorPowerPin, LOW);  // Schaltet den Strom für den Sensor aus
  float sound=digitalRead(soundSensor);
  bmp280.takeForcedMeasurement();
  float temp     = bmp280.readTemperature();
  float pressure = bmp280.readPressure();
  float hpa = pressure / 100;
  float temperature = 0;
  float humidity = 0;
  int err = dht22.read2(&temperature, &humidity, NULL);
  float temp2 = (float)temperature;
  float humid = (float)humidity;
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hpa);
  ThingSpeak.setField(3, humid);
  ThingSpeak.setField(4, rainAnalog);
  ThingSpeak.setField(5, temp2);
  ThingSpeak.setField(6, lux);
  ThingSpeak.writeFields(ch_no, write_api);
  Serial.println(temp);
  Serial.println(hpa);
  Serial.println(humid);
  Serial.println(temp2);
  Serial.println(rainAnalog);
  Serial.println(sound);
  Serial.println("Next Point");  
  delay(10000);
}