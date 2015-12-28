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


	 a = 2;
	CvSize PlotEyeSize= cvSize( 100 * a,40 * a);
	 a = a*1.5;
	CvSize TmpPlotEyeSize= cvSize( 100 * a + 10,40 * a + 10);


	 m_LeftEyePlotImg =cvCreateImage( PlotEyeSize, 8, 3); ; 
	 m_RightEyePlotImg = cvCreateImage( PlotEyeSize, 8, 3);; 
	 m_TmpEyePlotImg= cvCreateImage( TmpPlotEyeSize, 8, 3);; 

	ImgCopy =cvCreateImage( ImgSize, 8, 3);
	gray =  cvCreateImage( ImgSize, 8, 1);
	TreshImg =  cvCreateImage( ImgSize, 8, 1);
	plotImg =  cvCreateImage( ImgSize, 8, 3);

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


	//EstimatedCenter.x =EyeROI.width / 2;
	//EstimatedCenter.y = EyeROI.height / 2;

		EstimatedCenter.x = TreshImg->width / 2;
	EstimatedCenter.y =  TreshImg->height / 2;
	// -------------- teshold 1------------------------
	for ( TresholdValue1 = 1 ; TresholdValue1 < 40 ; TresholdValue1++)
	{
		cvThreshold( gray,  TreshImg, TresholdValue1 , 250, CV_THRESH_BINARY );
		FirstGuaseCenter = FitBlobElipse(TreshImg , EstimatedCenter,false,BlobArea);


		if (TreshImg->width * TreshImg->height * ( 1/50.0)  < BlobArea)
			break; 

		if (FirstGuaseCenter.size.width * FirstGuaseCenter.size.height > TreshImg->width * TreshImg->height / 300)
			break; 
	};

	// -------------- teshold 2------------------------
	double CurrentFit = -1000;;

	float MinBlobArea = ((float)TreshImg->width * (float)TreshImg->height )/100.0 ;
	float MaxBlobArea = ((float)TreshImg->width * (float)TreshImg->height ) /5.0 ;

	BestTresholdValue2 =TresholdValue1;

	for ( TresholdValue2 = TresholdValue1 ; TresholdValue2 < 80 ; TresholdValue2++)
	{
		cvThreshold( gray,  TreshImg, TresholdValue2 , 250, CV_THRESH_BINARY );
		SecondGuaseCenter = FitBlobElipse(TreshImg , FirstGuaseCenter.center,true,BlobArea);

		if ((BlobArea > MaxBlobArea)  /*||(BlobArea < MinBlobArea)  */)
			continue;


	float factorX =  (float)TreshImg->width/ (float)g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width;
	float factorY =  (float)TreshImg->height/ (float)g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height;
	float Rx = m_EyeDistans * 0.11 *factorX; 
	float Ry = m_EyeDistans * 0.11 *factorY; 





	/*if ((SecondGuaseCenter.size.width > (float)TreshImg->height / 3.0) || 
			(SecondGuaseCenter.size.height > (float)TreshImg->height / 3.0) )
		continue;*/




		if ((SecondGuaseCenter.size.width >Rx) || 
			(SecondGuaseCenter.size.height > Ry ))
		{
		
		cout<<"second treshold r to big"<<endl;
		break;
		continue;
		}

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


		float diff = 1 + abs(SecondGuaseCenter.size.width - SecondGuaseCenter.size.height);

		if (SecondGuaseCenter.size.width * SecondGuaseCenter.size.height <  MinBlobArea)
			CurrentFit = sqrt((SecondGuaseCenter.size.width * SecondGuaseCenter.size.height));
		else
			CurrentFit = sqrt((SecondGuaseCenter.size.width * SecondGuaseCenter.size.height)) - diff *0.4;

		//CurrentFit = sqrt((SecondGuaseCenter.size.width * SecondGuaseCenter.size.height));
	


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
	//BestTresholdValue2 = TresholdValue1 +  ( BestTresholdValue2 -TresholdValue1 ) / 1.5;

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


	TreshVal1 = TresholdValue1;
	TreshVal2  =BestTresholdValue2; 
	if (TreshVal1 == TreshVal2)
		TreshVal2 += TreshVal1 *0.5 + 1; 


	cout<<"tresh1:"<<TresholdValue1<<endl;
	cout<<"tresh2:"<<BestTresholdValue2<<endl;


	return BestTresholdValue2;
}
//===========================================================================

CvPoint2D32f CGazeDetector::GetEyePlotPoint(CvPoint2D32f point,  CvRect EyeROI)
{
CvPoint2D32f result;
	float factorX =  (float)m_LeftEyePlotImg->width/ (float)EyeROI.width;
	float factorY =  (float)m_LeftEyePlotImg->height / (float)EyeROI.height;

	result.x = cvRound((point.x - EyeROI.x) * factorX);
	result.y = cvRound((point.y - EyeROI.y) * factorY);
	return result;
}
//===========================================================================

void CGazeDetector::DrawEyeFuter(shared_ptr<CTrackingFuture> futer,  CvRect EyeROI,IplImage* Img )
{
	//shared_ptr<CTrackingFuture> futer = g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter;

			//	if (futer->m_Model3DState ==InModelState)
			{
				if (futer->m_FuterLocationSource  == SecondEstymationOFState)
					cvCircle( Img,cvPointFrom32f( GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),4, CV_RGB(0,222,0), -1, 8,0);

				if (futer->m_FuterLocationSource  == FirstEstymationOFState)
					cvCircle( Img,cvPointFrom32f( GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),4, CV_RGB(222,222,0), -1, 8,0);
			};

			if (futer->m_FuterLocationSource  == EyeCorrectState)
					cvCircle( Img,cvPointFrom32f( GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),4, CV_RGB(222,0,0), -1, 8,0);
	
			

			if (futer->m_Model3DState == OutLintersKeyFrameMoveState)
					cvCircle( Img,cvPointFrom32f( GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),6, CV_RGB(222,0,0), 2, 8,0);


			if (futer->m_Model3DState == OutLintersOFMoveState)
					cvCircle( Img,cvPointFrom32f( GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),6, CV_RGB(0,0,222), 2, 8,0);

			
				if ( futer->m_Model3DState  != NotLocalizedState)
					{
						//modle 3d
						
						cvLine(Img,cvPointFrom32f(GetEyePlotPoint(futer->m_CurrentLocation,EyeROI ) ),
						cvPointFrom32f(GetEyePlotPoint((g_TrackinkSystem->m_HeadPosition.ProjectTo2D(futer->m_Model3DCoordinate)),EyeROI ) ), CV_RGB(222,222,222),1);;
						cvCircle( Img,cvPointFrom32f(GetEyePlotPoint((g_TrackinkSystem->m_HeadPosition.ProjectTo2D(futer->m_Model3DCoordinate)),EyeROI ) ),1, CV_RGB(222,222,222), 2, 8,0);

					cvLine(Img, cvPointFrom32f(GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),cvPointFrom32f(GetEyePlotPoint(futer->m_PrevLocation,EyeROI )), CV_RGB(222,0,0),1);;
					cvLine(Img, cvPointFrom32f(GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),cvPointFrom32f(GetEyePlotPoint(futer->m_TransformKeyFrameLocation,EyeROI )), CV_RGB(0,0,222),1);;
					cvLine(Img, cvPointFrom32f(GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),cvPointFrom32f(GetEyePlotPoint(futer->m_bSecondOFEstymationLocation,EyeROI )), CV_RGB(0,222,0),1);;
					cvLine(Img, cvPointFrom32f(GetEyePlotPoint(futer->m_CurrentLocation,EyeROI )),cvPointFrom32f(GetEyePlotPoint(futer->m_bFirstOFEstymationLocation,EyeROI )), CV_RGB(222,222,0),1);;

					};
	//------------------------------------
}
//===========================================================================
void CGazeDetector::PlotLeftEye()
{
	CvRect TmpEyeROI;
	CvRect LeftEyeROI;

	LeftEyeROI.x =cvRound( m_LeftEyeCenter.x - g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/2.5 );
	LeftEyeROI.y = cvRound(m_LeftEyeCenter.y - g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/9.0 );
	LeftEyeROI.width = cvRound(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/1.25 );
	LeftEyeROI.height = cvRound(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/4.5 );

	//m_TmpEyePlotImg
	CvRect EyeROI;
	TmpEyeROI.x =LeftEyeROI.x - 1.0;
	TmpEyeROI.y = LeftEyeROI.y - 1.0;
	TmpEyeROI.width = LeftEyeROI.width + 2.0;
	TmpEyeROI.height = LeftEyeROI.height + 2.0;

	cvSetImageROI(g_TrackinkSystem->m_Graber.m_pFrame, TmpEyeROI );
	cvResize(g_TrackinkSystem->m_Graber.m_pFrame, m_TmpEyePlotImg, /*CV_INTER_NN*/CV_INTER_LINEAR );
	cvResetImageROI(g_TrackinkSystem->m_Graber.m_pFrame);
	float factorX =  (float)m_TmpEyePlotImg->width/ (float)TmpEyeROI.width;
	float factorY =  (float)m_TmpEyePlotImg->height / (float)TmpEyeROI.height;


	EyeROI.x = cvRound(((float)(LeftEyeROI.x)   - (float)TmpEyeROI.x) *factorX );
	EyeROI.y =  cvRound(((float)(LeftEyeROI.y) - (float)TmpEyeROI.y)*factorY);
	EyeROI.width =  cvRound((LeftEyeROI.width) *factorX) ;
	EyeROI.height =  cvRound((LeftEyeROI.height)*factorY);
	cvSetImageROI(m_TmpEyePlotImg, EyeROI );
	cvResize(m_TmpEyePlotImg, m_LeftEyePlotImg, /*CV_INTER_NN*/CV_INTER_LINEAR );
	cvResetImageROI(m_TmpEyePlotImg);

	CvPoint P1 = cvPoint(EyeROI.x,EyeROI.x);
	CvPoint P2 = cvPoint(EyeROI.x +EyeROI.width  ,EyeROI.x + EyeROI.height );
		cvRectangle(m_TmpEyePlotImg,P1,P2,CV_RGB(0,0,0),2);


	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter,LeftEyeROI,m_LeftEyePlotImg);
	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL,LeftEyeROI,m_LeftEyePlotImg);
	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR,LeftEyeROI,m_LeftEyePlotImg);

	//cvCircle( m_LeftEyePlotImg,cvPointFrom32f( GetEyePlotPoint(m_LeftEyeCenter,LeftEyeROI )),4, CV_RGB(222,0,222), -1, 8,0);
	//cvLine(m_LeftEyePlotImg, cvPointFrom32f(GetEyePlotPoint(m_LeftEyeCenter,LeftEyeROI )),cvPointFrom32f(GetEyePlotPoint(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,LeftEyeROI )), CV_RGB(222,0,222),1);;
	//


	 factorX =  (float)m_LeftEyePlotImg->width/ (float)LeftEyeROI.width;
	cvCircle( m_LeftEyePlotImg,cvPointFrom32f( GetEyePlotPoint(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,LeftEyeROI )),factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(), CV_RGB(0,255,0), 1, 8,0);
	
	
	


	
	cvEllipse(m_LeftEyePlotImg, cvPointFrom32f( GetEyePlotPoint(m_LeftEyeCenter,LeftEyeROI )),
		cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
		factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
		0, 0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);


	//------------estymated eye center---------------------

			cvEllipse(m_LeftEyePlotImg, cvPointFrom32f( GetEyePlotPoint(	 g_TrackinkSystem->m_PointTracker.GetLeftEyeCenter(),LeftEyeROI )),
		cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
		factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
		0, 0, 360,CV_RGB(0,0,255), 1, CV_AA, 0);



	CvPoint2D32f tmp;


	////-------eye corner--------
	//tmp.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2;
	//tmp.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2;

	//tmp = PointAdd( tmp, m_LeftEyeCenterShift);
	//	
	//cvEllipse(m_LeftEyePlotImg, cvPointFrom32f( GetEyePlotPoint(tmp,LeftEyeROI )),
	//	cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
	//	factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
	//	0, 0, 360,CV_RGB(0,222,0), 1, CV_AA, 0);


	//
	////-------eye model--------
	//tmp.x = g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_Model3dProjectLocation.x ;
	//tmp.y = g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_Model3dProjectLocation.y ;

	//	
	//cvEllipse(m_LeftEyePlotImg, cvPointFrom32f( GetEyePlotPoint(tmp,LeftEyeROI )),
	//	cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
	//	factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
	//	0, 0, 360,CV_RGB(0,0,222), 1, CV_AA, 0);

//	g_GUI.ShowImage(m_LeftEyePlotImg,"LeftEye");
	//g_GUI.ShowImage(m_TmpEyePlotImg,"tmp LeftEye");
}

//===========================================================================
void CGazeDetector::PlotRightEye()
{
	CvRect TmpEyeROI;
	CvRect RightEyeROI;

	RightEyeROI.x =cvRound( m_RightEyeCenter.x - g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/2.5 );
	RightEyeROI.y = cvRound(m_RightEyeCenter.y - g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/9.0 );
	RightEyeROI.width = cvRound(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/1.25 );
	RightEyeROI.height = cvRound(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/4.5 );

	//m_TmpEyePlotImg
	CvRect EyeROI;
	TmpEyeROI.x =RightEyeROI.x - 1.0;
	TmpEyeROI.y = RightEyeROI.y - 1.0;
	TmpEyeROI.width = RightEyeROI.width + 2.0;
	TmpEyeROI.height = RightEyeROI.height + 2.0;

	cvSetImageROI(g_TrackinkSystem->m_Graber.m_pFrame, TmpEyeROI );
	cvResize(g_TrackinkSystem->m_Graber.m_pFrame, m_TmpEyePlotImg, /*CV_INTER_NN*/CV_INTER_LINEAR );
	cvResetImageROI(g_TrackinkSystem->m_Graber.m_pFrame);
	float factorX =  (float)m_TmpEyePlotImg->width/ (float)TmpEyeROI.width;
	float factorY =  (float)m_TmpEyePlotImg->height / (float)TmpEyeROI.height;


	EyeROI.x = cvRound(((float)(RightEyeROI.x)   - (float)TmpEyeROI.x) *factorX );
	EyeROI.y =  cvRound(((float)(RightEyeROI.y) - (float)TmpEyeROI.y)*factorY);
	EyeROI.width =  cvRound((RightEyeROI.width) *factorX) ;
	EyeROI.height =  cvRound((RightEyeROI.height)*factorY);
	cvSetImageROI(m_TmpEyePlotImg, EyeROI );
	cvResize(m_TmpEyePlotImg, m_RightEyePlotImg, /*CV_INTER_NN*/CV_INTER_LINEAR );
	cvResetImageROI(m_TmpEyePlotImg);

	CvPoint P1 = cvPoint(EyeROI.x,EyeROI.x);
	CvPoint P2 = cvPoint(EyeROI.x +EyeROI.width  ,EyeROI.x + EyeROI.height );
		cvRectangle(m_TmpEyePlotImg,P1,P2,CV_RGB(0,0,0),2);


	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter,RightEyeROI,m_RightEyePlotImg);
	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL,RightEyeROI,m_RightEyePlotImg);
	//DrawEyeFuter(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR,RightEyeROI,m_RightEyePlotImg);

	//cvCircle( m_RightEyePlotImg,cvPointFrom32f( GetEyePlotPoint(m_RightEyeCenter,RightEyeROI )),4, CV_RGB(222,0,222), -1, 8,0);
	//cvLine(m_RightEyePlotImg, cvPointFrom32f(GetEyePlotPoint(m_RightEyeCenter,RightEyeROI )),cvPointFrom32f(GetEyePlotPoint(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,RightEyeROI )), CV_RGB(222,0,222),1);;
	//	

	 factorX =  (float)m_RightEyePlotImg->width/ (float)RightEyeROI.width;
	cvCircle( m_RightEyePlotImg,cvPointFrom32f( GetEyePlotPoint(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,RightEyeROI )),factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(), CV_RGB(0,255,0), 1, 8,0);


		
	cvEllipse(m_RightEyePlotImg, cvPointFrom32f( GetEyePlotPoint(m_RightEyeCenter,RightEyeROI )),
		cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
		factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
		0, 0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);


//------------estymated eye center---------------------

			cvEllipse(m_RightEyePlotImg, cvPointFrom32f( GetEyePlotPoint(	g_TrackinkSystem->m_PointTracker.GetRightEyeCenter(),RightEyeROI )),
		cvSize(factorX*0.25*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans(),
		factorX*0.1*  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()),
		0, 0, 360,CV_RGB(0,0,255), 1, CV_AA, 0);


	//g_GUI.ShowImage(m_RightEyePlotImg,"RightEye");
	//g_GUI.ShowImage(m_TmpEyePlotImg,"tmp RightEye");
}

//===========================================================================
CvBox2D32f CGazeDetector::GetLeftPupilElipse(CvRect EyeROI)
{
	if (m_RightTresholdValue1 < 0 ) 
		throw("negativ left eye treshold");
		//UpdateTresholdLeft( EyeROI );



	m_LeftPupilElipse = FindEyeElipseDoubleTreshold( EyeROI,m_LeftTresholdValue1 ,m_LeftTresholdValue2);;

	cvEllipse(ImgCopy, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 2, CV_AA, 0);
	cvEllipse(ImgCopy, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(0,0,255), 2, CV_AA, 0);
	cvCircle( ImgCopy,cvPointFrom32f(SecondGuaseCenter.center), 2, CV_RGB(222,1,1), -1, 8,0);


	//CvPoint2D32f point;
	//point.x =( g_TrackinkSystem->m_HeadPosition.EstimateLeftEyeCenter2D.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_HeadPosition.EstimateLeftEyeCenter2D.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,222,1), -1, 8,0);


	//point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(222,222,1), -1, 8,0);

	//point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);

	//point.x =( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);


	//CvPoint2D32f LeftCenter;
	//LeftCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2.0;
	//LeftCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2.0;


	//LeftCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2.0;
	//LeftCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2.0;




	//LeftCenter.x = (LeftCenter.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//LeftCenter.y = (LeftCenter.y- EyeROI.y) *  gray->height  /EyeROI.height ;

	//cvCircle( ImgCopy,cvPointFrom32f(LeftCenter), 4, CV_RGB(1,0,222), 1, 8,0);


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
	//plot tmp

	if (m_RightTresholdValue1 < 0 ) 
		UpdateTresholdRight( EyeROI );

	m_RightPupilElipse = FindEyeElipseDoubleTreshold( EyeROI,m_RightTresholdValue1 ,m_RightTresholdValue2);

	cvEllipse(ImgCopy, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 2, CV_AA, 0);
	cvEllipse(ImgCopy, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(0,0,255), 2, CV_AA, 0);
	cvCircle( ImgCopy,cvPointFrom32f(SecondGuaseCenter.center), 2, CV_RGB(222,1,1), -1, 8,0);



	//CvPoint2D32f point;
	//point.x =( g_TrackinkSystem->m_HeadPosition.EstimateRightEyeCenter2D.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_HeadPosition.EstimateRightEyeCenter2D.y- EyeROI.y) *  gray->height  /EyeROI.height ;	
	//cvCircle( ImgCopy,cvPointFrom32f(point), 3, CV_RGB(1,222,1), 3, 8,0);

	//cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,222,1), -1, 8,0);
	//point.x = gray->width /2 ;
	//point.y = gray->height /2 ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 2, CV_RGB(1,0,222), -1, 8,0);




	//point.x =( g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(222,222,1), -1, 8,0);




	//point.x =( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);

	//point.x =( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//point.y = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y- EyeROI.y) *  gray->height  /EyeROI.height ;
	//cvCircle( ImgCopy,cvPointFrom32f(point), 4, CV_RGB(0,0,222), -1, 8,0);


	//CvPoint2D32f RightCenter;
	//RightCenter.x = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	//RightCenter.y = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;
	//RightCenter.x = (RightCenter.x- EyeROI.x) *  gray->width  /EyeROI.width ;
	//RightCenter.x = (RightCenter.y- EyeROI.y) *  gray->height  /EyeROI.height ;


	//cvCircle( ImgCopy,cvPointFrom32f(RightCenter), 4, CV_RGB(1,0,222), 1, 8,0);


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
bool CGazeDetector::InitPupilsCenters()
{
	m_PrevLeftPupilCenter = m_LeftPupilCenter ;
	m_PrevRightPupilCenter = m_RightPupilCenter ;


	m_UpdateTresholdValueCount = 0;

	CvPoint2D32f PLeft, PRight;
	PLeft.x  = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.5;
	PLeft.y  = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height * 0.5;
	
	PRight.x  = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.5;
	PRight.y  = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height * 0.5;
	

	m_EyeDistans = PointDistans(PLeft, PRight);

	cout<<"eye distans"<<m_EyeDistans<<endl;
	if (m_UpdateTresholdValueCount < UpdateTresholdValueInitCount )
	{
		UpdateTresholdLeft(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
		UpdateTresholdRight(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect );
		m_UpdateTresholdValueCount++;
	};

	GetLeftPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
	GetRightPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect);


	m_LeftPupilCenter.x = m_LeftPupilElipse.center.x;
	m_LeftPupilCenter.y = m_LeftPupilElipse.center.y;
	m_RightPupilCenter.x = m_RightPupilElipse.center.x;
	m_RightPupilCenter.y = m_RightPupilElipse.center.y;

		m_EyeDistans = PointDistans(m_LeftPupilCenter,m_RightPupilCenter);

	//=================poprawka z nowym dystansem=====================================

		UpdateTresholdLeft(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
		UpdateTresholdRight(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect );

			GetLeftPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect);
	GetRightPupilElipse(g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect);


	m_LeftPupilCenter.x = m_LeftPupilElipse.center.x;
	m_LeftPupilCenter.y = m_LeftPupilElipse.center.y;
	m_RightPupilCenter.x = m_RightPupilElipse.center.x;
	m_RightPupilCenter.y = m_RightPupilElipse.center.y;

//=================end poprawka z nowym dystansem=====================================


	m_EyeDistans = PointDistans(m_LeftPupilCenter,m_RightPupilCenter);

	InitEyesCorners();


	m_LeftEyeCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2;
	m_LeftEyeCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2;

	m_RightEyeCenter.x = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	m_RightEyeCenter.y = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;




	//m_LeftEyeCenter= g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_Model3dProjectLocation;
	//m_RightEyeCenter=  g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_Model3dProjectLocation;




m_LeftEyeCenterShift = PointSub(m_LeftPupilCenter,m_LeftEyeCenter);
m_RightEyeCenterShift = PointSub(m_RightPupilCenter,m_RightEyeCenter);

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

}


//===========================================================================
bool CGazeDetector::UpdatePupilsCenters()
{

	float alfa = 0;
	// ---------- using optical flow to estimate current eye center
	m_LeftEyeCenter.x = m_LeftEyeCenter.x * alfa + (1-alfa)*(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2;
	m_LeftEyeCenter.y = m_LeftEyeCenter.y * alfa +(1-alfa)*(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2;

	m_RightEyeCenter.x =  m_RightEyeCenter.x * alfa +(1-alfa)*(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	m_RightEyeCenter.y =  m_RightEyeCenter.y * alfa +(1-alfa)*(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;



		// ---------- using optical flow to estimate current eye center
	m_LeftEyeCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2;
	m_LeftEyeCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2;

	m_RightEyeCenter.x = ( g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	m_RightEyeCenter.y = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;




	//m_LeftEyeCenter= g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_Model3dProjectLocation;
	//m_RightEyeCenter=  g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_Model3dProjectLocation;



	m_LeftEyeCenter = PointAdd( m_LeftEyeCenter, m_LeftEyeCenterShift);
	m_RightEyeCenter = PointAdd( m_RightEyeCenter, m_RightEyeCenterShift);


	//--------------from futers---------------------


	m_LeftEyeCenter =  g_TrackinkSystem->m_PointTracker.GetLeftEyeCenter();
	m_RightEyeCenter =  g_TrackinkSystem->m_PointTracker.GetRightEyeCenter();



	

	m_LeftEyeCenter = g_pTrackinkSystem->m_PointTracker.m_LeftEyeCenterEstymation;
	m_RightEyeCenter =  g_pTrackinkSystem->m_PointTracker.m_RightEyeCenterEstymation;

	//CvPoint2D32f tmp = cvPoint2D32f(0,0);






//-----------end corect face rect-------------------------------------------------
			return (true );

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

	

	 //factorX =  (float)m_LeftEyePlotImg->width/ (float)LeftEyeROI.width;





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


float EyeDistans = PointDistans(m_LeftPupilCenter,m_RightPupilCenter);

LeftL.x = m_LeftPupilCenter.x - EyeDistans*0.3;
LeftL.y = m_LeftPupilCenter.y - EyeDistans*0.01;
LeftR.x = m_LeftPupilCenter.x + EyeDistans*0.3;
LeftR.y = m_LeftPupilCenter.y + EyeDistans*0.01;


RightL.x = m_RightPupilCenter.x - EyeDistans*0.3;
RightL.y = m_RightPupilCenter.y - EyeDistans*0.01;
RightR.x = m_RightPupilCenter.x + EyeDistans*0.3;
RightR.y = m_RightPupilCenter.y + EyeDistans*0.01;


//LeftL.x = m_LeftPupilCenter.x - EyeDistans*0.2;
//LeftL.y = m_LeftPupilCenter.y - EyeDistans*0.3;
//LeftR.x = m_LeftPupilCenter.x + EyeDistans*0.2;
//LeftR.y = m_LeftPupilCenter.y - EyeDistans*0.3;
//
//
//RightL.x = m_RightPupilCenter.x - EyeDistans*0.2;
//RightL.y = m_RightPupilCenter.y - EyeDistans*0.3;
//RightR.x = m_RightPupilCenter.x + EyeDistans*0.2;
//RightR.y = m_RightPupilCenter.y - EyeDistans*0.3;

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



static bool T1DebugPlotLeft;
	static bool T2DebugPlotLeft;


	cvSetImageROI(g_TrackinkSystem->m_Graber.m_pFrame, EyeROI );
	cvResize(g_TrackinkSystem->m_Graber.m_pFrame, ImgCopy, /*CV_INTER_NN*/CV_INTER_LINEAR );
	//cvSmooth(ImgCopy, ImgCopy, CV_GAUSSIAN, 3);
	int x = g_GUI.param1 + 1;
	IplConvKernel* v =  cvCreateStructuringElementEx(9,9,5,5,CV_SHAPE_RECT,NULL);
	
	//cvDilate(ImgCopy,ImgCopy,v,g_GUI.param3);
	cvCvtColor( ImgCopy, gray, CV_BGR2GRAY );
	cvNormalize(gray,gray,250,0,CV_MINMAX,0);


//
//	if (g_PlotAllDebug)
//	{
//			ostringstream ss;
//			if (T1DebugPlotLeft)
//	ss <<  "clear"<<"L";
//else
//	ss <<  "clear"<<"R";
//	
//T1DebugPlotLeft = ! T1DebugPlotLeft;
//
//	string name = ss.str();
//	g_GUI.ShowImage(gray,(char*)name.c_str());
//
//	};


	cvErode(gray,gray,v,2);
	cvDilate(gray,gray,v,2);
	//cvEqualizeHist( gray, gray );
	cvNormalize(gray,gray,250,0,CV_MINMAX,0);
	cvResetImageROI(g_TrackinkSystem->m_Graber.m_pFrame);

//		if (g_PlotAllDebug)
//	{
//ostringstream ss;
//			if (T1DebugPlotLeft)
//	ss <<  "Erode"<<"L";
//else
//	ss <<  "Erode"<<"R";
//	
//T1DebugPlotLeft = ! T1DebugPlotLeft;
//
//	string name = ss.str();
//	g_GUI.ShowImage(gray,(char*)name.c_str());
//
//	};

		
}

//===========================================================================
CvBox2D32f CGazeDetector::FindEyeElipseDoubleTreshold(CvRect EyeROI ,int TreshVal1,int TreshVal2)
{
static bool T2DebugPlotLeft;
static bool T1DebugPlotLeft;
	float BlobAre;
	//CvBox2D32f FirstGuaseCenter;
	//CvBox2D32f SecondGuaseCenter;

	PreperImg(EyeROI);

	//cvEqualizeHist( gray, gray );
	cvThreshold( gray,  TreshImg, TreshVal1 , 250, CV_THRESH_BINARY );

	cvCvtColor( TreshImg, plotImg, CV_GRAY2RGB );


	CvPoint2D32f EstimatedCenter;
	EstimatedCenter.x = EyeROI.width / 2;
	EstimatedCenter.y = EyeROI.height / 2;
	FirstGuaseCenter = FitBlobElipse(TreshImg , EstimatedCenter,false,BlobAre);




	if (FirstGuaseCenter.size.width == 0)
		FirstGuaseCenter.center = EstimatedCenter;

ostringstream ss;
//if (g_PlotAllDebug)
//{
//	if (T1DebugPlotLeft)
//	ss <<  "Tresh 1"<<"L";
//else
//	ss <<  "Tresh 1"<<"R";
//	
//T1DebugPlotLeft = ! T1DebugPlotLeft;
//	int i = EyeROI.x;
//	
//
//	string name = ss.str();
//
//	cvEllipse(plotImg, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 2, CV_AA, 0);
//
//
//	//cvEllipse(TreshImg, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);
// g_GUI.ShowImage(plotImg,(char*)name.c_str());
//};


	cvThreshold( gray,  TreshImg,TreshVal2, 250, CV_THRESH_BINARY );
cvCvtColor( TreshImg, plotImg, CV_GRAY2RGB );




	SecondGuaseCenter = FitBlobElipse(TreshImg , FirstGuaseCenter.center, true,BlobAre);
	

//		if (g_PlotAllDebug)
//	{
//			ostringstream ss;
//
//if (T2DebugPlotLeft)
//	ss <<  "Tresh 2"<<"L";
//else
//	ss <<  "Tresh 2"<<"R";
//T2DebugPlotLeft = ! T2DebugPlotLeft;
//	int i = EyeROI.x;
//
//	string name = ss.str();
//
//	cvEllipse(plotImg, cvPointFrom32f(SecondGuaseCenter.center), cvSize(SecondGuaseCenter.size.width,SecondGuaseCenter.size.height),SecondGuaseCenter.angle, 0, 360,CV_RGB(255,0,0), 2, CV_AA, 0);
// g_GUI.ShowImage(plotImg,(char*)name.c_str());
//	};

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

	cvErode(gray,gray,NULL,4);
	cvDilate(gray,gray,NULL,4);

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


	
TmpResult = FitElipse(m_pConturSeq);


		if (UseEstimatedCenter)
		{
			CvRect rect = cvContourBoundingRect(m_pConturSeq);

			/*		CvBox2D32f tmp = FitElipse(m_pConturSeq);
			CurrentScore = 1000000 -  (tmp.center.x - EstimatedCenter.x)*(tmp.center.x - EstimatedCenter.x) +
			(tmp.center.y - EstimatedCenter.y)*(tmp.center.y - EstimatedCenter.y);*/

		CurrentScore = 2 * sqrt(CurrentBlobArea)- PointDistans(TmpResult.center,EstimatedCenter );

			if (!((EstimatedCenter.x > rect.x )&&
				(EstimatedCenter.x < rect.x  +rect.width  )&&
				(EstimatedCenter.y > rect.y )&&
				(EstimatedCenter.y < rect.y + rect.height  )))
				CurrentScore = -1; // curent contour dont containt estimated center  

			if ( (abs(EstimatedCenter.x - (rect.x  + rect.width* 0.5) ) > gray->width/4.0 ) || 
				(abs(EstimatedCenter.y -(rect.y  + rect.height* 0.5) ) > gray->height/3.0 ))
				CurrentScore = -1; 
		}
		else
		{
	
			
		CvPoint2D32f center = cvPoint2D32f( gray->width * 0.5 , gray->height * 0.5 );
		CurrentScore =sqrt( CurrentBlobArea) /( 1 + PointDistans(TmpResult.center,center ));
		CurrentScore = CurrentBlobArea;
		}






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

