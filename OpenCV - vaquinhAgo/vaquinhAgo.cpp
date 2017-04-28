#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

Mat frame, HSV, canny, mitPunkte, mitLines, src_grey, pontoLinha;

vector<Vec4i> lParalelas;
vector<Point2f> corners;

void istInDerLinie(){
  float null, eins, zwei, drei, x, y;
  float A, B;

  for (size_t i = 0; i < corners.size(); i++) {
    x = corners[i].x;
    y = corners[i].y;

    for( size_t i = 0; i < lParalelas.size(); i++ ){
        null = lParalelas[i][0];
        eins = lParalelas[i][1];
        zwei = lParalelas[i][2];
        drei = lParalelas[i][3];

        A = (drei-eins)/(zwei-null);
        B = eins - A*null;

        if (A*x + B == y) {
          line(pontoLinha, Point(null, eins),
    				  Point(zwei, drei), Scalar(0,0,255), 5, 8 );
        }
    }
  }

  namedWindow("Detected Quinas nas Linhas", WINDOW_NORMAL);
  resizeWindow("Detected Quinas nas Linhas", 640,480);
  imshow( "Detected Quinas nas Linhas", pontoLinha);

}

void swap(int i,int j, vector<float> &a){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void quickSort(vector<float> &arr, int left, int right){
    int min = (left+right)/2;
    int i = left;
    int j = right;
    float pivot = arr[min];

    while(left<j || i<right)
    {
        while(arr[i]<pivot)
        i++;
        while(arr[j]>pivot)
        j--;

        if(i<=j){
            swap(i,j,arr);
            i++;
            j--;
        }
        else{
            if(left<j)
                quickSort(arr, left, j);
            if(i<right)
                quickSort(arr,i,right);
            return;
        }
    }
}

void select_paralelas (vector<Vec4i> &lines) {
  // TANGENTE DAS LINHAS
	vector<float> angles;
  float tang = 0;
  float null, eins, zwei, drei;

  for( size_t i = 0; i < lines.size(); i++ ){
      tang = 0;
      null = lines[i][0];
      eins = lines[i][1];
      zwei = lines[i][2];
      drei = lines[i][3];

      tang = (atan2(drei-eins, zwei-null)) * 180.0 / CV_PI;
      angles.push_back(tang);
      // TODAS OS VALORES DE TANGENTES SAO ADD NESSE VETOR
  }

  bool find_angles = false;

  if (angles.size() > 2) { // ORDENA O VETOR P DEIXAR MAIS FÁCIL DE COMPARAR
    find_angles = true;
    quickSort(angles, 0, angles.size()-1);
  }

	bool mesmo_angulo = false;

  // COMPARA OS ELEMENTOS DO VETOR, SE FOREM IGUAIS -> DESENHA
  // E ADICIONA NO VETOR lParalelas para uso posterior
  if (find_angles){
  	for (int i = 0; i < angles.size()-1; i++) {
  		if ((angles[i] - angles[i+1] < 20 || angles[i] - angles[i+1] > -20) && !mesmo_angulo) {
        lParalelas.push_back(lines[i]);
        lParalelas.push_back(lines[i+1]);
  			line( mitLines, Point(lines[i][0], lines[i][1]),
  					Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
  			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
  					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,0,255), 3, 8 );
  			mesmo_angulo = true;
  		} else if (angles[i] - angles[i+1] < 20 || angles[i] - angles[i+1] > -20) {
        lParalelas.push_back(lines[i+1]);
  			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
  					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,0,255), 3, 8 );
  		} else {
  			mesmo_angulo = false;
  		}
  	}
  }
}

void all_lines() {
	Canny(frame, canny, 50, 200, 3 );
  cvtColor(canny, mitLines, CV_GRAY2BGR );

	vector<Vec2f> lines0;
	vector<Vec4i> lines;


	//TRANSFORMADA DE HOUGH --- ACHAR LINHAS
	#if 0
		HoughLines(canny, lines0, 1, CV_PI/180, 100 );

		for( size_t i = 0; i < lines0.size(); i++ )
		{
				float rho = lines0[i][0];
				float theta = lines0[i][1];
				double a = cos(theta), b = sin(theta);
				double x0 = a*rho, y0 = b*rho;
				Point pt1(cvRound(x0 + 1000*(-b)),
									cvRound(y0 + 1000*(a)));
				Point pt2(cvRound(x0 - 1000*(-b)),
									cvRound(y0 - 1000*(a)));
				line( grey, pt1, pt2, Scalar(0,0,255), 3, 8 );
		}
	#else
		HoughLinesP( canny, lines, 1, CV_PI/180, 10, 10, 10);
    select_paralelas(lines);

		/*for( size_t i = 0; i < lines.size(); i++ ){
				line( grey, Point(lines[i][0], lines[i][1]),
						Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
		}*/

	#endif
}

void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
  erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
  dilate(thresh,thresh,dilateElement);

}

void find_corners(){
  //ACHA AS QUINAS E MARCA -> PRECISA MELHORAR E AJUSTAR
  //MELHOR A FUNCAO

  mitPunkte = src_grey.clone();
  pontoLinha = src_grey.clone();

  corners.clear();
  double qualityLevel = 0.05;
  double minDistance = 100;
  int blockSize = 3;
  bool useHarrisDetector = false;
  double k = 0.04;
  int MAX_QUINAS = 40;

  /// Apply corner detection
  goodFeaturesToTrack(mitPunkte, corners, MAX_QUINAS,
    qualityLevel, minDistance, Mat(), blockSize,
    useHarrisDetector, k);

    cvtColor(mitPunkte, mitPunkte, CV_GRAY2BGR);
    cvtColor(pontoLinha, pontoLinha, CV_GRAY2BGR);

  for( int i = 0; i < corners.size(); i++ ){
    circle(mitPunkte, corners[i], 10, Scalar(173, 0, 0), -1, 8, 0 );
    circle(mitPunkte, corners[i], 12, Scalar(0, 0, 255), -1, 8, 0 );
  }
}

  int main(){
//int main( int argc, char** argv ){

  VideoCapture capture("vaquinha_melhor.mp4");
  if ( !capture.isOpened() ){
  	cout << "Cannot open the video file. \n";
  	return -1;
  }

  //frame = imread( argv[1], 1 );

  namedWindow("vaquinhAgo", WINDOW_NORMAL);
  resizeWindow("vaquinhAgo", 640,480);

  while(1){
  	if (!capture.read(frame)) {
  		cout<<"\n Cannot read the video file. \n";
      break;
    }

    cvtColor(frame, src_grey, CV_BGR2GRAY );
		cvtColor(frame,HSV,COLOR_BGR2HSV);
		morphOps(frame);

    lParalelas.clear();

// FUNCOES PRINCIPAIS
		all_lines();
    find_corners();
    istInDerLinie();


    imshow("vaquinhAgo", frame);

    namedWindow("Detected Lines", WINDOW_NORMAL);
	  resizeWindow("Detected Lines", 640,480);
    imshow( "Detected Lines", mitLines );

    namedWindow("Detected Quinas", WINDOW_NORMAL);
	  resizeWindow("Detected Quinas", 640,480);
    imshow( "Detected Quinas", mitPunkte);

    //waitKey(0);

		if(waitKey(30) == 27){
      break;
    }
  }

  return 0;
}
