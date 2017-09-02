#include <Skills.h>
#include <extras.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <functionsOpenCV.h>
#include <neuron.h>

using namespace cv;
using namespace std;


VDC vdc;

/*
SKILLS::SKILLS() {
    sensor[8] = {};
    joint[9] = {};
    angle[5] = {};



}
 */

void SKILLS::connectToRobot() {

    /* inicialização dos motores do lado esquerdo 
    NOME DO JOIN MOTOR  \/ AONDE ELE VAI GUARDAR \/                               */
    vdc.conectJoints("Motor_esquerdo_Tras", joint[1]);
    vdc.conectJoints("Motor_esquerdo_frente", joint[3]);


    // inicialização dos motores do lado direito 
    vdc.conectJoints("Motor_direito_frente", joint[0]);
    vdc.conectJoints("Motor_direito_Tras", joint[2]);



    //  juntas da GARRA
    for (int i = 4; i < 8; i++) { // 
        vdc.conectJoints("Joint#" + std::to_string(i - 3), joint[i]);


    }

    // inicialização dos sensores de proximidade  (remoteApi)
    for (int i = 0; i < 8; i++) {
        vdc.conectProximitySensors("Proximity_sensor_" + std::to_string(i + 1), sensor[i]);
    }

    vdc.conectJoints("Webcam", Webcam);






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

        dist[i] = vdc.getDistance(sensor[i]);
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



    for (int i = 4; i < 9; i++) {
        if (i == 5 && angle[1] < 0.09) //Joint#2 movimento linear
        {
            angle[1] += 0.0005;
            vdc.setJointPosition(joint[i], angle[1]);

        } else if (i == 6 && angle[2] < degree(180) && angle[1] >= 0.030) // Joint#3
        {
            angle[2] += 0.01;
            vdc.setJointPosition(joint[i], angle[2]);
        } else if (i == 4 && angle[0] < degree(40) && angle[4] >= 0.030 && angle[2] >= degree(90)) //joint#1
        {
            angle[0] += 0.003;
            vdc.setJointPosition(joint[i], angle[0]);

        }
        //
        //joint#5

        if (i == 8 && angle[4] < degree(60)) {
            angle[4] += 0.01;
            vdc.setJointPosition(joint[7], angle[4]);

        }



    }




}

void SKILLS::testGetImage() {
    Mat image;
    vdc.debug = true;
    if (vdc.imageVrepToOpencv(Webcam, image)) {

    }



}

void SKILLS::testSetImage() {
    vdc.setImageVisionSensor(Webcam);
}

void SKILLS::testReadCam() {
    vdc.readVisionSensor(Webcam);
}

void SKILLS::goToTank() {
    Mat imageVrep;
    int rx, ry;
    int q3 = 240; // dividi o eixo x da imagem em 7 quartis  
    int q5 = 400;
    float velocityRight = 1;
    float velocityLeft = 1;

    if (vdc.imageVrepToOpencv(Webcam, imageVrep)) {
        if (imageVrep.data && debug) {

            namedWindow("vrep2", CV_WINDOW_AUTOSIZE);
            imshow("vrep2", imageVrep);
            waitKey(30);

        }
        findRedColorMass(imageVrep, rx, ry);

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

void SKILLS::WhereIsTheCow() {
    Mat image;
    float rx = 123;
    float velocityLeft = 1;
    float velocityRight = 1;

    if (vdc.imageVrepToOpencv(Webcam, image)) {
        rx = findCow(image);
        cout << rx << endl;


        if (rx < 0.01)
            velocityLeft = 0;
        else if (rx > 0.01)
            velocityRight = 0;

        SKILLS::setVelocityInRobot(velocityRight, velocityLeft);

    }




}

void SKILLS::setVelocityInRobot(float velocityRight, float velocityLeft) {

    // atualiza velocidades dos motores
    vdc.setJointVelocity(joint[0], velocityRight);
    vdc.setJointVelocity(joint[2], velocityRight);

    vdc.setJointVelocity(joint[1], velocityLeft);
    vdc.setJointVelocity(joint[3], velocityLeft);
    //*/


}

void SKILLS::seguirParedeMLP() {

    vector < double > inputs(6);
    vector < double > direction(4);
    vector< int > capas = {6, 200, 4};
    double distance[4];

    int rows = 0;
    int columns = 0;
    std::fstream data;
    std::string aux;


    data.open("inputMlP.csv");

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
    std::vector < std::vector < double > > entrenador2(rows , std::vector< double >(4, 0));
    
    
    
    data.open("inputMlP.csv");
    
    std::getline(data,aux); // pula primeira linha
    
    for(int i=0; i <rows; i++){
        std::getline(data, aux);
        std::stringstream lineStream(aux);
        std::string cell;
        
        for(int j =0; j <columns;j++){
            std::getline(lineStream, cell, ',');
            if(j<6)
                entrenador1[i][j] = std::stod(cell);
            else
                entrenador2[i][j-6] = std::stod(cell);
            
            
        }
    }
    
 
    //Creamos la red
    cout << "Creando red..." << endl;
    Network red(capas);
    cout << "Red creada" << endl;

    //Entrenamos la red con los ejemplos
    cout << "Entrenando red..." << endl;
    red.Aprendizaje_Prop_Atras(entrenador1, entrenador2);
    cout << "Red entrenada" << endl;

    //red.Mostrar_Pesos(); //Mostramos los pesos definitivos

    //Mostrar los outputs:
    for (int i = 0; i < entrenador1.size(); i++) {
        red.Calcular_Output(entrenador1[i]);
        red.Mostrar_Output();
    }


    while (VDC::simulationIsActive()) {


        for (int i = 0; i < 6; i++) {

            distance[i] = VDC::getDistance(sensor[i]);


        }

        for (int i = 0; i < 6; i++) {
            inputs[i] = distance[i];
        }

        direction = red.Calcular_Output(inputs);





        for (int i = 0; i < 4; i++)
            cout << i << " " << direction[i] << endl;

        // frente 0,1,0,0
        // ré 0,0,0,1
        // direita 0,0,1,0
        // esquerda 1,0,0,0

        cout << direction[0] << endl;
        float l = 0.5;
        float vel = 1.0;
        
        if (direction[0] < l && direction[1] > l && direction[2] < l && direction[3] < l) //frente
            SKILLS::setVelocityInRobot(vel, vel);

        if (direction[0] < l && direction[1] < l && direction[2] < l && direction[3] > l) // tras
            SKILLS::setVelocityInRobot(-vel, -vel);

        if (direction[0] < l && direction[1] < l && direction[2] > l && direction[3] < l) // direta
            SKILLS::setVelocityInRobot(-vel, vel);

        if (direction[0] > l && direction[1] < l && direction[2] < l && direction[3] < l) // esquerda
            SKILLS::setVelocityInRobot(vel, -vel);


    }
}

void SKILLS::verDistancia(int i) {
    double teste = VDC::getDistance(sensor[i]);
    cout << "distancia entre 0 e 1:  " << teste << endl;

}

bool SKILLS::controlerRobot() {

    float vel = 1.0;


    //cout << "\n\n  os controles são: w ,s,a,d \n  e a letra:e para sair\n";


    system("stty raw");

    char input = getchar();

    switch (input) {
        case 'w':
            SKILLS::setVelocityForControler(vel, vel);
            controlData = ",0,1,0,0"; // frente 0,1,0,0
            return true;
        case 's':
            SKILLS::setVelocityForControler(-vel, -vel);
            controlData = ",0,0,0,1"; // ré 0,0,0,1
            return true;

        case 'd':
            SKILLS::setVelocityForControler(-vel, vel);
            controlData = ",0,0,1,0"; // direita 0,0,1,0
            return true;

        case 'a':
            SKILLS::setVelocityForControler(vel, -vel);
            controlData = ",1,0,0,0"; // esquerda 1,0,0,0
            return true;

        case 'q':
            SKILLS::takePhotos();
            countImage++;
            return true;



        case 'e':
            system("stty cooked");
            VDC::finish();
            return true;



    }

    return false;
}

void SKILLS::collectDataforMLP() {
    Extras extras;
    string data;
    string header = "sensor1,sensor2,sensor3,sensor4,sensor5,sensor6,a,w,d,s";
    string fileName = "inputMlP.csv";
    double distance[6];

    while (VDC::simulationIsActive()) {

        for (int i = 0; i < 6; i++) {
            distance[i] = VDC::getDistance(sensor[i]);
        }
        if (SKILLS::controlerRobot()) {
            data = to_string(distance[0]);
            data += "," + to_string(distance[1]);
            data += "," + to_string(distance[2]);
            data += "," + to_string(distance[3]);
            data += "," + to_string(distance[4]);
            data += "," + to_string(distance[5]);
            data += controlData;

            extras.logCsv(data.c_str(), fileName.c_str(), header.c_str());
        }

    }



}

void SKILLS::takePhotos() {
    Mat vrep;
    string nameImage = "ImageFromVrep" + to_string(countImage) + ".jpg";

    if (VDC::imageVrepToOpencv(Webcam, vrep))
        imwrite(nameImage.c_str(), vrep);
}

void SKILLS::controlTheRobot() {

    while (VDC::simulationIsActive()) {

        SKILLS::controlerRobot();

    }

}

void SKILLS::setVelocityForControler(float velocityRight, float velocityLeft) {
    int for1sec = 1000;

    SKILLS::setVelocityInRobot(velocityRight, velocityLeft);



    VDC::delay(for1sec);

    SKILLS::setVelocityInRobot(0.0, 0.0);


}