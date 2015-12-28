




#include "HeadPositon.h"
#include "Utilitis.h"
#include "CTrackinkSystem.h"
#include "HeadPositionDebug.h"

CHeadPositon::CHeadPositon(void)
{
	m_IterationCount = 0;
	m_bPositMatrixInitialized = false;
	m_bComputeHeadPositionFirst = false;
	//m_pHeadPositionDebug = new CHeadPositionDebug;
	//m_pHeadPositionDebug->go();
}

CHeadPositon::~CHeadPositon(void)
{
}


//===================================================================================

CvPoint3D32f CHeadPositon::get3dModelCoordinates(CvPoint2D32f ImageCordinate, float headWidth, float headHeight)
{
//	float s = 0.4;
//float fheadWidth =headWidth *s;
//float fheadHeight =headHeight *s;
//	ImageCordinate.x -= headWidth*((1-s)/2.0);
//	ImageCordinate.y -= headHeight*((1-s)/2.0);

		float s = 1;
float fheadWidth =headWidth *s;
float fheadHeight =headHeight *s;
	ImageCordinate.x -= headWidth*((1-s)/2.0);
	ImageCordinate.y -= headHeight*((1-s)/2.0);


	CvPoint3D32f result;

	result.x = ( ImageCordinate.x/(1.0*fheadWidth)) - 0.5;
	result.y = ( ImageCordinate.y/(1.0*fheadHeight)) - 0.5;
	result.z = (.5*(sin(((result.x+0.5)*1.667 - .332)*3.141593) ))/1.667;
	//+ sin(((result.x+0.5))*3.141593 ) *	(.05*(sin(((result.y+0.5))*3.141593) ))/1.667  -0.05/1.667;



	result.x *= MODELSCALE;
	result.y *= MODELSCALE;
	result.z *= MODELSCALE;

	return result;

}
//===================================================================================
bool CHeadPositon::InitModel()
{
	m_fModelScale = 1.0;
	m_bPositMatrixInitialized = false;
	m_fAvrFitError = 10;
	m_IterationCount = 0;
	m_fAvrFitError = 0;
	m_bComputeHeadPositionFirst = false;
	list<shared_ptr<CTrackingFuture>>::iterator it;


	CvPoint2D32f ImageCordinate;

	int ModelPointCount = 0;


	m_InitEyesDistans = sqrt( (g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x - g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.x) *  (g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x - g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.x)
		+(g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.y - g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.y) *  (g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.y - g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.y));


	//Jako poczotkowea lokalizacje centrum akoa jest brana aktualna pozycja srodka zrenicy

	ImageCordinate.x = g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x  -g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.x;
	ImageCordinate.y =  g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.y - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.y;
	LeftEyeCenter3D = get3dModelCoordinates(ImageCordinate,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.width,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.height  );

	// todo sprawdzic jaka wartos bedzie najlepsza (experyment)

	//LeftEyeCenter3D.z /= 1.505;

	ImageCordinate.x = g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.x  -g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.x;
	ImageCordinate.y =  g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.y - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.y;
	RightEyeCenter3D = get3dModelCoordinates(ImageCordinate,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.width,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.height  );

	//RightEyeCenter3D.z /= 1.505;

	//StartLeftEyeCenter2D = g_pTrackinkSystem->LeftEyeCenter;
	//StartRightEyeCenter2D = g_pTrackinkSystem->RightEyeCenter;

	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
		(*it)->m_Model3DCounter = 0;
		(*it)->m_Model3DAvrFitErr = 0;


		if ((*it)->IsModelPoint()) 
			ModelPointCount++;
	};

	//if (ModelPointCount > 10)
	//{
	//	m_bModelInitialized = true;
	//}
	//else
	//	return;
	int count = 0;

	m_RefStartHeadPosition.x = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.x  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.width * 0.5;
	m_RefStartHeadPosition.y = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.y  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.height * 0.5;


	m_RefStartHeadPosition.x = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.x  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.width * 0.5;
	m_RefStartHeadPosition.y = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.y  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.height * 0.5;


	//m_RefStartHeadPosition.x -=  g_TrackinkSystem->m_Graber.m_pFrame->width* 0.5;
	//m_RefStartHeadPosition.y -= g_TrackinkSystem->m_Graber.m_pFrame->height* 0.5;;

	////
	//m_RefStartHeadPosition.x =  0;
	//m_RefStartHeadPosition.y =0;

	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
		//if ((!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))
		{
			count++;
			ImageCordinate.x = (*it)->m_CurrentLocation.x  - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.x;
			ImageCordinate.y = (*it)->m_CurrentLocation.y - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.y ;

			/*	if (!(*it)->m_bIsModel3D)
			continue;*/

			if (!g_pTrackinkSystem->M_FaceExtractor.m_bFaceIsDetected)
				throw("face not detected");

			//float s = 1.6;
			(*it)->m_Model3DCoordinate = get3dModelCoordinates(ImageCordinate,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.width,g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.height  );
			(*it)->m_bIsModel3D = true;
			(*it)->m_bIsToRemoveModel3D = false;

			(*it)->m_bIsToRemoveModel3D = false;
			if ((!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))
				(*it)->m_Model3DState = InModelState;
			else
				(*it)->m_Model3DState = InModelNotUseState;
			/*m_vModelPoints.push_back(   (*it)->m_Model3DCoordinate);
			m_vImagePoints.push_back((*it)->m_CurrentLocation );*/
			/*if (count > 20 )
			return;*/



		};

	};

	CheckReferencePoint(true);
	return count > 10;


}
//===================================================================
//Sprawdza czy punkt referrenyjny zostal usunieny jesli tak do ustawia nowy
void CHeadPositon::CheckReferencePoint(bool GetNew)
{
	if (m_pTracker->m_vTrackingFutures.size() < 20 ) return;
	list<shared_ptr<CTrackingFuture>>::iterator it;
	//if (m_pReferenceModelPoint == NULL)
	//;
	shared_ptr<CTrackingFuture> BestReferencePoint;
	float BestDistansToHeadCenter  = 99999999;

	float CurrentDistansToHeadCenter  = 99999999;

	CvPoint2D32f HeadCenter;
	//HeadCenter.x = 	(g_TrackinkSystem->M_FaceExtractor.m_FaceRect.x + g_TrackinkSystem->FaceCenterRoi.width/2 );
	//HeadCenter.y = 	(g_TrackinkSystem->M_FaceExtractor.m_FaceRect.y + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.height/2 );




	HeadCenter.x  = g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x + 
		(g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.x - g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x) / 2;
	HeadCenter.y  = g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.y + 
		(g_pTrackinkSystem->m_GazeDetector.m_RightPupilCenter.x - g_pTrackinkSystem->m_GazeDetector.m_LeftPupilCenter.x) /1.2;

	cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(HeadCenter,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),6, CV_RGB(0,0,0),-1, 8,0);


	//usuniento poprzedni pkt refernencyjny
	int counter = 0;

	//BestReferencePoint =  (*m_pTracker->m_vTrackingFutures.begin());
	if ((m_pReferenceModelPoint.use_count() == 0) || (m_pReferenceModelPoint->m_bIsErased) || GetNew ||
		(m_pReferenceModelPoint->m_IsLocated  == false) )
	{


		it = m_pTracker->m_vTrackingFutures.begin();
		it++;
		it++;
		it++;
		it++;
		it++;
		it++;
		for( ; it != m_pTracker->m_vTrackingFutures.end(); it++)
		{	
			counter++;
			//if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint() &&(!(*it)->m_bIsToRemoveModel3D) 
			//	//&&(!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get()))
			if ( ((*it)->m_Model3DState != InModelNotUseState)
				//&&(!(*it)->m_bBadFit)
				)	
			{
				CurrentDistansToHeadCenter = ((*it)->m_CurrentLocation.x - HeadCenter.x)*((*it)->m_CurrentLocation.x - HeadCenter.x) + 
					((*it)->m_CurrentLocation.y - HeadCenter.y)*((*it)->m_CurrentLocation.y - HeadCenter.y) ;
				//counter * g_TrackinkSystem->FaceCenterRoi.width /50;		

				if (CurrentDistansToHeadCenter < BestDistansToHeadCenter)
				{
					BestDistansToHeadCenter = CurrentDistansToHeadCenter ;
					BestReferencePoint = (*it);
					//break; // bierze pierwszy
				};
			};
		};//for


		//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
		//{	
		//	counter++;
		//if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint() &&(!(*it)->m_bIsToRemoveModel3D) 
		//		&&(!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get()))
		//		&& ((*it)->m_Model3DState != InModelNotUseState)
		//		&&(!(*it)->m_bBadFit))	
		//	{
		//		CurrentDistansToHeadCenter = ((*it)->m_CurrentLocation.x - HeadCenter.x)*((*it)->m_CurrentLocation.x - HeadCenter.x) + 
		//			((*it)->m_CurrentLocation.y - HeadCenter.y)*((*it)->m_CurrentLocation.y - HeadCenter.y) ;
		//		//counter * g_TrackinkSystem->FaceCenterRoi.width /50;		

		//		if (CurrentDistansToHeadCenter < BestDistansToHeadCenter)
		//		{
		//			BestDistansToHeadCenter = CurrentDistansToHeadCenter ;
		//			BestReferencePoint = (*it);
		//			break;
		//			//break; // bierze pierwszy
		//		};
		//	};
		//};//for



		//delete m_pReferenceModelPoint;
		//m_pReferenceModelPoint.
		m_pReferenceModelPoint  = BestReferencePoint;
		m_RefPoint = m_pReferenceModelPoint->m_Model3DCoordinate;
	}//if erased


	if (g_TrackinkSystem->m_bKeyChangeRef3DPoint)
	{

		int index = rand()%m_pTracker->m_vTrackingFutures.size();
		it = m_pTracker->m_vTrackingFutures.begin();
		for (int i = 0; i<index; i++)
			it++;
		m_pReferenceModelPoint = (*it);
		m_RefPoint = m_pReferenceModelPoint->m_Model3DCoordinate;


		//m_RefStartHeadPosition.x = m_pReferenceModelPoint->m_CurrentLocation.x;
		//m_RefStartHeadPosition.y = m_pReferenceModelPoint->m_CurrentLocation.y;

		//m_RefStartHeadPosition.x -=  g_TrackinkSystem->m_Graber.m_pFrame->width* 0.5;
		//m_RefStartHeadPosition.y -= g_TrackinkSystem->m_Graber.m_pFrame->height* 0.5;;

	};

}

//===================================================================
bool  CHeadPositon::Update(bool flag )
{
if (m_pTracker->m_vTrackingFutures.size() < 20 )
		throw("too small amount of tracking futers");
	list<shared_ptr<CTrackingFuture>>::iterator it;

	m_vModelPoints.clear();
	m_vImagePoints.clear();

	CvPoint3D32f tmpPoint;
	CheckReferencePoint();

	m_vModelPoints.push_back( m_pReferenceModelPoint->m_Model3DCoordinate);
	m_vImagePoints.push_back( m_pReferenceModelPoint->m_CurrentLocation);

	int NotUsedFuterCount = 0;
	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{	
		// starre punkty

		////todo : rzkminic dlaczego zakomentowanie tego powoduje inne inicjowanie modelu
		//if (g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get()))
		//{
		//	(*it)->m_Model3DState = InModelNotUseState;
		//	//(*it)->m_bIsModel3D = false;
		//	continue;
		//};

		/*		else
		if ((*it)->m_Model3DState == InModelNotUseState)
		((*it)->m_Model3DState = FitErrorState*/;

	/*if (m_bPositMatrixInitialized)
		{
			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);


			double pointToKey = sqrt(((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x )* ((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x ) +
				((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y )* ((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y ) );

		
				double pointToHeadCenter = PointDistans((*it)->m_CurrentLocation ,ProjectTo2D( m_CurrentHeadCenter3D));

			if (pointToHeadCenter >   m_InitEyesDistans * m_fModelScale * 8 ) 
				if (pointToKey >   m_InitEyesDistans * m_fModelScale  )
			{
					(*it)->m_CurrentLocation =  (*it)->m_TransformKeyFrameLocation;
					(*it)->m_bLocationCorected = true;
					(*it)->m_Model3DState = OutLintersState;
				};
	};*/



	if ((*it)->m_bBadFit)
	{
		if (m_bPositMatrixInitialized)
		{

			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);


			double pointToKey = sqrt(((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x )* ((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x ) +
				((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y )* ((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y ) );

			// todo treshold zalezny od scali

			if (pointDistance >   m_InitEyesDistans * m_fModelScale  ) 
				if (pointToKey >   m_InitEyesDistans * m_fModelScale  )
		//	if (pointDistance >  RANSAC_DISTANCE_THRESHOLD)
			//	if (pointToKey >  RANSAC_DISTANCE_THRESHOLD)
				{
				 
					(*it)->m_CurrentLocation = CurrentPoint;/*(*it)->m_TransformKeyFrameLocation;*/
					(*it)->m_bLocationCorected = true;
					(*it)->m_Model3DState = OutLintersState;
				};


			double KeyToModel = PointDistans((*it)->m_TransformKeyFrameLocation,CurrentPoint);

			if(KeyToModel <  m_InitEyesDistans * m_fModelScale * 0.05 )
			if (pointDistance >   m_InitEyesDistans * m_fModelScale * 0.2 ) 
				if (pointToKey >   m_InitEyesDistans * m_fModelScale* 0.2   )
				{
				 
					(*it)->m_CurrentLocation = CurrentPoint/*(*it)->m_TransformKeyFrameLocation*/;
					(*it)->m_bLocationCorected = true;
					(*it)->m_Model3DState = OutLintersState;
				};


		///distans to center
	


		};
	}
	else
	{
	
	if (m_bPositMatrixInitialized)
		{

			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);


			double pointToKey = sqrt(((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x )* ((*it)->m_CurrentLocation.x  - (*it)->m_TransformKeyFrameLocation.x ) +
				((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y )* ((*it)->m_CurrentLocation.y  - (*it)->m_TransformKeyFrameLocation.y ) );

			// todo treshold zalezny od scali

			if (pointDistance >   m_InitEyesDistans * m_fModelScale  * 2.0) 
				if (pointToKey >   m_InitEyesDistans * m_fModelScale *  2.0)
		//	if (pointDistance >  RANSAC_DISTANCE_THRESHOLD)
			//	if (pointToKey >  RANSAC_DISTANCE_THRESHOLD)
				{
				 ;
					(*it)->m_CurrentLocation =  CurrentPoint /*(*it)->m_TransformKeyFrameLocation*/;
					(*it)->m_bLocationCorected = true;
					(*it)->m_Model3DState = OutLintersState;
				};


		///distans to center
	


		};
}

CvPoint2D32f CurrentPoint;
	double pointDistance = 0;

		if (m_bPositMatrixInitialized)
		{
		
				 CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
				double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
				double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
				 pointDistance = sqrt(dx*dx+dy*dy);

		};//m_bPositMatrixInitialized

	/*	if ((*it)->m_Model3DState != InModelNotUseState && (*it)->m_bIsModel3D)
			if (pointDistance < RANSAC_DISTANCE_THRESHOLD)*/

// if (pointDistance < m_InitEyesDistans * m_fModelScale  )

				if ((*it)->m_Model3DState == InModelState)
			{
				tmpPoint.x = (*it)->m_Model3DCoordinate.x  ; 
				tmpPoint.y = (*it)->m_Model3DCoordinate.y  ; 
				tmpPoint.z = (*it)->m_Model3DCoordinate.z ; 

				m_vModelPoints.push_back( tmpPoint);
				m_vImagePoints.push_back((*it)->m_CurrentLocation );
			}
				else
				{
				(*it)->m_CurrentLocation =   (*it)->m_TransformKeyFrameLocation;
					(*it)->m_bLocationCorected = true;
				
				}

	

	};//for





	if (m_vModelPoints.size()<4)
	{
		m_bModelInitialized = false;
		return false;
	};


	if(m_vModelPoints.size()>4)
	{
		//if (!m_bPositMatrixInitialized )
		{

			//cout<<"Used points: "<<m_vModelPoints.size()<<endl;
			CvPOSITObject *positObject = cvCreatePOSITObject( &m_vModelPoints[0], static_cast<int>(m_vModelPoints.size()) );
			CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
			cvPOSIT( positObject, &m_vImagePoints[0], EHCIFOCUS, criteria, m_RotationMatrix, m_TransactionMatrix );
			cvReleasePOSITObject (&positObject);


			//------------key frame transform update-------------------------
			if (!m_bPositMatrixInitialized)
			{
				//cout<<"first posit gues"<<endl;

				// inicjalizacja modelu po raz pierwszy odpalnoy zostal posit
				CvPoint3D32f ModelHeadCenter3D;
				CvPoint3D32f CurrentHeadCenter3D;
				ModelHeadCenter3D.x = 0;
				ModelHeadCenter3D.y = 0;
				ModelHeadCenter3D.z = 0;
				m_fModelScale = 1;

				m_ModelRefHeadCenter3D  = GetTransformPoint(ModelHeadCenter3D); 

				m_KeyFrameTransformBaseP1 = ProjectTo2D(m_KeyFrameTransformModelP1 );;
				m_KeyFrameTransformBaseP2= ProjectTo2D(m_KeyFrameTransformModelP2 );;
				m_KeyFrameTransformBaseP3= ProjectTo2D(m_KeyFrameTransformModelP3);;
				m_KeyFrameTransformBaseP4= ProjectTo2D(m_KeyFrameTransformModelP4 );;
			};

			m_KeyFrameTransformCurrentP1 =ProjectTo2D(m_KeyFrameTransformModelP1 );;
			m_KeyFrameTransformCurrentP2= ProjectTo2D(m_KeyFrameTransformModelP2 );;
			m_KeyFrameTransformCurrentP3= ProjectTo2D(m_KeyFrameTransformModelP3);;
			m_KeyFrameTransformCurrentP4= ProjectTo2D(m_KeyFrameTransformModelP4 );;

			//------------end key frame transform update-------------------------
			m_bPositMatrixInitialized = true;
		}
		//else



		//----------------scale compute---------------------------
		CvPoint3D32f ModelHeadCenter3D;

		ModelHeadCenter3D.x = 0;
		ModelHeadCenter3D.y = 0;
		ModelHeadCenter3D.z = 0;
		m_CurrentHeadCenter3D = GetTransformPoint(ModelHeadCenter3D);

		//m_fModelScale = m_fModelScale * 0.5 + 0.5* m_ModelRefHeadCenter3D.z / CurrentHeadCenter3D.z;
		m_fModelScale =  m_ModelRefHeadCenter3D.z / m_CurrentHeadCenter3D.z;
		//cout<<"-------model Scale: "<<m_fModelScale<<endl;

		if (!m_bComputeHeadPositionFirst)


			//----------------end scale compute---------------------------


			//if ( !Ransac())
			//{


			//	CvPOSITObject *positObject = cvCreatePOSITObject( &m_vModelPoints[0], static_cast<int>(m_vModelPoints.size()) );
			//	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
			//	cvPOSIT( positObject, &m_vImagePoints[0], EHCIFOCUS, criteria, m_RotationMatrix, m_TransactionMatrix );
			//	cvReleasePOSITObject (&positObject);
			//	m_bPositMatrixInitialized = true;
			//};
			m_bPositMatrixComputed = true;
		//cout<<"model point count:" << m_vModelPoints.size()<<endl;
		//cout<<"Reference:" << m_RefPoint.x << " : "<< m_RefPoint.y << " : "<<  m_RefPoint.z << " : " <<endl;


		////////////////////

		//if (flag)
		//{
		//	if (RemoveWrongPoint() > 0)
		//	{
		//		Update(false);
		//		return;
		//	};
		//}





		////-----------------update punktow-----------------------------
		//float AllAvrFitError = 0;
		//int modelPointCount = 0;
		//for( it =  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it !=  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
		//{
		//	//if (((*it)->m_bIsModel3D) )	

		//	if ((*it)->m_Model3DState != NotInModelState)
		//	{
		//		CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
		//		double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
		//		double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
		//		double pointDistance = sqrt(dx*dx+dy*dy);

		//		int index = (*it)->m_Model3DCounter %Model3DFitHistoryCount;
		//		if ((*it)->m_Model3DCounter == 0)
		//			(*it)->m_Model3DAvrFitErr = pointDistance;
		//		else
		//		if ((*it)->m_Model3DCounter < Model3DFitHistoryCount)
		//			(*it)->m_Model3DAvrFitErr = (*it)->m_Model3DAvrFitErr * (*it)->m_Model3DCounter / ((*it)->m_Model3DCounter + 1) + pointDistance *(1  / ((*it)->m_Model3DCounter + 1) );
		//		else
		//			(*it)->m_Model3DAvrFitErr = (*it)->m_Model3DAvrFitErr * Model3DFitHistoryCount / (Model3DFitHistoryCount + 1) + pointDistance *(1  / (Model3DFitHistoryCount + 1) );


		//		//(*it)->m_vModel3DFitHistory[index] = pointDistance ;
		//		(*it)->m_Model3DCounter++;


		//		//sredni bload
		//		if ( (*it)->m_Model3DState  == InModelState)
		//		{
		//			AllAvrFitError += pointDistance;
		//			modelPointCount++;
		//		}

		//		//nowe punkty
		//		if ( (*it)->m_Model3DState  == NewPointState)
		//		{
		//		if ((*it)->m_Model3DCounter >Model3DFitHistoryCount  )
		//		{
		//			if  ((*it)->m_Model3DAvrFitErr < 0.4 * m_fAvrFitError ||(*it)->m_Model3DAvrFitErr < 0.2 )
		//				(*it)->m_Model3DState = InModelState;
		//			else
		//			{
		//				(*it)->m_Model3DState  == FitErrorState;
		//				(*it)->m_bIsToRemoveModel3D = true;
		//			}

		//		}
		//			
		//		};


		//		//przywracanie punktow o zbyt duzym bledzie
		//		if ( ((*it)->m_Model3DState  == FitErrorState) && (!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))
		//		{
		//		if (( (pointDistance < m_fAvrFitError)||(pointDistance  < 2))  && (((*it)->m_Model3DAvrFitErr < m_fAvrFitError)||((*it)->m_Model3DAvrFitErr < 2)))
		//			(*it)->m_Model3DState = InModelState;

		//		if(((*it)->m_Model3DAvrFitErr > 10 *m_fAvrFitError) &&((*it)->m_Model3DAvrFitErr > 5))
		//			(*it)->m_bIsToRemoveModel3D = true;

		//		};


		//		//usuwanie punktow za bardzo odstajacych
		//		if ((pointDistance > RANSAC_DISTANCE_THRESHOLD) )
		//		{
		//			(*it)->m_bIsToRemoveModel3D = true;
		//			(*it)->m_Model3DState  == FitErrorState;
		//		};

		//	}
		//}//for

		//AllAvrFitError = AllAvrFitError / modelPointCount;
		//if (m_IterationCount == 0)
		//	m_fAvrFitError = AllAvrFitError;
		//else
		//if ((m_IterationCount < Model3DFitHistoryCount))
		//	m_fAvrFitError = m_fAvrFitError * m_IterationCount / (m_IterationCount + 1) + AllAvrFitError *(1  / (m_IterationCount + 1) );
		//else
		//	m_fAvrFitError = m_fAvrFitError * Model3DFitHistoryCount / (Model3DFitHistoryCount + 1) + AllAvrFitError *(1 / (Model3DFitHistoryCount + 1) );


		//--------------------------------------------------

		ComputeHeadPosition();

		if (g_pTrackinkSystem->m_bKeyDebugPlot)
			Plot();


	}

	//CorrectPointsToModelPosition();

	//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	//{	
	//	// nowe punkty do modelu
	//	if(m_bPositMatrixComputed)
	//	{
	//	//	if ((*it)->IsModelPoint() && (!(*it)->m_bIsModel3D) &&(!(*it)->m_bIsToRemoveModel3D) &&(!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))			
	//	if (((*it)->m_Model3DState == NotInModelState)&&(!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))
	//		{
	//			(*it)->m_Model3DCoordinate = get3dFrom2D((*it)->m_CurrentLocation);
	//			(*it)->m_bIsModel3D = true;
	//			(*it)->m_Model3DState = NewPointState;

	//		};
	//	};
	//};
	m_IterationCount ++;
	return true;
}

//
//===================================================================
bool CHeadPositon::Ransac()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;
	std::vector<CvPoint3D32f> TestModelPoints;
	std::vector<CvPoint2D32f> TestImagePoints;

	std::vector<CvPoint3D32f> BestSetModelPoints;
	std::vector<CvPoint2D32f> BestSetImagePoints;


	std::vector<int> BestSetPointsIndexs;

	std::vector<int> CurrentSetPointsIndexs;



	CvMatr32f BestRotationMatrix = new float[9];
	CvVect32f BestTranslationVector = new float[3];


	CvMatr32f rotationTestMatrix = new float[9];
	CvVect32f translationTestVector = new float[3];


	int bestSize = 0;

	double distans;
	double errorSum;
	int PointCount = 0;
	int BestSetPointCount = -10000000;
	double BestSetErrorSum = 99999999;


	CvPoint3D32f RefPoint = m_RefPoint;

	double TresholdDistans =  2* getDistanceThreshold();

	if (m_vModelPoints.size() <RANSAC_SAMPLES )
		return false;

	for(int k=0;k<RANSAC_ITERATIONS;k++)
	{
		TestModelPoints.clear();
		TestImagePoints.clear();
		//add first reference point
		TestModelPoints.push_back(m_vModelPoints[0]);
		TestImagePoints.push_back(m_vImagePoints[0]);

		for(int z = 0 ; z < RANSAC_SAMPLES ; z++)
		{
			int index = rand()%m_vModelPoints.size();
			TestModelPoints.push_back(m_vModelPoints[index]);
			TestImagePoints.push_back(m_vImagePoints[index]);
		};


		CvPOSITObject *positObject = cvCreatePOSITObject( &TestModelPoints[0], static_cast<int>(TestModelPoints.size()) );
		CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
		cvPOSIT( positObject, &TestImagePoints[0], EHCIFOCUS, criteria, rotationTestMatrix, translationTestVector );
		cvReleasePOSITObject (&positObject);


		PointCount = 0;
		errorSum = 0;
		CurrentSetPointsIndexs.clear();
		for(int i = 0; i< m_vModelPoints.size(); i++)
		{	
			CvPoint2D32f CurrentPoint =  _ProjectTo2D(m_vModelPoints[i], RefPoint,	rotationTestMatrix ,translationTestVector );
			double dx = CurrentPoint.x - m_vImagePoints[i].x ;
			double dy = CurrentPoint.y - m_vImagePoints[i].y ;
			double pointDistance = sqrt(dx*dx+dy*dy);
			errorSum+=pointDistance;

			//if(pointDistance <RANSAC_DISTANCE_THRESHOLD)//threshold){//
			//if(pointDistance <	TresholdDistans)

			if (/*(pointDistance < m_fAvrFitError) ||(pointDistance > 2 )||*/(pointDistance <	TresholdDistans))



			{
				CurrentSetPointsIndexs.push_back(i);
				PointCount++;
			}

		};// for

		if (  BestSetPointCount /*- BestSetErrorSum *0.01*/ <= PointCount /*- errorSum*0.01*/)
		{

			if (BestSetPointCount ==  PointCount )
				if (BestSetErrorSum <= errorSum)
					continue;
			BestSetErrorSum = errorSum;
			BestSetPointCount = PointCount;

			BestSetPointsIndexs.clear();

			for(int j=0; j<CurrentSetPointsIndexs.size(); j++)
				BestSetPointsIndexs.push_back(CurrentSetPointsIndexs[j]);
			//BestSetPointsIndexs[j] = CurrentSetPointsIndexs[j];
			/*		for(int j = 0 ; j <9 ; j++)
			BestRotationMatrix[j] = rotationTestMatrix[j];
			for(int j = 0 ; j <3 ; j++)
			BestTranslationVector[j] = translationTestVector[j];*/
		};

	};//	for(int k=0;k<RANSAC_ITERATIONS;k++)



	if(BestSetPointsIndexs.size() < 4) return false;

	if (BestSetPointsIndexs.size() < m_vModelPoints.size())

		cout<<"rans delete :"<< m_vModelPoints.size() - BestSetPointsIndexs.size()  << endl;
	for(int j = 0 ; j <BestSetPointsIndexs.size() ; j++)
	{
		BestSetModelPoints.push_back(m_vModelPoints[BestSetPointsIndexs[j]]);
		BestSetImagePoints.push_back(m_vImagePoints[BestSetPointsIndexs[j]]);
	};


	CvPOSITObject *positObject = cvCreatePOSITObject( &BestSetModelPoints[0], static_cast<int>(BestSetModelPoints.size()) );
	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
	cvPOSIT( positObject, &BestSetImagePoints[0], EHCIFOCUS, criteria, m_RotationMatrix, m_TransactionMatrix );
	cvReleasePOSITObject (&positObject);



	int BestSetIndex = 0;

	it = m_pTracker->m_vTrackingFutures.begin();
	for(int j = 0 ; j <BestSetPointsIndexs.size() ; j++)
	{

		while( BestSetPointsIndexs[j] != BestSetIndex)
		{
			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);

			if ((pointDistance >0.5 * m_fAvrFitError) && (pointDistance > 2))
			{
				(*it)->m_Model3DState = FitErrorState;
			};
			it++;
			BestSetIndex++;	
		};
		it++;
		BestSetIndex++;	
	};//for




	//for(int j = 0 ; j <9 ; j++)
	//	m_RotationMatrix[j] = BestRotationMatrix[j];
	//for(int j = 0 ; j <3 ; j++)
	//	m_TransactionMatrix[j] = BestTranslationVector[j];

	return true;
}

//===================================================================
void CHeadPositon::Plot()
{

	list<shared_ptr<CTrackingFuture>>::iterator it;


	int d = 1000;
	CvPoint3D32f point;
	CvPoint3D32f point2;
	point.x = 0;
	point.y = 0;
	point.z = 0;
	g_TrackinkSystem->m_HeadCenterPoint = ((ProjectTo2D(point)));



	//------------------------------siatka---------------------------------------------
	// estimatet eye center na posit plot
	cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(EstimateLeftEyeCenter2D,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(100,100,0), 2, 8,0);



	for(int j =0 ; j<= 5 ; j++)
		for(int i =0 ; i<= 100 ; i++)
		{
			float fx,fy,fz;
			CvPoint2D32f ImageCordinate;
			ImageCordinate.x = i;
			ImageCordinate.y = j;
			point =  get3dModelCoordinates(ImageCordinate,100, 5);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(EstimateRightEyeCenter2D,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(100,100,0), 2, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);
		}


		for(int j =0 ; j<= 100 ; j++)
			for(int i =0 ; i<= 5 ; i++)
			{
				float fx,fy,fz;
				CvPoint2D32f ImageCordinate;
				ImageCordinate.x = i;
				ImageCordinate.y = j;
				point =  get3dModelCoordinates(ImageCordinate,5, 100);
				cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(111,111,100), 0, 8,0);
				cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);

			}


			//------------------osie-----------------------------------------------
			for(int j =-d ; j< d ; j++)
			{
				point.x =j;
				point.y =0;
				point.z =0;
				cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,0,255), 0, 8,0);
				cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )), 1, CV_RGB(0,0,255), 0, 8,0);

			}
			for(int j =-d ; j< d ; j++)
			{
				point.x =0;
				point.y =j;
				point.z =0;
				cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(250,0,0), 0, 8,0);
				cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )), 1,  CV_RGB(250,0,0), 0, 8,0);
			}
			for(int j =-d ; j< 4*d ; j++)
			{
				point.x =0;
				point.y =0;
				point.z =j;
				cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,(j*255/4*d),0), 0, 8,0);
				cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor )), 1,  CV_RGB(0,(j*255/4*d),0), 0, 8,0);
			}


			//-----------------rotation vector-----------------------------------------------

			point.x =0;
			point.y =0;
			point.z =0;

			point2.x =m_rotVect[0] * 1000;
			point2.y =m_rotVect[1] * 1000;
			point2.z =m_rotVect[2] * 1000;


			cvLine(g_TrackinkSystem->m_Graber.m_pFrame,cvPointFrom32f(ProjectTo2D(point2) ),
				cvPointFrom32f(ProjectTo2D(point) ), CV_RGB(0,0,0),1);;




			EstimateLeftEyeCenter2D = ProjectTo2D(LeftEyeCenter3D);
			EstimateRightEyeCenter2D = ProjectTo2D(RightEyeCenter3D);
			//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(LeftEyeCenter3D)), 1, CV_RGB(0,0,200), 3, 8,0);
			//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(RightEyeCenter3D)), 1, CV_RGB(0,0,200), 3, 8,0);




			/*	cvCircle( g_TrackinkSystem->m_Graber.m_pFrame,cvPointFrom32f( StartLeftEyeCenter2D), 1, CV_RGB(222,0,200), 3, 8,0);
			cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(StartRightEyeCenter2D), 1, CV_RGB(222,0,200), 3, 8,0);*/


			//g_GUI.ShowImage(g_TrackinkSystem->m_Graber.m_pFrame, "posit result");




			for( it =  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it !=  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
			{


				//if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint())	
				{


					if ( (*it)->m_Model3DState  != NotInModelState)
					{
						//cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((*it)->m_CurrentLocation,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(222,0,0), 2, 8,0);
						cvLine(g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((*it)->m_CurrentLocation,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),
							cvPointFrom32f(PointMul((ProjectTo2D((*it)->m_Model3DCoordinate)),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ), CV_RGB(222,222,222),1);;
						cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((ProjectTo2D((*it)->m_Model3DCoordinate)),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(222,222,222), 2, 8,0);
					}
					// nowe punkty

					/*		if ( (*it)->m_Model3DState  == NewPointState)
					cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((ProjectTo2D((*it)->m_Model3DCoordinate)),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(0,222,0), 2, 8,0);


					if ( (*it)->m_Model3DState  == FitErrorState)
					cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((ProjectTo2D((*it)->m_Model3DCoordinate)),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),1, CV_RGB(0,100,100), 2, 8,0);

					*/
					if((*it)->m_bLocationCorected )
						cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((*it)->m_CurrentLocation,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),4, CV_RGB(220,0,222), 2, 8,0);


					// usuniente punkty
					if((*it)->m_bIsToRemoveModel3D)
						cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul((ProjectTo2D((*it)->m_Model3DCoordinate)),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),4, CV_RGB(220,0,222), 2, 8,0);


				};
			}


			// estimatet eye center na posit plot
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(EstimateLeftEyeCenter2D,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),4, CV_RGB(200,200,0), 1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(EstimateRightEyeCenter2D,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),4, CV_RGB(200,200,0), 1, 8,0);


			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeCenter,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(220,222,0), 1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->M_FaceExtractor.m_RightEyeCenter,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(220,222,0), 1, 8,0);



			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,0), -1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,0), -1, 8,0);


			//Key frame points

			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP1,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP2,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP3,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP4,g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);


			//Referenc point

			cvCircle( g_TrackinkSystem->m_PointTracker.m_PlotResultImg,cvPointFrom32f(PointMul(ProjectTo2D(m_RefPoint),g_TrackinkSystem->m_PointTracker.m_PlotImgSizeFactor ) ),6, CV_RGB(220,222,0), 4, 8,0);

			g_GUI.ShowImage(g_TrackinkSystem->m_PointTracker.m_PlotResultImg, "posit");

}

//===================================================================
int CHeadPositon::RemoveWrongPoint()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;
	int result = 0;
	double TresholdDistans = getDistanceThreshold();

	for( it =  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it !=  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
	{

		if (m_pReferenceModelPoint == (*it))
		{
			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);
			if ((pointDistance > 2* TresholdDistans) && (pointDistance > 1) )
			{
				//	std::cout<<" reference point error removed point 3d"<<std::endl;

			};
			continue;
		};


		//if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint() )	
		if ((*it)->m_Model3DState == InModelState)
		{

			CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);


			if ((pointDistance > m_fAvrFitError) && (pointDistance > 2)
				&&
				(pointDistance > 2* TresholdDistans))
			{
				//(*it)->m_bIsToRemoveModel3D = true;
				(*it)->m_Model3DState = FitErrorState;
				result++;
				//std::cout<<"removed point 3d"<<std::endl;
			};

			//if ((pointDistance > 3* TresholdDistans) && (pointDistance > 1) )
			//{
			//	//(*it)->m_bIsToRemoveModel3D = true;
			//	(*it)->m_Model3DState = FitErrorState;
			//	result++;
			//	//std::cout<<"removed point 3d"<<std::endl;
			//};;
			//else


			//	if ((pointDistance > RANSAC_DISTANCE_THRESHOLD) )
			//	{
			//		(*it)->m_bIsToRemoveModel3D = true;
			//		result++;
			//		//	std::cout<<"removed point 3d less then treshold"<<std::endl;
			//	};

			//if (g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get()))
			//{
			//	(*it)->m_bIsToRemoveModel3D = true;
			//	//	std::cout<<"removed point 3d in eye region"<<std::endl;
			//}



		};
	};
	return result;
}

//===================================================================
double CHeadPositon::getDistanceThreshold()
{
	vector<double> distanceErrors;
	double errorSum = 0;

	CvPoint2D32f CurrentPoint;
	list<shared_ptr<CTrackingFuture>>::iterator it;

	int ModelPointCount = 0;

	for( it =  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it !=  g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
	{


		//if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint()  &&(!(*it)->m_bIsToRemoveModel3D))	
		if ((*it)->m_Model3DState == InModelState)
		{
			ModelPointCount++;
			CvPoint2D32f CurrentPoint =  ProjectTo2D((*it)->m_Model3DCoordinate );
			double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
			double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
			double pointDistance = sqrt(dx*dx+dy*dy);
			errorSum+=pointDistance;
			distanceErrors.push_back(pointDistance);

		}
	}

	double deviations = 0;
	double meanError = errorSum/ModelPointCount;
	for(int i=0;i<ModelPointCount;i++){
		deviations+=(distanceErrors[i]-meanError)*(distanceErrors[i]-meanError);
	}
	//printf("Deviations %lf numpts %d mean error %lf errorSum %lf\n",deviations, NUMPTS,meanError,errorSum);
	double sigma = sqrt(deviations/ModelPointCount);

	return sqrt(5.99 * sigma*sigma);

}

//===================================================================
bool  CHeadPositon::CheckNewEstimation(CvPoint2D32f &ImageCordinate,CvPoint2D32f &Current2D,double & bestError,CvPoint3D32f &BestEstimated3D , bool x)
{
	CvPoint3D32f CurentGues ;
	CvPoint2D32f Estimated2D;
	CvPoint3D32f Estimated3D;
	double Error;
	CurentGues = get3dModelCoordinates(Current2D, 1, 1);
	Estimated2D = ProjectTo2D(CurentGues );

	if (x)
		Error = sqrt((Estimated2D.x - ImageCordinate.x)*(Estimated2D.x - ImageCordinate.x));
	else
		Error = sqrt( (Estimated2D.y - ImageCordinate.y)*(Estimated2D.y - ImageCordinate.y));

	if(Error <  bestError) // czy poprawa 
	{
		bestError = Error;
		BestEstimated3D = CurentGues;
		return true;
	}
	else
		return false;
}
//===================================================================
CvPoint3D32f CHeadPositon::get3dFrom2D(CvPoint2D32f ImageCordinate)
{
	CvPoint3D32f result;

	CvPoint2D32f Current2D = ImageCordinate;

	CvPoint3D32f CurentGues ;
	double Error;
	double BestError = 99999999999;

	CvPoint3D32f BestEstimated3D;


	for (float x = 0 ; x < 1 ; x+= 0.0001)
	{
		Current2D.x = x;
		Current2D.y = 0;
		if (!CheckNewEstimation(ImageCordinate, Current2D,  BestError,BestEstimated3D,true ))
			break;
		/*	if ( BestError )
		break;*/
	};

	BestError = 99999999999;
	for (float y = 0 ; y < 1 ; y+= 0.0001)
	{
		//Current2D.x = x;
		Current2D.y =y;
		if (!CheckNewEstimation(ImageCordinate, Current2D,  BestError,BestEstimated3D,false))
			break;
		/*break;*/
	}
	return BestEstimated3D;
}//===================================================================

//===================================================================
void CHeadPositon::CorrectPointsToModelPosition()
{


	list<shared_ptr<CTrackingFuture>>::iterator it;


	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{	
		// starre punkty
		if (((*it)->m_bIsModel3D)&&(*it)->IsModelPoint() &&(!(*it)->m_bIsToRemoveModel3D) &&(!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))	
		{
			(*it)->m_CurrentLocation  = ProjectTo2D((*it)->m_Model3DCoordinate);

		};
	};//for
}




//===================================================================
void CHeadPositon::ComputeHeadPosition()
{
if (!m_bPositMatrixInitialized) 
	throw("posit not initilized");

	cvZero(m_HeadPositionGraph);
	static int k ;
	static float StartYaw, StartPitch , StartRoll;

	CvPoint3D32f ModelHeadCenter3D;
	CvPoint3D32f CurrentHeadCenter3D;
	//ModelHeadCenter3D.x = 0;
	//ModelHeadCenter3D.y = 0;
	//ModelHeadCenter3D.z = 0;
	CurrentHeadCenter3D = m_CurrentHeadCenter3D;

	if (!m_bComputeHeadPositionFirst)
	{
		//m_bComputeHeadPositionFirst = true;
		k = 2 *  CurrentHeadCenter3D.z / 400;
	}

	float rotMatr[9];
	float transVect[3];

	//CvMat MatrDescr = cvMat(3, 3, CV_32FC1, rotMatr);
	//CvMat rotMatrDescr = cvMat(3, 3, CV_32FC1, m_RotationMatrix);
	//CvMat rotVectDescr = cvMat(3, 1, CV_32FC1, m_rotVect);
	//cvRodrigues2(  &rotVectDescr,&MatrDescr );

	float d = 0;
	for(int i= 0 ; i<9 ; i++)
	{
		rotMatr[i] =  m_RotationMatrix[i];
		d+=(rotMatr[i] - m_RotationMatrix[i])*(rotMatr[i] - m_RotationMatrix[i]);
	}


	float X =sin((double)180);
	float Yaw, Pitch, Roll;

	//m_HeadGazeEstymation

	Yaw = atan2(rotMatr[3], rotMatr[0]);
	Pitch = -asin(rotMatr[6]);
	Roll = atan2(rotMatr[7],rotMatr[8]);


	if (!m_bComputeHeadPositionFirst)
	{
		m_bComputeHeadPositionFirst = true;
		StartYaw = Yaw;
		StartPitch = Pitch;
		StartRoll = Roll;
		cout<<"start yaw pitch roll"<<StartYaw<<" , "<<StartPitch<<" , "<<StartRoll<<endl;
	};


//cout<<" yaw pitch roll"<<Yaw<<" , "<<Pitch<<" , "<<Roll<<endl;


	Yaw -= StartYaw;
	Pitch-= StartPitch;
	Roll -= StartRoll;

	m_HeadYaw = Yaw;
	m_HeadPitch = Pitch;
	m_HeadRoll = Roll;

	CvPoint p;

	CvPoint2D32f point;



	///head position move
	double MoveFactor;
	MoveFactor = 1/(1 +  g_GUI.param2*0.1);
	CvPoint2D32f PositionMove ;
	PositionMove.x = m_ModelRefHeadCenter3D.x - m_CurrentHeadCenter3D.x;
	PositionMove.y = m_ModelRefHeadCenter3D.y - m_CurrentHeadCenter3D.y;

	PositionMove.x *=  MoveFactor;
	PositionMove.y *= MoveFactor;
	PositionMove = cvPoint2D32f(0,0);

	//ztadywanka musi byc na podstawie wielkosci monitora , rozmiaru glowy, f komery
	double Factor;

	Factor = m_fModelScale/m_InitEyesDistans *(100 + g_GUI.param1);


	m_HeadGazeEstymation.x =  Factor* tan(Pitch);
	m_HeadGazeEstymation.y = 1.2* Factor* tan(Roll);

	//m_HeadGazeEstymation = cvPoint2D32f(0,0);

	m_HeadGazeEstymation.x = 0.5 - m_HeadGazeEstymation.x + PositionMove.x;
	m_HeadGazeEstymation.y = 0.5 - m_HeadGazeEstymation.y +  PositionMove.y;
	//point.x =  Factor* tan(Pitch);
	//point.y =  Factor* tan(Roll);

	point.x = CurrentHeadCenter3D.z * tan(Pitch) / k;
	point.y =  CurrentHeadCenter3D.z * tan(Roll) / k;

	point.x *= 13;
	point.y *= 13;

	point.x = 600 - point.x;
	point.y = 450 - point.y;
	p.x = m_HeadGazeEstymation.x * m_HeadPositionGraph->width ;
	p.y = m_HeadGazeEstymation.y * m_HeadPositionGraph->height ;





	//czyszczene 
	if(	g_TrackinkSystem->m_bKeyInitModel3D||g_TrackinkSystem->m_bKeyClearPlot)
	{
		m_vHeadPositionGraph.clear();
	};


	if (!g_TrackinkSystem->m_bKeyDebugPlot)
		return;

	m_vHeadPositionGraph.push_back( p);
	if (m_vHeadPositionGraph.size() >100 )
		m_vHeadPositionGraph.erase(m_vHeadPositionGraph.begin());


	list<CvPoint>::iterator it;



	if (g_pTrackinkSystem->m_GazeMaping.m_Caliblator.m_bHeadAngleMapCoputed)
	{
		float HeadPitch =g_TrackinkSystem->m_HeadPosition.m_fModelScale *  tan( g_TrackinkSystem->m_HeadPosition.m_HeadPitch);
		float HeadRoll  =g_TrackinkSystem->m_HeadPosition.m_fModelScale * tan( g_TrackinkSystem->m_HeadPosition.m_HeadRoll ) ;
		CvPoint2D32f HeadAngle = cvPoint2D32f( HeadPitch,HeadRoll);
		CvPoint2D32f pkt = g_pTrackinkSystem->m_GazeMaping.m_Caliblator.GetMapedPoint(HeadAngle,g_pTrackinkSystem->m_GazeMaping.m_Caliblator.m_HeadAngleMapData, cvSize(m_HeadPositionGraph->width,m_HeadPositionGraph->height ) );
		cvCircle( m_HeadPositionGraph,cvPointFrom32f (pkt), 7, CV_RGB(222,222,0), -1, 8,0);	
	};

	int tmp = 0;
	for( it = m_vHeadPositionGraph.begin(); it != m_vHeadPositionGraph.end(); it++)
	{
		cvCircle( m_HeadPositionGraph,(*it), 1, CV_RGB(50+ (tmp * 200)/m_vHeadPositionGraph.size(),0,0), -1, 8,0);
		tmp++;
	};
	cvCircle( m_HeadPositionGraph,(*(--m_vHeadPositionGraph.end())), 3, CV_RGB(0,255,0), -1, 8,0);



	//-----------tmp------
	point.x = CurrentHeadCenter3D.z * (Pitch) / k;
	point.y =  CurrentHeadCenter3D.z * (Roll) / k;

	point.x *= 13;
	point.y *= 13;

	point.x = 600 - point.x;
	point.y = 450 - point.y;
	p.x = point.x;
	p.y = point.y;

	cvCircle( m_HeadPositionGraph, p, 5, CV_RGB(222,222,222), -1, 8,0);	

	//-------------------

	Yaw = Yaw*180 / 3.141593;
	Pitch =Pitch*180 /  3.141593;
	Roll = Roll*180 /  3.141593;

	g_GUI.ShowImage(m_HeadPositionGraph,"Head POsition");


	//cout<<"================-Yaw:" <<Yaw<< " : "<<Pitch<< " : "<< Roll<< " : " <<endl;;
}
//=========================================================================================
void CHeadPositon::GetKeyFramePerspectiveTransform( CvMat* map_matrix )
{



	CvPoint2D32f srcQuad[4], dstQuad[4];


	srcQuad[0]= m_KeyFrameTransformBaseP1;
	srcQuad[1]= m_KeyFrameTransformBaseP2;
	srcQuad[2]= m_KeyFrameTransformBaseP3;
	srcQuad[3]= m_KeyFrameTransformBaseP4;


	dstQuad[0]= m_KeyFrameTransformCurrentP1;
	dstQuad[1]= m_KeyFrameTransformCurrentP2;
	dstQuad[2]= m_KeyFrameTransformCurrentP3;
	dstQuad[3]= m_KeyFrameTransformCurrentP4;


	bool IsGoodModel;

	double AvrDistansToKeyFrame = 0;


	list<shared_ptr<CTrackingFuture>>::iterator it;





IsGoodModel =	(m_HeadYaw  < 20) &&
				(m_HeadPitch< 20) &&
				(m_HeadRoll  < 20) ;


				
	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{	

	AvrDistansToKeyFrame +=PointDistans((*it)->m_CurrentLocation ,(*it)->m_TransformKeyFrameLocation);
	};

	AvrDistansToKeyFrame /=m_pTracker->m_vTrackingFutures.size();



	if (	AvrDistansToKeyFrame  > 	m_InitEyesDistans * m_fModelScale * 0.3)
			IsGoodModel = false;



if (!IsGoodModel)
{
	 g_TrackinkSystem->M_FaceExtractor.FindFace();

	 if (!g_TrackinkSystem->M_FaceExtractor.m_bFaceIsDetected)
	 {
	 IsGoodModel = true;
	 cout<<"key frame transorm not find face center use model fit"<<endl;
	 };
}



	if (IsGoodModel)
	{
	if (m_bPositMatrixInitialized)

		cvGetAffineTransform(srcQuad,dstQuad,map_matrix);
	else
		cvGetAffineTransform(srcQuad,srcQuad,map_matrix);
	//	cvGetPerspectiveTransform(dstQuad,srcQuad,map_matrix);

	}
	else
	{

		cout<<"key frame transorm from head center"<<endl;
		CvPoint2D32f tmp;
	
	tmp.x = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.x  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.width * 0.5;
	tmp.y = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.y  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.height * 0.5;



	tmp.x -= 	g_TrackinkSystem->m_HeadPosition.m_RefStartHeadPosition.x;
	tmp.y -= 	g_TrackinkSystem->m_HeadPosition.m_RefStartHeadPosition.y;

	float * _Map = map_matrix->data.fl;

	_Map[0] = 1;
	_Map[1] = 0;
	_Map[2] = tmp.x;
	_Map[3] = 0;
	_Map[4] = 1;
	_Map[5] = tmp.y ;


	}




}
//=========================================================================================