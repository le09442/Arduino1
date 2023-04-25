#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <avr/wdt.h>  
LiquidCrystal_I2C lcd(0x27, 16, 2);
int water_leve ;
int soil_moisture;
long duration, cm;
int delays=1500;
int sum_cm=0,count_cm=0,per_cm=0;
int show_rbg=1,shom_moisture=0;
int halh=1;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.noBlink();

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);  
  pinMode(13, OUTPUT);  
  pinMode(12, INPUT);   
   // put your setup code here, to run once:

}

void loop() {
  ///////////////////////วัดระดับน้ำ
  water_leve=analogRead(0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print((String("WaterLevel : ") ) );
  lcd.setCursor(0,1);
  lcd.print(String(water_leve));
  if(show_rbg==1)
  {
  if(water_leve > 200){
    analogWrite(9,0);
    analogWrite(10,255);
    analogWrite(11,0);
  }else if(water_leve < 200){
    analogWrite(9,255);
    analogWrite(10,255);
    analogWrite(11,0);  
  
  } else  if(water_leve < 100) {
    analogWrite(9,255);
    analogWrite(10,0);
    analogWrite(11,0);
  }
  }
  delay(delays);
////////////////////วัดความชื่นในดิน
///Serial.println(digitalRead(2));
  soil_moisture = analogRead(1);
  soil_moisture = map(soil_moisture, 0, 1023, 100, 0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String("soil moisture : "));
  lcd.setCursor(0,1);
  lcd.print(String(soil_moisture));
  Serial.println(soil_moisture);
  if(shom_moisture==1){
    soil_moisture=100;
  }
  if(soil_moisture <= 71){
    analogWrite(9,0);
    analogWrite(10,255);
    analogWrite(11,0);
    
    while (soil_moisture <= 71) {
        digitalWrite(2, HIGH);
        soil_moisture = analogRead(1);
        soil_moisture = map(soil_moisture, 0, 1023, 100, 0);
        Serial.println(soil_moisture);
        lcd.clear();
        lcd.print(String("WATERING"));
        lcd.setCursor(0,1);
        lcd.print(String("YOUR PLANTS."));
        //ArduinoReset();
        delay(500);
    }
    if(show_rbg==0){
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("THANK YOU"));
    digitalWrite(2, LOW);
  }
  delay(delays);
  
  /////////////////////ตัวรับระยะทาง

  digitalWrite(13, LOW);
  delayMicroseconds(2);
  digitalWrite(13, HIGH);
  delayMicroseconds(5);
  digitalWrite(13, LOW);
  duration = pulseIn(12, HIGH);
  cm = microsecondsToCentimeters(duration);
  
  sum_cm=sum_cm+cm;
  count_cm+=halh;
  per_cm=sum_cm/count_cm;  

  if(per_cm-10>cm){
    sum_cm=0;
    count_cm=0;
    Serial.println("-------------------------------");
    digitalWrite(3, HIGH);
    LCD_ON(); 
    show_rbg=1;
  }
  if(count_cm/2>=7){
    LCD_OFF();
    sum_cm=0;
    count_cm=0;
    Serial.println("+++++++++++++++++++++++++++++++");
    //digitalWrite(3, LOW);
    digitalWrite(9,  LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    show_rbg=0;
  }
  
  if(cm > 1180 ){
    digitalWrite(2, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("WATERING"));
    lcd.setCursor(0,1);
    lcd.print(String("YOUR PLANTS."));
    delay(750);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("THANK YOU"));
    delay(500);
    digitalWrite(2, LOW);
    
    
  }
  

  /*while (cm > 1180 ) {
    digitalWrite(2, HIGH);
    delay(1000);
    pulseIn(12, LOW);
    digitalWrite(13, LOW);
    delayMicroseconds(2);
    digitalWrite(13, HIGH);
    delayMicroseconds(5);
    digitalWrite(13, LOW);
    duration = pulseIn(12, HIGH);
    cm = microsecondsToCentimeters(duration);
    Serial.println(duration);
    Serial.println();
    //if(per_cm-20<cm){
     // cm = microsecondsToCentimeters(duration);
    //}
    //cm=1181;
  }*/
  
 
}
void LCD_ON() {
  lcd.display(); // เปิดการแสดงตัวอักษร
  lcd.backlight(); // เปิดไฟแบล็กไลค์
}

void LCD_OFF() {
  lcd.noDisplay(); // ปิดการแสดงตัวอักษร
  lcd.noBacklight(); // ปิดไฟแบล็กไลค์
}
long microsecondsToCentimeters(long microseconds)
{
// ความเร็วของเสียงคือ 340 m/s หรือ 29 ไมโครวินาทีต่อเซนติเมตร
// ค่า ping เดินทางไปและกลับ ดังนั้น เพื่อหาระยะทางของ
// วัตถุเราใช้ระยะทางครึ่งหนึ่งของระยะทางที่เดินทาง
return microseconds / 29 / 2;
}
