#include "GazeDetector.h"

#include "CFaceExtractor.h"
#include "assert.h"
#include "cv.h"
#include "cvaux.h"
#include "cv.hpp"
#include "highgui.h"
#include "highgui.hpp"
#include "GUI.h"
#include "Utilitis.h"
#include "CTrackinkSystem.h"
#include <vector>

#include "ransac_ellipse.h"
#include "remove_corneal_reflection.h"
//#include "BlobResult.h"

using namespace std;

CGazeDetector::CGazeDetector(void)
{

	m_UpdateTresholdValueCount = 0;
	m_TresholdValue1 = -1;
	m_TresholdValue2  =-1;
	m_LeftTresholdValue1 = -1;
	m_LeftTresholdValue2 = -1;
	m_RightTresholdValue1 = -1;
	m_RightTresholdValue2 = -1;
	m_MemStorage = cvCreateMemStorage(0);
	m_PupilRadious = -1;

	int a = 6;
	CvSize ImgSize = cvSize( 100 * a,60 * a);

	ImgCopy =cvCreateImage( ImgSize, 8, 3);
	gray =  cvCreateImage( ImgSize, 8, 1);
	TreshImg =  cvCreateImage( ImgSize, 8, 1);

	m_pConturSeq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , m_MemStorage);
	m_pConturSeqBegin = m_pConturSeq;
}

CGazeDetector::~CGazeDetector(void)
{
	cvReleaseMemStorage(&m_MemStorage);
}



//===========================================================================

bool CGazeDetector::IsEyeFuter(CTrackingFuture * pFuter)
{

	CvRect LeftRoi;
	/*			LeftRoi.x = g_TrackinkSystem->M_FaceExtractor.m_pLeftEyeImg->m_ROI.x /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	LeftRoi.y = g_TrackinkSystem->M_FaceExtractor.m_pLeftEyeImg->m_ROI.y /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	LeftRoi.width = g_TrackinkSystem->M_FaceExtractor.m_pLeftEyeImg->m_ROI.width /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	LeftRoi.height = g_TrackinkSystem->M_FaceExtractor.m_pLeftEyeImg->m_ROI.height /g_TrackinkSystem->M_FaceExtractor.m_fScale;*/

	LeftRoi = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect;

	CvRect RightRoi;
	/*	RightRoi.x = g_TrackinkSystem->M_FaceExtractor.m_pRightEyeImg->m_ROI.x /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	RightRoi.y = g_TrackinkSystem->M_FaceExtractor.m_pRightEyeImg->m_ROI.y /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	RightRoi.width = g_TrackinkSystem->M_FaceExtractor.m_pRightEyeImg->m_ROI.width /g_TrackinkSystem->M_FaceExtractor.m_fScale;
	RightRoi.height = g_TrackinkSystem->M_FaceExtractor.m_pRightEyeImg->m_ROI.height /g_TrackinkSystem->M_FaceExtractor.m_fScale;*/
	RightRoi = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect;

	bool result = (/*pFuter->IsModelPoint() &&*/
		(	(PointInRect(PointMul(pFuter->m_CurrentLocation , g_TrackinkSystem->m_PointTracker.m_fScale), LeftRoi))||
		(PointInRect(PointMul(pFuter->m_CurrentLocation ,  g_TrackinkSystem->m_PointTracker.m_fScale), RightRoi)))
		) ;

	return  result;

}

//===========================================================================


void Draw_Cross(IplImage *image, int centerx, int centery, int x_cross_length, int y_cross_length, CvScalar color)
{
	CvPoint pt1,pt2,pt3,pt4;

	pt1.x = centerx - x_cross_length;
	pt1.y = centery;
	pt2.x = centerx + x_cross_length;
	pt2.y = centery;

	pt3.x = centerx;
	pt3.y = centery - y_cross_length;
	pt4.x = centerx;
	pt4.y = centery + y_cross_length;

	cvLine(image,pt1,pt2,color,1,8);
	cvLine(image,pt3,pt4,color,1,8);
}
//===========================================================================
//CvPoint CGazeDetector::FindEyeCenterBoost(CImageRgion _EyeImg , CvPoint Center)
//{
//	//CvPoint corneal_reflection = {0,0};
//	//CvScalar White, Red, Green, Blue, Yellow;
//	//White = CV_RGB(255,255,255);
//	//Red = CV_RGB(255,0,0);
//	//Green = CV_RGB(0,255,0);
//	//Blue = CV_RGB(0,0,255);
//	//Yellow = CV_RGB(255,255,0);
//	//_EyeImg.Lock();
//	//IplImage* gray =  cvCreateImage( cvSize(_EyeROI.width,_EyeROI.height), 8, 1);
//	//IplImage* EyeImg =  cvCreateImage( cvSize(_EyeROI.width,_EyeROI.height), 8, 3);
//
//	//cvCopy( _EyeImg.getImg(), EyeImg);
//	//cvCvtColor( _EyeImg.getImg(), gray, CV_BGR2GRAY );
//	//IplImage* ellipse_image = EyeImg;
//	//int edge_threshold = 20;		//threshold of pupil edge points detection
//	//int rays =20;				//number of rays to use to detect feature points
//	//int min_feature_candidates = 10;	//minimum number of pupil feature candidates
//	//int cr_window_size = 100;		//corneal refelction search window size
//
//	//int *inliers_index;
//	//CvPoint pupil = {0,0}; 
//	//CvSize ellipse_axis;
//	//start_point.x = m_Center.x;
//	//start_point.y = m_Center.y;
//	//int corneal_reflection_r;	//			start_point.y 
//	////	cvNormalize(gray,gray,250,0,CV_MINMAX,0);
//	////	cvSmooth(gray, gray, CV_GAUSSIAN, 5, 5);
//	//IplImage*  threshold_image = cvCloneImage(gray);
//
//
//	//// remove_corneal_reflection(gray, threshold_image, (int)start_point.x, (int)start_point.y, cr_window_size, (int)gray->height/10, corneal_reflection.x, corneal_reflection.y, corneal_reflection_r);  
//	//printf("corneal reflection: (%d, %d)\n", corneal_reflection.x, corneal_reflection.y);
//	//Draw_Cross(ellipse_image, corneal_reflection.x, corneal_reflection.y, 15, 15, Yellow);  
//
//	////starburst pupil contour detection
//	//starburst_pupil_contour_detection((UINT8*)gray->imageData, gray->width, gray->height,
//	//	edge_threshold, rays, min_feature_candidates);
//
//
//
//	//inliers_num = 0;
//	//int o = 0;
//	//inliers_index = pupil_fitting_inliers((UINT8*)gray->imageData, gray->width, gray->height, inliers_num);
//	//int width = (int)pupil_param[0];
//	//int height = (int)pupil_param[1];
//
//	//ellipse_axis.width = (int)pupil_param[0];
//	//ellipse_axis.height = (int)pupil_param[1];
//
//	////		Draw_Cross(ellipse_image, pupil.x, pupil.y, 15, 15, Red);
//	////		cvLine(eye_image, pupil, corneal_reflection, Red, 4, 8);
//	////		cvLine(ellipse_image, pupil, corneal_reflection, Red, 4, 8);
//
//	//CvPoint result;
//	//result.x = (int)pupil_param[2];
//	//result.y = (int)pupil_param[3];
//
//	//m_Center = result;
//	//pupil = result;
//	//_EyeImg.Unlock();
//
//
//
//	//////////////
//
//
//	//bool is_inliers = 0;
//	//for (int i = 0; i < edge_point.size(); i++) {
//	//	is_inliers = 0;
//	//	for (int j = 0; j < inliers_num; j++) {
//	//		if (i == inliers_index[j])
//	//			is_inliers = 1;
//	//	}
//	//	stuDPoint *edge = edge_point.at(i);
//	//	if (!is_inliers)
//	//		Draw_Cross(ellipse_image, (int)edge->x,(int)edge->y, 5, 5, Red);
//	//	else
//	//		Draw_Cross(ellipse_image, (int)edge->x,(int)edge->y, 3, 3, Green);
//	//}
//	//free(inliers_index);
//
//	//if (ellipse_axis.width > 0 && ellipse_axis.height > 0) {
//	//	start_point.x = pupil.x;
//	//	start_point.y = pupil.y;
//	//	//printf("start_point: %d,%d\n", start_point.x, start_point.y);
//	//	Draw_Cross(EyeImg , pupil.x, pupil.y, 10, 10, Green);
//	//	cvEllipse(EyeImg , pupil, ellipse_axis, -pupil_param[4]*180/PI, 0, 360, Red, 2);
//	//	cvEllipse(ellipse_image, pupil, ellipse_axis, -pupil_param[4]*180/PI, 0, 360, Green, 2);
//
//	//};
//	//Draw_Cross(ellipse_image, (int)start_point.x, (int)start_point.y, 7, 7, Blue);
//	//Draw_Cross(EyeImg , (int)start_point.x, (int)start_point.y, 7, 7, Blue);
//
//	////////////////////
//
//
//	//g_GUI.ShowImage(EyeImg , "boost");
//
//CvPoint result;
//	return result;
//}
//===========================================================================


//CvPoint CGazeDetector::FindEyeCenterHung(CvRect EyeROI , CvPoint Center)
//{
//
//	//float r = 0;
//	//CvPoint result;
//	//CvMemStorage* storage = cvCreateMemStorage(0);
//	//IplImage* gray =  cvCreateImage( cvSize(EyeROI.width,EyeROI.height), 8, 1);
//	//cvCvtColor( EyeImg.getImg(), gray, CV_BGR2GRAY );
//	//IplImage* canny =cvCreateImage( cvGetSize(gray),8, 1 );
//
//	////	cvCopy(gray,copy,0);
//	//cvSmooth(gray, canny, CV_GAUSSIAN, 3, 0);
//	//cvCanny(canny, canny, g_GUI.param2,250, 3);
//	//cvEqualizeHist( canny, canny );
//	//IplImage* copy =cvCloneImage(gray);
//
//	//CvSeq* results1 = cvHoughCircles(
//	//	canny,
//	//	storage,
//	//	CV_HOUGH_GRADIENT,
//	//	1,
//	//	1,
//	//	1,
//	//	1,
//	//	canny->width/40,
//	//	canny->width/5
//	//	);
//
//	//if (m_PupilRadious == -1)
//	//	m_PupilRadious = canny->width/ 6;
//
//	//for( int i = 0; i < results1->total; i++ ) 
//	//{
//	//	float* p = (float*) cvGetSeqElem( results1, i );
//	//	CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
//
//	//	if ( abs(m_PupilRadious - r) > abs(m_PupilRadious - p[2]) )
//	//	{
//	//		result  = pt;
//	//		r = p[2];
//	//	};
//
//	//	if ( (pt.x- result.x)*(pt.x- result.x) + (pt.y- result.y)*(pt.y- result.y)  < 100 );
//	//}
//	///*		cvCircle(
//	//canny,
//	//result,
//	//cvRound( r ),
//	//CV_RGB(0xff,0xff,0xff));*/
//
//
//	////cout<<"r:"<<r<<"width"<<canny->width<<endl;
//	//if (r > 0 )
//	//	m_PupilRadious = ( 9*m_PupilRadious + r) /10;
//
//
//
//
//
//	////----------------snake---------------------------
//	//int PointCount = 10;
//
//
//	//CvPoint* PointArray = (CvPoint*)malloc( PointCount*sizeof(CvPoint) );
//	//float angle;
//	//for (int i = 1 ; i < PointCount ; i++)
//	//{
//	//	angle = i * 2*3.14 / PointCount;
//	//	PointArray[i].x = r * cos(angle) + result.x ;
//	//	PointArray[i].y = r * sin(angle) + result.y ;
//	//	//cvCircle(copy ,PointArray[i], 7, CV_RGB(200,222,222), 1, 8,0);		
//	//}
//
//	//float alpha = g_GUI.param1;
//	//float beta =g_GUI.param2;
//	//float gamma =g_GUI.param3;
//	////	cvSnakeImage( work2, snk, settings->params->snakeCalib->m_nPoints, &alpha, &beta, &gamma, CV_VALUE, win, criteria, 1);
//
//	//cvSnakeImage(canny,PointArray,PointCount, &alpha, &beta, &gamma, CV_VALUE, cvSize(11,11), 	cvTermCriteria(CV_TERMCRIT_EPS,200,1), 1);
//
//	//cvCircle(
//	//	copy,
//	//	result,
//	//	cvRound( r ),
//	//	CV_RGB(0xff,0xff,0xff));
//
//
//	//for (int i = 1 ; i < PointCount ; i++)
//	//{
//
//	//	cvCircle(copy,PointArray[i], 1, CV_RGB(222,222,222), 2, 8,0);		
//	//}
//	//g_GUI.ShowImage(canny, "hung");
//	////	g_GUI.ShowImage(copy, "snake");
//	//return result;
//
//}
//
//
//
//
//

//===========================================================================


//CvPoint CGazeDetector::FindEyeCenterBlob(CvRect EyeROI , CvPoint Center)
//{
//	CvPoint result;
//	//		CBlobResult blobs;
//	//	EyeImg.Lock();
//	//	//IplImage* Img =  cvCloneImage( EyeImg.getImg());
//	//
//	//	CBlob *currentBlob;
//	//	IplImage* smallgray =  cvCreateImage( cvSize(EyeROI.width,EyeROI.height), 8, 1);
//	//	IplImage* gray =  cvCreateImage( cvSize(800,600), 8, 1);
//	//	IplImage* Img =  cvCreateImage(cvSize(800,600), 8,3);
//	//
//	//	cvCvtColor( EyeImg.getImg(), smallgray, CV_BGR2GRAY );
//	//
//	//	cvResize(smallgray,gray);
//	//	cvResize(EyeImg.getImg(),Img);
//	//
//	//	
//	//	
//	//		cvThreshold( gray, gray,100, 250, CV_THRESH_BINARY );
//	//		g_GUI.ShowImage(gray,"ttt");
//	//
//	//	blobs = CBlobResult( gray, NULL,255);
//	//	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS, 100 );
//	//
//	//	// display filtered blobs
//	//
//	//	cvZero(Img);
//	//
//	//	for (int i = 0; i < blobs.GetNumBlobs(); i++ )
//	//	{
//	//		currentBlob = blobs.GetBlob(i);
//	//		float a  = currentBlob->Area();
//	//		currentBlob->FillBlob( Img, CV_RGB(0,0,233),0,0);
//	//
//	//			CvBox2D box = currentBlob->GetEllipse();
//	//	cvCircle( Img,cvPointFrom32f(currentBlob->GetEllipse().center),4, CV_RGB(222,250,222), -1, 8,0);
//	//	cvEllipse(Img,cvPointFrom32f( box.center), cvSize(box.size.width,box.size.height),
//	//		box.angle, 0, 360,
//	//		CV_RGB(0,0,255), 1, CV_AA, 0);
//	//	result = cvPointFrom32f(currentBlob->GetEllipse().center);
//	//
//	//	};
//	//
//	//
//	//
//	//g_GUI.ShowImage(Img,"Blob");
//	//	EyeImg.Unlock();
//	return result;
//
//}
//===========================================================================

void CGazeDetector::UpdateTreshold(CvRect LeftRoi,CvRect RightRoi )
{
	UpdateTresholdLeft(LeftRoi);
	UpdateTresholdRight(RightRoi);
}
//===========================================================================

void CGazeDetector::UpdateTresholdLeft(CvRect EyeROI )
{
	UpdateOptimalTresholdValue(EyeROI,m_LeftTresholdValue1 ,m_LeftTresholdValue2);
	cout<<"ltresh1:"<<m_LeftTresholdValue1<<endl;
	cout<<"ltresh2:"<<m_LeftTresholdValue2<<endl;
}
//===========================================================================

void CGazeDetector::UpdateTresholdRight(CvRect EyeROI )
{
	UpdateOptimalTresholdValue(EyeROI,m_RightTresholdValue1 ,m_RightTresholdValue2);
	cout<<"Rtresh1:"<<m_RightTresholdValue1<<endl;
	cout<<"Rtresh2:"<<m_RightTresholdValue2<<endl;
}


//===========================================================================

int CGazeDetector::UpdateOptimalTresholdValue(CvRect EyeROI , int &TreshVal1,int &TreshVal2 )
{
	float BlobArea;
	int TresholdValue1;
	int TresholdValue2;
	int BestTresholdValue2 = -10000;
	double BestFit = -10000;
	CvBox2D32f FirstGuaseCenter;
	CvBox2D32f SecondGuaseCenter;


	PreperImg(EyeROI);

	CvPoint2D32f EstimatedCenter;


	EstimatedCenter.x =EyeROI.width / 2;
	EstimatedCenter.y = EyeROI.height / 2;
	// -------------- teshold 1------------------------
	for ( TresholdValue1 = 3 ; TresholdValue1 < 40 ; TresholdValue1++)
	{
		cvThreshold( gray,  TreshImg, TresholdValue1 , 250, CV_THRESH_BINARY );
		FirstGuaseCenter = FitBlobElipse(TreshImg , EstimatedCenter,false,BlobArea);


		if (TreshImg->width * TreshImg->height * ( 1/50.0)  < BlobArea)
			break; 

		if (FirstGuaseCenter.size.width * FirstGuaseCenter.size.height > TreshImg->width * TreshImg->height / 200)
			break; 
	};

	// -------------- teshold 2------------------------
	double CurrentFit = -1000;;

	float MinBlobArea = ((float)TreshImg->width * (float)TreshImg->height )/60.0 ;
	float MaxBlobArea = ((float)TreshImg->width * (float)TreshImg->height ) /5.0 ;

	BestTresholdValue2 =TresholdValue1;

	for ( TresholdValue2 = TresholdValue1 ; TresholdValue2 < 80 ; TresholdValue2++)
	{
		cvThreshold( gray,  TreshImg, TresholdValue2 , 250, CV_THRESH_BINARY );
		SecondGuaseCenter = FitBlobElipse(TreshImg , FirstGuaseCenter.center,true,BlobArea);

		if ((BlobArea > MaxBlobArea)  /*||(BlobArea < MinBlobArea)  */)
			continue;

		if ((SecondGuaseCenter.size.width > (float)TreshImg->height / 4.0) || 
			(SecondGuaseCenter.size.height > (float)TreshImg->height / 4.0) )
		continue;

		float a = 1 + (float)g_GUI.param1 / 5;

		float b = 1 + (float)g_GUI.param2 / 5;
		a = 3;
		b =2;
		//float OptimalSize = TreshImg->width / ((float)g_GUI.param1 / 4.0 + 1.0);

		//CurrentFit = abs(OptimalSize - SecondGuaseCenter.size.width) +   abs(OptimalSize - SecondGuaseCenter.size.height);
		//			

		//CurrentFit = (
		//			  SecondGuaseCenter.size.width * SecondGuaseCenter.size.height
		//			 - 20*abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ) * abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height )
		//			/*- b*(SecondGuaseCenter.size.width * SecondGuaseCenter.size.height -  BlobArea)
		//			-((SecondGuaseCenter.center.x - FirstGuaseCenter.center.x)*(SecondGuaseCenter.center.x - FirstGuaseCenter.center.x) + 
		//			(SecondGuaseCenter.center.y - FirstGuaseCenter.center.y)*(SecondGuaseCenter.center.y - FirstGuaseCenter.center.y))
		//		*/	 );




		CurrentFit = 1 / CurrentFit;

		CurrentFit = 1 /(
			abs(0.2*(TreshImg->width * TreshImg->height ) - (SecondGuaseCenter.size.width * SecondGuaseCenter.size.height) ) + 
			+ 22*abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ) * abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height )
			);


		//float Tmp = SecondGuaseCenter.size.width * SecondGuaseCenter.size.height;



		//if (Tmp > MaxBlobArea || Tmp < MinBlobArea )
		//	CurrentFit = -100000;

		if ( SecondGuaseCenter.size.width  > (float)TreshImg->width / 4.0 )
			CurrentFit = -100000;


		//		CurrentFit = (SecondGuaseCenter.size.width * SecondGuaseCenter.size.height)  
		//			//-20* abs(( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height )*( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ));
		//
		//		*(((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2) - abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ))
		///((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2)
		//		*(((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2) - abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ))
		///((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2)
		//				*(((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2) - abs( SecondGuaseCenter.size.width - SecondGuaseCenter.size.height ))
		///((SecondGuaseCenter.size.width + SecondGuaseCenter.size.height) / 2);
		////		* SecondGuaseCenter.size.width > SecondGuaseCenter.size.height ? SecondGuaseCenter.size.height/  SecondGuaseCenter.size.width : SecondGuaseCenter.size.height /  SecondGuaseCenter.size.width
		////* SecondGuaseCenter.size.width > SecondGuaseCenter.size.height ? SecondGuaseCenter.size.height/  SecondGuaseCenter.size.width : SecondGuaseCenter.size.height /  SecondGuaseCenter.size.width;

		if (CurrentFit > BestFit) 
		{
			BestFit = CurrentFit;
			BestTresholdValue2 =TresholdValue2 ;//* 0.9;
		};
	};
	BestTresholdValue2 = TresholdValue1 +  ( BestTresholdValue2 -TresholdValue1 ) / 1.5;

	//if (TreshVal1 < 0 )
	//{
	//	TreshVal1 = TresholdValue1;
	//	TreshVal2  =BestTresholdValue2 ;
	//}
	//else
	{
		TreshVal1 =(float) TreshVal1 *	(float)m_UpdateTresholdValueCount / (float)(m_UpdateTresholdValueCount + 1)+ (float)TresholdValue1 *1/ (float)(m_UpdateTresholdValueCount + 1);
		TreshVal2 = (float)TreshVal2 *	(float)m_UpdateTresholdValueCount / (float)(m_UpdateTresholdValueCount + 1)+ (float)BestTresholdValue2 *1/ (float)(m_UpdateTresholdValueCount + 1);

		//TreshVal2 = TreshVal2 * 0.9 + BestTresholdValue2 * 0.1;
	};


	cout<<"tresh1:"<<TresholdValue1<<endl;
	cout<<"tresh2:"<<BestTresholdValue2<<endl;


	return BestTresholdValue2;
}
//===========================================================================
CvBox2D32f CGazeDetector::GetLeftPupilElipse(CvRect EyeROI)
{
	if (m_RightTresholdValue1 < 0 ) 
		throw("negativ left eye treshold");
		//UpdateTresholdLeft( EyeROI );



	m_LeftPupilElipse = FindEyeElipseDoubleTreshold( EyeROI,m_LeftTresholdValue1 ,m_LeftTresholdValue2);;

	cvEllipse(ImgCopy, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);
	cvEllipse(ImgCopy, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(0,0,255), 1, CV_AA, 0);
	cvCircle( ImgCopy,cvPointFrom32f(SecondGuaseCenter.center), 2, CV_RGB(222,1,1), -1, 8,0);


	CvPoint2D32f point;
	point.x =( g_TrackinkSystem->m_HeadPosition.EstimateLeftEyeCenter2D.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_HeadPosition.EstimateLeftEyeCenter2D.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,222,1), -1, 8,0);





	point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(222,222,1), -1, 8,0);

	point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);

	point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);


	CvPoint2D32f LeftCenter;
	LeftCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2.0;
	LeftCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2.0;


	LeftCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2.0;
	LeftCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2.0;




	LeftCenter.x = (LeftCenter.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	LeftCenter.y = (LeftCenter.y- EyeROI.y) *  gray->height  /EyeROI.height ;

	cvCircle( ImgCopy,cvPointFrom32f(LeftCenter), 4, CV_RGB(1,0,222), 1, 8,0);



	g_GUI.ShowImage(ImgCopy, "Left Eye");



	//przesuwanie todo poprawic zeby nie bylo trzeba
	m_LeftPupilElipse.center.x += g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x;
	m_LeftPupilElipse.center.y += g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y;



	if ( (abs(m_LeftPupilElipse.center.x - (EyeROI.x  + EyeROI.width* 0.5) ) > gray->width/4.0 ) || 
		(abs(m_LeftPupilElipse.center.y -(EyeROI.y  + EyeROI.height* 0.5) ) > gray->height/3.0 ))
		m_LeftCenterDetected = false;
	else
		m_LeftCenterDetected = true;


	CvBox2D32f m_RightPupilElipse;
	return m_LeftPupilElipse;
}

//===========================================================================
CvBox2D32f CGazeDetector::GetRightPupilElipse(CvRect EyeROI)
{
	if (m_RightTresholdValue1 < 0 ) 
		UpdateTresholdRight( EyeROI );

	m_RightPupilElipse = FindEyeElipseDoubleTreshold( EyeROI,m_RightTresholdValue1 ,m_RightTresholdValue2);

	cvEllipse(ImgCopy, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);
	cvEllipse(ImgCopy, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(0,0,255), 1, CV_AA, 0);
	cvCircle( ImgCopy,cvPointFrom32f(SecondGuaseCenter.center), 2, CV_RGB(222,1,1), -1, 8,0);



	CvPoint2D32f point;
	point.x =( g_TrackinkSystem->m_HeadPosition.EstimateRightEyeCenter2D.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_HeadPosition.EstimateRightEyeCenter2D.y- EyeROI.y) *  gray->height  /EyeROI.height ;	
	cvCircle( ImgCopy,cvPointFrom32f(point), 3, CV_RGB(1,222,1), 3, 8,0);

	cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,222,1), -1, 8,0);
	point.x = gray->width /2 ;
	point.y = gray->height /2 ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,0,222), -1, 8,0);




	point.x =( g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(222,222,1), -1, 8,0);




	point.x =( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);

	point.x =( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);


	CvPoint2D32f RightCenter;
	RightCenter.x = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	RightCenter.y = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;
	RightCenter.x = (RightCenter.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	RightCenter.x = (RightCenter.y- EyeROI.y) *  gray->height  /EyeROI.height ;


	cvCircle( ImgCopy,cvPointFrom32f(RightCenter), 4, CV_RGB(1,0,222), 1, 8,0);


	g_GUI.ShowImage(ImgCopy, "Right Eye");



	m_RightPupilElipse.center.x += g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x;
	m_RightPupilElipse.center.y += g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y;

	if ( (abs(m_RightPupilElipse.center.x - (EyeROI.x  + EyeROI.width* 0.5) ) > gray->width/4.0 ) || 
		(abs(m_RightPupilElipse.center.y -(EyeROI.y  + EyeROI.height* 0.5) ) > gray->height/3.0 ))
		m_RightCenterDetected = false;
	else
		m_RightCenterDetected = true;
	return m_RightPupilElipse;
}



//===========================================================================
bool CGazeDetector::UpdatePupilsCenters()
{

	m_PrevLeftPupilCenter = m_LeftPupilCenter ;
	m_PrevRightPupilCenter = m_RightPupilCenter ;


	//if(	g_TrackinkSystem->m_bKeyInitModel3D)
	//	if(	g_TrackinkSystem->m_CurentFaze == InitFaze)
	//{
	//	if (m_UpdateTresholdValueCount > 0.5 *UpdateTresholdValueInitCount)
	//		//m_UpdateTresholdValueCount = UpdateTresholdValueInitCount * 0.2;
	//		m_UpdateTresholdValueCount = 0;
	//};

	m_UpdateTresholdValueCount = 0;

	if (m_UpdateTresholdValueCount < UpdateTresholdValueInitCount )
	{
		UpdateTresholdLeft(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
		UpdateTresholdRight(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect );
		m_UpdateTresholdValueCount++;
	};

	GetLeftPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
	GetRightPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect);



	//g_TrackinkSystem->M_FaceExtractor.m_pLeftEyeImg->GetParentPointFloat(m_LeftPupilElipse.center);

	//	m_RightPupilElipse.center = g_TrackinkSystem->M_FaceExtractor.m_pRightEyeImg->GetParentPointFloat(m_RightPupilElipse.center);
	//todo uwzglednic scale

	m_LeftPupilCenter.x = m_LeftPupilElipse.center.x;
	m_LeftPupilCenter.y = m_LeftPupilElipse.center.y;
	m_RightPupilCenter.x = m_RightPupilElipse.center.x;
	m_RightPupilCenter.y = m_RightPupilElipse.center.y;


	//if(	g_TrackinkSystem->m_bKeyInitModel3D)
	//		if(	g_TrackinkSystem->m_CurentFaze == InitFaze)
	//{

	//	
	//};

InitEyesCorners();



//-----------corect face rect-------------------------------------------------

//CvRect FaceRect;
//
//
//float s = 1.1;
//float PupilCenterDistans = PointDistans(m_LeftPupilCenter,m_RightPupilCenter);
//
//FaceRect.x =g_TrackinkSystem->M_FaceExtractor.getFaceCenter().x - PupilCenterDistans* 0.8 * s;
//FaceRect.y = g_TrackinkSystem->M_FaceExtractor.getFaceCenter().y - PupilCenterDistans* 1.2 * s;
//FaceRect.width =2*PupilCenterDistans* 0.8 * s;
//FaceRect.height = 2*PupilCenterDistans* 1.2 * s;
//
//g_TrackinkSystem->M_FaceExtractor.m_FaceRect = FaceRect;

//-----------end corect face rect-------------------------------------------------
			return (m_LeftCenterDetected && m_RightCenterDetected );

	//m_LeftPupilCenterOF = GetPupilCenterOpticalFlow(m_LeftPupilElipse,m_PrevLeftPupilCenter);;
	//m_RightPupilCenterOF  = GetPupilCenterOpticalFlow(m_RightPupilElipse,m_PrevRightPupilCenter);
}


//===========================================================================

void CGazeDetector::InitEyesCorners()
{
	int count;
	CvPoint2D32f*	NewGoodPoints= new CvPoint2D32f[10] ;
	CvRect Roi = g_TrackinkSystem->M_FaceExtractor.m_FaceRect;
	//IplImage* GrayImg =  g_TrackinkSystem->m_Graber.m_pGrayFrame;
	//IplImage* EigImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	//IplImage* TmpImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	//IplImage* MaskImg = cvCreateImage( cvSize(Roi.width,Roi.height), 8, 1 );
	CvPoint2D32f Center;
	CvPoint2D32f PupilCenter;
	CvPoint2D32f P1,P2;
	float size ;
	double Treshold ;
	float quality;
	//
	//cvSetImageROI(GrayImg, Roi );

	int FuterCount = 50;

	CvPoint2D32f LeftR, LeftL;
	LeftL.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1; 
	LeftL.y = m_LeftPupilCenter.y +  g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1; 
	LeftR.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width* 0.95;   ; 
	LeftR.y = m_LeftPupilCenter.y +  g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1;  ; 

	CvPoint2D32f RightR, RightL;
	RightL.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.95; 
	RightL.y = m_RightPupilCenter.y +  g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1; 
	RightR.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1; ; 
	RightR.y = m_RightPupilCenter.y +  g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1; 


	////-------------------left eye center----------------------------------------------
	PupilCenter.x =m_LeftPupilCenter.x -Roi.x;
	PupilCenter.y =m_LeftPupilCenter.y -Roi.y;
	Treshold = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height / 35.0;
	g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation = FindCorner(PupilCenter, Treshold );
	g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation =m_LeftPupilCenter;
	g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_KeyFrameLocation = g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation;
	//-------------------right eye center----------------------------------------------
	PupilCenter.x =m_RightPupilCenter.x -Roi.x;
	PupilCenter.y =m_RightPupilCenter.y -Roi.y;
	Treshold = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height / 35.0;
	g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation = FindCorner(PupilCenter, Treshold );
	g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation = m_RightPupilCenter;
	g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_KeyFrameLocation = g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation;

	/////////////////////////////corners////////////////////////////////////////////////////
	//-------------------left eye CornerR----------------------------------------------
	PupilCenter.x =m_LeftPupilCenter.x -Roi.x;
	PupilCenter.y =m_LeftPupilCenter.y -Roi.y;
	Center.x =LeftR.x -Roi.x;
	Center.y =LeftR.y -Roi.y;
	float DistansToPupilCenter = sqrt((PupilCenter.x - Center.x)* (PupilCenter.x - Center.x) + (PupilCenter.y - Center.y)* (PupilCenter.y- Center.y));
	LeftR = FindCorner(Center, DistansToPupilCenter );
	//-------------------left eye CornerL----------------------------------------------
	PupilCenter.x =m_LeftPupilCenter.x -Roi.x;
	PupilCenter.y =m_LeftPupilCenter.y -Roi.y;
	Center.x =LeftL.x -Roi.x;
	Center.y =LeftL.y -Roi.y;
	DistansToPupilCenter = sqrt((PupilCenter.x - Center.x)* (PupilCenter.x - Center.x) + (PupilCenter.y - Center.y)* (PupilCenter.y- Center.y));
	LeftL = FindCorner(Center, DistansToPupilCenter );
	//-------------------right eye CornerL----------------------------------------------
	PupilCenter.x =m_RightPupilCenter.x -Roi.x;
	PupilCenter.y =m_RightPupilCenter.y -Roi.y;
	Center.x =RightL.x -Roi.x;
	Center.y =RightL.y -Roi.y;
	DistansToPupilCenter = sqrt((PupilCenter.x - Center.x)* (PupilCenter.x - Center.x) + 
		(PupilCenter.y - Center.y)* (PupilCenter.y- Center.y));
	RightL = FindCorner(Center, DistansToPupilCenter );

	//-------------------right eye CornerR----------------------------------------------

	PupilCenter.x =m_RightPupilCenter.x -Roi.x;
	PupilCenter.y =m_RightPupilCenter.y -Roi.y;
	Center.x =RightR.x -Roi.x;
	Center.y =RightR.y -Roi.y;
	DistansToPupilCenter = sqrt((PupilCenter.x - Center.x)* (PupilCenter.x - Center.x) + 
		(PupilCenter.y - Center.y)* (PupilCenter.y- Center.y));
	RightR = FindCorner(Center, DistansToPupilCenter );
	//----------------------------------------------------------------------------



	g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation =  LeftL;
	g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation = LeftR;
	g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_KeyFrameLocation =  LeftL;
	g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_KeyFrameLocation = LeftR;

	g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation =  RightL;
	g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation = RightR;
	g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_KeyFrameLocation =  RightL;
	g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_KeyFrameLocation = RightR;



}
//===========================================================================

CvPoint2D32f  CGazeDetector::FindCorner(CvPoint2D32f InitGuas, double TresholdDistans )
{
	CvRect Roi = g_TrackinkSystem->M_FaceExtractor.m_FaceRect;
	IplImage* GrayImg =  g_TrackinkSystem->m_Graber.m_pGrayFrame;
	IplImage* EigImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	IplImage* TmpImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	IplImage* MaskImg = cvCreateImage( cvSize(Roi.width,Roi.height), 8, 1 );


	CvPoint2D32f*	NewGoodPoints= new CvPoint2D32f[10] ;
	int blockSize = 5;
	double MaxVal = 10;
	cv::Mat eig, tmp, mask, tmp2;
	cvSetImageROI(g_TrackinkSystem->m_Graber.m_pGrayFrame, Roi );
	cv::Mat image = cv::cvarrToMat(g_TrackinkSystem->m_Graber.m_pGrayFrame);


	cvCircle( MaskImg,cvPointFrom32f(InitGuas), TresholdDistans, CV_RGB(1,1,1), -1, 8,0);



	cvCornerMinEigenVal(GrayImg,EigImg,blockSize,3);


	cvNormalize(EigImg,EigImg,250,0,CV_MINMAX,MaskImg);
	//g_GUI.ShowImage(EigImg,"eig img");


	MaxVal = 250;


	//float a = MaxVal/( (TresholdDistans*TresholdDistans*TresholdDistans*TresholdDistans));
	//
	//	// cv::minMaxLoc(tmp, 0, &MaxVal, 0, 0 ,mask);
	for( int y=0; y<EigImg->height; y++ ) {
		float* ptr = (float*) (EigImg->imageData + y * EigImg->widthStep);
		float* ptr2 = (float*) (TmpImg->imageData + y * TmpImg->widthStep);
		for( int x=0; x<EigImg->width; x++ )
		{
			double distans = sqrt(4*(InitGuas.x - x)*(InitGuas.x - x) + (InitGuas.y - y)*(InitGuas.y - y));
			distans = distans/(TresholdDistans);

			//if (x > 100)
			ptr[x] = ptr[x] - MaxVal* distans* distans*distans* distans /* +  MaxVal/2 */ ;
			ptr2[x] =	ptr[x]  ;
		}
	}
	//g_GUI.ShowImage(TmpImg,"TmpImg TmpImg TmpImg");


	double max;
	CvPoint maxLoc;
	cvMinMaxLoc(EigImg,0,&max,0,&maxLoc,0);


	cvNormalize(EigImg,EigImg,250,0,CV_MINMAX,MaskImg);
	//g_GUI.ShowImage(EigImg,"after eig img");


	NewGoodPoints[0].x = maxLoc.x;
	NewGoodPoints[0].y = maxLoc.y;

	cvFindCornerSubPix( g_TrackinkSystem->m_Graber.m_pGrayFrame, NewGoodPoints, 1,
		cvSize(5,5), cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,100,0.001));
	CvPoint2D32f result;
	result.x = NewGoodPoints[0].x + Roi.x;;
	result.y = NewGoodPoints[0].y + Roi.y;;
	return result;



	//-------------------right eye CornerL----------------------------------------------

}
//===========================================================================


CvPoint2D32f  CGazeDetector::GetPupilCenterOpticalFlow(CvBox2D32f PupilElipse,CvPoint2D32f PrevPupilCenter )
{

	CvPoint2D32f result;
	CvPoint2D32f CenterMove;
	CenterMove.x = 0;
	CenterMove.y = 0;

	int PupilFuterCount = 0;

	list<shared_ptr<CTrackingFuture>>::iterator it;
	for( it = g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it != g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint()) 
		{
			float distans = sqrt((PupilElipse.center.x - (*it)->m_CurrentLocation.x)*(PupilElipse.center.x - (*it)->m_CurrentLocation.x) + 
				(PupilElipse.center.y - (*it)->m_CurrentLocation.y)*(PupilElipse.center.y - (*it)->m_CurrentLocation.y) );

			if (distans < (PupilElipse.size.width < PupilElipse.size.height ? PupilElipse.size.width / 2.2: PupilElipse.size.height /2.2 )
				) // in pupil elipse

			{ 
				/*	result.x =   m_PrevLocation.x - m_CurrentLocation.x;
				result.y = m_PrevLocation.y - m_CurrentLocation.y;*/

				CenterMove.x += (*it)->m_CurrentLocation.x - (*it)->m_PrevLocation.x ;
				CenterMove.y += (*it)->m_CurrentLocation.y - (*it)->m_PrevLocation.y ;
				PupilFuterCount++;
				break;
			}
		}


	};//for

	if (PupilFuterCount > 0)
	{
		CenterMove.x /=PupilFuterCount;
		CenterMove.y /=PupilFuterCount;
		result.x = PrevPupilCenter.x + CenterMove.x;
		result.y = PrevPupilCenter.y + CenterMove.y;

		//czy duza rozbieznosc z wyliczonym srodkiem
		float diff = sqrt(result.x - PupilElipse.center.x)*(result.x - PupilElipse.center.x) + 
			(result.y - PupilElipse.center.y)*(result.y - PupilElipse.center.y) ;
		if ( diff > (PupilElipse.size.width > PupilElipse.size.height ? PupilElipse.size.width: PupilElipse.size.height) 
			)
		{
			result.x = PupilElipse.center.x ;
			result.y = PupilElipse.center.y ;
		};

	}
	else
	{
		result.x = PupilElipse.center.x ;
		result.y = PupilElipse.center.y;
	}


	return result;
}

//===========================================================================
void  CGazeDetector::PreperImg(CvRect EyeROI )
{	

	cvSetImageROI(g_TrackinkSystem->m_Graber.m_pFrame, EyeROI );
	cvResize(g_TrackinkSystem->m_Graber.m_pFrame, ImgCopy, /*CV_INTER_NN*/CV_INTER_LINEAR );
	//cvSmooth(ImgCopy, ImgCopy, CV_GAUSSIAN, 3);
	int x = g_GUI.param1 + 1;
	//IplConvKernel* v =  cvCreateStructuringElementEx(x,x,0,0,CV_SHAPE_ELLIPSE,NULL);
	//cvErode(ImgCopy,ImgCopy,v,g_GUI.param3);
	//cvDilate(ImgCopy,ImgCopy,v,g_GUI.param3);
	cvCvtColor( ImgCopy, gray, CV_BGR2GRAY );
	cvNormalize(gray,gray,250,0,CV_MINMAX,0);
	//cvEqualizeHist( gray, gray );
	cvResetImageROI(g_TrackinkSystem->m_Graber.m_pFrame);
}

//===========================================================================
CvBox2D32f CGazeDetector::FindEyeElipseDoubleTreshold(CvRect EyeROI ,int TreshVal1,int TreshVal2)
{

	float BlobAre;
	//CvBox2D32f FirstGuaseCenter;
	//CvBox2D32f SecondGuaseCenter;

	PreperImg(EyeROI);

	//cvEqualizeHist( gray, gray );
	cvThreshold( gray,  TreshImg, TreshVal1 , 250, CV_THRESH_BINARY );
	//g_GUI.ShowImage(TreshImg, "tresh1");


	CvPoint2D32f EstimatedCenter;
	EstimatedCenter.x = EyeROI.width / 2;
	EstimatedCenter.y = EyeROI.height / 2;
	FirstGuaseCenter = FitBlobElipse(TreshImg , EstimatedCenter,false,BlobAre);

	if (FirstGuaseCenter.size.width == 0)
		FirstGuaseCenter.center = EstimatedCenter;

	cvThreshold( gray,  TreshImg,TreshVal2, 250, CV_THRESH_BINARY );
//	g_GUI.ShowImage(TreshImg, "Treshelipse");

	SecondGuaseCenter = FitBlobElipse(TreshImg , FirstGuaseCenter.center, true,BlobAre);

	const float MinBlobArea = TreshImg->width * TreshImg->height * ( 1/30) ;

	//if (SecondGuaseCenter.size.width * SecondGuaseCenter.size.height <  MinBlobArea)
	//	SecondGuaseCenter = FirstGuaseCenter;
	m_Center1 =SecondGuaseCenter.center;


	CvBox2D32f result ;
	result.center.x = SecondGuaseCenter.center.x * EyeROI.width / gray->width;
	result.center.y = SecondGuaseCenter.center.y * EyeROI.height / gray->height;
	result.size.width = SecondGuaseCenter.size.width * EyeROI.width / gray->width;
	result.size.height = SecondGuaseCenter.size.height * EyeROI.height / gray->height;
	return result;;
}

//==========================================================================================================

//===========================================================================

//CvPoint2D32f CGazeDetector::FindEyeCenterFitElipse(CvRect EyeROI , CvPoint2D32f LastCenter)
//{
//	//CvPoint2D32f result = LastCenter;
//	//float BestScore = 0;
//	//CvBox2D MomentElipse;
//	//CvBox2D32f LastSquereElipse;
//	//CvBox2D32f CurrentSegmentElipse;
//	////cvFitEllipse(PointArray2D32f, count, &LastSquereElipse);
//
//	//CvMemStorage* stor;
//	//CvSeq* ConturSeq;
//	//CvBox2D32f* box;
//	////CvBox2D32f* Resultbox;
//	//float BoxEdge;
//	//CvPoint* PointArray;
//	//CvPoint2D32f* PointArray2D32f;
//	//CvPoint Dimension = cvPoint(EyeROI.width / 3 , EyeROI.height / 3);
//
//
//	//CvPoint ImgCenter = cvPoint(EyeROI.width / 2 , EyeROI.height / 2);
//	//float ImgArea = EyeROI.width * EyeROI.height;
//	//float BestBlobArea = 0;
//	//float  BestBlobDistansToImgCenter = EyeROI.width * EyeROI.width  +  EyeROI.height *  EyeROI.height;
//
//	//float CurrentScore;
//	//EyeImg.Lock();
//
//	//IplImage* gray =  cvCreateImage( cvSize(EyeROI.width,EyeROI.height), 8, 1);
//
//	////Center.x = EyeROI.width / 2;
//	////Center.y = EyeROI.height / 2;
//	//cvCvtColor( EyeImg.getImg(), gray, CV_BGR2GRAY );
//
//	//IplImage* copy =cvCloneImage(EyeImg.getImg());
//	//cvNormalize(gray,gray,250,0,CV_MINMAX,0);
//	////cvEqualizeHist( copy, copy );
//
//	////cvThreshold( gray, copy,20, 250, CV_THRESH_BINARY );
//	//cvThreshold( gray,  gray,20 +g_GUI.param1, 250, CV_THRESH_BINARY );
//	////cvThreshold( gray, 50, 250, CV_THRESH_BINARY );
//	////cvEqualizeHist( copy, copy );
//
//	//int param2 =1;
//
//
//	////cvErode(gray,gray,v,param2);
//	////cvDilate(gray,gray,v,param2);
//
//	//g_GUI.ShowImage(gray, "grneSmooth2");
//
//
//	////-----------------------------------
//
//	//// Create dynamic structure and sequence.
//
//	//ConturSeq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , m_MemStorage);
//
//
//	//// Find all contours.
//	//cvFindContours( gray, m_MemStorage, &ConturSeq, sizeof(CvContour), CV_RETR_LIST , 1, cvPoint(0,0));
//
//	//// Clear images. IPL use.
//
//	//int count = ConturSeq->total;
//	//for(;ConturSeq;)
//	//{   
//	//	int i; 
//	//	count = ConturSeq->total;
//	//	if( count < 10 )
//	//	{
//	//		ConturSeq = ConturSeq->h_next;
//	//		continue;
//	//	}
//	//	PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
//	//	PointArray2D32f= (CvPoint2D32f*)malloc( count*sizeof(CvPoint2D32f) );
//	//	cvCvtSeqToArray(ConturSeq, PointArray, CV_WHOLE_SEQ);
//
//
//	//	for(i=0; i<count; i++)
//	//	{
//	//		PointArray2D32f[i].x = (float)PointArray[i].x;
//	//		PointArray2D32f[i].y = (float)PointArray[i].y;
//	//	}
//
//
//	//	// check how good is new center
//	//	cvFitEllipse(PointArray2D32f, count, &CurrentSegmentElipse);
//	//	float CurrentBlobArea =  cvContourArea(ConturSeq);
//	//	//CurrentBlobArea = 0;
//	//	float CurrentBlobDistansToImgCenter = (ImgCenter.x - CurrentSegmentElipse.center.x)* (ImgCenter.x - CurrentSegmentElipse.center.x) + 
//	//		(ImgCenter.y - CurrentSegmentElipse.center.y)* (ImgCenter.y - CurrentSegmentElipse.center.y);
//
//
//	//	CurrentScore = BestBlobArea / CurrentBlobArea  + BestBlobDistansToImgCenter> 0 ?CurrentBlobDistansToImgCenter/BestBlobDistansToImgCenter: 0; 
//
//	//	CurrentScore = CurrentBlobArea;
//	//	//cvDrawContours(gray,cont,CV_RGB(255,255,255),CV_RGB(255,255,255),0,1,8,cvPoint(0,0));
//
//	//	if ( CurrentScore > BestScore)
//	//	{
//	//		BestScore = CurrentScore;
//	//		BestBlobArea = CurrentBlobArea;
//	//		BestBlobDistansToImgCenter = CurrentBlobDistansToImgCenter;
//	//		cvFitEllipse(PointArray2D32f, count, &LastSquereElipse);
//	//		MomentElipse = FitElipse(ConturSeq);
//	//		m_Center1 =LastSquereElipse.center;
//	//		m_Center2 = MomentElipse.center;
//	//		result = m_Center1;
//	//	};
//
//	//	ConturSeq = ConturSeq->h_next;
//	//};//for
//
//
//	//free(PointArray);
//	//free(PointArray2D32f);
//
//	////blob->m10/blob->area, blob->m01/blob->area
//
//
//	//EyeImg.Unlock();
//
//	//return result;
//}
//

//===========================================================================


//===========================================================================
CvBox2D32f CGazeDetector::FitBlobElipse(IplImage* gray , CvPoint2D32f EstimatedCenter, bool UseEstimatedCenter, float &BlobArea)
{
	CvBox2D32f result ;
	CvBox2D32f TmpResult ;

	result.size.width = 0;
	result.size.height = 0;
	result.center.x =EstimatedCenter.x;
	result.center.y = EstimatedCenter.y;
	float MaxBlobArea = gray->width * gray->height / 5.0;

	float BestScore = -1;
	float CurrentScore = -1;
	//m_pConturSeq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , m_MemStorage);
	//m_pConturSeqBegin = m_pConturSeq;

	cvFindContours( gray, m_MemStorage, &m_pConturSeq, sizeof(CvContour), CV_RETR_LIST , 1, cvPoint(0,0));
	int count;
	for(;m_pConturSeq;)
	{   
		count = m_pConturSeq->total;
		if( count < 10 )
		{
			m_pConturSeq = m_pConturSeq->h_next;
			continue;
		};
		float CurrentBlobArea =  cvContourArea(m_pConturSeq);
		CurrentScore = CurrentBlobArea;



		if (UseEstimatedCenter)
		{
			CvRect rect = cvContourBoundingRect(m_pConturSeq);

			/*		CvBox2D32f tmp = FitElipse(m_pConturSeq);
			CurrentScore = 1000000 -  (tmp.center.x - EstimatedCenter.x)*(tmp.center.x - EstimatedCenter.x) +
			(tmp.center.y - EstimatedCenter.y)*(tmp.center.y - EstimatedCenter.y);*/

			if (!((EstimatedCenter.x > rect.x )&&
				(EstimatedCenter.x < rect.x  +rect.width  )&&
				(EstimatedCenter.y > rect.y )&&
				(EstimatedCenter.y < rect.y + rect.height  )))
				CurrentScore = -1; // curent contour dont containt estimated center  

			if ( (abs(EstimatedCenter.x - (rect.x  + rect.width* 0.5) ) > gray->width/4.0 ) || 
				(abs(EstimatedCenter.y -(rect.y  + rect.height* 0.5) ) > gray->height/3.0 ))
				CurrentScore = -1; 
		};






		if ( (CurrentScore > BestScore) )
		{

			TmpResult = FitElipse(m_pConturSeq);
			//if (TmpResult.size.width *TmpResult.size.height < MaxBlobArea )
			{
				BestScore = CurrentScore;
				result = TmpResult;
				BlobArea = CurrentBlobArea;
			}
		};
		m_pConturSeq = m_pConturSeq->h_next;
	};//for
	cvClearSeq(m_pConturSeqBegin);
	return result;
}


//===========================================================================
CvPoint2D32f  CGazeDetector::GetBlobCenter(CvSeq* Contour)
{

	CvPoint * CurrentPoint;
	CvPoint2D32f result;
	return result;
	//for(;m_pConturSeq;)
	//	{   
	//
	//		ptr
	//		count = m_pConturSeq->total;
	//		if( count < 10 )
	//		{
	//			m_pConturSeq = m_pConturSeq->h_next;
	//			continue;
	//		};
	//		float CurrentBlobArea =  cvContourArea(m_pConturSeq);
	//		CurrentScore = CurrentBlobArea;
	//
	//		if (UseEstimatedCenter)
	//		{
	//			CvRect rect = cvContourBoundingRect(m_pConturSeq);
	//
	//			if (((EstimatedCenter.x > rect.x )&&
	//				(EstimatedCenter.x < rect.x  +rect.width  )&&
	//				(EstimatedCenter.y > rect.y )&&
	//				(EstimatedCenter.y > rect.y + rect.height  )))
	//					CurrentScore = -1; // curent contour dont containt estimated center  
	//		};
	//
	//		if ( CurrentScore > BestScore)
	//		{
	//			BestScore = CurrentScore;
	//			result = FitElipse(m_pConturSeq);
	//		};
	//		m_pConturSeq = m_pConturSeq->h_next;
	//	};//for
	//
	//	CvMoments Moments;
	//	cvContourArea(Contour)
	//cvContourMoments(Contour,&Moments);
	//u00 = cvGetSpatialMoment( &Moments,0,0);
	//blob->m10/blob->area, blob->m01/blob->area
}
//===========================================================================

CvBox2D32f  CGazeDetector::FitElipse(CvSeq* Contour)
{

	CvBox2D m_ellipse;
	CvMoments Moments;

	cvContourMoments(Contour,&Moments);

	double u00,u11,u01,u10,u20,u02, delta, num, den, temp;

	// central moments calculation
	u00 = cvGetSpatialMoment( &Moments,0,0);

	// empty blob?
	if ( u00 <= 0 )
	{
		m_ellipse.size.width = 0;
		m_ellipse.size.height = 0;
		m_ellipse.center.x = 0;
		m_ellipse.center.y = 0;
		m_ellipse.angle = 0;
		return m_ellipse;
	}
	u10 = cvGetSpatialMoment( &Moments,1,0) / u00;
	u01 = cvGetSpatialMoment( &Moments,0,1) / u00;

	u11 = -(cvGetSpatialMoment( &Moments,1,1) - cvGetSpatialMoment( &Moments,1,0) * cvGetSpatialMoment( &Moments,0,1) / u00 ) / u00;
	u20 = (cvGetSpatialMoment( &Moments,2,0) - cvGetSpatialMoment( &Moments,1,0) * cvGetSpatialMoment( &Moments,1,0) / u00 ) / u00;
	u02 = (cvGetSpatialMoment( &Moments,0,2) - cvGetSpatialMoment( &Moments,0,1) * cvGetSpatialMoment( &Moments,0,1) / u00 ) / u00;


	// elipse calculation
	delta = sqrt( 4*u11*u11 + (u20-u02)*(u20-u02) );
	m_ellipse.center.x = u10;
	m_ellipse.center.y = u01;

	temp = u20 + u02 + delta;
	if( temp > 0 )
	{
		m_ellipse.size.width = sqrt( 2*(u20 + u02 + delta ));
	}	
	else
	{
		m_ellipse.size.width = 0;
		return m_ellipse;
	}

	temp = u20 + u02 - delta;
	if( temp > 0 )
	{
		m_ellipse.size.height = sqrt( 2*(u20 + u02 - delta ) );
	}
	else
	{
		m_ellipse.size.height = 0;
		return m_ellipse;
	}

	// elipse orientation
	if (u20 > u02)
	{
		num = u02 - u20 + sqrt((u02 - u20)*(u02 - u20) + 4*u11*u11);
		den = 2*u11;
	}
	else
	{
		num = 2*u11;
		den = u20 - u02 + sqrt((u20 - u02)*(u20 - u02) + 4*u11*u11);
	}
	if( num != 0 && den  != 00 )
	{
		m_ellipse.angle = 180.0 + (180.0 / CV_PI) * atan( num / den );
	}
	else
	{
		m_ellipse.angle = 0;
	}

	return m_ellipse;

}



//===========================================================================
//CvPoint CGazeDetector::FindEyeCenter2(CvRect EyeROI)
//{
//	EyeImg.Lock();
//
//	IplImage* copy =  cvCreateImage( cvSize(128,128), 8, 3);
//	IplImage* in =  cvCreateImage(cvSize(128,128), 8, 3);
//	//cvCopy(EyeImg.getImg(),copy);
//	//cvCopy(EyeImg.getImg(),in);
//
//	cvResize(EyeImg.getImg(),copy,CV_INTER_LINEAR);
//	cvResize(EyeImg.getImg(),in,CV_INTER_LINEAR);
//
//	cvPyrMeanShiftFiltering(
//		in,
//		copy,
//		40/*g_GUI.param1*/,
//		10/*g_GUI.param2*/,
//		2,
//		cvTermCriteria(
//		CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,
//		10,
//		0.1
//		)
//		);
//
//	g_GUI.ShowImage(in, "orgincamsif");
//
//	g_GUI.ShowImage(copy, "camsif");
//
//	return cvPoint(0,0);;
//
//	//IplImage* gray =  cvCreateImage( cvSize(EyeROI.width,EyeROI.height), 8, 1);
//	//
//
//	//cvCvtColor( EyeImg.getImg(), gray, CV_BGR2GRAY );
//	//IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	////cvEqualizeHist( GrayEye, GrayEye );
//
//	//cvDilate(gray,gray,v,2);
//	//cvSmooth( gray, EdgeEye, CV_BLUR, 5,5,0,0 );
//	//cvEqualizeHist( gray, gray );
//	////g_GUI.ShowImage(gray, "gray");
//	//cvCanny(gray, EdgeEye, 10,250, 3);
//	//cvEqualizeHist( EdgeEye, EdgeEye );
//
//	////g_GUI.ShowImage(EdgeEye, "CannyEye");
//	//EyeImg.Unlock();
//	//return cvPoint(0,0);
//}
//===========================================================================
//CvPoint CGazeDetector::FindEyeCenter(CvRect EyeROI)
//{
//
//	return CGazeDetector::FindEyeCenterMatchDisk( EyeImg);
//}

//===========================================================================


//CvPoint CGazeDetector::FindEyeCenterMatchDisk(CvRect EyeROI)
//{
//	EyeImg.Lock();
//	CvPoint s;
//	double max, min;
//	CvPoint pmax, pmin;
//	IplImage* matchDisk,* matchRing,*result;
//	IplImage* result2;
//	IplImage* TemplateImg,*RingTemplate;
//
//	IplImage* copy = cvCreateImage( cvSize(EyeROI.width,EyeROI.width), 8, 3 );
//	IplImage* gray =  cvCreateImage( cvGetSize(copy), 8, 1);
//	IplImage* Edge =  cvCreateImage( cvGetSize(copy), 8, 1);
//
//
//	float HeightFactor =  (float)EyeROI.height /(float)EyeROI.width ;
//	//template size
//	int x =cvGetSize(gray).width / g_GUI.param1 + 1;
//	x =cvGetSize(gray).width /7 + 1;
//	if (x > cvGetSize(gray).width )  x =cvGetSize(gray).width ;
//	int y = x;
//
//	cvResize(EyeImg.getImg(),copy,CV_INTER_LINEAR);
//	cvCvtColor( copy, gray, CV_BGR2GRAY );
//
//	TemplateImg = cvCreateImage( cvSize(x,y), 8, 1 );; 
//	RingTemplate = cvCreateImage( cvSize(x,y), 8, 1 ); 
//	cvFillImage(TemplateImg,250);
//	cvFillImage(RingTemplate,0);
//	cvFillImage(Edge,67);
//
//	//cvEqualizeHist( gray, gray);
//	//cvCanny(gray,Edge,20,100);
//	cvSobel(gray,Edge,1,0);
//	cvNormalize(Edge,Edge,0,250,CV_MINMAX,0);
//
//	matchDisk =cvCreateImage( cvSize(cvGetSize(gray).height -x +1,cvGetSize(gray).width -y +1),32, 1 );
//	matchRing =cvCreateImage( cvSize(cvGetSize(gray).height -x +1,cvGetSize(gray).width -y +1),32, 1 );
//	result  =cvCreateImage( cvSize(cvGetSize(gray).height -x +1,cvGetSize(gray).width -y +1),32, 1 );
//
//	s.x = x/2;
//	s.y = y/2;
//
//	//cvZero(temp);
//	cvCircle( TemplateImg, s, x/2, CV_RGB(1,1,1), -1, 8,0);
//
//	cvCircle( RingTemplate, s, x/2, CV_RGB(250,250,250), 1, 8,0);
//	//cvEqualizeHist( GrayEye, GrayEye);
//	cvMatchTemplate( gray,TemplateImg,
//		matchDisk,2);
//	//cvEqualizeHist( result, result);
//	cvNormalize(matchDisk,matchDisk,0,1,CV_MINMAX,0);
//
//	cvMatchTemplate( Edge,RingTemplate,
//		matchRing,2);
//	cvNormalize(matchRing,matchRing,0,1,CV_MINMAX,0);
//
//	cvSub(matchDisk, matchRing,  result);
//	cvNormalize(result,result,0,1,CV_MINMAX,0);
//	cvMinMaxLoc( result, &max, &min,&pmin,&pmax,0);
//
//	cvReleaseImage(& result);
//	cvReleaseImage(&TemplateImg );
//	cvReleaseImage(& copy);
//	cvReleaseImage(& gray);
//
//	EyeImg.Unlock();
//	return cvPoint(pmin.x +(x - 1)/2,(pmin.y + (y - 1)/2) *HeightFactor );
//}
//===========================================================================

//void  CGazeDetector::FindEyeCoreners(CvRect EyeROI , CvPoint EyeCenter)
//{
//	CvMemStorage* storage;
//	CvSeq* contures;
//	CvBox2D32f* box;
//	CvPoint* PointArray;
//	CvPoint2D32f* PointArray2D32f;
//	CvSeq *comp;
//
//	// Create dynamic structure and sequence.
//
//	storage = cvCreateMemStorage( 0);
//	contures = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , storage);
//
//	//CvRect EyeRect;
//	//EyeRect =cvRect( m_LeftEyeRect.x + m_LeftEyeRect.width * 0.05,m_LeftEyeRect.y + m_LeftEyeRect.height * 0.25, m_LeftEyeRect.width * 0.9,  m_LeftEyeRect.height * 0.5);
//	//	m_RightEyeRect =cvRect( m_RightEyeRect.x,int(m_RightEyeRect.y +m_RightEyeRect.width * 0.25), m_RightEyeRect.width, int(m_RightEyeRect.width * 0.5));
//
//
//
//
//	IplConvKernel* v =  cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT,NULL);
//	IplImage* copy = cvCreateImage( cvSize(EyeROI.width,EyeROI.height), 8, 3 );
//
//
//	CvSize size = cvSize(cvGetSize(copy).width * 0.9,cvGetSize(copy).height * 0.5);
//	size = cvGetSize(copy);
//
//	int EyeRadius = cvGetSize(copy).width / 4;
//
//	//----------segmentation
//
//	//IplImage* copy = cvCreateImage( cvSize(300,300), 8, 3 );
//	IplImage* gray =  cvCreateImage( size, 8, 1);
//	IplImage* result =  cvCreateImage( size, 8, 3);
//	IplImage* Edge =  cvCreateImage( size, 8, 1);
//
//	cvResize(EyeImg.getImg(),copy,CV_INTER_LINEAR);
//
//	//CvRect EyeRect = cvRect(  size.width * 0.05, size.height * 0.25, size.width,  size.height );
//	//	cvSetImageROI(copy, EyeRect );
//
//	cvCopy(copy,result);
//
//	IplImage* r = cvCreateImage( size, IPL_DEPTH_8U, 1 );
//	IplImage* g = cvCreateImage( size, IPL_DEPTH_8U, 1 );
//	IplImage* b = cvCreateImage( size, IPL_DEPTH_8U, 1 );
//	IplImage* s = cvCreateImage( size, IPL_DEPTH_8U, 3 );
//	cvCvtColor( copy, copy, CV_RGB2HSV );
//	cvSplit( copy, r, g, b, NULL );
//	//cvEqualizeHist( g, g );
//	//
//	//cvEqualizeHist( g, g );
//	//cvEqualizeHist( r, r );
//	//cvEqualizeHist( b, b );
//	//cvEqualizeHist( b, b );
//	cvNormalize(g, g,0,100,CV_MINMAX,0);
//	cvNormalize(r, r,0,100,CV_MINMAX,0);
//	cvNormalize(b, b,0,100,CV_MINMAX,0);
//
//
//
//	int level = g_GUI.param3;
//
//	cvSobel(b,r,1,0);
//	cvNormalize(r, r,0,250,CV_MINMAX,0);
//	/* cvPyrSegmentation(copy, result, storage, &comp, 
//	1, g_GUI.param1+200, g_GUI.param2+50);*/
//
//	g_GUI.ShowImage(r, "sobel");
//
//	cvSplit( copy, r, g, b, NULL );
//	g_GUI.ShowImage(r, "r");
//	g_GUI.ShowImage(g, "g");
//	g_GUI.ShowImage(b, "b");
//
//
//	//cvAddWeighted(b,1,r,-1,0,g);
//	cvSub(b,r,g);
//	cvNormalize(g, g,0,250,CV_MINMAX,0);
//	g_GUI.ShowImage(g, "clear");
//
//
//	cvThreshold( g, g,g_GUI.param1, 250, CV_THRESH_BINARY );
//
//	//cvErode(g,g,v,2);
//	//cvDilate(g,g,v,2);
//	g_GUI.ShowImage(g, "tresh");
//
//
//	//-------edge detection-------------------
//
//
//
//	// Threshold the source image. This needful for cvFindContours().
//
//
//	// Find all contours.
//	cvFindContours( g, storage, &contures, sizeof(CvContour), 
//		CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
//
//
//	cvDrawContours( result, contures, CV_RGB(255,0,0), CV_RGB(0,255,0),1, 1, CV_AA, cvPoint(0,0) );
//
//	// Clear images. IPL use.
//
//	g_GUI.ShowImage(result, "corntu");
//
//
//	//contures = cvApproxPoly( contures, sizeof(CvContour), storage, CV_POLY_APPROX_DP,1, 1 );
//
//
//	//cvDrawContours( result, contures, CV_RGB(0,0,250), CV_RGB(0,255,0),1, 1, CV_AA, cvPoint(0,0) );
//
//	//g_GUI.ShowImage(result, "poly");
//
//
//	///-----------------corners------------------------
//
//
//	class CCorner
//	{
//	public:
//		CvPoint m_LeftCorner, m_RightCorner;
//	};
//
//	CCorner curent;
//	CvPoint LeftCorner, RightCorner;
//
//	vector<CCorner> m_vCorners;
//	for(;contures;contures = contures->h_next)
//	{   
//		int i; // Indicator of cycle.
//
//		int count = contures->total; // This is number point in contour
//
//
//		// Number point must be more than or equal to 6 (for cvFitEllipse_32f).        
//		if( count < 20 )
//			continue;
//
//
//		// Alloc memory for contour point set.    
//		PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
//		//PointArray2D32f= (CvPoint2D32f*)malloc( count*sizeof(CvPoint2D32f) );
//
//
//
//		// Get contour point set.
//		cvCvtSeqToArray(contures, PointArray, CV_WHOLE_SEQ);
//
//		// Convert CvPoint set to CvBox2D32f set.
//
//		if (0<count)
//		{
//			LeftCorner = PointArray[0];
//			RightCorner = PointArray[0];;
//
//		};
//		for(i=0; i<count; i++)	
//		{
//			if(LeftCorner.x > (float)PointArray[i].x) LeftCorner = PointArray[i];
//			if(RightCorner.x < (float)PointArray[i].x) RightCorner = PointArray[i];
//		}
//		curent.m_LeftCorner = LeftCorner;
//		curent.m_RightCorner = RightCorner; 
//		m_vCorners.push_back(curent);
//	}
//
//
//}

//===========================================================================
//void CGazeDetector::DrawSurf(IplImage *ImgIn, IplImage *ImgOut)
//{
//	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
//
//	IplImage *gray = cvCreateImage(cvSize( ImgIn->width, ImgIn->height), 8, 1 );
//
//
//	cvCvtColor( ImgIn, gray, CV_BGR2GRAY );
//
//	//cvEqualizeHist( gray,gray );
//
//
//	CvMemStorage* storage = cvCreateMemStorage(0);
//
//	CvSURFParams aa = cvSURFParams(500, 1);
//
//	//g_GUI.ShowImage(gray, "LeftEyegray");
//
//	cvExtractSURF( gray, 0, &objectKeypoints, &objectDescriptors, storage, aa );
//
//
//
//	int i;
//	CvSeqReader reader, kreader;
//	cvStartReadSeq( objectKeypoints, &kreader );
//	cvStartReadSeq( objectDescriptors, &reader );
//
//	std::cout<<"point count:"<<objectDescriptors->total<<std::endl;
//	for( i = 0; i < objectDescriptors->total; i++ )   
//	{
//		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
//		kp->pt;
//		const float* descriptor = (const float*)reader.ptr;
//		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
//		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
//
//		cvCircle( ImgOut,cvPointFrom32f( kp->pt), 2, CV_RGB(250,0,0), -1, 8,0);
//
//	}
//}

//===========================================================================
