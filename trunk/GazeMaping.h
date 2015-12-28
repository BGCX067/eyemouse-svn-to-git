#pragma once
#include "Utilitis.h"
#include "Calibration.h"
#include "GazeEstimator.h"

const int GazeMapInitIterationCount = 10;
const int PlotGazePointCount = 100;
//maping eye position to screen cordinates
class CGazeMaping
{
public:
	CGazeMaping(void)
	{
		
		m_IterationCount = 0;
		m_AvrLeftEyeCenter.x = 0;
		m_AvrLeftEyeCenter.y = 0;
		m_AvrRightEyeCenter.x = 0;
		m_AvrRightEyeCenter.y = 0;
		//m_GazeGraph =  cvCreateImage( cvSize(800,600), 8, 3);
		m_GazeGraph =  cvCreateImage( cvSize(800,600), 8, 3);

		m_HeadMoveGraph =  cvCreateImage( cvSize(800,600), 8, 3);
		m_EstimateGraph =  cvCreateImage( cvSize(800,600), 8, 3);
		
	m_pCalibrationScreenAvrImg = cvCreateImage( cvSize(800,600), 8, 3);

	 alfa = 0.9;
	 beta = 1 - alfa;
	}
public:
	~CGazeMaping(void)
	{
		cvReleaseImage(&m_GazeGraph);
		cvReleaseImage(&m_HeadMoveGraph);
	}


	 CvPoint2D32f OldLeftGaze;
	 	 CvPoint2D32f OldRightGaze;
		 CvPoint2D32f CurrentGaze;


IplImage * m_pCalibrationScreenAvrImg;
	CGazeEstimator m_Estipator;
	CCalibration m_Caliblator;
	int m_IterationCount;

	CvPoint m_ScreenX;
	CvPoint m_ScreenY;

		CvPoint2D32f m_HeadMove;

	float alfa ;
	float beta ;


		list<CvPoint> m_vLeftEye;
		
		list<CvPoint> m_vRightEye;
		list<CvPoint> m_vLeftEyeCalibrated;
		list<CvPoint> m_vLeftKonpensateEye;

				list<CvPoint> m_vHeadGaze;


		float factor; // plot factor
	CvPoint2D32f m_AvrLeftEyeCenter;
	CvPoint2D32f m_AvrRightEyeCenter;

	CvSize m_LeftEyeSize;


	CvPoint2D32f	HeadGraphCenter;

	CvPoint2D32f m_LeftGaze;
	CvPoint2D32f m_RightGaze;

	CvPoint2D32f m_LeftGazeOF;
	CvPoint2D32f m_RightGazeOF;

	CvPoint2D32f m_LeftGazeKonpensate;
	CvPoint2D32f m_RightGazeKonpensate;

	double  m_LeftGazeKonpensateAlfa;
		double  m_RightGazeKonpensateAlfa;


	CvPoint2D32f  m_LeftGazeKonpensateV;
	CvPoint2D32f  m_RightGazeKonpensateV;



		CvPoint2D32f _m_LeftGazeKonpensate;

	double  _m_LeftGazeKonpensateAlfa;

	CvPoint2D32f  _m_LeftGazeKonpensateV;



	CvPoint2D32f m_MinCorrner;
	CvPoint2D32f m_MaxCorrner;

	//CvPoint2D32f m_LeftEyeCenter;
	//CvPoint2D32f m_RightEyeCenter;

	CvPoint2D32f m_HadeCenter;

	CvPoint2D32f m_LeftEyeCenter;
	CvPoint2D32f m_RightEyeCenter;

	
	CvPoint2D32f m_LeftPupilCenter;
	CvPoint2D32f m_RightPupilCenter;


	double m_HeadDistansScale;

	IplImage* m_GazeGraph ;
	IplImage* m_HeadMoveGraph ;
	IplImage* m_EstimateGraph ;

	//============================================================================
	void Init(  )
	{
		m_LeftGazeKonpensate.x =  0;
		m_LeftGazeKonpensate.y =  0;
		m_LeftGazeKonpensateAlfa = 0.5;


	_m_LeftGazeKonpensate.x =  0;
		_m_LeftGazeKonpensate.y =  0;
		_m_LeftGazeKonpensateAlfa = 0.5;


	};
	//============================================================================
void UpdateCalibration();

	//============================================================================
CvPoint getScreenGazePoint()
{
return m_Caliblator.MapPoint(m_LeftGaze);
};

	//============================================================================
	void Update(  );
	//============================================================================

void PlotCalibrationResult()
{
		//-------------------------------------
		CvPoint	Point;
	int factor = 30 * 10;
	factor =  ( m_GazeGraph->width / m_LeftEyeSize.width ) * 4;	
	
	for (int i = 0 ; i < CalibrationScreenPointsCount; i++)
	{
			Point.x = m_GazeGraph->width/2 + ( m_AvrLeftEyeCenter.x - m_Caliblator.GazePoints[i].x   ) * factor;
			Point.y = m_GazeGraph->height/2 + ( m_AvrLeftEyeCenter.y - m_Caliblator.GazePoints[i].y  ) * factor ;
			cvCircle( m_pCalibrationScreenAvrImg, Point, 5, CV_RGB(0,222,0), -1, 8,0);	
	}
	g_GUI.ShowImage(m_pCalibrationScreenAvrImg, "cal");
	//------------------------------------------------



}

//============================================================================




	void PlotGraph();
	void CGazeMaping::CheckBondary(CvPoint &point );
	//============================================================================




};
