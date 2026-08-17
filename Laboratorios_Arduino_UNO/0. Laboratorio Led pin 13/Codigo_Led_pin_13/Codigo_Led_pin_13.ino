void setup() {
  pinMode(13, OUTPUT);      // pin 13 como salida
}
void loop(){
  digitalWrite(13, HIGH);   // pin 13 a nivel alto (1)
  delay(1200);              // demora de 1 seg.
  digitalWrite(13, LOW);    // pin 13 a nivel bajo (0)
  delay(1200);              // demora de 1 seg.
}
