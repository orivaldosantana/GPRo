/* 
 * File:   Sample.cpp
 * Author: oriva
 * 
 * Created on 10 de Outubro de 2014, 14:20
 */

#include "Sample.h"
#include <cmath>
#include <exception>
#include "Utils.h"

Sample::Sample(){
    
}

Sample::Sample(int s) {
    for (int i = 0; i < s; i++) {
        information.push_back(0);
    }
}

Sample::Sample(std::vector<double>& info){
    information = info; 
}

Sample::Sample(const Sample& orig) {
     
    information = orig.information; 
}

Sample Sample::operator +(Sample& sIn) {
    int size = information.size(); 
    Sample r(size);
    for (int i = 0; i < size; i++) {
        r.information[i] = this->information[i] + sIn.information[i];
    }
    return r;
}

Sample Sample::operator -(Sample& sIn) {
    int size = information.size();     
    Sample r(size);
    for (int i = 0; i < size; i++) {
        r.information[i] = this->information[i] - sIn.information[i];
    }
    return r;
}

Sample Sample::operator /(double d) {
    int size = information.size();     
    Sample r(size);
    for (int i = 0; i <  size; i++) {
        r.information[i] = this->information[i] / d;
    }
    return r;
}

double Sample::distance(Sample& sIn){
   double sum = 0;
   std::vector<double>::iterator itB;
   std::vector<double>::iterator itA;
   itB = sIn.information.begin();
   for (itA = information.begin(); (itA < information.end()) and (itB < sIn.information.end()); itA++) {
      sum +=  (*itA - *itB)*(*itA - *itB);  // (ai - bi)^2 
      itB++;
   }
   return sqrt(sum);
}

double Sample::distance(std::vector<double>  &sIn, unsigned int initPos, unsigned int finalPos) {
    double sum = 0; 
    for (unsigned int i = initPos; i <= finalPos && i < sIn.size(); i++ ) {
        sum += (information[i] - sIn[i])*(information[i] - sIn[i]); 
    }
    return sqrt(sum); 
}

std::string Sample::toString() {
    std::stringstream result;
    for (unsigned int i = 0; i < information.size(); i++) {
        result << information.at(i) << " ";
    }
    std::string s = result.str(); 
    return s;
}

void Sample::print(){
    std::cout << " [" << toString() << "] "; 
}
 
void Sample::getFeatures(std::vector<double>  &info){
    info = information; 
}

void Sample::getFeaturesRange(unsigned int iStart, unsigned int iEnd, std::vector<double>& info){
    info.clear(); 
    if (( iEnd < information.size()) && (iEnd > iStart) )
        for (unsigned int i=iStart; i <= iEnd; i++)
            info.push_back(information[i]);
}

double Sample::getFeature(int index){
    double v = 0;
    try {
        v = information.at(index); 
    }
    catch (exception& e)
    {
        std::cout <<" Erro ao acessar índice do vetor de características: " << e.what() << std::endl;
    }
    return v;
}

int Sample::getSize() {
    return information.size();
}

void Sample::putNormalNoise(double s){
    for ( std::vector<double>::iterator itI = information.begin(); itI != information.end(); itI++ ) {
      *itI += Utils::normalDistribution(0,s);
   }
}

void Sample::setFeatures(std::vector<double>  &info){
    information = info; 
}

Sample::~Sample() {
}

