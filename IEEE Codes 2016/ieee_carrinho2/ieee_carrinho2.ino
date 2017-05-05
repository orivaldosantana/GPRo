#include <Ultrasonic.h>

#define TRIGGER_PIN    12
#define ECHO_PIN       13
#define CONSTTIME 500

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

#define VELOCIDADE_BASE  180

const int IN1 = 5, IN2 = 6, IN3 = 9, IN4 = 10;
int flagtime = 0;

// IN1 e IN2 -> motor esquerdo
// IN3 e IN4 -> motor direito

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
  /*float distancia;
  long microsec = ultrasonic.timing();
  distancia = ultrasonic.convert(microsec, Ultrasonic::CM);

  // feature scaling para o dado de entrada do ultrassom
  int ultrassom_pd = (int)distancia;
  ultrassom_pd = constrain(ultrassom_pd, 10, 300);
  
  float ultrassom_normal = (map(ultrassom_pd, 10, 300, 0, 100))/100.0;    // variável de entrada normalizada do ultrassom
                                                                       // entre 0 e 1*/

  Serial.readBytesUntil(';', S, 7); // leitura do segundo dado (pd no eixo x) -> esse já vem normalizado, basta ajustar as unidades
  
  if(S[0] == 'r' && (millis() - flagtime) > CONSTTIME){
    Serial.println((millis() - flagtime));
    flagtime = millis();
    int vel_pd = (int)S[1];
    vel_pd = vel_pd - 100;
    float vel_normal = vel_pd/100.0;
    int velEsq, velDir;
    if (vel_normal >= 0) {
      velEsq = VELOCIDADE_BASE;
      velDir = (int)(velEsq*(1-vel_normal));
    } else {
      vel_normal = vel_normal*-1;
      velDir = VELOCIDADE_BASE;
      velEsq = (int)(velDir*(1-vel_normal));
    }
      //velDir = (9*velDir + ultrassom_normal)/10;
      //velEsq = (9*velEsq + ultrassom_normal)/10;

//    Serial.println(velDir);
//    Serial.println(velEsq);
//    Serial.println();
    
    analogWrite(IN4, velDir);
    analogWrite(IN2, velEsq);
    
  } else if (S[0] == 'p') {
      analogWrite(IN4,0);
      analogWrite(IN2,0);
  }
}
