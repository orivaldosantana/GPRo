/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VrepToOpencv.cpp
 * Author: samuel
 * 
 * Created on 9 de Novembro de 2017, 19:19
 */

#include "VrepToOpencv.h"


VrepToOpencv::VrepToOpencv(std::string serverIP, int serverPort, std::string nameInVrep){
    ConnectToVrep(serverIP.c_str() ,serverPort);
    connectVisionSensor(nameInVrep.c_str());
    
}

VrepToOpencv::VrepToOpencv(std::string nameInVrep, int _clientID){
    clientID = _clientID;
    
    
    
}

VrepToOpencv::VrepToOpencv(int _clientID, int _visionSensor) {
    clientID = _clientID;
    visionSensor = _visionSensor;
    debug = true;
}

void VrepToOpencv::showImage(std::string NameWindow) {
    cv::Mat image = getImage();

    if (!image.empty()) {
        cv::namedWindow(NameWindow.c_str(), cv::WINDOW_NORMAL);
        cv::imshow(NameWindow.c_str(), image);
        cv::waitKey(25);
    }




}

cv::Mat VrepToOpencv::getImage() {
    simxInt res[2]; // vai entender esse vrep  ?                                                                                                                                                                                                                                                                    não.
    simxUChar* image;
    cv::Mat imageVrep;


    int retVal = simxGetVisionSensorImage(clientID, visionSensor, res, &image, 0, simx_opmode_streaming);
    if (retVal == simx_return_ok) { // vefica se pegou uma imagem 
        imageVrep = convertVrepToOpenCV(image, res[0], res[1]);
        return imageVrep;

    }
    return imageVrep;

}

cv::Mat VrepToOpencv::convertVrepToOpenCV(simxUChar image[], simxInt resX, simxInt resY) {

    //    Mat frame = cvCreateImage(cvSize(resX, resY), 8, 3); cv::Mat frame = cv::Mat(cvSize(depth_width, depth_height), CV_8UC3)
    cv::Mat frame = cv::Mat::zeros(resY, resX, CV_8UC3);

    //from v-rep to OpenCV...
    //remeber that colors in opencv Mat are ordered as BGR
    for ( int i = 0; i < resY; i++) {
        for ( int j = 0; j < resX; j++) {
            int r, g, b;
            r = cvRound(1 * image[3 * ((resY - i) * resX + j) + 0]);
            //                cout <<"r: " << r << endl;
            g = cvRound(1 * image[3 * ((resY - i) * resX + j) + 1]);
            //                cout <<"g: " << g << endl;
            b = cvRound(1 * image[3 * ((resY - i) * resX + j) + 2]);
            //                cout <<"b: " << b << endl;

            frame.at<cv::Vec3b>(i, j)[0] = (double) b;
            frame.at<cv::Vec3b>(i, j)[1] = (double) g;
            frame.at<cv::Vec3b>(i, j)[2] = (double) r;

        }
    }

    return frame;


}

bool VrepToOpencv::findRedColorMass(double &rx, double &ry) {
    cv::Mat Vrep = getImage();


    rx = -1;
    ry = -1;
  

    if (Vrep.empty()) // se não resceber nada 
        return false;

    cv::cvtColor(Vrep, Vrep, CV_BGR2HSV);



    cv::Mat canny_output, Vrep_threshold;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::RNG rng(12345);





    inRange(Vrep, cv::Scalar(0, 0, 0), cv::Scalar(0, 255, 255), Vrep_threshold);


    cv::Mat element = cv::getStructuringElement(
            cv::MORPH_RECT,
            cv::Size(5, 5),
            cv::Point(3, 3));

    cv::erode(Vrep_threshold, Vrep_threshold, element);

    cv::erode(Vrep_threshold, Vrep_threshold, element);

    cv::dilate(Vrep_threshold, Vrep_threshold, element);


    /// Find contours
    cv::findContours(Vrep_threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    if (!contours.size()) {

        return false;
    }
    //  cout << contours.size() << endl;




    /// Get the moments
    std::vector<cv::Moments> mu(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        mu[i] = moments(contours[i], false);
    }

    ///  Get the mass centers:
    std::vector<cv::Point2f> mc(contours.size());
    for (unsigned int i = 0; i < contours.size(); i++) {
        mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
        if (mc[i].x > rx && mc[i].y > ry) {
            rx = mc[i].x;
            ry = mc[i].y;
        }
    }
    rx /= 512; ry = 1 - ry/512;
    
    //        std::cout << rx <<std::endl;
    //        std::cout << ry <<std::endl;


 if (debug) {
    /// Draw contours
    cv::Mat drawing = cv::Mat::zeros(Vrep.size(), CV_8UC3);
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
        cv::circle(drawing, mc[i], 4, color, -1, 8, 0);
    }
    cv::Mat soMassa = cv::Mat::zeros(Vrep.size(), CV_8UC3);
    for (unsigned int i = 0; i < contours.size(); i++) {
        cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //  drawContours(soMassa, contours, i, color, 2, 8, hierarchy, 0, Point());
        cv::circle(soMassa, mc[i], 4, color, -1, 8, 0);
    }


   
        printImage("Contours", drawing);
        printImage("Centro De Massa", soMassa);
        printImage("Vrep_threshold", Vrep_threshold);
        printImage("Vrep", Vrep);

    }
    return true;


}

void VrepToOpencv::printImage(std::string NameWindow, cv::Mat frame) {


    if (!frame.empty()) {
        cv::namedWindow(NameWindow.c_str(), cv::WINDOW_NORMAL);
        cv::imshow(NameWindow.c_str(), frame);
        cv::waitKey(25);
    }

}

void VrepToOpencv::connectVisionSensor(std::string nameInVrep){
      if (simxGetObjectHandle(clientID, (const simxChar*) nameInVrep.c_str(), (simxInt *) & visionSensor, (simxInt) simx_opmode_oneshot_wait) != simx_return_ok) {
        if (debug)
            std::cout << nameInVrep << " nao encontrado!" << std::endl;
        visionSensor = -1;
        
    } else {
        if (debug)
            std::cout << "Conectado ao " << nameInVrep << std::endl;

        
    }
    
    
    
}

void VrepToOpencv::ConnectToVrep(std::string serverIP, int serverPort){
    clientID = simxStart((simxChar*) serverIP.c_str(), serverPort, true, true, 2000, 5);
}

void VrepToOpencv::takePhotos(std::string photoName){
    cv::Mat image = getImage();
    
    if (! image.empty())
        cv::imwrite(photoName.c_str(), image);
    
}
void VrepToOpencv::takePhotos(std::string photoName, int sizeX, int sizeY){
    cv::Mat image = getImage();
    
    cv::resize(image,image,cv::Size(sizeX,sizeY));
    
    cv::imwrite(photoName.c_str(), image);
    
    
}