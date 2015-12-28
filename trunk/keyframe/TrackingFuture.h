#pragma once
#include "Utilitis.h"

#include "Interfaces.h"

//const int  m_FutureImgDescriptorSize  = 3;
//
//const int m_MinFutureDistans =20;

//Param to set
const int  FutureImgDescriptorSize  = 5;
const int MinFutureDistans = 5;
const int Model3DFitHistoryCount = 100;

////////////////////////////////////////////////////////////////////////
enum FutureState {  TrackigState, LerningState, MissingState, NullState};


enum Model3DPointState {  NewPointState, InModelState,InModelNotUseState, FitErrorState, NotInModelState, OutLintersState, NotLocalizedState};
class CTrackingFuture
{
public:
	int m_Model3DCounter;
		//float m_vModel3DFitHistory[Model3DFitHistoryCount];
		float m_Model3DAvrFitErr;
		//float m_Model3DAvrFitErr;
	CTrackingFuture(void)
	{

		m_bIsKeyFramePoint = false;
		 //m_vModel3DFitHistory[Model3DFitHistoryCount];
		 m_Model3DAvrFitErr =  0;

		 m_bIsModel3D = false;
		m_Model3DCounter = 0;
		m_bMoved = false;
		m_FutureState = NullState;
		m_Model3DState = NotInModelState;
		m_FindCount = 0;
		m_MissCount = 0;
		m_LastFindCount = 0;
		m_LastMissCount = 0;
	//	m_ImgDescriptor =  cvCreateImage( cvSize(FutureImgDescriptorSize,FutureImgDescriptorSize), 8, 1 );
		m_CorrnerEigVaule = 0;
		m_CurrentCorrnerEigValue = 0;
		m_FitError = 0;
		m_PrevLocation = cvPoint2D32f(0,0);
		m_bFindByDescriptor = false;

		m_XCenterDistans =  - 10000000;
		m_YCenterDistans = - 10000000;
		m_CenterDistansError = 0;
		m_PrevLocation.x = 999999998888888;
		m_PrevLocation.y = -99999999999999;

		m_ShiftError.x = 0;
		m_ShiftError.y=0;

		m_Model3DCoordinate.x  = 0;
		m_Model3DCoordinate.y  = 0;
		m_Model3DCoordinate.z  = 0;
		m_bIsModel3D = false;
		m_bIsToRemoveModel3D = false;
		m_bIsErased = false;
		m_bLocationCorected = false;

	}
public:

	FutureState m_FutureState;
	Model3DPointState m_Model3DState;
	~CTrackingFuture(void)
	{
	//	cvReleaseImage(&m_ImgDescriptor);
		int b = 4;
	}


	//for POSIT
	CvPoint3D32f m_Model3DCoordinate;
	bool m_bIsModel3D;


// jesli zostanie usunienty z listy cech jest ustawiane
	bool m_bIsErased;

	bool m_bIsKeyFramePoint;
	bool m_bIsToRemoveModel3D;

	int m_FindCount;
	int m_MissCount;

	 unsigned int m_FuterStron; // okresla jak dana cecha wyroznia sie z otoczenia


	bool m_bMoved; // zostala zmieniona pozycja w modelu np ruch galki okas
	bool m_InitFaze;
	bool m_bFindByDescriptor;

	double m_CorrnerEigVaule;
	double m_CurrentCorrnerEigValue;
	double m_FitError;

	


	IplImage* m_ImgDescriptor;

	int m_LastFindCount;
	int m_LastMissCount;

	float m_XCenterDistans;
	float m_YCenterDistans;

	float m_CenterDistansError; // blad roznicy odleglosci do centrum modelu

	bool m_bLocationCorected;

	bool m_bLocationCorected1;
	bool m_bBadFit; // slapo pasuje


	bool m_bFirstOFEstymationOk;
	bool m_bSecondOFEstymationOk; // slapo pasuje


	CvPoint2D32f m_bFirstOFEstymationLocation;
	CvPoint2D32f m_bSecondOFEstymationLocation;

	float m_bFirstOFEstymationFitError;
	float m_bSecondOFEstymationFitError;

	bool m_IsLocated;

	//IFutureDescriptor *m_FutureDescriptor;

	CvSURFPoint m_SURFPoint;
	//CvPoint2D32f m_Point;

	CvPoint2D32f m_PrevLocation;
	CvPoint2D32f m_CurrentLocation;
	CvPoint2D32f m_KeyFrameLocation;
	CvPoint2D32f m_TransformKeyFrameLocation;

	CvPoint2D32f m_ShiftError;
	CvPoint2D32f m_ScaleError;
	CvPoint2D32f m_StableError; // proporcjonalne do ruchchliwosci punktu




	float m_Descriptor[128];

	//cvExtractSURF( ImgIn, 0, &objectKeypoints, &objectDescriptors, storage, aa );
	//====================================================================
	void UpdateCenterDistansError(CvPoint2D32f center)
	{
			m_CenterDistansError +=
			(m_CurrentLocation.x  - m_XCenterDistans - center.x) * (m_CurrentLocation.x  - m_XCenterDistans - center.x) +
			(m_CurrentLocation.y  - m_YCenterDistans - center.y) * (m_CurrentLocation.y  - m_YCenterDistans - center.y);
	
	}
//====================================================================

	void setModelCenterDistans(CvPoint2D32f center, double scale = 1)
	{
			m_XCenterDistans = (m_CurrentLocation.x  - center.x) *scale ;
			m_YCenterDistans =  (m_CurrentLocation.y  - center.y)*scale ;
	}
	//====================================================================


	CvPoint2D32f getCurrentModelCenter(double scale)
	{
		CvPoint2D32f center;
			center.x = m_CurrentLocation.x  - m_XCenterDistans * scale;
			center.y =  m_CurrentLocation.y  -m_YCenterDistans * scale;
			return center;
	}
	//====================================================================

	void UpdatePosition(CvPoint2D32f position)
	{
		m_PrevLocation = m_CurrentLocation;
		m_CurrentLocation = position;	
		//m_Point = position;
	}

		//====================================================================

	// czy cecha jest zakceptowana jako czesc modelu
	bool IsModelPoint()
	{
		return true;
// TODO cos lepszebo
		return (m_LastFindCount > 1);
		return (m_LastFindCount > 3 ) && ( m_FindCount > 10) ;

	}
	//====================================================================
	void IsLocated()
	{
		m_FutureState = TrackigState;
		m_IsLocated =true;
		m_FindCount++;
		m_LastFindCount ++;
		m_LastMissCount = 0;
	}


	//====================================================================
	void IsMissing()
	{
		m_bFindByDescriptor = false;
		m_FutureState =MissingState;
		m_IsLocated = false;
		m_MissCount ++;
		m_LastFindCount = 0;
		m_LastMissCount ++;
	}


	//====================================================================
	bool IsGoodToTrack()
	{
		return true;
		double min, max;
		cvMinMaxLoc(m_ImgDescriptor,&min, &max);
		return min < 125;
	}


	//====================================================================
	//bool IsInRect(CvRect Window)
	//{

	//	const int margin = 0;
	//	int minX = Window.x - margin;
	//	int minY = Window.y - margin;
	//	int maxX =  Window.x +Window.width + margin;
	//	int maxY =  Window.y +Window.height+ margin;

	//	return( (m_CurrentLocation.x > minX)&&
	//		(m_CurrentLocation.x < maxX)&&
	//		(m_CurrentLocation.y > minY)&&
	//		(m_CurrentLocation.y < maxY));

	//}

	//====================================================================
	bool IsInRect(CvRect Window, float Tolerant = 0)
	{

		 int margin = Window.width * Tolerant;
		int minX = Window.x - margin;
		int minY = Window.y - margin;
		int maxX =  Window.x +Window.width + 2*margin;
		int maxY =  Window.y +Window.height+ 2*margin;

		return( (m_CurrentLocation.x > minX)&&
			(m_CurrentLocation.x < maxX)&&
			(m_CurrentLocation.y > minY)&&
			(m_CurrentLocation.y < maxY));

	}
	//====================================================================

	float getScore()
	{

		if (m_MissCount == 0)
			return m_FindCount;
		else

			return m_FindCount / m_MissCount;


	}


	//====================================================================

float CheckMoveDistans(CvPoint2D32f newPosition)
{
		return 
		( m_CurrentLocation.x - newPosition.x)*( m_CurrentLocation.x - newPosition.x) + 
		( m_CurrentLocation.y - newPosition.y) * ( m_CurrentLocation.y - newPosition.y);
	
}

//====================================================================

float getMove()
{
		return 
		( m_PrevLocation.x - m_CurrentLocation.x)*( m_PrevLocation.x - m_CurrentLocation.x) + 
		( m_PrevLocation.y - m_CurrentLocation.y) * ( m_PrevLocation.y - m_CurrentLocation.y);
	
}

//====================================================================

CvPoint2D32f getShift()
{




	CvPoint2D32f result;
	result.x = m_PrevLocation.x - m_CurrentLocation.x;
	result.y = m_PrevLocation.y - m_CurrentLocation.y;

	return result;
	
}
//====================================================================



void UpdateShiftError(CvPoint2D32f AvrShrift)
{
	m_ShiftError.x += abs( AvrShrift.x - getShift().x) ;
	m_ShiftError.y += abs( AvrShrift.x - getShift().y) ;
}

//====================================================================



double GetShiftError()
{
	return m_ShiftError.x +m_ShiftError.y  ;
}
//====================================================================

float getMove(CvPoint2D32f CurrentLocation)
{
		return 
		( m_PrevLocation.x - CurrentLocation.x)*( m_PrevLocation.x - CurrentLocation.x) + 
		( m_PrevLocation.y - CurrentLocation.y) * ( m_PrevLocation.y - CurrentLocation.y);
	
}
//====================================================================

float getCenterMove(CvPoint2D32f CurrentLocation,CvPoint2D32f CurrentCenter)
{

			CvPoint2D32f center;
			center.x = CurrentLocation.x  - m_XCenterDistans;
			center.y =  CurrentLocation.y  -m_YCenterDistans;
			return (center.x - CurrentCenter.x) * (center.x - CurrentCenter.x) + 
				(center.y - CurrentCenter.y) * (center.y - CurrentCenter.y) ;
}

//====================================================================

double getScale(CvPoint2D32f CurrentCenter)
{

			double result = 

			(m_XCenterDistans*m_XCenterDistans + m_YCenterDistans* m_YCenterDistans) / 

			(( m_CurrentLocation.x  - CurrentCenter.x)*( m_CurrentLocation.x  - CurrentCenter.x) +
			( m_CurrentLocation.y  - CurrentCenter.y)*( m_CurrentLocation.y  - CurrentCenter.y));

			return result;
}
//====================================================================

	float getFitScore()
	{
		//m_FitError / m_FindCount
	//	float t =  ((m_FindCount  - m_MissCount) * getScore())   -( m_FitError / 300)  - m_CenterDistansError ;

	float t =  (m_FindCount  - m_MissCount)   - ( m_FitError / 300) ;// - m_CenterDistansError/10 ;
	
		return t;
//return getScore();

	}

	//====================================================================

	float getCenterDistansFactor()
	{
		//m_FitError / m_FindCount
		return 1;
		

		float t =  10  - (m_CenterDistansError / m_FindCount)   ;
		t = t > 0 ? t * t : 0;
		t = t / 50;
		int f = ( m_FindCount )  / 100;
		return 1;
		return t + f;
	

	}

	//====================================================================

	void UpdateImgDescriptor(IplImage* frame)
	{
		//CvRect Roi;
		//Roi.x	= m_Point.x - m_ImgDescriptor->width/2;
		//Roi.x	= m_Point.y - m_ImgDescriptor->height/2;
		//Roi.width = m_ImgDescriptor->width;
		//Roi.height = m_ImgDescriptor->height;
		//cvSetImageROI(frame, Roi );
		//cvCopy(frame,m_ImgDescriptor);
		//cvResetImageROI(frame);
		try{
			cvCopy(frame,m_ImgDescriptor);
		}
		catch(...)
		{
			;
		};


	}
	//====================================================================
};

////////////////////////////////////////////////////////////////////////


//bool CompareTrackingFutures (shared_ptr<CTrackingFuture> a,shared_ptr<CTrackingFuture> b) 
//{ 
//	return a->getFitScore() <b->getFitScore();
//	
//}