#include "Calibration.h"
#include "CTrackinkSystem.h"

void CCalibrationMesure::UpdateData()
{
m_LeftGaze =g_TrackinkSystem->m_GazeMaping.m_LeftGazeOF ;
m_RightGaze = g_TrackinkSystem->m_GazeMaping.m_RightGazeOF;
//m_HeadPosition =g_TrackinkSystem->m_HeadPosition.ProjectTo2D( g_TrackinkSystem->m_HeadPosition.CurrentHeadCenter3D) ;


m_HeadPosition.x = g_TrackinkSystem->m_HeadPosition.m_CurrentHeadCenter3D.x;
m_HeadPosition.y = g_TrackinkSystem->m_HeadPosition.m_CurrentHeadCenter3D.y;
//
//m_HeadPosition.x *= 1/ g_TrackinkSystem->m_HeadPosition.m_fModelScale;
//m_HeadPosition.y *= 1/ g_TrackinkSystem->m_HeadPosition.m_fModelScale;
m_ScreenPoint =  g_TrackinkSystem->m_CalibrationPlot.GetCurrentPoint();
m_MesureType  = g_TrackinkSystem->m_CalibrationPlot.m_CalibrationType;
m_HeadPitch =g_TrackinkSystem->m_HeadPosition.m_fModelScale *  tan( g_TrackinkSystem->m_HeadPosition.m_HeadPitch);
m_HeadRoll  =g_TrackinkSystem->m_HeadPosition.m_fModelScale * tan( g_TrackinkSystem->m_HeadPosition.m_HeadRoll ) ;
m_HeadAngle = cvPoint2D32f( m_HeadPitch,m_HeadRoll);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//CCalibration::CCalibration(void)
//{
//}
//
//CCalibration::~CCalibration(void)
//{
//}

void CCalibration::RemoveOutLiners()
{


	vector<CCalibrationMesure*> vInLiniers;
	vector<CCalibrationMesure> vInLiniersTmp;


	vector<CCalibrationMesure>::iterator it;
	vector<CCalibrationMesure>::iterator it2;

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
	BeginCalibrationIterator = m_CalibrationMesurePoint.begin();
	int index = 0;


	for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); /*it++*/)
	{
		Count = 0;
		AvrGaze.x = 0;
		AvrGaze.y = 0;
		meanError = 0;

		vInLiniers.clear();
		BeginCalibrationIterator = it;
		int SkipFirtstPointCount = (int)((float)Count * SkipFirstPointCountFactor);
		SkipFirtstPointCount =   g_TrackinkSystem->m_CalibrationPlot.m_SampleCountPerPoint * 0.3;

		int tmp = 0;
		for(;(it!= m_CalibrationMesurePoint.end())&&
			((*it).m_ScreenPoint.x ==( *BeginCalibrationIterator).m_ScreenPoint.x )&&
			((*it).m_ScreenPoint.y ==( *BeginCalibrationIterator).m_ScreenPoint.y ); it++)
		{

			if ( (abs((*it).m_LeftGaze.x) < MaxGazeValue) && 
				(abs((*it).m_LeftGaze.y) < MaxGazeValue)&&
				tmp > SkipFirtstPointCount
				)
				vInLiniers.push_back(&(*it));
			tmp++;
		}
		/*			if (it ==  m_CalibrationMesurePoint.end())
		break*/;
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
				if ( vInLiniersTmp[j].m_LeftGaze.x > vInLiniersTmp[k].m_LeftGaze.x  )
				{
					Swap = vInLiniersTmp[j].m_LeftGaze.x;
					vInLiniersTmp[j].m_LeftGaze.x = vInLiniersTmp[k].m_LeftGaze.x;
					vInLiniersTmp[k].m_LeftGaze.x  = Swap;
				}	;	

				if ( vInLiniersTmp[j].m_LeftGaze.y > vInLiniersTmp[j].m_LeftGaze.y  )
				{
					Swap = vInLiniersTmp[j].m_LeftGaze.y;
					vInLiniersTmp[j].m_LeftGaze.y = vInLiniersTmp[k].m_LeftGaze.y;
					vInLiniersTmp[k].m_LeftGaze.y  = Swap;
				}	;	
			}

			AvrGaze.x = vInLiniersTmp[vInLiniersTmp.size() / 2].m_LeftGaze.x;
			AvrGaze.y = vInLiniersTmp[vInLiniersTmp.size() / 2].m_LeftGaze.y;
			//---------------------------------------------------------------------



			for (int j = 0 ; j < vInLiniers.size(); j++)
			{
				meanError += sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
					(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

			};


			for (int j = 0 ; j < vInLiniers.size(); j++)
			{
				meanError += sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
					(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y- vInLiniers[j]->m_LeftGaze.y));
			};
			meanError /=Count;



			double deviations = 0;
			float TmpErr;
			for (int j = 0 ; j < vInLiniers.size(); j++)
			{
				TmpErr =  sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x -  vInLiniers[j]->m_LeftGaze.x)+
					(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

				deviations += (TmpErr - meanError)*(TmpErr - meanError);
			};

			double sigma = sqrt(deviations/Count);

			double TresholdValue = 	 sqrt(5.99 * sigma*sigma);


			AvrGazeCorected.x = 0;
			AvrGazeCorected.y = 0;
			CorectedPointCount = 0;

			for (int j = 0 ; j < vInLiniers.size(); j++)
			{
				TmpErr =  sqrt((AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)*(AvrGaze.x - vInLiniers[j]->m_LeftGaze.x)+
					(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y)*(AvrGaze.y - vInLiniers[j]->m_LeftGaze.y));

				//if (TmpErr < TresholdValue  )
				if (TmpErr <3* meanError  )

				{
					AvrGazeCorected.x += vInLiniers[j]->m_LeftGaze.x;
					AvrGazeCorected.y += vInLiniers[j]->m_LeftGaze.y;
					CorectedPointCount++;
					vInLiniers[j]->m_bIsOutLiners = false;
				}	
				else
					vInLiniers[j]->m_bIsOutLiners = true;

			};

			cout<<"Corecte Point Count: "<<CorectedPointCount<<endl;
			AvrGazeCorected.x /= CorectedPointCount;
			AvrGazeCorected.y /= CorectedPointCount;


			//if (TresholdValue < MaxTresholdGaze)
			{
				m_vScreenPoints.push_back( (*BeginCalibrationIterator).m_ScreenPoint) ;
				m_vGazePoints.push_back( AvrGazeCorected) ;; 
			};
	}


}
//================================================================================================
void CCalibration::ComputeCalibration()
{


	switch(	g_pTrackinkSystem->m_CalibrationPlot.m_CalibrationType)
	{

case CalibrationMesureEyeAngle  :
	{

		m_bRightEyeAngleMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_RightGaze , &m_RightEyeAngleMap,0.3,"Right Eye Plot result");
		m_bLeftEyeAngleMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_LeftGaze , &m_LeftEyeAngleMap,0.3,"Left Eye Plot result");

		m_HeadShift  = g_pTrackinkSystem->m_HeadPosition.m_HeadGazeEstymation;
	
		break;
	}
case CalibrationMesureHeadAngle:
	{
		m_bHeadAngleMapCoputed = ComputeHomograpyMatrix(  &CCalibrationMesure::m_HeadAngle , &m_HeadAngleMap,0.4,"head angle Plot result");

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



				for (int j = 0 ; j < vInLiniers.size(); j++)
				{
					meanError += sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
						(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

				};


				for (int j = 0 ; j < vInLiniers.size(); j++)
				{
					meanError += sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
						(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y- (vInLiniers[j]->*ptrSrcPoint).y));
				};
				meanError /=Count;



				double deviations = 0;
				float TmpErr;
				for (int j = 0 ; j < vInLiniers.size(); j++)
				{
					TmpErr =  sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x -  (vInLiniers[j]->*ptrSrcPoint).x)+
						(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

					deviations += (TmpErr - meanError)*(TmpErr - meanError);
				};

				double sigma = sqrt(deviations/Count);

				double TresholdValue = 	 sqrt(5.99 * sigma*sigma);


				AvrGazeCorected.x = 0;
				AvrGazeCorected.y = 0;
				CorectedPointCount = 0;

				for (int j = 0 ; j < vInLiniers.size(); j++)
				{
					TmpErr =  sqrt((AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)*(AvrGaze.x - (vInLiniers[j]->*ptrSrcPoint).x)+
						(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y)*(AvrGaze.y - (vInLiniers[j]->*ptrSrcPoint).y));

					//if (TmpErr < TresholdValue  )
					if (TmpErr <3* meanError  )

					{
						AvrGazeCorected.x += (vInLiniers[j]->*ptrSrcPoint).x;
						AvrGazeCorected.y += (vInLiniers[j]->*ptrSrcPoint).y;
						CorectedPointCount++;
						vInLiniers[j]->m_bIsOutLiners = false;
					}	
					else
						vInLiniers[j]->m_bIsOutLiners = true;

				};

				//	cout<<"Corecte Point Count: "<<CorectedPointCount<<endl;
				AvrGazeCorected.x /= CorectedPointCount;
				AvrGazeCorected.y /= CorectedPointCount;

				//if (TresholdValue < MaxTresholdGaze)
				{
					vScreenPoints.push_back( (*BeginCalibrationIterator).m_ScreenPoint) ;
					vGazePoints.push_back( AvrGazeCorected) ;; 
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
				cvCircle( img,point, 1, CV_RGB(200,200,0), -1, 8,0);	
			else
				cvCircle( img,point, 1, CV_RGB(200,0,0), -1, 8,0);	
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

