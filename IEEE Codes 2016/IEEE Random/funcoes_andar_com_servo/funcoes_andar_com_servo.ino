#include <Servo.h>
#define pinEsqFrente 4
#define pinEsqTras 5
#define pinDirFrente 6
#define pinDirTras 7
Servo EsqFrente;
Servo DirFrente;
Servo EsqTras;
Servo DirTras;
void declara(){
  EsqFrente.attach(pinEsqFrente);
  DirFrente.attach(pinEsqTras);
  EsqTras.attach(pinDirFrente);
  DirTras.attach(pinDirTras);
}
void desdeclara(){
  EsqFrente.detach();
  DirFrente.detach();
  EsqTras.detach();
  DirTras.detach();
}

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}
void andaReto(int vel){
  declara();
  EsqFrente.write(map(vel,0,180,0,0,180));
  DirFrente.write(vel);
  EsqTras.write(map(vel,0,180,0,0,180));
  DirTras.write(vel);
  
}
void giraNoEixo(int vel){
  declara();
  EsqFrente.write(map(vel,0,180,0,0,180));
  DirFrente.write(vel);
  EsqTras.write(map(vel,0,180,0,0,180));
  DirTras.write(vel);
}
void loop() {
  andaReto(180);
  delay(2000);
  giraNoEixo(70);
  delay(2000);
}

