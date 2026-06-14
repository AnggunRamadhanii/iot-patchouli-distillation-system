#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

const int ONE_WIRE_BUS = 5;      
const int WATER_LEVEL_PIN = 34;  
const int HEATER_PIN = 16;       
const int PUMP_PIN = 17;         
const int BUZZER_PIN = 18;       
const int START_BUTTON_PIN = 27; 
const int LED_HEATER_PIN = 25;   
const int LED_PUMP_PIN = 26;     

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const float TARGET_TEMP = 100.0;
const float HYSTERESIS_RANGE = 1.0;
const float TEMP_MIN = TARGET_TEMP - HYSTERESIS_RANGE; 
const float TEMP_MAX = TARGET_TEMP + HYSTERESIS_RANGE; 
const float ALARM_TEMP = 105.0;
const int LOW_WATER_THRESHOLD = 500;
const unsigned long PROCESS_DURATION_MS = 4 * 60 * 1000;

bool processActive = false;
unsigned long startTime = 0;
unsigned long previousMillisLCD = 0;
const long intervalLCD = 2000; 


void stopProcess() {
    processActive = false;
    digitalWrite(HEATER_PIN, LOW);
    digitalWrite(PUMP_PIN, LOW);
    digitalWrite(LED_HEATER_PIN, LOW);
    digitalWrite(LED_PUMP_PIN, LOW);
}

void checkManualButton() {
    if (digitalRead(START_BUTTON_PIN) == LOW) {
        delay(50);
        if (digitalRead(START_BUTTON_PIN) == LOW) {
            if (processActive) {
                stopProcess();
            } else {
                startTime = millis();
                processActive = true;
            }
            while (digitalRead(START_BUTTON_PIN) == LOW) {}
        }
    }
}

void processProcessTimer() {
    if (!processActive) {
        return;
    }

    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime >= PROCESS_DURATION_MS) {
        stopProcess();
        lcd.clear();
        lcd.print("PROCESS DONE!");
        lcd.setCursor(0, 1);
        lcd.print("MINYAK SIAP");
    }
}

void checkSensorsAndControl() {
    sensors.requestTemperatures();
    float currentTemp = sensors.getTempCByIndex(0);
    int waterLevelAnalog = analogRead(WATER_LEVEL_PIN);
    bool waterLevelIsLow = (waterLevelAnalog < LOW_WATER_THRESHOLD);

    if (waterLevelIsLow || currentTemp >= ALARM_TEMP) {
        stopProcess(); 

        tone(BUZZER_PIN, 1500);
        lcd.clear();
        lcd.print("EMERGENCY SHUTDOWN!");
        lcd.setCursor(0, 1);
        lcd.print(waterLevelIsLow ? "WATER LOW" : "OVERHEAT"); 
        
        delay(500); noTone(BUZZER_PIN); delay(500); 
        return;
    }
    
    if (!processActive) {
        noTone(BUZZER_PIN);
    }
    
    if (processActive) {
        noTone(BUZZER_PIN);

        if (currentTemp <= TEMP_MIN) {
            digitalWrite(HEATER_PIN, HIGH); 
        } else if (currentTemp >= TEMP_MAX) {
            digitalWrite(HEATER_PIN, LOW);  
        }
        
        digitalWrite(PUMP_PIN, HIGH);
    } 

    digitalWrite(LED_HEATER_PIN, digitalRead(HEATER_PIN));
    digitalWrite(LED_PUMP_PIN, digitalRead(PUMP_PIN));
}

void updateLCD() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisLCD < intervalLCD) {
        return; 
    }
    previousMillisLCD = currentMillis;

    sensors.requestTemperatures();
    float currentTemp = sensors.getTempCByIndex(0);
    int heaterStatus = digitalRead(HEATER_PIN);
    int waterLevelAnalog = analogRead(WATER_LEVEL_PIN);
    
    lcd.clear();
    
    if (processActive) {
        lcd.setCursor(0, 0);
        lcd.print("T:");
        lcd.print(currentTemp, 1);
        lcd.print((char)223); lcd.print("C RUN"); 

        unsigned long elapsedTime = millis() - startTime;
        unsigned long timeLeftMs = PROCESS_DURATION_MS - elapsedTime;
        int minutes = (timeLeftMs % 3600000) / 60000;
        int seconds = (timeLeftMs % 60000) / 1000;
        char timeBuffer[10];
        sprintf(timeBuffer, "%02d:%02d m", minutes, seconds);

        lcd.setCursor(0, 1);
        lcd.print("H:");
        lcd.print(heaterStatus == HIGH ? "ON" : "OFF");
        lcd.print(" TIME:");
        lcd.print(timeBuffer);

    } else {
        lcd.setCursor(0, 0);
        lcd.print("SYSTEM READY");
        lcd.setCursor(0, 1);
        lcd.print("Press START");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(HEATER_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_HEATER_PIN, OUTPUT);
    pinMode(LED_PUMP_PIN, OUTPUT);

    stopProcess(); 
    noTone(BUZZER_PIN);

    pinMode(START_BUTTON_PIN, INPUT_PULLUP);
    
    sensors.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Standalone Mode");
    lcd.setCursor(0, 1);
    lcd.print("System Loading...");
    delay(2000);
}

void loop() {
    checkManualButton();         
    checkSensorsAndControl();    
    processProcessTimer();      
    updateLCD();                 
}