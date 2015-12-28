#pragma once
#include "PointTracker.h"

class CEyeCorner
{
	CPointTracker * m_pTracker;
	


public:
	CEyeCorner(void)
	{
		m_bInitialized = false;
		m_bFutureLocated = false;
	}
public:
	//~CEyeCorner(void);


	bool m_bInitialized;
	bool m_bFutureLocated;

	shared_ptr<CTrackingFuture> m_pCornerCurrentFuture;

	CvPoint2D32f m_EyeCorner;


	CvPoint2D32f m_EyeCenterSift;


	CvBox2D32f m_PupilElipse;
	CvRect m_EyeRoi;

	CvPoint2D32f m_HeadCenterDistans;

	CvPoint2D32f m_Corner;
	CvPoint2D32f m_CornerHead;
	CvPoint2D32f m_FuterPosition;



	//===================================================================
	void  Init(CPointTracker * Tracker)
	{
		m_pTracker = Tracker;
	}
	//===================================================================


	bool FindNewCornerFuture()
	{
		bool result = false;
		list<shared_ptr<CTrackingFuture>>::iterator it;
		CvRect Roi;
		CvRect PupilRoi;
		Roi.x = (m_PupilElipse.center.x + m_PupilElipse.size.width *1.2) / m_pTracker->m_fScale  ;
		Roi.y  = (m_PupilElipse.center.y  - m_PupilElipse.size.height * 0.7)/ m_pTracker->m_fScale  ;;;
		Roi.width= (m_PupilElipse.size.width * 1)/ m_pTracker->m_fScale  ;;
		Roi.height= (m_PupilElipse.size.height* 2 ) / m_pTracker->m_fScale  ;;

		
		//Roi.x = m_EyeRoi.x + m_EyeRoi.width * 0.9;
		//Roi.y  = m_EyeRoi.y  + m_EyeRoi.height * 0.25;
		//Roi.width= m_EyeRoi.width * 0.4;
		//Roi.height=m_EyeRoi.height *0.5;
	
		PupilRoi.x = m_PupilElipse.center.x ;
		PupilRoi.y  = m_PupilElipse.center.y ;
		PupilRoi.width= m_PupilElipse.size.width ;
		PupilRoi.height= m_PupilElipse.size.height;


		for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
		{
			if ((*it)->IsModelPoint())
			{
				if ((*it)->IsInRect(Roi) /*&& !(*it)->IsInRect(PupilRoi)*/)
				{
					m_pCornerCurrentFuture = (*it);
					result = true;
					m_bFutureLocated = true;
					return result;
				};

			}
		}//for
		return result;
	}
	//===================================================================
	void Update(CvBox2D32f PupilElipse, CvRect EyeRoi)
	{

		m_PupilElipse =PupilElipse;
		m_EyeRoi = EyeRoi;

		FindEyeShift();
		

		if (!m_bInitialized)
		{
			m_Corner = PupilElipse.center;
			m_CornerHead = PupilElipse.center;
			m_bInitialized = true;
		}

		m_CornerHead.x -= m_pTracker->m_Model.m_ModelShift.x  * m_pTracker->m_fScale;
		m_CornerHead.y -= m_pTracker->m_Model.m_ModelShift.y * m_pTracker->m_fScale;;


	//if 	(m_bFutureLocated)
	//{
	//	if ( (m_pCornerCurrentFuture->m_FutureState == MissingState)) 
	//	{
	//		FindNewCornerFuture();
	//		m_Corner.x -= m_pTracker->m_Model.m_ModelShift.x;
	//		m_Corner.y -= m_pTracker->m_Model.m_ModelShift.y;
	//		m_FuterPosition.x = 0;
	//		m_FuterPosition.y= 0;

	//	}
	//	else
	//	{
	//		
	//		m_Corner.x -=m_pCornerCurrentFuture->getShift().x * m_pTracker->m_fScale;;
	//		m_Corner.y -= m_pCornerCurrentFuture->getShift().y * m_pTracker->m_fScale;;

	//		m_FuterPosition.x = m_pCornerCurrentFuture->m_CurrentLocation.x * m_pTracker->m_fScale;;;
	//		m_FuterPosition.y=  m_pCornerCurrentFuture->m_CurrentLocation.y * m_pTracker->m_fScale;;;
	//	};
	//}
	//else
	//{
	//	FindNewCornerFuture();
	//	m_Corner.x -= m_pTracker->m_Model.m_ModelShift.x  * m_pTracker->m_fScale;
	//	m_Corner.y -= m_pTracker->m_Model.m_ModelShift.y * m_pTracker->m_fScale;;
	//}

	}
	//===================================================================
	void Update(CvBox2D32f PupilElipse)
	{
		CvRect EyeRoi;
		Update( PupilElipse,EyeRoi);
	}

	//===================================================================

bool IsEyeFuter(CTrackingFuture * pFuter)
{
	return (/*pFuter->IsModelPoint() &&*/
		(PointInRect(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_EyeRoi)) ||
		(!PointInElipse(PointMul(pFuter->m_CurrentLocation ,  m_pTracker->m_fScale), m_PupilElipse))) ;
				
}

//===================================================================

	void FindEyeShift()
	{	
	list<shared_ptr<CTrackingFuture>>::iterator it;
	float AvrShiftX = 0;
	float AvrShiftY = 0;
	float AvrX = 0;
	float AvrY = 0;
	float AvrShiftErrX = 0;
	float AvrShiftErrY = 0;
	float AvrShiftResultX = 0;
	float AvrShiftResultY = 0;
	float factor = 0;
	float tmpfactor = 0;


	float XDistans = m_pTracker->m_SearchNewFutureWindowSize.width/2;
	float YDistans = m_pTracker->m_SearchNewFutureWindowSize.width/2;

	int ModelPointCount = 0;
	int UsedModelPointCount = 0;



	////--------------------------errror-------------------------------------------
	//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	//{
	//	if (IsEyeFuter((*it).get()))	
	//	{
	//		AvrX+= ((*it))->getShift().x ;
	//		AvrY+= ((*it))->getShift().y ;
	//		ModelPointCount++;
	//	};
	//}
	//AvrX = AvrX / ModelPointCount;
	//AvrY = AvrY / ModelPointCount;

	//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	//{
	//	if (IsEyeFuter((*it).get()))	
	//	{
	//		AvrShiftErrX+= (AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x) ;
	//		AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) ;
	//	};
	//};

	//AvrShiftErrX = AvrShiftErrX / ModelPointCount;
	//AvrShiftErrY = AvrShiftErrY / ModelPointCount;
	////--------------------------errror-------------------------------------------




float distans;



	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
				if (IsEyeFuter((*it).get()))	
		{
	/*		if (
				((AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x)  < ErrorFactor * AvrShiftErrX) &&
				(AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) < ErrorFactor * AvrShiftErrY)
				)*/
			{
		
					//distans = (m_ModelShift.x - (*it)->m_CurrentLocation.x) * (m_ModelShift.x - (*it)->m_CurrentLocation.x)+ (m_ModelShift.y - (*it)->m_CurrentLocation.y) * (m_ModelShift.y - (*it)->m_CurrentLocation.y);


			//	tmpfactor = 1/ (10 * distans ) ;
				tmpfactor = 1;

				AvrShiftX+= (*it)->getShift().x * tmpfactor;
				AvrShiftY+= (*it)->getShift().y *tmpfactor;
				factor += tmpfactor;
				UsedModelPointCount++;
			};
		};
		//TODO poprawiic 
		if (UsedModelPointCount >400 ) break;
	}
	

	AvrShiftX = AvrShiftX / factor;
	AvrShiftY = AvrShiftY / factor;

	if ( UsedModelPointCount <  2) 
	{
		//cout<<"take head sift"<<endl;
		m_EyeCenterSift =PointMul( m_pTracker->m_Model.m_ModelShift , m_pTracker->m_fScale);
	}

	m_EyeCenterSift.x = AvrShiftX;
	m_EyeCenterSift.y = AvrShiftY;

	m_Corner.x -=m_EyeCenterSift.x;
	m_Corner.y -= m_EyeCenterSift.x;

}
	//============================================



	};
