#include "TrackinkModel.h"
#include "PointTracker.h"
CTrackinkModel::CTrackinkModel(void)
{
	m_OrgianlPerspectiveImg  = NULL;
	m_PerspectiveImg = NULL;;
}

CTrackinkModel::~CTrackinkModel(void)
{
}



void  CTrackinkModel::Init(CPointTracker * Tracker)
{
	m_pPointTracker = Tracker;
	m_ModelScale =1;
	m_FundamentalMatrix = cvMat(3, 3, CV_64F, m_FundamentalMatrixData);
	cvZero(&m_FundamentalMatrix);
	m_FundamentalMatrixData[0] = 1;
	m_FundamentalMatrixData[4] = 1;
	m_FundamentalMatrixData[8] = 1;
	//dst_corners[0 ]=	src_corners[0] = cvPoint(FrameSize.width/3,FrameSize.height/3);
	//dst_corners[1 ]=	src_corners[1] = cvPoint(FrameSize.width -FrameSize.width/3,FrameSize.height/3);
	//dst_corners[2 ]=	src_corners[2] = cvPoint(FrameSize.width -FrameSize.width/3,FrameSize.height - FrameSize.height/3);
	//dst_corners[3 ]=	src_corners[3] = cvPoint(FrameSize.width/3,FrameSize.height - FrameSize.height/3);

	m_PerspectiveMatrix =cvCreateMat(3,3,CV_32FC1) ;

	cvZero(m_PerspectiveMatrix);

	*( (float*)CV_MAT_ELEM_PTR( *m_PerspectiveMatrix, 0, 0 ) ) =1;
	*( (float*)CV_MAT_ELEM_PTR( *m_PerspectiveMatrix, 1, 1 ) ) =1;
	*( (float*)CV_MAT_ELEM_PTR( *m_PerspectiveMatrix, 2, 2 ) ) =1;
	m_PerspectiveMatrix =cvCreateMat(3,3,CV_32FC1) ;
	m_CurrentPoints = new CvPoint2D32f[MaxFutureCount] ;
	m_NewPoints = new CvPoint2D32f[MaxFutureCount] ;

	//-------kalman-----------------


	float A[] = { 1,1, 0,1};
	float B[] = { 1, 1};


	kalman = cvCreateKalman( 2, 2, 0 );
	state = cvCreateMat( 2, 1, CV_32FC1 ); 
	process_noise = cvCreateMat( 2, 1, CV_32FC1 );
	measurement = cvCreateMat( 2, 1, CV_32FC1 );
	rng = cvRNG(-1);
	code = -1;
	cvRandArr( &rng, state, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(0.1) );

	memcpy( kalman->transition_matrix->data.fl, A, sizeof(A));
	memcpy( kalman->measurement_matrix->data.fl, B, sizeof(B));

	kalman->measurement_matrix->data.fl[0] = 1;
	kalman->measurement_matrix->data.fl[1] = 0;
	cvSetIdentity( kalman->process_noise_cov, cvRealScalar(0.0001) );
	cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(0.01) );
	cvSetIdentity( kalman->error_cov_post, cvRealScalar(1));

	memcpy( kalman->state_post->data.fl, B, sizeof(B));

}
//=============================================================}
void  CTrackinkModel::Update()
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

	int ModelPointCount = 0;
	int UsedModelPointCount = 0;



	//--------------------------errror-------------------------------------------
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrX+= (*it)->getShift().x ;
			AvrY+= (*it)->getShift().y ;
			ModelPointCount++;
		};
	}
	AvrX = AvrX / ModelPointCount;
	AvrY = AvrY / ModelPointCount;

	m_ModelCenterShiftAvr.x = AvrX;
	m_ModelCenterShiftAvr.y = AvrY;

	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrShiftErrX+= (AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x) ;
			AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) ;
		};
	};

	AvrShiftErrX = AvrShiftErrX / ModelPointCount;
	AvrShiftErrY = AvrShiftErrY / ModelPointCount;
	


for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
			if ((*it)->IsModelPoint())	
	if (  (((*it)->getShift().x - AvrX)*((*it)->getShift().x - AvrX) >100) || 
			(((*it)->getShift().y - AvrY)*((*it)->getShift().y - AvrY) >100))


		

		(*it)->m_bMoved = true;
	else
		int a = 9;

}

}

//=============================================================}

void CTrackinkModel::CheckModelGemometry()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;

	int point_count = 1000;
	CvMat points1;
	CvMat points2;
	CvMat* status;
	CvMat* fundamental_matrix;

	double *h;
	float Tmph[9];
	CvMat _h = cvMat(3, 3, CV_32FC1, Tmph);

	int 	i = 0;
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())
		{

			float dist = ((*it)->m_CurrentLocation.x - (*it)->m_PrevLocation.x  )*((*it)->m_CurrentLocation.x - (*it)->m_PrevLocation.x  )

				+ ((*it)->m_CurrentLocation.y - (*it)->m_PrevLocation.y ) * ((*it)->m_PrevLocation.y - (*it)->m_PrevLocation.y);

			if (dist<25)
			{

				m_NewPoints[i] = (*it)->m_CurrentLocation;
				m_CurrentPoints[i] = (*it)->m_PrevLocation;
				i++;}
			else
			{
				int t = 4;
			}

		}
		if( i > 20 ) break;
	};
	point_count =i;
	if (point_count < 8) 
		return;

	points1 = cvMat(1,point_count,CV_32FC2,m_CurrentPoints);
	points2 = cvMat(1,point_count,CV_32FC2,m_NewPoints);
	status = cvCreateMat(1,point_count,CV_8UC1);

	//cvInitMatHeader( points1, 1,point_count,CV_32FC2,(void*)points1->data.ptr);
	//cvInitMatHeader( points2, 1, point_count,CV_32FC2,(void*)points1->data.ptr);
	/* Fill the points here ... */


	fundamental_matrix = cvCreateMat(3,3,CV_32FC1);
	int fm_count = cvFindFundamentalMat( &points1,&points2,fundamental_matrix,
		CV_FM_RANSAC,2,CV_LMEDS,status );

	if (fm_count = 0)
	{
		int i = 9;
		return;
	};

	it = m_pPointTracker->m_vTrackingFutures.begin();

	for( i = 0; i < point_count; i++ )
	{
		//TODO
		if ( ! CV_MAT_ELEM(*status,unsigned char,0,i) ) 
		{
			while (! (*it)->IsModelPoint() ) it++;
			(*it)->m_IsLocated = false;
		};
	};
	/////////////////////////////////////

	if( !cvFindHomography(&points1,&points2, &_h, CV_RANSAC,0.01 ))
		return ;

	//cvMul( &m_FundamentalMatrix,&_h, &m_FundamentalMatrix );

	cvCopy( fundamental_matrix, m_PerspectiveMatrix );
	cvCopy( &_h, m_PerspectiveMatrix );

}






//==========================================================================
//===================================================================================================

void CTrackinkModel::BuildModel()
{
	m_ModelScale = 1;
	list<shared_ptr<CTrackingFuture>>::iterator it;

	CvPoint2D32f Center;// = cvPoint( m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2,m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2);
	Center.x =  0;
	Center.y = 0;


	int count = 0;
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			Center.x += (*it)->m_CurrentLocation.x;
			Center.y += (*it)->m_CurrentLocation.y;
			count++;
		};
	};
	Center.x = Center.x / count;
	Center.y = Center.y / count;
	//
	//Center.x =  m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2;
	//	Center.y =m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2;


	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		(*it)->setModelCenterDistans( Center,m_ModelScale);
	}
	m_ModelCenter = Center;
	m_ModelCenterShift = Center;
}


//===================================================================================================


void 	CTrackinkModel::FindModelCenterInternal()
{

	//FindModelScale();
	float ModelPointCount = 0;
	CvPoint2D32f TmpCenter ;
	CvPoint2D32f EstimateCenter ;
	TmpCenter.x = 0;
	TmpCenter.y = 0;
	EstimateCenter.x = 0;
	EstimateCenter.y = 0;

	shared_ptr<CTrackingFuture> CurrentModelFutures[500];
	int UsedModelFuturesCount =0;
	list<shared_ptr<CTrackingFuture>>::iterator it;
	float AvrCenterDistans = 0;
	float AvrCenterDistansX;
	float AvrCenterDistansY;

	//	CvPoint Center = cvPoint( m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width / 2,m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.height / 2);
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			TmpCenter.x += (*it)->getCurrentModelCenter(m_ModelScale).x;
			TmpCenter.y += (*it)->getCurrentModelCenter(m_ModelScale).y;
			ModelPointCount++;
		}
		//if (ModelPointCount > 10 ) break;
	}

	if (ModelPointCount > 1 )
	{
		TmpCenter.x = float(TmpCenter.x  / ModelPointCount);
		TmpCenter.y = float(TmpCenter.y  / ModelPointCount);
		m_ModelCenter = TmpCenter;
	}
	else
	{
		int k = 9;
		return;
	};

	// standardowe odchlenie
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrCenterDistans +=
				((*it)->getCurrentModelCenter(m_ModelScale).x  -  TmpCenter.x) * ((*it)->getCurrentModelCenter(m_ModelScale).x  -  TmpCenter.x) + 
				((*it)->getCurrentModelCenter(m_ModelScale).y  -  TmpCenter.y) * ((*it)->getCurrentModelCenter(m_ModelScale).y  -  TmpCenter.y) ; 
		};
	}

	AvrCenterDistans = AvrCenterDistans / ModelPointCount;

	if (AvrCenterDistans > 25)
	{
		int p =9;
	};

	float CenterFactor =0;
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			if (
				((*it)->getCurrentModelCenter(m_ModelScale).x  -  TmpCenter.x) * ((*it)->getCurrentModelCenter(m_ModelScale).x  -  TmpCenter.x) + 
				((*it)->getCurrentModelCenter(m_ModelScale).y  -  TmpCenter.y) * ((*it)->getCurrentModelCenter(m_ModelScale).y  -  TmpCenter.y)  <  900 *AvrCenterDistans)
			{

				EstimateCenter.x += (*it)->getCurrentModelCenter(m_ModelScale).x *(*it)->getCenterDistansFactor();
				EstimateCenter.y += (*it)->getCurrentModelCenter(m_ModelScale).y*(*it)->getCenterDistansFactor();
				CurrentModelFutures[UsedModelFuturesCount] = (*it);
				UsedModelFuturesCount++;
				CenterFactor += (*it)->getCenterDistansFactor();
			}
			else
			{

				// wkszy blod niz dozwlony
				int h = 0;
			}
		}
	}

	if ( UsedModelFuturesCount <  5) 
		return;

	EstimateCenter.x = float(EstimateCenter.x  / CenterFactor);
	EstimateCenter.y = float(EstimateCenter.y  / CenterFactor);
	m_ModelCenter = EstimateCenter;


};
//====================================================================================

void CTrackinkModel::FindModelCenter()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;
	for (int i = 0 ; i < 1 ; i++)
	{
		FindModelScale();
		FindModelCenterInternal();
	};

	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
		if ((*it)->IsModelPoint())	
		{
			(*it)->UpdateCenterDistansError(m_ModelCenter);
		}


}
//===========================================================================

void CTrackinkModel::FindModelCenterShift()
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


	float XDistans = m_pPointTracker->m_SearchNewFutureWindowSize.width/2;
	float YDistans = m_pPointTracker->m_SearchNewFutureWindowSize.width/2;

	int ModelPointCount = 0;
	int UsedModelPointCount = 0;



	//--------------------------errror-------------------------------------------
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrX+= (*it)->getShift().x ;
			AvrY+= (*it)->getShift().y ;
			ModelPointCount++;
		};
	}
	AvrX = AvrX / ModelPointCount;
	AvrY = AvrY / ModelPointCount;

	m_ModelCenterShiftAvr.x = AvrX;
	m_ModelCenterShiftAvr.y = AvrY;



	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrShiftErrX+= (AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x) ;
			AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) ;
		};
	};

	AvrShiftErrX = AvrShiftErrX / ModelPointCount;
	AvrShiftErrY = AvrShiftErrY / ModelPointCount;
	//--------------------------errror-------------------------------------------



	float MaxDistans = 0;
	float MinDistans = 1000000;
	float MaxShift = 0;
	float MinShift = 100000;


float distans;
float shift;

	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
		distans = (m_ModelShift.x - (*it)->m_CurrentLocation.x) * (m_ModelShift.x - (*it)->m_CurrentLocation.x)+ 
						(m_ModelShift.y - (*it)->m_CurrentLocation.y) * (m_ModelShift.y - (*it)->m_CurrentLocation.y);



		shift = (*it)->GetShiftError() > 0 ? 1 / ((*it)->GetShiftError()/ (*it)->m_LastFindCount) : 1;
		if(MaxDistans < distans) 
			MaxDistans =distans ;
		if(MinDistans > distans) 
			MinDistans =distans ;

		if(MaxShift < shift) 
			MaxShift =shift ;
		if(MinShift > shift) 
			MinShift =shift ;
		}

	};




	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			if (
				((AvrX - (*it)->getShift().x) * (AvrX - (*it)->getShift().x)  < ErrorFactor * AvrShiftErrX) &&
				(AvrShiftErrY+= (AvrY- (*it)->getShift().y) * (AvrY - (*it)->getShift().y) < ErrorFactor * AvrShiftErrY)
				)
			{

				tmpfactor = (XDistans*XDistans - (*it)->CheckMoveDistans(m_ModelCenterShift))*(XDistans*XDistans - (*it)->CheckMoveDistans(m_ModelCenterShift));
				tmpfactor = 1 / (((*it)->CheckMoveDistans(m_ModelCenterShift)*(*it)->CheckMoveDistans(m_ModelCenterShift)+ 1) );


						tmpfactor = 1/ (10 * distans * distans) ;

				tmpfactor = (*it)->GetShiftError() > 0 ? 1 / ((*it)->GetShiftError()/ (*it)->m_LastFindCount) : 1;

			distans = (m_ModelShift.x - (*it)->m_CurrentLocation.x) * (m_ModelShift.x - (*it)->m_CurrentLocation.x)+ (m_ModelShift.y - (*it)->m_CurrentLocation.y) * (m_ModelShift.y - (*it)->m_CurrentLocation.y);
			//distans = distans*distans;
			shift = (*it)->GetShiftError() > 0 ? 1 / ((*it)->GetShiftError()/ (*it)->m_LastFindCount) : 0;

			tmpfactor = ((MaxShift - shift)/MaxShift + (MaxDistans - distans)/MaxDistans);
			//tmpfactor = (MaxDistans - distans)/MaxDistans;
		//	tmpfactor = 10* (MaxShift - shift)/MaxShift;

				tmpfactor = 1/ (10 * distans ) ;
			//tmpfactor = 1;
			//tmpfactor = (MaxDistans - distans)/MaxDistans;

				AvrShiftX+= (*it)->getShift().x * tmpfactor;
				AvrShiftY+= (*it)->getShift().y *tmpfactor;
				factor += tmpfactor;
				UsedModelPointCount++;
			};
		};
		//TODO poprawiic 
		if (UsedModelPointCount >400 ) break;
	}

	if (factor)
	{
	AvrShiftX = AvrShiftX / factor;
	AvrShiftY = AvrShiftY / factor;
	};
	if ( UsedModelPointCount <  1) return;
	m_ModelCenterShift.x -= AvrShiftX;
	m_ModelCenterShift.y -= AvrShiftY;

	m_ModelShift.x = AvrShiftX;
	m_ModelShift.y = AvrShiftY;




	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
			(*it)->UpdateShiftError(m_ModelShift);
	}
}



//===========================================================================
void CTrackinkModel::FindModelScale()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;
	float AvrScale = 0;
	float AvrResultScale = 0;
	float AvrScaleErr = 0;
	int ModelPointCount = 0;
	int UsedModelPointCount = 0;
	double Factor = 0;
	double OldScalse;
		double CoputedScalse;

	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrScale+= (*it)->getScale(m_ModelCenterShift) ;//*(*it)->getCenterDistansFactor() ;
			ModelPointCount++;
			Factor += (*it)->getCenterDistansFactor();
		};
		//if (ModelPointCount >400 ) break;
	}
	AvrScale = AvrScale / ModelPointCount;

	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			AvrScaleErr+= (AvrScale - (*it)->getScale(m_ModelCenterShift)) * (AvrScale - (*it)->getScale(m_ModelCenterShift));//*(*it)->getCenterDistansFactor() ;
			Factor += (*it)->getCenterDistansFactor();
		};
	};

	AvrScaleErr = AvrScaleErr / ModelPointCount;
	AvrResultScale = 0;
	//ModelPointCount = 0;
	Factor = 0;
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())	
		{
			if ( (AvrScale  - (*it)->getScale(m_ModelCenterShift)) *  ( AvrScale - (*it)->getScale(m_ModelCenterShift)) <  ErrorFactor* AvrScaleErr ) 
			{
				AvrResultScale+= (*it)->getScale(m_ModelCenterShift) *(*it)->getCenterDistansFactor() ;
				UsedModelPointCount++;
				Factor += (*it)->getCenterDistansFactor();
			};
		};
		if (ModelPointCount > 400) break;
	}


	if ( UsedModelPointCount <  5) 
		return;
	OldScalse =m_ModelScale; 

	CoputedScalse = m_ModelScale = AvrResultScale / Factor;

	//cout<<"rejeect center scale  point count :  "<< ModelPointCount - UsedModelPointCount<<endl;
	//-------------kalman---------

	const CvMat* prediction = cvKalmanPredict( kalman, 0 );
	float predict_angle = prediction->data.fl[0];
	float predict_v = prediction->data.fl[1];


	measurement->data.fl[0] = m_ModelScale;
	measurement->data.fl[1] = OldScalse - m_ModelScale;
	cvKalmanCorrect( kalman, measurement );
	//kalman->
	//m_ModelScale = predict_angle;

	//cout<<"dif scale  "<< CoputedScalse - m_ModelScale<<endl;
	//cout<<"predict_angle "<< predict_angle<<endl;
	//cout<<"m_ModelScale:  "<< m_ModelScale<<endl;

}
//----------------------------------------------------------------



//==========================================================================


void  CTrackinkModel::UpdatePerspective()
{

	//cvGetAffineTransform(

	CvMat *PerspectiveMatrix = cvCreateMat(3,3,CV_32FC1);
	list<shared_ptr<CTrackingFuture>>::iterator it;

	int point_count = 1000;
	//CvMat PrevPoints = cvCreateMat(3,4,CV_32FC1);;
	//CvMat CurrentPoints = cvCreateMat(3,4,CV_32FC1);;

	int 	i = 0;
	bool fit = false;
	for( it = m_pPointTracker->m_vTrackingFutures.begin(); it != m_pPointTracker->m_vTrackingFutures.end(); it++)
	{
		if ((*it)->IsModelPoint())
		{

			fit = false;
			for (int j = 0 ; j <= i ; j++)
			{

				float dist = ((*it)->m_CurrentLocation.x - m_NewPoints[j].x  )* ((*it)->m_CurrentLocation.x - m_NewPoints[j].x  )
					+ ((*it)->m_CurrentLocation.y - m_NewPoints[j].y  )* ((*it)->m_CurrentLocation.y - m_NewPoints[j].y  );


				if (dist < 250)
					continue;

				if (   j == i  )
					fit = true;

			};
			if (fit)
			{
				m_NewPoints[i] = (*it)->m_CurrentLocation;
				m_CurrentPoints[i] = (*it)->m_PrevLocation;
				/*			m_CurrentPoints[i].x = m_NewPoints[i].x +  m_NewPoints[i].x * 0.01;
				m_CurrentPoints[i].y = m_NewPoints[i].y + 50;*/


				i++;
			};
		};

		if (i > 4) break;
	};// it;

	point_count = i;
	if (point_count < 4) 
		return;

	//cvGetAffineTransform( m_CurrentPoints, m_NewPoints, PerspectiveMatrix );

	cvGetPerspectiveTransform(m_CurrentPoints,m_NewPoints,PerspectiveMatrix);

	cvCopy(PerspectiveMatrix,m_PerspectiveMatrix);

	//cvMul( m_PerspectiveMatrix, PerspectiveMatrix ,m_PerspectiveMatrix);
	CheckModelGemometry();


}

//===================================================================================================


//==============================================================================


void  CTrackinkModel::InitPerspectiveImg()
{
	m_OrgianlPerspectiveImg = cvCloneImage(m_pPointTracker->m_GreyImg);
	m_PerspectiveImg = cvCloneImage(m_pPointTracker->m_GreyImg);

	//homograpy
	for(int  i = 0; i < 4; i++ )
	{
		CvPoint r1 = dst_corners[i%4];
		CvPoint r2 = dst_corners[(i+1)%4];
		cvLine( m_OrgianlPerspectiveImg, cvPoint(r1.x, r1.y ),
			cvPoint(r2.x, r2.y), CV_RGB(222,222,222) );
	}

}
//==============================================================================


void  CTrackinkModel::DrawPerspecitve()
{
	//m_FundamentalMatrix
	cvWarpPerspective(m_OrgianlPerspectiveImg,m_PerspectiveImg,m_PerspectiveMatrix);

	//cvWarpPerspective(m_OrgianlPerspectiveImg,m_PerspectiveImg,m_PerspectiveMatrix);

	//cvWarpAffine( m_OrgianlPerspectiveImg, m_PerspectiveImg, m_PerspectiveMatrix );
	g_GUI.ShowImage(m_PerspectiveImg, "Perspective");

}
//==============================================================================