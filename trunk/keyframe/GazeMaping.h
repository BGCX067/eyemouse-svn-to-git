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
		m_GazeGraph =  cvCreateImage( cvSize(1200,800), 8, 3);

		m_HeadMoveGraph =  cvCreateImage( cvSize(800,600), 8, 3);
		m_EstimateGraph =  cvCreateImage( cvSize(800,600), 8, 3);
		
	m_pCalibrationScreenAvrImg = cvCreateImage( cvSize(800,600), 8, 3);


	}
public:
	~CGazeMaping(void)
	{
		cvReleaseImage(&m_GazeGraph);
		cvReleaseImage(&m_HeadMoveGraph);
	}

IplImage * m_pCalibrationScreenAvrImg;
	CGazeEstimator m_Estipator;
	CCalibration m_Caliblator;
	int m_IterationCount;

	CvPoint m_ScreenX;
	CvPoint m_ScreenY;

		CvPoint2D32f m_HeadMove;


		list<CvPoint> m_vLeftEye;
		
		list<CvPoint> m_vRightEye;
		list<CvPoint> m_vLeftEyeCalibrated;
		list<CvPoint> m_vLeftKonpensateEye;


		float factor; // plot factor
	CvPoint2D32f m_AvrLeftEyeCenter;
	CvPoint2D32f m_AvrRightEyeCenter;

	CvSize m_LeftEyeSize;


	CvPoint2D32f	HeadGraphCenter;

	CvPoint2D32f m_LeftGaze;
	CvPoint2D32f m_RightGaze;

	CvPoint2D32f m_LeftGazeOF;
	CvPoint2D32f m_RightGazeOF;

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
		//Uncoment
		//if (m_IterationCount==GazeMapInitIterationCount +1)
		//{
		//m_AvrLeftEyeCenter.x = m_AvrLeftEyeCenter.x /GazeMapInitIterationCount; 
		//m_AvrLeftEyeCenter.y = m_AvrLeftEyeCenter.y /GazeMapInitIterationCount; 


		//m_AvrRightEyeCenter.x = m_AvrRightEyeCenter.x /GazeMapInitIterationCount; 
		//m_AvrRightEyeCenter.y = m_AvrRightEyeCenter.y /GazeMapInitIterationCount; 
		//HeadGraphCenter = m_HadeCenter;

		//m_Caliblator.ComputeHeadMoveFactor();

		//};

		//factor =  40;
		//if (m_IterationCount< 5)
		//	factor =  ( m_GazeGraph->width / m_LeftEyeSize.width ) *4;

		//if (m_IterationCount <= GazeMapInitIterationCount)
		//{
		//	m_AvrLeftEyeCenter.x += m_LeftGaze.x;
		//	m_AvrLeftEyeCenter.y += m_LeftGaze.y;

		//	m_AvrRightEyeCenter.x += m_RightGaze.x;
		//	m_AvrRightEyeCenter.y += m_RightGaze.y;


		//	m_AvrLeftEyeCenter.x= m_LeftGaze.x;
		//	m_AvrLeftEyeCenter.y = m_LeftGaze.y;

		//	m_AvrRightEyeCenter.x = m_RightGaze.x;
		//	m_AvrRightEyeCenter.y = m_RightGaze.y;

		//	//calibration
		//	//m_Caliblator.AddMesurePoint(m_LeftEyeCenter,m_HadeCenter);

		//};

	return;
		PlotGraph();

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
