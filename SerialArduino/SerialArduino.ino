#include <SoftwareSerial.h>

SoftwareSerial s(6,7);
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
s.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  s.println("Hola");
  digitalWrite(13,HIGH);
  delay(500);

  digitalWrite(13,LOW);
delay(500);
  }
