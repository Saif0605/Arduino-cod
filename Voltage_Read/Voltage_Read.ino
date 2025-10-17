#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// LDR is connected to GPIO 34 (Analog ADC1_CH6)
const int LDR_Pin = 34;

// PV is connected to GPIO 35 (Analog ADC1_CH7)
const int PV_Pin = 35;

// variable for storing the LDR value
int LDR_Value = 0;

// variable for storing the LDR value
float PV_Value = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}
void loop() {
  LDR_Value = analogRead(LDR_Pin);
  lcd.setCursor(0, 0);
  lcd.print("L/I:");
  lcd.setCursor(4, 0);
  lcd.print(LDR_Value);
  PV_Value = ((analogRead(PV_Pin)) / 4096.0) * 3.3;
  lcd.setCursor(0, 1);
  lcd.print("PV:");
  lcd.setCursor(4, 1);
  lcd.print(PV_Value);
  delay(1000);
  lcd.clear();
}
