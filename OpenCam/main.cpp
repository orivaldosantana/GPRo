
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>



using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	Mat image;
	int width, height;
	VideoCapture cap;
	

	cap.open(0);
	if(!cap.isOpened()){
		cout << "cameras indisponiveis";
		return -1;
	}
	
	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "largura = " << width << endl;
	cout << "altura = " << height << endl;


	while(1){
		cap >> image;
		imshow("image", image);
		
		if(waitKey(30) >= 0) break;
	}




	return 0;
}
