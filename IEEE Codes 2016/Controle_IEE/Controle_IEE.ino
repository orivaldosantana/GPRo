#include <Robo.h>

struct Data{
  char ref;
  int value;
};

const int BUFFER_LENGTH = 5;

Robo robo(11,6,5,3);
Data data[2];

char buffer[BUFFER_LENGTH];
int count =0 ;

void setup(){
  Serial.begin(9600);
}
void loop(){
  Serial.flush();
  if(Serial.available() > 0){
    delay(100);
    int numChar = Serial.available();
    char c = Serial.read();
    Serial.readBytesUntil(';',buffer,BUFFER_LENGTH);
    splitString(numChar,c,buffer);

    if(numChar <= BUFFER_LENGTH){
      switch(data[0].ref){
        case 'r': robo.rotacionar(data[1].value,data[0].value); break;
        case 'f': robo.frente(data[0].value);                   break;
        case 't': robo.tras(data[0].value);                     break;
        case 'p': robo.parar();                                 break;
      }
    }
    for(int i=0; i<BUFFER_LENGTH; i++)
      buffer[i] = '\0';
  }
}

void splitString(int numChar, char c, char* buffer){
  if(numChar <= BUFFER_LENGTH){
    data[0 + count].ref = c;
    data[0 + count].value = strtol(buffer,NULL,10);
    if(count == 1){
      Serial.print("data[0]: ");
      Serial.print(data[0].value);
      Serial.print("\tdata[1]: ");
      Serial.println(data[1].value);    
    }
    else{
      Serial.print("data[0]: ");
      Serial.println(data[0].value);  
    }
    count = 0;
  }
  else if(numChar <= (BUFFER_LENGTH<<1) ){
    data[0].ref = c;
    data[0].value = strtol(buffer,NULL,10);
    count++;
  }
}
