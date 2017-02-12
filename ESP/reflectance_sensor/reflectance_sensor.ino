#define REFLECTANCE_SENSOR D8
#define LED D15

void setup() {
  pinMode(REFLECTANCE_SENSOR,INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200); 
  Serial.println("\n\nESP8266 ligado.\n\n");
}

void loop() {
 if ( digitalRead(REFLECTANCE_SENSOR) == 0 ) {
    digitalWrite(LED, HIGH);
    Serial.println("... HIGH ... ");
 }
 else {
    digitalWrite(LED, LOW);
    Serial.println("... LOW ...");
 }
 delay(100); 
}
