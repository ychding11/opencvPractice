#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <cstdio>

using namespace cv;
using namespace std;

void drawText(const char *text, Mat & image);

int main()
{
    cout << "Built with OpenCV " << CV_VERSION << endl;
    Mat image;
    int frame = 0;
    int frameWidth = 0;
    int frameHeight = 0;
	int writeVideo = 0;
    char keyValue = 0;
    char frameInfo[256];
    VideoCapture capture;
    VideoWriter  videosink;
    capture.open(0); // open camera
    if(capture.isOpened())
    {
        cout << "Capture opened" << endl;
        frameWidth  = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        frameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        videosink.open("video.avi", CV_FOURCC('M','J','P','G'), 15, Size(frameWidth,frameHeight), true);
        if (!videosink.isOpened())
        {
            printf("[ERR] Open Video Sink error! line:%d\n", __LINE__);
        }
	else
	{
            printf("[INFO] Open Video Sink OK!size %dx%d. \n", frameWidth, frameHeight);
	}
        for(;;)
        {
            capture >> image;
            if(image.empty())
            {
                printf("[ERR] Capture Image error line:%d\n", __LINE__);
                break;
            }
            sprintf(frameInfo, "Frame%d.jpg", frame++);
            drawText(frameInfo, image);
            imshow("Sample", image);
            keyValue = (char)waitKey(10);
            //videosink << image;
            if(keyValue >= 0) //wait for  key
            {
                if (keyValue == 27)
                {
                    printf("[INFO] Key %c ESC striked. EXIT.\n", keyValue);
                    break;
                }
                else if (keyValue == 'p')
                {
                    imwrite(frameInfo, image); //write frame to file.
                    printf("[INFO] Key %c striked. Save pic to %s!\n", keyValue, frameInfo);
                }
                else if ('v' == keyValue)
                {
                    writeVideo = writeVideo ^ 1;
		    printf("[INFO] Key %c striked. writeVideo = %d \n", keyValue, writeVideo);
                }
                else
                {
                    printf("[INFO] Key %c striked. Do Nothing!\n", keyValue);
                }
            }
            if (1 == writeVideo)
            {
                videosink.write(image); //write frame to video.
//                printf("[INFO] writeVideo flag set.  Save video to disk!\n");
            }
        }
        
    }
    else
    {
        cout << "No capture" << endl;
        image = Mat::zeros(480, 640, CV_8UC1);
        sprintf(frameInfo, "Capture error");
        drawText(frameInfo, image);
        imshow("Sample", image);
        waitKey(0); //wait key event forever
    }
    printf("Total %d Frames!\n", frame);
    return 0;
}

void drawText(const char *text, Mat & image)
{
    putText(image, text,
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}
