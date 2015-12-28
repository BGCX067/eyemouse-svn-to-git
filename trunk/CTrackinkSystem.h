///////////////////////////////////////////////////////////
//  CTrackinkSystem.h
//  Implementation of the Class CTrackinkSystem
//  Created on:      10-lut-2009 15:40:47
//  Original author: Szymon
///////////////////////////////////////////////////////////
#pragma once
//#include <string>
//using namespace std;
#include "Utilitis.h"

#include "Graber.h"
#include "CFaceExtractor.h"
#include "GazeDetector.h"
//#include <string>
//sing namespace std;

#include "EyePosition.h"
#include "PointTracker.h"
#include "GazeMaping.h"
#include "EyeCorner.h"
#include "Rejestrator.h"
#include "HeadCenter.h"
#include "HeadPositon.h"


#include  "Timer.h"

const float DispleyFactorY = 1 ;//1.3;

enum TraskingSystemFaze {  InitFaze, CalibrationFaze, TrackingFaze};

class CTrackinkSystem 
{
public:
	TraskingSystemFaze m_CurentFaze;
	CTimer m_Timer;
	CRejestrator m_VideoRejestrator;
	CvPoint m_CurrentCalibrationScreenPoint ;
	int m_IterationCount;
	bool m_bCalibrationFaze;
	CGraber m_Graber;
	CFaceExtractor M_FaceExtractor;
	CGazeDetector m_GazeDetector;

	bool m_bKeyInitModel3D;
	bool m_bKeyChangeRef3DPoint;
	bool m_bKeyPause;
	bool	m_bKeyNextFrame ;
	bool m_bKeyStartCalibrationHeadAngle ;
		bool m_bKeyStartCalibrationHeadPosition ;
	bool m_bKeyClearPlot ;

	bool m_bKeyDebugPlot;

	bool m_bKeyStartCalibration;

	bool m_bKeyPaint;
	CvPoint2D32f m_HeadCenterPoint;

	// Srodek zrenicy
	//CvPoint2D32f LeftEyeCenter;
	//CvPoint2D32f RightEyeCenter;


	// lokalizacja glowy
	//CvRect FaceCenterRoi;

	CCalibrationPlot m_CalibrationPlot;



	CHeadPositon m_HeadPosition;

	CPointTracker m_PointTracker;


	CPointTracker m_EyePointTracker;

	CEyeCorner m_LeftEyeCorner;
	CEyeCorner m_EyesCenter;

//	CHeadCenter m_HeadCenter;

	CGazeMaping m_GazeMaping;

	CTrackinkSystem();
	~CTrackinkSystem();


	bool Process();
	void Start();
	void CTrackinkSystem::SaveKeyPresed(void);
	void CTrackinkSystem::LoadKeyPresed(void);

	void ShowCalibrationPoint();

	
	CEyePosition m_LeftEyePosition;

	CEyePosition m_RightEyePosition;

		IplImage *m_pCalibrationScreen;



};
