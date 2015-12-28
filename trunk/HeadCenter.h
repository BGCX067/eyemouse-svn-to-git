#pragma once
#include "PointTracker.h"

class CHeadCenter
{
public:
	CHeadCenter(void);
public:
	~CHeadCenter(void);
	bool m_bInitialized;

	CvPoint2D32f m_EyeCorner;
	CvPoint2D32f m_HeadCenterSift;

	CvPoint2D32f m_EyeCenterSift;

	//CvBox2D32f m_LeftPupilElipse;
	//CvBox2D32f m_RightPupilElipse;

	CvRect m_LeftPupilRect;
	CvRect m_RightPupilRect;

	CvRect m_HeadRoi;

	CvPoint2D32f m_HeadCenter;

	CPointTracker * m_pTracker;


	//===================================================================
	void  Init(CPointTracker * Tracker)
	{
		m_pTracker = Tracker;
	}
	//===================================================================

	bool IsEyeFuter(CTrackingFuture * pFuter)
	{
		bool result = (pFuter->IsModelPoint() &&
			(IsPupilFuter(pFuter))
			) ;
		if (result )
		{
			int i = 1;;
		}
		else
		{
			int i = 9;
		}

		return  result;

	}

	//===================================================================

	bool IsPupilFuter(CTrackingFuture * pFuter)
	{
		//return (
		//	(
		//	(PointInElipse(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_LeftPupilElipse, 2))||
		//	(PointInElipse(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_RightPupilElipse, 2))
		//	)
		//	) ;


			return (
			(
			(PointInRect(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_LeftPupilRect))||
					(PointInRect(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_RightPupilRect))
			)
			) ;
		
	


	}

	//===================================================================

	void FindHeadShift();
	//============================================




	//void Update(CvBox2D32f LeftPupilElipse,CvBox2D32f RightPupilElipse, CvRect HeadRoi)
	void Update(CvRect LeftPupilRect,CvRect RightPupilRect, CvRect HeadRoi)
	{

		//m_LeftPupilElipse =LeftPupilElipse;
		//m_RightPupilElipse =RightPupilElipse;
		
		m_LeftPupilRect =LeftPupilRect;
		m_RightPupilRect =RightPupilRect;
		m_HeadRoi = HeadRoi;

		FindHeadShift();


		if (!m_bInitialized)
		{
			/*m_HeadCenter.x = (LeftPupilElipse.center.x + RightPupilElipse.center.x ) /2;
			m_HeadCenter.y = (LeftPupilElipse.center.y+ RightPupilElipse.center.y ) /2;*/

			
			m_HeadCenter.x = HeadRoi.x + HeadRoi.width/2;
			m_HeadCenter.y = HeadRoi.y + HeadRoi.height/2;
			//m_HeadCenter = RightPupilElipse.center;
			m_bInitialized = true;
		}


	}
	//=========================================================================

};
