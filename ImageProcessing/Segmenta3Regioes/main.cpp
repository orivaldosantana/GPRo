
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#define TOP_R
#define MIN_R

#define TOP_G
#define MIN_G

#define TOP_B 240

using namespace cv;
using namespace std;

void help()
{
 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
         "Usage:\n"
         "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}

void morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat bigElement = getStructuringElement( MORPH_RECT,Size(4,4));

    Mat smallElement = getStructuringElement( MORPH_RECT,Size(3,3));

    //erode(thresh,thresh,erodeElement);
    //erode(thresh,thresh,erodeElement);

    //dilate(thresh,thresh,dilateElement);
    //dilate(thresh,thresh,dilateElement);
    
    dilate(thresh,thresh,bigElement);
    dilate(thresh,thresh,bigElement);
    erode(thresh,thresh,smallElement);
    erode(thresh,thresh,smallElement);
}


void colorFilter(Mat &frame)
{
    //NOTE: OpenCV uses BGR instead of RGB
    Vec3b channels;
    int R,G,B;
    int thresholdBlack = 60; 
    int thresholdWhite = 230; 
    int range = 55; 
    int rangeChannel = 15;

    for(int i=0; i<frame.rows; i++)
    {
        for(int j=0; j<frame.cols; j++)
        {
            channels = frame.at<Vec3b>(i,j);

            B = channels.val[0]; //Blue value
            G = channels.val[1]; //Green value
            R = channels.val[2]; //Red value

   
			if(	B>thresholdBlack-range && B<thresholdBlack+range && 
				G>thresholdBlack-range && G<thresholdBlack+range && 
				R>thresholdBlack-range && R<thresholdBlack+range &&
				abs(G-R) < rangeChannel && abs(R-B) < rangeChannel 
				)	
            {
				//cout <<"("<<i<<","<<j<<")" <<" R:" << R << " G:" << G << " B:" << B << endl; 
                //BLACK
                channels.val[0] =0; //Blue value
                channels.val[1] =0; //Green value
                channels.val[2] =0; //Red value
                frame.at<Vec3b>(i,j) = channels;
            }
            else if( B>thresholdWhite-range && B<thresholdWhite+range && 
					 G>thresholdWhite-range && G<thresholdWhite+range && 
					 R>thresholdWhite-range && R<thresholdWhite+range &&
					 abs(G-R) < rangeChannel && abs(R-B) < rangeChannel
            )
            {
                //WHITE
                channels.val[0] = 255; //Blue value
                channels.val[1] = 255; //Green value
                channels.val[2] = 255; //Red value
                frame.at<Vec3b>(i,j) = channels;
            }
            else{
                //OTHER
                channels.val[0] = 125; //Blue value
                channels.val[1] = 125; //Green value
                channels.val[2] = 125; //Red value
                frame.at<Vec3b>(i,j) = channels;
            }
        }
    }
}


int main(int argc, char** argv)
{
 string fileName = "../data/frame9.jpg"; 
 //string fileName = "../data/00000101.jpg";
 Mat src = imread( fileName.c_str(), CV_LOAD_IMAGE_COLOR);

 if(src.empty())
 {
     cout << "can not open " << fileName << endl;
     return -1;
 }

// Size size(640, 480);
// resize(src, src, size);

 //TODO make a test with histogram
 imshow("image", src);
 colorFilter(src);

 imshow("filtered", src);
 imwrite("out/filtered.jpg",src);
 
 morphOps(src); 
 imshow("morphOps", src);
 imwrite("out/morphOps.jpg",src);

 waitKey();

 return 0;
}
