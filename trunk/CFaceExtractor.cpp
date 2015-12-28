///////////////////////////////////////////////////////////
//  CFaceExtractor.cpp
//  Implementation of the Class CFaceExtractor
//  Created on:      10-lut-2009 15:41:16
///////////////////////////////////////////////////////////

#include "CFaceExtractor.h"
#include "assert.h"
#include "cv.h"
#include "highgui.h"
#include "GUI.h"
#include "CTrackinkSystem.h"
#include <sys/stat.h>
//===========================================================================
CFaceExtractor::CFaceExtractor(){

	m_LeftCounter =0;
	m_RightCounter = 0;
	m_bInitailized = false;
	m_pFrame = NULL;
	m_pScaledGrayFrame = NULL;
	m_pGrayFrame = NULL;
	m_pCascade = NULL;
	m_pNestedCascade = NULL;

	m_fScale = 3;

	m_MinFaceSizeFactor = 15;
	m_MinEyeSizeFactor = 1; //  %

	//temp
	//m_pFaceImg = NULL;
	//m_pLeftEyeImg= NULL;
	//m_pRightEyeImg= NULL;
	//m_pFaceImgScaled =NULL; 
}

//===========================================================================

CFaceExtractor::~CFaceExtractor(){

	//if (m_pGrayFrame)
	//	cvReleaseImage(&m_pGrayFrame);
	//if (m_pScaledGrayFrame)
	//	cvReleaseImage(&m_pScaledGrayFrame);

	//cvReleaseImageHeader(&m_pFaceImg);
	//cvReleaseImageHeader(&m_pLeftEyeImg);
	//cvReleaseImageHeader(&m_pRightEyeImg);
	//cvReleaseImageHeader(&m_pFaceImgScaled);

	//cvReleaseImage(&m_pScaledGrayFrame);
	//cvReleaseImage(&m_pGrayFrame);

}
//===========================================================================
void CFaceExtractor::Process(IplImage **Frame)
{

	m_IsFaceLocalized = false;
	m_IsEyeLocalized = false;
	m_bLeftEyeIsDetected = false;
	m_bFaceIsDetected = false;
	m_bRightEyeIsDetected = false;

	m_bFaceIsDetected= false;
	m_bLeftEyeIsDetected= false;
	m_bRightEyeIsDetected= false;
	m_bMouthIsDetected= false;
	m_bNoseIsDetected= false;
	if (m_pFrame == NULL)
		return;
	m_pFrame = Frame;
	PrepareFrame();
	//FindFace();
	//if (m_IsFaceLocalized = FindFace())
	//	m_IsEyeLocalized = FindEyes();

}
//===========================================================================

void CFaceExtractor::ComputeScaleFactor(float Width, float Height)
{
	float tmp;
	tmp = m_iFrameWith  / Width;
	m_fScale = m_iFrameHeight / Height;
	m_fScale = m_fScale > tmp ?  m_fScale : tmp;
}

//===========================================================================
void CFaceExtractor::Init(int with, int height)
{

	m_iFaceRecognitionFailCount = 0;
	m_iLeftEyeRecognitionFailCount = 0;
	m_iRightEyeRecognitionFailCount = 0;

	m_iFaceSearchFactor = 20;
	m_iEyeSearchFactor = 20;
	m_bFaceIsDetected = false;
	m_bLeftEyeIsDetected = false;
	m_bRightEyeIsDetected = false;


	try
	{
		struct stat stFileInfo;
		int intStat;


		intStat = stat(CASCADE_LEFT_EYE,&stFileInfo);
		intStat = stat(CASCADE_RIGHT_EYE,&stFileInfo);
		intStat = stat(CASCADE_MOUTH,&stFileInfo);


		m_pCascadeLeftEye= (CvHaarClassifierCascade*)cvLoad( CASCADE_LEFT_EYE, 0, 0, 0 );
		m_pCascade = (CvHaarClassifierCascade*)cvLoad( CASCADE_NAME, 0, 0, 0 );
		m_pNestedCascade= (CvHaarClassifierCascade*)cvLoad( NESTED_CASCADE_NAME, 0, 0, 0 );

		m_pCascadeFace= (CvHaarClassifierCascade*)cvLoad( CASCADE_FACE, 0, 0, 0 );

		m_pCascadeRightEye= (CvHaarClassifierCascade*)cvLoad( CASCADE_RIGHT_EYE, 0, 0, 0 );
		m_pCascadeNose= (CvHaarClassifierCascade*)cvLoad( CASCADE_NOSE, 0, 0, 0 );
		m_pCascadeMouth= (CvHaarClassifierCascade*)cvLoad( CASCADE_MOUTH, 0, 0, 0 );

	}
	catch (...)
	{
		cout<<"loading cascade file fail"<<endl;
		throw "load haarclasificer fail!";
	}

	int block_size = 1000;

	m_pStorage = cvCreateMemStorage ( block_size );

	cvClearMemStorage( m_pStorage );

	if(!m_pCascadeFace || ! m_pCascadeLeftEye || ! m_pCascadeRightEye|| ! m_pCascadeNose|| ! m_pCascadeMouth)


		if(!m_pCascade || ! m_pNestedCascade )
			throw "load haarclasificer fail!";

	m_iFrameWith = with;
	m_iFrameHeight = height; // orginal img dimensionss

	ComputeScaleFactor(with,height);
	m_pGrayFrame = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_pScaledGrayFrame = g_TrackinkSystem->m_Graber.getFrameEqualizeHist();

	m_pFaceImg = new CImageRgion(&m_pScaledGrayFrame,m_fScale);
	m_pLeftEyeImg= new CImageRgion(m_pFrame);
	m_pRightEyeImg= new CImageRgion(m_pFrame);



}

//===========================================================================
//curent frame inicjalization
void CFaceExtractor::PrepareFrame()
{
	assert(*m_pFrame);
	//	m_pGrayFrame = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	//m_pScaledGrayFrame = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_pGrayFrame = g_TrackinkSystem->m_Graber.getFrameEqualizeHist();
	m_pScaledGrayFrame = g_TrackinkSystem->m_Graber.getFrameEqualizeHist();

	PerformFramePreprocessing();
}

//===========================================================================



void  CFaceExtractor::Update()
{
	m_IsFaceLocalized = false;
	m_IsEyeLocalized = false;
	m_bLeftEyeIsDetected = false;
	m_bFaceIsDetected = false;
	m_bRightEyeIsDetected = false;

	m_bFaceIsDetected= false;
	m_bLeftEyeIsDetected= false;
	m_bRightEyeIsDetected= false;
	m_bMouthIsDetected= false;
	m_bNoseIsDetected= false;
PrepareFrame();
}
//===========================================================================

void CFaceExtractor::PerformFramePreprocessing()
{
	//	cvEqualizeHist( m_pGrayFrame, m_pGrayFrame );
	//cvEqualizeHist( m_pScaledGrayFrame, m_pScaledGrayFrame );

}

//===========================================================================

IplImage * CFaceExtractor::FillImageRegion(IplImage * Img,IplImage * ImgRegion, CvRect & Rect, float ScaleFactor)
{

	if ((Rect.x < 0)|| (Rect.y < 0))
	{
		cout<<"FillImageRegion fail negativ corrdinates"<<endl;
		return ImgRegion;
	};
	CvMat small_img_roi;
	CvRect ScaledRect;
	int ChanelCount  = ImgRegion ? ImgRegion->nChannels : 1;

	ChanelCount =  ImgRegion->nChannels;

	//cout<<"chanel:"<<ChanelCount<<endl;
	if (ScaleFactor == 1)
		ScaledRect = Rect;
	else
		ScaledRect = cvRect(Rect.x *ScaleFactor,Rect.y *ScaleFactor,Rect.width*ScaleFactor,Rect.height*ScaleFactor);
	cvInitImageHeader( ImgRegion, cvSize(ScaledRect.width, ScaledRect.height),8,ChanelCount);
	cvGetSubRect( Img, &small_img_roi, ScaledRect );
	ImgRegion = cvGetImage( &small_img_roi, ImgRegion );

	return ImgRegion;
	//return (IplImage*)(((CvArr*)(&small_img_roi)));	
}
//===========================================================================


bool CFaceExtractor::FindFace()
{
	m_bFaceIsDetected= false;
	bool result = false;
	assert(m_pScaledGrayFrame);
	//TODO add fame preprocesing
	cvClearMemStorage( m_pStorage );
	CvRect* LeftEye = NULL;

	CvRect eyeRec;
	CvSeq* faces;

	int MinFaceSize = ((m_MinFaceSizeFactor  * m_pScaledGrayFrame->width / 100)) ;
	MinFaceSize = 5;
	if( m_pStorage )
	{

		m_iCurrentFaceSearchFactor = m_iFaceSearchFactor + m_iFaceSearchFactor*m_iFaceRecognitionFailCount;

		m_CurrentFaceMinSize = 5;

		faces = cvHaarDetectObjects( m_pScaledGrayFrame, m_pCascade, m_pStorage,
			1.1, 2, 0
			|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|CV_HAAR_DO_CANNY_PRUNING
			//|CV_HAAR_SCALE_IMAGEs
			,
			cvSize(m_CurrentFaceMinSize, m_CurrentFaceMinSize) );



		//TODO odnajduje tylko najwieksza twarz

		if (faces && faces->total >= 1)
		{
			CvRect* r = (CvRect*)cvGetSeqElem( faces, 0 );

			m_pFaceImg->SetROI(*r);
			m_FaceRect = *r;

			m_iFaceRecognitionFailCount = 0;
			m_bFaceIsDetected = true;
			result = true;

		}
		else
		{
			m_iFaceRecognitionFailCount++;
			m_bFaceIsDetected = false;
		}



	};
	cvClearSeq(faces);
	return result;	
}
//===========================================================================
//bool CFaceExtractor::FindEyes()
//{
//	bool result = false;
//	assert(m_pFaceImg);
//	CvSeq* nested_objects;
//	//int MinEyeSize =  ((m_MinEyeSizeFactor  * m_pScaledGrayFrame->width / 100)) ;
//
//	m_iCurrentFaceSearchFactor = m_iFaceSearchFactor + m_iFaceSearchFactor*m_iFaceRecognitionFailCount;
//
//
//	if (m_bLeftEyeIsDetected)
//		m_CurrentEyeMinSize =  m_LeftEyeRect.width /m_fScale  - 5*m_iLeftEyeRecognitionFailCount - 5;
//	else		
//		m_CurrentEyeMinSize =  ((m_MinEyeSizeFactor  * m_pFaceImg->m_ROI.width / 100)) ;
//
//
//
//	CvRect OldFaceROI = m_pFaceImg->m_ROI;
//	m_pFaceImg->Unlock();
//
//
//	//	m_EyeROIRect.x = (m_EyeROIRect.x > 0 ?  m_EyeROIRect.x : 0) ;
//	//	m_EyeROIRect.y  = (m_EyeROIRect.y > 0 ?  m_EyeROIRect.y : 0 );
//	//	m_EyeROIRect.height = (m_EyeROIRect.y + m_EyeROIRect.height <  m_pScaledGrayFrame->height ?  m_EyeROIRect.height :  m_pScaledGrayFrame->height - m_FaceROIRect.y) ;
//	m_FaceROIRect.width = (m_FaceROIRect.x + m_FaceROIRect.width <  m_pScaledGrayFrame->width ?  m_FaceROIRect.width :  m_pScaledGrayFrame->width- m_FaceROIRect.x  );
//	m_FaceROIRect.x = 	m_pFaceImg->m_ROI.x + m_pFaceImg->m_ROI.width * 0.1;
//	m_FaceROIRect.y = 	m_pFaceImg->m_ROI.y  + m_pFaceImg->m_ROI.height * 0.15;
//	m_FaceROIRect.height =  m_pFaceImg->m_ROI.height * 0.45;
//	m_FaceROIRect.width =  m_pFaceImg->m_ROI.width * 0.8;
//
//	m_pFaceImg->m_ROI = m_FaceROIRect;
//
//	cvSetImageROI(m_pFaceImg->getImg(), m_FaceROIRect );
//
//
//
//
//	m_pFaceImg->Lock();
//	//g_GUI.ShowImage(m_pFaceImg->getImg(),"roi");
//	nested_objects = cvHaarDetectObjects(m_pFaceImg->getImg(), 
//		m_pNestedCascade, 
//		m_pStorage, 
//		1.1, 
//		2, 
//		0|CV_HAAR_DO_CANNY_PRUNING
//		//|CV_HAAR_SCALE_IMAGE   
//		,
//		cvSize(m_CurrentEyeMinSize, m_CurrentEyeMinSize));
//
//	m_pFaceImg->Unlock();
//
//
//
//	assert(m_pFaceImg);
//
//	if (nested_objects && nested_objects->total >= 2)
//	{
//
//		CvRect* eye1 = (CvRect*)cvGetSeqElem( nested_objects, 0  );
//		CvRect* eye2 = (CvRect*)cvGetSeqElem( nested_objects, 1  );
//
//		if (eye1->x < eye2->x)
//		{
//			m_LeftEyeRect = *eye1;
//			m_RightEyeRect = *eye2;
//		}
//		else
//		{
//			m_LeftEyeRect = *eye2;
//			m_RightEyeRect = *eye1;
//		};
//
//		// przycinenie gory i dolu oka
//		m_LeftEyeRect =cvRect( m_LeftEyeRect.x + m_LeftEyeRect.width * 0.05,m_LeftEyeRect.y + m_LeftEyeRect.height * 0.2, m_LeftEyeRect.width * 0.9,  m_LeftEyeRect.height * 0.6);
//		m_RightEyeRect =cvRect( m_RightEyeRect.x,int(m_RightEyeRect.y +m_RightEyeRect.width * 0.25), m_RightEyeRect.width, int(m_RightEyeRect.width * 0.5));
//
//		m_LeftEyeRect = m_pFaceImg->GetParentRect(m_LeftEyeRect);
//		m_RightEyeRect = m_pFaceImg->GetParentRect(m_RightEyeRect);
//
//		m_pLeftEyeImg->SetROI(m_LeftEyeRect);
//		m_pRightEyeImg->SetROI(m_RightEyeRect);
//
//
//		// przeskalowanie
//		//m_LeftEyeRect =cvRect((m_FaceRect.x + m_LeftEyeRect.x) *m_fScale,
//		//	(m_FaceRect.y + m_LeftEyeRect.y) *m_fScale + m_EyesROIRect.y,
//		//	m_LeftEyeRect.height *m_fScale,
//		//	m_LeftEyeRect.width *m_fScale);
//
//		//m_RightEyeRect =cvRect((m_FaceRect.x + m_RightEyeRect.x) *m_fScale,
//		//	(m_FaceRect.y + m_RightEyeRect.y) *m_fScale +m_EyesROIRect.y,
//		//	m_RightEyeRect.height *m_fScale,
//		//	m_RightEyeRect.width *m_fScale);
//
//
//
//		m_iLeftEyeRecognitionFailCount = 0;
//		m_iRightEyeRecognitionFailCount = 0;
//		m_bLeftEyeIsDetected = true;
//		m_bRightEyeIsDetected = true;
//
//		m_pFaceImg->m_ROI = OldFaceROI;
//		result = true;
//
//
//	}
//	else
//	{
//
//
//		m_iLeftEyeRecognitionFailCount++;
//		m_iRightEyeRecognitionFailCount++;
//		if (m_iFaceRecognitionFailCount >5)
//		{
//			m_bLeftEyeIsDetected = false;
//			m_bRightEyeIsDetected = false;
//		}
//		else
//		{
//			m_bLeftEyeIsDetected = true;
//			m_bRightEyeIsDetected = true;
//		}
//		m_pFaceImg->m_ROI = OldFaceROI;
//
//		//m_bLeftEyeIsDetected = false;
//		//m_bRightEyeIsDetected = false;
//		//m_bLeftEyeIsDetected = false;
//		//m_bRightEyeIsDetected = false;
//		result = false;
//
//	}
//	cvClearSeq(nested_objects);
//	return result;
//}
//===========================================================================
void CFaceExtractor::FindLeftEyes()
{
	m_bLeftEyeIsDetected= false;
	CvSeq* eyeSeq;
	CvRect Roi;
	Roi.x = 	m_FaceRect.x + m_FaceRect.width * 0.1;
	Roi.y = 	m_FaceRect.y  + m_FaceRect.height * 0.2;
	Roi.height =  m_FaceRect.height * 0.45;
	Roi.width = m_FaceRect.width * 0.4;

	cvSetImageROI(m_pScaledGrayFrame, Roi );
	eyeSeq = cvHaarDetectObjects(m_pScaledGrayFrame, m_pCascadeLeftEye, 
		m_pStorage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_FIND_BIGGEST_OBJECT  ,cvSize(Roi.width / 10, Roi.height / 10));
	CvRect* r ;
	m_bLeftEyeIsDetected = false;
	if (eyeSeq && eyeSeq->total >= 1)
	{
		r = (CvRect*)cvGetSeqElem( eyeSeq, 0 );
		//m_LeftEyeRect = *r;

		r->x += Roi.x ;
		r->y += Roi.y ;

		m_bLeftEyeIsDetected = true;
		m_pLeftEyeImg->SetROI(m_LeftEyeRect);	
	};
	cvClearSeq(eyeSeq);
	float alfa = 0.9;
	float beta = 1 - alfa;
	if (m_bLeftEyeIsDetected)
	{
		//if (m_LeftCounter == 0)
		{
			m_LeftEyeCenter.x=  r->x + r->width * 0.5 ;
			m_LeftEyeCenter.y =r->y + r->height * 0.5;
			m_LeftEyeRect = *r;
			m_LeftEyeSize.width = r->width ;
			m_LeftEyeSize.height = r->height ;
		}
		//else
		//{
		//	//TODO : uwzglednic przesuniencie modelu i skale
		//	m_LeftEyeCenter.x = m_LeftEyeCenter.x * alfa +  (r->x + r->width * 0.5)  * beta;
		//	m_LeftEyeCenter.y = m_LeftEyeCenter.y *alfa +  (r->y + r->height * 0.5)  * beta;
		//	m_LeftEyeSize.width = m_LeftEyeSize.width * alfa+ r->width * beta;
		//	m_LeftEyeSize.height =m_LeftEyeSize.height *alfa+  r->height *beta ;
		//};


		m_LeftCounter++;
		m_LeftEyeRect.x   =m_LeftEyeCenter.x  - m_LeftEyeSize.width *0.5 ;
		m_LeftEyeRect.y = m_LeftEyeCenter.y  - m_LeftEyeSize.height *0.5 ;
		m_LeftEyeRect.width = m_LeftEyeSize.width;
		m_LeftEyeRect.height = m_LeftEyeSize.height;
	};

}

//===========================================================================
void CFaceExtractor::FindRightEyes()
{
	m_bRightEyeIsDetected= false;
	CvSeq* eyeSeq;
	CvRect Roi;
	Roi.x = 	m_FaceRect.x + m_FaceRect.width * 0.5;
	Roi.y = 	m_FaceRect.y  + m_FaceRect.height * 0.2;
	Roi.height =  m_FaceRect.height * 0.45;
	Roi.width = m_FaceRect.width * 0.4;

	cvSetImageROI(m_pScaledGrayFrame, Roi );
	eyeSeq = cvHaarDetectObjects(m_pScaledGrayFrame, m_pCascadeRightEye, 
		m_pStorage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_FIND_BIGGEST_OBJECT  ,cvSize(Roi.width / 10, Roi.height / 10));
	CvRect* r ;
	m_bRightEyeIsDetected = false;
	if (eyeSeq && eyeSeq->total >= 1)
	{
		r = (CvRect*)cvGetSeqElem( eyeSeq, 0 );
		//m_RightEyeRect = *r;

		r->x += Roi.x ;
		r->y += Roi.y ;

		m_bRightEyeIsDetected = true;
		m_pRightEyeImg->SetROI(m_RightEyeRect);	
	};
	cvClearSeq(eyeSeq);
	float alfa = 0.9;
	float beta = 1 - alfa;
	if (m_bRightEyeIsDetected)
	{
		//if (m_RightCounter == 0)
		{
			m_RightEyeCenter.x=  r->x + r->width * 0.5 ;
			m_RightEyeCenter.y =r->y + r->height * 0.5;
			m_RightEyeRect = *r;
			m_RightEyeSize.width = r->width ;
			m_RightEyeSize.height = r->height ;
		}
		//else
		//{
		//	//TODO : uwzglednic przesuniencie modelu i skale
		//	m_RightEyeCenter.x = m_RightEyeCenter.x * alfa +  (r->x + r->width * 0.5)  * beta;
		//	m_RightEyeCenter.y = m_RightEyeCenter.y *alfa +  (r->y + r->height * 0.5)  * beta;
		//	m_RightEyeSize.width = m_RightEyeSize.width * alfa+ r->width * beta;
		//	m_RightEyeSize.height =m_RightEyeSize.height *alfa+  r->height *beta ;
		//};
		m_RightCounter++;
		m_RightEyeRect.x   =m_RightEyeCenter.x  - m_RightEyeSize.width *0.5 ;
		m_RightEyeRect.y = m_RightEyeCenter.y  - m_RightEyeSize.height *0.5 ;
		m_RightEyeRect.width = m_RightEyeSize.width;
		m_RightEyeRect.height = m_RightEyeSize.height;
	};

}

//===========================================================================

//void CFaceExtractor::FindRightEyes()
//{
//
//
//	m_bRightEyeIsDetected= false;
//	CvSeq* Seq;
//	CvRect Roi;
//	Roi.x = 	m_FaceRect.x + m_FaceRect.width * 0.5;
//	Roi.y = 	m_FaceRect.y  + m_FaceRect.height * 0.2;
//	Roi.height =  m_FaceRect.height * 0.45;
//	Roi.width = m_FaceRect.width * 0.4;
//
//	cvSetImageROI(m_pScaledGrayFrame, Roi );
//	Seq = cvHaarDetectObjects(m_pScaledGrayFrame, m_pCascadeRightEye, 
//		m_pStorage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_FIND_BIGGEST_OBJECT  ,cvSize(Roi.width / 10, Roi.height / 10));
//
//	m_bRightEyeIsDetected = false;
//	if (Seq && Seq->total >= 1)
//	{
//		CvRect* r = (CvRect*)cvGetSeqElem( Seq, 0 );
//		m_RightEyeRect = *r;
//		m_bRightEyeIsDetected = true;
//		m_pRightEyeImg->SetROI(m_RightEyeRect);
//	};
//	cvClearSeq(Seq);
//
//	m_RightEyeRect.x += Roi.x ;
//	m_RightEyeRect.y += Roi.y ;
//
//
//
//	m_RightEyeCenter.x=  m_RightEyeRect.x + m_RightEyeRect.width * 0.5;
//	m_RightEyeCenter.y = m_RightEyeRect.y + m_RightEyeRect.height * 0.5;
//
//
//}
//===========================================================================
void CFaceExtractor::FindNoses()
{
	m_bNoseIsDetected= false;
	CvSeq* Seq;
	CvRect Roi;
	Roi.x = 	m_FaceRect.x + m_FaceRect.width * 0.3;
	Roi.y = 	m_FaceRect.y  + m_FaceRect.height * 0.3;
	Roi.height =  m_FaceRect.height * 0.6;
	Roi.width = m_FaceRect.width * 0.6;

	cvSetImageROI(m_pScaledGrayFrame, Roi );
	Seq = cvHaarDetectObjects(m_pScaledGrayFrame, m_pCascadeNose, 
		m_pStorage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_FIND_BIGGEST_OBJECT  ,cvSize(Roi.width / 10, Roi.height / 10));

	m_bNoseIsDetected = false;
	if (Seq && Seq->total >= 1)
	{
		CvRect* r = (CvRect*)cvGetSeqElem( Seq, 0 );
		m_NoseRect = *r;
		m_bNoseIsDetected = true;
	};
	cvClearSeq(Seq);
	m_NoseRect.x += Roi.x ;
	m_NoseRect.y += Roi.y ;
}

//===========================================================================

void CFaceExtractor::FindMouths()
{
	m_bMouthIsDetected= false;
	CvSeq* Seq;
	CvRect Roi;
	Roi.x = 	m_FaceRect.x + m_FaceRect.width * 0.2;
	Roi.y = 	m_FaceRect.y  + m_FaceRect.height * 0.6;
	Roi.height =  m_FaceRect.height * 0.4;
	Roi.width = m_FaceRect.width * 0.8;

	cvSetImageROI(m_pScaledGrayFrame, Roi );
	Seq = cvHaarDetectObjects(m_pScaledGrayFrame, m_pCascadeMouth, 
		m_pStorage, 1.1, 2, 0|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_FIND_BIGGEST_OBJECT  ,cvSize(Roi.width / 10, Roi.height / 10));

	m_bMouthIsDetected = false;
	if (Seq && Seq->total >= 1)
	{
		CvRect* r = (CvRect*)cvGetSeqElem( Seq, 0 );
		m_MouthRect = *r;
		m_bMouthIsDetected = true;
	};
	cvClearSeq(Seq);
	m_MouthRect.x += Roi.x ;
	m_MouthRect.y += Roi.y ;
}

//===========================================================================
CvPoint2D32f CFaceExtractor::getFaceCenter()
{
	if (!m_bFaceIsDetected)
		throw("face not detected");
	return cvPoint2D32f( m_FaceRect.x +m_FaceRect.width * 0.5  ,m_FaceRect.y +m_FaceRect.height * 0.5);

}

//===========================================================================
void CFaceExtractor::FindFactialFuters()
{
	PrepareFrame();


	FindFace();
	if (!  m_bFaceIsDetected)
		return;
	FindRightEyes();
	FindLeftEyes();
	FindMouths();
	FindNoses();


	IplImage *img = g_TrackinkSystem->m_Graber.getFrameCopy();
	cvResetImageROI(img );

	if ( m_bFaceIsDetected)
		cvRectangle( img, cvPoint(m_FaceRect.x,m_FaceRect.y) ,cvPoint(m_FaceRect.x +m_FaceRect.width  ,m_FaceRect.y +m_FaceRect.height), CV_RGB(222,222,222),2);

	if ( m_bLeftEyeIsDetected)
		cvRectangle( img, cvPoint(m_LeftEyeRect.x,m_LeftEyeRect.y) ,cvPoint(m_LeftEyeRect.x +m_LeftEyeRect.width ,m_LeftEyeRect.y +m_LeftEyeRect.height), CV_RGB(222,222,222),2);

	if ( m_bRightEyeIsDetected)
		cvRectangle( img, cvPoint(m_RightEyeRect.x,m_RightEyeRect.y) ,cvPoint(m_RightEyeRect.x +m_RightEyeRect.width ,m_RightEyeRect.y +m_RightEyeRect.height), CV_RGB(222,222,222),2);

	//if ( m_bMouthIsDetected)
	//	cvRectangle( img, cvPoint(m_MouthRect.x,m_MouthRect.y) ,cvPoint(m_MouthRect.x +m_MouthRect.width ,m_MouthRect.y +m_MouthRect.height), CV_RGB(222,222,222));

	//if ( m_bNoseIsDetected)
	//	cvRectangle( img, cvPoint(m_NoseRect.x,m_NoseRect.y) ,cvPoint(m_NoseRect.x +m_NoseRect.width ,m_NoseRect.y +m_NoseRect.height), CV_RGB(222,222,222));

	g_GUI.ShowImage(img, "fectial futers");


}
//===========================================================================