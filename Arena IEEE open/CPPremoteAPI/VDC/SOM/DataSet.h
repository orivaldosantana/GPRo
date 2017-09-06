/* 
 * File:   DataSet.h
 * Author: oriva
 *
 * Created on 10 de Outubro de 2014, 15:32
 */

#ifndef DATASET_H
#define	DATASET_H

#include "Sample.h"
#include <cstdlib> 
#include <limits> 
#include <iostream>
#include <fstream>
#include <algorithm>

class DataSet {
public:
    DataSet         ( );
    DataSet         (std::string fn);
    DataSet         (const DataSet& orig);
    virtual ~DataSet( );

    void            generateRandomIndex         ( );
    //void            getRandomSample             (Sample &s);
    bool            getRandomSample             (Sample* &s); 
    Sample*         getRandomSample             ( ); 
    Sample*         getSample                   (unsigned int i);
    int             getSize                     ( ); 
    int             getSampleSize               ( );
    bool            isEmpty                     ( );
    void            loadDataFromFile            (std::string fileName);
    void            normalizeData               ( );
    void            putNoise                    (float                 sigma, 
                                                 unsigned int    repetitions);
    void            saveToFile                  (std::string        fileName, 
                                                 bool       randomize = true, 
                                                 bool      normalize = false);
    void            saveToFileOctaveFormat      (std::string        fileName,
                                                 unsigned int       rows = 0);
    void            savePartToFile              (std::string        fileName);
    void            show                        ( );
    void            randomizeIndex              ( );
   
    //void            resetRandomSampleIndex      ( );
    
    static int srandInc;  ///< Utilizada para incrementar o semente que aleatoriza o vetor de incices 
    std::string features; // cabeçalho para o output
//    std::vector<double> maxValues(12, std::numeric_limits<double>::max());
//    std::vector<double> minValues(12,  std::numeric_limits<double>::min());
private:
    int dataSetSize; 
    std::string fileName;
    bool empty;
    std::vector<Sample*> matrix;
    std::vector<unsigned int> index;
    int sampleI; 
    int sampleSize;
    

};

#endif	/* DATASET_H */

