

/* TEST - DHT22, ESP32 & MYSQL*/

/*
    install the following Adafruit libraries for the DHT22 sensor

    1º Adafruit unified sensor
    2º DHT Sensor library
*/

/*
    install the following library by Adrian McEwen in order to comunicate with google spreadsheet via HTTP

    1º HTTP client
*/


#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#define DHTPIN 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/*configure the necessary WIFI connection info */
const char* ssid     = "your-SSID";
const char* password = "your-PASSWORD";
const char* host ="";



void setup() 
{
  // Starting the serial monitor

    Serial.begin(115200);

  // Starting the DHT22
    Serial.println("DHT22 Output!");
    dht.begin();


  // connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address:");
    Serial.println(WiFi.localIP());
    }

void loop() 
{
  // reads the information from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();  
  
  if(isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read DHT22");
    delay(2000);
  }
  
  //displays the temperature and humidity
  else
  {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" ºC");
    delay(2000);
  }

    //---------------------------------------------------------------------

    // passing the code's ID (Google Script ID) that will insert the sensors readings into the table
    
    String ScriptId = "Insert-here-your-scriptID";
    
    // passing the table credentials & the colums names
    String urlFinal = "https://script.google.com/macros/s/"+ScriptId+"/exec?"+"temperature="+temperature+"&humidity="+humidity;

    // posting the data
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    
    //---------------------------------------------------------------------
    
    //getting response from google sheet
    String payload;
    if (httpCode > 0)
    {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
}
