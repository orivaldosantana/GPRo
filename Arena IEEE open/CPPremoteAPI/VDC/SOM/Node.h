/* 
 * File:   Node.h
 * Author: oriva
 *
 * Created on 06 de Janeiro de 2014, 15:15
 */

#ifndef NODE_H
#define	NODE_H

#include <iostream> 
 
#include "Sample.h"
#include <list>

 

class Node: public Sample  {
     
public:
    
    Node                ( ); 
    Node                (const Node&               orig);
    Node                (const Sample                &s,
                         std::string           l = "n ",
                         std::string           d = "d "); 
    virtual ~Node();


 

 
    bool                isEnabled                   ( ) const { return enabled; }    

    std::string         getLabel                    ( ); 



    void                print                       ( );
    std::string         toCSV                       ( ); 
    void                printNeigbors               ( ); 

    void                setFeaturesFromSample       (Sample                 *s); 
    void                setLabel                    (std::string             l);




    void                updateFeatures              (double                  d,
                                                     Sample*                 s);

    void                setEnabled                  (bool enabled) { this->enabled = enabled;  }


    
     

protected:


    std::string                 label;
    std::string                 description;
    bool                        enabled; 

};

#endif	/* NODO_H */

