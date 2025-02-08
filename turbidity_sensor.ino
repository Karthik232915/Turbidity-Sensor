#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change to 0x3F if needed

// Define the OneWire bus
#define ONE_WIRE_BUS 2  // Use the correct pin for your sensor
OneWire oneWire(ONE_WIRE_BUS);

// Initialize DallasTemperature
DallasTemperature suhu(&oneWire);

// Define Turbidity Sensor Pin and LEDs
#define TURBIDITY_SENSOR A0
#define CLEAR_LED 3  // Green LED
#define CLOUDY_LED 4
#define DIRTY_LED 5  // Red LED

int turbidity = 50; // Initialize turbidity value

void setup() {
    Serial.begin(9600);  
    lcd.begin(16, 2);    // Set the LCD to 16 columns and 2 rows
    lcd.backlight();     

    pinMode(CLEAR_LED, OUTPUT);
    pinMode(CLOUDY_LED, OUTPUT);
    pinMode(DIRTY_LED, OUTPUT);

    suhu.begin();  // Initialize the DS18B20 sensor

    Serial.println("System Initialized!");
}

void loop() {
    int sensorValue = analogRead(TURBIDITY_SENSOR);
    int rawTurbidity = map(sensorValue, 0, 640, 0, 100);  // Convert to 0-100%

    // Debugging output
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print(" | Turbidity: ");
    Serial.print(rawTurbidity);
    Serial.println("%");

    // If the light is green (clear water), increase turbidity value
    if (digitalRead(CLEAR_LED) == HIGH) {
        turbidity = min(turbidity + 1, 100); // Increase turbidity but max out at 100%
        Serial.println("Green Light Detected: Increasing Turbidity");
    }
    // If the light is red (dirty water), decrease turbidity value
    else if (digitalRead(DIRTY_LED) == HIGH) {
        turbidity = max(turbidity - 1, 0); // Decrease turbidity but not below 0
        Serial.println("Red Light Detected: Decreasing Turbidity");
    } else {
        turbidity = rawTurbidity; // Use the raw turbidity value from the sensor if no light detected
    }

    // Display Turbidity on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turbidity: ");
    lcd.print(turbidity);
    lcd.print("%  ");

    // Updated Ranges for Water Quality
    if (turbidity <= 30) {  
        digitalWrite(CLEAR_LED, HIGH);
        digitalWrite(CLOUDY_LED, LOW);
        digitalWrite(DIRTY_LED, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Water is CLEAR ");
    } 
    else if (turbidity >= 31 && turbidity <= 75) {  
        digitalWrite(CLEAR_LED, LOW);
        digitalWrite(CLOUDY_LED, HIGH);
        digitalWrite(DIRTY_LED, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Water is CLOUDY ");
    } 
    else {  
        digitalWrite(CLEAR_LED, LOW);
        digitalWrite(CLOUDY_LED, LOW);
        digitalWrite(DIRTY_LED, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Water is DIRTY  ");
    }

    suhu.requestTemperatures();
    float celcius = suhu.getTempCByIndex(0);  // Get temperature of the first sensor
    Serial.print("Temperature: ");
    Serial.print(celcius);
    Serial.println(" °C");

    delay(1000);
}








