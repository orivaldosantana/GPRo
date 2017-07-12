#include <Skills.h>
#include <Utilities.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <functionsOpenCV.h>

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

    // atualiza velocidades dos motores
    vdc.setJointVelocity(joint[0], velocityRight);
    vdc.setJointVelocity(joint[2], velocityRight);

    vdc.setJointVelocity(joint[1], velocityLeft);
    vdc.setJointVelocity(joint[3], velocityLeft);
    //*/






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



bool SKILLS::searchTank(int &rx, int &ry) {
    Mat imageVrep;
   
    if (vdc.imageVrepToOpencv(Webcam, imageVrep)) {
        if (imageVrep.data && debug) {

            namedWindow("vrep2", CV_WINDOW_AUTOSIZE);
            imshow("vrep2", imageVrep);
            waitKey(0);

        }
     findRedColorMass(imageVrep, rx, ry);
        
        if(debug){
            cout<< "posição x do tank: " << rx  << " y: " << ry <<endl;
        }
        
  
      







    }



}

void SKILLS::WhereIsTheCow() {
    Mat image;
    if (vdc.imageVrepToOpencv(Webcam, image)) {
        findYourMother(image);
    }


    if (image.data) {
        namedWindow("CamilaCode", CV_WINDOW_AUTOSIZE);
        imshow("CamilaCode", image);
        waitKey(0);
    }


}

