#include <Ultrasonic.h>

#define TRIGGER_PIN    12
#define ECHO_PIN       13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

const int IN1 = 5, IN2 = 6, IN3 = 9, IN4 = 10, vel=100,velmax=150;

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  analogWrite(IN1,0);
  analogWrite(IN3,0);
  Serial.begin(9600); 
}
int loops = 0;
unsigned char S[10];
void loop() {
  float distancia;
  long microsec = ultrasonic.timing();
  distancia = ultrasonic.convert(microsec, Ultrasonic::CM);

  // feature scaling para o dado de entrada do ultrassom
  int ultrassom_pd = (int)distancia;
  ultrassom_pd = constrain(ultrassom_pd, 10, 300);
  
  float ultrassom_normal = (map(ultrassom_pd, 10, 300, 0, 100))/100.0;    // variável de entrada normalizada do ultrassom
                                                                       // entre 0 e 1
  
 if(loops>=1000000/30)
  {
    digitalWrite(IN2,0);
    digitalWrite(IN4,0);
    loops=0;
  }
  Serial.readBytesUntil(';', S, 7);
  if(S[0]=='r'){
      int velDir = map((int)S[1],100,200,vel,velmax);
      int velEsq = vel;

      velDir = (int)((9*velDir+ultrassom_normal)/10);
      velEsq = (int)((9*velEsq+ultrassom_normal)/10);
    
      analogWrite(IN4,velDir);
      analogWrite(IN2,velEsq);
      loops=0;
        Serial.println(velDir);

  }

  
  delayMicroseconds(1);
  loops++;





  
}
