#pragma once

#pragma once

#include "cv.h"
#include "highgui.h"
#include "ImageRgion.h"
#include "EyePosition.h"


const int UpdateTresholdValueInitCount = 10;
class CTrackingFuture;
class CGazeDetector
{
	CvMemStorage* m_MemStorage;
	CvSeq* m_pConturSeq;
	CvSeq* m_pConturSeqBegin;
	IplImage* ImgCopy; 
	IplImage* ImgGrayCopy; 
	IplImage* gray; 
	IplImage* TreshImg ;


public:
	CGazeDetector(void);
public:
	~CGazeDetector(void);

	int m_TresholdValue1;
	int m_TresholdValue2;
	int m_UpdateTresholdValueCount;
	int m_LeftTresholdValue1;
	int m_LeftTresholdValue2;
	int m_RightTresholdValue1;
	int m_RightTresholdValue2;

	
	CvBox2D32f m_LeftPupilElipse;
	CvBox2D32f m_RightPupilElipse;


	CvPoint2D32f m_LeftPupilCenterOF ;
	CvPoint2D32f m_RightPupilCenterOF ;

	CvPoint2D32f m_LeftPupilCenter ;
	CvPoint2D32f m_RightPupilCenter ;

	CvPoint2D32f m_PrevLeftPupilCenter ;
	CvPoint2D32f m_PrevRightPupilCenter ;

	bool m_LeftCenterDetected;
	bool m_RightCenterDetected;



		CvBox2D32f FirstGuaseCenter;
	CvBox2D32f SecondGuaseCenter;

	CvPoint m_Center;

	float m_PupilRadious;

	CEyePosition m_LeftEyePosition;

	CEyePosition m_RightEyePosition;

	CvPoint2D32f m_Center1;
	CvPoint2D32f m_Center2;


	//	void Update( )

		bool CGazeDetector::IsEyeFuter(CTrackingFuture * pFuter);

	void CGazeDetector::DrawSurf(IplImage *ImgIn, IplImage *ImgOut);
	CvPoint CGazeDetector::FindEyeCenter(CvRect EyeROI);


	CvBox2D FitElipse(CvSeq* contour);
	CvPoint2D32f  CGazeDetector::GetBlobCenter(CvSeq* Contour);

//	CvPoint CGazeDetector::FindEyeCenter2(CvRect EyeROI);
	//CvPoint CGazeDetector::FindEyeCenterFitElipse(CvRect EyeROI, CvPoint Center = cvPoint(0,0));
//	void  CGazeDetector::FindEyeCoreners(CvRect EyeROI, CvPoint EyeCenter = cvPoint(0,0));

	//CvPoint CGazeDetector::FindEyeCenterHung(CvRect EyeROI , CvPoint Center);

	//CvPoint2D32f CGazeDetector::FindEyeCenterDoubleTreshold(CvRect EyeROI );
	CvBox2D32f CGazeDetector::FindEyeElipseDoubleTreshold(CvRect EyeROI ,int TreshVal1,int TreshVal2);

	void  CGazeDetector::PreperImg(CvRect EyeROI );
	CvBox2D32f CGazeDetector::GetLeftPupilElipse(CvRect EyeROI);
	CvBox2D32f CGazeDetector::GetRightPupilElipse(CvRect EyeROI);
	bool CGazeDetector::UpdatePupilsCenters();

CvPoint2D32f  CGazeDetector::GetPupilCenterOpticalFlow(CvBox2D32f PupilElipse,CvPoint2D32f PrevPupilCenter );
CvPoint2D32f  CGazeDetector::FindCorner(CvPoint2D32f InitGuas, double TresholdDistans );
void CGazeDetector::InitEyesCorners();

	void CGazeDetector::UpdateTresholdLeft(CvRect EyeROI );
	void CGazeDetector::UpdateTresholdRight(CvRect EyeROI );
void CGazeDetector::UpdateTreshold(CvRect LeftRoi,CvRect RightRoi );
	int CGazeDetector::UpdateOptimalTresholdValue(CvRect EyeROI , int &TreshVal1,int &TreshVal2 );
	CvPoint CGazeDetector::FindEyeCenterBlob(CvRect EyeROI , CvPoint Center);

	CvPoint2D32f CGazeDetector::FindEyeCenterFitElipse(CvRect EyeROI , CvPoint2D32f LastCenter);

	CvPoint CGazeDetector::FindEyeCenterBoost(CvRect EyeROI , CvPoint Center = cvPoint(0,0));

//	CvPoint CGazeDetector::FindEyeCenterMatchDisk(CvRect EyeROI);

	CvBox2D32f CGazeDetector::FitBlobElipse(IplImage* gray , CvPoint2D32f EstimatedCenter, bool UseEstimatedCenter, float &BlobArea);
};


