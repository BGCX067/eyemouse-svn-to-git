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


		OldLeftGaze.x =  m_GazeGraph->width / 2.0;
		OldLeftGaze.y =  m_GazeGraph->height / 2.0;

		OldRightGaze = OldLeftGaze;
		CurrentGaze = OldLeftGaze;

		alfa = 0.5;
		beta = 0.5;
		m_vRightEye.clear();
		m_vLeftEye.clear();
		m_vLeftEyeCalibrated.clear();
	};

	CvPoint	Point;
	CvPoint	LeftGaze;

		CvPoint	LeftGazeNoKonpensate;


	CvPoint	RightGaze;
	CvPoint	HeadGaze;

	m_AvrLeftEyeCenter.x = 0;
	m_AvrLeftEyeCenter.y = 0;
	m_AvrRightEyeCenter.x = 0;
	m_AvrRightEyeCenter.y = 0;





	//{
	//	LeftGaze.x = m_GazeGraph->width/2 - ( m_AvrLeftEyeCenter.x - m_LeftGaze.x   ) * factor;
	//	LeftGaze.y = m_GazeGraph->height/2 +( m_AvrLeftEyeCenter.y - m_LeftGaze.y  ) * factor ;
	//};



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

		//cvCircle( m_GazeGraph,LeftGaze, 7, CV_RGB(222,222,0), -1, 8,0);



		LeftGaze =  m_Caliblator.GetMapedPointLeftEye( m_LeftGazeOF, cvSize(m_GazeGraph->width,m_GazeGraph->height) );
			
     //  cvCircle( m_GazeGraph,LeftGaze, 7, CV_RGB(222,0,222), -1, 8,0);


		LeftGaze.x -= m_GazeGraph->width* 0.5  - 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.x*(float)m_GazeGraph->width;
		LeftGaze.y -= m_GazeGraph->height *0.5 - 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.y*(float)m_GazeGraph->height;

		LeftGaze.x += m_GazeGraph->width* 0.5  - 	 m_Caliblator.m_HeadShift.x*(float)m_GazeGraph->width;
		LeftGaze.y += m_GazeGraph->height *0.5 - 	 m_Caliblator.m_HeadShift.y*(float)m_GazeGraph->height;


		LeftGaze.x += m_GazeGraph->width* 0.5  - 	 m_Caliblator.m_HeadShift.x*(float)m_GazeGraph->width;
		LeftGaze.y += m_GazeGraph->height *0.5 - 	 m_Caliblator.m_HeadShift.y*(float)m_GazeGraph->height;

	

		float	 d = g_TrackinkSystem->m_HeadPosition.m_InitEyesDistans * (  g_TrackinkSystem->m_Graber.m_iWith / 640.0);

	//Factor =   (0.03* (d )*( g_GUI.param1));

	//Factor = Factor/m_fModelScale;

		float factor =   1100000 * ((g_GUI.param3 + 2) / 4.0)   *(1/(d*d)) * 0.8;




		HeadGaze.x = 	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.x*(float)m_GazeGraph->width;
		HeadGaze.y =  	g_TrackinkSystem->m_HeadPosition.m_HeadGazeEstymation.y*(float)m_GazeGraph->height;



		CvPoint	OrginalLeftGaze;
		OrginalLeftGaze = 	HeadGaze;
		OrginalLeftGaze.x += factor* m_LeftGazeKonpensate.x ;
		OrginalLeftGaze.y -=  DispleyFactorY * factor* m_LeftGazeKonpensate.y /** 1.3*/;



		CvPoint	HomograpyLeftGaze;


	if ( !m_Caliblator.m_bLeftEyeAngleMapCoputed)
		{

		LeftGaze = 	HeadGaze;
		RightGaze = HeadGaze;


		RightGaze.x += factor* m_RightGazeOF.x ;
		RightGaze.y -= DispleyFactorY *factor* m_RightGazeOF.y /** 1.3*/;

		LeftGaze.x += factor* m_LeftGazeKonpensate.x ;
		LeftGaze.y -=  DispleyFactorY * factor* m_LeftGazeKonpensate.y /** 1.3*/;



		LeftGazeNoKonpensate  =HeadGaze;
		LeftGazeNoKonpensate.x += factor* m_LeftGazeOF.x ;
		LeftGazeNoKonpensate.y -= DispleyFactorY *factor* m_LeftGazeOF.y /** 1.3*/;
		

		}

	else
	{
		//LeftGaze = 	HeadGaze;
		CCalibrationMesure tmp;
		tmp.UpdateData();

		CvPoint2D32f t =g_TrackinkSystem->m_GazeMaping.m_LeftGazeOF ;

		HomograpyLeftGaze =   m_Caliblator.GetMapedPoint( tmp, cvSize(m_GazeGraph->width,m_GazeGraph->height) );

		LeftGaze = cvPointFrom32f( m_Caliblator.GetMapedLeft( t, cvSize(m_GazeGraph->width,m_GazeGraph->height) ));
			

		LeftGaze.x +=  - 0.5 * m_GazeGraph->width + HeadGaze.x;
		LeftGaze.y += - 0.5 * m_GazeGraph->height + HeadGaze.y;
		//LeftGaze.y -= factor* m_LeftGazeKonpensate.y /** 1.3*/;


		HomograpyLeftGaze.x +=  - 0.5 * m_GazeGraph->width + HeadGaze.x;
		HomograpyLeftGaze.y += - 0.5 * m_GazeGraph->height + HeadGaze.y;
		

		LeftGazeNoKonpensate = LeftGaze;

		CvPoint2D32f  V = PointSub(_m_LeftGazeKonpensate,cvPointTo32f( LeftGaze));

		_m_LeftGazeKonpensateV = PointAdd(PointMul(_m_LeftGazeKonpensateV,0.5) ,PointMul(V,0.5));


		double diff = PointDistans(_m_LeftGazeKonpensate,cvPointTo32f(LeftGaze));
		diff = sqrt(_m_LeftGazeKonpensateV.x * _m_LeftGazeKonpensateV.x + _m_LeftGazeKonpensateV.y * _m_LeftGazeKonpensateV.y);
		_m_LeftGazeKonpensateAlfa = ( 1-  diff*diff   /(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()*150));
		if (_m_LeftGazeKonpensateAlfa > 1)_m_LeftGazeKonpensateAlfa = 1;
		if (_m_LeftGazeKonpensateAlfa < 0)_m_LeftGazeKonpensateAlfa = 0;
		double beta = 1 - _m_LeftGazeKonpensateAlfa;

		_m_LeftGazeKonpensate.x = _m_LeftGazeKonpensate.x  * _m_LeftGazeKonpensateAlfa + LeftGaze.x * beta;
		_m_LeftGazeKonpensate.y = _m_LeftGazeKonpensate.y  * _m_LeftGazeKonpensateAlfa + LeftGaze.y * beta;

	LeftGaze = cvPointFrom32f(_m_LeftGazeKonpensate);


	};

		CheckBondary(LeftGaze);
		CheckBondary(RightGaze);


	
	
//--------------------cal points------------------------------------------------
		for(int i = 0; i< m_Caliblator.m_vUsedCalPoint.size(); i++)

		{
			CvPoint p;
			int color;
			p.x = m_Caliblator.m_vScreenPoints[i].x * m_GazeGraph->width;
			p.y = m_Caliblator.m_vScreenPoints[i].y * m_GazeGraph->height;
			

					color  =m_Caliblator.m_vUsedCalPoint[i] * 255 ;

		   cvCircle( m_GazeGraph,( p) , 2, CV_RGB(color,0,color), -1, 8,0);
		}


//--------------------cal points------------------------------------------------
		

	m_vHeadGaze.push_back( HeadGaze);
	if (m_vHeadGaze.size() >PlotGazePointCount )
		m_vHeadGaze.erase(m_vHeadGaze.begin());



	m_vLeftEyeCalibrated.push_back( LeftGaze);
	if (m_vLeftEyeCalibrated.size() >PlotGazePointCount )
		m_vLeftEyeCalibrated.erase(m_vLeftEyeCalibrated.begin());




		//	LeftGaze =  cvPointFrom32f( PointAdd(PointMul(OldLeftGaze,alfa),PointMul(cvPointTo32f(LeftGaze),beta)));
		//OldLeftGaze = cvPointTo32f(LeftGaze);

		//RightGaze =  cvPointFrom32f( PointAdd(PointMul(OldRightGaze,alfa),PointMul(cvPointTo32f(RightGaze),beta)));
		//OldRightGaze = cvPointTo32f(RightGaze);


		//CvPoint2D32f gaze ;

		//double diff;

		//gaze.x =( LeftGaze.x + RightGaze.x) / 2.0;
		//gaze.y =( LeftGaze.y + RightGaze.y) / 2.0;


		//gaze = cvPointTo32f(LeftGaze);
		//diff = PointDistans(CurrentGaze,gaze);

		//alfa = /*alfa * 0.5 + 0.5 **/( 1- 2 * diff  /(sqrt(float(g_TrackinkSystem->m_Graber.m_iWith  *  g_TrackinkSystem->m_Graber.m_iHeight))));
		//if (alfa > 1)alfa = 1;
		//if (alfa < 0)alfa = 0;
		//beta = 1 - alfa;

		//CurrentGaze.x = CurrentGaze.x  * alfa + gaze.x * beta;
		//CurrentGaze.y = CurrentGaze.y  * alfa + gaze.y * beta;
      /* cvCircle( m_GazeGraph,LeftGaze, 5, CV_RGB(222,0,0), -1, 8,0);
	   cvCircle( m_GazeGraph,RightGaze, 5, CV_RGB(0,222,0), -1, 8,0);*/

	   cvCircle( m_GazeGraph,( LeftGaze) , 12, CV_RGB(222,222,222), -1, 8,0);

	   // cvCircle( m_GazeGraph,( LeftGazeNoKonpensate) , 15, CV_RGB(0,100,100), -1, 8,0);

	   

	//   cvCircle( m_GazeGraph,( OrginalLeftGaze) , 5, CV_RGB(0,100,100), -1, 8,0);


	   	//   cvCircle( m_GazeGraph,( HomograpyLeftGaze) , 5, CV_RGB(100,100,0), -1, 8,0);
	   

  //cvCircle( m_GazeGraph,cvPointFrom32f( CurrentGaze) , 12, CV_RGB(222,222,222), -1, 8,0);

	   
	   
	m_vRightEye.push_back( (LeftGazeNoKonpensate));
	if (m_vRightEye.size() >PlotGazePointCount )
		m_vRightEye.erase(m_vRightEye.begin());



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
	list<CvPoint>::iterator it2;


	int	tmp = 0;
	//for( it = m_vLeftEye.begin(); it != m_vLeftEye.end(); it++)
	//{
	//	cvCircle( m_GazeGraph,(*it), 1, CV_RGB(50+ (tmp * 200)/m_vLeftEye.size(),0,0), -1, 8,0);
	//	tmp++;
	//};



	//tmp = 0;
	//for( it = m_vHeadGaze.begin(); it != m_vHeadGaze.end(); it++)
	//{
	//	cvCircle( m_GazeGraph,(*it), 1, CV_RGB(0,0,20+ (tmp * 230)/m_vHeadGaze.size()), -1, 8,0);
	//	tmp++;
	//};



	//tmp = 0;
	for( it = m_vRightEye.begin(); it != m_vRightEye.end(); it++)
	{
		cvCircle( m_GazeGraph,(*it), 1, CV_RGB(0,20+ (tmp * 230)/m_vLeftEye.size(),0), -1, 8,0);
		tmp++;
	};


	tmp = 0;
	for( it = m_vLeftEyeCalibrated.begin(); it != m_vLeftEyeCalibrated.end(); it++)
	{
		int color = 50+ (tmp * 200)/m_vLeftEyeCalibrated.size();
		cvCircle( m_GazeGraph,(*it), 2, CV_RGB(color,0,0), -1, 8,0);
		tmp++;
	};


//		tmp = 0;
//	for( it = m_vLeftEyeCalibrated.begin(); it != m_vLeftEyeCalibrated.end(); it++)
//	{
//it2 = it;
//it2++;
//if  (it2 == m_vLeftEyeCalibrated.end()) break;
//
//	int color = 20+ (tmp * 200)/m_vLeftEyeCalibrated.size();
//		cvLine(m_GazeGraph,(*it),(*it2), CV_RGB(color,0,0),1);
//		tmp++;
//	};
//


//----------------------- all calibration mix---------------------
	CCalibrationMesure mesure;
	mesure.UpdateData();
	CvPoint  Pt =   m_Caliblator.GetMapedPoint(mesure,  cvSize(m_GazeGraph->width,m_GazeGraph->height));
	//cvCircle( m_GazeGraph,Pt , 6, CV_RGB(0,222,222), -1, 8,0);






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




	CvPoint2D32f LeftCenter;
	CvPoint2D32f RightCenter;



	LeftCenter = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter;
	RightCenter =  g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter;

	m_LeftGazeOF.x =  LeftCenter.x - g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.x;
	m_LeftGazeOF.y =  LeftCenter.y - g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation.y;
	m_RightGazeOF.x =  RightCenter.x - g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.x;
	m_RightGazeOF.y =	RightCenter.y - g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation.y;

	double ScaleFactor = 1 / (g_TrackinkSystem->m_HeadPosition.m_fModelScale*  g_TrackinkSystem->m_HeadPosition.m_fModelScale);
	m_LeftGazeOF.x *=   ScaleFactor;
	m_LeftGazeOF.y *=   ScaleFactor;
	m_RightGazeOF.x*=  ScaleFactor;
	m_RightGazeOF.y *=   ScaleFactor;





	{
	CvPoint2D32f  V = PointSub(m_LeftGazeKonpensate,m_LeftGazeOF);

	m_LeftGazeKonpensateV = PointAdd(PointMul(m_LeftGazeKonpensateV,0.5) ,PointMul(V,0.5));


		double diff = PointDistans(m_LeftGazeKonpensate,m_LeftGazeOF);
		diff = sqrt(m_LeftGazeKonpensateV.x * m_LeftGazeKonpensateV.x + m_LeftGazeKonpensateV.y * m_LeftGazeKonpensateV.y);
		m_LeftGazeKonpensateAlfa = ( 1- 10* 2 * diff * diff  /(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 30.0));
		if (m_LeftGazeKonpensateAlfa > 1)m_LeftGazeKonpensateAlfa = 1;
		if (m_LeftGazeKonpensateAlfa < 0.1)m_LeftGazeKonpensateAlfa = 0.1;



		double beta = 1 - m_LeftGazeKonpensateAlfa;

		m_LeftGazeKonpensate.x = m_LeftGazeKonpensate.x  * m_LeftGazeKonpensateAlfa + m_LeftGazeOF.x * beta;
		m_LeftGazeKonpensate.y = m_LeftGazeKonpensate.y  * m_LeftGazeKonpensateAlfa + m_LeftGazeOF.y * beta;
	}


	
	{
	CvPoint2D32f  V = PointSub(m_RightGazeKonpensate,m_RightGazeOF);

	m_RightGazeKonpensateV = PointAdd(PointMul(m_RightGazeKonpensateV,0.5) ,PointMul(V,0.5));


		double diff = PointDistans(m_RightGazeKonpensate,m_RightGazeOF);
		diff = sqrt(m_RightGazeKonpensateV.x * m_RightGazeKonpensateV.x + m_RightGazeKonpensateV.y * m_RightGazeKonpensateV.y);
		m_RightGazeKonpensateAlfa = ( 1- 2 * diff * diff  /(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 30.0));
		if (m_RightGazeKonpensateAlfa > 1)m_RightGazeKonpensateAlfa = 1;
		if (m_RightGazeKonpensateAlfa < 0)m_RightGazeKonpensateAlfa = 0;
		double beta = 1 - m_RightGazeKonpensateAlfa;

		m_RightGazeKonpensate.x = m_RightGazeKonpensate.x  * m_RightGazeKonpensateAlfa + m_RightGazeOF.x * beta;
		m_RightGazeKonpensate.y = m_RightGazeKonpensate.y  * m_RightGazeKonpensateAlfa + m_RightGazeOF.y * beta;
	}




	m_IterationCount++;

	//CvPoint2D32f TmpGaze;
	//if(g_TrackinkSystem->m_GazeDetector.m_LeftCenterDetected)
	//{

	//	TmpGaze.x = m_LeftEyeCenter.x - m_LeftPupilCenter.x;
	//	TmpGaze.y = m_LeftEyeCenter.y - m_LeftPupilCenter.y;
	//	if ( TmpGaze.x < g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width / 10 &&
	//		TmpGaze.y < g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height /5  )
	//		m_LeftGaze = TmpGaze;
	//	//else
	//	//	cout<<"bad left gaze"<<endl;
	//};
	////m_LeftGaze.y = -m_LeftEyeCenter.y + m_LeftPupilCenter.y;

	//if(g_TrackinkSystem->m_GazeDetector.m_RightCenterDetected)
	//{
	//	TmpGaze.x = m_RightEyeCenter.x - m_RightPupilCenter.x;
	//	TmpGaze.y = m_RightEyeCenter.y - m_RightPupilCenter.y;
	//	if ( TmpGaze.x < g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width / 10 &&
	//		TmpGaze.y < g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height / 5  )
	//		m_RightGaze = TmpGaze;
	//	/*		else
	//		cout<<"bad right gaze"<<endl;*/
	//};


	//m_HeadMove.x = HeadGraphCenter.x - m_HadeCenter.x;
	//m_HeadMove.y = HeadGraphCenter.y - m_HadeCenter.y;


	PlotGraph();
	//Init();

};