#ifndef DCMOTOR_H
#define DCMOTOR_H




class DCMotor
{
  public:
    DCMotor(int in1, int in2, int in3, int in4);
    void forward();   // Andar para frente
    void backward(); //Andar para tras
    void left();    // Esquerda
    void right();  //Direita
    void stop();  //Parada
    void leftBack();      //roda Esquerda para tras
    void rightBack();    //  roda  Direita para tras

  private:
    void clockwise(int inx, int iny);   //Faz o motor girar no sentido horário
    void antiClockwise(int inx, int iny);     //Faz o motor girar no sentido anti-horario
    int _in1, _in2, _in3, _in4;
    //const int  IN1 = 5, IN2 = 4, IN3 = 3, IN4 = 2;//pinos

};

#endif
