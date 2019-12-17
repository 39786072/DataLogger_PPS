void setup() {
  // put your setup code here, to run once:
  pinMode(A5,INPUT);
  pinMode(A4,INPUT);
  pinMode(A3,INPUT);
  pinMode(A2,INPUT);
  pinMode(A1,INPUT);
  pinMode(A0,INPUT);
  Serial.begin(230400);
}

void loop() {
  // put your main code here, to run repeatedly:

Serial.print((analogRead(A0)/1024.0)*5.0);
Serial.print('\t');
Serial.print((analogRead(A1)/1024.0)*5.0);
Serial.println('\t');
}
