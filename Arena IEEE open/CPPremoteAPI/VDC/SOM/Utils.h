/***************************************************************************
 *   Copyright (C) 2009 by Orivaldo Santana Jr,,,   *
 *   oriva@oriva-desktop   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <limits>
#include <vector>
#include <values.h>


#define INFINITY_FLOAT 3.40282e+38
#define PI 3.1415926535

using namespace std;

// antes de fazer uma operação o elemento que vai receber o resultado precisa ser limpado
class Array{
   public:
      vector<double>* elements;
   public:
      Array(){
         elements = new vector<double>;
      }

      Array(vector<double> *e){
         elements = e;
      }

      Array(vector<double*> *e){

         elements = point2double(e);
      }

      vector<double>* point2double(vector<double*> *e){
         vector<double*>::iterator itP;
         itP = e->begin();
         while (itP != e->end())
         {
            elements->push_back( (*(*itP)) );
            itP++;
         }
         return elements;
      }

      Array(uint amounts, double value){
         elements = new vector<double>(amounts,value);
      }

      Array operator+( const Array &operand2 ) const {
         Array aResult;
         for (uint i = 0; i < elements->size() ; i++){
            aResult.elements->push_back(elements->at(i) + operand2.elements->at(i));
         }
         return aResult;
      }

      Array operator*=( const double factor ) const {
         for (uint i = 0; i < elements->size() ; i++){
            elements->at(i) *= factor;
         }
         return *this;
      }

      Array operator+=( const double factor ) const {
         for (uint i = 0; i < elements->size() ; i++){
            elements->at(i) += factor;
         }
         return *this;
      }

      Array operator-( const Array &operand2 ) const {
         Array aResult;
         for (uint i = 0; i < elements->size() ; i++){
            aResult.elements->push_back(elements->at(i) - operand2.elements->at(i));
         }
         return aResult;
      }

};

class Utils{
   public:
      static float cosLaw(float a, float b, float c){
         return (a*a - b*b - c*c)/(2*b*c);
      }
      static float distance(float x1, float y1, float x2, float y2){
         return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
      }
      static float angleFromThreePoints(float xa, float ya, float xc, float yc, float xb, float yb){
         float a = distance(xb,yb,xc,yc);
         float b = distance(xa,ya,xc,yc);
         float c = distance(xb,yb,xa,ya);
         return acos(cosLaw(c,a,b));
      }
      static float euclideanNorm(vector<float>* v) {
         float result = 0;
         for (uint i = 0; i < v->size(); i++)
            result += (v->at(i))*(v->at(i));
         return sqrt(result);
      }
      static float inline hexapodeFootHeight(float b, float g, float l2, float l3) {
         return sin(b)*l2 - sin(g - b)*l3;
      }
      static float radians(float degrees){
         return (PI/180.0)*degrees;
      }
      static float degrees(float radians){
         return radians/(PI/180);
      }
      static float infinityFloat(){
         return numeric_limits<int>::max( );
      }
      static float randomUniform(){
         return double ( rand())/double(RAND_MAX);  /* ranf() is uniform in 0..1 */
      }

      static inline float normalDistribution(float mean, float sigma){
         return boxMuller(mean, sigma);
      }

      /*  the method boxMuller implements the Polar form of the Box-Muller Transformation

      (c) Copyright 1994, Everett F. Carter Jr.
         Permission is granted by the author to use
         this function for any application provided this
         copyright notice is preserved.
         source: http://www.taygeta.com/random/gaussian.html
      */
      static float boxMuller(float m, float s)/* normal random variate generator */
      {                                       /* mean m, standard deviation s */
         float x1, x2, w, y1;
         static float y2;
         static int use_last = 0;
         if (use_last){                       /* use value from previous call */
            y1 = y2;
            use_last = 0;
         }
         else {
            do {
               x1 = 2.0 * randomUniform() - 1.0;
               x2 = 2.0 * randomUniform() - 1.0;
               w = x1 * x1 + x2 * x2;
            } while ( w >= 1.0 );
            w = sqrt( (-2.0 * log( w ) ) / w );
            y1 = x1 * w;
            y2 = x2 * w;
            use_last = 1;
         }
         return( m + y1 * s );
      }

      static string int2string( int i )
      {
         std::string s;
         std::stringstream out;
         out << i;
         return out.str();
      }

      static string float2string( float f )
      {
         std::string s;
         std::stringstream out;
         out << f;
         return out.str();
      }


      static void printVector(vector<double> *v)
      {
         for ( uint i = 0; i < v->size(); i++)
         {
            std::cout << (*v)[i] << " ";
         }
         std::cout << std::endl;
      }
      static void printVector(vector<float> *v)
      {
         for ( uint i = 0; i < v->size(); i++)
         {
            std::cout << (*v)[i] << " ";
         }
         std::cout << std::endl;
      }
      static void printVector(vector<float*> *v)
      {
         for ( uint i = 0; i < v->size(); i++)
         {
            std::cout << *(v->at(i)) << " ";
         }
         std::cout << std::endl;
      }
      static float distance(vector<float>* va, vector<float>* vb){
         float sum = 0;
         if (va == NULL)
            return sum;
         if (vb == NULL)
            return sum;
         vector<float>::iterator itA, itB;
         itB = vb->begin();
         for (itA = va->begin(); itA < va->end() or itB < vb->end(); itA++) {
            sum += Utils::distance(*itA,*itB);
            itB++;
         }
         return sqrt(sum);
      }

      inline static double distance(double a, double b) {
         return (a-b)*(a-b);
      }
      inline static float distance(float a, float b) {
         return (a-b)*(a-b);
      }
      static int float2int(float r)
      {
         if ((r - (int)r) > 0.5)
            return ((int)r + 1);
         return (int)r;
      }

      static float roundToZero(float r, float l = 1.0e-10)
      {
         if (r < l and r > -l)
            r = 0;
         return r;
      }


      static int min(int a, int b)
      {
         return ((a<b)?a:b);
      }

      static int max(int a, int b)
      {
         return ((a>b)?a:b);
      }

      static float min(float a, float b, float c)
      {
        return ((a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c));
      }

      static float DTWError(vector<float> &q, vector<float> &c, int maxq, int maxc)
      {
          /* Implemented By Hansenclever Bassani */
          int i, j;
          float *dq, *dc;
          float cost, result;

          //Alocate warp matrix
          float **DTW = new float*[maxq+1];
          for (int i=0; i< maxq+1; i++)
            DTW[i] = new float[maxc+1];

          dc = new float[maxc];
          dq = new float[maxq];

          //Initialize warp matrix and derivatives vector
          for (i = 1; i < maxq-1; i++)
          {
              DTW[i][0] = MAXFLOAT;
              dq[i] = ((q[i]-q[i-1]) + ((q[i+1]-q[i-1])/2))/2;
          }
          DTW[maxq-1][0] = DTW[maxq][0] = MAXFLOAT;
          dq[0] = dq[1];
          dq[maxq-1] = dq[maxq-2];

          //Initialize warp matrix
          for (j = 1; j < maxc-1; j++)
          {
              DTW[0][j] = MAXFLOAT;
              dc[j] = ((c[j]-c[j-1]) + ((c[j+1]-c[j-1])/2))/2; //c derivative
          }
          DTW[0][maxc-1] = DTW[0][maxc] = MAXFLOAT;
          dc[0] = dc[1];
          dc[maxc-1] = dc[maxc-2];

          DTW[0][0] = 0;  //first cost is zero

          //find warping path
          for (i = 1; i< maxq+1; i++)
           for (j = 1; j< maxc+1; j++)
           {
             cost = fabs(dq[i-1] - dc[j-1]);
             //              insertion           deletion            match
             DTW[i][j] = min(DTW[i-1][j] + cost, DTW[i][j-1] + cost, DTW[i-1][j-1] + cost);
           }

          /*DEBUG1
            FILE *debugfile = fopen("debugfilematrix.txt", "wt");
            fprintf(debugfile, "%f\t", DTW[0][0]);

            for (i = 0; i< maxc; i++)
              fprintf(debugfile, "%f\t", c[i]);

            fprintf(debugfile, "\n");

            for (i = 1; i< maxq+1; i++)
            {
              fprintf(debugfile, "%f\t", q[i-1]);
              for (j = 1; j< maxc+1; j++)
                fprintf(debugfile, "%f\t", DTW[i][j]);

              fprintf(debugfile, "\n");
            }
            fclose(debugfile);
          //*/// END DEBUG

          /*DEBUG2
            int **path = new int*[2];
            path[0] = new int[maxq+maxc];
            path[1] = new int[maxq+maxc];
            int k=0;

            i = maxq;
            j = maxc;
            while (i>0 || j>0)
            {
              path[0][k]=i-1;
              path[1][k]=j-1;
              k++;

              if (DTW[i-1][j-1]<DTW[i][j-1])
              {
                 if (DTW[i-1][j-1]<DTW[i-1][j])
                 {
                   i--; j--; //match
                 }
                 else
                   i--;
              }
              else
              {
                 if (DTW[i][j-1]<DTW[i-1][j])
                   j--; //deletion
                 else
                   i--; //insertion
              }
            }

            debugfile = fopen("debugfilelist.txt", "wt");
            if (debugfile!=NULL)
            {
               float deltai, deltaj;
               float i = 0, j = 0;
               int maxk = k-1;
               deltai = (maxq-1)/(float)maxk;
               deltaj = (maxc-1)/(float)maxk;

               for (k=maxk; k>=0; k--)
               {
      //         fprintf(debugfile, "%d\t%d\t%f\t%f\n", path[0][k], path[1][k], q[path[0][k]], c[path[1][k]]);
                 fprintf(debugfile, "%d\t%d\t%f\t%f\t%f\t%f\n", path[0][k], path[1][k], q[path[0][k]], c[path[1][k]], q[(int)i], c[(int)j]);
                 i+=deltai;
                 j+=deltaj;
               }

               fclose(debugfile);
           }
            delete[] path[0];
            delete[] path[1];
            delete[] path;
          //*/// END DEBUG

          //keep result
          result = DTW[maxq][maxc];

          //free up alocated memory
          for (int i=0; i< maxq+1; i++)
            delete[] DTW[i];
          delete[] DTW;
          delete dc;
          delete dq;

          return result;
      }

      static void copyVectors(vector<float*> *source, vector<float*> *target, uint sBegin, uint sEnd){
         if ( (source != NULL) and (target != NULL) ) {
            for (uint i = sBegin; (i < source->size()) and (i <= sEnd); i++){
                  target->push_back(source->at(i));
            }
         }
      }

};

#endif

