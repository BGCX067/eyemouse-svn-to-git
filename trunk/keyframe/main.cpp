#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <string>
using namespace std;
#include "CTrackinkSystem.h"

//
//#include "HeadPositionDebug.h"
//#include "GUI.h"
//
//#ifdef _EiC
//#define WIN32
//#endif
//void ShowEye(IplImage* eye, 	char* window );
//
//static CvMemStorage* storage = 0;
//static CvHaarClassifierCascade* cascade = 0;
//static CvHaarClassifierCascade* nested_cascade = 0;
//int use_nested_cascade = 0;
//
//void detect_and_draw( IplImage* image );
//
//const char* cascade_name ="haarcascade_frontalface_alt.xml";
//
//const char* nested_cascade_name ="haarcascade_eye.xml";
////    "../../data/haarcascades/haarcascade_eye.xml";
//double scale = 1;
//int size_factor = 8;
//
//IplImage * EyeImg =0;
//IplImage * EdgeEye =0;
//IplImage * TmpEye =0;
//IplImage * GrayEye =0;
//IplImage * PastGrayImg =0;
//IplImage * GrayImg =0;
//CvMemStorage* storage ;
//CvSeq* results;
//
//int size = 500;
//	CvCapture* capture = 0;
//int param1= 0;
//int param2= 0;
//int param3= 0;
//
//const int MAX_COUNT = 500;
//void (*func)( );
//
//
//
////====================================================================
//void match()
//{
//		CvPoint s;
//		double max, min;
//	CvPoint pmax, pmin;
//		IplImage* result;
//				IplImage* TemplateImg;
//IplImage* copy = cvCreateImage( cvSize(300,300), 8, 3 );
//IplImage* gray =  cvCreateImage( cvGetSize(copy), 8, 1);
//
//
//
//
//
//	//template size
//	int x =cvGetSize(gray).height / 25 + 1;
//	int y = x;
//
//	cvResize(EyeImg,copy,CV_INTER_LINEAR);
//	cvCvtColor( copy, gray, CV_BGR2GRAY );
//	TemplateImg = cvCreateImage( cvSize(x,y), 8, 1 );; 
//
//
//	result =cvCreateImage( cvSize(cvGetSize(gray).height -x +1,cvGetSize(gray).width -y +1),32, 1 );
//
//s.x = x/2;
//s.y = y/2;
//
////cvZero(temp);
//cvCircle( TemplateImg, s, x/2, CV_RGB(0,34,0), -1, 8,0);
////cvEqualizeHist( GrayEye, GrayEye);
//cvMatchTemplate( gray,TemplateImg,
//                     result,2);
//
//cvNormalize(result,result,1,0,CV_MINMAX,0);
// cvMinMaxLoc( result, &max, &min,&pmin,&pmax,0);
// cvCircle( copy, pmax, 3, CV_RGB(200,200,200), -1, 8,0);
//cvCircle( copy,pmin, 3, CV_RGB(200,0,0), -1, 8,0);
//
////cvEqualizeHist( result, result);
////cvNamedWindow( "yyEye", 1 );
////cvShowImage("yyEye", copy);
//
//ShowEye(copy,"yyEye");
//
//
////cvNamedWindow( "template", 1 );
////cvShowImage("template", temp);
//cvReleaseImage(& result);
//cvReleaseImage(&TemplateImg );
//cvReleaseImage(& copy);
//cvReleaseImage(& gray);
//
//}
////====================================================================
//void flow()
//{
//int win_size = 10;
//int i,k,count = 0;
//int flags = 0;
//	 IplImage* grey = GrayEye;
//			CvPoint2D32f* points[2] = {0,0};
//			
//			IplImage* copy =cvCreateImage( cvGetSize(grey), 8, 3 );
//			IplImage* copy2 =cvCreateImage( cvGetSize(grey), 8, 3 );
//            IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 );
//            IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 );
//            double quality = 0.01;
//            double min_distance = param2 + 5;
//
//			cvCopy( EyeImg,copy,0);
//				cvCopy( EyeImg,copy2,0);
//
//			count = param1 + 1;
//			       points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
//            points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
//			cvCvtColor( EyeImg, GrayEye, CV_BGR2GRAY );
//
//
//cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,
//                                   quality, min_distance, 0, 5, 0, 0.04 );
//
// 
//            for( i = k = 0; i < count; i++ )
//            {
//                cvCircle( copy, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
//            };
//
//				cvNamedWindow( "yyEye", 1 );
//cvShowImage("yyEye", copy);
//
//
//  cvFindCornerSubPix( grey, points[1], count,
//                cvSize(win_size,win_size), cvSize(-1,-1),
//                cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
//
//              for( i = k = 0; i < count; i++ )
//            {
//                cvCircle( copy2, cvPointFrom32f(points[1][i]), 3, CV_RGB(0,255,0), -1, 8,0);
//            };
//
//
//
//				cvNamedWindow( "copy2Eye", 1 );
//cvShowImage("copy2Eye", copy2);
//
//}
////====================================================================
//
//
//void diff()
//{
//	;
//}
////====================================================================
//
//void his()
//{
//
//	IplImage* r = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* g = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* b = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* s = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 3 );
//	IplConvKernel* v =  cvCreateStructuringElementEx(param3+3,param3+3,0,0,CV_SHAPE_RECT,NULL);
//
//
//	cvCvtColor( EyeImg, GrayEye, CV_BGR2GRAY );
//	cvNamedWindow( "yyEye", 1 );
//	cvEqualizeHist( GrayEye, GrayEye);
//	cvShowImage("yyEye", GrayEye);
//
//	// Split image onto the color planes.
//
//	cvMorphologyEx(EyeImg,s,NULL,v,CV_MOP_OPEN,param2);
//
//
//	cvSplit( s, r, g, b, NULL );
//	// Temporary storage.
//
//	cvEqualizeHist( r, r );
//	cvEqualizeHist( g, g );
//	cvEqualizeHist( b, b );
//
//
//	//
//	//cvAddWeighted( r, 1./3., r, 0, 0.0, r );
//	//cvAddWeighted( g, 1./3., g, 0, 0.0, g );
//	//cvAddWeighted( b, 1./3., b, 0, 0.0, b );
//
//
//	cvMerge( r,g,b,NULL,s );
//	//cvCvtColor( s, s, CV_HSV2RGB );
//	cvNamedWindow( "equalizeEye", 1 );
//	cvShowImage("equalizeEye", s);
//
//
//	cvCvtColor( s,EdgeEye, CV_BGR2GRAY );
//	cvNamedWindow( "ttEye", 1 );
//	cvShowImage("ttEye", EdgeEye);
//
//	cvThreshold( GrayEye, EdgeEye,125, 250, CV_THRESH_BINARY );
//
//
//cvAbsDiff(EdgeEye,GrayEye, TmpEye);
//	//cvSub( GrayEye,EdgeEye, TmpEye,NULL);
//	cvNamedWindow( "dif", 1 );
//	cvShowImage("dif", TmpEye);
//	// Truncate values above 100.
//
//	cvReleaseImage( &r );
//	cvReleaseImage( &g );
//	cvReleaseImage( &b );
//	cvReleaseImage( &s );
//}
//
//
//
////====================================================================
//
//void segmentation()
//{
//	IplConvKernel* v;
//	IplImage* s ;
//	IplImage* tmpi ;
//	int block_size = 1000;
//	CvSeq *comp;
//	CvMemStorage *storage;	
//	storage = cvCreateMemStorage ( block_size );
//
//	comp = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , storage);
//
//
//	s = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 3 );
//	tmpi  = cvCreateImage( cvGetSize(GrayEye),IPL_DEPTH_16S, 1 );
//
//	cvPyrSegmentation(EyeImg, s, storage, &comp, 
//		3, param1+1,5);
//
//	cvCvtColor( s, GrayEye, CV_BGR2GRAY );
//
//	cvNamedWindow( "gwEye", 1 );
//	cvShowImage("gwEye", GrayEye);
//
//	v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	cvEqualizeHist( GrayEye, GrayEye );	
//	//	cvCanny(GrayEye, EdgeEye, 100,250, 3);
//
//	cvSobel(GrayEye, EdgeEye,1,1,  3);
//
//	//cvThreshold( GrayEye, EdgeEye,param2, param3, CV_THRESH_BINARY );
//	// cvMorphologyEx(GrayEye,EdgeEye,TmpEye,v,CV_MOP_GRADIENT,2  );
//
//
//
//
//	cvNamedWindow( "cawEye", 1 );
//	cvShowImage("cawEye", tmpi);
//
//
//	//	cvCanny(GrayEye, EdgeEye, param1,param2, 3);*/
//
//	//cvPyrSegmentation(
//	//cvPyrMeanShiftFiltering( EyeImg, s, param1, param2, param3,cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1));
//	//cvWatershed(EyeImg, s);
//	cvNamedWindow( "wEye", 1 );
//	cvShowImage("wEye", s);
//
//
//
//};
//
////====================================================================
//
//void chanel()
//{
//
//	IplImage* r = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* g = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* b = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 1 );
//	IplImage* s = cvCreateImage( cvGetSize(EyeImg), IPL_DEPTH_8U, 3 );
//
//
//	cvCvtColor( EyeImg, s, CV_RGB2HSV );
//
//
//	// Split image onto the color planes.
//
//	cvSplit( s, r, g, b, NULL );
//cvEqualizeHist( g, g );
//		cvNamedWindow( "org2Eye", 1 );
//	cvShowImage("org2Eye", g);
//	// Temporary storage.
//
//	//cvEqualizeHist( r, r );
//	//cvEqualizeHist( g, g );
//	//cvEqualizeHist( b, b );
//	//cvSub(g, b ,g,0);
//cvNot( r, r );
//	cvNot( g, g );
//	cvNot(b,b );
//	cvAddWeighted(g,1,b,-1./(1.+param1),0,g);
//	cvAddWeighted(g,1,r,-1./(1.+param2),0,g);
//
//	/*	cvAddWeighted(g,1,b,-1./3,0,g);
//	cvAddWeighted(g,1,r,-1./3,0,g);*/
//
//
//
//	//cvSub(g, b ,g,0);
//
//	//cvSub(g, r ,g,0);
//
//	//	cvEqualizeHist( r, r );
////	cvEqualizeHist( g, g );
//	//cvEqualizeHist( b, b );
//
//cvThreshold( g, g,param3, 250, CV_THRESH_BINARY );
////cvThreshold( g, g,245, 250, CV_THRESH_BINARY );
//
//cvEqualizeHist(  r,  r );
//	cvNamedWindow( "1Eye", 1 );
//	cvShowImage("1Eye", r);
//	cvNamedWindow( "2Eye", 1 );
//	cvShowImage("2Eye", g);
//	cvEqualizeHist(   b,   b );
//	cvNamedWindow( "3Eye", 1 );
//	cvShowImage("3Eye", b);
//
//	//
//	//cvAddWeighted( r, param1/250., g, 0, 0.0, r );
//	//cvAddWeighted( g, param2/250., g, 0, 0.0, g );
//	//cvAddWeighted( b, param3/250., g, 0, 0.0, b );
//	// Add equally weighted rgb values.
//	//cvAddWeighted( r, param1/250., g, param2/250., 0.0, s );
//	//cvAddWeighted( s, (param1 + param2)/250., b, param3/250., 0.0, s );
//	//cvThreshold( s, s, 100, 100, CV_THRESH_TRUNC );
//
//
//	cvMerge( r,g,b,NULL,s );
//	cvCvtColor( s, s, CV_HSV2RGB );
//	cvNamedWindow( "kkkkkEye", 1 );
//	cvShowImage("kkkkkEye", s);
//
//	// Truncate values above 100.
//
//	cvReleaseImage( &r );
//	cvReleaseImage( &g );
//	cvReleaseImage( &b );
//	cvReleaseImage( &s );
//}
//
//
//
////====================================================================
//
//void tresh()
//{
//
//	IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	IplImage* laplace =cvCreateImage( cvGetSize(GrayEye),32, 1 );
//	IplImage* copy =cvCreateImage( cvGetSize(GrayEye),8, 1 );
//	cvCopy(GrayEye,copy,0);
//
//	cvEqualizeHist( copy, copy );
//	cvThreshold( copy, TmpEye,param1, 250, CV_THRESH_BINARY );
//
//	cvEqualizeHist( copy, copy );
//
//
//cvErode(copy,copy,v,param2);
//	cvDilate(copy,copy,v,param2);
//
//	//if (param3 > 2)
//	//	cvSmooth( copy, copy, CV_BLUR, 5,5,0,0 );
//
//	cvNamedWindow( "Smooth", 1 );
//	cvShowImage("Smooth", copy);
//
//
//	/*		cvEqualizeHist( GrayEye, GrayEye );
//
//	cvDilate(GrayEye,GrayEye,v,param2);
//	cvSmooth( GrayEye, EdgeEye, CV_BLUR, 5,5,0,0 );*/
//
//	cvCanny(copy, EdgeEye, 100,250, param3 +3);
//
//	//cvHoughCircles(
//	cvNamedWindow( "tEye", 1 );
//	cvShowImage("tEye", TmpEye);
//
//	cvLaplace(TmpEye,laplace,3);
//
//	cvNamedWindow( "laplace", 1 );
//	cvShowImage("laplace", laplace);
//
//	cvNamedWindow( "edgeEye", 1 );
//	cvShowImage("edgeEye", EdgeEye);
//
//}
//
////====================================================================
//
//void edge()
//{
//
//	IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	//cvEqualizeHist( GrayEye, GrayEye );
//
//	//cvDilate(GrayEye,GrayEye,v,param2);
//	//cvSmooth( GrayEye, EdgeEye, CV_BLUR, 5,5,0,0 );
//
//	cvCanny(GrayEye, EdgeEye, 10,250, 3);
//cvEqualizeHist( EdgeEye, EdgeEye );
//	cvNamedWindow( "CannyEye", 1 );
//	cvShowImage("CannyEye", EdgeEye);
//		cvNamedWindow( "eye", 1 );
//	cvShowImage("eye", GrayEye);
//
//}
////====================================================================
//
//void morf()
//{
//	IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	cvEqualizeHist( GrayEye, GrayEye );
//
//
//
//	cvDilate(GrayEye,EdgeEye,v,param1);
//
//	cvNamedWindow( "erodeEye", 1 );
//	cvShowImage("erodeEye", EdgeEye);
//
//
//	//cvMorphologyEx(GrayEye,EdgeEye,TmpEye,v,CV_MOP_OPEN,param1);
//
//	cvMorphologyEx(EdgeEye,EdgeEye,TmpEye,v,CV_MOP_GRADIENT,param2    );
//
//
//	cvNamedWindow( "trEye", 1 );
//	cvShowImage("trEye", EdgeEye);
//
//
//};
//
////====================================================================
//
////Chenge
//void process_image();
////func = morf;
//void on_param1(int i)
//{
//	//cvSetCaptureProperty(
//	//morf();
//	//tresh();
//	//edge();
//	//process_image();
////	 cvSetCaptureProperty(capture,CV_CAP_PROP_BRIGHTNESS,param1) ;
////match();
//};
//
//
//void on_param2(int i)
//{
//	//morf();
//	//edge();
//	on_param1(9);
//};
////====================================================================
//
//void ShowEye(IplImage* eye, 	char* window )
//{
//	//int x = 500;
//	int x = eye->width;
//IplImage* result;
//if (eye->nChannels == 1)
// result =cvCreateImage( cvSize(x, x),8,1);
//else
//result =cvCreateImage( cvSize(x, x),8,3);
//
//cvNamedWindow( window, 1 );
//cvResize(eye,result,CV_INTER_LINEAR);
//cvShowImage(window, result);
//cvReleaseImage(&result);
//}
//
//// Define trackbar callback functon. This function find contours,
//// draw it and approximate it by ellipses.
//void process_image()
//{
//	int x ;
//	int i;
//	IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	CvMemStorage* stor;
//	CvSeq* cont;
//	CvBox2D32f* box;
//	CvPoint* PointArray;
//	CvPoint2D32f* PointArray2D32f;
//
//	// Create dynamic structure and sequence.
//	stor = cvCreateMemStorage(0);
//	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
//
//	//cvThreshold( GrayEye, EdgeEye,param1+1, 255, CV_THRESH_BINARY );
//	cvCopy( GrayEye, EdgeEye,0 );
//
//	i = param2 + ((param2 / 2) - (int)(param2 % 2) );
//	//cvSmooth( GrayEye, EdgeEye, CV_BLUR, 3,3,0,0 );
//
////cvEqualizeHist( EdgeEye, EdgeEye );
//	// cvThreshold( EdgeEye, EdgeEye,param1+10, 255, CV_THRESH_BINARY );
//	//----------------------------------------------
//
//
//	cvCanny(GrayEye, EdgeEye, param1+1,param2+2, 3);
//
//	 x = EdgeEye->width;
//	storage = cvCreateMemStorage(0);
//	results = cvHoughCircles(
//		EdgeEye,
//		storage,
//		CV_HOUGH_GRADIENT,
//		1,
//		size/10,1,1,0,0
//		);
//
//	for(  i = 0; i < results->total; i++ ) {
//		float* p = (float*) cvGetSeqElem( results, i );
//		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
//		cvCircle(
//			EdgeEye,
//			pt,
//			cvRound( p[2] ),
//			CV_RGB(0xff,0xff,0xff),1,8,0);
//		if (i > 6) break;
//	};
//
//ShowEye( EdgeEye, "CircleEye");
//return;
//
//	//--------------------------------------------------
//
//
//
//
//	//cvEqualizeHist( GrayEye, GrayEye );
//	//cvCanny(GrayEye, EdgeEye, 10,250, 3);
//
//
//	cvNamedWindow( "CannyEye", 1 );
//	cvShowImage("CannyEye", EdgeEye);
//
//	// cvThreshold( GrayEye, EdgeEye,param1+1, 255, CV_THRESH_BINARY );
//
//	 	cvCanny(GrayEye, EdgeEye, param1,param2, 3);
//
//	//
//	//cvDilate(EdgeEye,EdgeEye,v,param2);
//	//cvErode(EdgeEye,EdgeEye,v,param2);
//
//	// cvAdaptiveThreshold(GrayEye, EdgeEye, 1,0,0,101,5);//,CV_ADAPTIVE_THRESH_MEAN_C);//,CV_THRESH_BINARY,3,5);
//	//threshold_type, block_size, offset);
//
//	cvNamedWindow( "tresh", 1 );
//	cvShowImage("tresh", EdgeEye);
//
//
//	// Find all contours.
//	cvFindContours( EdgeEye, stor, &cont, sizeof(CvContour), 
//		CV_RETR_LIST, 2, cvPoint(0,0));
//
//	// Clear images. IPL use.
//	// cvZero(image02);
//	 cvZero(TmpEye);
//
//	// This cycle draw all contours and approximate it by ellipses.
//	for(;cont;cont = cont->h_next)
//	{   
//		int i; // Indicator of cycle.
//		int count = cont->total; // This is number point in contour
//		CvPoint center;
//		CvSize size;
//
//		// Number point must be more than or equal to 6 (for cvFitEllipse_32f).        
//		if( count < 6 )
//			continue;
//
//		// Alloc memory for contour point set.    
//		PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
//		PointArray2D32f= (CvPoint2D32f*)malloc( count*sizeof(CvPoint2D32f) );
//
//		// Alloc memory for ellipse data.
//		box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
//
//		// Get contour point set.
//		cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);
//
//		// Convert CvPoint set to CvBox2D32f set.
//		for(i=0; i<count; i++)
//		{
//			PointArray2D32f[i].x = (float)PointArray[i].x;
//			PointArray2D32f[i].y = (float)PointArray[i].y;
//		}
//
//		// Fits ellipse to current contour.
//		cvFitEllipse(PointArray2D32f, count, box);
//		cvCopy(GrayEye,TmpEye,0);
//
//		// Draw current contour.
//		//cvDrawContours(TmpEye,cont,CV_RGB(255,255,255),CV_RGB(255,255,255),0,1,8,cvPoint(0,0));
//
//		// Convert ellipse data from float to integer representation.
//		center.x = cvRound(box->center.x);
//		center.y = cvRound(box->center.y);
//		size.width = cvRound(box->size.width*0.5);
//		size.height = cvRound(box->size.height*0.5);
//		box->angle = -box->angle;
//
//		// Draw ellipse.
//		cvEllipse(TmpEye, center, size,
//		box->angle, 0, 360,
//		CV_RGB(0,0,255), 1, CV_AA, 0);
//
//
//
//		// Free memory.          
//		free(PointArray);
//		free(PointArray2D32f);
//		free(box);
//	}
//
//	// Show image. HighGUI use.
//	//cvNamedWindow( "a", 1 );
//	// cvShowImage( "a", TmpEye );
//ShowEye( TmpEye, "end");
//
//};
//
////=======================================================
//
//int _main( int argc, char** argv )
//{
//	int max_val = 250;
//
//	IplImage *frame,*oldframe,*newframe, *frame_copy = 0;
//	IplImage *image = 0;
//
//
//	int i, j;
//	const char* input_name = "4.avi";
////const char* input_name = "sample.avi";
////	const char* input_name = "ja.jpg";
//	//	const char* input_name = "2.jpg";
//
//	//cvNamedWindow( "d", 1 );
//	cvNamedWindow( "Params", 1 );
//	cvCreateTrackbar("param1", "Params", &param1, max_val, on_param1);
//	cvCreateTrackbar("param2", "Params", &param2, max_val, on_param1);
//	cvCreateTrackbar("param3", "Params", &param3, max_val, on_param1);
//
//
//	cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
//
//	nested_cascade = (CvHaarClassifierCascade*)cvLoad( nested_cascade_name, 0, 0, 0 );
//
//
//	storage = cvCreateMemStorage(0);
//
//	//if( input_name )
//	//{
//		image = cvLoadImage( input_name, 1 );
//	//	if( !image )
//	//		capture = cvCaptureFromAVI( input_name );
//	//}
//
//
// capture = cvCaptureFromCAM( CV_CAP_ANY );
//
//	 cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,800) ;
//	  cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,600) ;
//	cvNamedWindow( "result", 1 );
//	cvNamedWindow( "eye", 1 );
//	//cvNamedWindow( "EdgeEye", 1 );
//newframe = 0;
//	
//oldframe = 0;
//	if( capture )
//	{
//		for(;;)
//		{
//			if( !cvGrabFrame( capture ))
//				break;
//			frame = cvRetrieveFrame( capture );
//			if( !frame )
//				break;
//			if( !frame_copy )
//				frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
//				IPL_DEPTH_8U, frame->nChannels );
//
//			if( frame->origin == IPL_ORIGIN_TL )
//				cvCopy( frame, frame_copy, 0 );
//			else
//				cvFlip( frame, frame_copy, 0 );
//
//
//			if( newframe == 0  )
//					newframe = cvCreateImage( cvSize(frame->width,frame->height),8,1);
//			cvCvtColor( frame_copy, newframe, CV_BGR2GRAY );
//
//
//			
//			if( oldframe == 0  )
//			{
//				oldframe = cvCreateImage( cvSize(frame->width,frame->height),8,1);
//				cvCopy( newframe, oldframe, 0 );
//			};
//			
//			
//			cvAbsDiff(newframe,oldframe,oldframe);
//cvThreshold(
//oldframe,
//oldframe,
//30,
//255,
//CV_THRESH_BINARY
//);
//
//
//		//cvEqualizeHist( oldframe, oldframe );
//			//cvShowImage("d", oldframe);
//			cvCopy(newframe, oldframe, 0 );
//
//			detect_and_draw( frame_copy );
//
//			
//
//			if( cvWaitKey( 10 ) >= 0 )
//				goto _cleanup_;
//		}
//
//		cvWaitKey(0);
//_cleanup_:
//		cvReleaseImage( &frame_copy );
//		cvReleaseCapture( &capture );
//	}
//
//	else
//	{
//		if( image )
//		{
//			detect_and_draw( image );
//			cvWaitKey(0);
//			cvReleaseImage( &image );
//		}
//	};
//
//	cvDestroyWindow("result");
//
//	return 0;
//}
//
//
//
////=======================================================
//CvMat a;
//CvRect* LeftEye = NULL;
//
//CvRect eyeRec;
//void detect_and_draw( IplImage* img )
//{
//
//	IplImage *gray, *small_img;
//	int i, j;
//
//	gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
//	small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
//		cvRound (img->height/scale)), 8, 1 );
//
//	cvCvtColor( img, gray, CV_BGR2GRAY );
//	cvResize( gray, small_img, CV_INTER_LINEAR );
//	cvEqualizeHist( small_img, small_img );
//	cvClearMemStorage( storage );
//
//	cvShowImage( "result", img );
//	if( cascade )
//	{
//		double t = (double)cvGetTickCount();
//		CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
//			1.1, 2, 0
//			|CV_HAAR_FIND_BIGGEST_OBJECT
//			//|CV_HAAR_DO_ROUGH_SEARCH
//			|CV_HAAR_DO_CANNY_PRUNING
//			//|CV_HAAR_SCALE_IMAGE
//			,
//			cvSize(100, 100) );
//		t = (double)cvGetTickCount() - t;
//		printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
//		for( i = 0; i < (faces ? faces->total : 0); i++ )
//		{
//			CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
//			CvMat small_img_roi;
//			CvSeq* nested_objects;
//			CvPoint center;
//			CvScalar color = {0,0,255};
//			int radius;
//
//			cvRectangle(
//				img,
//				cvPoint(r->x,r->y),
//				cvPoint(r->x+r->width,r->y+r->height),
//				color,1,8,0 );
//
//
//
//			if( !nested_cascade )
//				continue;
//			cvGetSubRect( small_img, &small_img_roi, *r );
//			nested_objects = cvHaarDetectObjects( &small_img_roi, nested_cascade, storage,
//				1.1, 20, 0
//				//|CV_HAAR_FIND_BIGGEST_OBJECT
//				//|CV_HAAR_DO_ROUGH_SEARCH
//				|CV_HAAR_DO_CANNY_PRUNING
//				//|CV_HAAR_SCALE_IMAGE
//				,
//				cvSize(10, 10) );
//
//
//			for( j = 0; j < (nested_objects ? nested_objects->total : 0); j++ )
//			{
//				CvRect* nr = (CvRect*)cvGetSeqElem( nested_objects, j );
//
//				if (LeftEye) // czy pierwsze oko
//				{
//					if (nr->x < LeftEye->x)
//					{
//						LeftEye = nr;
//						eyeRec.x = r->x +nr->x;		
//						eyeRec.y = r->y +nr->y;
//						eyeRec.width = nr->width;
//						eyeRec.height = nr->height;
//					}
//				}
//				else
//					LeftEye = nr;
//
//
//										eyeRec.x = r->x +nr->x;		
//						eyeRec.y = r->y +nr->y;
//						eyeRec.width = nr->width;
//						eyeRec.height = nr->height;
//
//				cvRectangle(
//					img,
//					cvPoint(r->x +nr->x -1,r->y +nr->y -1),
//					cvPoint(r->x +nr->x+nr->width + 1,r->y +nr->y+nr->height + 1),
//					color,1,8,0 );
//
//				center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
//				center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
//				radius = cvRound((nr->width + nr->height)*0.25*scale);
//				radius = 3;
//				//	cvCircle( img, center, radius, color, 3, 8, 0 );
//
//			}
//			cvGetSubRect(  img, &a ,eyeRec);
//
//		//	size =eyeRec.height * size_factor ;
//
//size = 300;
//size =eyeRec.height;
//			EyeImg = cvCreateImage( cvSize(size,size), 8, 3  );
//			//EdgeEye = cvCreateImage( cvSize(eyeRec.width,eyeRec.height), IPL_DEPTH_8U, 1  );
//			//GrayEye =  cvCreateImage( cvSize(eyeRec.width,eyeRec.height), IPL_DEPTH_8U, 1  );
//			EdgeEye = cvCreateImage( cvSize(size,size), 8, 1  );
//			GrayEye = cvCreateImage( cvSize(size,size), 8, 1  );
//			TmpEye = cvCreateImage( cvSize(size,size), 8, 1  );
//
//
//
//			if (eyeRec.width < 1)
//				continue;
//			cvResize( &a, EyeImg, CV_INTER_LINEAR );
////			//cvResize( &a, GrayEye, CV_INTER_LINEAR );
//
//			cvCvtColor( EyeImg, GrayEye, CV_BGR2GRAY );
//
//
//
//			GrayImg = cvCreateImage( cvSize(img->width,img->height), 8,1  );
//			cvCvtColor( img, GrayImg, CV_BGR2GRAY );
//
//
//			//	cvGetSubRect(  &small_img_roi, &a ,*LeftEye )
//
//			//process_image();
//on_param1(1);
//			//func();
//
//			//cvShowImage("EdgeEye", EdgeEye);
//
//			//cvNamedWindow( "GrayEye"S, 1 );
//			//cvShowImage("GrayEye", GrayEye);
//
//
//			if( cvWaitKey( 10 ) >= 0 )
//				exit(0);
//
//			//cvWaitKey(0);
//		}
//
//
//	}
//
//
//	//cvReleaseImage( &gray );
//	//cvReleaseImage(&EyeImg);
//	//cvReleaseImage( &small_img );
//}


int main( int argc, char** argv )
{

try
{
//

	//CHeadPositionDebug  m_pHeadPositionDebug;
	//m_pHeadPositionDebug.go(argc,argv);
g_pTrackinkSystem = new CTrackinkSystem;
//CTrackinkSystem TrackinkSystem;
cout<<"start"<<endl;
g_pTrackinkSystem->Start();
}
catch (string exp)
{
	cout<<"Exection raised:" <<exp<<endl;
};
}