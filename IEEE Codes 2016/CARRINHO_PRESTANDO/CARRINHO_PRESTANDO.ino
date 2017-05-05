#include <Ultrasonic.h>
#include <Servo.h>

int QRE1113_Pin = 0;
Servo servo_copo;
int pos = 0;
bool pegouCopo = false;

#define TRIGGER_PIN    12
#define ECHO_PIN       13

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

#define VELOCIDADE_BASE  180

const int IN1 = 5, IN2 = 6, IN3 = 9, IN4 = 10, STA1 = 2;

// IN1 e IN2 -> motor esquerdo
// IN3 e IN4 -> motor direito

int CURRENT_STATE = 0;
int loops = 0;
unsigned char S[10];
int velEsq, velDir;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(STA1, OUTPUT);
  
  analogWrite(IN1, 0);
  analogWrite(IN3, 0);
  Serial.begin(9600);
  digitalWrite(STA1, 0);
  
  servo_copo.attach(9);
  servo_copo.write(0);
  pinMode(8,OUTPUT);
  digitalWrite(8, HIGH);
}

void track_ultrassom_frontal() {
  if (S[0] == 'r') {
    int vel_pd = (int)S[1];
    vel_pd = vel_pd - 100;
    float vel_normal = vel_pd / 100.0;
    if (vel_normal >= 0) {
      velEsq = VELOCIDADE_BASE;
      velDir = (int)(velEsq * (1 - vel_normal));
    } else {
      vel_normal = vel_normal * -1;
      velDir = VELOCIDADE_BASE;
      velEsq = (int)(velDir * (1 - vel_normal));
    }

  } else if (S[0] == 'p') {
    analogWrite(IN4, 0);
    analogWrite(IN2, 0);
  } else if (S[0] == 'l') {
      analogWrite(IN4, 80);
      analogWrite(IN2, 50);
      delay(500);
      analogWrite(IN4, 0);
      analogWrite(IN2, 0);
      delay(2000);
  } else if (S[0] == 'c') {
      analogWrite(IN4, 0);
      analogWrite(IN2, 0);
      CURRENT_STATE++;
      Serial.println(CURRENT_STATE);
  }

  float distancia;
  long microsec = ultrasonic.timing();
  distancia = ultrasonic.convert(microsec, Ultrasonic::CM);

  // feature scaling para o dado de entrada do ultrassom
  int ultrassom_pd = (int)distancia;
  ultrassom_pd = constrain(ultrassom_pd, 10, 300);

  float ultrassom_normal = (map(ultrassom_pd, 10, 300, 0, 100)) / 100.0;  // variável de entrada normalizada do ultrassom
  // entre 0 e 1*/

  //Serial.println(distancia);
  
  if (distancia < 50) {
    digitalWrite(STA1, 1);
    velDir = 0;
    velEsq = 0;
        
    while(Serial.available()){
      Serial.read();
    }
  
  } else {
    digitalWrite(STA1, 0);
    velDir = (4 * velDir + ultrassom_normal) / 5;
    velEsq = (4 * velEsq + ultrassom_normal) / 5;
  }
  
  analogWrite(IN4, velDir);
  analogWrite(IN2, velEsq);
}

void loop() {
  Serial.readBytesUntil(';', S, 7); // leitura do segundo dado (pd no eixo x) -> esse já vem normalizado, basta ajustar as unidades

  switch(CURRENT_STATE){
    case 0:
      if (S[0] == 'c') {
        analogWrite(IN4, 0);
        analogWrite(IN2, 0);
        CURRENT_STATE++;
        Serial.println(CURRENT_STATE);
      } 
    break;
    case 1:
      track_ultrassom_frontal();
    break;
    case 2:
    
      
    break;
    case 3:
      int QRE_Value = analogRead(QRE1113_Pin);
      if(!pegouCopo && QRE_Value<200){
          pegouCopo = true;
          for(pos = 0; pos <= 180; pos += 1) {
            servo_copo.write(pos);
            delay(15);
          }
        }
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
  }


  
  
}
