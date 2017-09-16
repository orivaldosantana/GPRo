extern "C" {
#include "extApi.h"
}
#include <VDC.h>
#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

VDC::VDC() {
  //  cout << "hellow world 1" << endl;


}

VDC::VDC(const VDC& orig) {
}

void VDC::conectJoints(std::string nameInVrep, int &nameInRemoteAPI) {

    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else
        std::cout << "Conectado ao " << nameInVrep << std::endl;






}

void VDC::conectProximitySensors(std::string nameInVrep, int &nameInRemoteAPI) {


    if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & nameInRemoteAPI, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok)
        std::cout << nameInVrep << " nao encontrado!" << std::endl;
    else {
        std::cout << "Conectado ao " << nameInVrep << std::endl;
        simxReadProximitySensor(clientID, nameInRemoteAPI, NULL, NULL, NULL, NULL, simx_opmode_streaming);

    }


}

void VDC::setClientID(int clientID) {
    this->clientID = clientID;
}

double VDC::getDistance(int sensor) {
    simxUChar state;
    simxFloat coord[3];

    if (simxReadProximitySensor(clientID, sensor, &state, coord, NULL, NULL, simx_opmode_buffer) == simx_return_ok) {
        if (state > 0) {
            
            return(double)coord[2];    
        } 

    }
    return 1;



}

void VDC::setJointPosition(int joint, double angle) {
    simxSetJointPosition(clientID, joint, angle, simx_opmode_oneshot);

}

void VDC::setJointVelocity(int joint, float velocity) {
    simxSetJointTargetVelocity(clientID, joint, (simxFloat) velocity, simx_opmode_streaming);


}

float VDC::getJointPosition(int joint){
    float position =-1;
    int ok ;
    int i;
    simxGetJointPosition(clientID, joint, &position ,simx_opmode_streaming);

    
  //simxGetJointPosition(clientID, joint, &position ,simx_opmode_streaming);
  
    for(i = 0; ok != simx_return_ok and i < 100000000 ; i++){
        ok = simxGetJointPosition(clientID, joint, &position ,simx_opmode_buffer); 
    }
    
    if(i>= 100000000)
        std::cout << "erro and positon = " <<  position<<std::endl;
    
    if(debug)
     std::cout << position << " i: "<< i << std::endl;

 
  return position;
    
}

void VDC::finish() {
    simxFinish(clientID);
}

bool VDC::connection_is_OK() {

    if (clientID != -1)
        return true;
    else
        return false;
}

bool VDC::simulationIsActive() {

    if (simxGetConnectionId(clientID) != -1)
        return true;
    else
        return false;


}

void VDC::delay(int time) {

    extApi_sleepMs(time);
}

int VDC::getClientID() {
    return this->clientID;
}

void VDC::setImageVisionSensor(int Webcam) {
    /*
               //... and back from OpenCV to v-rep
               for (unsigned int i = 0; i < res[1]; i++) {
                   for (unsigned int j = 0; j < res[0]; j++) {
                       float r, g, b;
                       b = (float) mitNeural.at<Vec3b>(i, j)[0];
                       g = (float) mitNeural.at<Vec3b>(i, j)[1];
                       r = (float) mitNeural.at<Vec3b>(i, j)[2];

                       image[3 * ((res[1] - i - 1) * res[0] + j) + 0] = r / 255;
                       image[3 * ((res[1] - i - 1) * res[0] + j) + 1] = g / 255;
                       image[3 * ((res[1] - i - 1) * res[0] + j) + 2] = b / 255;

                   }
               }

               bufferSize = res[0] * res[1] * 3;
               cout << "buffer: " << bufferSize << endl;



               simxSetVisionSensorImage(clientID, Webcam, image, bufferSize, 0, simx_opmode_oneshot);
               // simxSetVisionSensorImage(clientID,Webcam,image,bufferSize,0,simx_opmode_oneshot);


     */

    //   simxUChar* image;
    //    simxInt bufferSize;

    //  simxSetVisionSensorImage(clientID, Webcam, image, bufferSize, 0, simx_opmode_oneshot);
}

void VDC::readVisionSensor(int cam) {


}

Mat VDC::convertVrepToOpenCV(simxUChar image[], simxInt resX, simxInt resY) {

    Mat vrep = cvCreateImage(cvSize(resX, resY), 8, 3);
 //   Mat vrep = Mat::zeros(resY, resX, CV_8UC3);
    //from v-rep to OpenCV...
    //remeber that colors in opencv Mat are ordered as BGR
    for (int i = 0; i < resY; i++) {
        for (int j = 0; j < resX; j++) {
            int r, g, b;
            r = cvRound(1*image[3 * ((resY - i) * resX + j) + 0]);
            //                cout <<"r: " << r << endl;
            g = cvRound(1*image[3 * ((resY - i) * resX + j) + 1]);
            //                cout <<"g: " << g << endl;
            b = cvRound(1*image[3 * ((resY - i) * resX + j) + 2]);
            //                cout <<"b: " << b << endl;

            vrep.at<Vec3b>(i, j)[0] = (uchar) b;
            vrep.at<Vec3b>(i, j)[1] = (uchar) g;
            vrep.at<Vec3b>(i, j)[2] = (uchar) r;

        }
    }

    return vrep;
}

bool VDC::imageVrepToOpencv(int cam,Mat &imageVrep) {
    simxInt res[2]; // vai entender esse vrep  ?                                                                                                                                                                                                                                                                    não.
    simxUChar* image;
 
    
    //                   _______________________________________________________________________________
    //                   \  segundo o vrep, tem que mandar um arry com 2 espaços, mandei bugadamente ? /
    //                    \ Sim, mas a regra é clara, se Funcionou, não mexe.  :)                     /
    // simxUChar options;  ****************\    /*****************************************************
    // options = '0';                       \  /
    // res de resolução                      \/ 
  //  simxGetVisionSensorImage(clientID, cam, &res[0], &image, 0, simx_opmode_streaming); // a primeira chamda prepara o vision Sensor
    
    int retVal = simxGetVisionSensorImage(clientID, cam, &res[0], &image, 0, simx_opmode_streaming); 
    if (retVal == simx_return_ok) { // vefica se pegou uma imagem 
        imageVrep = VDC::convertVrepToOpenCV(image, res[0], res[1]);

        if (debug) {
            std::cout << "resolutionX: " << res[0] << " resolutionY: " << res[1] << std::endl;
            if (imageVrep.data) {
                namedWindow("Vrep", CV_WINDOW_AUTOSIZE);
                imshow("Vrep", imageVrep);
                waitKey(25);
            }
        }

        if(imageVrep.data)
            return true;
       
    }
    return false;
   


}

void VDC::setDebug(bool debug) {
    this->debug = debug;
}

VDC::~VDC() {

}

