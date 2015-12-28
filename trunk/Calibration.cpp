#include "Calibration.h"
#include "CTrackinkSystem.h"

void CCalibrationMesure::UpdateData()
{
m_LeftGaze =g_TrackinkSystem->m_GazeMaping.m_LeftGazeOF ;

//m_LeftGaze =g_TrackinkSystem->m_GazeMaping.m_LeftGazeKonpensate;
m_RightGaze = g_TrackinkSystem->m_GazeMaping.m_RightGazeOF;
//m_HeadPosition =g_TrackinkSystem->m_HeadPosition.ProjectTo2D( g_TrackinkSystem->m_HeadPosition.CurrentHeadCenter3D) ;


m_HeadPosition.x = g_TrackinkSystem->m_HeadPosition.m_CurrentHeadCenter3D.x;
m_HeadPosition.y = g_TrackinkSystem->m_HeadPosition.m_CurrentHeadCenter3D.y;
//
//m_HeadPosition.x *= 1/ g_TrackinkSystem->m_HeadPosition.m_fModelScale;
//m_HeadPosition.y *= 1/ g_TrackinkSystem->m_HeadPosition.m_fModelScale;
m_ScreenPoint =  g_TrackinkSystem->m_CalibrationPlot.GetCurrentPoint();
m_MesureType  = g_TrackinkSystem->m_CalibrationPlot.m_CalibrationType;
m_HeadPitch =  tan( g_TrackinkSystem->m_HeadPosition.m_HeadPitch) / g_TrackinkSystem->m_HeadPosition.m_InitEyesDistans;
m_HeadRoll  = tan( g_TrackinkSystem->m_HeadPosition.m_HeadRoll )/ g_TrackinkSystem->m_HeadPosition.m_fModelScale  ;
m_HeadAngle = cvPoint2D32f( m_HeadPitch,m_HeadRoll);

}

///////////////////////////////////////////////////////////////////////////////////////////////////



	CCalibrationPlot::CCalibrationPlot()
	{

		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.5,0.5));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.1,0.1));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.5,0.1));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.9,0.1));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.9,0.5));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.1,0.5));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.1,0.9));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.5,0.9));
		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.9,0.9));





		//m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.25,0.25));

		//m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.25,0.75));

		//m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.75,0.25));

		//m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.75,0.75));



	//m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.05,0.05));
	//	m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.05,0.95));

	//		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.95,0.05));
	//	m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.95,0.95));


		m_vCalibrationScreenPointsEyeAngle.push_back(cvPoint2D32f(0.9,0.9));












m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));





//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.1));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.9));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.5));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.9,0.5));

		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.1));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.9,0.1));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.9,0.9));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.9));

		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.1));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.5));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.9));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.9,0.5));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.5));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.5));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.1,0.9));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.5,0.9));
		//m_vCalibrationScreenPointsHeadAngle.push_back(cvPoint2D32f(0.9,0.9));


		m_vCalibrationScreenPointsHeadPosition.push_back(cvPoint2D32f(0.5,0.5));
		m_vCalibrationScreenPointsHeadPosition.push_back(cvPoint2D32f(0.3,0.3));
		m_vCalibrationScreenPointsHeadPosition.push_back(cvPoint2D32f(0.7,0.3));
		m_vCalibrationScreenPointsHeadPosition.push_back(cvPoint2D32f(0.3,0.7));
		m_vCalibrationScreenPointsHeadPosition.push_back(cvPoint2D32f(0.7,0.7));


		Init();
		m_pCalibrationScreen = cvCreateImage( cvSize(800,600), 8, 3);

	}

//============================================================================================
bool  CCalibrationPlot::PlotNextPoint()
	{
		if (m_CurrentPointIndex >= m_CalibrationScreenPointsCount-1)
			return false;
		CvPoint2D32f point;
		string CalibrationWindowName = "Calibration";
		cvNamedWindow( CalibrationWindowName.c_str(), 0 );

		cvZero(m_pCalibrationScreen);

		point  = GetCurrentPoint();
		point.x *= m_pCalibrationScreen->width;
		point.y *= m_pCalibrationScreen->height;
		cvCircle( m_pCalibrationScreen,cvPointFrom32f(point), 10,color, -1, 8,0);	
		cvShowImage(CalibrationWindowName.c_str(), m_pCalibrationScreen);

		if (m_SampleCountPerPoint == m_CurrentSampleNr)
		{
			m_CurrentSampleNr = 0;
			m_CurrentPointIndex++;
		}
		m_CurrentSampleNr++;

		return (m_CurrentPointIndex < m_CalibrationScreenPointsCount) ; // czy juz wszystkie zostaly narysowane

	}
//============================================================================================

	CvPoint2D32f  CCalibrationPlot::GetCurrentPoint()
	{
		CvPoint2D32f point = cvPoint2D32f(0,0);

		float StableFactor = 0.5;
	if (m_CurrentPointIndex >= m_CalibrationScreenPointsCount-1)
			return point;
		
		if (m_MoveBeetwenPoints)
		{

			double factor = (float)m_CurrentSampleNr/((float)m_SampleCountPerPoint*StableFactor);
			if (factor > 1) factor = 1.0;
			point.x = (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].x + 
				((*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex + 1].x 
				- (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex ].x) *(factor);
			point.y = (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].y 
				+ ((*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex + 1].y
				- (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex ].y)	*(factor);
		}
		else
		{
			point.x = (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].x ;
			point.y = (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].y ;

		};

		/*	point.x = (*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].x ;
			point.y =(*m_pCurrentCalibrationScreenPoints)[m_CurrentPointIndex].y ;*/
		return point;

	}


//============================================================================================
void CCalibrationPlot::Init(CalibrationMesureType CalibrationType)
	{
		m_CalibrationType = CalibrationType;
		m_MoveBeetwenPoints = false;

		m_SampleCountPerPoint = 20;
		m_CurrentPointIndex = 0;
		m_CurrentSampleNr = 0;
		m_CalibrationScreenPointsCount = 0;

		switch(m_CalibrationType)
		{

		case CalibrationMesureEyeAngle  :
			{
				m_MoveBeetwenPoints = false;
				m_SampleCountPerPoint = 20;
				m_CalibrationScreenPointsCount = m_vCalibrationScreenPointsEyeAngle.size();
				color = CV_RGB(255,0,0);
				m_pCurrentCalibrationScreenPoints =  &m_vCalibrationScreenPointsEyeAngle;
				break;
			}
		case CalibrationMesureHeadAngle:
			{
				m_MoveBeetwenPoints = true;
				m_SampleCountPerPoint = 50;
				m_CalibrationScreenPointsCount = m_vCalibrationScreenPointsHeadAngle.size();
				m_pCurrentCalibrationScreenPoints =  &m_vCalibrationScreenPointsHeadAngle;
				color = CV_RGB(0,222,0);
				break;
			}
		case CalibrationMesureHeadPosition:
			{
				m_MoveBeetwenPoints = true;
				m_SampleCountPerPoint = 50;
				m_pCurrentCalibrationScreenPoints =  &m_vCalibrationScreenPointsHeadPosition;
				m_CalibrationScreenPointsCount = m_vCalibrationScreenPointsHeadPosition.size();
				color = CV_RGB(0,0,222);
				break;
			}

		}

		//m_CalibrationScreenPointsCount =CalibrationScreenPointsCount;


	}

//CCalibration::~CCalibration(void)
//{
//}

void CCalibration::RemoveOutLiners()
{


	//vector<CCalibrationMesure*> vInLiniers;
	//vector<CCalibrationMesure> vInLiniersTmp;


	//vector<CCalibrationMesure>::iterator it;
	//vector<CCalibrationMesure>::iterator it2;

	//vector<CCalibrationMesure>::iterator BeginCalibrationIterator;
	//vector<CCalibrationMesure>::iterator EndCalibrationIterator;

	//CvPoint2D32f CurrentPointAvr;
	//CurrentPointAvr.x = 0;
	//CurrentPointAvr.y = 0;


	//CvPoint2D32f	MeanPoint; 

	//CvPoint2D32f	PointAvrCorrected; 

	//int CurrentPointMesureCount = 0;
	//float AvrDistansErr = 0;
	//double meanError = 0;

	//int PrevScreenPointIndex = 0;

	//CvPoint2D32f AvrGaze;
	//CvPoint2D32f AvrGazeCorected;
	//int Count;
	//int CorectedPointCount;
	//float Swap;
	//BeginCalibrationIterator = m_CalibrationMesurePoint.begin();
	//int index = 0;


	//for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); /*it++*/)
	//{
	//	Count = 0;
	//	AvrGaze.x = 0;
	//	AvrGaze.y = 0;
	//	meanError = 0;

	//	vInLiniers.clear();
	//	BeginCalibrationIterator = it;
	//	int SkipFirtstPointCount = (int)((float)Count * SkipFirstPointCountFactor);
	//	SkipFirtstPointCount =   g_TrackinkSystem->m_CalibrationPlot.m_SampleCountPerPoint * 0.3;

	//	int tmp = 0;
	//	for(;(it!= m_CalibrationMesurePoint.end())&&
	//		((*it).m_ScreenPoint.x ==( *BeginCalibrationIterator).m_ScreenPoint.x )&&
	//		((*it).m_ScreenPoint.y ==( *BeginCalibrationIterator).m_ScreenPoint.y ); it++)
	//	{

	//		if ( (abs((*it).m_LeftGaze.x) < MaxGazeValue) && 
	//			(abs((*it).m_LeftGaze.y) < MaxGazeValue)&&
	//			tmp > SkipFirtstPointCount
	//			)
	//			vInLiniers.push_back(&(*it));
	//		else
	//			(*it)->m_bIsOutLiners = true;

	//		tmp++;
	//	}
	//	/*			if (it ==  m_CalibrationMesurePoint.end())
	//	break*/;
	//	EndCalibrationIterator = it;

	//	if (vInLiniers.size()  < 3)
	//		continue;
	//	/*it--;*/

	//	Count = vInLiniers.size();



	//	/// ------------mediana------------------
	//	for (int j = 0 ; j < vInLiniers.size(); j++)
	//	{vInLiniersTmp.push_back(*vInLiniers[j]);};


	//	for (int j = 0 ; j < vInLiniersTmp.size(); j++)
	//		for (int k = j ; k < vInLiniersTmp.size(); k++)
	//		{
	//			if ( vInLiniersTmp[j].m_LeftGaze.x > vInLiniersTmp[k].m_LeftGaze.x  )
	//			{
	//				Swap = vInLiniersTmp[j].m_LeftGaze.x;
	//				vInLiniersTmp[j].m_LeftGaze.x = vInLiniersTmp[k].m_LeftGaze.x;
	//				vInLiniersTmp[k].m_LeftGaze.x  = Swap;
	//			}	;	

	//			if ( vInLiniersTmp[j].m_LeftGaze.y > vInLiniersTmp[j].m_LeftGaze.y  )
	//			{
	//				Swap = vInLiniersTmp[j].m_LeftGaze.y;
	//				vInLiniersTmp[j].m_LeftGaze.y = vInLiniersTmp[k].m_LeftGaze.y;
	//				vInLiniersTmp[k].m_LeftGaze.y  = Swap;
	//			}	;	
	//		}

	//		AvrGaze.x = vInLiniersTmp[vInLiniersTmp.size() / 2].m_LeftGaze.x;
	//		AvrGaze.y = vInLiniersTmp[vInLiniersTmp.size() / 2].m_LeftGaze.y;
	//		//---------------------------------------------------------------------



	//		for (int j = 0 ; j < vInLiniers.size(); j++)
	//		{
	//			meanError += sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
	//				(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

	//		};


	//		for (int j = 0 ; j < vInLiniers.size(); j++)
	//		{
	//			meanError += sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
	//				(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y- vInLiniers[j]->m_LeftGaze.y));
	//		};
	//		meanError /=Count;



	//		double deviations = 0;
	//		float TmpErr;
	//		for (int j = 0 ; j < vInLiniers.size(); j++)
	//		{
	//			TmpErr =  sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x -  vInLiniers[j]->m_LeftGaze.x)+
	//				(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

	//			deviations += (TmpErr - meanError)*(TmpErr - meanError);
	//		};

	//		double sigma = sqrt(deviations/Count);

	//		double TresholdValue = 	 sqrt(5.99 * sigma*sigma);


	//		AvrGazeCorected.x = 0;
	//		AvrGazeCorected.y = 0;
	//		CorectedPointCount = 0;

	//		for (int j = 0 ; j < vInLiniers.size(); j++)
	//		{
	//			TmpErr =  sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
	//				(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

	//			//if (TmpErr < TresholdValue  )
	//			if (TmpErr <3* meanError  )

	//			{
	//				AvrGazeCorected.x += vInLiniers[j]->m_LeftGaze.x;
	//				AvrGazeCorected.y += vInLiniers[j]->m_LeftGaze.y;
	//				CorectedPointCount++;
	//				vInLiniers[j]->m_bIsOutLiners = false;
	//			}	
	//			else
	//				vInLiniers[j]->m_bIsOutLiners = true;

	//		};

	//		cout<<"Corecte Point Count: "<<CorectedPointCount<<endl;
	//		AvrGazeCorected.x /= CorectedPointCount;
	//		AvrGazeCorected.y /= CorectedPointCount;


	//		//if (TresholdValue < MaxTresholdGaze)
	//		{
	//			m_vScreenPoints.push_back( (*BeginCalibrationIterator).m_ScreenPoint) ;
	//			m_vGazePoints.push_back( AvrGazeCorected) ;; 
	//		};
	//}


}
//================================================================================================
void CCalibration::ComputeCalibration()
{


	switch(	g_pTrackinkSystem->m_CalibrationPlot.m_CalibrationType)
	{

case CalibrationMesureEyeAngle  :
	{
ComputeCalibrationAll();
//m_bLasstSquareCoputed = true;
		m_bRightEyeAngleMapCoputed = _ComputeHomograpyMatrix(  &CCalibrationMesure::m_LeftGaze , &m_RightEyeAngleMap,0.3,"all left Eye Plot result");
		
		m_bLeftEyeAngleMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_LeftGaze , &m_LeftEyeAngleMap,0.3,"Left Eye Plot result");

		m_HeadShift  = g_pTrackinkSystem->m_HeadPosition.m_HeadGazeEstymation;
	
		break;
	}
case CalibrationMesureHeadAngle:
	{

		m_bLasstSquareCoputed = true;
		//ComputeCalibrationAll();
		ComputeCalibrationHeadAngle();
		//m_bHeadAngleMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_HeadAngle , &m_HeadAngleMap,0.4,"head angle Plot result");

		break;
	}
case CalibrationMesureHeadPosition:
	{
		m_bHeadPositionMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_HeadPosition , &m_HeadPositionMap,0.4,"head position Plot result");

		break;
	}
	};


	}




	//================================================================================================

	void CCalibration::PlotCalibrationResult()
	{
		float factor = 61;
		IplImage *img;
		vector<CCalibrationMesure>::iterator it;

		img = cvCreateImage( cvSize(800,600), 8, 3);
		cvZero(img);

		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			CvPoint point =  GetMapedPoint((*it).m_LeftGaze,(*it).m_RightGaze, cvSize(800,600));
			if ((*it).m_bIsOutLiners)
				cvCircle( img,point, 1, CV_RGB(200,200,0), -1, 8,0);	
			else
				cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	
		};


		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			CvPoint point;
			point.x = img->width/2 + (*it).m_LeftGaze.x   * factor;
			point.y = img->height/2 - (*it).m_LeftGaze.y   * factor ;
			cvCircle( img,point,1, CV_RGB(100,122,122), -1, 8,0);		
		};



		for(int i = 0; i< m_vGazePoints.size(); i++)
		{
			CvPoint2D32f t;
			CvPoint point =  GetMapedPoint(m_vGazePoints[i],t, cvSize(800,600));
			cvCircle( img,point, 6, CV_RGB(200,222,0), 3, 8,0);		
		};



		for(int i = 0; i< m_vGazePoints.size(); i++)
		{
			CvPoint point;
			point.x = img->width/2 + m_vGazePoints[i].x   * factor;
			point.y = img->height/2 - m_vGazePoints[i].y   * factor ;
			cvCircle( img,point, 6, CV_RGB(200,222,222), 2, 8,0);		
		};



		g_GUI.ShowImage(img, "Calibration REsult");

	}
	//================================================================================================

//================================================================================================

CvPoint  CCalibration::GetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize )
{
	CvPoint point = cvPoint(0,0);
	if (m_bLeftEyeAngleMapCoputed)
		 point =  cvPointFrom32f(GetMapedPoint(LeftGaze,m_LeftEyeAngleMapData, ScreenSize ));
	return point;
}

//================================================================================================

CvPoint2D32f  CCalibration::fGetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize )
{
	CvPoint2D32f point ;
	if (m_bLeftEyeAngleMapCoputed)
		 point =  (GetMapedPoint(LeftGaze,m_LeftEyeAngleMapData, ScreenSize ));
	return point;
}


//=========================================================================================
CvPoint  CCalibration::_GetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize )
{
	CvPoint result = cvPoint(0,0);
		//if (m_bLasstSquareCoputed)
		//{

		//result.x = (LeftGaze.x * LeftGx  + mesure.m_HeadPitch * LeftAx+  LeftSx)* ScreenSize.width;
		//result.y = (LeftGaze.y * LeftGy  + mesure.m_HeadRoll * LeftAy+LeftSy)* ScreenSize.height;
		//
		//};
return result;
}


//================================================================================================
	CvPoint  CCalibration::GetMapedPoint(CCalibrationMesure mesure, CvSize ScreenSize )
	{

	//	float m_HeadAngleMapData[9];
	//float m_HeadPositionMapData[9];
	//float m_LeftEyeAngleMapData[9];
	//float m_RightEyeAngleMapData[9];

	//bool m_bHeadAngleMapCoputed;
	//bool m_bHeadPositionMapCoputed;
	//bool m_bLeftEyeAngleMapCoputed;

	//bool m_bRightEyeAngleMapCoputed;
		CvPoint2D32f result = cvPoint2D32f(ScreenSize.width / 2.0,ScreenSize.height / 2.0 );
	CvPoint2D32f tmp;





		
		if (m_bLasstSquareCoputed &&m_bLeftEyeAngleMapCoputed )
		{

		result.x = (fGetMapedPointLeftEye(mesure.m_LeftGaze, cvSize(1,1) ).x + mesure.m_HeadPitch * LeftAx +   LeftSx)* ScreenSize.width;
		result.y = (fGetMapedPointLeftEye(mesure.m_LeftGaze, cvSize(1,1) ).y  + mesure.m_HeadRoll * LeftAy +   LeftSy)* ScreenSize.height;
		return cvPointFrom32f(result);
		};



			if (m_bLasstSquareCoputed)
		{

		result.x = (mesure.m_LeftGaze.x * LeftGx  + mesure.m_HeadPitch * LeftAx+  LeftSx)* ScreenSize.width;
		result.y = (mesure.m_LeftGaze.y * LeftGy  + mesure.m_HeadRoll * LeftAy+LeftSy)* ScreenSize.height;
		return cvPointFrom32f(result);
		};




		if (m_bLeftEyeAngleMapCoputed)
		{
			 tmp =  GetMapedPoint(mesure.m_LeftGaze,m_LeftEyeAngleMapData, ScreenSize );
			result.x -= ScreenSize.width / 2.0 - tmp.x;
			result.y -= ScreenSize.height / 2.0 - tmp.y;
		};


		if (m_bHeadAngleMapCoputed)
		{
			 tmp =  GetMapedPoint(mesure.m_HeadAngle,m_HeadAngleMapData, ScreenSize );
			result.x -= ScreenSize.width / 2.0 - tmp.x;
			result.y -= ScreenSize.height / 2.0 - tmp.y;
		};


		
		if (m_bHeadPositionMapCoputed)
		{
			 tmp =  GetMapedPoint(mesure.m_HeadPosition,m_HeadPositionMapData, ScreenSize );
			result.x -= ScreenSize.width / 2.0 - tmp.x;
			result.y -= ScreenSize.height / 2.0 - tmp.y;
		};


return cvPointFrom32f(result);


	}

//================================================================================================
	CvPoint  CCalibration::GetMapedPoint(CvPoint2D32f LeftGaze,CvPoint2D32f RightGaze, CvSize ScreenSize )
	{
		CvPoint result;
		/*
		float X = LeftGaze.x *m_Scale +  m_Shift.x;
		float Y = LeftGaze.y *m_Scale +  m_Shift.y;
		return cvPoint(cvRound(X * ScreenSize.width), cvRound(Y * ScreenSize.height));
		*/

		if (m_bHomograpyMatrixCoputed)
		{
			float *h = m_HomograpyMatrixData;
			float x = LeftGaze.x;
			float y = LeftGaze.y;

			float Z = 1./(h[6]*x + h[7]*y + h[8]);
			float X = (h[0]*x + h[1]*y + h[2])*Z;
			float Y = (h[3]*x + h[4]*y + h[5])*Z;
			if (X > 1)
				X = 1;
			if (X < 0)
				X = 0;

			if (Y > 1)
				Y = 1;
			if (Y < 0)
				Y = 0;

			return cvPoint(cvRound(X * ScreenSize.width), cvRound(Y * ScreenSize.height));

		}
		else
			return cvPoint(0,0);
	};
	//================================================================================================



	//================================================================================================
	CvPoint2D32f  CCalibration::GetMapedPoint(CvPoint2D32f SrcPoint,float * HomograpyMatrixData, CvSize ScreenSize )
	{
		CvPoint2D32f result;
		/*
		float X = LeftGaze.x *m_Scale +  m_Shift.x;
		float Y = LeftGaze.y *m_Scale +  m_Shift.y;
		return cvPoint(cvRound(X * ScreenSize.width), cvRound(Y * ScreenSize.height));
		*/

		//if (m_bHomograpyMatrixCoputed)
		{
			float *h = HomograpyMatrixData;
			float x = SrcPoint.x;
			float y = SrcPoint.y;

			float Z = 1./(h[6]*x + h[7]*y + h[8]);
			float X = (h[0]*x + h[1]*y + h[2])*Z;
			float Y = (h[3]*x + h[4]*y + h[5])*Z;
			//if (X > 1)
			//	X = 1;
			//if (X < 0)
			//	X = 0;

			//if (Y > 1)
			//	Y = 1;
			//if (Y < 0)
			//	Y = 0;

			return cvPoint2D32f((X * ScreenSize.width), (Y * ScreenSize.height));

		}
		//else
		//	return cvPoint(0,0);
	};
	//================================================================================================



	CvPoint2D32f  CCalibration::GetMapedLeft(CvPoint2D32f gaze, CvSize ScreenSize )
	{

	vector<int> vUsedPoint;

		vector<float> vUsedPointX;

		vector<float> vUsedPointY;

	int UsedPointCount =0;
	CvPoint2D32f swap;

	float d;

	UsedPointCount = 4;


		

			if (m_vUsedCalPoint.size() < 2)
			{
				m_vUsedCalPoint.clear();
			for(int i = 0; i< m_vGazePoints.size(); i++)
				m_vUsedCalPoint.push_back(0);
			
			};


				for(int i = 0; i< m_vGazePoints.size(); i++)
			{
				vUsedPoint.push_back(i);
				vUsedPointX.push_back(0);
				vUsedPointY.push_back(0);
				m_vUsedCalPoint[i] = 0;
					
			}
					

	/*
		for(int i = 0; i< m_vGazePoints.size(); i++)
			for(int j = 0; j< m_vGazePoints.size();j++)
			{
				d = PointDistans(gaze,m_vGazePoints[j]);
				if (d <PointDistans(gaze,m_vGazePoints[vUsedPoint[i]]) )
					
				{
					int s=vUsedPoint[i]; 
		vUsedPoint[i] = vUsedPoint[j];
		vUsedPoint[j] =s;
				}
			};


			
		for(int i = 0; i< m_vGazePoints.size(); i++)
			for(int j = 0; j< m_vGazePoints.size();j++)
			{
				d = abs(gaze.x - m_vGazePoints[j].x);
				float a = abs(gaze.x - m_vGazePoints[i].x);
				if (d <a  )
					
				{
		int s=vUsedPointX[i]; 
		vUsedPointX[i] = vUsedPointX[j];
		vUsedPointX[j] =s;
				}
			};


			for(int i = 0; i< m_vGazePoints.size(); i++)
			for(int j = 0; j< m_vGazePoints.size();j++)
			{
				d = abs(gaze.y - m_vGazePoints[j].y);
				float a = abs(gaze.y - m_vGazePoints[i].y);
				if (d <a  )
					
				{
		int s=vUsedPointY[i]; 
		vUsedPointY[i] = vUsedPointY[j];
		vUsedPointY[j] =s;
				}
			};*/


	//	
	//for(int i = 0; i< m_vGazePoints.size(); i++)
	//{
	//d = PointDistans(gaze,m_vGazePoints[i]);

	//if (d <PointDistans(gaze,m_vGazePoints[vUsedPoint[UsedPointCount]]) )
	//	vUsedPoint[UsedPointCount] = i;
	//};

	//UsedPointCount++;

	//for(int i = 0; i< m_vGazePoints.size(); i++)
	//{
	//	if ( i = vUsedPoint[0] ) continue;
	//d = PointDistans(gaze,m_vGazePoints[i]);

	//if (d <PointDistans(gaze,m_vGazePoints[vUsedPoint[UsedPointCount]]) )
	//	vUsedPoint[UsedPointCount] = i;
	//};




		CvPoint2D32f result;

		result.x = 0;
		result.y = 0;

		int XCount = 0;
		int YCount = 0;

		float XFactor = 0;
		float YFactor = 0;




//		
////------------------sdfsdf--------------------------------
//		
//		for(int i = 0; i<UsedPointCount; i++)
//			for(int j = 0; j< UsedPointCount;j++)
//		{
//
//if (m_vScreenPoints[vUsedPoint[i]].x !=  m_vScreenPoints[vUsedPoint[j]].x)
//{
//XCount++;
//float f = /*1000 **/PointDistans(gaze,m_vGazePoints[vUsedPoint[i]]) + PointDistans(gaze,m_vGazePoints[vUsedPoint[j]]) ;
////f = f * f * f * f;
//f = 1 / f;
//XFactor += f ;
//};
//
//
//if (m_vScreenPoints[vUsedPoint[i]].y !=  m_vScreenPoints[vUsedPoint[j]].y)
//{
//YCount++;
//float f = /*1000 **/PointDistans(gaze,m_vGazePoints[vUsedPoint[i]]) + PointDistans(gaze,m_vGazePoints[vUsedPoint[j]]) ;
////f = f * f * f * f;
//f = 1 / f;
//YFactor +=f ;
//};
//			};
//int XMaxCount = 4;
//		int xcount = 0;
//
//int YMaxCount = 4;
//		int ycount = 0;
//for(int i = 0; i< m_vGazePoints.size(); i++)
//			for(int j = 0; j< m_vGazePoints.size();j++)
//		{
//	if (xcount <XMaxCount )
//if ((m_vScreenPoints[vUsedPointX[i]].x !=  m_vScreenPoints[vUsedPointX[j]].x)&& (m_vScreenPoints[vUsedPointX[i]].y ==  m_vScreenPoints[vUsedPointX[j]].y))
//{
//	
//	xcount++;
//	result.x += (1 / (float)XMaxCount) *( m_vScreenPoints[vUsedPointX[i]].x + 
//				((gaze.x - m_vGazePoints[vUsedPointX[i]].x) /(m_vGazePoints[vUsedPointX[j]].x- m_vGazePoints[vUsedPointX[i]].x)    ) *
//				(m_vScreenPoints[vUsedPointX[j]].x -  m_vScreenPoints[vUsedPointX[i]].x ));
//
//
//
//};
//
//
//	if (ycount < YMaxCount )
//if ((m_vScreenPoints[vUsedPointY[i]].y !=  m_vScreenPoints[vUsedPointY[j]].y)&& (m_vScreenPoints[vUsedPointY[i]].x ==  m_vScreenPoints[vUsedPointY[j]].x))
//{
//		
//	ycount++;
//	float f = /*1000 **/PointDistans(gaze,m_vGazePoints[vUsedPointY[i]]) + PointDistans(gaze,m_vGazePoints[vUsedPointY[j]]) ;
//	//f = f * f * f * f;
//	f = 1 / f;
//	f = XFactor / YMaxCount;
//
//	result.y += (1 / (float)YMaxCount) *( m_vScreenPoints[vUsedPointY[i]].y + 
//				((gaze.y - m_vGazePoints[vUsedPointY[i]].y) /(m_vGazePoints[vUsedPointY[j]].y- m_vGazePoints[vUsedPointY[i]].y)    ) *
//				(m_vScreenPoints[vUsedPointY[j]].y -  m_vScreenPoints[vUsedPointY[i]].y ));
//
//
//
//};
//
//			}
//
//			return cvPoint2D32f((result.x* ScreenSize.width), (result.y * ScreenSize.height));
//
//
//
//for(int i = 0; i< m_vGazePoints.size(); i++)
//			for(int j = 0; j< m_vGazePoints.size();j++)
//		{
//		
//if (m_vScreenPoints[i].x !=  m_vScreenPoints[j].x)
//{
//	float f = /*1000 **/PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
//	//f = f * f * f * f;
//	f = 1 / f;
//	//f = XFactor / XCount;
//
//	result.x += (f / XFactor) *( m_vScreenPoints[i].x + 
//				((gaze.x - m_vGazePoints[i].x) /(m_vGazePoints[j].x- m_vGazePoints[i].x)    ) *
//				(m_vScreenPoints[j].x -  m_vScreenPoints[i].x ));
//
//};
//
//
//if (m_vScreenPoints[i].y !=  m_vScreenPoints[j].y)
//{
//	float f = /*1000 **/PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
//	//f = f * f * f * f;
//	f = 1 / f;
//	//f = YFactor / YCount;
//	result.y += (f / YFactor) *( m_vScreenPoints[i].y + 
//				((gaze.y - m_vGazePoints[i].y) /(m_vGazePoints[j].y- m_vGazePoints[i].y)   ) *
//				(m_vScreenPoints[j].y -  m_vScreenPoints[i].y ));
//
//};
//
//			}
//
//			return cvPoint2D32f((result.x* ScreenSize.width), (result.y * ScreenSize.height));
//



//------------------factors--------------------------------
		
float t = 1000;
		for(int i = 0; i< m_vGazePoints.size(); i++)
			for(int j = 0; j< m_vGazePoints.size();j++)
		{

if ((m_vScreenPoints[i].x !=  m_vScreenPoints[j].x)/*&&(m_vScreenPoints[i].y !=  m_vScreenPoints[j].y)*/)
{
XCount++;
float f = t *PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
	f = f * f    *f * f;
f = 1 / f;
XFactor += f ;
};


if ((m_vScreenPoints[i].y !=  m_vScreenPoints[j].y)/*&&(m_vScreenPoints[i].x !=  m_vScreenPoints[j].x)*/)
{
YCount++;
float f = t *PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
	f = f * f    *f * f;
f = 1 / f;
YFactor +=f ;
};
			};

	for(int i = 0; i< m_vGazePoints.size(); i++)
			for(int j = 0; j< m_vGazePoints.size();j++)
		{

if ((m_vScreenPoints[i].x !=  m_vScreenPoints[j].x)/*&&(m_vScreenPoints[i].y !=  m_vScreenPoints[j].y)*/)
{
	float f = t *PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
	f = f * f    *f * f;
	f = 1 / f;
	//f = XFactor / XCount;

	m_vUsedCalPoint[i] += f;
	m_vUsedCalPoint[j] += f;

	result.x += (f / XFactor) *( m_vScreenPoints[i].x + 
				((gaze.x - m_vGazePoints[i].x) /(m_vGazePoints[j].x- m_vGazePoints[i].x)    ) *
				(m_vScreenPoints[j].x -  m_vScreenPoints[i].x ));

};

if ((m_vScreenPoints[i].y !=  m_vScreenPoints[j].y)/*&&(m_vScreenPoints[i].x !=  m_vScreenPoints[j].x)*/)
{
	float f = t *PointDistans(gaze,m_vGazePoints[i]) + PointDistans(gaze,m_vGazePoints[j]) ;
	f = f * f    *f * f;
	f = 1 / f;
	//f = YFactor / YCount;
	result.y += (f / YFactor) *( m_vScreenPoints[i].y + 
				((gaze.y - m_vGazePoints[i].y) /(m_vGazePoints[j].y- m_vGazePoints[i].y)   ) *
				(m_vScreenPoints[j].y -  m_vScreenPoints[i].y ));


	m_vUsedCalPoint[i] += f;
	m_vUsedCalPoint[j] += f;

};

			}


		if (YFactor + XFactor != 0)
				for(int i = 0; i< m_vUsedCalPoint.size(); i++)
					m_vUsedCalPoint[i] /= YFactor + XFactor;

			return cvPoint2D32f((result.x* ScreenSize.width), (result.y * ScreenSize.height));


	};
	//================================================================================================


	bool CCalibration::ComputeHomograpyMatrix(CvPoint2D32f  CCalibrationMesure::*ptrSrcPoint /*CalibrationMesureType MesureType*/, CvMat * MapMatrix, float SkipFirstMeserFactor, char * WinName)
	{
		vector<CvPoint2D32f> vScreenPoints;
		vector<CvPoint2D32f> vGazePoints;

		vector<CCalibrationMesure*> vInLiniers;
		vector<CCalibrationMesure> vInLiniersTmp;

		vector<CCalibrationMesure>::iterator it;
		//vector<CCalibrationMesure>::iterator it2;

		vector<CCalibrationMesure>::iterator BeginCalibrationIterator;
		vector<CCalibrationMesure>::iterator EndCalibrationIterator;

		CvPoint2D32f CurrentPointAvr;
		CurrentPointAvr.x = 0;
		CurrentPointAvr.y = 0;


		CvPoint2D32f	MeanPoint; 


		CvPoint2D32f	PointAvrCorrected; 
		int CurrentPointMesureCount = 0;
		float AvrDistansErr = 0;
		double meanError = 0;

		int PrevScreenPointIndex = 0;

		CvPoint2D32f AvrGaze;
		CvPoint2D32f AvrGazeCorected;
		int Count;
		int CorectedPointCount;
		float Swap;
		//BeginCalibrationIterator = NULL;
		int index = 0;


		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); /*it++*/)
		{
			Count = 0;
			AvrGaze.x = 0;
			AvrGaze.y = 0;
			meanError = 0;
			vInLiniers.clear();
			vInLiniersTmp.clear();

			//if ((*it).m_MesureType ==MesureType )
			BeginCalibrationIterator = it;

			int tmp = 0;
			for(;(it!= m_CalibrationMesurePoint.end())&&
				//((*it).m_MesureType ==MesureType ) &&
				((*it).m_ScreenPoint.x ==( *BeginCalibrationIterator).m_ScreenPoint.x )&&
				((*it).m_ScreenPoint.y ==( *BeginCalibrationIterator).m_ScreenPoint.y ); it++)
			{
				vInLiniersTmp.push_back((*it));
				tmp++;
			};

			for (int k = 0 ; k < vInLiniersTmp.size(); k++)
				if (k > SkipFirstMeserFactor * vInLiniersTmp.size())
					vInLiniers.push_back(&vInLiniersTmp[k]);
				else
					vInLiniersTmp[k].m_bIsOutLiners = true;

			vInLiniersTmp.clear();

			EndCalibrationIterator = it;

			if (vInLiniers.size()  < 3)
				continue;
			/*it--;*/

			Count = vInLiniers.size();


			/// ------------mediana------------------
			for (int j = 0 ; j < vInLiniers.size(); j++)
			{vInLiniersTmp.push_back(*vInLiniers[j]);};


			for (int j = 0 ; j < vInLiniersTmp.size(); j++)
				for (int k = j ; k < vInLiniersTmp.size(); k++)
				{
					if ( (vInLiniersTmp[j].*ptrSrcPoint).x > (vInLiniersTmp[k].*ptrSrcPoint).x  )
					{
						Swap = (vInLiniersTmp[j].*ptrSrcPoint).x;
						(vInLiniersTmp[j].*ptrSrcPoint).x = (vInLiniersTmp[k].*ptrSrcPoint).x;
						(vInLiniersTmp[k].*ptrSrcPoint).x  = Swap;
					}	;	

					if ( (vInLiniersTmp[j].*ptrSrcPoint).y > (vInLiniersTmp[j].*ptrSrcPoint).y  )
					{
						Swap = (vInLiniersTmp[j].*ptrSrcPoint).y;
						(vInLiniersTmp[j].*ptrSrcPoint).y = (vInLiniersTmp[k].*ptrSrcPoint).y;
						(vInLiniersTmp[k].*ptrSrcPoint).y  = Swap;
					}	;	
				}

				AvrGaze.x = (vInLiniersTmp[vInLiniersTmp.size() / 2].*ptrSrcPoint).x;
				AvrGaze.y = (vInLiniersTmp[vInLiniersTmp.size() / 2].*ptrSrcPoint).y;
				//---------------------------------------------------------------------



				//for (int j = 0 ; j < vInLiniers.size(); j++)
				//{
				//	meanError += sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
				//		(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

				//};


				//for (int j = 0 ; j < vInLiniers.size(); j++)
				//{
				//	meanError += sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
				//		(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y- (vInLiniers[j]->*ptrSrcPoint).y));
				//};
				//meanError /=Count;



				//double deviations = 0;
				//float TmpErr;
				//for (int j = 0 ; j < vInLiniers.size(); j++)
				//{
				//	TmpErr =  sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x -  (vInLiniers[j]->*ptrSrcPoint).x)+
				//		(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

				//	deviations += (TmpErr - meanError)*(TmpErr - meanError);
				//};

				//double sigma = sqrt(deviations/Count);

				//double TresholdValue = 	 sqrt(5.99 * sigma*sigma);


				//AvrGazeCorected.x = 0;
				//AvrGazeCorected.y = 0;
				//CorectedPointCount = 0;

				//for (int j = 0 ; j < vInLiniers.size(); j++)
				//{
				//	TmpErr =  sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
				//		(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

				//	//if (TmpErr < TresholdValue  )
				//	if (TmpErr <3* meanError  )

				//	{
				//		AvrGazeCorected.x += (vInLiniers[j]->*ptrSrcPoint).x;
				//		AvrGazeCorected.y += (vInLiniers[j]->*ptrSrcPoint).y;
				//		CorectedPointCount++;
				//		vInLiniers[j]->m_bIsOutLiners = false;
				//	}	
				//	else
				//		vInLiniers[j]->m_bIsOutLiners = true;

				//};

				////	cout<<"Corecte Point Count: "<<CorectedPointCount<<endl;
				//AvrGazeCorected.x /= CorectedPointCount;
				//AvrGazeCorected.y /= CorectedPointCount;

				AvrGazeCorected = AvrGaze;
		


				//if (TresholdValue < MaxTresholdGaze)
				{
					m_vScreenPoints.push_back( (*BeginCalibrationIterator).m_ScreenPoint) ;
					m_vGazePoints.push_back( AvrGazeCorected) ;
				};
		}

		CvMat ScreenPointsMat = cvMat(1,m_vScreenPoints.size(),CV_32FC2,&m_vScreenPoints[0]);;
		CvMat GazePointsMat = cvMat(1,m_vGazePoints.size(),CV_32FC2,&m_vGazePoints[0]);
		bool bHomograpyMatrixCoputed = cvFindHomography(&GazePointsMat, &ScreenPointsMat,MapMatrix);


		//--------------------------plot cal result---------------------------------------------------

		float factor = 61;
		IplImage *img;


		img = cvCreateImage( cvSize(800,600), 8, 3);
		cvZero(img);

		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{


			CvPoint point = cvPointFrom32f( GetMapedPoint((*it).*ptrSrcPoint,(float *) MapMatrix->data.fl, cvSize(800,600) ));
			if ((*it).m_bIsOutLiners)
				cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	
			else
				cvCircle( img,point, 1, CV_RGB(200,222,0), -1, 8,0);	


				point = cvPointFrom32f( GetMapedLeft((*it).*ptrSrcPoint, cvSize(800,600)));
			cvCircle( img,point, 1, CV_RGB(0,0,255), -1, 8,0);
		};


		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			CvPoint point;


			point.x = img->width/2 + ((*it).*ptrSrcPoint).x   * factor;
			point.y = img->height/2 - ((*it).*ptrSrcPoint).y   * factor ;
			cvCircle( img,point,1, CV_RGB(100,122,122), -1, 8,0);		
		};



		for(int i = 0; i< m_vGazePoints.size(); i++)
		{
			CvPoint2D32f t;
			CvPoint point = cvPointFrom32f( GetMapedPoint(m_vGazePoints[i],(float *) MapMatrix->data.fl, cvSize(800,600)));

			
			cvCircle( img,point, 6, CV_RGB(200,222,0), 3, 8,0);	

			
		};



		for(int i = 0; i< m_vGazePoints.size(); i++)
		{
			CvPoint point;
			point.x = img->width/2 + m_vGazePoints[i].x   * factor;
			point.y = img->height/2 - m_vGazePoints[i].y   * factor ;
			cvCircle( img,point, 6, CV_RGB(200,222,222), 2, 8,0);		
		};


		//----------interpolate cal result--------------------


		for(int i = 0; i< m_vGazePoints.size(); i++)
		{
			CvPoint2D32f t;
			CvPoint point = cvPointFrom32f( GetMapedLeft(m_vGazePoints[i], cvSize(800,600)));
			cvCircle( img,point, 10, CV_RGB(0,222,222), 3, 8,0);		
		};


		
		for(int i = 0; i< m_vScreenPoints.size(); i++)
		{
			CvPoint2D32f t;
			CvPoint point ;
			point.x = m_vScreenPoints[i].x * 800;
			point.y = m_vScreenPoints[i].y * 600;
			
			cvCircle( img,point, 4, CV_RGB(0,222,0), -1, 8,0);		
		};

//string a =  "Calibration REsult" ;
//a += (char*)(MapMatrix->data.ptr);
g_GUI.ShowImage(img,WinName);
		//---------------------------------------------------
		return bHomograpyMatrixCoputed;

	}
	
	//================================================================================================

	bool CCalibration::_ComputeHomograpyMatrix(CvPoint2D32f  CCalibrationMesure::*ptrSrcPoint /*CalibrationMesureType MesureType*/, CvMat * MapMatrix, float SkipFirstMeserFactor, char * WinName)
	{
		vector<CvPoint2D32f> vScreenPoints;
		vector<CvPoint2D32f> vGazePoints;


		vector<CCalibrationMesure>::iterator it;


		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			
				{
					vScreenPoints.push_back( (*it).m_ScreenPoint) ;
					vGazePoints.push_back( ((*it).*ptrSrcPoint)) ;; 
				};
		}

		CvMat ScreenPointsMat = cvMat(1,vScreenPoints.size(),CV_32FC2,&vScreenPoints[0]);;
		CvMat GazePointsMat = cvMat(1,vGazePoints.size(),CV_32FC2,&vGazePoints[0]);
		bool bHomograpyMatrixCoputed = cvFindHomography(&GazePointsMat, &ScreenPointsMat,MapMatrix);


		//--------------------------plot cal result---------------------------------------------------

		float factor = 61;
		IplImage *img;


		img = cvCreateImage( cvSize(800,600), 8, 3);
		cvZero(img);

		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			CvPoint point = cvPointFrom32f( GetMapedPoint((*it).*ptrSrcPoint,(float *) MapMatrix->data.fl, cvSize(800,600) ));
			if ((*it).m_bIsOutLiners)
				cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	
			else
				cvCircle( img,point, 1, CV_RGB(200,222,0), -1, 8,0);	
		};


		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			CvPoint point;


			point.x = img->width/2 + ((*it).*ptrSrcPoint).x   * factor;
			point.y = img->height/2 - ((*it).*ptrSrcPoint).y   * factor ;
			cvCircle( img,point,1, CV_RGB(100,122,122), -1, 8,0);		
		};



		for(int i = 0; i< vGazePoints.size(); i++)
		{
			CvPoint2D32f t;
			CvPoint point = cvPointFrom32f( GetMapedPoint(vGazePoints[i],(float *) MapMatrix->data.fl, cvSize(800,600)));
			cvCircle( img,point, 6, CV_RGB(200,222,0), 3, 8,0);		
		};



		for(int i = 0; i< vGazePoints.size(); i++)
		{
			CvPoint point;
			point.x = img->width/2 + vGazePoints[i].x   * factor;
			point.y = img->height/2 - vGazePoints[i].y   * factor ;
			cvCircle( img,point, 6, CV_RGB(200,222,222), 2, 8,0);		
		};


//string a =  "Calibration REsult" ;
//a += (char*)(MapMatrix->data.ptr);
g_GUI.ShowImage(img,WinName);
		//---------------------------------------------------
		return bHomograpyMatrixCoputed;

	}

	
	//=============================================================================================================
	bool CCalibration::ComputeCalibrationAll()
	{

		vector<CCalibrationMesure>::iterator it;

		int index1 = 0;
			int index2 = 0;
	const int MaxPointCount = 10000;




	//CvPoint2D32f src1[MaxPointCount]; // A
	//CvPoint2D32f src2[MaxPointCount];// B
	//CvPoint2D32f CalFactor[2];//X

	float src1[MaxPointCount]; // A
	float src2[MaxPointCount];// B
	float CalFactor[3];//X

	int MeserCount = m_CalibrationMesurePoint.size();


	CvMat MatSrc1 =cvMat(MeserCount,3,CV_32FC1,&src1[0]);
	CvMat MatSrc2 =cvMat(MeserCount,1,CV_32FC1,&src2[0]);
	CvMat MatCalFactor=cvMat(3,1,CV_32FC1,&CalFactor[0]);
	//CvMat MatSrc1 =cvMat(MeserCount,2,CV_32FC1,&src1[0]);
	//CvMat MatSrc2 =cvMat(MeserCount,1,CV_32FC1,&src2[0]);
	//CvMat MatCalFactor=cvMat(2,1,CV_32FC1,&CalFactor[0]);


//double LeftGx, LeftGy;
//double LeftAx, LeftAy;

//double lGx, mGx;
//double lAx, mAx;
//double lGy, mGy;
//double lAy, mAy;
//lGx = 0;
//mGx = 0;
//lAx = 0;
//mAx = 0;
//------------------------left x------------------------------------------------------------
index1 = 0;
index2 = 0;
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end();it++)
		{
			src1[index1++] = (*it).m_LeftGaze.x;
			src1[index1++]= (*it).m_HeadPitch;
			src1[index1++]= 1.0;
			src2[index2++]= (*it).m_ScreenPoint.x;
		}
		int r =  cvSolve(&MatSrc1,&MatSrc2,&MatCalFactor);
 LeftGx = CalFactor[0];
LeftAx= CalFactor[1];
LeftSx = CalFactor[2];

//------------------------left y------------------------------------------------------------
index1 = 0;
index2 = 0;
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			src1[index1++] = (*it).m_LeftGaze.y;
			src1[index1++]= (*it).m_HeadRoll;
			src1[index1++]= 1.0;
			src2[index2++]= (*it).m_ScreenPoint.y;
		}
		 r =  cvSolve(&MatSrc1,&MatSrc2,&MatCalFactor);
 LeftGy = CalFactor[0];
LeftAy= CalFactor[1];

LeftSy= CalFactor[2];

//return true;

//--------------------------plot cal result---------------------------------------------------

		float factor = 61;
		IplImage *img;


		//img = cvCreateImage( cvSize(800,600), 8, 3);
		//cvZero(img);
		//CvPoint point ;
		//CvPoint point2 ;
		//for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		//{


		//point.x = ((*it).m_LeftGaze.x * LeftGx+  LeftSx)* img->width;
		//point.y = ((*it).m_LeftGaze.y * LeftGy +LeftSy) * img->height;;
		//cvCircle( img,point, 1, CV_RGB(200,200,222), -1, 8,0);	


		//point.x = ((*it).m_LeftGaze.x * LeftGx  + (*it).m_HeadPitch * LeftAx+  LeftSx)* img->width;
		//point.y = ((*it).m_LeftGaze.y * LeftGy  + (*it).m_HeadRoll * LeftAy+LeftSy)* img->height;
		//cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	



		//point.x = (  (*it).m_HeadPitch * LeftAx+  LeftSx)* img->width;
		//point.y = ( (*it).m_HeadRoll * LeftAy+LeftSy)* img->height;
		//cvCircle( img,point, 1, CV_RGB(0,222,0), -1, 8,0);	


	/*		
		point.x = ((*it).m_LeftGaze.x * LeftGx)* img->width + img->width* 0.5;
		point.y = ((*it).m_LeftGaze.y * LeftGy ) * img->height+ img->height* 0.5;
		cvCircle( img,point, 1, CV_RGB(200,200,222), -1, 8,0);	


		point.x = ((*it).m_LeftGaze.x * LeftGx  + (*it).m_HeadPitch * LeftAx)* img->width + img->width* 0.5;;
		point.y = ((*it).m_LeftGaze.y * LeftGy  + (*it).m_HeadRoll * LeftAy)* img->height;
		cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	



		point.x = (  (*it).m_HeadPitch * LeftAx)* img->width + img->width* 0.5;;
		point.y = ( (*it).m_HeadRoll * LeftAy)* img->height+ img->height* 0.5;;
		cvCircle( img,point, 1, CV_RGB(0,222,0), -1, 8,0);	
		*/
		
	//	};


		cout<<"LeftGx:"<<LeftGx<<endl;
		cout<<"LeftGy:"<<LeftGy<<endl;
		cout<<"LeftAx:"<<LeftAx<<endl;
		cout<<"LeftAy:"<<LeftAy<<endl;

		cout<<"LeftSx:"<<LeftSx<<endl;
		cout<<"LeftSy:"<<LeftSy<<endl;
//double LeftGx, LeftGy;
//double LeftAx, LeftAy;


//string a =  "Calibration REsult" ;
//a += (char*)(MapMatrix->data.ptr);
//g_GUI.ShowImage(img,"ccc");
		//---------------------------------------------------
return true;

	}
//=============================================================================================================
	bool CCalibration::ComputeCalibrationHeadAngle()
	{

		vector<CCalibrationMesure>::iterator it;

		int index1 = 0;
			int index2 = 0;
	const int MaxPointCount = 10000;


	float src1[MaxPointCount]; // A
	float src2[MaxPointCount];// B
	float CalFactor[3];//X

	int MeserCount = m_CalibrationMesurePoint.size();

//
//CvPoint  CCalibration::GetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize )
//{
//	CvPoint point = cvPoint(0,0);
//	if (m_bLeftEyeAngleMapCoputed)
//		 point =  cvPointFrom32f(GetMapedPoint(LeftGaze,m_LeftEyeAngleMapData, ScreenSize ));
//	return point;
//}
int n = 2;

	CvMat MatSrc1 =cvMat(MeserCount,n ,CV_32FC1,&src1[0]);
	CvMat MatSrc2 =cvMat(MeserCount,1,CV_32FC1,&src2[0]);
	CvMat MatCalFactor=cvMat(n ,1,CV_32FC1,&CalFactor[0]);
	//CvMat MatSrc1 =cvMat(MeserCount,2,CV_32FC1,&src1[0]);
	//CvMat MatSrc2 =cvMat(MeserCount,1,CV_32FC1,&src2[0]);
	//CvMat MatCalFactor=cvMat(2,1,CV_32FC1,&CalFactor[0]);


//double LeftGx, LeftGy;
//double LeftAx, LeftAy;

//double lGx, mGx;
//double lAx, mAx;
//double lGy, mGy;
//double lAy, mAy;
//lGx = 0;
//mGx = 0;
//lAx = 0;
//mAx = 0;
//------------------------left x------------------------------------------------------------
index1 = 0;
index2 = 0;
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end();it++)
		{
//if ((*it).m_MesureType != CalibrationMesureHeadAngle) continue;
			
			//src1[index1++] = 0;
			src1[index1++]= (*it).m_HeadPitch;
			src1[index1++]= 1.0;
			src2[index2++]= (*it).m_ScreenPoint.x -fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).x ;
		}
		int r =  cvSolve(&MatSrc1,&MatSrc2,&MatCalFactor);
 LeftGx = 0;
LeftAx= CalFactor[0];
LeftSx = CalFactor[1];
//------------------------left y------------------------------------------------------------
index1 = 0;
index2 = 0;
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			//if ((*it).m_MesureType != CalibrationMesureHeadAngle) continue;
			//src1[index1++] =0;
			src1[index1++]= (*it).m_HeadRoll;
			src1[index1++]= 1.0;
			src2[index2++]= (*it).m_ScreenPoint.y -  fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).y;;
		}
		 r =  cvSolve(&MatSrc1,&MatSrc2,&MatCalFactor);
 LeftGy =0;
LeftAy= CalFactor[0];

LeftSy= CalFactor[1];;



//--------------------------plot cal result---------------------------------------------------

		float factor = 61;
		IplImage *img;


		img = cvCreateImage( cvSize(800,600), 8, 3);
		cvZero(img);
		CvPoint point ;
		CvPoint point2 ;
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			if ((*it).m_MesureType != CalibrationMesureHeadAngle)
			{
			
		point.x = (fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).x + (*it).m_HeadPitch * LeftAx +   LeftSx)* img->width;
		point.y = (fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).y  + (*it).m_HeadRoll * LeftAy +   LeftSy)* img->height;
		cvCircle( img,point, 1, CV_RGB(0,222,0), -1, 8,0);	
			}

		point.x = (fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).x + (*it).m_HeadPitch * LeftAx +   LeftSx)* img->width;
		point.y = (fGetMapedPointLeftEye((*it).m_LeftGaze, cvSize(1,1) ).y  + (*it).m_HeadRoll * LeftAy +   LeftSy)* img->height;
		cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	

		};


		cout<<"LeftGx:"<<LeftGx<<endl;
		cout<<"LeftGy:"<<LeftGy<<endl;
		cout<<"LeftAx:"<<LeftAx<<endl;
		cout<<"LeftAy:"<<LeftAy<<endl;

		cout<<"LeftSx:"<<LeftSx<<endl;
		cout<<"LeftSy:"<<LeftSy<<endl;
//double LeftGx, LeftGy;
//double LeftAx, LeftAy;


//string a =  "Calibration REsult" ;
//a += (char*)(MapMatrix->data.ptr);
g_GUI.ShowImage(img,"calall");

	// CCalibration::ComputeCalibrationAll();
		//---------------------------------------------------
return true;

	}
//=============================================================================================================
