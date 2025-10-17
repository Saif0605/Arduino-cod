#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Saif,Badhon,Orpl");
  lcd.setCursor(3,1);
  lcd.print("1 2 3 4 5");
}
void loop()
{
}
