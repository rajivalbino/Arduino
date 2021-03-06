/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/151ec4be-5c5f-47b2-9225-e383f05732e5 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float humidity;
  float temperature;
  int light;
  float pressure;
  String weather_report;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
  
#include "thingProperties.h"
#include <Arduino_MKRIoTCarrier.h>
#include <RTCZero.h>

MKRIoTCarrier carrier;
RTCZero alarm;

enum modes{TEMPERATURE, HUMIDITY, LIGHT, PRESSURE, DISPLAY_OFF};
int refreshCount = 0;
 
void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(1000);
 
  // Defined in thingProperties.h
  initProperties();
 
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //Get Cloud Info/errors , 0 (only errors) up to 4
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
 
  //Wait to get cloud connection to init the carrier
  while (ArduinoCloud.connected() != 1) {
    ArduinoCloud.update();
    delay(500);
  }
  delay(500);

  // Init carrier
  CARRIER_CASE = true;
  carrier.begin();
  carrier.display.setRotation(0);

  // Print when successfully connected
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.setCursor(70, 80);
  carrier.display.print("Connected");
  carrier.display.setCursor(50, 110);
  carrier.display.print("To IoT Cloud");
  delay(1000);
  
  carrier.display.setCursor(60, 80);
  carrier.display.print("Weather");
  carrier.display.setCursor(60, 120);
  carrier.display.print("Station");
  delay(1000);
}
 
void loop() {
  ArduinoCloud.update();
  carrier.Buttons.update();

  // Readings
  while(!carrier.Light.colorAvailable()) {
    delay(5);
  }
  int _;
  carrier.Light.readColor(_, _, _, light);
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  pressure = carrier.Pressure.readPressure();
  
  static modes modeSelect = TEMPERATURE;
  static modes previousMode = TEMPERATURE;
  
  if(carrier.Button0.onTouchDown()) {
    modeSelect = TEMPERATURE;
  }
  else if(carrier.Button1.onTouchDown()) {
    modeSelect = HUMIDITY;
  }
  else if(carrier.Button2.onTouchDown()) {
    modeSelect = LIGHT;
  }
  else if(carrier.Button3.onTouchDown()) {
    modeSelect = PRESSURE;
  }
  else if(carrier.Button4.onTouchDown()) {
    modeSelect = DISPLAY_OFF;
  }
   
  if(modeSelect != previousMode) {
    updateDisplay(modeSelect);
    previousMode = modeSelect;
    refreshCount = 0;
  }
  else if (refreshCount >= 50) {
    updateDisplay(modeSelect);
    refreshCount = 0;
  }
 
  if (humidity >= 60 && temperature >= 15) {
    weather_report = "It is very humid outside";
  }
  else if (temperature >= 25 && light >= 700) {
    weather_report = "Hot and sunny outside";
  }
  else if (light <= 100) {
    weather_report = "It is dark outside";
  }
  else if (temperature >= 15 && light >= 700) {
    weather_report = "Warm and sunny outside";
  }
  else if (temperature <= 16 && light >= 700) {
    weather_report = "A little cold, but sunny outside";
  }
  else if (humidity >= 90 && temperature <= 15) {
    weather_report = "It is wet and rainy outside";
  }
  refreshCount++;
}

void updateDisplay (modes select) {
  switch (select) {
    case TEMPERATURE:
      carrier.display.fillScreen(ST77XX_RED);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(4);
      
      carrier.display.setCursor(70, 50);
      carrier.display.print("Temp:");
      carrier.display.setCursor(40, 110);
      carrier.display.print(temperature);
      carrier.display.print(" C");
      break;
    
    case HUMIDITY:
      carrier.display.fillScreen(ST77XX_BLUE);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(4);
      
      carrier.display.setCursor(70, 50);
      carrier.display.print("Humi:");
      carrier.display.setCursor(40, 110);
      carrier.display.print(humidity);
      carrier.display.print(" %");
      break;
    
    case LIGHT:
      carrier.display.fillScreen(ST77XX_GREEN);
      carrier.display.setTextColor(ST77XX_BLACK);
      carrier.display.setTextSize(4);
      
      carrier.display.setCursor(60, 60);
      carrier.display.print("Light:");
      carrier.display.setCursor(80, 120);
      carrier.display.print(light);
      break;
    
    case PRESSURE:
      carrier.display.fillScreen(ST77XX_CYAN);
      carrier.display.setTextColor(ST77XX_BLACK);
      carrier.display.setTextSize(4);
      
      carrier.display.setCursor(60, 60);
      carrier.display.print("Press:");
      carrier.display.setCursor(20, 110);
      carrier.display.print(pressure);
      carrier.display.print(" Pa");
      break;
    
    case DISPLAY_OFF:
      carrier.display.fillScreen(ST77XX_BLACK);
      break;
  }
}
