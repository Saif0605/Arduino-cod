#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// LDR is connected to GPIO 34 (Analog ADC1_CH6) 
const int LDR_Pin = 34;

// variable for storing the LDR value
int LDR_Value = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
}
void loop()
{
  LDR_Value = analogRead(LDR_Pin);
  lcd.setCursor(0,0);
  lcd.print("Light Intensity:");
  lcd.setCursor(0,1);
  lcd.print(LDR_Value);
  delay(1000);
  lcd.clear();
}
