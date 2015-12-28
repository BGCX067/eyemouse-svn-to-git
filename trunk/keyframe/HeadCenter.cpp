#include "HeadCenter.h"
#include "CTrackinkSystem.h"

CHeadCenter::CHeadCenter(void)
{
	m_bInitialized = false;
}

CHeadCenter::~CHeadCenter(void)
{
}


void CHeadCenter::FindHeadShift()
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
	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
		if (IsEyeFuter((*it).get()))	
		{
			AvrX+= ((*it))->getShift().x ;
			AvrY+= ((*it))->getShift().y ;
			ModelPointCount++;
		};
	}
	AvrX = AvrX / ModelPointCount;
	AvrY = AvrY / ModelPointCount;

	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{


		if (IsEyeFuter((*it).get()))	
		{
			AvrShiftErrX+= (AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x) ;
			AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) ;
		};
	};

	AvrShiftErrX = AvrShiftErrX / ModelPointCount;
	AvrShiftErrY = AvrShiftErrY / ModelPointCount;
	////--------------------------errror-------------------------------------------

	float distans;

	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
		//if (IsPupilFuter((*it).get()))	
		//	cvCircle( g_pTrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((*it)->m_CurrentLocation ,g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),3, CV_RGB(0,0,222), -1, 8,0);

		if (IsEyeFuter((*it).get()))
			if (
				((AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x)  < 2 *sqrt(  AvrShiftErrX)) &&
				(AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) < 2 *sqrt( AvrShiftErrY))
				)

			{

				distans = 
					tmpfactor = 1 / (m_HeadCenter.x - (*it)->m_CurrentLocation.x) * (m_HeadCenter.x - (*it)->m_CurrentLocation.x)+ 
					(m_HeadCenter.y - (*it)->m_CurrentLocation.y) * (m_HeadCenter.y - (*it)->m_CurrentLocation.y);

				tmpfactor = 1 / (distans * distans);
				tmpfactor = 1;
				AvrShiftX+= (*it)->getShift().x * tmpfactor;
				AvrShiftY+= (*it)->getShift().y *tmpfactor;
				factor += tmpfactor;
				UsedModelPointCount++;
			};

		//TODO poprawiic 
		if (UsedModelPointCount >400 ) break;
	}

	if (factor == 0)
	{
		AvrShiftX = 0;
		AvrShiftY = 0;
	}
	else
	{

		AvrShiftX = AvrShiftX / factor;
		AvrShiftY = AvrShiftY / factor;
	}
	if (( UsedModelPointCount <  3) ||( AvrShiftX > 10) ||( AvrShiftY > 10))
	{
		cout<<"take head sift"<<endl;
		m_EyeCenterSift =PointMul( m_pTracker->m_Model.m_ModelShift , m_pTracker->m_fScale);
	}
	else
	{
		m_EyeCenterSift.x = AvrShiftX;
		m_EyeCenterSift.y = AvrShiftY;
	};

	m_HeadCenter.x -=m_EyeCenterSift.x;
	m_HeadCenter.y -= m_EyeCenterSift.y;


	//cvEllipse(g_pTrackinkSystem->m_PointTracker.m_PlotResultImg,
	//	cvPointFrom32f(PointMul(m_LeftPupilElipse.center ,
	//	g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )),
	//	cvSize(m_LeftPupilElipse.size.width * g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor.x,m_LeftPupilElipse.size.height *g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor.y),
	//	m_LeftPupilElipse.angle,
	//	0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);

	//cvEllipse(g_pTrackinkSystem->m_PointTracker.m_PlotResultImg,
	//	cvPointFrom32f(PointMul(m_RightPupilElipse.center ,
	//	g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )),
	//	cvSize(m_RightPupilElipse.size.width * g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor.x,m_RightPupilElipse.size.height *g_pTrackinkSystem->m_PointTracker.m_PlotImgSizeFactor.y),
	//	m_RightPupilElipse.angle,
	//	0, 360,CV_RGB(255,0,0), 1, CV_AA, 0);
	//cvEllipse(ImgCopy, cvPointFrom32f(FirstGuaseCenter.center), cvSize(FirstGuaseCenter.size.width,FirstGuaseCenter.size.height),FirstGuaseCenter.angle, 0, 360,CV_RGB(0,0,255), 1, CV_AA, 0);


	//CVAPI(void)  cvRectangle( _pTrackinkSystem->m_PointTracker.m_PlotResultImg,
	//						 , CvPoint pt1, CvPoint pt2,
	//                          CvScalar color, int thickness CV_DEFAULT(1),
	//                          int line_type CV_DEFAULT(8),
	//                          int shift CV_DEFAULT(0));


	//g_GUI.ShowImage(g_pTrackinkSystem->m_PointTracker.m_PlotResultImg, "PupilFuters");
}