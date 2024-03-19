#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
Servo myservo;  
int led = 2;
int buzzer = 3;
int servo = 9;
int pos = 0; 
int estado = 0; //0: bajo, 1: intermedio, 2: alto.
int angulo = 120;
int divisiones = 10;
int tempEstado = 0;
LiquidCrystal_I2C lcd(0x27,20,4);
const int Trigger = 11;  
const int Echo = 10;   

void setup() {
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  myservo.attach(servo);
  myservo.write(pos);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("DISTANCIA:");
  lcd.setCursor(0,1);
  lcd.print("ESTADO:  _____");
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT); 
  digitalWrite(Trigger, LOW);
}

void loop() {
  lcd.setCursor(10,0);
  lcd.print("_");
  long t; 
  long d; 
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH); 
  d = t/59;             
  lcd.setCursor(10,0);
  lcd.print(d);
  lcd.print("cm");
  lcd.setCursor(7,1);
  if (d <= 5){
    tempEstado = 2;
    lcd.print("  LLENO");
  } else if (d > 5 && d <= 10) {
    tempEstado = 1;
    lcd.print("  MEDIA");
  } else {
    tempEstado = 0;
    lcd.print("  VACIO");
  }
  if (tempEstado != estado) {
    if (estado <= 1 && tempEstado == 2) {
        for(int i = 0; i < divisiones; i++) {
          myservo.write((angulo/divisiones)*(i+1));
          delay(200);
        }
        myservo.write(0);
        delay(2000);
        digitalWrite(Trigger, HIGH);
        delayMicroseconds(10);          
        digitalWrite(Trigger, LOW);
        t = pulseIn(Echo, HIGH); 
        d = t/59;
        if (d > 5) {
          tempEstado = 1;
        } else {
          digitalWrite(led,HIGH);
          digitalWrite(buzzer,HIGH);
          delay(200);
          digitalWrite(buzzer,LOW);
          delay(200);
          digitalWrite(buzzer,HIGH);
          delay(200);
          digitalWrite(buzzer,LOW);
          delay(200);
          digitalWrite(buzzer,HIGH);
          delay(200);
          digitalWrite(buzzer,LOW);
        }
    }
    if (estado = 2 && tempEstado < 2) {
      digitalWrite(led,LOW);
    }
    estado = tempEstado;
  }
  delay(2000);
}
