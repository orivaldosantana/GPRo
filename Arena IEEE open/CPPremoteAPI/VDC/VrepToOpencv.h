/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VrepToOpencv.h
 * Author: samuel
 *
 * Created on 9 de Novembro de 2017, 19:19
 */

#ifndef VREPTOOPENCV_H
#define VREPTOOPENCV_H
extern "C" {
#include <extApi.h>
}

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>




class VrepToOpencv {
public:
    VrepToOpencv(std::string serverIP, int serverPort,std::string nameInVrep );
    VrepToOpencv(std::string nameInVrep , int _clientID);
    VrepToOpencv(int _clientID, int _visionSensor);
    void showImage(std::string NameWindow);
    bool findRedColorMass(double &rx,double &ry);
    void printImage( std::string NameWindow, cv::Mat frame);
    void takePhotos( std::string photoName);
    void takePhotos(std::string photoName, int sizeX , int sizeY);
    int getClientID();
private:
   cv::Mat getImage(); 
   cv::Mat convertVrepToOpenCV (simxUChar image[], simxInt resX, simxInt resY);
   void ConnectToVrep( std::string serverIP, int serverPort);
   void connectVisionSensor(std::string nameInVrep);
    int clientID;
    int visionSensor;
    int debug;
};

#endif /* VREPTOOPENCV_H */

