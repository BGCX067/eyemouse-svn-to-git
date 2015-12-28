#include "PointTracker.h"
#include "TrackingFuture.h"
#include "CTrackinkSystem.h"

const int g_TrackedFuterCount = 60;
//CPointTracker::CPointTracker(void)
//{
//}
//
//CPointTracker::~CPointTracker(void)
//{
//}
//==============================================================================

bool CompareTrackingFutures (shared_ptr<CTrackingFuture> a,shared_ptr<CTrackingFuture> b) 
{ 
	return a->getFitScore() <b->getFitScore();

}


////==============================================================================

void  CPointTracker::Init(CvSize FrameSize, CvSize searchNewFutureWindowSize)
{
	Counter = 0;
	m_InitFaze = true;
	m_InterationCount = 0;
	m_MinFutureDistans =  FrameSize.width / 50.0 > 3 ?  FrameSize.width / 100.0 : 3;


	m_FutureBlockSize = FrameSize.width / 100 > 3 ?  FrameSize.width / 100 : 3;
	m_FutureBlockSize = 11;
	m_FutureBlockSize = m_MinFutureDistans;
	if (!( m_FutureBlockSize  % 2) )
		m_FutureBlockSize ++;


	m_SearchNewFutureWindowSize = FrameSize;
	m_bFirstFrame = true;
	m_CurrentPoints = new CvPoint2D32f[MaxFutureCount] ;
	m_NewPoints = new CvPoint2D32f[MaxFutureCount] ;
	m_NewGoodPoints= new CvPoint2D32f[MaxFutureCount] ;
	m_Flags = 0;
	//m_ImgSize = size;



	m_GreyImg = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_PrevGreyImg = g_TrackinkSystem->m_Graber.m_pPrevGrayFrame;


	//m_GreyImg  = cvCreateImage( FrameSize, 8, 1 );
	CvSize PlotResultImgSize = cvSize(800,600);



	m_PlotResultImg  = cvCreateImage( PlotResultImgSize, 8, 3);
	m_PlotResultImgGray = cvCreateImage( PlotResultImgSize, 8, 1);
	m_PlotImgSizeFactor.x = ( float)PlotResultImgSize.width / (float)FrameSize.width ;
	m_PlotImgSizeFactor.y = (float)PlotResultImgSize.height /  (float)FrameSize.height ;


	m_PyramidImg  = cvCreateImage( FrameSize, 8, 1 );
	m_PrevPyramidImg  = cvCreateImage( FrameSize, 8, 1 );

	 m_PyramidImgKeyFrame= cvCreateImage( FrameSize, 8, 1 );
	m_PrevPyramidImgKeyFrame= cvCreateImage( FrameSize, 8, 1 );

	m_KeyFrameImg = cvCreateImage( FrameSize, 8, 1 );
	m_TransformKeyFrameImg = cvCreateImage( FrameSize, 8, 1 );
	//m_PrevGreyImg = cvCreateImage( FrameSize, 8, 1 );
	m_EigImg = cvCreateImage( m_SearchNewFutureWindowSize, 32, 1 );
	m_TmpImg = cvCreateImage( m_SearchNewFutureWindowSize, 32, 1 );
	m_WinSize = cvSize(FutureImgDescriptorSize,FutureImgDescriptorSize);

	IplImage* m_pImgDescriptor =  cvCreateImage( cvSize(FutureImgDescriptorSize,FutureImgDescriptorSize), 8, 1 );


	//shared_ptr<CTrackingFuture> tmp1(new CTrackingFuture() ) ;
	//shared_ptr<CTrackingFuture> tmp2(new CTrackingFuture() ) ;



	//this->m_Model.Init(this);


	m_pLeftPupilCenter.reset(new CTrackingFuture());
	m_pRightPupilCenter.reset(new CTrackingFuture());

	m_pLeftEyeCornerL.reset(new CTrackingFuture());
	m_pLeftEyeCornerR.reset(new CTrackingFuture());;

	m_pRightEyeCornerL.reset(new CTrackingFuture());;
	m_pRightEyeCornerR.reset(new CTrackingFuture());;

	m_pLeftPupilCenter->m_Model3DState = InModelNotUseState;
	m_pRightPupilCenter->m_Model3DState = InModelNotUseState;
	m_pLeftEyeCornerL->m_Model3DState = InModelNotUseState;
	m_pLeftEyeCornerR->m_Model3DState = InModelNotUseState;
	m_pRightEyeCornerL->m_Model3DState = InModelNotUseState;
	m_pRightEyeCornerR->m_Model3DState = InModelNotUseState;
}
//==============================================================================
void CPointTracker::TransformKeyFrame()
{

	float *_Map = new float[6];
	CvMat Map = cvMat(2,3,CV_32F,_Map);

	if (	m_vTrackingFutures.size() < 10 )
		throw("to small amout of tracking futers");



	//IplImage *img =cvCreateImage(  cvSize(g_TrackinkSystem->m_Graber.m_iWith,g_TrackinkSystem->m_Graber.m_iHeight), 8, 1 );
	//IplImage *cpyimg =cvCreateImage(  cvSize(g_TrackinkSystem->m_Graber.m_iWith,g_TrackinkSystem->m_Graber.m_iHeight), 8, 1 );




	CvPoint3D32f OrginalPoints[1000];
	CvPoint2D32f _OrginalPoints[1000];
	CvPoint2D32f TransformPoints[1000];
	CvPoint2D32f _TransformPoints[1000];

	CvMat MatOrginalPoints =cvMat(m_vTrackingFutures.size(),1,CV_32FC3,&OrginalPoints[0]);
	CvMat _MatOrginalPoints =cvMat(m_vTrackingFutures.size(),1,CV_32FC2,&_OrginalPoints[0]);
	CvMat MatTransformPoints=cvMat(m_vTrackingFutures.size(),1,CV_32FC2,&TransformPoints[0]);
	CvMat _MatTransformPoints=cvMat(m_vTrackingFutures.size(),1,CV_32FC2,&_TransformPoints[0]);
	int PointCount = 0;

	list<shared_ptr<CTrackingFuture>>::iterator it;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++) 
	{
		OrginalPoints[PointCount].x =(*it)->m_KeyFrameLocation.x;
		OrginalPoints[PointCount].y =(*it)->m_KeyFrameLocation.y;
		_OrginalPoints[PointCount].x =(*it)->m_KeyFrameLocation.x;
		_OrginalPoints[PointCount].y =(*it)->m_KeyFrameLocation.y;
		OrginalPoints[PointCount].z =1;
		PointCount++;
	};



	//CvPoint2D32f tmp;
	// g_TrackinkSystem->M_FaceExtractor.FindFace();
	//tmp.x = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.x  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.width * 0.5;
	//tmp.y = g_TrackinkSystem->M_FaceExtractor.m_FaceRect.y  + g_TrackinkSystem->M_FaceExtractor.m_FaceRect.height * 0.5;



	//g_TrackinkSystem->m_HeadPosition.m_CurrentHeadCenter3D
	//tmp.x -= 	g_TrackinkSystem->m_HeadPosition.m_RefStartHeadPosition.x;
	//tmp.y -= 	g_TrackinkSystem->m_HeadPosition.m_RefStartHeadPosition.y;

	//_Map[0] = 1;
	//_Map[1] = 0;
	//_Map[2] = tmp.x;
	//_Map[3] = 0;
	//_Map[4] = 1;
	//_Map[5] = tmp.y ;
	////TODO transform z modelu
	////cvZero( m_TransformKeyFrameImg);

	// cvTransform( &MatOrginalPoints,&MatTransformPoints, &Map);
	//cvWarpAffine(m_KeyFrameImg,m_TransformKeyFrameImg, &Map);



	float m_HomograpyMatrixData[6];
	CvMat m_HomograpyMatrix = cvMat(2, 3, CV_32FC1, m_HomograpyMatrixData);
	////CvMat m_HomograpyMatrix = cvMat(3, 3, CV_32FC1, m_HomograpyMatrixData);
	CvMat* map_matrix = &m_HomograpyMatrix;
	g_TrackinkSystem->m_HeadPosition.GetKeyFramePerspectiveTransform( map_matrix );
	////m_HomograpyMatrixData[1] = 0;
	////m_HomograpyMatrixData[3] = 0;
	////cvPerspectiveTransform( &_MatOrginalPoints,&_MatTransformPoints, map_matrix);
	////cvWarpPerspective( m_KeyFrameImg, img, map_matrix );


	cvTransform( &MatOrginalPoints,&MatTransformPoints, map_matrix);
	cvWarpAffine( m_KeyFrameImg, m_TransformKeyFrameImg, map_matrix );



	//cvFlip
	//
	///* Mirror array data around horizontal (flip=0),
	//   vertical (flip=1) or both(flip=-1) axises:
	//   cvFlip(src) flips images vertically and sequences horizontally (inplace) */
	//CVAPI(void)  cvFlip( const CvArr* src, CvArr* dst CV_DEFAULT(NULL),
	//                     int flip_mode CV_DEFAULT(0));


	//cvCopy(m_TransformKeyFrameImg,img);
	PointCount = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++) 
	{
		(*it)->m_TransformKeyFrameLocation.x = TransformPoints[PointCount].x /*/ TransformPoints[PointCount].z*/;
		(*it)->m_TransformKeyFrameLocation.y = TransformPoints[PointCount].y /*/ TransformPoints[PointCount].z*/;;


		//
		//		 		float *h = m_HomograpyMatrixData;
		//				float x = OrginalPoints[PointCount].x;
		//		float y= OrginalPoints[PointCount].y;
		//		float Z = 1./(h[6]*x + h[7]*y + h[8]);
		//		float X = (h[0]*x + h[1]*y + h[2])*Z;
		//		float Y = (h[3]*x + h[4]*y + h[5])*Z;
		//
		//		CvPoint p;
		//p.x = X;
		//p.y = Y;

		//cvCircle( img,cvPoint(_TransformPoints[PointCount].x,_TransformPoints[PointCount].y),6, CV_RGB(222,0,0), -1, 8,0);	

		// cvCircle( img,cvPoint(TransformPoints[PointCount].x,TransformPoints[PointCount].y),2, CV_RGB(222,222,222), -1, 8,0);	
		PointCount++;
	};
	//cvAddWeighted(m_TransformKeyFrameImg, 0.8, g_TrackinkSystem->m_Graber.m_pGrayFrame, 0.4,0.0,img);

	//g_GUI.ShowImage(m_TransformKeyFrameImg,"orgin transform key frame");
	if (g_pTrackinkSystem->m_bKeyDebugPlot)
		g_GUI.ShowImage(m_TransformKeyFrameImg,"transform key frame");

}

//==============================================================================
void  CPointTracker::FindNewGoodFuterToTrack()
{

	//




	int count= 0;
	IplImage* MaskImg = cvCreateImage( cvSize(m_GreyImg->width,m_GreyImg->width), 8, 1 );
	CvPoint P1,P2;
	cvZero(MaskImg);
	cvSet(MaskImg,CV_RGB(1,1,1));
	P1.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1 ;;
	P1.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height * 0.1 ;;;
	P2.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width* 1.1 ;;  ;
	P2.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height* 1.1 ;;;
	cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);

	P1.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1 ;;
	P1.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height * 0.1 ;;;
	P2.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width* 1.1 ;;  ;
	P2.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height* 1.1 ;;;
	cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);


	cvSetImageROI(MaskImg, m_SearchNewFutureWindow );
	cvSetImageROI(m_GreyImg, m_SearchNewFutureWindow );
	cvSetImageROI(m_EigImg, m_SearchNewFutureWindow );
	cvSetImageROI(m_TmpImg, m_SearchNewFutureWindow );

	int FuterCount = g_TrackedFuterCount;
	while (count < FuterCount )
	{
		count = MaxFutureCount;
		cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
			M_fGoodToTrackQuality, m_MinFutureDistans, MaskImg, m_FutureBlockSize, 0, 0.04 );

		M_fGoodToTrackQuality -= 0.008 *(FuterCount - count) / FuterCount;
		//}

		if (M_fGoodToTrackQuality < 0.0001)
		{
			M_fGoodToTrackQuality = 0.0001;
			break;
		}
	};

	cvFindCornerSubPix( m_GreyImg, m_NewGoodPoints, count,
		cvSize(5,5)/*m_WinSize*/, cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,40,0.01));

	cvResetImageROI(m_GreyImg);

	CvPoint2D32f CurrentPoint;
	for( int i= 0; i < count; i++ )
	{
		CurrentPoint = m_NewGoodPoints[i];
		CurrentPoint.x += m_SearchNewFutureWindow.x;
		CurrentPoint.y += m_SearchNewFutureWindow.y;
		//if (IsGoodDistans(CurrentPoint)) // czy nie lezy zbyt blisok obok cech aktualnie sledzonych
		{

			// DODAWANIE NOWYCH CECH DO SLEDZENIA
			shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;

			tmp->m_Model3DState = InModelState;
			tmp->m_PrevLocation = CurrentPoint;
			tmp->m_CurrentLocation= CurrentPoint;
			tmp->m_KeyFrameLocation = CurrentPoint;
			tmp->m_TransformKeyFrameLocation = CurrentPoint;
			tmp->m_IsLocated = true;
			tmp->m_bIsModel3D = true;
			m_vTrackingFutures.push_back(tmp);

			if (m_vTrackingFutures.size() > FuterCount)
				break;
		};


	}
}
//==============================================================================
//
bool  CPointTracker::Initialize()
{
	m_LKFlag = 0;
	m_LKFlagKeyFrame = 0;
	m_Frame = g_pTrackinkSystem->m_Graber.m_pGrayFrame;
	m_SearchNewFutureWindow = g_pTrackinkSystem->M_FaceExtractor.m_FaceRect;



	if ( g_pTrackinkSystem->M_FaceExtractor.m_bFaceIsDetected)

		if (!g_pTrackinkSystem->M_FaceExtractor.m_bFaceIsDetected || !g_pTrackinkSystem->M_FaceExtractor.m_bLeftEyeIsDetected || !g_pTrackinkSystem->M_FaceExtractor.m_bRightEyeIsDetected )
			throw("Point tracker init fail, no face detected");
	//// obcinanie rogow twarzy zostaje tylko centurm
	m_SearchNewFutureWindow.x += m_SearchNewFutureWindow.width*0.2;		
//	FaceCenterRoi.y += FaceCenterRoi.width*0.3;	
	m_SearchNewFutureWindow.width -= m_SearchNewFutureWindow.width*0.4;	
	m_GreyImg = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_PrevGreyImg = g_TrackinkSystem->m_Graber.m_pPrevGrayFrame;
	m_bFirstFrame = true;

	cvResetImageROI(m_GreyImg);

	cvCopy(m_GreyImg,m_KeyFrameImg);
	cvCopy(m_GreyImg,m_TransformKeyFrameImg);



	m_vTrackingFutures.clear();
	m_vTrackingFutures.push_back(m_pLeftPupilCenter);
	m_vTrackingFutures.push_back(m_pRightPupilCenter);
	m_vTrackingFutures.push_back(m_pLeftEyeCornerL);
	m_vTrackingFutures.push_back(m_pLeftEyeCornerR);
	m_vTrackingFutures.push_back(m_pRightEyeCornerL);
	m_vTrackingFutures.push_back(m_pRightEyeCornerR);
	FindNewGoodFuterToTrack();


	return m_vTrackingFutures.size() > 30;
}

//==============================================================================
void  CPointTracker::Update(IplImage* frame,CvRect SearchNewFutureWindow)
{
	Counter++;
	OpticalFlow();
	//UpdateLK();
	if (g_pTrackinkSystem->m_bKeyDebugPlot)
		PlotResult();
	m_bFirstFrame = false;
	//	DrawPerspecitve();
}
//=================================================================================
//TODO temp
void CPointTracker::PlotPoint(CvPoint2D32f point)
{
	//cvCircle( m_PlotResultImg,cvPointFrom32f(PointMul(point,m_PlotImgSizeFactor ) ),2, CV_RGB(222,0,0), -1, 8,0);
	//g_GUI.ShowImage(m_PlotResultImg, "Futers");
}

//=================================================================================
void CPointTracker::PlotResult()
{
	list<shared_ptr<CTrackingFuture>>::iterator it;

	cvResize(	g_TrackinkSystem->m_Graber.m_pFrame,m_PlotResultImg);

	int f = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		f++;
		if ((*it)->IsModelPoint())	
		{

			if ((*it)->m_bBadFit)
				cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(222,222,0), -1, 8,0);
			else
				cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(222,0,0), -1, 8,0);


if ((*it)->m_Model3DState ==OutLintersState)	
cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),4, CV_RGB(0,222,0), -1, 8,0);

			cvLine(m_PlotResultImg, cvPointFrom32f(PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul((*it)->m_PrevLocation,m_PlotImgSizeFactor )), CV_RGB(222,0,0),1);;
			cvLine(m_PlotResultImg, cvPointFrom32f(PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),cvPointFrom32f(PointMul((*it)->m_TransformKeyFrameLocation,m_PlotImgSizeFactor )), CV_RGB(222,222,0),1);;


			if ((*it)->m_Model3DState == InModelNotUseState)
				cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul((*it)->m_CurrentLocation,m_PlotImgSizeFactor )),6, CV_RGB(0,0,222), 2, 8,0);

		};
	};//for
	CvPoint2D32f HeadCenter;
	HeadCenter.x = m_SearchNewFutureWindow.x + m_SearchNewFutureWindow.width/2;
	HeadCenter.y = m_SearchNewFutureWindow.y + m_SearchNewFutureWindow.height/2;



	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_HeadCenterPoint,m_PlotImgSizeFactor )),1, CV_RGB(1,255,1), 2, 8,0);
	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(HeadCenter,m_PlotImgSizeFactor )),10, CV_RGB(1,1,1), 1, 8,0);


	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenterOF,m_PlotImgSizeFactor )),5, CV_RGB(222,222,222), -1, 8,0);
	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(g_TrackinkSystem->m_GazeDetector.m_RightPupilCenterOF,m_PlotImgSizeFactor )),5, CV_RGB(222,222,222),-1, 8,0);

	//eye corner
	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(m_pLeftEyeCornerL->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), -1, 8,0);
	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(m_pLeftEyeCornerR->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), -1, 8,0);

	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(m_pRightEyeCornerL->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), -1, 8,0);
	cvCircle( m_PlotResultImg,cvPointFrom32f( PointMul(m_pRightEyeCornerR->m_CurrentLocation,m_PlotImgSizeFactor )),5, CV_RGB(0,252,252), -1, 8,0);


}



//==============================================================================
void CPointTracker::UpdateLK()
{
	//list<shared_ptr<CTrackingFuture>>::iterator it;
	//CvPoint2D32f CurrentPoint;

	////Param to set
	//double quality = 0.05;
	//int count;
	//count= 0;


	//int TrackingFutureCount = 0;
	//for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++) 
	//{
	//	if  ( (*it)->m_IsLocated  )TrackingFutureCount++;
	//	(*it)->m_IsLocated = false;
	//}


	//OpticalFlow();

	//if (m_vTrackingFutures.size() < 10)
	//{
	//	IplImage* MaskImg = cvCreateImage( cvSize(m_GreyImg->width,m_GreyImg->width), 8, 1 );
	//	CvPoint P1,P2;
	//	cvZero(MaskImg);
	//	cvSet(MaskImg,CV_RGB(1,1,1));
	//	P1.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1 ;;
	//	P1.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height * 0.1 ;;;
	//	P2.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width* 1.1 ;;  ;
	//	P2.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height* 1.1 ;;;
	//	cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);

	//	P1.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1 ;;
	//	P1.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height * 0.1 ;;;
	//	P2.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width* 1.1 ;;  ;
	//	P2.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height* 1.1 ;;;
	//	cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);


	//	//g_GUI.ShowImage(MaskImg,"eye mask");

	//	cvSetImageROI(MaskImg, m_SearchNewFutureWindow );
	//	cvSetImageROI(m_GreyImg, m_SearchNewFutureWindow );
	//	cvSetImageROI(m_EigImg, m_SearchNewFutureWindow );
	//	cvSetImageROI(m_TmpImg, m_SearchNewFutureWindow );

	//	int FuterCount = 60;
	//	while (count < FuterCount )
	//	{
	//		count = MaxFutureCount;
	//		cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
	//			M_fGoodToTrackQuality, m_MinFutureDistans, MaskImg, m_FutureBlockSize, 0, 0.04 );

	//		M_fGoodToTrackQuality -= 0.008 *(FuterCount - count) / FuterCount;
	//		//}

	//		if (M_fGoodToTrackQuality < 0.0001)
	//		{
	//			M_fGoodToTrackQuality = 0.0001;
	//			break;
	//		}
	//	};



	//	//count = 100;
	//	//cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
	//	//0.0001, m_MinFutureDistans, 0, m_FutureBlockSize, 0, 0.04 );

	//	cvFindCornerSubPix( m_GreyImg, m_NewGoodPoints, count,
	//		cvSize(5,5)/*m_WinSize*/, cvSize(-1,-1),
	//		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,40,0.01));


	//	//if (count < 100)
	//	//{
	//	M_fGoodToTrackQuality -= 0.001 *(100 - count) / 100;
	//	//}

	//	if (M_fGoodToTrackQuality < 0.003)
	//		M_fGoodToTrackQuality = 0.003;

	//	cvResetImageROI(m_GreyImg);
	//	/*	g_GUI.ShowImage(m_EigImg,"eigen img 2");*/
	//	//CheckModelGemometry();

	//	//todo dotac

	//	if ( TrackingFutureCount < 150)
	//		for( int i= 0; i < count; i++ )
	//		{
	//			CurrentPoint = m_NewGoodPoints[i];
	//			CurrentPoint.x += m_SearchNewFutureWindow.x;
	//			CurrentPoint.y += m_SearchNewFutureWindow.y;
	//			if (IsGoodDistans(CurrentPoint)) // czy nie lezy zbyt blisok obok cech aktualnie sledzonych
	//			{
	//				//FindFuter(CurrentPoint);

	//				// DODAWANIE NOWYCH CECH DO SLEDZENIA
	//				shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;
	//				tmp->UpdatePosition(CurrentPoint);
	//				tmp->m_KeyFrameLocation = CurrentPoint;
	//				tmp->m_IsLocated = true;
	//				tmp->m_bIsModel3D = true;

	//				//tmp->setModelCenterDistans( m_Model.m_ModelCenterShift,m_Model.m_ModelScale);
	//				m_vTrackingFutures.push_back(tmp);

	//				//int g = m_NewGoodPoints[i].y * m_EigImg->widthStep;
	//				//int b = (int)m_EigImg->imageData ;
	//				//b +=g+1;
	//				// unsigned int* ptr = ( unsigned int*)(void*)b;
	//				//;
	//				////float rte = (m_EigImg->imageData)[0] + m_NewGoodPoints[i].y * m_EigImg->widthStep;
	//				////float* ptr = (float*)(m_EigImg->imageData[ m_NewGoodPoints[i].y * m_EigImg->widthStep]);
	//				//g = m_NewGoodPoints[i].x+1;
	//				//tmp->m_FuterStron = *(( unsigned int*)ptr) ;


	//			};

	//		};
	//};

	//for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	//{
	//	if ((*it)->m_IsLocated) 
	//	{
	//		(*it)->IsLocated();
	//	}
	//	else
	//		(*it)->IsMissing();
	//};


}
//=============================================================}

bool CPointTracker::FindFuter(CvPoint2D32f CurrentPoint )
{
	return true;;

}
//=============================================================}
void CPointTracker::ClearMissingFutures()
{
	return;
	if (m_vTrackingFutures.size() < 1 ) return;
	list<shared_ptr<CTrackingFuture>>::iterator it;

	//TODO erase
	//m_vTrackingFutures.erase(m_vTrackingFutures.begin());



	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{

		/*	if(((*it)->m_LastMissCount >0 ) ||
		(*it)->m_LastMissCount >2  && ((*it)->getScore() < 0.5) ||
		!(*it)->IsInRect(m_SearchNewFutureWindow) ||
		(*it)->m_bMoved 
		)*/


		if (	((*it)->m_bIsToRemoveModel3D )||
			!(*it)->IsInRect(m_SearchNewFutureWindow,0.1) ||
			(!(*it)->IsInRect(m_SearchNewFutureWindow,0.1) &&  (*it)->m_Model3DState  != InModelState)
			)
			//if(((*it)->m_LastMissCount >0 ) ||
			//	(*it)->m_LastMissCount >2  && ((*it)->getScore() < 0.5) ||
			//	!(*it)->IsInRect(m_SearchNewFutureWindow,0.1) ||
			//	(*it)->m_bIsToRemoveModel3D // todo nie usuwac od razu tylko sprawdzic czy sie poprawilo
			//	)
		{
			(*it)->m_bIsErased = true; // bo korzysta z scopepointer
			it = m_vTrackingFutures.erase(it);

		}

		if (it ==  m_vTrackingFutures.end())
			break;
	}


}
//=============================================================}

bool CPointTracker::IsGoodDistans(CvPoint2D32f pCurrentPoint)
{

	bool GoodLocation = false;
	list<shared_ptr<CTrackingFuture>>::iterator it;
	float distans = 0;
	{
		for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
		{
			if (!(*it)->m_IsLocated)  // nie sprawdza zagubionych
				continue;
			/*	if ((*it)->m_FutureState != TrackigState) 
			continue;*/

			distans = 	(pCurrentPoint.x - (*it)->m_CurrentLocation.x)*(pCurrentPoint.x - (*it)->m_CurrentLocation.x) +
				(pCurrentPoint.y - (*it)->m_CurrentLocation.y)*(pCurrentPoint.y - (*it)->m_CurrentLocation.y) ;

			if (distans < m_MinFutureDistans * m_MinFutureDistans)
				return false;

			/*	if (pCurrentPoint->hessian > m_AkceptFutureHesianThreshold)
			return false;*/

		};
	};

	return true;
}
//=============================================================}
void CPointTracker::EyeOpticalFlow()
{
	int i = 0;
	int PointCount;
	/////----------------------pupil center-------------------------------

	m_CurrentModelFutures[i++] = m_pLeftPupilCenter;
	m_CurrentModelFutures[i++] = m_pRightPupilCenter;

	///----------------------eye corner-------------------------------

	m_CurrentModelFutures[i++] = m_pLeftEyeCornerL;
	m_CurrentModelFutures[i++] = m_pLeftEyeCornerR;
	m_CurrentModelFutures[i++] = m_pRightEyeCornerL;
	m_CurrentModelFutures[i++] = m_pRightEyeCornerR;

	PointCount = i;
	//---------------prev frame----------------------------------
	for(  i = 0; i < PointCount; i++ )
	{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;
	};
	int WinS = 5;

	cvCalcOpticalFlowPyrLK( m_TransformKeyFrameImg, m_GreyImg, m_PrevPyramidImgKeyFrame, m_PyramidImgKeyFrame,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,2, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,1000,0.0001), CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY || CV_LKFLOW_INITIAL_GUESSES);



	int BadFitCount = 0;

	for(  i = 0; i < PointCount; i++ )
	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/)
		{
			CvPoint2D32f PrevLocation = m_CurrentModelFutures[i]->m_CurrentLocation;
			double dist;
			dist = sqrt((m_NewPoints[i].x - PrevLocation.x)*(m_NewPoints[i].x - PrevLocation.x) + 
				(m_NewPoints[i].y - PrevLocation.y)*(m_NewPoints[i].y - PrevLocation.y));

			if (dist > 50)
			{
				continue;
				cout<<"Big pupil move detect"<<endl;
			}

			if (( m_FitErrors[i] <800))
			{
				m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
				m_CurrentModelFutures[i]->m_bBadFit = false;
			}
			else
			{

				//m_CurrentModelFutures[i]->m_bBadFit = true;
				//m_CurrentModelFutures[i]->UpdatePosition(g_TrackinkSystem->m_HeadPosition.ProjectTo2D(m_CurrentModelFutures[i]->m_Model3DCoordinate));

				BadFitCount++;
			}
			//m_CurrentModelFutures[i]->m_IsLocated = true;

			//m_CurrentModelFutures[i]->m_FitError += m_FitErrors[i];
		}
		else
		{
			//	m_CurrentModelFutures[i]->m_bBadFit = true;
			//	m_CurrentModelFutures[i]->m_IsLocated = false;
			//m_CurrentModelFutures[i]->UpdatePosition(g_TrackinkSystem->m_HeadPosition.ProjectTo2D(m_CurrentModelFutures[i]->m_Model3DCoordinate));
			//	m_CurrentModelFutures[i]->m_bIsToRemoveModel3D = true;
		};
		//cout<<"BadFitCount:"<<BadFitCount<<endl;

		//it++;

	}
}


//=============================================================}
void CPointTracker::OpticalFlow()
{

	if (m_vTrackingFutures.size() < 10)
		throw ("to less tracking tuter");
	int PointCount;
	cvResetImageROI(m_GreyImg);

	int WinS = 11;//m_FutureBlockSize;
	list<shared_ptr<CTrackingFuture>>::iterator it;
	int 	i = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		//if ((*it)->m_FutureState == TrackigState)
		{
			//m_NewPoints[i] = (*it)->m_CurrentLocation;
			//m_CurrentPoints[i] = (*it)->m_KeyFrameLocation;
			m_CurrentModelFutures[i] = (*it);
			(*it)->m_bFirstOFEstymationOk = false;
			(*it)->m_bSecondOFEstymationOk = false;
			i++;
		}
	}

	PointCount = i;


	for(  i = 0; i < PointCount; i++ )
	{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
	};

	// first calculate the new position of the features based
	// on the (pyramidal) last frame and position estimations



	m_GreyImg = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_PrevGreyImg = g_TrackinkSystem->m_Graber.m_pPrevGrayFrame;

	cvCalcOpticalFlowPyrLK( m_PrevGreyImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS/2.0,WinS/2.0) /*m_WinSize*/,0, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.01),m_LKFlag );



	for(  i = 0; i < PointCount; i++ )
	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/)
		{
			// to do uwzglednic fiterror jak za duzy to nie brac
			m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError =  m_FitErrors[i];;

			//if (m_FitErrors[i] <700)
			{
				m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
				m_CurrentModelFutures[i]->m_Model3DState= InModelState;
			}
			m_CurrentModelFutures[i]->m_FitError += m_FitErrors[i];
			m_CurrentModelFutures[i]->m_bFirstOFEstymationOk = true;
			m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation =m_NewPoints[i] ;


		}
		else
		{
			m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError =  200000;
			m_CurrentModelFutures[i]->m_bFirstOFEstymationOk = false;
			//	m_CurrentModelFutures[i]->UpdatePosition(g_TrackinkSystem->m_HeadPosition.ProjectTo2D(m_CurrentModelFutures[i]->m_Model3DCoordinate));
			;
			//TODO brac lokacje z modelu
			//m_CurrentModelFutures[i]->m_IsLocated = false;
			//	m_CurrentModelFutures[i]->m_bIsToRemoveModel3D = true;
		};

		//it++;

	};



	//---------------key frame----------------------------------

	TransformKeyFrame();


	for(  i = 0; i < PointCount; i++ )
	{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;
	};

WinS = 20;

	cvCalcOpticalFlowPyrLK( m_TransformKeyFrameImg, m_GreyImg,m_PrevPyramidImgKeyFrame, m_PyramidImgKeyFrame,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,3, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,400,0.0005), CV_LKFLOW_INITIAL_GUESSES || m_LKFlagKeyFrame/*CV_LKFLOW_INITIAL_GUESSES || CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY */  );




	//cvCalcOpticalFlowPyrLK( m_KeyFrameImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
	//	m_NewPoints, m_NewPoints, PointCount,cvSize(5,5) /*m_WinSize*/,0, m_FindStatus, m_FitErrors,
	//	cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.01), 0 );




	int BadFitCount = 0;

	it = m_vTrackingFutures.begin();
	for(  i = 0; i < PointCount; i++ )
	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/)
		{
			m_CurrentModelFutures[i]->m_bLocationCorected = false;
			// TODO nowa lista 
			//m_CurrentModelFutures[i]
			//while ((*it)->m_FutureState != TrackigState ) it++;
			if (( m_FitErrors[i] <500))
			{
				m_CurrentModelFutures[i]->m_bBadFit = false;
				m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
				m_CurrentModelFutures[i]->m_Model3DState= InModelState;
				
			}
			else
			{
				m_CurrentModelFutures[i]->m_bBadFit = true;

				BadFitCount++;
			}
			m_CurrentModelFutures[i]->m_Model3DState= InModelState;
			//m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
			m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation = m_NewPoints[i];
			m_CurrentModelFutures[i]->m_bSecondOFEstymationOk = true;
			m_CurrentModelFutures[i]->m_IsLocated = true;
			m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError = m_FitErrors[i];;
			m_CurrentModelFutures[i]->m_FitError += m_FitErrors[i];
		}
		else
		{
			m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError = 20000;
			m_CurrentModelFutures[i]->m_bSecondOFEstymationOk = false;
			m_CurrentModelFutures[i]->m_bBadFit = true;
			m_CurrentModelFutures[i]->m_IsLocated = false;
			//m_CurrentModelFutures[i]->UpdatePosition(g_TrackinkSystem->m_HeadPosition.ProjectTo2D(m_CurrentModelFutures[i]->m_Model3DCoordinate));
			//	m_CurrentModelFutures[i]->m_bIsToRemoveModel3D = true;
		};
		//

		//it++;

	};
	//cout<<"BadFitCount:"<<BadFitCount<<endl;
	//cvCopy( m_GreyImg,m_PrevGreyImg);

//	for(  i = 0; i < PointCount; i++ )
//	{
////if ( m_CurrentModelFutures[i]->m_Model3DState != OutLintersState)
//{
//		if ((m_CurrentModelFutures[i]->m_bSecondOFEstymationOk ||  m_CurrentModelFutures[i]->m_bSecondOFEstymationOk)&&
//			(( m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError < 1000)||( m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError < 500)))
//			
//		{
//			//if ( m_CurrentModelFutures[i]->m_Model3DState != OutLintersState)
//			//{
//			//	m_CurrentModelFutures[i]->m_Model3DState= InModelState;
//			//	if (m_CurrentModelFutures[i]->m_bSecondOFEstymationOk )
//			//	{
//			//		m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation);
//			//	}
//			//	else
//			//	{
//			//		if (m_CurrentModelFutures[i]->m_bFirstOFEstymationOk )
//			//		{
//			//			m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation);
//			//		}
//			//		else
//			//			m_CurrentModelFutures[i]->m_Model3DState= OutLintersState;
//			//	}
//
//				m_CurrentModelFutures[i]->m_Model3DState= InModelState;
//			if ( m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError > 100 * m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError)
//				m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation);
//			else
//				m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation);
//
//		}
//		else
//		{
//			m_CurrentModelFutures[i]->m_Model3DState= OutLintersState;
//		}
//	}
//
//if (m_CurrentModelFutures[i]->m_Model3DState== OutLintersState)
//		m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_TransformKeyFrameLocation);
//
//	//else
//	//{
//	//	if (m_CurrentModelFutures[i]->m_bSecondOFEstymationOk )
//	//	{
//	//		m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation);
//	//		m_CurrentModelFutures[i]->m_Model3DState= InModelState;
//	//	}
//	//	else
//	//	{
//	//		m_CurrentModelFutures[i]->m_Model3DState= OutLintersState;
//	//		if (m_CurrentModelFutures[i]->m_bFirstOFEstymationOk )
//	//		{
//	//			m_CurrentModelFutures[i]->UpdatePosition(m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation);
//	//		}
//
//	//	};
//
//	//};
//
//};

EyeOpticalFlow();

	m_LKFlag  |= CV_LKFLOW_PYR_A_READY;
	m_LKFlagKeyFrame |= CV_LKFLOW_PYR_A_READY;
	IplImage *swap_temp = 0;
	CV_SWAP( m_PrevPyramidImg, m_PyramidImg, swap_temp );
	CV_SWAP(m_PrevPyramidImgKeyFrame, m_PyramidImgKeyFrame, swap_temp );
	



}

//=============================================================}


void CPointTracker::SortFutures()
{

	list<shared_ptr<CTrackingFuture>>::iterator it1;
	list<shared_ptr<CTrackingFuture>>::iterator it2;

	shared_ptr<CTrackingFuture> tmp;
	shared_ptr<CTrackingFuture> tmpSwp;

	for( it1 = m_vTrackingFutures.begin(); it1 != m_vTrackingFutures.end(); it1++)
		for( it2 = m_vTrackingFutures.begin(); it2 != m_vTrackingFutures.end(); it2++)

			if ( ! CompareTrackingFutures( (*it1), (*it2) ))
			{
				tmpSwp = (*it1);
				(*it1) = (*it2);
				(*it2) = tmpSwp;
			};

	//sort (m_vTrackingFutures.begin(), m_vTrackingFutures.end(), CompareTrackingFutures);

}

//==========================================================================




















