#include "GazeMaping.h"
#include "CTrackinkSystem.h"
//============================================================================
void CGazeMaping::CheckBondary(CvPoint &point )
{
				if (point.x > m_GazeGraph->width)
				point.x=  m_GazeGraph->width;

					if (point.x <0)
				point.x=  0;
			if (point.y > m_GazeGraph->height)
				point.y=  m_GazeGraph->height;

					if (point.y <0)
				point.y=  0;

}
//================================================================================================

void CGazeMaping::PlotGraph()
{

	//czyszczene 
	if(	g_TrackinkSystem->m_bKeyInitModel3D || g_TrackinkSystem->m_bKeyClearPlot)
	{
		m_vRightEye.clear();
		m_vLeftEye.clear();
		m_vLeftEyeCalibrated.clear();
	};

	CvPoint	Point;
	CvPoint	LeftGaze;
	CvPoint	RightGaze;
	m_AvrLeftEyeCenter.x = 0;
	m_AvrLeftEyeCenter.y = 0;
	m_AvrRightEyeCenter.x = 0;
	m_AvrRightEyeCenter.y = 0;

	//----------left eye orginal----------
	////if (this->m_Caliblator.m_bHomograpyMatrixCoputed)
	////{
	////	RightGaze =  m_Caliblator.GetMapedPoint( m_LeftGaze, m_RightGaze, cvSize(m_GazeGraph->width,m_GazeGraph->width) );
	////}
	////else
	//{
	//	RightGaze.x = m_GazeGraph->width/2 -( m_AvrRightEyeCenter.x - m_RightGaze.x   ) * factor;
	//	RightGaze.y = m_GazeGraph->height/2 + ( m_AvrRightEyeCenter.y- m_RightGaze.y  ) * factor ;
	//};



		RightGaze.x = m_GazeGraph->width/2 -( m_AvrRightEyeCenter.x - m_LeftGazeOF.x   ) * factor;
		RightGaze.y = m_GazeGraph->height/2 + ( m_AvrRightEyeCenter.y- m_LeftGazeOF.y  ) * factor ;
	

	m_vRightEye.push_back( RightGaze);
	if (m_vRightEye.size() >PlotGazePointCount )
		m_vRightEye.erase(m_vRightEye.begin());


	//----------right eye orginal----------



	//if (this->m_Caliblator.m_bHomograpyMatrixCoputed)
	//{
	//	LeftGaze =  m_Caliblator.GetMapedPoint( m_LeftGaze, m_RightGaze, cvSize(m_GazeGraph->width,m_GazeGraph->width) );
	//}
	//else
	{
		LeftGaze.x = m_GazeGraph->width/2 - ( m_AvrLeftEyeCenter.x - m_LeftGaze.x   ) * factor;
		LeftGaze.y = m_GazeGraph->height/2 +( m_AvrLeftEyeCenter.y - m_LeftGaze.y  ) * factor ;
	};



	m_vLeftEye.push_back( LeftGaze);
	if (m_vLeftEye.size() >PlotGazePointCount )
		m_vLeftEye.erase(m_vLeftEye.begin());


	static CvPoint2D32f PrevLeftEyeCenter;
	cvZero(m_GazeGraph);
	//CvPoint2D32f

	cvCircle( m_GazeGraph,LeftGaze, 5, CV_RGB(222,0,0), -1, 8,0);
	cvCircle( m_GazeGraph,RightGaze, 5, CV_RGB(0,222,0), -1, 8,0);
	PrevLeftEyeCenter  = m_LeftEyeCenter ;


	//if (this->m_Caliblator.m_bHomograpyMatrixCoputed)
	//{
		LeftGaze =  m_Caliblator.GetMapedPointLeftEye( m_LeftGazeOF, cvSize(m_GazeGraph->width,m_GazeGraph->height) );


		CheckBondary(LeftGaze);

		cvCircle( m_GazeGraph,LeftGaze, 7, CV_RGB(222,222,0), -1, 8,0);



		LeftGaze =  m_Caliblator.GetMapedPointLeftEye( m_LeftGazeOF, cvSize(m_GazeGraph->width,m_GazeGraph->height) );
			
     //  cvCircle( m_GazeGraph,LeftGaze, 7, CV_RGB(222,0,222), -1, 8,0);


		LeftGaze.x -= m_GazeGraph->width* 0.5  - 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.x*(float)m_GazeGraph->width;
		LeftGaze.y -= m_GazeGraph->height *0.5 - 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.y*(float)m_GazeGraph->height;

		LeftGaze.x += m_GazeGraph->width* 0.5  - 	 m_Caliblator.m_HeadShift.x*(float)m_GazeGraph->width;
		LeftGaze.y += m_GazeGraph->height *0.5 - 	 m_Caliblator.m_HeadShift.y*(float)m_GazeGraph->height;

		

	if ( !m_Caliblator.m_bLeftEyeAngleMapCoputed)
		{
		LeftGaze.x = 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.x*(float)m_GazeGraph->width;
		LeftGaze.y =  	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.y*(float)m_GazeGraph->height;
		}

		CheckBondary(LeftGaze);
	
       cvCircle( m_GazeGraph,LeftGaze, 7, CV_RGB(222,0,0), -1, 8,0);
	
	

	m_vLeftEyeCalibrated.push_back( LeftGaze);
	if (m_vLeftEyeCalibrated.size() >PlotGazePointCount )
		m_vLeftEyeCalibrated.erase(m_vLeftEyeCalibrated.begin());


	//CvPoint2D32f TmpGaze;
	//TmpGaze.x = m_HadeCenter.x - g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenterOF.x;
	//TmpGaze.y = m_HadeCenter.y - g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenterOF.y;
	//Point.x = m_GazeGraph->width/2 + ( m_AvrLeftEyeCenter.x + TmpGaze.x   ) * factor;
	//Point.y = m_GazeGraph->height/2 + ( m_AvrLeftEyeCenter.y - TmpGaze.y  ) * factor ;
	////cvCircle( m_GazeGraph,(Point), 3, CV_RGB(0,0,222), -1, 8,0);
	//m_vLeftKonpensateEye.push_back( Point);
	//if (m_vLeftKonpensateEye.size() >PlotGazePointCount )
	//	m_vLeftKonpensateEye.erase(m_vLeftKonpensateEye.begin());

	list<CvPoint>::iterator it;


	int	tmp = 0;
	for( it = m_vLeftEye.begin(); it != m_vLeftEye.end(); it++)
	{
		cvCircle( m_GazeGraph,(*it), 1, CV_RGB(50+ (tmp * 200)/m_vLeftEye.size(),0,0), -1, 8,0);
		tmp++;
	};

	tmp = 0;
	for( it = m_vRightEye.begin(); it != m_vRightEye.end(); it++)
	{
		cvCircle( m_GazeGraph,(*it), 1, CV_RGB(0,0,50+ (tmp * 200)/m_vLeftEye.size()), -1, 8,0);
		tmp++;
	};


	tmp = 0;
	for( it = m_vLeftEyeCalibrated.begin(); it != m_vLeftEyeCalibrated.end(); it++)
	{
		int color = 50+ (tmp * 200)/m_vLeftEyeCalibrated.size();
		cvCircle( m_GazeGraph,(*it), 1, CV_RGB(color,color,0), -1, 8,0);
		tmp++;
	};
//----------------------- all calibration mix---------------------
	CCalibrationMesure mesure;
	mesure.UpdateData();
	CvPoint  Pt =   m_Caliblator.GetMapedPoint(mesure,  cvSize(m_GazeGraph->width,m_GazeGraph->height));
	cvCircle( m_GazeGraph,Pt , 6, CV_RGB(0,222,222), -1, 8,0);






	//----------left eye estimate----------
	//CvPoint2D32f CurrentPoint;
	//Point.x = m_GazeGraph->width/2 - ( m_AvrLeftEyeCenter.x - m_LeftGaze.x   ) * factor;
	//Point.y = m_GazeGraph->height/2 +( m_AvrLeftEyeCenter.y - m_LeftGaze.y  ) * factor ;

	//CurrentPoint.x =m_GazeGraph->width/2 - ( m_AvrLeftEyeCenter.x - m_LeftGaze.x   ) * factor;
	//CurrentPoint.y = m_GazeGraph->height/2 +( m_AvrLeftEyeCenter.y - m_LeftGaze.y  ) * factor;
	//CurrentPoint =  m_Estipator.GetKonpensatePosition(CurrentPoint);
	//Point = cvPointFrom32f(CurrentPoint);
	//cvCircle( m_GazeGraph,Point, 3, CV_RGB(222,222,222), 3, 8,0);



	
		cvNamedWindow( "trajectory", 0 );
		cvShowImage("trajectory", m_GazeGraph);
	//g_GUI.ShowImage(m_GazeGraph, "trajectory");

}
//========================================================================================

void  CGazeMaping::UpdateCalibration()
{
CCalibrationMesure tmp;
tmp.UpdateData();
//tmp.m_LeftGaze =m_LeftGazeOF ;
//tmp.m_RightGaze = m_RightGazeOF;
//tmp.m_ScreenPoint =  g_TrackinkSystem->m_CalibrationPlot.GetCurrentPoint();
//tmp.m_MesureType  = g_TrackinkSystem->m_CalibrationPlot.m_CalibrationType;
//	
//tmp.m_HeadPitch =g_TrackinkSystem->m_HeadPosition.m_fModelScale *  tan( g_TrackinkSystem->m_HeadPosition.m_HeadPitch);
//tmp.m_HeadRoll  =g_TrackinkSystem->m_HeadPosition.m_fModelScale * tan( g_TrackinkSystem->m_HeadPosition.m_HeadRoll ) ;
//
//tmp.m_HeadAngle = cvPoint2D32f( tmp.m_HeadPitch,tmp.m_HeadRoll);
m_Caliblator.AddMesure(tmp);

}
//========================================================================================
void CGazeMaping::Update(  )
{
	m_LeftEyeCenter = g_TrackinkSystem->m_HeadPosition.EstimateLeftEyeCenter2D;
	m_RightEyeCenter =  g_TrackinkSystem->m_HeadPosition.EstimateRightEyeCenter2D;

	m_LeftPupilCenter =  g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenter;;
	m_RightPupilCenter =  g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter;;


	CvPoint2D32f LeftCenter;
	LeftCenter.x = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.x)/2;
	LeftCenter.y = (g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation.y)/2;

	CvPoint2D32f RightCenter;
	RightCenter.x = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.x + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.x)/2;
	RightCenter.y = (g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation.y + g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation.y)/2;


	m_LeftGazeOF.x =  LeftCenter.x - g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.x;
	m_LeftGazeOF.y =  LeftCenter.y - g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.y;
	m_RightGazeOF.x =  RightCenter.x - g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.x;
	m_RightGazeOF.y =	RightCenter.y - g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.y;

	double ScaleFactor = 1 / (g_TrackinkSystem->m_HeadPosition.m_fModelScale*  g_TrackinkSystem->m_HeadPosition.m_fModelScale);
	m_LeftGazeOF.x *=   ScaleFactor;
	m_LeftGazeOF.y *=   ScaleFactor;
	m_RightGazeOF.x*=  ScaleFactor;
	m_RightGazeOF.y *=   ScaleFactor;


	m_IterationCount++;

	CvPoint2D32f TmpGaze;
	if(g_TrackinkSystem->m_GazeDetector.m_LeftCenterDetected)
	{

		TmpGaze.x = m_LeftEyeCenter.x - m_LeftPupilCenter.x;
		TmpGaze.y = m_LeftEyeCenter.y - m_LeftPupilCenter.y;
		if ( TmpGaze.x < g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width / 10 &&
			TmpGaze.y < g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height /5  )
			m_LeftGaze = TmpGaze;
		//else
		//	cout<<"bad left gaze"<<endl;
	};
	//m_LeftGaze.y = -m_LeftEyeCenter.y + m_LeftPupilCenter.y;

	if(g_TrackinkSystem->m_GazeDetector.m_RightCenterDetected)
	{
		TmpGaze.x = m_RightEyeCenter.x - m_RightPupilCenter.x;
		TmpGaze.y = m_RightEyeCenter.y - m_RightPupilCenter.y;
		if ( TmpGaze.x < g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width / 10 &&
			TmpGaze.y < g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height / 5  )
			m_RightGaze = TmpGaze;
		/*		else
			cout<<"bad right gaze"<<endl;*/
	};


	m_HeadMove.x = HeadGraphCenter.x - m_HadeCenter.x;
	m_HeadMove.y = HeadGraphCenter.y - m_HadeCenter.y;


	PlotGraph();
	//Init();

};