/**
   This file is part of Inpaint.

   Copyright Christoph Heindl 2014
   Modified by ychding11<ych_ding@163.com>
   Ioobar is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   Inpaint is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with Inpaint.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <inpaint/criminisi_inpainter.h>

#include <iostream>
#include <opencv2/opencv.hpp>

struct ImageInfo
{
	cv::Mat image;
	cv::Mat targetMask;
    cv::Mat sourceMask;
	cv::Mat displayImage;
	bool leftMouseDown;
    bool rightMouseDown;
	int patchSize;
};

void onMouse(int eventType, int x, int y, int flags, void* data)
{
    // reinterpret generic data type to specificed type.
	ImageInfo &ii = *reinterpret_cast<ImageInfo*>(data);

	if (eventType == cv::EVENT_LBUTTONDOWN)
		ii.leftMouseDown = true;
	else if (eventType == cv::EVENT_LBUTTONUP)
		ii.leftMouseDown = false;
    else if (eventType == cv::EVENT_RBUTTONDOWN)
		ii.rightMouseDown = true;
    else if (eventType == cv::EVENT_RBUTTONUP)
		ii.rightMouseDown = false;

    if (!ii.leftMouseDown && !ii.rightMouseDown)
        return;

    cv::Mat &mask = ii.leftMouseDown ? ii.targetMask : ii.sourceMask;
    cv::Scalar color = ii.leftMouseDown ? cv::Scalar(0,250,0) : cv::Scalar(0,250,250);
    // Generate the mask of the processing image.
    cv::circle(mask, cv::Point(x, y), ii.displayImage.rows / 60, cv::Scalar(255), -1);
	ii.displayImage.setTo(color, mask);	    
   // cv::imshow("Image Inpaint", ii.displayImage);
}

int main(int argc, char **argv)
{
	if (argc != 2)
    {
		std::cerr << argv[0] << " image.jpg" << std::endl;
		return -1;
	}

	cv::Mat inputImage = cv::imread(argv[1]);
    // How to check read image ok?
    printf("- Image size w=%d, h=%d.\n", inputImage.cols, inputImage.rows);
    if (inputImage.cols > 720)
    {
        cv::resize(inputImage, inputImage, cv::Size(round(0.5 * inputImage.cols),
                                                round(0.5 * inputImage.rows)));
        printf("- After resize, Image size w=%d, h=%d.\n", inputImage.cols, inputImage.rows);
    }

	ImageInfo ii;
	ii.leftMouseDown = false;
    ii.rightMouseDown = false;
	ii.patchSize = 9;

	ii.image = inputImage.clone();
	ii.displayImage = ii.image.clone();
    ii.targetMask.create(ii.image.size(), CV_8UC1);
	ii.targetMask.setTo(0); // a black picture.
    //cv::imshow("Target mask", ii.targetMask);
    ii.sourceMask.create(ii.image.size(), CV_8UC1);
	ii.sourceMask.setTo(0); // a black picture.
    //cv::imshow("Source mask", ii.sourceMask);

	//cv::namedWindow("Image Inpaint", cv::WINDOW_NORMAL);
	cv::namedWindow("Image Inpaint");
	cv::setMouseCallback("Image Inpaint", onMouse, &ii);
	cv::createTrackbar("Patchsize", "Image Inpaint", &ii.patchSize, 50);
	
	bool done = false;
	bool editingMode = true;
    int iterations = 0;

    Inpaint::CriminisiInpainter inpainter;
	cv::Mat image;	

	while (!done)
    {
		if (editingMode)
        {
			image = ii.displayImage.clone();
		}
        else
        {
			if (inpainter.hasMoreSteps())
            {
                ++iterations;
				inpainter.step();
                inpainter.image().copyTo(image); // copy temp result into image
                // Set image's target regiion to specified value.
                // The target region decrease with algorithm proceed.
                image.setTo(cv::Scalar(0,250,0), inpainter.targetRegion());
                // Only a single window is displayed.
                // check the processing result of each iteration.
               // cv::imshow("Processing", inpainter.image()); 
			}
            else
            {
                // algorithm finished.
                printf("- No more processings, iterations = %d.\n", iterations);
				ii.image = inpainter.image().clone();
				ii.displayImage = ii.image.clone();
                ii.targetMask = inpainter.targetRegion().clone();
				editingMode = true;
			}
		}

		cv::imshow("Image Inpaint", image);
		int key = cv::waitKey(10); // wait for key strok for 10 ms.

		if (key == 'x')
        {
            printf("- Done.\n");
			done = true;
		}
        else if (key == 'e')
        {
			if (editingMode)
            {
				// Was in editing, now perform
                printf("- In edit mode, set parameters.\n");

                // Check the source and target mask here
		        //cv::imshow("Target mask",ii.targetMask);
		        //cv::imshow("Source mask", ii.sourceMask);
                inpainter.setSourceImage(ii.image);
                inpainter.setTargetMask(ii.targetMask);
                inpainter.setSourceMask(ii.sourceMask);
                inpainter.setPatchSize(ii.patchSize);
                inpainter.initialize();
			} 
			editingMode = !editingMode;
		}
        else if (key == 'r')
        {
			// revert
            printf("- Revert to initial state and set to edit mode.\n");
			ii.image = inputImage.clone();
			ii.displayImage = ii.image.clone();
			ii.targetMask.create(ii.image.size(), CV_8UC1);
			ii.targetMask.setTo(0);
            ii.sourceMask.create(ii.image.size(), CV_8UC1);
			ii.sourceMask.setTo(0);
			editingMode = true;
		}
        else
        {
            //printf("- Press other keys.\n");
        }
	}

	cv::imshow("Source", inputImage);
    if (!inpainter.image().empty())
        cv::imshow("Final", inpainter.image()); // bug here.
	cv::waitKey(); // wait for key strok forever.
	//cv::imwrite("Final.png", inpainter.image());

	return 0;
}
