
#include <Ultrasonic.h>

const int DISTANCIA_LIMITE = 20;

Ultrasonic uFrontalEsq (13,12);
Ultrasonic uFrontalDir (15,14);

float distancia[2] = {0, 0}; // [0] = esquerda ---------- [1] = direita
float motorDir1 = 2, motorDir2 = 3;
float motorEsq1 = 4, motorEsq2 = 5;
int velEsq = 200;
int velDir = 200;

bool distLimit[2] = {false, false}; 
bool alinhou = false;

void setup()
{
  Serial.begin(9600);

  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(motorEsq1, OUTPUT);
  pinMode(motorEsq2, OUTPUT);
}

void loop(){
  if(alinhou){
    if(verificarDistLimit()){
      alinharDistLimit();
    }
  }
   
}

void get_distancia(){
  long microsecEsq = uFrontalEsq.timing();
  long microsecDir = uFrontalDir.timing();
  float distEsq = uFrontalEsq.convert(microsecEsq, Ultrasonic::CM);
  float distDir = uFrontalDir.convert(microsecDir, Ultrasonic::CM);

  distancia[0] = distEsq;
  distancia[1] = distDir;
  /*
  Serial.print("Esq = ");
  Serial.print(distancia[0]);
  Serial.print("\tDir = ");
  Serial.println(distancia[1]);
  */
}

void setDistancia(){
  if(distancia[0] < DISTANCIA_LIMITE){ 
    distLimit[0] = true; 
  }else{
    distLimit[0] = false;
  }
  if(distancia[1] < DISTANCIA_LIMITE){
    distLimit[1] = true;  
  }else{
    distLimit[1] = false;
  }
   Serial.print(distLimit[0]);
   Serial.print(" ");
   Serial.println(distLimit[1]);
}

bool verificarDistLimit(){
    get_distancia();
    setDistancia();
    
    if(distLimit[0] || distLimit[1])
      return true; 
    else 
      return false;    
}

void alinharDistLimit(){
  float deltaEsq = distancia[0] - distancia[1];
  float erro = 2;

  parar();
  if(deltaEsq > erro){    
    analogWrite(motorEsq1, 100);
    analogWrite(motorEsq2, 100); 
    deltaEsq = distancia[0] - distancia[1];
    get_distancia();
    Serial.print("deltaEsq = ");
    Serial.println(deltaEsq);
     
  }
  else if(deltaEsq < -erro){    
    analogWrite(motorDir1, 100);
    analogWrite(motorEsq2, 100);
    deltaEsq = distancia[0] - distancia[1];
    get_distancia();    
    Serial.print("deltaEsq = ");
    Serial.println(deltaEsq);
  }
  else if(deltaEsq < erro && deltaEsq > -erro && distancia[0] + erro > DISTANCIA_LIMITE){
    alinhou = true;  
  } 
}

void parar(){
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorEsq2, LOW); 
}
/*
void alinhar(){
  if(Serial.available() > 0){
    bit val = Serial.read();
    if(val == 1){
      
    }
  }  
}
*/ 
