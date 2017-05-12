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
vector<float> angles, anglesP;

void istInDerLinie(){
  float y1, x1, y2, x2, x, y;
  float A, B;

  for (int i = 0; i < corners.size(); i++) {
    x = corners[i].x;
    y = corners[i].y;

    for(int j = 0; j < lParalelas.size(); j++ ){
        x1 = lParalelas[j][0];
        y1 = lParalelas[j][1];
        x2 = lParalelas[j][2];
        y2 = lParalelas[j][3];

        A = (y2-y1)/(x2-x1);
        B = y1 - A*x1;

        if (A*x + B - y < 0.0005 || A*x + B - y > -0.0005) {
          if (anglesP[j] > 80 && anglesP[j] < 100){

          line(pontoLinha, Point(x1,y1),
    				  Point(x2, y2), Scalar(0,0,255), 5, 8 );
        } else {
          /*line(pontoLinha, Point(x1, y1),
    				  Point(x2, y2), Scalar(0,255,0), 5, 8 );*/
        }
      }
    }
  }
}

//no momento, essa funcao n tá sendo chamada
/*int distancia_retas_paralelas(Vec4i line1, Vec4i line2){
  // extraindo pontos line1
  // reta s
  int x1 = line1[0];
  int y1 = line1[1];
  int x2 = line1[2];
  int y2 = line1[3];
  //extraindo pontos line2 - reta t
  int x3 = line2[0];
  int y3 = line2[1];
  int x4 = line2[2];
  int y4 = line2[3];
  Vec2f r = (x3-x1,y3-y1); // reta q sai de s e vai a t
  Vec2f s = (x2-x1,y2-y1);
  cout << r[0] << endl << r[1] << endl << s[0] << endl << s[1] << endl;
  if (r[0] == s[0]) {
    return abs((int)(r[1]-s[1]));
  } else if (r[1] == s[1]) {
    return abs((int)(r[0]-s[0]));
  }
  return 50;
  float theta = 0;
  theta = acos(s[0]*r[0]+ s[1]*r[1])/(sqrt(s[0]*s[0]+s[1]*s[1])*sqrt(r[0]*r[0]+r[1]*r[1]));
  float distancia = 0;
  distancia = sqrt(r[0]*r[0]+r[1]*r[1]) * sin(theta);
}*/

void swap(int i,int j, vector<float> &a, vector<Vec4i> &lines){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;

    temp = lines[i][0];
    lines[i][0] = lines[j][0];
    lines[j][0] = temp;

    temp = lines[i][1];
    lines[i][1] = lines[j][1];
    lines[j][1] = temp;

    temp = lines[i][2];
    lines[i][2] = lines[j][2];
    lines[j][2] = temp;

    temp = lines[i][3];
    lines[i][3] = lines[j][3];
    lines[j][3] = temp;

}

void quickSort(vector<Vec4i> &lines, vector<float> &arr, int left, int right){
    int min = (left+right)/2;
    int i = left;
    int j = right;
    float pivot = arr[min];

    while(left<j || i<right) {
        while(arr[i]<pivot)
        i++;
        while(arr[j]>pivot)
        j--;

        if(i<=j){
            swap(i,j,arr, lines);
            i++;
            j--;
        }
        else{
            if(left<j)
                quickSort(lines, arr, left, j);
            if(i<right)
                quickSort(lines, arr,i,right);
            return;
        }
    }
}

void filtrar_linhas (vector<Vec4i> &lines) {
  // TANGENTE DAS LINHAS
  angles.clear();
  anglesP.clear();
  lParalelas.clear();
  double theta = 0.0;
  float y1, x1, y2, x2;

  int tam = lines.size();

  for(int i = 0; i < tam; i++ ){
      theta = 0.0;
      x1 = lines[i][0]; //x1
      y1 = lines[i][1]; //y1
      x2 = lines[i][2]; //x2
      y2 = lines[i][3]; //y2

      theta = (atan2(y2-y1, x2-x1)) * 180.0 / CV_PI;
      angles.push_back(abs(theta));
      // TODAS OS VALORES DE TANGENTES SAO ADD NESSE VETOR

  }

  bool find_angles = false;

  if (tam > 2) { // ORDENA O VETOR P DEIXAR MAIS FÁCIL DE COMPARAR
    find_angles = true;
    quickSort(lines, angles, 0, tam-1);
  }

  // COMPARA OS ELEMENTOS DO VETOR, SE FOREM IGUAIS -> DESENHA
  // E ADICIONA NO VETOR lParalelas para uso posterior
  if (find_angles){
  	for (int i = 0; i < tam-1; i=i+2) {
  		if (angles[i] - angles[i+1] < 5 && angles[i] - angles[i+1] > -5) {
          lParalelas.push_back(lines[i]);
          lParalelas.push_back(lines[i+1]);
          anglesP.push_back(angles[i]);
          anglesP.push_back(angles[i+1]);

        // COMPARAR AQUI PRA ELIMINAR LINHAS MTO PROXIMAS

        // Pensar nas seguintes situacoes ::: vaca mto perto
                                          //  vaca mto longe

        // fazer um degrade de cores pra conferir as linhas paralelas
        // havendo padrao entre as que identificam a vaca
                              // tentar identificar e tchau migs

          if (angles[i] > 85 && angles[i] < 95){
            line( mitLines, Point(lines[i][0], lines[i][1]),
      					Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
      			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
      					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,0,255), 3, 8 );

          } else {
            /*line( mitLines, Point(lines[i][0], lines[i][1]),
      					Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 3, 8 );
      			line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
      					Point(lines[i+1][2], lines[i+1][3]), Scalar(0,255,0), 3, 8 );*/
          }

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
  // ultimos tres elementos                  (threshold, minLineLength, maxLineGap)
		HoughLinesP( canny, lines, 1, CV_PI/180, 75, 50, 30);
    filtrar_linhas(lines);

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

void find_corners(){ // NAO MEXE NOS PARAMETROS PELO AMOR DE DEUS
  mitPunkte = src_grey.clone();
  pontoLinha = src_grey.clone();

  corners.clear();
  double qualityLevel = 0.05;
  double minDistance = 100;
  int blockSize = 3;
  bool useHarrisDetector = false;
  double k = 0.04;
  int MAX_QUINAS = 20;

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

  VideoCapture capture("vaquinha_melhor.mp4");
  if ( !capture.isOpened() ){
  	cout << "Cannot open the video file. \n";
  	return -1;
  }

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
		all_lines(); // pega todas as linhas e coloca num vec4f e dps chama filtrar_linhas(lines) pra selecionar só as paralelas
    find_corners(); // usa o algoritmo shi pra achar pontos de interesse (quinas)
    istInDerLinie(); // mantém as linhas que cruzam os pontos achados na funcao anterior

    namedWindow("Detected Lines", WINDOW_NORMAL);
	  resizeWindow("Detected Lines", 640,480);
    imshow( "Detected Lines", mitLines );

    namedWindow("Detected Quinas", WINDOW_NORMAL);
	  resizeWindow("Detected Quinas", 640,480);
    imshow( "Detected Quinas", mitPunkte);

    namedWindow("Detected Quinas nas Linhas", WINDOW_NORMAL);
    resizeWindow("Detected Quinas nas Linhas", 640, 480);
    imshow( "Detected Quinas nas Linhas", pontoLinha);

		if(waitKey(30) == 27){
      break;
    }
  }

  return 0;
}
