




#include "HeadPositon.h"
#include "Utilitis.h"
#include "CTrackinkSystem.h"
#include "HeadPositionDebug.h"

CHeadPositon::CHeadPositon(void)
{

	TmpImg = NULL;
	float f = 2;
	 m_PlotResultImgSize = cvSize(f*	640,f*480);
	m_PlotResultImg  = cvCreateImage( m_PlotResultImgSize, 8, 3);



		 m_DebugPlotResultImgSize = cvSize(	2*640,480);
	m_DebugPlotResultImg  = cvCreateImage( m_DebugPlotResultImgSize, 8, 3);

	

	
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



	m_PlotImgSizeFactor.x = ( float)m_PlotResultImgSize.width / (float)	g_TrackinkSystem->m_Graber.m_pGrayFrame->width ;
	m_PlotImgSizeFactor.y = (float)m_PlotResultImgSize.height /  (float)	g_TrackinkSystem->m_Graber.m_pGrayFrame->height ;

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



	m_InitEyesDistans = g_pTrackinkSystem->m_GazeDetector.m_EyeDistans ;
	//m_InitEyesDistans = m_InitEyesDistans * 640.0/  g_TrackinkSystem->m_Graber.m_iWith

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

	CvRect FaceRegion;
	

	float p = 2.5;
	
	FaceRegion.x  =g_pTrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x - g_pTrackinkSystem->m_GazeDetector.m_EyeDistans*((p-1)/2.0) ;
	FaceRegion.y  =g_pTrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y - g_pTrackinkSystem->m_GazeDetector.m_EyeDistans*((p)/2.3) ;

	FaceRegion.width  = g_pTrackinkSystem->m_GazeDetector.m_EyeDistans *p;
	FaceRegion.height  =g_pTrackinkSystem->m_GazeDetector.m_EyeDistans *(p);

		
	//FaceRegion.x  = g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.x  ;
	//FaceRegion.y  = g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.y;

	//FaceRegion.width  =g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.width;
	//FaceRegion.height  =g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.height;

	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
		//if ((!g_TrackinkSystem->m_GazeDetector.IsEyeFuter((*it).get())))
		{
			count++;
	/*		ImageCordinate.x = (*it)->m_CurrentLocation.x  - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.x;
			ImageCordinate.y = (*it)->m_CurrentLocation.y - g_pTrackinkSystem->M_FaceExtractor.m_FaceRect.y ;*/

	ImageCordinate.x = (*it)->m_CurrentLocation.x  - FaceRegion.x;
			ImageCordinate.y = (*it)->m_CurrentLocation.y - FaceRegion.y ;

			
			/*	if (!(*it)->m_bIsModel3D)
			continue;*/

			if (!g_pTrackinkSystem->M_FaceExtractor.m_bFaceIsDetected)
				throw("face not detected");

			//float s = 1.6;
			(*it)->m_Model3DCoordinate = get3dModelCoordinates(ImageCordinate,FaceRegion.width,FaceRegion.height  );
			//(*it)->m_bIsModel3D = true;
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

//	m_pTracker->m_pLeftPupilCenter->m_Model3DCoordinate.z = m_pTracker->m_pLeftEyeCornerL->m_Model3DCoordinate.z;


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

	cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(HeadCenter,m_PlotImgSizeFactor ) ),6, CV_RGB(0,0,0),-1, 8,0);


	//usuniento poprzedni pkt refernencyjny
	int counter = 0;

	//BestReferencePoint =  (*m_pTracker->m_vTrackingFutures.begin());
	if ((m_pReferenceModelPoint.use_count() == 0) || (m_pReferenceModelPoint->m_bIsErased) || GetNew ||
		(m_pReferenceModelPoint->m_IsLocated  == false)/* || (m_pReferenceModelPoint->IsOutLiter())*/ )
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
		m_RefPointShift = m_RefPoint;

		
//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
//	(*it)->m_Model3DCoordinate =PointSub((*it)->m_Model3DCoordinate,m_RefPoint);


	}//if erased


	if (g_TrackinkSystem->m_bKeyChangeRef3DPoint)
	{

		int index = rand()%m_pTracker->m_vTrackingFutures.size();
		cout<<"new ref point index:"<<index<<endl;
		it = m_pTracker->m_vTrackingFutures.begin();
		for (int i = 0; i<index; i++)
			it++;
		m_pReferenceModelPoint = (*it);
		m_RefPoint = m_pReferenceModelPoint->m_Model3DCoordinate;

	m_RefPointShift = m_RefPoint;
		
//for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
//	PointSub((*it)->m_Model3DCoordinate,m_RefPoint);
//



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

	//if (((*it)->m_Model3DState == InModelState) && (*it)->m_bIsModel3D)
	if(	(!(*it)->IsOutLiter())&& (!(*it)->m_bIsEyeCorner ))
	{
		tmpPoint.x = (*it)->m_Model3DCoordinate.x  ; 
		tmpPoint.y = (*it)->m_Model3DCoordinate.y  ; 
		tmpPoint.z = (*it)->m_Model3DCoordinate.z ; 

		m_vModelPoints.push_back( tmpPoint);
		m_vImagePoints.push_back((*it)->m_CurrentLocation );
	};

	};//for

	cout<<"model used count point :"<<m_vImagePoints.size()<<endl;



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


		double s= m_fModelScale * 0.3 + 0.7*  m_ModelRefHeadCenter3D.z / m_CurrentHeadCenter3D.z;


	//	cout<<"------corrected -model Scale: "<<s<<endl;

		m_fModelScale =  m_ModelRefHeadCenter3D.z / m_CurrentHeadCenter3D.z;

		m_fModelScale = s;
		//cout<<"-------model Scale: "<<m_fModelScale<<endl;

//cout<<"------- Scale diff : "<<m_fModelScale - s<<endl;

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


	//update futers

//----------------------------------------------------------------------

double AvrDistansToModel = 0;
double AvrDistansToKeyFrame = 0;

int count = 0;
	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{
			if ( !(((*it)->m_Model3DState == InModelState) && (*it)->m_bIsModel3D))
			continue;

		(*it)->m_Model3dProjectLocation = ProjectTo2D((*it)->m_Model3DCoordinate );
		AvrDistansToModel += PointDistans((*it)->m_CurrentLocation, (*it)->m_Model3dProjectLocation);
		AvrDistansToKeyFrame += PointDistans((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation);
		count++;
	};

AvrDistansToModel /= count;
AvrDistansToKeyFrame /= count;

//----------------------------------------------------------------------


	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{	
		// nowe punkty do modelu
		if(m_bPositMatrixComputed)
		{
			(*it)->m_Model3dProjectLocation = ProjectTo2D((*it)->m_Model3DCoordinate );
				
					// corect outliners

			if ((*it)->IsOutLiter())
	/*			if ( ((*it)->m_Model3DState != InModelState) &&
						((*it)->m_Model3DState !=InModelNotUseState))*/
				{
					//to do : jesli lepsze z key frame to brac
				//(*it)->m_CurrentLocation  = PointSub((*it)->m_Model3dProjectLocation, (*it)->m_ShiftToModel3dProjectLocation ); 
				
				if (AvrDistansToModel < AvrDistansToKeyFrame) 
						(*it)->m_CurrentLocation  = (*it)->m_Model3dProjectLocation;
				else
						(*it)->m_CurrentLocation  = (*it)->m_TransformKeyFrameLocation;
				};
				(*it)->m_ShiftToModel3dProjectLocationPrev =(*it)->m_ShiftToModel3dProjectLocation;
				(*it)->m_ShiftToModel3dProjectLocation =  PointSub((*it)->m_Model3dProjectLocation, (*it)->m_CurrentLocation );
				(*it)->m_PrevLocation = (*it)->m_CurrentLocation;

	
		};
	};

	m_IterationCount ++;
	return true;
}

//
//===================================================================
bool CHeadPositon::Ransac()
{
	//list<shared_ptr<CTrackingFuture>>::iterator it;
	//std::vector<CvPoint3D32f> TestModelPoints;
	//std::vector<CvPoint2D32f> TestImagePoints;

	//std::vector<CvPoint3D32f> BestSetModelPoints;
	//std::vector<CvPoint2D32f> BestSetImagePoints;


	//std::vector<int> BestSetPointsIndexs;

	//std::vector<int> CurrentSetPointsIndexs;



	//CvMatr32f BestRotationMatrix = new float[9];
	//CvVect32f BestTranslationVector = new float[3];


	//CvMatr32f rotationTestMatrix = new float[9];
	//CvVect32f translationTestVector = new float[3];


	//int bestSize = 0;

	//double distans;
	//double errorSum;
	//int PointCount = 0;
	//int BestSetPointCount = -10000000;
	//double BestSetErrorSum = 99999999;


	//CvPoint3D32f RefPoint = m_RefPoint;

	//double TresholdDistans =  2* getDistanceThreshold();

	//if (m_vModelPoints.size() <RANSAC_SAMPLES )
	//	return false;

	//for(int k=0;k<RANSAC_ITERATIONS;k++)
	//{
	//	TestModelPoints.clear();
	//	TestImagePoints.clear();
	//	//add first reference point
	//	TestModelPoints.push_back(m_vModelPoints[0]);
	//	TestImagePoints.push_back(m_vImagePoints[0]);

	//	for(int z = 0 ; z < RANSAC_SAMPLES ; z++)
	//	{
	//		int index = rand()%m_vModelPoints.size();
	//		TestModelPoints.push_back(m_vModelPoints[index]);
	//		TestImagePoints.push_back(m_vImagePoints[index]);
	//	};


	//	CvPOSITObject *positObject = cvCreatePOSITObject( &TestModelPoints[0], static_cast<int>(TestModelPoints.size()) );
	//	CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
	//	cvPOSIT( positObject, &TestImagePoints[0], EHCIFOCUS, criteria, rotationTestMatrix, translationTestVector );
	//	cvReleasePOSITObject (&positObject);


	//	PointCount = 0;
	//	errorSum = 0;
	//	CurrentSetPointsIndexs.clear();
	//	for(int i = 0; i< m_vModelPoints.size(); i++)
	//	{	
	//		CvPoint2D32f CurrentPoint =  _ProjectTo2D(m_vModelPoints[i], RefPoint,	rotationTestMatrix ,translationTestVector );
	//		double dx = CurrentPoint.x - m_vImagePoints[i].x ;
	//		double dy = CurrentPoint.y - m_vImagePoints[i].y ;
	//		double pointDistance = sqrt(dx*dx+dy*dy);
	//		errorSum+=pointDistance;

	//		//if(pointDistance <RANSAC_DISTANCE_THRESHOLD)//threshold){//
	//		//if(pointDistance <	TresholdDistans)

	//		if (/*(pointDistance < m_fAvrFitError) ||(pointDistance > 2 )||*/(pointDistance <	TresholdDistans))



	//		{
	//			CurrentSetPointsIndexs.push_back(i);
	//			PointCount++;
	//		}

	//	};// for

	//	if (  BestSetPointCount /*- BestSetErrorSum *0.01*/ <= PointCount /*- errorSum*0.01*/)
	//	{

	//		if (BestSetPointCount ==  PointCount )
	//			if (BestSetErrorSum <= errorSum)
	//				continue;
	//		BestSetErrorSum = errorSum;
	//		BestSetPointCount = PointCount;

	//		BestSetPointsIndexs.clear();

	//		for(int j=0; j<CurrentSetPointsIndexs.size(); j++)
	//			BestSetPointsIndexs.push_back(CurrentSetPointsIndexs[j]);
	//		//BestSetPointsIndexs[j] = CurrentSetPointsIndexs[j];
	//		/*		for(int j = 0 ; j <9 ; j++)
	//		BestRotationMatrix[j] = rotationTestMatrix[j];
	//		for(int j = 0 ; j <3 ; j++)
	//		BestTranslationVector[j] = translationTestVector[j];*/
	//	};

	//};//	for(int k=0;k<RANSAC_ITERATIONS;k++)



	//if(BestSetPointsIndexs.size() < 4) return false;

	//if (BestSetPointsIndexs.size() < m_vModelPoints.size())

	//	cout<<"rans delete :"<< m_vModelPoints.size() - BestSetPointsIndexs.size()  << endl;
	//for(int j = 0 ; j <BestSetPointsIndexs.size() ; j++)
	//{
	//	BestSetModelPoints.push_back(m_vModelPoints[BestSetPointsIndexs[j]]);
	//	BestSetImagePoints.push_back(m_vImagePoints[BestSetPointsIndexs[j]]);
	//};


	//CvPOSITObject *positObject = cvCreatePOSITObject( &BestSetModelPoints[0], static_cast<int>(BestSetModelPoints.size()) );
	//CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 1.0e-5 );
	//cvPOSIT( positObject, &BestSetImagePoints[0], EHCIFOCUS, criteria, m_RotationMatrix, m_TransactionMatrix );
	//cvReleasePOSITObject (&positObject);



	//int BestSetIndex = 0;

	//it = m_pTracker->m_vTrackingFutures.begin();
	//for(int j = 0 ; j <BestSetPointsIndexs.size() ; j++)
	//{

	//	while( BestSetPointsIndexs[j] != BestSetIndex)
	//	{
	//		CvPoint2D32f CurrentPoint  = ProjectTo2D((*it)->m_Model3DCoordinate );
	//		double dx = CurrentPoint.x - (*it)->m_CurrentLocation.x ;
	//		double dy = CurrentPoint.y - (*it)->m_CurrentLocation.y ;
	//		double pointDistance = sqrt(dx*dx+dy*dy);

	//		if ((pointDistance >0.5 * m_fAvrFitError) && (pointDistance > 2))
	//		{
	//			(*it)->m_Model3DState = FitErrorState;
	//		};
	//		it++;
	//		BestSetIndex++;	
	//	};
	//	it++;
	//	BestSetIndex++;	
	//};//for




	////for(int j = 0 ; j <9 ; j++)
	////	m_RotationMatrix[j] = BestRotationMatrix[j];
	////for(int j = 0 ; j <3 ; j++)
	////	m_TransactionMatrix[j] = BestTranslationVector[j];

	return true;
}

//===================================================================
void CHeadPositon::PlotFuter(shared_ptr<CTrackingFuture> futer)
{ 

			if (futer->m_Model3DState ==InModelState)
			{
				if (futer->m_FuterLocationSource  == SecondEstymationOFState)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(0,222,0), -1, 8,0);

				if (futer->m_FuterLocationSource  == FirstEstymationOFState)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(222,222,0), -1, 8,0);
			};

			if (futer->m_Model3DState == OutLintersKeyFrameMoveState)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),6, CV_RGB(222,0,0), 2, 8,0);


	if (futer->m_Model3DState == OutLintersModel3DShiftJump)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),6, CV_RGB(222,100,100), 2, 8,0);

			

		if (futer->m_Model3DState == OutLintersModel3DShift)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),6, CV_RGB(0,150,0), 2, 8,0);



			if (futer->m_Model3DState == OutLintersOFMoveState)
					cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),6, CV_RGB(0,0,222), 2, 8,0);

	if (futer->m_bMoved)
			cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),8, CV_RGB(222,222,222), 2, 8,0);


	if (futer->m_Model3DState == OutLintersEyeFuter)
		
		cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),12, CV_RGB(0,222,222), 3, 8,0);

			

		if ((!futer->m_bIsModel3D )||(futer->m_bIsEyeCorner))
		{
			CvScalar c;
			double Color = (float)((float)futer->m_EyeFuterFactor* 255.0 *5)  ;
			if (Color > 255) Color = 255;
			//if (Color < 10)
				//cout<<"black future:"<<Color<<endl;
				c = CV_RGB(Color,0,Color);
			cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),6, c, 2, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),10, CV_RGB(Color + 30,100,0), 2, 8,0);

		}


			
				if ( futer->m_Model3DState  != NotLocalizedState)
					{
						//modle 3d
						cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor ) ),
						cvPointFrom32f(PointMul((ProjectTo2D(futer->m_Model3DCoordinate)),m_PlotImgSizeFactor ) ), CV_RGB(222,222,222),1);;
						cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul((ProjectTo2D(futer->m_Model3DCoordinate)),m_PlotImgSizeFactor ) ),1, CV_RGB(222,222,222), 2, 8,0);

				/*	cvLine(m_PlotResultImg, cvPointFrom32f(PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul(futer->m_PrevLocation,m_PlotImgSizeFactor )), CV_RGB(222,0,0),1);;
					cvLine(m_PlotResultImg, cvPointFrom32f(PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul(futer->m_TransformKeyFrameLocation,m_PlotImgSizeFactor )), CV_RGB(0,0,222),1);;
					cvLine(m_PlotResultImg, cvPointFrom32f(PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul(futer->m_bSecondOFEstymationLocation,m_PlotImgSizeFactor )), CV_RGB(0,222,0),1);;
					cvLine(m_PlotResultImg, cvPointFrom32f(PointMul(futer->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul(futer->m_bFirstOFEstymationLocation,m_PlotImgSizeFactor )), CV_RGB(222,222,0),1);;

	*/				};
	

}

//===================================================================
void CHeadPositon::Plot()
{
	DebugPlot(); return;

	list<shared_ptr<CTrackingFuture>>::iterator it;

	cvResize(	g_TrackinkSystem->m_Graber.m_pFrame,m_PlotResultImg);



	int d = 1000;
	CvPoint3D32f point;
	CvPoint3D32f point2;
	point.x = 0;
	point.y = 0;
	point.z = 0;
	g_TrackinkSystem->m_HeadCenterPoint = ((ProjectTo2D(point)));



	for(int j =0 ; j<= 5 ; j++)
		for(int i =0 ; i<= 100 ; i++)
		{
			float fx,fy,fz;
			CvPoint2D32f ImageCordinate;
			ImageCordinate.x = i;
			ImageCordinate.y = j;
			point =  get3dModelCoordinates(ImageCordinate,100, 5);
			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(EstimateRightEyeCenter2D,m_PlotImgSizeFactor ) ),1, CV_RGB(100,100,0), 2, 8,0);
			cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);
		}


		for(int j =0 ; j<= 100 ; j++)
			for(int i =0 ; i<= 5 ; i++)
			{
				float fx,fy,fz;
				CvPoint2D32f ImageCordinate;
				ImageCordinate.x = i;
				ImageCordinate.y = j;
				point =  get3dModelCoordinates(ImageCordinate,5, 100);
			//	cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(111,111,100), 0, 8,0);
				cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);

			}


			//------------------osie-----------------------------------------------
			for(int j =-d ; j< d ; j++)
			{
				point.x =j;
				point.y =0;
				point.z =0;
				//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,0,255), 0, 8,0);
				cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(0,0,255), 0, 8,0);

			}
			for(int j =-d ; j< d ; j++)
			{
				point.x =0;
				point.y =j;
				point.z =0;
				//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(250,0,0), 0, 8,0);
				cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1,  CV_RGB(250,0,0), 0, 8,0);
			}
			for(int j =-d ; j< 4*d ; j++)
			{
				point.x =0;
				point.y =0;
				point.z =j;
				//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,(j*255/4*d),0), 0, 8,0);
				cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1,  CV_RGB(0,(j*255/4*d),0), 0, 8,0);
			}


		

int PupilR = 0.25* GetReferenceDistans() * m_PlotImgSizeFactor.x * 0.5;


			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor ) ),PupilR, CV_RGB(0,222,0), 1, 8,0);
			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor ) ),PupilR, CV_RGB(0,222,0), 1, 8,0);



			// paste eyes

			CvRect LeftRoi,RightRoi;
			LeftRoi.x = 0;
			LeftRoi.y = 0;
			LeftRoi.width = g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg->width;
			LeftRoi.height = g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg->height;
			cvSetImageROI(m_PlotResultImg,LeftRoi);
			cvCopy(g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg,m_PlotResultImg);


			RightRoi.x = m_PlotResultImg->width - g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->width ;
			RightRoi.y = 0 ;
			RightRoi.width = g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->width;
			RightRoi.height = g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->height;
			cvSetImageROI(m_PlotResultImg,RightRoi);
			cvCopy(g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg,m_PlotResultImg);

			cvResetImageROI(m_PlotResultImg);

			CvRect R1,R2;

			R1.x = 0 ;
			R1.y = 0 ;
			R1.width = m_DebugPlotResultImg->width * 0.5;
			R1.height =m_DebugPlotResultImg->height;

			R2.x = m_DebugPlotResultImg->width * 0.5 ;
			R2.y = 0 ;
			R2.width = m_DebugPlotResultImg->width * 0.5;
			R2.height = m_DebugPlotResultImg->height;


			cvSetImageROI(m_DebugPlotResultImg,R1);
			cvResize(m_PlotResultImg,m_DebugPlotResultImg);

			

			
			cvSetImageROI(m_DebugPlotResultImg,R2);
			cvResize(g_TrackinkSystem->m_GazeMaping.m_GazeGraph,m_DebugPlotResultImg);


			cvResetImageROI(m_DebugPlotResultImg);
			g_GUI.ShowImage(m_PlotResultImg, "posit");

	/*		g_GUI.ShowImage(m_DebugPlotResultImg, "debug plot");
			g_TrackinkSystem->m_VideoRejestrator.SaveDebugFrame(m_DebugPlotResultImg);*/

}







//===================================================================
void CHeadPositon::DebugPlot()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;
		cvResize(	g_TrackinkSystem->m_Graber.m_pFrame,m_PlotResultImg);
if (g_PlotAllDebug)
{
	if (TmpImg == NULL)
		TmpImg	= cvCloneImage(m_PlotResultImg);

	cvCopy(m_PlotResultImg,TmpImg);

for( it = g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it != g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
{

if ((*it)->m_Model3DState ==InModelState)
	cvCircle( TmpImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(0,222,0), -1, 8,0);


if ((*it)->IsOutLiter())
cvCircle( TmpImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(255,0,0), -1, 8,0);


	cvShowImage( "Tracking futers", TmpImg);

};


}









int	 f = 0;
for( it = g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.begin(); it != g_TrackinkSystem->m_PointTracker.m_vTrackingFutures.end(); it++)
{
	PlotFuter((*it));
};



	int d = 100;
	CvPoint3D32f point;
	CvPoint3D32f point2;
	point.x = 0;
	point.y = 0;
	point.z = 0;
	g_TrackinkSystem->m_HeadCenterPoint = ((ProjectTo2D(point)));



	//------------------------------siatka---------------------------------------------
	// estimatet eye center na posit plot
	//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(EstimateLeftEyeCenter2D,m_PlotImgSizeFactor ) ),1, CV_RGB(100,100,0), 2, 8,0);



	for(int j =0 ; j<= 5 ; j++)
		for(int i =0 ; i<= 100 ; i++)
		{
			float fx,fy,fz;
			CvPoint2D32f ImageCordinate;
			ImageCordinate.x = i;
			ImageCordinate.y = j;
			point =  get3dModelCoordinates(ImageCordinate,100, 5);
			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(EstimateRightEyeCenter2D,m_PlotImgSizeFactor ) ),1, CV_RGB(100,100,0), 2, 8,0);
			cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);
		}


		for(int j =0 ; j<= 100 ; j++)
			for(int i =0 ; i<= 5 ; i++)
			{
				float fx,fy,fz;
				CvPoint2D32f ImageCordinate;
				ImageCordinate.x = i;
				ImageCordinate.y = j;
				point =  get3dModelCoordinates(ImageCordinate,5, 100);
			//	cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(111,111,100), 0, 8,0);
				cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(111,111,100), 0, 8,0);

			}


			//------------------osie-----------------------------------------------

			
			cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(ProjectTo2D(cvPoint3D32f(d,0,0)),m_PlotImgSizeFactor )),
				cvPointFrom32f(   PointMul(   ProjectTo2D(  cvPoint3D32f(-d,0,0)  ) , m_PlotImgSizeFactor ) ),CV_RGB(0,0,255),2);

			

					cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(ProjectTo2D(cvPoint3D32f(0,d,0)),m_PlotImgSizeFactor )),
				cvPointFrom32f(PointMul(ProjectTo2D(cvPoint3D32f(0,-d,0)),m_PlotImgSizeFactor)),CV_RGB(255,0,0),2);

						cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(ProjectTo2D(cvPoint3D32f(0,0,0)),m_PlotImgSizeFactor )),
				cvPointFrom32f(PointMul(ProjectTo2D(cvPoint3D32f(0,0,4*d)),m_PlotImgSizeFactor)),CV_RGB(0,255,0),2);





			//for(int j =-d ; j< d ; j++)
			//{
			//	point.x =j;
			//	point.y =0;
			//	point.z =0;
			//	//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,0,255), 0, 8,0);
			//	cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1, CV_RGB(0,0,255), 0, 8,0);

			//}
			//for(int j =-d ; j< d ; j++)
			//{
			//	point.x =0;
			//	point.y =j;
			//	point.z =0;
			//	//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(250,0,0), 0, 8,0);
			//	cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1,  CV_RGB(250,0,0), 0, 8,0);
			//}
			//for(int j =-d ; j< 4*d ; j++)
			//{
			//	point.x =0;
			//	point.y =0;
			//	point.z =j;
			//	//cvCircle( g_TrackinkSystem->m_Graber.m_pFrame, cvPointFrom32f(ProjectTo2D(point)), 1, CV_RGB(0,(j*255/4*d),0), 0, 8,0);
			//	cvCircle( m_PlotResultImg, cvPointFrom32f(PointMul(ProjectTo2D(point),m_PlotImgSizeFactor )), 1,  CV_RGB(0,(j*255/4*d),0), 0, 8,0);
			//}



	//eye corner
	//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerL->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), 2, 8,0);
	//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftEyeCornerR->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), 2, 8,0);

	//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerL->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), 2, 8,0);
	//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_PointTracker.m_pRightEyeCornerR->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), 2, 8,0);




			//EstimateLeftEyeCenter2D = ProjectTo2D(LeftEyeCenter3D);
			//EstimateRightEyeCenter2D = ProjectTo2D(RightEyeCenter3D);

			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(EstimateLeftEyeCenter2D,m_PlotImgSizeFactor ) ),6, CV_RGB(200,200,0), 1, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(EstimateRightEyeCenter2D,m_PlotImgSizeFactor ) ),6, CV_RGB(200,200,0), 1, 8,0);


			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->M_FaceExtractor.m_LeftEyeCenter,m_PlotImgSizeFactor ) ),5, CV_RGB(220,222,0), 1, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->M_FaceExtractor.m_RightEyeCenter,m_PlotImgSizeFactor ) ),5, CV_RGB(220,222,0), 1, 8,0);


int PupilR = 0.25* GetReferenceDistans() * m_PlotImgSizeFactor.x * 0.5;
	//	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenterOF,m_PlotImgSizeFactor )),PupilR, CV_RGB(0,222,0), 2, 8,0);
	//cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_GazeDetector.m_RightPupilCenterOF,m_PlotImgSizeFactor )),PupilR, CV_RGB(0,222,0),2, 8,0);




			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor ) ),PupilR, CV_RGB(0,222,0), 1, 8,0);
			cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor ) ),PupilR, CV_RGB(0,222,0), 1, 8,0);


			//Key frame points

			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP1,m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP2,m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP3,m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);
			//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(m_KeyFrameTransformCurrentP4,m_PlotImgSizeFactor ) ),5, CV_RGB(0,222,222), -1, 8,0);


			//Referenc point;
		//	cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(ProjectTo2D(m_RefPoint),m_PlotImgSizeFactor ) ),6, CV_RGB(220,222,0), 4, 8,0);

			




			//-----------eye wectors------------------------------

			CvPoint3D32f P1;
			CvPoint2D32f p;
				CvPoint3D32f shift;
				float EyeR ;
	 EyeR = GetReferenceDistans() * 0.1;
			float L = 200;
	double ScaleFactor = (g_TrackinkSystem->m_HeadPosition.m_fModelScale*  g_TrackinkSystem->m_HeadPosition.m_fModelScale);



P1=g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_Model3DCoordinate;
				P1.z += L;
			
				shift.x =- L * ( 	(ScaleFactor*g_TrackinkSystem->m_GazeMaping.m_LeftGazeKonpensate.x )/ EyeR   ) ;
				shift.y = -L * ( 	(ScaleFactor*g_TrackinkSystem->m_GazeMaping.m_LeftGazeKonpensate.y)/ EyeR   ) ;
shift.z = 0;
 p = 	 _ProjectTo2D( P1, m_RefPoint,m_RotationMatrix ,m_TransactionMatrix ,shift);
			
				cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pLeftPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor )  ),
				cvPointFrom32f(PointMul(p,m_PlotImgSizeFactor )),CV_RGB(255,255,0),2);




		
P1=g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_Model3DCoordinate;
				P1.z += L;
			
				shift.x =- L * ( (ScaleFactor*	g_TrackinkSystem->m_GazeMaping.m_RightGazeKonpensate.x)/ EyeR   ) ;
				shift.y = -L * ((ScaleFactor* 	g_TrackinkSystem->m_GazeMaping.m_RightGazeKonpensate.y)/ EyeR   ) ;
shift.z = 0;
 p = 	 _ProjectTo2D( P1, m_RefPoint,m_RotationMatrix ,m_TransactionMatrix ,shift);
			
				cvLine(m_PlotResultImg,cvPointFrom32f(PointMul(g_TrackinkSystem->m_PointTracker.m_pRightPupilCenter->m_CurrentLocation,m_PlotImgSizeFactor )  ),
				cvPointFrom32f(PointMul(p,m_PlotImgSizeFactor )),CV_RGB(255,255,0),2);

			//---------------------------------------
			// paste eyes

			CvRect LeftRoi,RightRoi;
			LeftRoi.x = 0;
			LeftRoi.y = 0;
			LeftRoi.width = g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg->width;
			LeftRoi.height = g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg->height;
			cvSetImageROI(m_PlotResultImg,LeftRoi);
			cvCopy(g_TrackinkSystem->m_GazeDetector.m_LeftEyePlotImg,m_PlotResultImg);


			RightRoi.x = m_PlotResultImg->width - g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->width ;
			RightRoi.y = 0 ;
			RightRoi.width = g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->width;
			RightRoi.height = g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg->height;
			cvSetImageROI(m_PlotResultImg,RightRoi);
			cvCopy(g_TrackinkSystem->m_GazeDetector.m_RightEyePlotImg,m_PlotResultImg);

			cvResetImageROI(m_PlotResultImg);

	cvNamedWindow(  "posit", 0 );
		cvShowImage( "posit", m_PlotResultImg);
	//--------------------------------------------------------------------------	
			
	return;
			cvResetImageROI(m_PlotResultImg);

			CvRect R1,R2;

			R1.x = 0 ;
			R1.y = 0 ;
			R1.width = m_DebugPlotResultImg->width * 0.5;
			R1.height =m_DebugPlotResultImg->height;

			R2.x = m_DebugPlotResultImg->width * 0.5 ;
			R2.y = 0 ;
			R2.width = m_DebugPlotResultImg->width * 0.5;
			R2.height = m_DebugPlotResultImg->height;


			cvSetImageROI(m_DebugPlotResultImg,R1);
			cvResize(m_PlotResultImg,m_DebugPlotResultImg);

			

			
			cvSetImageROI(m_DebugPlotResultImg,R2);
			cvResize(m_HeadPositionGraph,m_DebugPlotResultImg);

			cvResetImageROI(m_DebugPlotResultImg);

			//g_GUI.ShowImage(m_DebugPlotResultImg, "debug plot");
			g_TrackinkSystem->m_VideoRejestrator.SaveDebugFrame(m_DebugPlotResultImg);

}






//===================================================================
float CHeadPositon::GetReferenceDistans()
{

	return m_fModelScale * m_InitEyesDistans ;
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


		 m_TransactionMatrixPrepared[0] =m_TransactionMatrix[0] ;
		 	 m_TransactionMatrixPrepared[1] =m_TransactionMatrix[1] ;
			 	 m_TransactionMatrixPrepared[2] =m_TransactionMatrix[2] ;
	};





	//cout<<" yaw pitch roll"<<Yaw<<" , "<<Pitch<<" , "<<Roll<<endl;




	//-------------------------------------------------------------
		 //m_RotationMatrixPrepared[0] = cos(Yaw)*cos(Pitch) ;
		 //m_RotationMatrixPrepared[1] = cos(Yaw)*sin(Pitch)*sin(Roll)  - sin(Yaw)*cos(Roll);
		 //m_RotationMatrixPrepared[2] = cos(Yaw)*sin(Pitch)*cos(Roll)  + sin(Yaw)*sin(Roll);

		 //m_RotationMatrixPrepared[3] = sin(Yaw)*cos(Pitch) ;
		 //m_RotationMatrixPrepared[4] = sin(Yaw)*sin(Pitch)*sin(Roll)  + cos(Yaw)*cos(Roll);
		 //m_RotationMatrixPrepared[5] = sin(Yaw)*sin(Pitch)*cos(Roll)  - cos(Yaw)*sin(Roll);

		 //m_RotationMatrixPrepared[6] = -sin(Yaw) ;
		 //m_RotationMatrixPrepared[7] = cos(Pitch)*sin(Roll) ;
		 //m_RotationMatrixPrepared[8] = cos(Yaw)*cos(Roll) ;

	//-------------------------------------------------------------



		 //m_RotationMatrixPrepared[0] = cos(Yaw)*cos(Pitch) ;
		 //m_RotationMatrixPrepared[1] = cos(Yaw)*sin(Pitch)*sin(Roll)  - sin(Yaw)*cos(Roll);
		 //m_RotationMatrixPrepared[2] = cos(Yaw)*sin(Pitch)*cos(Roll)  + sin(Yaw)*sin(Roll);

		 //m_RotationMatrixPrepared[3] = sin(Yaw)*cos(Pitch) ;
		 //m_RotationMatrixPrepared[4] = sin(Yaw)*sin(Pitch)*sin(Roll)  + cos(Yaw)*cos(Roll);
		 //m_RotationMatrixPrepared[5] = sin(Yaw)*sin(Pitch)*cos(Roll)  - cos(Yaw)*sin(Roll);

		 //m_RotationMatrixPrepared[6] = -sin(Yaw) ;
		 //m_RotationMatrixPrepared[7] = cos(Pitch)*sin(Roll) ;
		 //m_RotationMatrixPrepared[8] = cos(Yaw)*cos(Roll) ;


		 	//-------------------------------------------------------------
		 m_RotationMatrixPrepared[0] = cos(Yaw)*cos(Pitch) ;
		 m_RotationMatrixPrepared[1] = cos(Yaw)*sin(Pitch)*sin(Roll)  - sin(Yaw)*cos(Roll);
		 m_RotationMatrixPrepared[2] = cos(Yaw)*sin(Pitch)*cos(Roll)  + sin(Yaw)*sin(Roll);

		 m_RotationMatrixPrepared[3] = sin(Yaw)*cos(Pitch) ;
		 m_RotationMatrixPrepared[4] = sin(Yaw)*sin(Pitch)*sin(Roll)  + cos(Yaw)*cos(Roll);
		 m_RotationMatrixPrepared[5] = sin(Yaw)*sin(Pitch)*cos(Roll)  - cos(Yaw)*sin(Roll);

		 m_RotationMatrixPrepared[6] = -sin(Yaw) ;
		 m_RotationMatrixPrepared[7] = cos(Pitch)*sin(Roll) ;
		 m_RotationMatrixPrepared[8] = cos(Yaw)*cos(Roll) ;
	

//-------------------------------------------------------------


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

	 d = m_InitEyesDistans * (  g_TrackinkSystem->m_Graber.m_iWith / 640.0);

	Factor = (100* (1/d )*( g_GUI.param1));



		//if ( g_pTrackinkSystem->m_GazeMaping.m_Caliblator.m_bLeftEyeAngleMapCoputed)
		//{
		//Factor =  g_pTrackinkSystem->m_GazeMaping.m_Caliblator.LeftGx *  g_GUI.param1;
		//}



	Factor = Factor/m_fModelScale;


	m_HeadGazeEstymation.x = /* 0.8**/Factor* tan(Pitch);
	m_HeadGazeEstymation.y =  DispleyFactorY  *1.2*/*2**//*1.4**/ Factor* tan(Roll);

	//m_HeadGazeEstymation = cvPoint2D32f(0,0);

	m_HeadGazeEstymation.x = 0.5 - m_HeadGazeEstymation.x + PositionMove.x;
	m_HeadGazeEstymation.y = 0.5 - m_HeadGazeEstymation.y +  PositionMove.y;


	//------------------konpesator-----------------------------------
	{
	CvPoint2D32f  V = PointSub(m_HeadGazeEstymationKonpensate,m_HeadGazeEstymation);

	m_HeadGazeEstymationV = PointAdd(PointMul(m_HeadGazeEstymationV,0.5) ,PointMul(V,0.5));


		double diff = PointDistans(m_HeadGazeEstymationKonpensate,m_HeadGazeEstymation);
		diff = sqrt(m_HeadGazeEstymationV.x * m_HeadGazeEstymationV.x + m_HeadGazeEstymationV.y * m_HeadGazeEstymationV.y);
		double Alfa = ( 1-  0.3* 2 * diff * diff  /(g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 5000.0));
		if (Alfa > 1)Alfa = 1;
		if (Alfa < 0)Alfa = 0;
		double beta = 1 - Alfa;

		m_HeadGazeEstymationKonpensate.x = m_HeadGazeEstymationKonpensate.x  * Alfa + m_HeadGazeEstymation.x * beta;
		m_HeadGazeEstymationKonpensate.y = m_HeadGazeEstymationKonpensate.y  * Alfa + m_HeadGazeEstymation.y * beta;

		m_HeadGazeEstymation = m_HeadGazeEstymationKonpensate;
	}


	//------------------konpesator-----------------------------------

	//point.x =  Factor* tan(Pitch);
	//point.y =  Factor* tan(Roll);

	//point.x = CurrentHeadCenter3D.z * tan(Pitch) / k;
	//point.y =  CurrentHeadCenter3D.z * tan(Roll) / k;

	//point.x *= 13;
	//point.y *= 13;

	//point.x = 600 - point.x;
	//point.y = 450 - point.y;


	p.x = m_HeadGazeEstymation.x * m_HeadPositionGraph->width ;
	p.y = m_HeadGazeEstymation.y * m_HeadPositionGraph->height ;





	//czyszczene 
	if(	g_TrackinkSystem->m_bKeyInitModel3D||g_TrackinkSystem->m_bKeyClearPlot)
	{
		m_vHeadPositionGraph.clear();
	};


	CvPoint empty = cvPoint(9999,9999);
if (g_pTrackinkSystem->m_bKeyPaint)
	m_vHeadPositionGraph.push_back( p);
else
	m_vHeadPositionGraph.push_back( empty);

	//if (m_vHeadPositionGraph.size() >5000 )
	//	m_vHeadPositionGraph.erase(m_vHea      dPositionGraph.begin());


	list<CvPoint>::iterator it;
	list<CvPoint>::iterator it2;



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
	it2 = it;
	it2++;
	if  (it2 == m_vHeadPositionGraph.end()) break;
	

	if (((*it).x < 9999)&& ((*it2).x < 9999))

	//	cvLine(m_HeadPositionGraph,(*it),(*it2), CV_RGB(20+ (tmp * 200)/m_vHeadPositionGraph.size(),0,0),2);
	cvLine(m_HeadPositionGraph,(*it),(*it2), CV_RGB(222,0,0),2);
			tmp++;
	};


	 tmp = 0;
	//for( it = m_vHeadPositionGraph.begin(); it != m_vHeadPositionGraph.end(); it++)
	//{
	//	cvCircle( m_HeadPositionGraph,(*it), 4, CV_RGB(50+ (tmp * 200)/m_vHeadPositionGraph.size(),0,0), -1, 8,0);
	//	tmp++;
	//};
	// tmp = 0;




	//cvCircle( m_HeadPositionGraph,(*(--m_vHeadPositionGraph.end())), 6, CV_RGB(0,255,0), -1, 8,0);
	cvCircle( m_HeadPositionGraph,(p), 6, CV_RGB(0,255,0), -1, 8,0);



	//-----------tmp------
	//point.x = CurrentHeadCenter3D.z * (Pitch) / k;
	//point.y =  CurrentHeadCenter3D.z * (Roll) / k;

	//point.x *= 13;
	//point.y *= 13;

	//point.x = 600 - point.x;
	//point.y = 450 - point.y;
	//p.x = point.x;
	//p.y = point.y;

	//cvCircle( m_HeadPositionGraph, p, 5, CV_RGB(222,222,222), -1, 8,0);	

	//-------------------

	Yaw = Yaw*180 / 3.141593;
	Pitch =Pitch*180 /  3.141593;
	Roll = Roll*180 /  3.141593;

		cvNamedWindow( "Head POsition", 0 );
		cvShowImage("Head POsition", m_HeadPositionGraph);

	//g_GUI.ShowImage(m_HeadPositionGraph,"Head POsition");


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
	double AvrDistansToModel = 0;


	list<shared_ptr<CTrackingFuture>>::iterator it;





	IsGoodModel =	(m_HeadYaw  < 30) &&
		(m_HeadPitch< 30) &&
		(m_HeadRoll  < 30) ;

	int c = 0;


	for( it = m_pTracker->m_vTrackingFutures.begin(); it != m_pTracker->m_vTrackingFutures.end(); it++)
	{	

		if (!(*it)->IsOutLiter())
		{
		AvrDistansToKeyFrame +=PointDistans((*it)->m_CurrentLocation ,(*it)->m_TransformKeyFrameLocation);
		AvrDistansToModel +=PointDistans((*it)->m_CurrentLocation ,(*it)->m_Model3dProjectLocation);
		c++;
		}
	};

	if (c > 0)
	{
	AvrDistansToKeyFrame /=c;
	AvrDistansToModel /=c;
	};



	if ((	AvrDistansToKeyFrame  > 	GetReferenceDistans()* 0.25) || (AvrDistansToModel  > 	GetReferenceDistans()* 0.15) ||(c < 20))
	{
		cout<<"key frame bad fit"<<endl;
		IsGoodModel = false;
	}



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


	CvPoint2D32f CHeadPositon::ProjectTo2D(CvPoint3D32f point )
	{
		CvPoint3D32f shift;
		shift.x = 0;
shift.y = 0;
shift.z = 0;
		return _ProjectTo2D( point, m_RefPoint,m_RotationMatrix ,m_TransactionMatrix ,shift);
//	return _ProjectTo2D( point, m_RefPoint,m_RotationMatrixPrepared ,m_TransactionMatrix ,shift);
		
	}
	//===========================================================================================================================
