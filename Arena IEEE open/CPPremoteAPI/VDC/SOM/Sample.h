/* 
 * File:   Sample.h
 * Author: oriva
 *
 * Created on 10 de Outubro de 2014, 14:20
 */

#include <vector>
#include <string>
#include <sstream>
 

#ifndef SAMPLE_H
#define	SAMPLE_H

class Sample {
    friend class Node; 
 
public:
    Sample(); 
    Sample(int s);
    Sample(std::vector<double>  &info); 
    Sample(const Sample& orig);
    virtual ~Sample();
    

    Sample              operator+                 (Sample                &sIn);
    Sample              operator-                 (Sample                &sIn);
    Sample              operator/                 (double                   d);

    double              distance                  (Sample                &sIn);
    double              distance                  (std::vector<double>   &sIN, 
                                                   unsigned int       initPos, 
                                                   unsigned int       finalPos); 
    int                 getSize                   ( );
    void                getFeatures               (std::vector<double>  &info); 
    void                getFeaturesRange          (unsigned int iStart, unsigned int iEnd, std::vector<double>  &info); 
    double              getFeature                (int index); 
 
    void                putNormalNoise            (double                   s);
    void                print                     ( );
 
    void                setFeatures               (std::vector<double>  &info);
    std::string         toString                  ( );        

protected:
    std::vector<double>     information;
 
    
    
};

#endif	/* SAMPLE_H */

