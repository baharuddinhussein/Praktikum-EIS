#include <NewPing.h>

#define TRIGGER_PIN 11
#define ECHO_PIN 12

#define MAX_DISTANCE 200

NewPing sonar (TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
delay (1000);

unsigned int distance = sonar.ping_cm();

Serial.print ("Jarak = ");
Serial.print (distance);
Serial.println (" cm");
}
