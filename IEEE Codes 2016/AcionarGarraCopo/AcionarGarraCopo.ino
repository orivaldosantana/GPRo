#include <Servo.h>
#include <Ultrasonic.h>

#define TRIGGER_PIN 40
#define ECHO_PIN 41
#define QRE1113_Pin A0
#define garra_aberta 0
#define garra_fechada 120
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
Ultrasonic uFrontalEsq (33, 31);
Ultrasonic uFrontalDir (50, 52);

Servo garra, punho, braco;
int pos = 0;
int pinInfr = 5;
bool pegouCopo = false;
bool com_agua=0;
bool copoProximo = false;


void setup() {
  garra.attach(8);
  punho.attach(9);
  braco.attach(10);
  Serial.begin(9600);
  delay(1000);
  garra.write(0);
  braco.write(110);
  punho.write(180);
}
int posB = 110;
void loop() {

  float cmMsec;
  float dist;
  long microsec = uFrontalGarra.timing();
  
  //dist = uFrontalEsq.convert(microsec, Ultrasonic::CM)
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  
  delay(100);
  Serial.print(", CM: ");
  Serial.print(cmMsec);
  Serial.print("\tposB: ");
  Serial.println(posB);
  delay(10);
 
  if (cmMsec <= 13 && !pegouCopo) {
    posB -= 2;
    braco.write(posB);
    Serial.print("Test1\t");
    Serial.println(posB);
    delay(10);
    
    if(cmMsec <= 5){
      copoProximo = true;
      Serial.println("Test2");
    }    
  }else if (copoProximo) {
    Serial.print("Test3\t");
    int QRE_Value = analogRead(QRE1113_Pin);
    Serial.print(QRE_Value);
    Serial.print("\t");
     //posB -= 2;
    if (QRE_Value < 730 && copoProximo) {
      pegouCopo = true;
      for (pos = garra_aberta; pos <= garra_fechada; pos++) {
        garra.write(pos);
        delay(5);
      }
      copoProximo = false;
      braco.write(110);
    }
  }
  
}
