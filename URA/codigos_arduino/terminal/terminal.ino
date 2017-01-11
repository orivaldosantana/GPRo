#include "Ultrasonic.h"
#include "DCMotor.h"

const char C_LED = 'l';
const char C_ULTRASONIC = 'u'; 
const char C_DCMOTOR = 'm'; 

#define trigPin 12
#define echoPin 11

int state = 0; 
const int sizeB = 3; 
unsigned char commandName, commandParameter, buffer[sizeB];
bool ledOn; 

Ultrasonic ultrasonic(trigPin,echoPin); 
DCMotor dcMotor(5,4,3,2);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  ledOn = false;
}

void loop() {
  for (int i = 0; i < sizeB; i++) {
    buffer[i] = '#'; // inicialização 
  }
  delay(20);
  if (Serial.available() > 0) {
    
    Serial.readBytesUntil(';', buffer, sizeB);
    if (buffer[1] != '#' && buffer[2] == '#')  {
      commandName = buffer[0];
      commandParameter = buffer[1]; 
      /* Bloco de Código para modificar o estado do LED 13 */
      if (commandName == C_LED) {
        if (commandParameter == '1') {
          // comando recebido com sucesso c1;
          Serial.write("l1;");
          ledOn = true; 
        }
        else if (commandParameter == '0') {
          // comando recebido com sucesso c1; 
          Serial.write("l0;");
          ledOn = false;                       
        }
        else {
             // parametro inválido p0 - LED; 
             Serial.write("p0;"); 
        }
      }
      /* Bloco de código para realizar leitura do Ultrasom */
      else if (commandName == C_ULTRASONIC) {
        if (commandParameter == '0') { // ultrasom na posição 0 
          unsigned int distance = ultrasonic.getDistance();
          String resp = "ux;"; 
          resp[1] = char(distance);
          Serial.write(resp.c_str());
        }
        else {
             // parametro inválido p1 - Ultrasom; 
             Serial.write("p1;");
        }
      }
      /* Bloco de código para controlar o motor */
      else if (commandName == C_DCMOTOR) {
        if (commandParameter == '0') {
          dcMotor.stop();
          Serial.write("m0;");
        }
        else if (commandParameter == '1') {
          dcMotor.backward();
          Serial.write("m1;");
        }
        else if (commandParameter == '2') {
          dcMotor.left();
          Serial.write("m2;");
        }
        else if (commandParameter == '3') {
          dcMotor.right();
          Serial.write("m3;"); 
        }
        else if (commandParameter == '4') {
          dcMotor.forward(); 
          Serial.write("m4;");
        }
        else {
          // parametro inválido p2 - Motor DC; 
          Serial.write("p2;");
        }
      }
      /* Bloco de código para controlar o motor */
      else {
        Serial.write("c1;"); // comando não reconhecido  
      }
    }
    /* Comando não reconhecido */
    else {
      // comando inválido c0; 
      Serial.write("c0;"); 
    }
  }
  
  if (ledOn) {
    digitalWrite(LED_BUILTIN, HIGH); 
  }
  else {
    digitalWrite(LED_BUILTIN, LOW); 
  }
  
}
