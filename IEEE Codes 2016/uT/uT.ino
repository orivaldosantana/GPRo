#include <Servo.h>
#include <Ultrasonic.h>

const int DISTANCIA_LIMITE_PAREDE_FRONTAL_MIN = 15;
const int DISTANCIA_LIMITE_PAREDE_LATERAL_MIN = 10;
const int DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN = 20;
const int DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX = 40;

#define VELOCIDADE_BASE  200
#define QRE1113_Pin A0
#define garra_aberta 0
#define garra_fechada 120

Ultrasonic uFrontalEsq (49, 47);
Ultrasonic uFrontalDir (50, 52);
Ultrasonic uLateralEsq (51,53);
Ultrasonic uLateralDir (46, 48);
Ultrasonic uFrontalGarra (40,41);

float distanciaFrontal[2] = {0, 0}; // [0]= esquerda ---------- [1] = direita
float distanciaLateral[2] = {0, 0};
int motorDir1 = 6, motorDir2 = 7;
int motorEsq1 = 5, motorEsq2 = 4;
int velEsq;
int velDir;
unsigned long timing = 0;

long microsec=0;

int STA = 31;

int CURRENT_STATE = 1;
unsigned char S[10];

bool distLimitFrontal[2] = {false, false};
bool alinhouDistLimitFrontal = false;
bool alinhouDistCopo = false;
bool esq = false;
bool endTime(unsigned long init_time, unsigned long end_time = 1000);
void virarDir(int vel1 = 100, int vel2 = 100);
void virarEsq(int dir = 100, int esq = 100);
void tras(int dir = 100, int esq = 100);

Servo garra, punho, braco;
int pos = 0;
int posB = 110;
int pinInfr = 5;
bool pegouCopo = false;
bool com_agua = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(motorEsq1, OUTPUT);
  pinMode(motorEsq2, OUTPUT);
  pinMode(STA, OUTPUT);

  garra.attach(8);
  punho.attach(9);
  braco.attach(10);

  garra.write(0);
  braco.write(110);
  punho.write(180);
  
}

void get_distanciaFrontal() {
  long microsecEsq = uFrontalEsq.timing();
  long microsecDir = uFrontalDir.timing();
  float distEsq = uFrontalEsq.convert(microsecEsq, Ultrasonic::CM);
  delay(10);
  float distDir = uFrontalDir.convert(microsecDir, Ultrasonic::CM);
  delay(10);
  distanciaFrontal[0] = distEsq;
  distanciaFrontal[1] = distDir;
}

void get_distanciaLateral(){
  long microsecEsq = uLateralEsq.timing();
  long microsecDir = uLateralDir.timing();
  float distEsq = uLateralEsq.convert(microsecEsq, Ultrasonic::CM);
  float distDir = uLateralDir.convert(microsecDir, Ultrasonic::CM);

  distanciaLateral[0] = distEsq;
  distanciaLateral[1] = distDir;
}

// True -> Se o carrinho ainda estiver fora do alcance minimo, ele continuará indo para frente.
void setDistanciaFrontal() {
  if (distanciaFrontal[0] > DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN) {
    distLimitFrontal[0] = true;
  } else {
    distLimitFrontal[0] = false;
  }
  if (distanciaFrontal[1] > DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN) {
    distLimitFrontal[1] = true;
  } else {
    distLimitFrontal[1] = false;
  }
  Serial.print(distLimitFrontal[0]);
  Serial.print(" ");
  Serial.println(distLimitFrontal[1]);
}


bool verificarDistLimitFrontal() {
  get_distanciaFrontal();
  setDistanciaFrontal();
  frente();
  Serial.print("rangeEsq: ");
  Serial.print(distanciaFrontal[0]);
  Serial.print("\trangeEsq: ");
  Serial.println(distanciaFrontal[0]);
  if (distLimitFrontal[0] && distLimitFrontal[1])
    return true;
  else
    return false;
}

void alinharDistLimitFrontal() {
  float deltaEsq = distanciaFrontal[0] - distanciaFrontal[1];
  int erro = 5; 

  while(deltaEsq > erro){ //&& deltaEsq < DISTANCIA_LIMITE_FRONTAL_MAX)) {
    timing = startTime();
    while(!endTime(timing, 500)){ virarDir(); }
    deltaEsq = distanciaFrontal[0] - distanciaFrontal[1];
    get_distanciaFrontal();
    Serial.println("VIRANDO PARA DIREITA");
  }
  while(deltaEsq < -erro){ //&& deltaEsq > -DISTANCIA_LIMITE_FRONTAL_MAX) {
    timing = startTime();
    while(!endTime(timing, 500)){ virarDir(); }
    deltaEsq = distanciaFrontal[0] - distanciaFrontal[1];
    get_distanciaFrontal();
    
  }
  if (   distanciaFrontal[0] < DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN + erro
      && distanciaFrontal[0] > DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN - erro
      && distanciaFrontal[1] < DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN + erro
      && distanciaFrontal[1] > DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MIN - erro ) {
      alinhouDistLimitFrontal = true;
  }
   //frente();
}

void parar() {
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW);
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, LOW);
}

void virarDir(int dir = 100, int esq = 100){
    analogWrite(motorEsq1, esq);
    analogWrite(motorDir2, dir);
    analogWrite(motorEsq2, 0);
    analogWrite(motorDir1, 0);
}

void virarEsq(int dir = 100, int esq = 100){
    Serial.println("VIRANDO PARA ESQUERDA");
    analogWrite(motorDir1, dir);
    analogWrite(motorEsq2, esq);
    analogWrite(motorDir2, 0);
    analogWrite(motorEsq1, 0);
}

void tras(int dir = 100, int esq = 100){
    analogWrite(motorEsq2, esq);
    analogWrite(motorDir2, dir);
    analogWrite(motorDir1, 0);
    analogWrite(motorEsq1, 0);
}

unsigned long startTime(){
  return millis();
}

bool endTime(unsigned long init_time, unsigned long end_time = 1000){
 // Serial.println(" ");
  return (millis() - init_time >= end_time);
}

void frente(){
  Serial.println("frente");
  analogWrite(motorEsq1, 200);
  analogWrite(motorDir1, 200);
  analogWrite(motorEsq2, 0);
  analogWrite(motorDir2, 0);
}

//virar toda a frente do robo até a parede de trás dele
void localizarParede(){
  int tolerancia = 3;
  
  while(!(abs(distanciaFrontal[0] - distanciaFrontal[1] < tolerancia) && distanciaFrontal[0] < DISTANCIA_LIMITE_PAREDE_FRONTAL_MIN)){
    get_distanciaFrontal();
    get_distanciaLateral(); 
    virarDir(); 
  }
  parar();
}

void alinharDistCopo(){
  Serial.readBytesUntil(';', S, 7);
    if(S[0] == 'e'){
      Serial.println("test1");
      timing = startTime();
      while(!endTime(timing)){ virarDir(); }
      parar();
      Serial.println("test2");
      timing = startTime();
      while(!endTime(timing,2000)){ tras(); }
      Serial.println("test3");
      timing = startTime();
      while(!endTime(timing)){ virarEsq(); }
      parar();
      Serial.println("test4");
      timing = startTime();
      while(!endTime(timing, 2000)){ frente(); }
      parar();
      Serial.println("Alinhou a Esquerda");  
    } else if(S[0] == 'd'){
      Serial.println("test5");
      timing = startTime();
      Serial.print(timing);
      Serial.println();
      while(!endTime(timing)){ virarEsq(); }
      parar();
      Serial.println("test6");
      timing = startTime();
      Serial.print(timing);
      Serial.println();
      while(!endTime(timing, 2000)){ tras(); }
      Serial.println("test7");
      timing = startTime();
      while(!endTime(timing)){ virarDir(); }
      parar();
      Serial.println("test8");
      timing = startTime();
      while(!endTime(timing, 2000)){ frente(); }
      parar();  
      Serial.println("Alinhou a Direita");
    }else if(S[0] == 'u'){
      alinhouDistCopo = true;
      Serial.println("AlinhouDistCopo");
    }
  }

void track_ultrassons_frontal() {
  Serial.readBytesUntil(';', S, 7); // leitura do segundo dado (pd no eixo x) -> esse já vem normalizado, basta ajustar as unidades
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
    parar();
  } else if (S[0] == 'l') {
    analogWrite(motorDir1, 80);
    analogWrite(motorEsq2, 80);
    analogWrite(motorDir2, 0);
    analogWrite(motorEsq1, 0);
    delay(500);
    parar();
    delay(2000);
  } else if (S[0] == 'c') {
    parar();
    CURRENT_STATE++;
    Serial.println(CURRENT_STATE);
  }

  get_distanciaFrontal();

  distanciaFrontal[0] = distanciaFrontal[0] - DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX;
  distanciaFrontal[1] = distanciaFrontal[1] - DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX;

  // feature scaling para o dado de entrada do ultrassom
  int ultrassom_pd[2];

  ultrassom_pd[0] = (int)distanciaFrontal[0];
  ultrassom_pd[1] = (int)distanciaFrontal[1];

  ultrassom_pd[0] = constrain(ultrassom_pd[0], DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX, 200);
  ultrassom_pd[1] = constrain(ultrassom_pd[1], DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX, 200);


  float ultrassom_normal[2];
  ultrassom_normal[0] = (map(ultrassom_pd[0], 10, 300, 0, 100)) / 100.0;  // variável de entrada normalizada do ultrassom
  ultrassom_normal[1] = (map(ultrassom_pd[1], 10, 300, 0, 100)) / 100.0;  // variável de entrada normalizada do ultrassom

  int ultrassom;

  if (ultrassom_normal[0] < ultrassom_normal[1]) {
    ultrassom = (int)ultrassom_normal[0];
  } else {
    ultrassom = (int)ultrassom_normal[1];
  }

  if (distanciaFrontal[0] <= DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX || distanciaFrontal[1] <= DISTANCIA_LIMITE_ALINHAMENTO_FRONTAL_MAX) {
    digitalWrite(STA, 1);
    velDir = 0;
    velEsq = 0;

    while (Serial.available()) {
      Serial.read();
    }

  } else {
    digitalWrite(STA, 0);
    velDir = (int)((4*velDir + ultrassom_normal[1])/5.0);
    velEsq = (int)((4*velEsq + ultrassom_normal[0])/5.0);
  }

  analogWrite(motorEsq1, velEsq);
  analogWrite(motorDir1, velDir);
  analogWrite(motorEsq2, 0);
  analogWrite(motorDir2, 0);
}

void trackWall(){
  while (1) {
    get_distanciaFrontal();
    if (distanciaFrontal[0] > 15) {
      frente();
      delay(50);
    } else {
      break;
    }
  }
  while (1){
    get_distanciaLateral();
    if (distanciaLateral[0] > 15) {
       virarDir();
    } else {
      break;
    }
  }
  while (1) {
    get_distanciaFrontal();
    if (distanciaFrontal[0] > 25) {
      frente();
      delay(50);
    } else {
      break;
    }
  }

  digitalWrite(STA, HIGH);
  CURRENT_STATE++;      
}

void loop () {
  digitalWrite(STA, 0);
  switch (CURRENT_STATE) {
    /*case 0:
      Serial.readBytesUntil(';', S, 7); // leitura do segundo dado (pd no eixo x) -> esse já vem normalizado, basta ajustar as unidades
      if (S[0] == 'c') {
        parar();
        CURRENT_STATE++;
        Serial.println(CURRENT_STATE);
      }
      break;*/
    /*case 1:
      trackWindows();
      break;
    case 2:
      if (!alinhouDistLimitFrontal) {
  //Serial.println(verificarDistLimitFrontal());
      if(!verificarDistLimitFrontal()) {
      alinharDistLimitFrontal();
      parar();
    }
  } else if(!alinhouDistCopo){
        Serial.println("AlinhouDistLimit");
    //Serial.println("Fim!");
    //procurar();
        alinharDistCopo();
      } else {
        get_distanciaFrontal();
        if (distanciaFrontal[0] <= 6) {
          digitalWrite(STA, 1);
          CURRENT_STATE++;
        } else {
          frente();
          delay(50);
        }
      }
      break;*/
    case 1:
      while (1){
        get_distanciaFrontal();
        if (distanciaFrontal[1] > 5) {
          frente();
        } else {
          digitalWrite(STA, 1);
          CURRENT_STATE++;
          break;
        }        
      }
    case 2:
      float cmMsec;
      microsec = uFrontalGarra.timing();
      cmMsec = uFrontalGarra.convert(microsec, Ultrasonic::CM);
      delay(10);
      Serial.print(" CM: ");
      Serial.println(cmMsec);
      //delay(100);
      if (cmMsec <= 6) {
        int QRE_Value = analogRead(QRE1113_Pin);
        if (!pegouCopo && QRE_Value < 700) {
          pegouCopo = true;
          for (pos = garra_aberta; pos <= garra_fechada; pos++) {
            garra.write(pos);
            delay(5);
          }
          braco.write(110);
          digitalWrite(STA, 1);
          CURRENT_STATE++;
        }
      } else if (cmMsec <= 10 && !pegouCopo) {
        posB -= 2;
        braco.write(posB);
        delay(10);
      }
      break;
    case 3:
      track_ultrassons_frontal();
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case  9:
      break;
  }
}

// jhgfklhsdflhfdjgkls

