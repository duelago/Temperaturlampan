
void song(int buzzerPin){
  
  tone(buzzerPin, 330);
  delay(602);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(365);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(434);
  noTone(buzzerPin);

  tone(buzzerPin, 220);
  delay(255);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(203);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(203);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(226);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(781);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(185);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(226);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(243);
  noTone(buzzerPin);

  tone(buzzerPin, 330);
  delay(249);
  noTone(buzzerPin);

  tone(buzzerPin, 370);
  delay(336);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(625);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(203);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(237);
  noTone(buzzerPin);

  tone(buzzerPin, 294);
  delay(220);
  noTone(buzzerPin);

  tone(buzzerPin, 277);
  delay(214);
  noTone(buzzerPin);

  tone(buzzerPin, 247);
  delay(1169);
  noTone(buzzerPin);

 

  
}

void setup() {
  // put your setup code here, to run once:
  // call the song function with digital pin
  song(14);
}

void loop() {
  // put your main code here, to run repeatedly:
}
