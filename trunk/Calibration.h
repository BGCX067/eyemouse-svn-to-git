#pragma once
#include "Utilitis.h"


const int CalibrationScreenPointsCount = 7;
//const CvPoint2D32f g_CalibrationScreenPoints[] = {  {0.5,0.5}, 
//{0.1,0.1},{0.5,0.1},  {0.9,0.1},
//{0.9,0.5},{0.5,0.5},{0.1,0.5} ,
//{0.1,0.9},{0.5,0.9},  {0.9,0.9}};

const CvPoint2D32f g_CalibrationScreenPoints[] = {  {0.5,0.5}, 
{0.1,0.1},  {0.9,0.1},
{0.9,0.5},{0.1,0.5},
{0.1,0.9},  {0.9,0.9}};
//const CvPoint2D32f g_CalibrationScreenPoints[] = {  
//	{0.1,0.1},  {0.9,0.1},
//	{0.1,0.9},  {0.9,0.9}};

//
enum CalibrationMesureType {  CalibrationMesureEyeAngle,  CalibrationMesureHeadAngle, CalibrationMesureHeadPosition};


const float MaxGazeValue = 10;
const float MaxTresholdGaze= 10;


class CCalibrationPlot
{
public:

	CvScalar color;
	int m_CalibrationScreenPointsCount;
	int m_CurrentPointIndex;
	int m_SampleCountPerPoint;
	int m_CurrentSampleNr;
	bool m_MoveBeetwenPoints;

	vector<CvPoint2D32f> m_vCalibrationScreenPointsEyeAngle;
	vector<CvPoint2D32f> m_vCalibrationScreenPointsHeadAngle;
	vector<CvPoint2D32f> m_vCalibrationScreenPointsHeadPosition;

	vector<CvPoint2D32f>* m_pCurrentCalibrationScreenPoints;


	//float MaxTresholdGazeValue;

	CalibrationMesureType m_CalibrationType; // aktualny rodzaj kalibracji


	IplImage *m_pCalibrationScreen;

	CCalibrationPlot();
	//=========================================================================

	CvPoint2D32f GetCurrentPoint();
	//=========================================================================


	void Init(CalibrationMesureType CalibrationType = CalibrationMesureEyeAngle);
	//=========================================================================

	bool PlotNextPoint();
};/// ///////////////////////////////////////////////////////////////////////////////////////

//=====================================================================




/////////////////////////////////////////////////////////////
struct CCalibrationMesure
{
	//CvPoint m_ScreenPoint; // pozycja punktu kalibracyjnego (wyswietlany na monitorze)
	CvPoint2D32f m_StartHeadPosition; //poczatkowa pozycja glowy dla nowego punku kalibracyjnego
	CvPoint2D32f m_GazePosition; 

	float m_Scale;
	int m_ScreenPointIndex;

	CalibrationMesureType m_MesureType;
	bool m_bIsOutLiners;

	CvPoint2D32f m_LeftGaze; 
	CvPoint2D32f m_RightGaze; 
	CvPoint2D32f m_ScreenPoint;

	CvPoint2D32f m_SrcPoint;

	CvPoint2D32f m_HeadAngle;
	double m_HeadPitch; //x 
	double m_HeadRoll; //y

	CvPoint2D32f m_HeadPosition; // Head center position (model 0,0,0)

	void UpdateData(); // inicjuje dane

	//CvPoint2D32f getHeadMove()
	//{
	//	CvPoint2D32f result;
	//	result.x = m_StartHeadPosition.x - m_HeadPosition.x;
	//	result.y = m_StartHeadPosition.y - m_HeadPosition.y;
	//	return result;
	//}

};
/////////////////////////////////////////////////////////////
class CCalibration
{
public:
	double m_Scale ;
	CvPoint2D32f m_Shift;
	//	CCalibration(void);
	//public:
	//	~CCalibration(void);
	bool m_bHomograpyMatrixCoputed;
	float m_HomograpyMatrixData[9];

	float SkipFirstPointCountFactor;


	vector<CvPoint2D32f> m_vScreenPoints;
	vector<CvPoint2D32f> m_vGazePoints;


	CvPoint2D32f ScreenPoints[CalibrationScreenPointsCount];
	CvPoint2D32f GazePoints[CalibrationScreenPointsCount]; 

	CvMat m_HomograpyMatrix; 
	CvPoint2D32f m_HeadShift;

	CvMat m_HeadAngleMap; 
	CvMat m_HeadPositionMap; 
	CvMat m_LeftEyeAngleMap; 
	CvMat m_RightEyeAngleMap; 

	float m_HeadAngleMapData[9];
	float m_HeadPositionMapData[9];
	float m_LeftEyeAngleMapData[9];
	float m_RightEyeAngleMapData[9];

	bool m_bHeadAngleMapCoputed;
	bool m_bLasstSquareCoputed;
	bool m_bHeadPositionMapCoputed;
	bool m_bLeftEyeAngleMapCoputed;

	bool m_bRightEyeAngleMapCoputed;



	CvMat *  m_pPerspectiveMatrix; 

	int m_CurrentCaliprationPointNr;
	CvPoint2D32f m_HeadMoveFactor; // wspuczynniki wplywu ruchu glowy na wzroks

	CvPoint m_ScreenX;
	CvPoint m_ScreenY;

	CvSize m_ScreenSize;

	CvPoint m_CurrentScreenPoint;
	CvPoint2D32f m_StartHeadPosition;// poczatkowa pozycja glowy dla nowego punku kalibracyjnego
	CvPoint2D32f *m_CalibrationScreenPoints;


	CvPoint2D32f m_MoveFactor;
	CvPoint2D32f m_CenterShift;


	vector<float> m_vUsedCalPoint;



double LeftSx, LeftSy;
double LeftGx, LeftGy;
double LeftAx, LeftAy;


	vector<CCalibrationMesure> m_CalibrationMesurePoint;
	//============================================================
	CCalibration()
	{
		SkipFirstPointCountFactor = 0.4;


		m_pPerspectiveMatrix = cvCreateMat(3,3,CV_64FC1);
		m_HomograpyMatrix = cvMat(3, 3, CV_32FC1, m_HomograpyMatrixData);

		m_HeadAngleMap = cvMat(3, 3, CV_32FC1, m_HeadAngleMapData);; 
		m_HeadPositionMap = cvMat(3, 3, CV_32FC1, m_HeadPositionMapData);; 
		m_LeftEyeAngleMap = cvMat(3, 3, CV_32FC1, m_LeftEyeAngleMapData);; 
		m_RightEyeAngleMap = cvMat(3, 3, CV_32FC1, m_RightEyeAngleMapData);; 

		m_ScreenSize.width  = 800;
		m_ScreenSize.height  = 600;
		
		m_bHomograpyMatrixCoputed = false;
		m_bHeadAngleMapCoputed= false;;
		m_bHeadPositionMapCoputed= false;;
		m_bLeftEyeAngleMapCoputed= false;;
		m_bRightEyeAngleMapCoputed= false;;
		m_bLasstSquareCoputed = false;


		Init();
		//m_CalibrationScreenPoints = { {0.1,0.1}, {0.1,0.9}, {0.9,0.1}, {0.9,0.9} };



	}

	//============================================================
	void Init()
	{
		m_vScreenPoints.clear();
		m_vGazePoints.clear();
		m_CalibrationMesurePoint.clear();

		m_CurrentCaliprationPointNr = 0;
	}
	//============================================================

	bool GetNextCalibrationScreenPoint(CvPoint &outPoint)
	{
		m_CurrentCaliprationPointNr++;

		outPoint.x = (float)m_ScreenSize.width * g_CalibrationScreenPoints[m_CurrentCaliprationPointNr % CalibrationScreenPointsCount].x;
		outPoint.y= m_ScreenSize.height * g_CalibrationScreenPoints[m_CurrentCaliprationPointNr % CalibrationScreenPointsCount].y;
		return m_CurrentCaliprationPointNr <=   CalibrationScreenPointsCount;

	}

	//============================================================
	bool GetCalibrationScreenPoint(CvPoint &outPoint)
	{
		const int MeserCountPerPunkt = 15;
		if ((m_CalibrationMesurePoint.size() % MeserCountPerPunkt == 0)&&(m_CalibrationMesurePoint.size() > MeserCountPerPunkt))
			m_CurrentCaliprationPointNr++;


		outPoint.x = (float)m_ScreenSize.width * g_CalibrationScreenPoints[m_CurrentCaliprationPointNr % CalibrationScreenPointsCount].x;
		outPoint.y= m_ScreenSize.height * g_CalibrationScreenPoints[m_CurrentCaliprationPointNr % CalibrationScreenPointsCount].y;
		return m_CurrentCaliprationPointNr <=   CalibrationScreenPointsCount;

	}

	//============================================================
	void AddMesure(CvPoint2D32f LeftGaze,CvPoint2D32f RightGaze, CvPoint2D32f ScreenPoint)
	{
		CCalibrationMesure tmp;

		tmp.m_LeftGaze =LeftGaze ;
		tmp.m_RightGaze = RightGaze;
		tmp.m_ScreenPoint = ScreenPoint;
		m_CalibrationMesurePoint.push_back(tmp);

	}
	//============================================================
	void AddMesure(CCalibrationMesure mesure)
	{


		/*		switch(mesure.m_MesureType)
		{

		CalibrationMesureEyeAngle :
		{
		mesure.m_SrcPoint = 
		break;
		}
		CalibrationMesureHeadAngle:
		{
		m_CalibrationScreenPointsCount = m_vCalibrationScreenPointsHeadAngle.size();
		color = CV_RGB(0,222,0);
		break;
		}
		CalibrationMesureHeadPosition:
		{
		m_CalibrationScreenPointsCount = m_vCalibrationScreenPointsHeadPosition.size();
		color = CV_RGB(0,0,222);
		break;
		}

		}*/



		m_CalibrationMesurePoint.push_back(mesure);

	}
	//============================================================
bool CCalibration::ComputeCalibrationHeadAngle();

	void PlotCalibrationResult();
	//============================================================

	void RemoveOutLiners();
	//============================================================

	void ComputeCalibration();
		bool CCalibration::ComputeCalibrationAll();
	bool CCalibration::ComputeHomograpyMatrix(CvPoint2D32f  CCalibrationMesure::*ptrSrcPoint /*CalibrationMesureType MesureType*/, CvMat * MapMatrix, float SkipFirstMeserFactor,char * WinName = "plot result");
	
	bool CCalibration::_ComputeHomograpyMatrix(CvPoint2D32f  CCalibrationMesure::*ptrSrcPoint /*CalibrationMesureType MesureType*/, CvMat * MapMatrix, float SkipFirstMeserFactor, char * WinName);




	//============================================================
	CvPoint GetMapedPoint(CvPoint2D32f LeftGaze,CvPoint2D32f RightGaze, CvSize ScreenSize );
	CvPoint  CCalibration::GetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize );
CvPoint2D32f  CCalibration::fGetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize );

	CvPoint  CCalibration::_GetMapedPointLeftEye(CvPoint2D32f LeftGaze, CvSize ScreenSize );
	CvPoint2D32f    CCalibration::GetMapedPoint(CvPoint2D32f SrcPoint,float * HomograpyMatrixData, CvSize ScreenSize );
		CvPoint  CCalibration::GetMapedPoint(CCalibrationMesure mesure, CvSize ScreenSize );


		CvPoint2D32f  CCalibration::GetMapedLeft(CvPoint2D32f gaze, CvSize ScreenSize );
	//============================================================


	void _ComputeHeadMoveFactor()
	{
		int UsedPointMesuer = 0;
		m_HeadMoveFactor.x = 0;
		m_HeadMoveFactor.y = 0;
		vector<CCalibrationMesure>::iterator it;

		CCalibrationMesure PrevMesure;
		PrevMesure =(*m_CalibrationMesurePoint.begin());	

		int point_count = 1000;
		//CvMat PrevPoints = cvCreateMat(3,4,CV_32FC1);;
		//CvMat CurrentPoints = cvCreateMat(3,4,CV_32FC1);;


		CvPoint2D32f HeadMove;
		CvPoint2D32f EyeMove;

		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{

			HeadMove.x = PrevMesure.m_HeadPosition.x - (*it).m_HeadPosition.x;
			HeadMove.y = PrevMesure.m_HeadPosition.y - (*it).m_HeadPosition.y;

			EyeMove.x =  PrevMesure.m_GazePosition.x - (*it).m_GazePosition.x ;
			EyeMove.y =  PrevMesure.m_GazePosition.y - (*it).m_GazePosition.y ;
			m_HeadMoveFactor.x += HeadMove.x != 0 ? EyeMove.x /HeadMove.x : 0  ;
			m_HeadMoveFactor.y +=  HeadMove.y != 0 ? EyeMove.y /HeadMove.y : 0  ;
			PrevMesure = (*it);
			UsedPointMesuer++;
		};// for

		m_HeadMoveFactor.x = m_HeadMoveFactor.x /UsedPointMesuer  ;
		m_HeadMoveFactor.y = m_HeadMoveFactor.y /UsedPointMesuer  ;
		//cout<<"m_HeadMoveFactor.x :"<<m_HeadMoveFactor.x  <<endl;
		//cout<<"m_HeadMoveFactor.y:"<<m_HeadMoveFactor.y <<endl;

	}
	//============================================================

	void ComputeHeadMoveFactor()
	{
		int UsedPointMesuer = 0;
		m_HeadMoveFactor.x = 0;
		m_HeadMoveFactor.y = 0;
		vector<CCalibrationMesure>::iterator it;

		CCalibrationMesure FirstMesure;
		CCalibrationMesure PrevMesure;
		FirstMesure = (*m_CalibrationMesurePoint.begin());	
		PrevMesure =FirstMesure;

		int point_count = 1000;
		//CvMat PrevPoints = cvCreateMat(3,4,CV_32FC1);;
		//CvMat CurrentPoints = cvCreateMat(3,4,CV_32FC1);;


		CvPoint2D32f HeadMove;
		CvPoint2D32f EyeMove;

		CvPoint2D32f EyeMoveAvr; 
		CvPoint2D32f MaxFactor ; 
		CvPoint2D32f MinFactor;
		MaxFactor.x = 0;
		MaxFactor.y = 0;
		MinFactor.x = 11110;
		MinFactor.y = 11110;


		EyeMoveAvr.x = 0;
		EyeMoveAvr.y= 0;

		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{

			HeadMove.x = PrevMesure.m_HeadPosition.x -  FirstMesure.m_HeadPosition.x;
			HeadMove.y = PrevMesure.m_HeadPosition.y - FirstMesure.m_HeadPosition.y ;

			EyeMove.x =  PrevMesure.m_GazePosition.x -  FirstMesure.m_GazePosition.x ;
			EyeMove.y =  PrevMesure.m_GazePosition.y -  FirstMesure.m_GazePosition.y;


			float curentX = HeadMove.x != 0 ? abs(HeadMove.x /EyeMove.x ) : 0  ;
			float curentY =  HeadMove.y != 0 ? abs(HeadMove.y /EyeMove.y ) : 0  ;
			m_HeadMoveFactor.x += curentX;
			m_HeadMoveFactor.y += curentY;

			if (MaxFactor.x < curentX) 
				MaxFactor.x  = curentX;

			if (MaxFactor.y < curentY) 
				MaxFactor.y = curentY;

			if (MinFactor.x > curentX) 
				MinFactor.x  = curentX;

			if (MinFactor.y > curentY) 
				MinFactor.y = curentY;


			EyeMoveAvr.x +=  abs(EyeMove.x) ;
			EyeMoveAvr.y +=   abs(EyeMove.y) ;
			PrevMesure = (*it);
			UsedPointMesuer++;
		};// for

		m_HeadMoveFactor.x = m_HeadMoveFactor.x /UsedPointMesuer  ;
		m_HeadMoveFactor.y = m_HeadMoveFactor.y /UsedPointMesuer  ;

		EyeMoveAvr.x = EyeMoveAvr.x /UsedPointMesuer  ;
		EyeMoveAvr.y = EyeMoveAvr.y /UsedPointMesuer  ;

		cout<<"m_HeadMoveFactor.x :"<<m_HeadMoveFactor.x  <<endl;
		cout<<"m_HeadMoveFactor.y:"<<m_HeadMoveFactor.y <<endl;
		cout<<"EyeMoveAvr.x :"<<EyeMoveAvr.x  <<endl;
		cout<<"EyeMoveAvr.y:"<<EyeMoveAvr.y <<endl;

		cout<<"MaxFactor.x :"<<MaxFactor.x  <<endl;
		cout<<"MaxFactor.y:"<<MaxFactor.y <<endl;

		cout<<"MinFactor.x :"<<MinFactor.x  <<endl;
		cout<<"MinFactor.y:"<<MinFactor.y <<endl;



	}
	//============================================================




	void UpdateMaping()
	{

		//TODO: uncoment tmp
		//return;
		UpdateHomograpy();

		CvPoint2D32f MoveFactor;
		CvPoint2D32f CenterShift;

		CvPoint2D32f ScreenPointCenter;
		CvPoint2D32f GazePointCenter;



		CvPoint MoveFactorCount = cvPoint(0,0);

		CvPoint2D32f MapedGazePoints[CalibrationScreenPointsCount]; 

		MoveFactor.x = 0 ;
		MoveFactor.y = 0 ;
		CenterShift.x = 0 ;
		CenterShift.y = 0 ;

		ScreenPointCenter.x = 0 ;
		ScreenPointCenter.y = 0 ;
		GazePointCenter.x = 0 ;
		GazePointCenter.y = 0 ;

		for (int i = 2 ;i< CalibrationScreenPointsCount; i++)
		{
			if (abs(ScreenPoints[i-1].x - ScreenPoints[i].x) > 0 ) 
			{
				MoveFactor.x += (GazePoints[i-1].x - GazePoints[i].x) / (ScreenPoints[i-1].x - ScreenPoints[i].x);
				MoveFactorCount.x++;
			};

			if (abs(ScreenPoints[i-1].y - ScreenPoints[i].y) > 0 ) 
			{
				MoveFactor.y+= (GazePoints[i-1].y - GazePoints[i].y) / (ScreenPoints[i-1].y - ScreenPoints[i].y);
				MoveFactorCount.y++;
			};
		};
		MoveFactor.x = MoveFactor.x / MoveFactorCount.x;
		MoveFactor.y = MoveFactor.y / MoveFactorCount.y;


		for (int i = 1 ;i< CalibrationScreenPointsCount; i++)
		{
			MapedGazePoints[i].x = GazePoints[i].x /abs(MoveFactor.x);
			MapedGazePoints[i].y = GazePoints[i].y / abs(MoveFactor.y);
		};



		for (int i = 1 ;i< CalibrationScreenPointsCount; i++)
		{
			GazePointCenter.x += MapedGazePoints[i].x;
			GazePointCenter.y += MapedGazePoints[i].y;
			ScreenPointCenter.x += ScreenPoints[i].x;
			ScreenPointCenter.y += ScreenPoints[i].y;

		};


		GazePointCenter.x = GazePointCenter.x / CalibrationScreenPointsCount;
		GazePointCenter.y = GazePointCenter.y / CalibrationScreenPointsCount;

		ScreenPointCenter.x = ScreenPointCenter.x / CalibrationScreenPointsCount;
		ScreenPointCenter.y = ScreenPointCenter.x / CalibrationScreenPointsCount;



		CenterShift.x = ScreenPointCenter.x - GazePointCenter.x;
		CenterShift.y = ScreenPointCenter.y - GazePointCenter.y;

		m_MoveFactor = MoveFactor;
		m_CenterShift = CenterShift;


	}

	//============================================================

	void UpdateHomograpy()
	{

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

		int PrevScreenPointIndex = 0;



		BeginCalibrationIterator = m_CalibrationMesurePoint.begin();
		for( it =m_CalibrationMesurePoint.begin(); it != m_CalibrationMesurePoint.end(); it++)
		{
			if (PrevScreenPointIndex <(*it).m_ScreenPointIndex )
			{
				ScreenPoints[PrevScreenPointIndex].x = g_CalibrationScreenPoints[PrevScreenPointIndex].x *m_ScreenSize.width ;
				ScreenPoints[PrevScreenPointIndex].y = g_CalibrationScreenPoints[PrevScreenPointIndex].y *m_ScreenSize.height ;

				MeanPoint.x =  MeanPoint.x / CurrentPointMesureCount;
				MeanPoint.y =  MeanPoint.y / CurrentPointMesureCount;

				//------------------------------------------error mean---------------------------------
				AvrDistansErr = 0;
				for( it2 =BeginCalibrationIterator; it2 != it; it2++)
				{
					AvrDistansErr += ((*it2).m_GazePosition.x - MeanPoint.x)*((*it2).m_GazePosition.x - MeanPoint.x) + 
						((*it2).m_GazePosition.y - MeanPoint.y)*((*it2).m_GazePosition.y - MeanPoint.y) ;

				};
				AvrDistansErr = AvrDistansErr / CurrentPointMesureCount;

				PointAvrCorrected.x = 0;
				PointAvrCorrected.y = 0;
				for( it2 =BeginCalibrationIterator; it2 != it; it2++)
				{

					if ( ((*it2).m_GazePosition.x - MeanPoint.x)*((*it2).m_GazePosition.x - MeanPoint.x) + 
						((*it2).m_GazePosition.y - MeanPoint.y)*((*it2).m_GazePosition.y - MeanPoint.y)  < 3 * AvrDistansErr	)
					{ 
						PointAvrCorrected.x +=(*it).m_GazePosition.x ;
						PointAvrCorrected.y +=(*it).m_GazePosition.y ;
					}
				};

				//------------------------------------------end error mean---------------------------------

				GazePoints[PrevScreenPointIndex ].x = PointAvrCorrected.x / CurrentPointMesureCount;
				GazePoints[PrevScreenPointIndex ].y = PointAvrCorrected.y / CurrentPointMesureCount;

				//todo uncoment
				/*				GazePoints[PrevScreenPointIndex ].x = MeanPoint.x ;
				GazePoints[PrevScreenPointIndex ].y = MeanPoint.y ;*/


				PrevScreenPointIndex = (*it).m_ScreenPointIndex ; 
				CurrentPointAvr.x = 0;
				CurrentPointAvr.y = 0;
				CurrentPointMesureCount = 0;
				BeginCalibrationIterator =  it;
			}


			CurrentPointMesureCount++;
			MeanPoint.x +=(*it).m_GazePosition.x ;
			MeanPoint.y +=(*it).m_GazePosition.y ;

		};

		GazePoints[PrevScreenPointIndex ].x = CurrentPointAvr.x / CurrentPointMesureCount;
		GazePoints[PrevScreenPointIndex ].y = CurrentPointAvr.y / CurrentPointMesureCount;
		ScreenPoints[PrevScreenPointIndex].x = g_CalibrationScreenPoints[PrevScreenPointIndex].x *m_ScreenSize.width ;
		ScreenPoints[PrevScreenPointIndex].y = g_CalibrationScreenPoints[PrevScreenPointIndex].y *m_ScreenSize.height ;


		//PrevScreenPointIndex++;
		//GazePoints[PrevScreenPointIndex ].x = CurrentPointAvr.x / CurrentPointMesureCount;
		//GazePoints[PrevScreenPointIndex ].y = CurrentPointAvr.y / CurrentPointMesureCount;
		//ScreenPoints[PrevScreenPointIndex].x = g_CalibrationScreenPoints[PrevScreenPointIndex].x *m_ScreenSize.width ;
		//ScreenPoints[PrevScreenPointIndex].y = g_CalibrationScreenPoints[PrevScreenPointIndex].y *m_ScreenSize.height ;





		CvMat ScreenPointsMat = cvMat(1,CalibrationScreenPointsCount,CV_32FC2,ScreenPoints);;
		CvMat GazePointsMat = cvMat(1,CalibrationScreenPointsCount,CV_32FC2,GazePoints);

		cvGetPerspectiveTransform(&GazePoints[1],&ScreenPoints[1],m_pPerspectiveMatrix);
		m_bHomograpyMatrixCoputed = true;


		//if (cvFindHomography(&GazePointsMat, &ScreenPointsMat,&m_HomograpyMatrix, CV_RANSAC,0.1 ))
		//{
		//	m_bHomograpyMatrixCoputed = true;
		//cout<<"find homograpy maping"<<endl;
		//}
		//else
		//		{

		//cout<<"Dont find homograpy maping"<<endl;
		//};


	};

	//============================================================
	CvPoint MapPoint(CvPoint2D32f Gaze)
	{

		CvPoint result;

		if (m_MoveFactor.x > 0 )
			result.x = Gaze.x / m_MoveFactor.x + m_CenterShift.x;
		else
			result.x = 800 - (Gaze.x / abs(m_MoveFactor.x) + m_CenterShift.x);

		if (m_MoveFactor.y > 0 )
			result.y = Gaze.y / m_MoveFactor.y + m_CenterShift.y;
		else
			result.y = 600 - (Gaze.y / abs(m_MoveFactor.y) + m_CenterShift.y);

		return result;


		if (m_bHomograpyMatrixCoputed)
		{


			CvMat* src = cvCreateMat(1,1,CV_32FC3);
			CvMat* dst = cvCreateMat(1,1,CV_32FC3);

			cvPerspectiveTransform(  src,  dst, m_pPerspectiveMatrix );


			src->data.fl[0] = Gaze.x;
			src->data.fl[1] = Gaze.y;
			src->data.fl[2] = 1;
			result.x = dst->data.fl[0];
			result.y = dst->data.fl[1];
			float z = dst->data.fl[2];


			return cvPoint(cvRound(dst->data.fl[0]), cvRound(dst->data.fl[1]));

			//float *h = m_HomograpyMatrixData;
			//		float x = Gaze.x;
			//		float y = Gaze.y;
			//		float Z = 1./(h[6]*x + h[7]*y + h[8]);
			//
			//		float X = (h[0]*x + h[1]*y + h[2])*Z;
			//		float Y = (h[3]*x + h[4]*y + h[5])*Z;
			//
			//		return cvPoint(cvRound(X), cvRound(Y));

		}
		else
			return cvPoint(0,0);
	};

};
