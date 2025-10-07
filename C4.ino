#include <Bonezegei_LCD1602_I2C.h>
#include <driver/ledc.h>

const char* pwd = "7355608";
const uint8_t buzzer = 2;

Bonezegei_LCD1602_I2C lcd(0x27);

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  
  ledcAttach(buzzer,1500,8);
  ledcWrite(buzzer, 0);
  lcd.begin();
  lcd.print("Aucol");
  lcd.setPosition(0, 1);
  lcd.print("25_09_27");
  delay(2000);

  lcd.clear();
  lcd.setPosition(0, 0);
  lcd.print("Enter code:");
  lcd.setPosition(0, 1);
}

void loop() {
  for (static uint8_t idx = 0;idx < strlen(pwd);idx++) {
    lcd.setPosition(idx, 1);
    lcd.print(pwd[idx]);
    ledcWrite(buzzer,128);
    delay(100);
    ledcWrite(buzzer,0);
    delay(200);                   
  }
}