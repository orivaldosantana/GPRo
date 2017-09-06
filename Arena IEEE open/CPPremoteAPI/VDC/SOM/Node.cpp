/* 
 * File:   Node.cpp
 * Author: oriva
 * 
 * Created on 06 de Fevereiro de 2015, 15:15
 */

#include "Node.h"
#include <sstream>

Node::Node(){
    label = "";
    description = "";
    enabled = true; 
}

Node::Node(const Node& orig) {

}

Node::Node(const Sample& s, std::string l, std::string d) {
    information = s.information;
    label = l;
    description = d;
    enabled = true; 
}

Node::~Node() {
    
}

std::string Node::getLabel() {
    return label;
}
 
void Node::setFeaturesFromSample(Sample* s) {
    information = s->information;
}

void Node::setLabel(std::string l){
    label = l; 
}
 
void Node::print() {
 
    std::cout << label << " : ";
        std::cout << toString();
    std::cout << std::endl;
}

std::string Node::toCSV() {
    
    std::string s = "";
    if (information.size() > 0 ) {
        
        std::stringstream result;
        result << information.at(0); 
        for (unsigned int i = 1; i < information.size(); i++) {
            result << "," << information.at(i) ;
        }
        s = result.str(); 

    }
    return s;
}

void Node::updateFeatures(double d, Sample* s) {
    std::vector<double>::iterator itS = s->information.begin();
    for (std::vector<double>::iterator it = information.begin(); (itS != s->information.end())
            and (it != information.end()); it++) {
        *it += d * (*itS - *it);
        itS++;
    }
}

