#include <Skills.h>
#include <extras.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
//#include <functionsOpenCV.h>
#include <neuron.h>
#include <SOM.h>
#include <sys/stat.h>
#include <bits/basic_string.h>
#include <string>

#include "VrepToOpencv.h"






SKILLS::SKILLS(std::string serverIP, int serverPort) {
    sensor[8] = {};
    joint[9] = {};
    angle[5] = {};
    debug = false;
    
    vrep = new Vdc(serverIP.c_str(),serverPort );
    
    if (vrep->simulationIsActive())
        connectToRobot();
    else
        std::cout << "connection fail " << std::endl;


}
 

void SKILLS::connectToRobot() {

    // inicialização dos motores do lado esquerdo 
 // NOME DO JOIN MOTOR  \/ AONDE ELE VAI GUARDAR \/   
 
    
    vrep->conectpiece("Motor_esquerdo_Tras",joint[1]);
    
    
    vrep->conectpiece("Motor_esquerdo_frente",joint[3]);
    

    
    // inicialização dos motores do lado direito 
    vrep->conectpiece("Motor_direito_frente",joint[0]);
    
    
    vrep->conectpiece("Motor_direito_Tras",joint[2]);
    



    //  juntas da GARRA
    for (int i = 4; i < 8; i++) { // 
        vrep->conectpiece("Joint#" + std::to_string(i - 3), joint[i]);


    }

    // inicialização dos sensores de proximidade  (remoteApi)
    for (int i = 0; i < 8; i++) {
        vrep->conectpiece("Proximity_sensor_" + std::to_string(i + 1), sensor[i]);
    }

   vrep->conectpiece("Webcam", Webcam);

   

    for (int i = 4; i < 8; i++) {
        float position;
        vrep->getJointPosition(joint[i], position);
        angle[i - 4] = position;
        if (debug)
            std::cout << "i: " << i << " angle[" << i - 4 << "] = " << angle[i - 4] << std::endl;
    }


   visionSensor = new VrepToOpencv (vrep->getClientID(),Webcam);



}

double SKILLS::degree(double Degree) {
    return (Degree * 3.14159265359) / 180;
}

void SKILLS::seguidorDeParede() {
    //joint#1 == 4| joint#2 == 5 | joint#3 == 6 ...
    float velocityLeft = 0.75;
    float velocityRight = 0.75;
    float dist[8];


    for (int i = 0; i < 8; i++) {
        vrep->readProximitySensor(sensor[i], dist[i]);
        // sensor[0] =  Proximity_sensor_1 , sensor[1] = Proximity_sensor_2 ... 
    }

    if (dist[2] < 0.2) {

        velocityLeft = 2.0 * degree(15);
        velocityRight = 1.75;


    } else if (dist[2] > 0.22 && dist[2] != 500) {
        velocityLeft = 1.75;
        velocityRight = 2.0 * degree(15);

    } else if (dist[2] > 100) {

    }

    if (dist[4] < 0.33 || dist[5] < 0.33) {
        velocityLeft = -1;
        velocityRight = 1;

    }

    while (dist[2] < 0.3 && dist[3] < 0.3 || dist[0] < 0.3 && dist[1] < 0.3) {
        velocityLeft = -1;
        velocityRight = 1;
    }







    std::cout << "dist[0]: " << dist[0] << " dist[2]: " << dist[2] << " dist[7]: " << dist[7] << std::endl;




    SKILLS::setVelocityInRobot(velocityRight, velocityLeft);


}

void SKILLS::testJunta() {

    while (vrep->simulationIsActive()) {

        for (int i = 4; i < 9; i++) {
            if (i == 5 && angle[1] < 0.09) //Joint#2 movimento linear
            {
                angle[1] += 0.0005;
                vrep->setJointPosition(joint[i], angle[1]);

            } else if (i == 6 && angle[2] < degree(180) && angle[1] >= 0.030) // Joint#3
            {
                angle[2] += 0.01;
                 vrep->setJointPosition(joint[i], angle[2]);
            } else if (i == 4 && angle[0] < degree(40) && angle[4] >= 0.030 && angle[2] >= degree(90)) //joint#1
            {
                angle[0] += 0.003;
                 vrep->setJointPosition(joint[i], angle[0]);

            }
            //
            //joint#4

            if (i == 8 && angle[4] < degree(41.83)) {
                angle[4] += 0.01;
                 vrep->setJointPosition(joint[7], angle[4]);

            }



        }

        vrep->delay(100);

    }

}

void SKILLS::testGetImage() {
    visionSensor->showImage("Cam");

}

void SKILLS::testSetImage() {
  //  SKILLS::setImageVisionSensor(Webcam);
}

void SKILLS::testReadCam() {
    
//    SKILLS::readVisionSensor(Webcam);
}

void SKILLS::goToTank() {
     
   
    double rx, ry;
    int q3 = 240; // dividi o eixo x da imagem em 7 quartis  
    int q5 = 400;
    float velocityRight = 1;
    float velocityLeft = 1;

    while (vrep->simulationIsActive()) {
        velocityRight = 1;
        velocityLeft = 1;

        if (visionSensor->findRedColorMass(rx,ry) ) {
                      
//          findRedColorMass(imageVrep, rx, ry);

            if (debug) {
                cout << "posição x do tank: " << rx << " y: " << ry << endl;
            }


            if (rx < q3)
                velocityLeft = 0;
            else if (rx > q5)
                velocityRight = 0;





            SKILLS::setVelocityInRobot(velocityRight, velocityLeft);


        }

    }

}

void SKILLS::WhereIsTheCow() {
/*
   cv::Mat image;
    float rx = 123;
    int x, y;
    float velocityLeft = 1;
    float velocityRight = 1;

    if (SKILLS::imageVrepToOpencv(Webcam, image)) {
       // findCow(image, x, y);
        cout << rx << endl;


        if (rx < 0.01)
            velocityLeft = 0;
        else if (rx > 0.01)
            velocityRight = 0;

        SKILLS::setVelocityInRobot(velocityRight, velocityLeft);

    }

*/


}

void SKILLS::setVelocityInRobot(float velocityRight, float velocityLeft) {

    // atualiza velocidades dos motores
 
    vrep->setJointVelocity(joint[0], velocityRight);
    vrep->setJointVelocity(joint[2], velocityRight);

    vrep->setJointVelocity(joint[1], velocityLeft);
    vrep->setJointVelocity(joint[3], velocityLeft);
    //*/


}

void SKILLS::seguirParedeMLP() {

    vector < double > inputs(6);
    vector < double > direction(4);
    vector< int > capas = {6, 200, 4};


    int rows = 0;
    int columns = 0;
    std::fstream data;
    std::string aux;


    data.open("Coleta/inputMlP1.csv");

    if (data.is_open() && data.good()) {

        std::getline(data, aux); // pula primeira linha


        std::stringstream lineStreamHeader(aux);

        while (std::getline(lineStreamHeader, aux, ','))
            columns++;

        while (getline(data, aux))
            rows++;



        data.close();
    } else
        cout << "file not found" << endl;


    std::vector < std::vector < double > > entrenador1(rows, std::vector< double >(6, 0));
    std::vector < std::vector < double > > entrenador2(rows, std::vector< double >(4, 0));



    data.open("Coleta/inputMlP1.csv");

    std::getline(data, aux); // pula primeira linha

    for (int i = 0; i < rows; i++) {
        std::getline(data, aux);
        std::stringstream lineStream(aux);
        std::string cell;

        for (int j = 0; j < columns; j++) {
            std::getline(lineStream, cell, ',');
            if (j < 6)
                entrenador1[i][j] = std::stod(cell);
            else
                entrenador2[i][j - 6] = std::stod(cell);


        }
    }


    //Creamos la red
    cout << "criando rede..." << endl;
    Network red(capas);
    cout << "Rede criada" << endl;

    //Entrenamos la red con los ejemplos
    cout << "Treinando rede ..." << endl;
    red.Aprendizaje_Prop_Atras(entrenador1, entrenador2);
    cout << "Rede treinada" << endl;

    //red.Mostrar_Pesos(); //Mostramos los pesos definitivos

    //Mostrar los outputs:
    for ( unsigned int i = 0; i < entrenador1.size(); i++)
        red.Calcular_Output(entrenador1[i]);
    //red.Mostrar_Output();



    while (vrep->simulationIsActive()) {


        for (int i = 0; i < 6; i++){
            float distance;
            vrep->readProximitySensor(sensor[i],distance );
            inputs[i] = distance;
        }
          //  inputs[i] = SKILLS::getDistance(sensor[i]);



        direction = red.Calcular_Output(inputs);

        // frente 0,1,0,0
        // ré 0,0,0,1
        // direita 0,0,1,0
        // esquerda 1,0,0,0

        cout << direction[0] << endl;
        float l = 0.5;
        float vel = 1.0;

        if (direction[0] < l && direction[1] > l && direction[2] < l && direction[3] < l) //frente
            SKILLS::setVelocityForControler(vel, vel);

        if (direction[0] < l && direction[1] < l && direction[2] < l && direction[3] > l) // tras
            SKILLS::setVelocityForControler(-vel, -vel);

        if (direction[0] < l && direction[1] < l && direction[2] > l && direction[3] < l) // direta
            SKILLS::setVelocityForControler(-vel, vel);

        if (direction[0] > l && direction[1] < l && direction[2] < l && direction[3] < l) // esquerda
            SKILLS::setVelocityForControler(vel, -vel);


    }
}

void SKILLS::verDistancia(int i) {
    float teste;
    vrep->readProximitySensor(sensor[i],teste);
 //   double teste = SKILLS::getDistance(sensor[i]);
    cout << "distancia entre 0 e 1:  " << teste << endl;

}

bool SKILLS::controlerRobot() {

    float vel = 1.0;


    //cout << "\n\n  os controles são: w ,s,a,d \n  e a letra:e para sair\n";


    system("stty raw");

    char input = getchar();

    switch (input) { // esse controle não se aplica ao teclado de Camila, 
        case 'w':
            SKILLS::setVelocityForControler(vel, vel);
            controlData = "," + std::to_string(vel) + "," + std::to_string(vel); //",0,1,0,0"; // frente 0,1,0,0
            return true;
        case 's':
            SKILLS::setVelocityForControler(-vel, -vel);
            controlData = "," + std::to_string(-vel) + "," + std::to_string(-vel); //",0,0,0,1"; // ré 0,0,0,1
            return true;

        case 'd':
            SKILLS::setVelocityForControler(-vel, vel);
            controlData = "," + std::to_string(-vel) + "," + std::to_string(vel); //",0,0,1,0"; // direita 0,0,1,0
            return true;

        case 'a':
            SKILLS::setVelocityForControler(vel, -vel);
            controlData = "," + std::to_string(vel) + "," + std::to_string(-vel); //",1,0,0,0"; // esquerda 1,0,0,0
            return true;

        case 'q':
            SKILLS::takePhotos();
            countImage++;
            return true;

            /* A ideia do controle da garra é o seguinte:
             * as letras: y , u, i, o, representam os movimentos que setam valores positivos 
             * das seguintes juntas: Joint#1, Joint#2 , Joint#3 , Joint#4
             * e os movimentos negativos ou decrescentes, são as letras de baixo:
             * h, j, k, l, OBS: isso não se aplica a teclados bugados como o das de camila 
             */

        case 'y': // Joint#1 += 0.003 radianos
            SKILLS::setPositionForControler(1, true);
            return true;

        case 'h': // Joint#1 -=0.003 radianos
            SKILLS::setPositionForControler(1, false);
            return true;

        case 'u': // Joint#2 += 0.0005 movimento linar
            SKILLS::setPositionForControler(2, true);
            return true;

        case 'j': // Joint#2 -= 0.0005 radianos
            SKILLS::setPositionForControler(2, false);
            return true;

        case 'i': // Joint#3 += 0.01 radianos
            SKILLS::setPositionForControler(3, true);
            return true;

        case 'k': // Joint#3 -= 0.01 radianos
            SKILLS::setPositionForControler(3, false);
            return true;

        case 'o': //Joint#4  += 0.01 radianos
            SKILLS::setPositionForControler(4, true);
            return true;

        case 'l': //Joint#4  -= 0.01 radianos
            SKILLS::setPositionForControler(4, false);
            return true;











        case 'e':
            system("stty cooked");
            vrep->finish();
            return false;



    }

    return false;
}

void SKILLS::collectDataforNetWork() {
    /*
    Extras extras;
    string data;
    string header = "sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,motorRight,motorLeft,rx,ry";
    string fileName = "inputSOM.csv";
    double distance[6];



    while (vrep->simulationIsActive()) {



        if (SKILLS::visionInfo()) {
            printf("true\n");

            for (int i = 0; i < 6; i++) {
                distance[i] = SKILLS::getDistance(sensor[i]);
            }

            if (SKILLS::controlerRobot()) {
                data = to_string(distance[0]);
                data += "," + to_string(distance[1]);
                data += "," + to_string(distance[2]);
                data += "," + to_string(distance[3]);
                data += "," + to_string(distance[4]);
                data += "," + to_string(distance[5]);
                data += controlData;
                data += visionData;



                extras.logCsv(data.c_str(), fileName.c_str(), header.c_str());
            }

        }
    }

*/
}

bool ::SKILLS::visionInfo() {
    /*
    int x = -1;
    int y = -1;
    cv::Mat vrep;

    if (SKILLS::imageVrepToOpencv(Webcam, vrep)) {
      //  findCow(vrep, x, y);

        if (x != -1 && y != -1) {
            
            visionData = "," + std::to_string(x) + "," + std::to_string(y);
            printf("x: %d y: %d",x,y);
            return true;
        } else
            return false;

    }
    return false;

*/
    return true;
}

void SKILLS::takePhotos() {
   
    string nameImage = "ImageFromVrep" + to_string(countImage) + ".png";
    
    visionSensor->takePhotos(nameImage.c_str());
    
     
 
}

void SKILLS::controlTheRobot() {

    while (vrep->simulationIsActive()) {

        SKILLS::controlerRobot();

    }

}

void SKILLS::setVelocityForControler(float velocityRight, float velocityLeft) {
    int for1sec = 1000;

    SKILLS::setVelocityInRobot(velocityRight, velocityLeft);



    vrep->delay(for1sec);

    SKILLS::setVelocityInRobot(0.0, 0.0);


}

void SKILLS::setPositionForControler(int _joint, bool positive) {

    switch (_joint) {
        case 1: // joint#1
            if (positive)
                angle[0] += 0.003;
            else
                angle[0] -= 0.003;

            if (angle[0] < 0)
                angle[0] = 0;
            else if (angle[0] > 0.698131701)
                angle[0] = 0.698131701;

            vrep->setJointPosition(joint[4], angle[0]);
            controlData = "," + std::to_string(angle[0]);
            return;
        case 2: //joint#2
            if (positive)
                angle[1] += 0.0005;
            else
                angle[1] -= 0.0005;

            if (angle[1] < 0)
                angle[1] = 0;
            else if (angle[1] > 0.09)
                angle[1] = 0.09;

            vrep->setJointPosition(joint[5], angle[1]);
            controlData = "," + std::to_string(angle[1]);
            return;

        case 3: // joint#3
            if (positive)
                angle[2] += 0.01;
            else
                angle[2] -= 0.01;
            if (angle[2] < 0)
                angle[2] = 0;
            else if (angle[2] > 6.28318531)
                angle[2] = 6.28318531;

            vrep->setJointPosition(joint[6], angle[2]);
            controlData = "," + std::to_string(angle[2]);
            return;

        case 4: // joint#4
            if (positive)
                angle[3] += 0.01;
            else
                angle[3] -= 0.01;

            if (angle[3] < 0)
                angle[3] = 0;
            else if (angle[3] > 1.09955743)
                angle[3] = 1.09955743;

            vrep->setJointPosition(joint[7], angle[3]);
            controlData = "," + std::to_string(angle[3]);


    }




}

void SKILLS::trainingSOM(int size, std::string filename) {

    //setando posiçoes de leitura e escrita
    std::string subOutput = "output/" + filename + "Size:" + std::to_string(size);
    std::string dataFile = "Coleta/" + filename + ".csv";
    std::string csvHeader;

    // criando diretório
    mkdir("output", 0777);
    mkdir(subOutput.c_str(), 0777);

    SOM som(size);

    DataSet *data = new DataSet(dataFile);
    data->show();
    std::string outputFile = subOutput + "/output";
    som.setDataSet(data);
    // std::cout <<  outputFile << std::endl;



    float maxFeatureInitialValue = 0.01;
    // std::cout<< "data:: " << data->getSampleSize() << std::endl;
    som.initializeNodes(data->getSampleSize(), true, maxFeatureInitialValue);

    int iterations = 1000000;


    // Execute many iterations 
    int i = 0;

    csvHeader = "x,y," + data->features;

    som.saveNodes(outputFile, csvHeader.c_str(), false);
    while (i < iterations) {
        som.executeOneIt();
        i++;
        if (i % 1000 == 0)
            som.saveNodes(outputFile, csvHeader.c_str(), false);
    }
    std::cout << "Iteractions executed: " << iterations << std::endl;

    delete data;




}


void SKILLS::seguirParedeSOM() {
    SOM som(30);


    SKILLS::trainingSOM(18,"teste_seguirParede");


   // som.loadNodes("output/inputSOM2Size:30/output20000.csv");

    std::vector<double> input{0, 0, 0, 0, 0, 0, 0, 0};

    while (vrep->simulationIsActive()) {

        for (int i = 0; i < 6; i++) {
            float distance;
            vrep->readProximitySensor(sensor[i], distance);
            input[i] = distance;
            
        }

        som.findBest(input, 0, 5);


        SKILLS::setVelocityForControler(input[6], input[7]);


    }


}

void SKILLS::OpenTheClawCloseTheClaw(bool OpenTheClawCloseTheClaw) {




    if (OpenTheClawCloseTheClaw) {
        while (vrep->simulationIsActive()) {


            if (angle[3] < degree(41.83)) {

                angle[3] += 0.01;
                vrep->setJointPosition(joint[7], angle[3]);

            } else
                return;

            vrep->delay(100);

        }


    } else {
        while (vrep->simulationIsActive()) {

            if (angle[3] > 0) {
                angle[3] -= 0.01;
                vrep->setJointPosition(joint[7], angle[3]);
            } else
                return;

             vrep->delay(100);
        }

    }

}


void SKILLS::camilaSeguirLinha(){
 

}