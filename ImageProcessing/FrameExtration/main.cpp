
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
	
	int skipped_frames =0, frame_count=0;
    bool flag = 0;

	cap.open(1);
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
		if(skipped_frames > 9)
        {
            skipped_frames = 0;
            flag = 1; // Flag set to true. Which allows to code to save the image
        }
        
        if(flag ==1){

            String name = "out/frame";

            string frame_number;          // string which will contain the final path of the file with the specific name on it
            ostringstream convert;   // stream used for the conversion
            convert << frame_count;      // insert the textual representation of 'Number' in the characters in the stream
            frame_number = convert.str(); // set 'frame_number' to the contents of the stream
            name = name + frame_number + ".jpg";
            imwrite( name, image );
            frame_count++;
            flag = 0;
        }

        skipped_frames++;
        
		
		if(waitKey(30) >= 0) break;
	}



	return 0;
}
