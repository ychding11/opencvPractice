#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <cstdio>
#include <cstring>

using namespace std;
using namespace cv;

/// Global Variables
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 5000;
int MAX_KERNEL_LENGTH = 31;

Mat src; 
Mat dst;
char window_name[] = "Filter Demo";

/// Function headers
int display_caption( char* caption );
int display_dst( int delay );

/*
 * function main
 */
 int main( int argc, char** argv )
 {
    char info[64];
    if (2 != argc)
    {
        printf("USAGE ERROR\n");
        return -1;
    }
   //namedWindow( window_name, WINDOW_AUTOSIZE );

   src = imread(argv[1], 1 ); /// Load the source image
   if( display_caption( "Original Image" ) != 0 ) { return 0; }

   dst = src.clone();
   if( display_dst( DELAY_CAPTION ) != 0 ) { return 0; }
 
   /// Applying Median blur
   if( display_caption( "Median Blur" ) != 0 ) { return 0; }
   for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
   { 
  	medianBlur ( src, dst, i );
	sprintf(info, "Median Filter size = %d", i);
        if( display_caption(info) != 0 ) { return 0; }
        if( display_dst( DELAY_BLUR ) != 0 ) 
	{ return 0; } 
   }
    /// Applying Gaussian blur
    if( display_caption( "Gaussian Blur" ) != 0 ) { return 0; }
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    { 
        sprintf(info, "Gaussian Filter size = %d", i);
        if( display_caption(info) != 0 ) { return 0; }
	GaussianBlur( src, dst, Size( i, i ), 0, 0 );
        if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } 
    }   
   /// Wait until user press a key
   display_caption( "Finished!" );
   waitKey(0);
   return 0;
 }

 int display_caption( char* caption )
 {
   Mat dst = Mat::zeros( src.size(), src.type() );
   putText( dst, caption,
            Point( src.cols/3, src.rows/2),
            FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

   imshow( window_name, dst );
   char c = (char)waitKey( DELAY_CAPTION );
   if( 27 == c ) { return -1; } //ESC
   return 0;
  }

  int display_dst(int delay)
  {
    double scale = 0.5;
    Size dsize(dst.cols*scale, dst.rows*0.4);
    Mat scaledImg(dsize, CV_32S);
    resize(dst, scaledImg, dsize);
    imshow( window_name, scaledImg);
    char c = (char)waitKey( DELAY_CAPTION );
    if( 27 == c ) { return -1; } //ESC
    return 0; // ok
  }
