#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <WiFi.h>
#include <HTTPClient.h>

#define LCD_WIRE_ONE 6
#define LCD_WIRE_TWO 7

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const char *ssid = "Manticore";
const char *password = "Hba7gT9X2Xmmvu84zjxEC";
String serverName = "http://192.168.1.106:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String input = "Hallo Bubu, ich hab dich ganz doll lieb <3";

void lcd_setup()
{
  Wire.begin(LCD_WIRE_ONE,LCD_WIRE_TWO);
  if(int status = lcd.begin(LCD_COLS, LCD_ROWS))
  {
    Serial.println("ERROR WITH LCD OCCURED");
    hd44780::fatalError(status);
  }
  else{
    Serial.println("LCD UP");
  }
}

void wifi_setup()
{
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println("WIFI READY");
}

void print_lcd_firstline(const String &input)
{
  size_t inputLength = input.length();

  for(size_t i =0; i <= inputLength - LCD_COLS; ++i){
    lcd.setCursor(0,0);
    String segment = input.substring(i, i+16);
    lcd.print(segment);
    delay(500);
  }
  delay(2000);
}

void setup()
{
  Serial.begin(115200);
  while(!Serial){delay(100);}
  lcd_setup();
  wifi_setup();
  lcd.setCursor(0,1);
  lcd.print("Dein Hasi <3");
  //lcd.print(WiFi.localIP());
}

void loop() {
  print_lcd_firstline(input);
}

void network_stuff()
{
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + "?temperature=24.37";
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}