#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Updated for 20x4 LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;

#define ir_enter 2
#define ir_back  4

#define ir_car1 5
#define ir_car2 6
#define ir_car3 7
#define ir_car4 8

int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int flag1 = 0, flag2 = 0;
int slot = 4;

void setup() {
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);

  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(3);
  myservo.write(90);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("  HI WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("     IT PROJECT");
  lcd.setCursor(0, 2);
  lcd.print("     CAR PARK");
  lcd.setCursor(0, 3);
  lcd.print("     SYSTEM");
  delay(5000);
  lcd.clear();

  Read_Sensor();
  int total = S1 + S2 + S3 + S4;
  slot = slot - total;
}

void loop() {
  Read_Sensor();

  lcd.setCursor(0, 0);
  lcd.print("Free Slots: ");
  lcd.print(slot);
  lcd.print("    "); // clear residue

  // Use extra lines for better layout
  lcd.setCursor(0, 1);
  lcd.print("1:");
  lcd.print(S1 ? "Full" : "Empty");
  lcd.setCursor(10, 1);
  lcd.print("2:");
  lcd.print(S2 ? "Full" : "Empty");

  lcd.setCursor(0, 2);
  lcd.print("3:");
  lcd.print(S3 ? "Full" : "Empty");
  lcd.setCursor(10, 2);
  lcd.print("4:");
  lcd.print(S4 ? "Full" : "Empty");

  if (digitalRead(ir_enter) == 0 && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180);
        slot--;
      }
    } else {
      lcd.setCursor(0, 3);
      lcd.print(" Parking is Full   ");
      delay(1500);
    }
  }

  if (digitalRead(ir_back) == 0 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180);
      slot++;
    }
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(90);
    flag1 = 0;
    flag2 = 0;
  }

  delay(100);
}

void Read_Sensor() {
  S1 = digitalRead(ir_car1) == 0 ? 1 : 0;
  S2 = digitalRead(ir_car2) == 0 ? 1 : 0;
  S3 = digitalRead(ir_car3) == 0 ? 1 : 0;
  S4 = digitalRead(ir_car4) == 0 ? 1 : 0;
}
