int relay_pin = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(relay_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relay_pin, HIGH);
  delay(2000);
  digitalWrite(relay_pin, LOW);
  delay(2000);
}
