#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include "/home/barbosa/Libraries/aruco-2.0.10/src/aruco.h"
#include <iostream>
#include <string>
#define MAXIMO 1000
#define numPontos 100

using namespace cv;
using namespace std;
using namespace aruco;

void CentroMassaColorido(Mat &original,int objeto[][MAXIMO], int &tamanhoX, int &tamanhoY, int &quantidade, int cores[6]);
void CentroMassaPB(Mat &original, int &tamanhoX, int &tamanhoY, int &quantidade, int cor);
void binarizar(Mat &original, Mat &Binarizada, int cores[6]);
int pixels (Mat &image, int cor);
void Filtrar (Mat &imagem, int raio, int nivel);
int main(int argc, char* argv[]){
		VideoCapture cap(0); // open the video file for reading
		if ( !cap.isOpened() ){
			cout << "Cannot open the video file" << endl;
			return -1;
		}
			Mat original, hsv, filtro, imagemPB;
			cap>>original;
			  namedWindow("filtro",CV_WINDOW_AUTOSIZE);
			  namedWindow("Original",CV_WINDOW_AUTOSIZE);
			  moveWindow("filtro", 0,0);
			  moveWindow("Original", original.size().height*1.5,0);
			  int caminho [numPontos][2]={},PontoCaminho=0,X,Y, cores [6] = {75,125,0,50,0,50};

		while(1){
			double fps = cap.get(CV_CAP_PROP_FPS);
			cap>>original;
			flip(original,original,1);
			cvtColor(original, imagemPB, CV_BGR2GRAY);
			cvtColor(original, hsv, CV_BGR2HSV);
			filtro = original;
			hsv.at<Vec3b>((hsv.size().height)/2, (hsv.size().width)/2) = {0,0,0};
			Vec3b pixelI;
			pixelI.val[0] = uchar(0);
			pixelI.val[1] = uchar(0);
			pixelI.val[2] = uchar(255);
			//imshow("Centro hsv", hsv);
			cvtColor(filtro,filtro,CV_BGR2GRAY);

		  Point p;
		  int tamanhoX = 2, tamanhoY = 2, quantidade = 1,maior = 0, nobjects=0;
				binarizar(original,filtro,cores);
    	int v[MAXIMO]={};
		  Filtrar(filtro, 5, 5);
			for(int i=0; i<filtro.size().height; i++)
    		for(int j=0; j<filtro.size().width; j++)
      		if(filtro.at<uchar>(i, j) == 0){
						nobjects+=10;
						p.x=j;
						p.y=i;
						v[nobjects]=  floodFill(filtro,p,nobjects);
  				}
		  for(int n=0;n<=nobjects;n+=10){//encontra a cor que tem mais pixels
		  	if(v[maior]<v[n])
		  		maior = n;
		  }
			for (int i = 0; i < filtro.size().height; i++) 
		    for (int j = 0; j < filtro.size().width; j++) 
		    	if(filtro.at<uchar>(i, j) != maior)
		    		filtro.at<uchar>(i, j) = 255;

			CentroMassaPB(filtro,tamanhoX, tamanhoY, quantidade, maior);
	  		
    	if(v[maior]>70){
				X = (tamanhoX / quantidade);
				Y = tamanhoY / quantidade;
			  circle(filtro,Point(Y,X), 3, Scalar (125),2);
				circle(original, Point(Y,X), 3, Scalar (0,255,0),2);
			}
		  if(X>10&&Y>10){
			caminho[numPontos-1][0]=X;
			caminho[numPontos-1][1]=Y;
			}
			for(int i =1; i<numPontos; i++){
					caminho[i-1][0]=caminho[i][0];
					caminho[i-1][1]=caminho[i][1];					
			}
			for (int i = 1; i < numPontos; i++) {
				Point pi ;
				pi.y= caminho[i-1][0];
				pi.x= caminho[i-1][1];
				Point pf;
				pf.y= caminho[i][0];
				pf.x= caminho[i][1];
		    		//original.at<Vec3b>(caminho[i][0],caminho[i][1])= {0,0,255};
				line(original, pi, pf, Scalar (0,0,255));
		  }

		  imshow("filtro", filtro);			
		  //imshow("PB", imagemPB);			
			imshow("Original", original);
		  char botao = waitKey(10);
			if(botao == 27) break;
			if(botao == 32) 
				cout<<original.at<Vec3b>(original.size().height/2, original.size().width/2)<<endl;
			if(botao == 'n') cout<<"Objetos: "<<nobjects/10<<"  Numero de pixels: "<<v[maior]<<endl
				<<"tamanhoX= "<<tamanhoX<<endl
				<<"tamanhoY= "<<tamanhoY<<endl;
			if(botao == 'c') imwrite("foto.png", original);
			if(botao == 'v') imwrite("foto.png", original);
			if(botao == 'l') 
				for (int i = 0; i < numPontos; i++) {
					caminho[i][0]=0;
					caminho[i][1]=0;	
				}
			{
				/* code */
			}
			}
		return 0;
}
int pixels (Mat &image, int cor){
	int total=0;
	for(int i= 0; i<=image.size().height;i++)
		for (int j = 0; j < image.size().width; j++)
			if(image.at<uchar>(i,j)==cor)
				total++;
	return total;
}
void CentroMassaPB(Mat &original, int &tamanhoX, int &tamanhoY, int &quantidade, int cor){
			for (int i = 0; i < original.size().height; i++) 
			  for (int j = 0; j < original.size().width; j++) 
			    if ((int) original.at<uchar>(i, j) == cor) {
			    	tamanhoX += i;
			      tamanhoY += j;
			      quantidade++;
			    }
}
void binarizar(Mat &original, Mat &Binarizada, int cores[6]){
	for (int i = 0; i < original.size().height; i++) 
			  for (int j = 0; j < original.size().width; j++) {
			    if (
			    	((int) original.at<Vec3b>(i, j)[0] >= cores[0])&&
			    	((int) original.at<Vec3b>(i, j)[0] <= cores[1])&&
			    	((int) original.at<Vec3b>(i, j)[1] >= cores[2])&&
			    	((int) original.at<Vec3b>(i, j)[1] <= cores[3])&&
			    	((int) original.at<Vec3b>(i, j)[2] >= cores[4])&&
			    	((int) original.at<Vec3b>(i, j)[2] <= cores[5])) {
		        Binarizada.at<uchar>(i, j) = 0;
			    }
			    else
		        Binarizada.at<uchar>(i, j) = 255;
		    }
}
/*void CentroMassaPB(Mat &original, int &tamanhoX, int &tamanhoY, int &quantidade, int cor){
	for (int i = 0; i < original.size().height; i++) 
	  for (int j = 0; j < original.size().width; j++) 
	    if (
	    	((int) original.at<uchar>(i, j)== cor)) {
	    	tamanhoX += i;
	      tamanhoY += j;
	      quantidade++;
		    }
}*/
void Filtrar (Mat &imagem, int raio, int nivel){
      Mat element = getStructuringElement(cv::MORPH_RECT,
              cv::Size(2 * raio + 1, 2 * raio + 1),
              cv::Point(raio, raio) );
  		for(int i=1;i<=nivel;i++){
				erode(imagem,imagem,element);
      	dilate(imagem,imagem,element);
    	}
}