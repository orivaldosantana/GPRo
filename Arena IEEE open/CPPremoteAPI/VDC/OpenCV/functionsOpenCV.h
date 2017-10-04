#include <sstream>
#include <string>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/* 
 Camila Code 
 */


const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// LIMITES DOS HISTOGRAMAS
const int LIMIT_MINOR = 40;
const int LIMIT_MAJOR = 215;

Mat frame, HSV, canny, mitPunkte, mitLines, src_grey, pontoLinha, mitNeural;

vector<Vec4i> lParalelas;
vector<Point2f> corners;
vector<float> angles, anglesP;

int width = 640;
int height = 480;
float RX;
int R22x = -1;
int R22y = -1;
// ---------  COISAS PARA K-MEANS  -----------
// SET DE DADOS PARA IA

const int K = 2; // NUMERO DE CLUSTERS
const int nCICLOS = 250;



//////////////////////////////////////////////////////////////////////////////////////////////7

void PDControl(int x, int y) {
    float erro = x - (width / 2);
    erro = erro / (width / 2);
    RX = erro;
    cout << "error: " << erro << endl;
    int to_show;
    if (erro < 0.01) { // go left
        to_show = (int) (-100 * erro);
        line(frame, Point((width / 2), y), Point(x, y), Scalar(0, 255, 0), to_show);
        line(frame, Point((width / 2), y), Point(x, y), Scalar(0, 255, 0), to_show);
    } else if (erro > 0.01) { // go right
        to_show = (int) (100 * erro);
        line(frame, Point((width / 2), y), Point(x, y), Scalar(0, 0, 255), to_show);
        line(frame, Point((width / 2), y), Point(x, y), Scalar(0, 0, 255), to_show);
    }

    // QUANTO MENOR O NÚMERO, MAIOR O ALINHAMENTO DO ROBO
    // QUANTO MAIOR O NÚMERO, MAIOR A DIFERENCA ENTRE AS VELOCIDADES DOS MOTORES
    // QUANTO MENOR O NÚMERO, MAIS PRÓXIMAS SAO AS VELOCIDADES DOS MOTORES
    // TO FALANDO DA VARIÁVEL 	erro

}

void kmeans_training(vector<Point2f> corners) {
    unsigned int tam = corners.size(); // pegar tamanho de algum vetor
    int c[K][5];

    unsigned int clusters[tam];

    int trainingData[tam][5];
    /*
    [0] -> x
    [1] -> y
    [2] -> intensidade branco
    [3] -> medio
    [4] -> intensidade preto
     */

    int rows = src_grey.rows;
    int cols = src_grey.cols;

    for (unsigned int i = 0; i < tam; i++) {
        trainingData[i][0] = corners[i].x;
        trainingData[i][1] = corners[i].y;

        //GET HISTOGRAMAS



        int x1 = trainingData[i][0] - 5, x2 = trainingData[i][0] + 5, y1 = trainingData[i][1] - 5, y2 = trainingData[i][1] + 5;

        if (x1 < 0) {
            x1 = 0;
        }

        if (x2 > cols) {
            x2 = cols - 1;
        }

        if (y1 < 0) {
            y1 = 0;
        }

        if (y2 > rows) {
            y2 = rows - 1;
        }

        //Mat img = Mat(src_grey, Rect(x1,y1,x2,y2));


        trainingData[i][2] = 0;
        trainingData[i][3] = 0;
        trainingData[i][4] = 0;

        MatIterator_<uchar> it, end;
        for (it = src_grey.begin<uchar>(), end = src_grey.end<uchar>(); it != end; ++it) {

            if (*it <= LIMIT_MINOR) {
                trainingData[i][2]++;
            } else if (*it < LIMIT_MAJOR) {
                trainingData[i][3]++;
            } else {
                trainingData[i][4]++;
            }
        }
    }

    for (int n = 0; n < nCICLOS; n++) {

        // atribui aos primeiros centroides o centroide anterior identificado

        for (int j = 0; j < 5; j++) {
            c[0][j] = trainingData[0][j];
            c[1][j] = trainingData[tam - 1][j];
        }


        /*c[0][0] = centroides[0][0];
        c[0][1] = centroides[0][1];
        c[0][2] = centroides[0][2];
        c[0][3] = centroides[0][3];
        c[0][4] = centroides[0][4];
        c[1][0] = centroides[1][0];
        c[1][1] = centroides[1][1];
        c[1][2] = centroides[1][2];
        c[1][3] = centroides[1][3];
        c[1][4] = centroides[1][4];*/

        // DATA ASSIGMENT STEP - cada ponto é associado a um centroide
        for (unsigned int i = 0; i < tam; i++) {
            double distancia = sqrt((trainingData[i][0] - c[0][0])*(trainingData[i][0] - c[0][0]) +
                    (trainingData[i][1] - c[0][1])*(trainingData[i][1] - c[0][1]) +
                    (trainingData[i][2] - c[0][2])*(trainingData[i][2] - c[0][2]) +
                    (trainingData[i][3] - c[0][3])*(trainingData[i][3] - c[0][3]) +
                    (trainingData[i][4] - c[0][4])*(trainingData[i][4] - c[0][4]));
            clusters[i] = 0;
            for (int j = 1; j < K; j++) {
                if (sqrt((trainingData[i][0] - c[j][0])*(trainingData[i][0] - c[j][0]) +
                        (trainingData[i][1] - c[j][1])*(trainingData[i][1] - c[j][1]) +
                        (trainingData[i][2] - c[j][2])*(trainingData[i][2] - c[j][2]) +
                        (trainingData[i][3] - c[j][3])*(trainingData[i][3] - c[j][3]) +
                        (trainingData[i][4] - c[j][4])*(trainingData[i][4] - c[j][4])) < distancia) {

                    clusters[i] = j;

                    distancia = sqrt((trainingData[i][0] - c[j][0])*(trainingData[i][0] - c[j][0]) +
                            (trainingData[i][1] - c[j][1])*(trainingData[i][1] - c[j][1]) +
                            (trainingData[i][2] - c[j][2])*(trainingData[i][2] - c[j][2]) +
                            (trainingData[i][3] - c[j][3])*(trainingData[i][3] - c[j][3]) +
                            (trainingData[i][4] - c[j][4])*(trainingData[i][4] - c[j][4]));


                }
            }
        }


        // CENTROID UPDATE STEP - os centroides sao recomputados

        unsigned int cont = 0;
        for (unsigned int i = 0; i < K; i++) {
            c[i][0] = 0;
            c[i][1] = 0;
            c[i][2] = 0;
            c[i][3] = 0;
            c[i][4] = 0;
            cont = 0;
            for (unsigned int j = 0; j < tam; j++) {
                if (clusters[j] == i) {
                    c[i][0] = c[i][0] + trainingData[j][0];
                    c[i][1] = c[i][1] + trainingData[j][1];
                    c[i][2] = c[i][2] + trainingData[j][2];
                    c[i][3] = c[i][3] + trainingData[j][3];
                    c[i][4] = c[i][4] + trainingData[j][4];
                    cont++;
                }
            }

            c[i][0] = int(c[i][0] / cont);
            c[i][1] = int(c[i][1] / cont);
            c[i][2] = int(c[i][2] / cont);
            c[i][3] = int(c[i][3] / cont);
            c[i][4] = int(c[i][4] / cont);

        }



    }


    // ajustar retorno

    /*centroides[0][0] = c[0][0];
    centroides[0][1] = c[0][1];
    centroides[0][2] = c[0][2];
    centroides[0][3] = c[0][3];
    centroides[0][4] = c[0][4];
    centroides[1][0] = c[1][0];
    centroides[1][1] = c[1][1];
    centroides[1][2] = c[1][2];
    centroides[1][3] = c[1][3];
    centroides[1][4] = c[1][4];*/

    mitNeural = frame.clone();
    circle(mitNeural, Point(c[0][0], c[0][1]), 10, Scalar(173, 0, 0), -1, 8, 0);
    circle(mitNeural, Point(c[0][0], c[0][1]), 12, Scalar(0, 0, 255), -1, 8, 0);

    circle(mitNeural, Point(c[1][0], c[1][1]), 10, Scalar(173, 0, 0), -1, 8, 0);
    circle(mitNeural, Point(c[1][0], c[1][1]), 12, Scalar(0, 0, 255), -1, 8, 0);

    line(mitNeural, Point(c[0][0], 0), Point(c[0][0], rows), Scalar(0, 0, 255), 5, 8);
    line(mitNeural, Point(c[1][0], 0), Point(c[1][0], rows), Scalar(0, 0, 255), 5, 8);

    int X = int((c[0][0] + c[1][0]) / 2.0);
    int Y = int((c[0][1] + c[1][1]) / 2.0);

    // PDControl(X, Y);
    R22x = X;
    R22y = Y;


    circle(mitNeural, Point(X, Y), 20, Scalar(255, 0, 0), -1, 8, 0);
    circle(mitNeural, Point(X, Y), 25, Scalar(255, 0, 0), -1, 8, 0);

}

void istInDerLinie() {
    float y1, x1, y2, x2, x, y;
    float A, B;

    int n = 0;

    vector<Point2f>pontos;

    for (unsigned int i = 0; i < corners.size(); i++) {
        x = corners[i].x;
        y = corners[i].y;

        for (unsigned int j = 0; j < lParalelas.size(); j++) {
            x1 = lParalelas[j][0];
            y1 = lParalelas[j][1];
            x2 = lParalelas[j][2];
            y2 = lParalelas[j][3];

            A = (y2 - y1) / (x2 - x1);
            B = y1 - A*x1;


            if (A * x + B - y < 5 || A * x + B - y > -5) {
                if (anglesP[j] > 80 && anglesP[j] < 100) {
                    line(pontoLinha, Point(x1, y1),
                            Point(x2, y2), Scalar(0, 0, 255), 5, 8);
                    circle(pontoLinha, Point(x1, y1), 5, Scalar(0, 255, 0), -1, 8, 0);

                    circle(pontoLinha, Point(x2, y2), 5, Scalar(0, 255, 0), -1, 8, 0);

                    pontos.push_back(Point(x1, y1));
                    pontos.push_back(Point(x2, y2));

                    n = n + 2;

                } else {
                    /*line(pontoLinha, Point(x1, y1),
                                            Point(x2, y2), Scalar(0,255,0), 5, 8 );*/
                }
            }
        }

    }
    // CHAMAR ML

    /*if (pontos.size() > 10) {
        kmeans_training(pontos);
    }*/

}

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

void swap(int i, int j, vector<float> &a, vector<Vec4i> &lines) {
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

void quickSort(vector<Vec4i> &lines, vector<float> &arr, int left, int right) {
    int min = (left + right) / 2;
    int i = left;
    int j = right;
    float pivot = arr[min];

    while (left < j || i < right) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;

        if (i <= j) {
            swap(i, j, arr, lines);
            i++;
            j--;
        } else {
            if (left < j)
                quickSort(lines, arr, left, j);
            if (i < right)
                quickSort(lines, arr, i, right);
            return;
        }
    }
}

void filtrar_linhas(vector<Vec4i> &lines) {
    // TANGENTE DAS LINHAS
    angles.clear();
    anglesP.clear();
    lParalelas.clear();
    double theta = 0.0;
    float y1, x1, y2, x2;

    int tam = lines.size();

    for (int i = 0; i < tam; i++) {
        theta = 0.0;
        x1 = lines[i][0]; //x1
        y1 = lines[i][1]; //y1
        x2 = lines[i][2]; //x2
        y2 = lines[i][3]; //y2

        theta = (atan2(y2 - y1, x2 - x1)) * 180.0 / CV_PI;
        angles.push_back(abs(theta));
        // TODAS OS VALORES DE TANGENTES SAO ADD NESSE VETOR

    }

    vector<Point2f>pontos;

    bool find_angles = false;

    if (tam > 2) { // ORDENA O VETOR P DEIXAR MAIS FÁCIL DE COMPARAR
        find_angles = true;
        quickSort(lines, angles, 0, tam - 1);
    }

    // COMPARA OS ELEMENTOS DO VETOR, SE FOREM IGUAIS -> DESENHA
    // E ADICIONA NO VETOR lParalelas para uso posterior
    if (find_angles) {
        for (int i = 0; i < tam - 1; i = i + 2) {
            if (angles[i] - angles[i + 1] < 5 && angles[i] - angles[i + 1] > -5) {
                lParalelas.push_back(lines[i]);
                lParalelas.push_back(lines[i + 1]);
                anglesP.push_back(angles[i]);
                anglesP.push_back(angles[i + 1]);

                // COMPARAR AQUI PRA ELIMINAR LINHAS MTO PROXIMAS

                // Pensar nas seguintes situacoes ::: vaca mto perto
                //  vaca mto longe

                // fazer um degrade de cores pra conferir as linhas paralelas
                // havendo padrao entre as que identificam a vaca
                // tentar identificar e tchau migs

                if (angles[i] > 85 && angles[i] < 95) {
                    line(mitLines, Point(lines[i][0], lines[i][1]),
                            Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
                    line(mitLines, Point(lines[i + 1][0], lines[i + 1][1]),
                            Point(lines[i + 1][2], lines[i + 1][3]), Scalar(0, 0, 255), 3, 8);

                    pontos.push_back(Point(lines[i][0], lines[i][1]));
                    pontos.push_back(Point(lines[i + 1][0], lines[i + 1][1]));


                } else {
                    /*line( mitLines, Point(lines[i][0], lines[i][1]),
                                                Point(lines[i][2], lines[i][3]), Scalar(0,255,0), 3, 8 );
                                line( mitLines, Point(lines[i+1][0], lines[i+1][1]),
                                                Point(lines[i+1][2], lines[i+1][3]), Scalar(0,255,0), 3, 8 );*/
                }

            }
        }
    }



    /*if (pontos.size() > 10) {
        kmeans_training(pontos);
    }*/

}

void all_lines() {
    Canny(frame, canny, 50, 200, 3);

    cvtColor(canny, mitLines, CV_GRAY2BGR);

    vector<Vec2f> lines0;
    vector<Vec4i> lines;


    //TRANSFORMADA DE HOUGH --- ACHAR LINHAS
#if 0
    HoughLines(canny, lines0, 1, CV_PI / 180, 100);
    for (size_t i = 0; i < lines0.size(); i++) {
        float rho = lines0[i][0];
        float theta = lines0[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Point pt1(cvRound(x0 + 1000 * (-b)),
                cvRound(y0 + 1000 * (a)));
        Point pt2(cvRound(x0 - 1000 * (-b)),
                cvRound(y0 - 1000 * (a)));
        line(grey, pt1, pt2, Scalar(0, 0, 255), 3, 8);
    }
#else
    // ultimos tres elementos                  (threshold, minLineLength, maxLineGap)

    HoughLinesP(canny, lines, 1, CV_PI / 180, 75, 50, 30);

    /*Mat hough = src_grey;
    for (unsigned int i = 0; i < lines.size(); i++){
      line( hough, Point(lines[i][0], lines[i][1]),
          Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }
    namedWindow("Hough Lines", WINDOW_NORMAL);
          resizeWindow("Hough Lines", 640,480);
    imshow( "Hough Lines", hough );*/


    filtrar_linhas(lines);

    /*for( size_t i = 0; i < lines.size(); i++ ){
                    line( grey, Point(lines[i][0], lines[i][1]),
                                    Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
    }*/

#endif
}

void morphOps(Mat &thresh) {

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);

    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);

}

void find_corners() { // NAO MEXE NOS PARAMETROS PELO AMOR DE DEUS
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

    for (unsigned int i = 0; i < corners.size(); i++) {
        circle(mitPunkte, corners[i], 10, Scalar(173, 0, 0), -1, 8, 0);
        circle(mitPunkte, corners[i], 12, Scalar(0, 0, 255), -1, 8, 0);

        circle(pontoLinha, corners[i], 10, Scalar(255, 0, 0), -1, 8, 0);
        circle(pontoLinha, corners[i], 12, Scalar(255, 0, 0), -1, 8, 0);

    }

    if (corners.size() > 10) {
        kmeans_training(corners);
    }
}

void findCow(Mat image, int &R21x, int &R21y) {

    frame = image;

    cvtColor(frame, src_grey, CV_BGR2GRAY);
    cvtColor(frame, HSV, COLOR_BGR2HSV);
    morphOps(frame);

    lParalelas.clear();

    // FUNCOES PRINCIPAIS
    // pega todas as linhas e coloca num vec4f e dps chama filtrar_linhas(lines) pra selecionar só as paralelas
    find_corners(); // usa o algoritmo shi pra achar pontos de interesse (quinas)
    istInDerLinie(); // mantém as linhas que cruzam os pontos achados na funcao anterior



    if (!mitNeural.empty()) {
        namedWindow("Mit Neural", CV_WINDOW_AUTOSIZE);
        imshow("Mit Neural", mitNeural);


        waitKey(25);


    }

    R21x = R22x;
    R21y = R22y;








}

/*
 Samuel Code
 */

void massCenter(int, void*, Mat image) {

    RNG rng(12345);
    int thresh = 100;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny(image, canny_output, thresh, thresh * 2, 3);

    if (image.data) {

        namedWindow("vrep2", CV_WINDOW_AUTOSIZE);
        imshow("vrep2", image);
        waitKey(0);

    }

    /// Find contours
    findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (!contours.size())
        return;

    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        mu[i] = moments(contours[i], false);
    }

    ///  Get the mass centers:
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
    }

    /// Draw contours
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 8, 0);
    }


    if (drawing.data) {
        /// Show in a window
        namedWindow("Contours", CV_WINDOW_AUTOSIZE);
        imshow("Contours", drawing);
    }





}

void findRedColorMass(Mat Vrep, int &rx, int &ry) {

    rx = -1;
    ry = -1;


    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);



    namedWindow("Object Detection", WINDOW_NORMAL);
    inRange(Vrep, Scalar(0, 0, 0), Scalar(21, 255, 255), Vrep);
    imshow("Object Detection", Vrep);



    /// Find contours
    findContours(Vrep, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    if (!contours.size())
        return;


    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        mu[i] = moments(contours[i], false);
    }

    ///  Get the mass centers:
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++) {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
        if (mc[i].x > rx && mc[i].y > ry) {
            rx = mc[i].x;
            ry = mc[i].y;
        }
    }





    cout << rx << " " << ry << endl;
    /// Draw contours
    Mat drawing = Mat::zeros(Vrep.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(drawing, mc[i], 4, color, -1, 8, 0);
    }
    Mat soMassa = Mat::zeros(Vrep.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //  drawContours(soMassa, contours, i, color, 2, 8, hierarchy, 0, Point());
        circle(soMassa, mc[i], 4, color, -1, 8, 0);
    }

    if (drawing.data) {
        /// Show in a window
        namedWindow("Contours", CV_WINDOW_AUTOSIZE);
        imshow("Contours", drawing);
        namedWindow("Centro De Massa", CV_WINDOW_AUTOSIZE);
        imshow("Centro De Massa", soMassa);
    }

    waitKey(25); // 25 frames por segundo



}
