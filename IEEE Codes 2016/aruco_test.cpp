#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "aruco.h"
#include "cvdrawingutils.h"
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core/core.hpp>
#include <cv.h>
#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#include <errno.h>
#include <stdlib.h>
#include "GPIOClass.cpp"

using namespace cv;
using namespace aruco;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

int CURRENT_STATE = 0;
bool EXIT_STATE = false;

MarkerDetector MDetector;
VideoCapture TheVideoCapturer;
vector< Marker > TheMarkers;
Mat TheInputImage, TheInputImageCopy;
CameraParameters TheCameraParameters;
void cvTackBarEvents(int pos, void *);

pair< double, double > AvrgTime(0, 0); // determines the average time required for detection
 int iThresParam1, iThresParam2;
int waitTime = 0;
class CmdLineParser{int argc; char **argv; public: CmdLineParser(int _argc,char **_argv):argc(_argc),argv(_argv){}  bool operator[] ( string param ) {int idx=-1;  for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i;    return ( idx!=-1 ) ;    } string operator()(string param,string defvalue="-1"){int idx=-1;    for ( int i=0; i<argc && idx==-1; i++ ) if ( string ( argv[i] ) ==param ) idx=i; if ( idx==-1 ) return defvalue;   else  return ( argv[  idx+1] ); }};

bool GPIO_control_status(){
	string status;
    	GPIOClass* change_status = new GPIOClass("17"); //create new GPIO object to be attached to  
    	change_status->export_gpio();
    	change_status->setdir_gpio("in");

	change_status->getval_gpio(status); 
	if (status == "1") {
		CURRENT_STATE++;
		return true;
	}
	return false;
}

void sendLookAround(){
	char to_send_char [2] = {'l', ';'};
	
	FILE * pFile;

	pFile = fopen ("/dev/ttyUSB0", "w");

	cout<< to_send_char <<endl;

	fputs(to_send_char, pFile);
	fclose(pFile);
}

void sendStop(){
	char to_send_char [2] = {'p', ';'};
	
	FILE * pFile;

	pFile = fopen ("/dev/ttyUSB0", "w");

	cout<< to_send_char <<endl;

	fputs(to_send_char, pFile);
	fclose(pFile);
}

void send_SerialCommand(float erro){
	int to_send = (int)(erro*100+100);
	
	char to_send_char [3] = {'r', (char)to_send, ';'};
	
	FILE * pFile;

	pFile = fopen ("/dev/ttyUSB0", "w");

	cout<< to_send_char <<endl;

	fputs(to_send_char, pFile);
	fclose(pFile);
}

void PDControl(int xC, Mat &frame, int y){
	float erro = xC - (FRAME_WIDTH/2);
	erro = erro/(FRAME_WIDTH/2);
	int to_show;

	if (erro < 0.01) { // go left
		to_show = (int)(-100*erro);
		send_SerialCommand(erro);
		line(frame,Point((FRAME_WIDTH/2),y),Point(xC,y),Scalar(0,255,0),to_show);		
	} else if (erro > 0.01) { // go right
		to_show = (int)(100*erro);
		send_SerialCommand(erro);
		line(frame,Point((FRAME_WIDTH/2),y),Point(xC,y),Scalar(0,0,255),to_show);
	}
}


// TRACK COLOR
void drawObject(int x, int y,Mat &frame){

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	//putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}

void morphOps(Mat &thresh){
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

	Mat temp;
	threshold.copyTo(temp);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

        if(numObjects<MAX_NUM_OBJECTS){
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
                	if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
				x = moment.m10/area;
				y = moment.m01/area;
				objectFound = true;
				refArea = area;
			} else 
				objectFound = false;
			}
		
			if(objectFound == true){
				putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				drawObject(x,y,cameraFeed);
				PDControl(x, cameraFeed, y);
			} else {
				sendStop();
			}

	} else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

void cvTackBarEvents(int pos, void *) {
    (void)(pos);
    if (iThresParam1 < 3)  iThresParam1 = 3;
    if (iThresParam1 % 2 != 1)  iThresParam1++;
    if (iThresParam1 < 1)  iThresParam1 = 1;
    MDetector.setThresholdParams(iThresParam1, iThresParam2);
    // recompute
    MDetector.detect(TheInputImage, TheMarkers, TheCameraParameters);
    TheInputImage.copyTo(TheInputImageCopy);
    for (unsigned int i = 0; i < TheMarkers.size(); i++)
        TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);

    // draw a 3d cube in each marker if there is 3d info
    if (TheCameraParameters.isValid())
        for (unsigned int i = 0; i < TheMarkers.size(); i++)
            CvDrawingUtils::draw3dCube(TheInputImageCopy, TheMarkers[i], TheCameraParameters);

    cv::imshow("in", TheInputImageCopy);
    cv::imshow("thres", MDetector.getThresholdedImage());
}

bool findARUCO(int argc, char **argv) {
	try {
        CmdLineParser cml(argc,argv);
        if (argc < 2  || cml["-h"]) {
            cerr << "Invalid number of arguments" << endl;
            cerr << "Usage: (in.avi|live[:idx_cam=0]) [-c camera_params.yml] [-s  marker_size_in_meters] [-d dictionary:ARUCO by default] [-h]" << endl;
            cerr<<"\tDictionaries: "; for(auto dict:aruco::Dictionary::getDicTypes())    cerr<<dict<<" ";cerr<<endl;
            return false;
        }

        ///////////  PARSE ARGUMENTS
        string TheInputVideo = argv[1];
        // read camera parameters if passed
        if (cml["-c"] )  TheCameraParameters.readFromXMLFile(cml("-c"));
        float TheMarkerSize = std::stof(cml("-s","-1"));
        aruco::Dictionary::DICT_TYPES  TheDictionary= Dictionary::getTypeFromString( cml("-d","ARUCO") );

        ///////////  OPEN VIDEO
        // read from camera or from  file
        if (TheInputVideo.find("live") != string::npos) {
            int vIdx = 0;
            // check if the :idx is here
            char cad[100];
            if (TheInputVideo.find(":") != string::npos) {
                std::replace(TheInputVideo.begin(), TheInputVideo.end(), ':', ' ');
                sscanf(TheInputVideo.c_str(), "%s %d", cad, &vIdx);
            }
            cout << "Opening camera index " << vIdx << endl;
            TheVideoCapturer.open(vIdx);
            waitTime = 10;
        }
        else TheVideoCapturer.open(TheInputVideo);
        // check video is open
        if (!TheVideoCapturer.isOpened())  throw std::runtime_error("Could not open video");


        ///// CONFIGURE DATA
        // read first image to get the dimensions
        TheVideoCapturer >> TheInputImage;
        if (TheCameraParameters.isValid())
            TheCameraParameters.resize(TheInputImage.size());

        MDetector.setDictionary(TheDictionary);//sets the dictionary to be employed (ARUCO,APRILTAGS,ARTOOLKIT,etc)
        MDetector.setThresholdParams(7, 7);
        MDetector.setThresholdParamRange(2, 0);
        //gui requirements : the trackbars to change this parameters
        iThresParam1 = MDetector.getParams()._thresParam1;
        iThresParam2 = MDetector.getParams()._thresParam2;
        cv::namedWindow("in");
        cv::createTrackbar("ThresParam1", "in", &iThresParam1, 25, cvTackBarEvents);
        cv::createTrackbar("ThresParam2", "in", &iThresParam2, 13, cvTackBarEvents);

        //go!
        char key = 0;
        int index = 0;
        // capture until press ESC or until the end of the video
        do {

            TheVideoCapturer.retrieve(TheInputImage);
            // copy image
            double tick = (double)getTickCount(); // for checking the speed
            // Detection of markers in the image passed
            TheMarkers= MDetector.detect(TheInputImage, TheCameraParameters, TheMarkerSize);
            // chekc the speed by calculating the mean speed of all iterations
            AvrgTime.first += ((double)getTickCount() - tick) / getTickFrequency();
            AvrgTime.second++;
           // cout << "\rTime detection=" << 1000 * AvrgTime.first / AvrgTime.second << " milliseconds nmarkers=" 
            //<< TheMarkers.size() << std::endl;

            // print marker info and draw the markers in image
            TheInputImage.copyTo(TheInputImageCopy);

            for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                //cout << TheMarkers[i].getCenter() <<endl;

                Point2f center = TheMarkers[i].getCenter();
                //cout << center.x << endl;

                PDControl(center.x, TheInputImageCopy, center.y);

                TheMarkers[i].draw(TheInputImageCopy, Scalar(0, 0, 255), 1);
            }

	if (TheMarkers.size() == 0) {
		sendStop();
	}
	
	
	bool EXIT_STATE = GPIO_control_status();

            // draw a 3d cube in each marker if there is 3d info
            if (TheCameraParameters.isValid() && TheMarkerSize>0)
                for (unsigned int i = 0; i < TheMarkers.size(); i++) {
                    CvDrawingUtils::draw3dCube(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
                    CvDrawingUtils::draw3dAxis(TheInputImageCopy, TheMarkers[i], TheCameraParameters);
                }

            // DONE! Easy, right?
            // show input with augmented information and  the thresholded image
            cv::imshow("in", TheInputImageCopy);
            cv::imshow("thres", MDetector.getThresholdedImage());


            key = cv::waitKey(waitTime); // wait for key to be pressed
            if(key=='s')  waitTime= waitTime==0?10:0;
            index++; // number of images captured
        } while (key != 27 && (TheVideoCapturer.grab()) && !EXIT_STATE);

    } catch (std::exception &ex)

    {
        cout << "Exception :" << ex.what() << endl;
    }
}

void trackCOLORS(int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX) {
	bool trackObjects = true;
    bool useMorphOps = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x=0, y=0;
	//create slider bars for HSV filtering

	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if(useMorphOps)
		morphOps(threshold);
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if(trackObjects)
			trackFilteredObject(x,y,threshold,cameraFeed);

		//show frames 
		imshow(windowName2,threshold);
		imshow(windowName,cameraFeed);

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command

		bool EXIT_STATE = GPIO_control_status();

		if (EXIT_STATE)
			break;
	}
		
	waitKey(30);
}

int main(int argc, char **argv) {
	while(1) {
		EXIT_STATE = false;
		switch(CURRENT_STATE){
			case 0:
				findARUCO(argc, argv);
				break;
			case 1:
				trackCOLORS(42, 74, 0, 256, 0, 256); // HSV nessa ordem (MIN, MAX)
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			default:
				break;
		}
    }
}
