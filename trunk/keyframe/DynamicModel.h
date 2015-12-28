//#pragma once
//#include "utilitis.h"
//
//class CPointTracker;
//
//class CDynamicModel
//{
//public:
//	CDynamicModel(void);
//public:
//	~CDynamicModel(void);
//
//	CPointTracker m_pPointTracker;
//
//
//
////==========================================================================
//
//
//void UpdatePerspective()
//{
//
//	CvMat *PerspectiveMatrix = cvCreateMat(3,3,CV_32FC1);
//	list<shared_ptr<CTrackingFuture>>::iterator it;
//	int point_count = 1000;
//	//CvMat PrevPoints = cvCreateMat(3,4,CV_32FC1);;
//	//CvMat CurrentPoints = cvCreateMat(3,4,CV_32FC1);;
//
//	int 	i = 0;
//	bool fit = false;
//	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
//	{
//		if ((*it)->IsModelPoint())
//		{
//
//			fit = false;
//			for (int j = 0 ; j <= i ; j++)
//			{
//
//				float dist = ((*it)->m_CurrentLocation.x - m_NewPoints[j].x  )* ((*it)->m_CurrentLocation.x - m_NewPoints[j].x  )
//					+ ((*it)->m_CurrentLocation.y - m_NewPoints[j].y  )* ((*it)->m_CurrentLocation.y - m_NewPoints[j].y  );
//
//
//				if (dist < 250)
//					continue;
//
//				if (   j == i  )
//					fit = true;
//
//			};
//			if (fit)
//			{
//				m_NewPoints[i] = (*it)->m_CurrentLocation;
//				m_CurrentPoints[i] = (*it)->m_PrevLocation;
//				/*			m_CurrentPoints[i].x = m_NewPoints[i].x +  m_NewPoints[i].x * 0.01;
//				m_CurrentPoints[i].y = m_NewPoints[i].y + 50;*/
//				i++;
//			};
//		};
//
//		if (i > 4) break;
//	};// it;
//
//	point_count = i;
//	if (point_count < 4) 
//		return;
//
//	//cvGetAffineTransform( m_CurrentPoints, m_NewPoints, PerspectiveMatrix );
//	cvGetPerspectiveTransform(m_CurrentPoints,m_NewPoints,PerspectiveMatrix);
//	cvCopy(PerspectiveMatrix,m_PerspectiveMatrix);
//	//cvMul( m_PerspectiveMatrix, PerspectiveMatrix ,m_PerspectiveMatrix);
//	CheckModelGemometry();
//
//}
//
////===================================================================================================
//
//void BuildModel()
//{
//	m_ModelScale = 1;
//	list<shared_ptr<CTrackingFuture>>::iterator it;
//
//	CvPoint2D32f Center;// = cvPoint( m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2,m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2);
//Center.x =  m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2;
//Center.y =m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2;
//
//		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
//		{
//			if ((*it)->IsModelPoint())	
//			{
//				(*it)->setModelCenterDistans( Center);
//			}
//		}
//		m_ModelCenter = Center;
//}
//
//
////===================================================================================================
//
//void FindModelCenter()
//{
//	float ModelPointCount = 0;
//	CvPoint2D32f TmpCenter ;
//	TmpCenter.x = 0;
//	TmpCenter.y = 0;
//	list<shared_ptr<CTrackingFuture>>::iterator it;
//
//	CvPoint Center = cvPoint( m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2,m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2);
//		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
//		{
//			if ((*it)->IsModelPoint())	
//			{
//				TmpCenter.x += (*it)->getCurrentModelCenter(m_ModelScale).x;
//				TmpCenter.y += (*it)->getCurrentModelCenter(m_ModelScale).y;
//				ModelPointCount++;
//			}
//			if (ModelPointCount > 8 ) break;
//		}
//
//		if (ModelPointCount > 4 )
//		{
//TmpCenter.x = float(TmpCenter.x  / ModelPointCount);
//TmpCenter.y = float(TmpCenter.y  / ModelPointCount);
//m_ModelCenter = TmpCenter;
//		};
//
//}
////----------------------------------------------------------------
//
//
//
//
////=============================================================}
//
//
//void CheckModelGemometry()
//{
//
//	list<shared_ptr<CTrackingFuture>>::iterator it;
//
//	int point_count = 1000;
//	CvMat points1;
//	CvMat points2;
//	CvMat* status;
//	CvMat* fundamental_matrix;
//
//	double *h;
//
//	float Tmph[9];
//	CvMat _h = cvMat(3, 3, CV_32FC1, Tmph);
//	//CvMat* points1;
//	//CvMat* points2;
//	//cvInitMatHeader( points1, 1, int cols,CV_32FC2));
//
//
//	int 	i = 0;
//	for( it =                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                .begin(); it != m_vTrackingFutures.end(); it++)
//	{
//		if ((*it)->IsModelPoint())
//		{
//
//			float dist = ((*it)->m_CurrentLocation.x - (*it)->m_PrevLocation.x  )*((*it)->m_CurrentLocation.x - (*it)->m_PrevLocation.x  )
//
//				+ ((*it)->m_CurrentLocation.y - (*it)->m_PrevLocation.y ) * ((*it)->m_PrevLocation.y - (*it)->m_PrevLocation.y);
//
//			if (dist<25)
//			{
//
//				m_NewPoints[i] = (*it)->m_CurrentLocation;
//				m_CurrentPoints[i] = (*it)->m_PrevLocation;
//				/*points1->data.db[i*2] = (*it)->m_PrevLocation.x;
//				points1->data.db[i*2+1] = (*it)->m_PrevLocation.y;
//				points2->data.db[i*2] = (*it)->m_CurrentLocation.x;
//				points2->data.db[i*2+1] =  (*it)->m_CurrentLocation.y;*/
//				i++;}
//			else
//			{
//				int t = 4;
//			}
//
//
//		}
//		if( i > 20 ) break;
//	};
//	point_count =i;
//	if (point_count < 8) 
//		return;
//
//	points1 = cvMat(1,point_count,CV_32FC2,m_CurrentPoints);
//	points2 = cvMat(1,point_count,CV_32FC2,m_NewPoints);
//	status = cvCreateMat(1,point_count,CV_8UC1);
//
//	//cvInitMatHeader( points1, 1,point_count,CV_32FC2,(void*)points1->data.ptr);
//	//cvInitMatHeader( points2, 1, point_count,CV_32FC2,(void*)points1->data.ptr);
//	/* Fill the points here ... */
//
//
//	fundamental_matrix = cvCreateMat(3,3,CV_32FC1);
//	int fm_count = cvFindFundamentalMat( &points1,&points2,fundamental_matrix,
//		CV_FM_RANSAC,2,CV_LMEDS,status );
//
//	if (fm_count = 0)
//	{
//		int i = 9;
//		return;
//	};
//
//
//	it = m_vTrackingFutures.begin();
//
//	for( i = 0; i < point_count; i++ )
//	{
//		//TODO
//		if ( ! CV_MAT_ELEM(*status,unsigned char,0,i) ) 
//		{
//			while (! (*it)->IsModelPoint() ) it++;
//			(*it)->m_IsLocated = false;
//		};
//	};
//	/////////////////////////////////////
//
//
//
//	if( !cvFindHomography(&points1,&points2, &_h, CV_RANSAC,0.01 ))
//		return ;
//
//
//
//
//	//cvMul( &m_FundamentalMatrix,&_h, &m_FundamentalMatrix );
//
//	cvCopy( fundamental_matrix, m_PerspectiveMatrix );
//	cvCopy( &_h, m_PerspectiveMatrix );
//
//
//	//	CvPoint cpy_corners[4];
//	//for( i = 0; i < 4; i++ )
//	//	cpy_corners[i] = dst_corners[i];
//
//	//for( i = 0; i < 4; i++ )
//	//{
//	//	double x = src_corners[i].x, y = src_corners[i].y;
//	//	double Z = 1./(h[6]*x + h[7]*y + h[8]);
//
//	//	double X = (h[0]*x + h[1]*y + h[2])*Z;
//	//	double Y = (h[3]*x + h[4]*y + h[5])*Z;
//
//	//	dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
//	//}
//
//	//for( i = 0; i < 4; i++ )
//	//	src_corners[i] = cpy_corners[i];
//
//
//}
//
//
//
//
//
//
////==========================================================================
//
//
//
//
//};
