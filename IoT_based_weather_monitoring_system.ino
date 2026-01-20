#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#define DHTPIN 0 //DHT11sensor pin
#define DHTTYPE DHT11 //DHT11 sensortype

const char *ssid = "SSID";
const char *password = "PASSWORD";
const char *thingSpeakApiKey = "6WJHNQK1T846YR04";
const char *thingSpeakAddress = "api.thingspeak.com";
const String apiUri = "/update";

Adafruit_BMP085 bmp;
DHTdht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
Serial.begin(115200);
connectToWiFi();
dht.begin();
}
if (!bmp.begin()) {
Serial.println("Could not find BMP180 sensor, check wiring!");
while (1);
}
void loop() {
float pressure = bmp.readPressure() / 100.0; // Pressure in hPa
int rain = analogRead(A0);
rain = map(rain, 0, 1024, 0, 100); // Convert analog reading to percentage
float temperature = dht.readTemperature(); // Read temperature from DHT11
float humidity = dht.readHumidity(); // Read humidity from DHT11 sensor

Serial.print("Pressure: ");
Serial.print(pressure);
Serial.println(" hPa");
Serial.print("Rain: ");
Serial.print(rain);
Serial.println("%");
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println(" °C");
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.println("%");
sendToThingSpeak(pressure, rain, temperature, humidity);
delay(20000); // 20 seconds delay between updates
}
void connectToWiFi() {
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
}
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
void sendToThingSpeak(float pressure, int rain, float temperature, float
humidity) {
if (client.connect(thingSpeakAddress, 80)) {
String postString = "api_key=";
postString += thingSpeakApiKey;
postString += "&field1=";
postString += String(pressure);
postString += "&field2=";
postString += String(rain);
postString += "&field3=";
postString += String(temperature);
postString += "&field4=";
postString += String(humidity);
client.print("POST ");
client.print(apiUri);
client.println(" HTTP/1.1");
client.println("Host: api.thingspeak.com");
client.println("Connection: close");
client.println("Content-Type: application/x-www-formurlencoded");
client.println("User-Agent: ESP8266");
client.print("Content-Length: ");
client.println(postString.length());
client.println();
client.print(postString);
Serial.println("Data sent to ThingSpeak");
} else {
Serial.println("Failed to connect to ThingSpeak");
}
//client.stop();
}
