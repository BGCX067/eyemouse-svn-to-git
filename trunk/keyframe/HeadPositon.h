#pragma once
#include "PointTracker.h"
#include "HeadCenter.h"
#include "HeadPositionDebug.h"
//#include <boost/thread/mutex.hpp>
const int USE_RANSAC=0;
const int RANSAC_SAMPLES=15;
const double RANSAC_DISTANCE_THRESHOLD = 60.0;
const int RANSAC_ITERATIONS = 30;
const float EHCIMODELSCALE = 1.0;//50;
const int EHCIFOCUS  = 1000;
const int MODELSCALE  = 1000; // to do musi zalezec od rozdzielczosci frame

class CHeadPositionDebug;

extern CvMatr32f rotation_matrix ;
extern	CvVect32f translation_vector ;
extern float GlobalPositMatrix[16];

//extern	boost::mutex MatrixMutex;

class CHeadPositon
{
public:
	CHeadPositon(void);
public:
	~CHeadPositon(void);

	bool m_bPositMatrixInitialized;

	
		float m_HeadYaw, m_HeadPitch, m_HeadRoll;



	int m_IterationCount;
	double m_InitEyesDistans;// odleglosc miedzy srodkami oczu przy inicjacji modelu
	CvPoint2D32f m_HeadGazeEstymation;


	float m_fAvrFitError;
	float m_fCurrentFitError;

	double m_fModelScale;
	CvPoint3D32f m_CurrentHeadCenter3D;

	CvPoint3D32f m_ModelRefHeadCenter3D; // init transormed posit matrix head center

	list<CvPoint> m_vHeadPositionGraph;

	CvPoint3D32f m_RefPoint; // pierwszy pkt modelu

	//key frame transform
	CvPoint3D32f m_KeyFrameTransformModelP1;
	CvPoint3D32f m_KeyFrameTransformModelP2;
	CvPoint3D32f m_KeyFrameTransformModelP3;
	CvPoint3D32f m_KeyFrameTransformModelP4;

	CvPoint2D32f m_KeyFrameTransformBaseP1;
	CvPoint2D32f m_KeyFrameTransformBaseP2;
	CvPoint2D32f m_KeyFrameTransformBaseP3;
	CvPoint2D32f m_KeyFrameTransformBaseP4;

	CvPoint2D32f m_KeyFrameTransformCurrentP1;
	CvPoint2D32f m_KeyFrameTransformCurrentP2;
	CvPoint2D32f m_KeyFrameTransformCurrentP3;
	CvPoint2D32f m_KeyFrameTransformCurrentP4;





	CvPoint2D32f m_RefStartHeadPosition; // pozycja srodka glowy w klatce inicjalizujacej model
	IplImage* m_HeadPositionGraph ;

	bool m_bModelInitialized;

	bool m_bPositMatrixComputed;

	bool m_bComputeHeadPositionFirst;

			float m_rotVect[3];

	CPointTracker * m_pTracker;
	//CHeadCenter * m_pHeadCenter;

	std::vector<CvPoint3D32f> m_vModelPoints;
	std::vector<CvPoint2D32f> m_vImagePoints;


	CvMatr32f m_RotationMatrix ;
	CvVect32f m_TransactionMatrix ;

	//CHeadPositionDebug * m_pHeadPositionDebug;

	CvPoint3D32f LeftEyeCenter3D;
	CvPoint3D32f RightEyeCenter3D;



	shared_ptr<CTrackingFuture> m_pReferenceModelPoint;

	// Obliczony srokek oka
	CvPoint2D32f EstimateLeftEyeCenter2D;
	CvPoint2D32f EstimateRightEyeCenter2D;




	//===================================================================
	void  Init(CPointTracker * Tracker)
	{
	
		m_HeadPositionGraph =  cvCreateImage( cvSize(1200,900), 8, 3);
			cvZero(m_HeadPositionGraph);
		//m_pReferenceModelPoint.reset
		//m_pReferenceModelPoint = NULL;
		m_bPositMatrixComputed = false;
		m_RotationMatrix = new float[9];
		m_TransactionMatrix = new float[3];

		m_RotationMatrix[0]= 1.0; m_RotationMatrix[1]=  0; m_RotationMatrix[2]=  0;
		m_RotationMatrix[3]=  0; m_RotationMatrix[4]=1.0; m_RotationMatrix[5]=  0;
		m_RotationMatrix[6]=  0; m_RotationMatrix[7]=  0; m_RotationMatrix[8]=1.0;

		m_TransactionMatrix[0]=   0;
		m_TransactionMatrix[1]=   0;
		m_TransactionMatrix[2]=0;

		m_bModelInitialized = false;
		m_pTracker = Tracker;
		//m_pHeadCenter = HeadCenter;

	m_KeyFrameTransformModelP1 = cvPoint3D32f( 0, 0,0);
	 m_KeyFrameTransformModelP2= cvPoint3D32f( MODELSCALE * -0.1,0,0);
	 m_KeyFrameTransformModelP3= cvPoint3D32f( 0, MODELSCALE * 0.1,0);
	 m_KeyFrameTransformModelP4= cvPoint3D32f( MODELSCALE * 0.1, MODELSCALE * -0.1,0);

	 m_KeyFrameTransformBaseP1 = cvPoint2D32f(0,0);;
				m_KeyFrameTransformBaseP2= cvPoint2D32f(0,0);;;
				m_KeyFrameTransformBaseP3= cvPoint2D32f(0,0);;;;
				m_KeyFrameTransformBaseP4= cvPoint2D32f(0,0);;
	

			m_KeyFrameTransformCurrentP1 =cvPoint2D32f(0,0);;;
			m_KeyFrameTransformCurrentP2= cvPoint2D32f(0,0);;;
			m_KeyFrameTransformCurrentP3=cvPoint2D32f(0,0);;;
			m_KeyFrameTransformCurrentP4=cvPoint2D32f(0,0);;;




	}
	//===================================================================
	// ImageCordinate wspurzedne to procentowa wartosc lokacji w obszarze glowy
	CvPoint3D32f get3dModelCoordinates(CvPoint2D32f ImageCordinate)
	{
		get3dModelCoordinates(ImageCordinate, 1,1);
	}

	//===================================================================


	CvPoint3D32f get3dModelCoordinates(CvPoint2D32f ImageCordinate, float headWidth, float headHeight);


		//===========================================================================================================================

CvPoint3D32f GetTransformPoint(CvPoint3D32f point)
{
	float PositMatrix[16];
		CvPoint3D32f result;
		float Point[4];
		PositMatrix[0] =  m_RotationMatrix[0] ;
		PositMatrix[1] =  m_RotationMatrix[1];
		PositMatrix[2] =  m_RotationMatrix[2];
		PositMatrix[3] =  m_TransactionMatrix[0] ;

		PositMatrix[4] =  m_RotationMatrix[3];
		PositMatrix[5] =  m_RotationMatrix[4];
		PositMatrix[6] =  m_RotationMatrix[5];
		PositMatrix[7] =  m_TransactionMatrix[1] ;

		PositMatrix[8] =   m_RotationMatrix[6];
		PositMatrix[9] =   m_RotationMatrix[7];
		PositMatrix[10] =  m_RotationMatrix[8];
		PositMatrix[11] =   m_TransactionMatrix[2] ;
		PositMatrix[12] =  0;
		PositMatrix[13] =  0;
		PositMatrix[14] =  0;
		PositMatrix[15] = 1.0; //homogeneous

		Point[0] = point.x;
		Point[1] = point.y ;
		Point[2] = point.z ;
		Point[0] -=   m_RefPoint.x;
		Point[1] -=   m_RefPoint.y;
		Point[2] -=	 m_RefPoint.z;
		Point[3] = 1;
		CvMat MatPoint = cvMat( 4, 1, CV_32FC1,&Point);
		CvMat ProjectionMatrix = cvMat( 4, 4, CV_32FC1,&PositMatrix);
		cvMatMul(&ProjectionMatrix,&MatPoint,&MatPoint);
		result.x =Point[0] ;
		result.y =Point[1] ;
		result.z =Point[2] ;
		//result.x =Point[0] -  m_TransactionMatrix[0];
		//result.y =Point[1]  -  m_TransactionMatrix[1];
		//result.z =Point[2]  -  m_TransactionMatrix[2];
		return result;
}
		//===========================================================================================================================

	CvPoint2D32f _ProjectTo2D(CvPoint3D32f point, CvPoint3D32f RefPoint,	CvMatr32f RotationMatrix ,CvVect32f TransactionMatrix )
	{
		float PositMatrix[16];
		CvPoint2D32f result;
		float Point[4];
		PositMatrix[0] =  RotationMatrix[0] ;
		PositMatrix[1] =  RotationMatrix[1];
		PositMatrix[2] =  RotationMatrix[2];
		PositMatrix[3] =  TransactionMatrix[0] ;

		PositMatrix[4] =  RotationMatrix[3];
		PositMatrix[5] =  RotationMatrix[4];
		PositMatrix[6] =  RotationMatrix[5];
		PositMatrix[7] =  TransactionMatrix[1] ;

		PositMatrix[8] =   RotationMatrix[6];
		PositMatrix[9] =   RotationMatrix[7];
		PositMatrix[10] =  RotationMatrix[8];
		PositMatrix[11] =  TransactionMatrix[2] ;



		PositMatrix[12] =  0;
		PositMatrix[13] =  0;
		PositMatrix[14] =  0;
		PositMatrix[15] = 1.0; //homogeneous

		Point[0] = point.x;
		Point[1] = point.y ;
		Point[2] = point.z ;
		Point[0] -=   RefPoint.x;
		Point[1] -=   RefPoint.y;
		Point[2] -=		RefPoint.z;
		Point[3] = 1;
		CvMat MatPoint = cvMat( 4, 1, CV_32FC1,&Point);
		CvMat ProjectionMatrix = cvMat( 4, 4, CV_32FC1,&PositMatrix);
		cvMatMul(&ProjectionMatrix,&MatPoint,&MatPoint);
		result.x =Point[0] * EHCIFOCUS / (Point[2] );
		result.y =Point[1] * EHCIFOCUS / (Point[2] );

	/*	result.x  +=	m_RefStartHeadPosition.x;
		result.y  +=	m_RefStartHeadPosition.y;*/

		//		result.x  -=	 g_TrackinkSystem->m_Graber.m_pFrame->width* 0.5;
		//result.y  -=	 g_TrackinkSystem->m_Graber.m_pFrame->height* 0.5;;




		return result;
	}

	//===========================================================================================================================

	CvPoint2D32f ProjectTo2D(CvPoint3D32f point )
	{
	return _ProjectTo2D( point, m_RefPoint,m_RotationMatrix ,m_TransactionMatrix );
	}



	//===================================================================

	bool InitModel();
		void CHeadPositon::CorrectPointsToModelPosition();
		void CHeadPositon::CheckReferencePoint(bool GetNew = false);
	bool CHeadPositon::Update(bool flag  = true);
	bool CHeadPositon::Ransac();
	int CHeadPositon::RemoveWrongPoint();
			void CHeadPositon::ComputeHeadPosition();
	void CHeadPositon::Plot();
	double CHeadPositon::getDistanceThreshold();
	void CHeadPositon::GetKeyFramePerspectiveTransform( CvMat* map_matrix );

	CvPoint3D32f CHeadPositon::get3dFrom2D(CvPoint2D32f ImageCordinate);

	bool  CHeadPositon::CheckNewEstimation(CvPoint2D32f &ImageCordinate,CvPoint2D32f &Current2D,double & bestError,CvPoint3D32f &BestEstimated3D , bool x);
	//===================================================================



};
