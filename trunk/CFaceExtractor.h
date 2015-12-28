///////////////////////////////////////////////////////////
//  CFaceExtractor.h
//  Implementation of the Class CFaceExtractor
//  Created on:      10-lut-2009 15:41:16
///////////////////////////////////////////////////////////

#pragma once



#include "Utilitis.h"

#define CASCADE_NAME "haarcascade_frontalface_alt.xml"
#define NESTED_CASCADE_NAME "haarcascade_eye.xml"



#define CASCADE_FACE "haarcascade_frontalface_default.xml"
#define CASCADE_LEFT_EYE "haarcascade_mcs_lefteye.xml"
#define CASCADE_RIGHT_EYE "haarcascade_mcs_righteye.xml"
#define CASCADE_MOUTH "haarcascade_mcs_mouth.xml"
#define CASCADE_NOSE "haarcascade_mcs_nose.xml"

						

class CTrackinkSystem;
class CFaceExtractor
{
public:

	CvHaarClassifierCascade* m_pCascade ;
	CvHaarClassifierCascade* m_pNestedCascade;

	CvHaarClassifierCascade* m_pCascadeFace ;
	CvHaarClassifierCascade* m_pCascadeLeftEye ;
	CvHaarClassifierCascade* m_pCascadeRightEye;
	CvHaarClassifierCascade* m_pCascadeNose ;
	CvHaarClassifierCascade* m_pCascadeMouth ;




	CvMemStorage* m_pStorage ;

	int m_iFrameWith, m_iFrameHeight; // orginal img dimensionss
	IplImage **m_pFrame; // curent frame from graber
	IplImage *m_pScaledGrayFrame; // current frame after resizing (scale factor)
	IplImage *m_pGrayFrame;

	CImageRgion* m_pFaceImg;
	CImageRgion* m_pLeftEyeImg;
	CImageRgion* m_pRightEyeImg;
			

	int m_LeftCounter;
		int m_RightCounter;

	int m_iFaceRecognitionFailCount; // ilosc poprzednich iteracji kiedy nie rozpoznano twarzy

	int m_iLeftEyeRecognitionFailCount;
	int m_iRightEyeRecognitionFailCount;

	int m_iCurrentFaceSearchFactor;
	int m_iCurrentEyeSearchFactor; 

	int m_CurrentFaceMinSize;
	int m_CurrentEyeMinSize;


	void 	ComputeScaleFactor(float Width, float Height);

	CvRect m_FaceROIRect;

	bool m_IsFaceLocalized;
	bool m_IsEyeLocalized;

	bool m_bInitailized;

	float m_fScale; // scale factor
	CFaceExtractor();
	~CFaceExtractor();

	//curent frame inicjalization
	void PrepareFrame();
	void CFaceExtractor::PerformFramePreprocessing();

	void Init(int with, int height); // allocate buffors

	void Process(IplImage **Frame);
	bool FindFace();
//	bool FindEyes();

	//void CFaceExtractor::DrawSurf(IplImage *ImgIn, IplImage *ImgOut);

	IplImage * CFaceExtractor::getFaceImg();
	IplImage * CFaceExtractor::FillImageRegion(IplImage * Img,IplImage * ImgRegion, CvRect & Rect, float ScaleFactor = 1);


	CvRect m_FaceRect;
	CvRect m_LeftEyeRect;
	CvRect m_RightEyeRect;

	CvRect m_MouthRect;
	CvRect m_NoseRect;

	

	CvPoint2D32f m_LeftEyeCenter;
	CvPoint2D32f m_RightEyeCenter;

	CvSize m_LeftEyeSize;
	CvSize m_RightEyeSize;


	int m_MinFaceSizeFactor; //  %
	int m_MinEyeSizeFactor; //  %



	int m_iFaceSearchFactor; // okresla jak daleko szukac od poprzedno znalezionej twarzy
	int m_iEyeSearchFactor;

	// okresla czy w poprzednich iteracjach zostalo odnalezione
	bool m_bFaceIsDetected;
	bool m_bLeftEyeIsDetected;
	bool m_bRightEyeIsDetected;
	bool m_bMouthIsDetected;
	bool m_bNoseIsDetected;


	
void  CFaceExtractor::Update();
void CFaceExtractor::FindFactialFuters();
CvPoint2D32f CFaceExtractor::getFaceCenter();
void CFaceExtractor::FindMouths();
void CFaceExtractor::FindNoses();
void CFaceExtractor::FindRightEyes();
void CFaceExtractor::FindLeftEyes();


	bool EyesLocalized()
	{
		return (!( m_iLeftEyeRecognitionFailCount + m_iRightEyeRecognitionFailCount ));

	};

	friend CTrackinkSystem;
};
