#include <Skills.h>
#include <Utilities.h>
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

    vector < double > inputs(4);
    vector < double > vel_motores(2);
    vector< int > capas = {4, 15, 15, 15, 2};
    double distance[4];

    //Vectores de entrenamiento: input (entrenador1) y output deseado (entrenador2)
    //Orden de los sensores: {0,1,2,3,4,5} 
    vector< vector< double > > entrenador1 = {
        {0.0442136, 0.0442136, 0.0442136, 0.0442136}, //1
        {0.0442136, 0.0442136, 0.0442136, 0.247024}, //2
        {0.0442136, 0.0442136, 0.247024, 0.0442136}, //3
        {0.0442136, 0.0442136, 0.247024, 0.247024}, //4
        {0.0442136, 0.247024, 0.0442136, 0.0442136}, //5
        {0.0442136, 0.247024, 0.0442136, 0.247024}, //6
        {0.0442136, 0.247024, 0.247024, 0.0442136}, //7
        {0.0442136, 0.247024, 0.247024, 0.247024}, //8
        {0.247024, 0.0442136, 0.0442136, 0.0442136}, //9
        {0.247024, 0.0442136, 0.0442136, 0.247024}, //10
        {0.247024, 0.0442136, 0.247024, 0.0442136}, //11
        {0.247024, 0.0442136, 0.247024, 0.247024}, //12
        {0.247024, 0.247024, 0.0442136, 0.0442136}, //13
        {0.247024, 0.247024, 0.0442136, 0.247024}, //14
        {0.247024, 0.247024, 0.247024, 0.0442136}, //15
        {0.247024, 0.0442136, 0.247024, 0.247024}, //16
    };
    //Orden de los motores: {a,b}, a-> motor derecho, b->motor izquierdo
    vector< vector< double > > entrenador2 = {
        {0, 0}, //1
        {0.5, -0.5}, //2
        {-0.5, 0.5}, //3
        {0.75, 0}, //4
        {-0.5, 0.5}, //5
        {-0.5, 0.5}, //6 */
        {1, 1}, //7
        {1, 0.5}, //8
        {0.5, -0.5}, //9
        {-1, -1}, //10/
        {0.75, -0.25}, //11
        {0.5, -0.5}, //12
        {-0.25, 0.75}, //13
        {-0.25, 0.75}, //14
        {-0.25, 0.75}, //15*/
        {-0.25, 0.75}//16
    };



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

    for (int i = 0; i < 4; i++) {

        distance[i] = VDC::getDistance(sensor[i]);


    }

    for (int i = 0; i < 4; i++) {
        inputs[i] = distance[i];
    }

    vel_motores = red.Calcular_Output(inputs);

    //El output de las redes neuronales es un valor entre 0 y 1. Queremos mapearlo para que vaya de
    // -2 (motores giran hacia atras) a 2 (motores giran hacia delante).
    vel_motores[0] -= 0.5;
    vel_motores[1] -= 0.5;
    vel_motores[0] *= 4;
    vel_motores[1] *= 4;

    
    setVelocityInRobot(-vel_motores[0],-vel_motores[1]);
}