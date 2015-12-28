/* Demo of modified Lucas-Kanade optical flow algorithm.
See the printf below */

#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#endif

CvSeq *objectKeypoints = 0, *objectDescriptors = 0;

IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;

int win_size = 5;
const int MAX_COUNT = 5000;
CvPoint2D32f* points[3] = {0,0,0}, *swap_points;
float feature_errors[ MAX_COUNT ];
char* status = 0;
int count = 0;
int need_to_init = 1;
int night_mode = 0;
int flags = 0;
int add_remove_pt = 0;
CvPoint pt;


void on_mouse( int event, int x, int y, int flags, void* param )
{
	if( !image )
		return;

	if( image->origin )
		y = image->height - y;

	if( event == CV_EVENT_LBUTTONDOWN )
	{
		pt = cvPoint(x,y);
		add_remove_pt = 1;
	}
}



void DrawSurf()
{

	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSURFParams aa = cvSURFParams(5800, 0);


	cvExtractSURF( grey, 0, &objectKeypoints, &objectDescriptors, storage, aa );
	printf("Object Descriptors: %d\n", objectDescriptors->total);


	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq( objectKeypoints, &kreader );
	cvStartReadSeq( objectDescriptors, &reader );
	std::cout<<"point count:"<<objectDescriptors->total<<std::endl;
	for( i = 0; i < objectDescriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		kp->pt;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );

		CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
		CvPoint center;
		int radius;
		center.x = cvRound(r->pt.x);
		center.y = cvRound(r->pt.y);
		radius = cvRound(r->size/2);
		int angle = r->dir;
		//  cvCircle( object_color, center, radius, colors[0], 1, 8, 0 );



		cvCircle( image,center, radius, CV_RGB(250,0,240), 1, 8,0);




		cvLine( image, center, cvPoint( cvRound( center.x + radius*cos(angle*CV_PI/180)),
			cvRound( center.y - radius*sin(angle*CV_PI/180))), CV_RGB(250,0,240),1, CV_AA, 0 );




		//if (i > 50) break;

	};



	///* automatic initialization */
	//IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 );
	//IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 );
	//double quality = 0.01;
	//double min_distance = 2;
	//int c;

	//c = MAX_COUNT;
	//cvGoodFeaturesToTrack( grey, eig, temp, points[2], &c,
	//	quality, min_distance, 0, 3, 0, 0.04 );

	////cvFindCornerSubPix( grey, points[1], count,
	////    cvSize(win_size,win_size), cvSize(-1,-1),
	////    cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,200,0.01));

	//int k;


	//for( int i = k = 0; i < c; i++ )
	//	cvCircle( image, cvPointFrom32f(points[2][i]), 3, CV_RGB(0,0,255), -1, 8,0);
	//cvReleaseImage( &eig );
	//cvReleaseImage( &temp );
};



int main( int argc, char** argv )
{
	CvCapture* capture = 0;

	//if( argc == 1 || (argc == 2 && strlen(argv[1]) == 1 && isdigit(argv[1][0])))
	//    capture = cvCaptureFromCAM( argc == 2 ? argv[1][0] - '0' : 0 );
	//else if( argc == 2 )
	//    capture = cvCaptureFromAVI( argv[1] );


	capture = cvCaptureFromCAM( CV_CAP_ANY );
	//capture = cvCaptureFromAVI( "sample.avi" );
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,800) ;
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,600) ;

	/*	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,1200) ;
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,1600) ;*/
	cvSetCaptureProperty(capture,CV_CAP_PROP_FPS,30) ;
	//capture = cvCaptureFromAVI( "sample.avi" );



	if( !capture )
	{
		fprintf(stderr,"Could not initialize capturing...\n");
		return -1;
	}

	/* print a welcome message, and the OpenCV version */
	printf ("Welcome to lkdemo, using OpenCV version %s (%d.%d.%d)\n",
		CV_VERSION,
		CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);

	printf( "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tr - auto-initialize tracking\n"
		"\tc - delete all the points\n"
		"\tn - switch the \"night\" mode on/off\n"
		"To add/remove a feature point click it\n" );

	cvNamedWindow( "LkDemo",0);
	cvSetMouseCallback( "LkDemo", on_mouse, 0 );
	IplImage* eig ;
	IplImage* temp ;

	while (1)
	{


		for(;;)
		{

			IplImage* frame = 0;
			int i, k, c;

			frame = cvQueryFrame( capture );
			if( !frame )
				break;

			if( !image )
			{
				/* allocate all the buffers */
				image = cvCreateImage( cvGetSize(frame), 8, 3 );
				image->origin = frame->origin;
				grey = cvCreateImage( cvGetSize(frame), 8, 1 );
				prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );
				pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
				prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
				points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
				points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
				points[2] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
				status = (char*)cvAlloc(MAX_COUNT);
				flags = 0;
				eig = cvCreateImage( cvGetSize(grey), 32, 1 );
				temp = cvCreateImage( cvGetSize(grey), 32, 1 );

			}

			cvCopy( frame, image, 0 );
			cvCvtColor( image, grey, CV_BGR2GRAY );


			//	cvEqualizeHist(grey,grey);



			if( night_mode )
				cvZero( image );

			if( need_to_init )
			{
				/* automatic initialization */
				IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 );
				IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 );
				double quality = 0.01;
				double min_distance = 2;

				count = MAX_COUNT;
				cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,
					quality, min_distance, 0, 3, 0, 0.04 );

				cvFindCornerSubPix( grey, points[1], count,
					cvSize(win_size,win_size), cvSize(-1,-1),
					cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,200,0.01));
				/*  cvReleaseImage( &eig );
				cvReleaseImage( &temp );*/

				add_remove_pt = 0;
			}
			else if( count > 0 )
			{
				cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
					points[0], points[1], count, cvSize(win_size,win_size),3, status, feature_errors,
					cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,100,0.01), flags );
				flags |= CV_LKFLOW_PYR_A_READY;
				for( i = k = 0; i < count; i++ )
				{

					if( (status[i] == 1)&&( feature_errors[i] <200))
					{

						points[0][k++] = points[1][i];
						cvCircle( image, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
					}


				};
				int newCount = MAX_COUNT;


				count = k;
			}


			CV_SWAP( prev_grey, grey, swap_temp );
			CV_SWAP( prev_pyramid, pyramid, swap_temp );
			CV_SWAP( points[0], points[1], swap_points );
			need_to_init = 0;

			//	DrawSurf();
			cvShowImage( "LkDemo", image );

			c = cvWaitKey(10);
			if( (char)c == 27 )
				break;
			switch( (char) c )
			{
			case 'r':
				need_to_init = 1;
				break;
			case 'c':
				count = 0;
				break;
			case 'n':
				night_mode ^= 1;
				break;
			default:
				;
			}
		}

	}

	cvReleaseCapture( &capture );
	cvDestroyWindow("LkDemo");

	return 0;
}

#ifdef _EiC
main(1,"lkdemo.c");
#endif
