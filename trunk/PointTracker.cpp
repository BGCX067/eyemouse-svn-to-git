#include "PointTracker.h"
#include "TrackingFuture.h"
#include "CTrackinkSystem.h"

const int g_TrackedFuterCount = 50;
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
	m_FutureBlockSize = 9;
	m_FutureBlockSize = m_MinFutureDistans;
	if (!( m_FutureBlockSize  % 2) )
		m_FutureBlockSize ++;

	m_FutureBlockSize = 11;
	m_SearchNewFutureWindowSize = FrameSize;
	m_bFirstFrame = true;
	m_CurrentPoints = new CvPoint2D32f[MaxFutureCount] ;
	m_NewPoints = new CvPoint2D32f[MaxFutureCount] ;
	m_NewGoodPoints= new CvPoint2D32f[MaxFutureCount] ;
	m_Flags = 0;
	//m_ImgSize = size;



	m_GreyImg = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_PrevGreyImg = g_TrackinkSystem->m_Graber.m_pPrevGrayFrame;


	m_PyramidImg  = cvCreateImage( FrameSize, 8, 1 );
	m_PrevPyramidImg  = cvCreateImage( FrameSize, 8, 1 );

	m_PyramidImgKeyFrame= cvCreateImage( FrameSize, 8, 1 );
	m_PrevPyramidImgKeyFrame= cvCreateImage( FrameSize, 8, 1 );

	m_KeyFrameImg = cvCreateImage( FrameSize, 8, 1 );

	FuterDebugPlotImg = cvCreateImage( FrameSize, 8, 3 );
	m_TransformKeyFrameImg = cvCreateImage( FrameSize, 8, 1 );
	//m_PrevGreyImg = cvCreateImage( FrameSize, 8, 1 );
	m_EigImg = cvCreateImage( m_SearchNewFutureWindowSize, 32, 1 );
	m_TmpImg = cvCreateImage( m_SearchNewFutureWindowSize, 32, 1 );
	m_TmpImg8  = cvCreateImage( m_SearchNewFutureWindowSize, 8, 1 );
	m_WinSize = cvSize(FutureImgDescriptorSize,FutureImgDescriptorSize);

	IplImage* m_pImgDescriptor =  cvCreateImage( cvSize(FutureImgDescriptorSize,FutureImgDescriptorSize), 8, 1 );


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
void  CPointTracker::FindEyesFuters()
{

	//
	m_vRightEyeFutures.clear();
	m_vLeftEyeFutures.clear();
	
	int FuterCount =10;
	int count= 0;
	IplImage* MaskImg = cvCreateImage( cvSize(m_GreyImg->width,m_GreyImg->width), 8, 1 );
	CvPoint P1,P2;
	CvRect Roi;

	int MinDistans =cvRound( g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.08);

	if (MinDistans < 3 ) MinDistans = 3;

	double quality = 0.5;
	//--------------left eye--------------------------------------------------
	
	cvResetImageROI(MaskImg );
	cvResetImageROI(m_GreyImg );
	cvResetImageROI(m_EigImg);
	cvResetImageROI(m_TmpImg );
	cvZero(MaskImg);
	cvSet(MaskImg,CV_RGB(222,222,222));

	


	
		P1.x = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x ;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y ;
		cvEllipse(MaskImg, (P1), cvSize(cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .25),
		cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .15)),0, 0, 360,CV_RGB(0,0,0), -1, CV_AA, 0);





	//cvCircle( MaskImg,cvPointFrom32f( g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenter),	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.25, CV_RGB(0,0,0), -1);
	Roi.x = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.4;
	Roi.y = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.35;
	Roi.width =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.9;
	Roi.height =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.6;

	

	cvSetImageROI(MaskImg, Roi );
	cvSetImageROI(m_GreyImg, Roi );
	cvSetImageROI(m_EigImg, Roi );
	cvSetImageROI(m_TmpImg, Roi );

	//g_GUI.ShowImage(MaskImg,"mask");

	 FuterCount = 8;
	while (count < FuterCount )
	{
		count = MaxFutureCount;
		cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
			quality, MinDistans, MaskImg, m_FutureBlockSize, 0, 0.04 );

		quality -= 0.008 *(FuterCount - count) / FuterCount;
		//}

		if (quality < 0.02)
		{
			quality = 0.02;
			break;
		}
	};

	cvFindCornerSubPix( m_GreyImg, m_NewGoodPoints, count,
		cvSize(2,2)/*m_WinSize*/, cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,40,0.01));

	cvResetImageROI(m_GreyImg);

	CvPoint2D32f CurrentPoint;
	for( int i= 0; i < count; i++ )
	{
		CurrentPoint = m_NewGoodPoints[i];
		CurrentPoint.x += Roi.x;
		CurrentPoint.y += Roi.y;

			shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;

			tmp->m_EyeCenterShift = PointSub(g_TrackinkSystem->m_GazeDetector.m_LeftPupilCenter,CurrentPoint);
		//	tmp->m_bEyeFuter = true;;
			tmp->m_Model3DState = InModelState;
			tmp->m_PrevLocation = CurrentPoint;
			tmp->m_CurrentLocation= CurrentPoint;
			tmp->m_KeyFrameLocation = CurrentPoint;
			tmp->m_TransformKeyFrameLocation = CurrentPoint;
			tmp->m_IsLocated = true;
		tmp->m_bIsModel3D = false;

			//do sprawdzenia czy mozna uzywac przy posit
			tmp->m_bIsModel3D = true;
			tmp->m_bIsEyeCorner = true;
			m_vLeftEyeFutures.push_back(tmp);
			m_vTrackingFutures.push_back(tmp);
			if (m_vLeftEyeFutures.size() > FuterCount)
				break;
	}


;
//--------------Right eye--------------------------------------------------
quality = 0.5;	

	cvResetImageROI(MaskImg );
	cvResetImageROI(m_GreyImg );
	cvResetImageROI(m_EigImg);
	cvResetImageROI(m_TmpImg );
	cvZero(MaskImg);
	cvSet(MaskImg,CV_RGB(222,222,222));

	//
	//cvCircle( MaskImg,cvPointFrom32f( g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter),	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.25, CV_RGB(0,0,0), -1);
	//Roi.x = g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter.x - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.4;
	//Roi.y = g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter.y - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.4;
	//Roi.width =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.9;
	//Roi.height =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.6;

	//


			P1.x = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.x ;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.y ;
		cvEllipse(MaskImg, (P1), cvSize(cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .25),
		cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .15)),0, 0, 360,CV_RGB(0,0,0), -1, CV_AA, 0);





	//cvCircle( MaskImg,cvPointFrom32f( g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter),	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.25, CV_RGB(0,0,0), -1);
	Roi.x = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.x - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.4;
	Roi.y = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.y - 	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.35;
	Roi.width =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.9;
	Roi.height =  	g_TrackinkSystem->m_GazeDetector.m_EyeDistans * 0.6;


	cvSetImageROI(MaskImg, Roi );
	cvSetImageROI(m_GreyImg, Roi );
	cvSetImageROI(m_EigImg, Roi );
	cvSetImageROI(m_TmpImg, Roi );

	//g_GUI.ShowImage(MaskImg,"mask");

	 FuterCount = 10;
	while (count < FuterCount )
	{
		count = MaxFutureCount;
		cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
			quality, MinDistans, MaskImg, m_FutureBlockSize, 0, 0.04 );

		quality -= 0.008 *(FuterCount - count) / FuterCount;
		//}

		if (quality < 0.02)
		{
			quality = 0.02;
			break;
		}
	};


	cvFindCornerSubPix( m_GreyImg, m_NewGoodPoints, count,
		cvSize(2,2)/*m_WinSize*/, cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,40,0.01));

	cvResetImageROI(m_GreyImg);

	
	for( int i= 0; i < count; i++ )
	{
		CurrentPoint = m_NewGoodPoints[i];
		CurrentPoint.x += Roi.x;
		CurrentPoint.y += Roi.y;

			shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;

			tmp->m_EyeCenterShift = PointSub(g_TrackinkSystem->m_GazeDetector.m_RightPupilCenter,CurrentPoint);
			//tmp->m_bEyeFuter = true;;
			tmp->m_Model3DState = InModelState;
			tmp->m_PrevLocation = CurrentPoint;
			tmp->m_CurrentLocation= CurrentPoint;
			tmp->m_KeyFrameLocation = CurrentPoint;
			tmp->m_TransformKeyFrameLocation = CurrentPoint;
			tmp->m_IsLocated = true;
			tmp->m_bIsModel3D = false;

			tmp->m_bIsModel3D = true;;

			tmp->m_bIsEyeCorner = true;
			m_vRightEyeFutures.push_back(tmp);
			m_vTrackingFutures.push_back(tmp);
			if (m_vRightEyeFutures.size() > FuterCount)
				break;
	}



}
//==============================================================================
void  CPointTracker::FindNewGoodFuterToTrack()
{

	//

	int count= 0;
	IplImage* MaskImg = cvCreateImage( cvSize(m_GreyImg->width,m_GreyImg->height), 8, 1 );
	CvPoint P1,P2;
	cvZero(MaskImg);
	//cvSet(MaskImg,CV_RGB(1,1,1));

	
	//P1.x =m_SearchNewFutureWindow.x;
	//P1.y = m_SearchNewFutureWindow.y;
	//P2.x = m_SearchNewFutureWindow.x+ m_SearchNewFutureWindow.width;
	//P2.y =m_SearchNewFutureWindow.y+ m_SearchNewFutureWindow.height;
	//cvRectangle(MaskImg,P1,P2,CV_RGB(1,1,1),-1);



	int d = PointDistans(g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter,g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter);
	P1.x = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x + d *0.5;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y+d *0.5;
	
	cvEllipse(MaskImg, (P1), cvSize(cvRound(d * 0.9),
		cvRound(d * 1.3)),0, 0, 360,CV_RGB(1,1,1), -1, CV_AA, 0);


	P1.x = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x + d *0.5;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y;

	cvEllipse(MaskImg, (P1), cvSize(cvRound(d * 0.95),
		cvRound(d * 0.9)),0, 0, 360,CV_RGB(1,1,1), -1, CV_AA, 0);





	//P1.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width * 0.1 ;;
	//P1.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height * 0.1 ;;;
	//P2.x = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.width* 1.1 ;;  ;
	//P2.y = g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_LeftEyeRect.height* 1.1 ;;;
	//cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);


	P1.x = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.x ;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter.y ;
		cvEllipse(MaskImg, (P1), cvSize(cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .25),
		cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .15)),0, 0, 360,CV_RGB(0,0,0), -1, CV_AA, 0);



		P1.x = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.x ;
	P1.y = g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter.y ;
		cvEllipse(MaskImg, (P1), cvSize(cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .25),
		cvRound(g_TrackinkSystem->m_GazeDetector.m_EyeDistans * .15)),0, 0, 360,CV_RGB(0,0,0), -1, CV_AA, 0);




	//P1.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width * 0.1 ;;
	//P1.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y+ g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height * 0.1 ;;;
	//P2.x = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.x + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.width* 1.1 ;;  ;
	//P2.y = g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.y + g_TrackinkSystem->M_FaceExtractor.m_RightEyeRect.height* 1.1 ;;;
	//cvRectangle(MaskImg,P1,P2,CV_RGB(0,0,0),-1);


g_GUI.ShowImage(m_EigImg,"mask");

	


	//-------------------------------------------------------

	cvResetImageROI(m_GreyImg);
	cvResetImageROI(m_EigImg);
	cvResetImageROI(MaskImg);
	cvCornerMinEigenVal(m_GreyImg,m_EigImg,m_FutureBlockSize,3);
	cvNormalize(m_EigImg,m_EigImg,0,10,CV_MINMAX,MaskImg);

	//g_GUI.ShowImage(m_EigImg,"eig img");


	cvSmooth(m_GreyImg, m_TmpImg8, CV_GAUSSIAN, 3);
	g_GUI.ShowImage(m_TmpImg8,"smoof");
	cvCornerMinEigenVal(m_TmpImg8,m_EigImg,m_FutureBlockSize,3);
	cvNormalize(m_EigImg,m_EigImg,0,10,CV_MINMAX,MaskImg);




if (g_PlotAllDebug)
{
	cvResetImageROI(m_GreyImg);
	cvResetImageROI(m_EigImg);
	cvResetImageROI(MaskImg);
	cvCornerMinEigenVal(m_GreyImg,m_EigImg,m_FutureBlockSize,3);
	cvNormalize(m_EigImg,m_EigImg,0,10,CV_MINMAX,MaskImg);

	g_GUI.ShowImage(m_EigImg,"eig img");


	cvSmooth(m_GreyImg, m_TmpImg8, CV_GAUSSIAN, 3);
	g_GUI.ShowImage(m_TmpImg8,"smoof");
	cvCornerMinEigenVal(m_TmpImg8,m_EigImg,m_FutureBlockSize,3);
	cvNormalize(m_EigImg,m_EigImg,0,10,CV_MINMAX,MaskImg);

	g_GUI.ShowImage(m_EigImg,"eig img 2");

	//cvCornerHarris( m_GreyImg, m_EigImg,
 //                          m_FutureBlockSize);
	//cvNormalize(m_EigImg,m_EigImg,0,250,CV_MINMAX);
	//g_GUI.ShowImage(m_EigImg,"haris eig img ");


	/*cvCornerMinEigenVal(m_GreyImg,m_EigImg,3,3);
	cvEqualizeHist( m_EigImg, m_EigImg );
	g_GUI.ShowImage(m_EigImg,"eig img equalize");*/
};
//-----------------------------------------------------------





	//cvSetImageROI(MaskImg, m_SearchNewFutureWindow );
	//cvSetImageROI(m_GreyImg, m_SearchNewFutureWindow );
	//cvSetImageROI(m_EigImg, m_SearchNewFutureWindow );
	//cvSetImageROI(m_TmpImg, m_SearchNewFutureWindow );

	int FuterCount = g_TrackedFuterCount;

m_MinFutureDistans = d * 0.1;
if (m_MinFutureDistans < 3)
m_MinFutureDistans = 3;

cout<<"min futer distans"<<m_MinFutureDistans<<endl;

	while (count < FuterCount )
	{
		count = MaxFutureCount;
		cvGoodFeaturesToTrack( m_TmpImg8, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
			M_fGoodToTrackQuality, m_MinFutureDistans, MaskImg, m_FutureBlockSize, 0, 0.04 );

		M_fGoodToTrackQuality -= 0.008 *(FuterCount - count) / FuterCount;
		//}

		if (M_fGoodToTrackQuality < 0.001)
		{
			M_fGoodToTrackQuality = 0.001;
			break;
		}
	};

	//cvCopy(m_GreyImg,m_EigImg);

	//-------------------corner------------------------------------------
	//CvRect Roi = g_TrackinkSystem->M_FaceExtractor.m_FaceRect;
	//IplImage* GrayImg =  g_TrackinkSystem->m_Graber.m_pGrayFrame;
	//IplImage* EigImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	//IplImage* TmpImg = cvCreateImage( cvSize(Roi.width,Roi.height), 32, 1 );
	//IplImage* MaskImg = cvCreateImage( cvSize(Roi.width,Roi.height), 8, 1 );


	//CvPoint2D32f*	NewGoodPoints= new CvPoint2D32f[10] ;
	//int blockSize = 5;
	//double MaxVal = 10;
	//cv::Mat eig, tmp, mask, tmp2;
	//cvSetImageROI(g_TrackinkSystem->m_Graber.m_pGrayFrame, Roi );
	//cv::Mat image = cv::cvarrToMat(g_TrackinkSystem->m_Graber.m_pGrayFrame);


	//cvCircle( MaskImg,cvPointFrom32f(InitGuas), TresholdDistans, CV_RGB(1,1,1), -1, 8,0);



	//cvCornerMinEigenVal(GrayImg,EigImg,blockSize,3);


	//cvNormalize(EigImg,EigImg,250,0,CV_MINMAX,MaskImg);
	////g_GUI.ShowImage(EigImg,"eig img");


	//MaxVal = 250;


	////float a = MaxVal/( (TresholdDistans*TresholdDistans*TresholdDistans*TresholdDistans));
	////
	////	// cv::minMaxLoc(tmp, 0, &MaxVal, 0, 0 ,mask);
	//for( int y=0; y<EigImg->height; y++ ) {
	//	float* ptr = (float*) (EigImg->imageData + y * EigImg->widthStep);
	//	float* ptr2 = (float*) (TmpImg->imageData + y * TmpImg->widthStep);
	//	for( int x=0; x<EigImg->width; x++ )
	//	{
	//		double distans = sqrt(4*(InitGuas.x - x)*(InitGuas.x - x) + (InitGuas.y - y)*(InitGuas.y - y));
	//		distans = distans/(TresholdDistans);

	//		//if (x > 100)
	//		ptr[x] = ptr[x] - MaxVal* distans* distans*distans* distans /* +  MaxVal/2 */ ;
	//		ptr2[x] =	ptr[x]  ;
	//	}
	//}
	////g_GUI.ShowImage(TmpImg,"TmpImg TmpImg TmpImg");


	//double max;
	//CvPoint maxLoc;
	//cvMinMaxLoc(EigImg,0,&max,0,&maxLoc,0);


	//cvNormalize(EigImg,EigImg,250,0,CV_MINMAX,MaskImg);
	////g_GUI.ShowImage(EigImg,"after eig img");


	//NewGoodPoints[0].x = maxLoc.x;
	//NewGoodPoints[0].y = maxLoc.y;



	//

	

	/*cvFindCornerSubPix( g_TrackinkSystem->m_Graber.m_pGrayFrame, NewGoodPoints, 1,
		cvSize(5,5), cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,100,0.001));
	CvPoint2D32f result;
	result.x = NewGoodPoints[0].x + Roi.x;;
	result.y = NewGoodPoints[0].y + Roi.y;;
	return result;*/

//-------------------corner------------------------------------------

	/*	cvResetImageROI(m_GreyImg);

		cvResetImageROI(m_GreyImg);*/

if (g_PlotAllDebug)
{
	cvCopy(g_TrackinkSystem->m_Graber.m_pFrame,FuterDebugPlotImg);
	for( int i= 0; i < count; i++)
		cvCircle( FuterDebugPlotImg,cvPointFrom32f(m_NewGoodPoints[i] ),4, CV_RGB(222,0,0), -1, 8,0);
	//g_GUI.ShowImage(m_EigImg,"tracking futer 1 ");	
}

	cvFindCornerSubPix( m_TmpImg8, m_NewGoodPoints, count,
		cvSize(2,2)/*m_WinSize*/, cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,400,0.0001));


	if (g_PlotAllDebug)
{
	for( int i= 0; i < count; i++)
		cvCircle( FuterDebugPlotImg,cvPointFrom32f(m_NewGoodPoints[i] ),3, CV_RGB(0,222,0), -1, 8,0);
	g_GUI.ShowImage(FuterDebugPlotImg,"tracking futer 1 ");	
}




//--------------after smoof-------------------------------


	//FuterCount = g_TrackedFuterCount;



	while (count < FuterCount )
	{
		count = MaxFutureCount;
		cvGoodFeaturesToTrack( m_GreyImg, m_EigImg, m_TmpImg, m_NewGoodPoints, &count,
			M_fGoodToTrackQuality, m_MinFutureDistans, MaskImg, m_FutureBlockSize, 0, 0.4 );

		M_fGoodToTrackQuality -= 0.008 *(FuterCount - count) / FuterCount;
		//}

		if (M_fGoodToTrackQuality < 0.001)
		{
			M_fGoodToTrackQuality = 0.001;
			break;
		}
	};


//	if (g_PlotAllDebug)
//{
//	cvCopy(g_TrackinkSystem->m_Graber.m_pFrame,FuterDebugPlotImg);
//	for( int i= 0; i < count; i++)
//		cvCircle( FuterDebugPlotImg,cvPointFrom32f(m_NewGoodPoints[i] ),4, CV_RGB(222,0,0), -1, 8,0);
//	//g_GUI.ShowImage(m_EigImg,"tracking futer 1 ");	
//}

	cvFindCornerSubPix( m_GreyImg, m_NewGoodPoints, count,
		cvSize(3,3)/*m_WinSize*/, cvSize(-1,-1),
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,400,0.0001));

//
//	if (g_PlotAllDebug)
//{
//	for( int i= 0; i < count; i++)
//		cvCircle( FuterDebugPlotImg,cvPointFrom32f(m_NewGoodPoints[i] ),3, CV_RGB(0,222,0), -1, 8,0);
//	g_GUI.ShowImage(FuterDebugPlotImg,"tracking futer 2");	
//}





//--------------end after smoof-------------------------------



	cvResetImageROI(m_GreyImg);

	CvPoint2D32f CurrentPoint;
	for( int i= 0; i < count; i++ )
	{
		CurrentPoint = m_NewGoodPoints[i];
		//CurrentPoint.x += m_SearchNewFutureWindow.x;
		//CurrentPoint.y += m_SearchNewFutureWindow.y;
		//if (IsGoodDistans(CurrentPoint)) // czy nie lezy zbyt blisok obok cech aktualnie sledzonych
		{

			// DODAWANIE NOWYCH CECH DO SLEDZENIA
			shared_ptr<CTrackingFuture> tmp(new CTrackingFuture() ) ;

			//tmp->m_bEyeFuter = false;;
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


	cvSmooth(m_GreyImg, m_KeyFrameImg, CV_GAUSSIAN, 3);
	cvSmooth(m_GreyImg, m_TransformKeyFrameImg, CV_GAUSSIAN, 3);
	//cvCopy(m_GreyImg,m_KeyFrameImg);
	//cvCopy(m_GreyImg,m_TransformKeyFrameImg);



	m_vTrackingFutures.clear();
	m_vTrackingFutures.push_back(m_pLeftPupilCenter);
	m_vTrackingFutures.push_back(m_pRightPupilCenter);
	m_vTrackingFutures.push_back(m_pLeftEyeCornerL);
	m_vTrackingFutures.push_back(m_pLeftEyeCornerR);
	m_vTrackingFutures.push_back(m_pRightEyeCornerL);
	m_vTrackingFutures.push_back(m_pRightEyeCornerR);
	FindNewGoodFuterToTrack();
	FindEyesFuters();


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
CvPoint2D32f CPointTracker::GetEyeCenter(list<shared_ptr<CTrackingFuture>> m_vEyeFutures)
{
	CvPoint2D32f result = cvPoint2D32f(0,0);
	CvPoint2D32f CenterEstymation = cvPoint2D32f(0,0);

	float d;
	CvPoint2D32f CurrentCenter;
	
	int MaxMove = g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans();
	list<shared_ptr<CTrackingFuture>>::iterator it;

	CvPoint2D32f AvrMove = cvPoint2D32f(0,0);
	CvPoint2D32f CurrentMove;



	int count = 0;
	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
	{
		if (abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x) >MaxMove ) 
			continue;
		if (abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y) >MaxMove ) 
			continue;
		if ((*it)->m_Model3DState == InModelState)
		{
			AvrMove.x += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x);
			AvrMove.y += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y);
			count++;
		};
	};

	if (count < 2)
		return AvrMove;
	if (count > 0)
	{
		AvrMove.x /= count;
		AvrMove.y /= count;
	};

float	 dl =  0.005* g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans();
	float factor =  5;

	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
	{	

	if ((*it)->m_Model3DState != InModelState)
		continue;

		CurrentMove.x = (PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x);
		CurrentMove.y = (PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y);
		if (  ( (abs(CurrentMove.x) > abs(factor* AvrMove.x)) && ( abs(AvrMove.x) > dl) )  || 
			((abs(CurrentMove.y) >  factor*AvrMove.y)&& ( abs(AvrMove.y) > dl) )  )
		{
			(*it)->m_Model3DState = OutLintersEyeFuter;
			(*it)->m_CurrentLocation = PointAdd((*it)->m_PrevLocation, AvrMove);	
		}


}

	//return AvrMove;


	//-----------------calculate center-----------------------------------


	////---------------------------fit error--------------------------------------

	//--------factor------------
	float Factor = 0.0;
	float tmp;


	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{

	if (!(*it)->IsOutLiter())
	{

		tmp = (  (*it)->m_bSecondOFEstymationFitError );	
		if ((*it)->m_FuterLocationSource == FirstEstymationOFState)
			tmp = ((*it)->m_bFirstOFEstymationFitError + 300);
		tmp *=100;
		tmp = 1 / (100* tmp* tmp);
		
		
		Factor += tmp;
	};
	}

	//---------estymate---------------

	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{
	if (!(*it)->IsOutLiter())
	{
				tmp = (*it)->m_bSecondOFEstymationFitError;	

			if ((*it)->m_FuterLocationSource == FirstEstymationOFState)
				tmp = ((*it)->m_bFirstOFEstymationFitError + 300);
tmp *=100;
					tmp = 1 / (100* tmp* tmp);
		
			float CurrentFactor = tmp/ Factor;

			(*it)->m_EyeFuterFactor =CurrentFactor;
	
	CurrentCenter = PointAdd( (*it)->m_CurrentLocation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale));
	CurrentCenter = PointMul(CurrentCenter ,CurrentFactor );
	result =PointAdd( result, CurrentCenter);
	};
	};

CenterEstymation = result;




//-----------------calculate center-----------------------------------


	////---------------------------Model distans to--------------------------------------

	//--------factor------------
 result = cvPoint2D32f(0,0);
	float FactorModelDistans = 0.0;


 d = g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans();



	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{

	if (!(*it)->IsOutLiter())
	{

		tmp = PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation);

		if (tmp < 0.001) tmp = 0.001;
	
		tmp = d / (  tmp);
		FactorModelDistans += tmp;
	};
	};

		//---------estymate move center---------------

	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{
	if (!(*it)->IsOutLiter())
	{
			tmp = PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation);

		if (tmp < 0.001) tmp = 0.001;
		tmp = d / (  tmp);
		float CurrentFactor = tmp/ FactorModelDistans;

		(*it)->m_EyeFuterFactor = (*it)->m_EyeFuterFactor * 0.7 + 0.3*CurrentFactor;
	
	CurrentCenter = PointAdd( (*it)->m_CurrentLocation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale));
	CurrentCenter = PointMul(CurrentCenter ,(*it)->m_EyeFuterFactor );
	result =PointAdd( result, CurrentCenter);
	};
	}

	CenterEstymation = result;




	

//----------------------move error------------------------------------------
	float MoveFactor = 0.0;
		float DistansToCenter = 0.0;

 d = g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans();
//for (int i = 0 ; i < 10 ; i++)
{
result = cvPoint2D32f(0,0);
	//--------factor------------


	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{

	if (!(*it)->IsOutLiter())
	{

		tmp = PointDistans((*it)->m_CurrentLocation,
					PointSub( CenterEstymation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale)));

		if (tmp < 0.001) tmp = 0.001;
	
		tmp = d / (  tmp);
		MoveFactor += tmp;
	};
	};

		//---------estymate move center---------------

	for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
{
	if (!(*it)->IsOutLiter())
	{
		tmp = PointDistans((*it)->m_CurrentLocation,
					PointSub( CenterEstymation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale)));
		
		if (tmp < 0.001) tmp = 0.001;
		tmp = d / (  tmp);
		float CurrentFactor = tmp/ MoveFactor;

		CurrentFactor = (*it)->m_EyeFuterFactor* 0.8 + 0.2*CurrentFactor;

		(*it)->m_EyeFuterFactor =CurrentFactor;
	
	CurrentCenter = PointAdd( (*it)->m_CurrentLocation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale));
	CurrentCenter = PointMul(CurrentCenter ,CurrentFactor );
	result =PointAdd( result, CurrentCenter);
	};
	};

	//cout<<"Center position distans : "<<PointDistans(CenterEstymation,result);
	CenterEstymation = result;

};

result = CenterEstymation;

//
//for( it = m_vEyeFutures.begin(); it != m_vEyeFutures.end(); it++)
//{
//	if (!(*it)->IsOutLiter())
//	{
//		tmp = PointDistans((*it)->m_CurrentLocation,
//					PointSub( CenterEstymation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale)));
//		
//		if (tmp < 0.001) tmp = 0.001;
//		tmp = d / (  tmp);
//		float CurrentFactor = tmp/ MoveFactor;
//
//		CurrentFactor = (*it)->m_EyeFuterFactor* 0.5 + CurrentFactor;
//
//		(*it)->m_EyeFuterFactor =CurrentFactor;
//	
//	};
//};






return result;

}


//=============================================================}
CvPoint2D32f CPointTracker::GetLeftEyeCenter(	)
{
//GetEyeCenter(m_vLeftEyeFutures);
	//---------------------check out liners----------------------------------
	CvPoint2D32f result = cvPoint2D32f(0,0);

list<shared_ptr<CTrackingFuture>>::iterator it;


int count = 0;
for( it = m_vLeftEyeFutures.begin(); it != m_vLeftEyeFutures.end(); it++)
{
	if (!(*it)->IsOutLiter())
	{
	result =PointAdd( result, PointAdd( (*it)->m_CurrentLocation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale)));
	count++;
	};
	}
result = PointMul(result, (1.0/(float)count));
return result;
}



//=============================================================}
CvPoint2D32f CPointTracker::GetRightEyeCenter()
{
	CvPoint2D32f result = cvPoint2D32f(0,0);
//GetEyeCenter(m_vRightEyeFutures);
list<shared_ptr<CTrackingFuture>>::iterator it;
int count = 0;
for( it = m_vRightEyeFutures.begin(); it != m_vRightEyeFutures.end(); it++)
{
	if (!(*it)->IsOutLiter())
	{
	result =PointAdd( result, PointAdd( (*it)->m_CurrentLocation,PointMul((*it)->m_EyeCenterShift,g_TrackinkSystem->m_HeadPosition.m_fModelScale)));
		count++;
	};

	}
result = PointMul(result, (1.0/(float)count));
return result;
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
void CPointTracker::CheckFutersMove()
{

	int OutLinerCount = 0;
	list<shared_ptr<CTrackingFuture>>::iterator it;

	CvPoint2D32f AvrOFMove = cvPoint2D32f(0,0);
	CvPoint2D32f CurrentMove;

	CvPoint2D32f AvrKeyFrameMove = cvPoint2D32f(0,0);
	CvPoint2D32f CurrentKeyFrameMove;

	
	CvPoint2D32f ModelJumpMove = cvPoint2D32f(0,0);
		float fAvrModelJumpMove = 0;
	CvPoint2D32f CurrentModelJumpMove;



	CvPoint2D32f ModelMove = cvPoint2D32f(0,0);
	float fAvrModelMove = 0;
	CvPoint2D32f CurrentModelMove;



	int FirstTreshold = g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()*30;


	int count = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		if ((*it)->m_Model3DState == InModelState)
		{
			AvrOFMove.x += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x);
			AvrOFMove.y += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y);
			count++;
		};
	};
	if (count > 0)
	{
		AvrOFMove.x /= count;
		AvrOFMove.y /= count;
	};
	//---------------------------------------------------------------------------------------
	count = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		if( (*it)->m_Model3DState == InModelState)
		{
			AvrKeyFrameMove.x += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).x);
			AvrKeyFrameMove.y += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).y);
			count++;
		};
	};

	if (count > 0)
	{
		AvrKeyFrameMove.x /= count;
		AvrKeyFrameMove.y /= count;
	};
	//---------------------------------------------------------------------------------------

	count = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		if( (*it)->m_Model3DState == InModelState)
		{
			//ModelJumpMove = PointSub(PointSub((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ) ,PointSub((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ));
			
			//cout<<"J : "<<abs(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) - PointDistans((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ))<<endl;
			fAvrModelJumpMove += abs(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) - PointDistans((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ));
		//	fAvrModelJumpMove +=sqrt(ModelJumpMove.x * ModelJumpMove.x + ModelJumpMove.y * ModelJumpMove.y);
			count++;
		};
	};

	if (count > 0)
	{
		fAvrModelJumpMove /= count;
	};

	cout<<"avr jump model"<<fAvrModelJumpMove<<endl;
		//---------------------------------------------------------------------------------------


	count = 0;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{
		if( (*it)->m_Model3DState == InModelState)
		{
			fAvrModelMove += abs(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) );
		//	fAvrModelJumpMove +=sqrt(ModelJumpMove.x * ModelJumpMove.x + ModelJumpMove.y * ModelJumpMove.y);
			count++;
		};
	};

	if (count > 0)
	{
		fAvrModelMove /= count;
	};

	cout<<"avr move model"<<fAvrModelMove<<endl;
		//---------------------------------------------------------------------------------------






	float MaxMove =  0.5*g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();

	float MinJump =  0.2 *g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();
	float MaxJump=   0.5*g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();

	float MaxModelMove =  0.6*g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();
	float MinModelMove =  0.1 *g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();


	
	float dl =  0.2*g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();

	float factor =  1/ ((g_TrackinkSystem->m_HeadPosition.m_InitEyesDistans * g_TrackinkSystem->m_HeadPosition.m_fModelScale));

	factor = 3;
	for( it = m_vTrackingFutures.begin(); it != m_vTrackingFutures.end(); it++)
	{	

		if ((*it)->m_Model3DState == NotLocalizedState)
			continue;

		// ----------------OF move-------------------------------------------
		//factor = 40* (AvrOFMove.x + AvrOFMove.y ) /( dl);
		CurrentMove.x = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x);
		CurrentMove.y = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y);
		float d = g_TrackinkSystem->m_HeadPosition.m_InitEyesDistans * g_TrackinkSystem->m_HeadPosition.m_fModelScale / 5.0;
		if (  ( (CurrentMove.x > factor* AvrOFMove.x) && ( AvrOFMove.x > dl) )  || 
			((CurrentMove.y >  factor*AvrOFMove.y)&& ( AvrOFMove.y > dl) )  )

		{
			if ( ((*it)->m_FuterLocationSource == SecondEstymationOFState) && 
				((*it)->m_bFirstOFEstymationFitError < FirstTreshold )) 
			{
				(*it)->m_FuterLocationSource == FirstEstymationOFState;
				(*it)->m_CurrentLocation = (*it)->m_bFirstOFEstymationLocation;
				CurrentMove.x = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).x);
				CurrentMove.y = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_PrevLocation).y);

		if ( ( ( (CurrentMove.x > factor* AvrOFMove.x) && ( AvrOFMove.x > dl) )  || 
			((CurrentMove.y >  factor*AvrOFMove.y)&& ( AvrOFMove.y > dl) ))||(CurrentMove.x+  AvrOFMove.y  > MaxMove )  )
				{
					(*it)->m_Model3DState = OutLintersOFMoveState;
					(*it)->m_FuterLocationSource = Model3dState;
					OutLinerCount++;
					continue;
				};
			}
			else
			{
				(*it)->m_Model3DState = OutLintersOFMoveState;
				(*it)->m_FuterLocationSource = Model3dState;
				OutLinerCount++;
				(*it)->m_bMoved = true;
				continue;
			};


		};



		// ----------------Key Frame move-------------------------------------------
//factor = 40* (AvrKeyFrameMove.x + AvrKeyFrameMove.y ) /( dl);
		CurrentMove.x += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).x);
		CurrentMove.y += abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).y);
		//float d = g_TrackinkSystem->m_HeadPosition.m_InitEyesDistans * g_TrackinkSystem->m_HeadPosition.m_fModelScale / 2.0;
		if (  ( (CurrentMove.x > factor* AvrKeyFrameMove.x) && ( AvrKeyFrameMove.x > dl) )  || 
			((CurrentMove.y >  factor*AvrKeyFrameMove.y)&& ( AvrKeyFrameMove.y > dl) )  )

		{
			if ( ((*it)->m_FuterLocationSource == SecondEstymationOFState) && 
				((*it)->m_bFirstOFEstymationFitError < FirstTreshold )) 
			{
				(*it)->m_FuterLocationSource == FirstEstymationOFState;
				(*it)->m_CurrentLocation = (*it)->m_bFirstOFEstymationLocation;
				CurrentMove.x = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).x);
				CurrentMove.y = abs(PointSub((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation).y);

			//	if (   (CurrentMove.x >  factor* AvrKeyFrameMove.x) || (CurrentMove.y >  factor*AvrKeyFrameMove.y) )

		if (  ( (CurrentMove.x > factor* AvrKeyFrameMove.x) && ( AvrKeyFrameMove.x > dl) )  || 
			((CurrentMove.y >  factor*AvrKeyFrameMove.y)&& ( AvrKeyFrameMove.y > dl) )  )
				{
					(*it)->m_Model3DState = OutLintersKeyFrameMoveState;
					(*it)->m_FuterLocationSource = Model3dState;
					OutLinerCount++;
					continue;
				}
			}
			else
			{
				(*it)->m_Model3DState = OutLintersKeyFrameMoveState;
				(*it)->m_FuterLocationSource = Model3dState;
				(*it)->m_bMoved = true;

				OutLinerCount++;
				continue;
			}
		};
	



	// ----------------jump move-------------------------------------------	// ----------------jump move-------------------------------------------
float CurrentJump;

factor = 3;
ModelJumpMove = PointSub(PointSub((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ) ,PointSub((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ));
			CurrentJump =sqrt(ModelJumpMove.x * ModelJumpMove.x + ModelJumpMove.y * ModelJumpMove.y);

			CurrentJump = /*abs*/(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) - PointDistans((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ));
			//CurrentJump =sqrt(ModelJumpMove.x * ModelJumpMove.x + ModelJumpMove.y * ModelJumpMove.y);


			count++;

			if (  ( (CurrentJump> factor* fAvrModelJumpMove) && (fAvrModelJumpMove> MinJump) ) ||(CurrentJump>MaxJump))

		{
			if ( ((*it)->m_FuterLocationSource == SecondEstymationOFState) && 
				((*it)->m_bFirstOFEstymationFitError < FirstTreshold )) 
			{
				(*it)->m_FuterLocationSource == FirstEstymationOFState;
				(*it)->m_CurrentLocation = (*it)->m_bFirstOFEstymationLocation;
	ModelJumpMove = PointSub(PointSub((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ) ,PointSub((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ));
	CurrentJump =sqrt(ModelJumpMove.x * ModelJumpMove.x + ModelJumpMove.y * ModelJumpMove.y);
CurrentJump = /*abs*/(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) - PointDistans((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ));
			//	if (   (CurrentMove.x >  factor* AvrKeyFrameMove.x) || (CurrentMove.y >  factor*AvrKeyFrameMove.y) )
			if (  ( (CurrentJump > factor* fAvrModelJumpMove) && (fAvrModelJumpMove> MinJump) )||(CurrentJump>MaxJump))
				{
					(*it)->m_Model3DState = OutLintersModel3DShiftJump;
					(*it)->m_FuterLocationSource = Model3dState;
					OutLinerCount++;
					continue;
				}
			}
			else
			{
				(*it)->m_Model3DState = OutLintersModel3DShiftJump;
				(*it)->m_FuterLocationSource = Model3dState;
				(*it)->m_bMoved = true;

				OutLinerCount++;
				continue;
			}
		};



			// ----------------model move-------------------------------------------	// ----------------jump move-------------------------------------------
float CurrentModelMove;
CurrentModelMove = abs(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) );
		
factor = 3;


			if (  ( (CurrentModelMove> factor* fAvrModelMove) && (fAvrModelMove> MinModelMove) ) ||(CurrentModelMove>MaxModelMove))

		{
			if ( ((*it)->m_FuterLocationSource == SecondEstymationOFState) && 
				((*it)->m_bFirstOFEstymationFitError < FirstTreshold )) 
			{
				(*it)->m_FuterLocationSource == FirstEstymationOFState;
				(*it)->m_CurrentLocation = (*it)->m_bFirstOFEstymationLocation;
			CurrentModelMove = abs(PointDistans((*it)->m_CurrentLocation,(*it)->m_Model3dProjectLocation ) - PointDistans((*it)->m_PrevLocation,(*it)->m_Model3dProjectLocation ));
	
			if (  ( (CurrentModelMove> factor* fAvrModelMove) && (fAvrModelMove> MinModelMove) ) ||(CurrentModelMove>MaxModelMove))
				{
					(*it)->m_Model3DState = OutLintersModel3DShift;
					(*it)->m_FuterLocationSource = Model3dState;
					OutLinerCount++;
					continue;
				}
			}
			else
			{
				(*it)->m_Model3DState = OutLintersModel3DShift;
				(*it)->m_FuterLocationSource = Model3dState;
				(*it)->m_bMoved = true;

				OutLinerCount++;
				continue;
			}
		};
	};//for



//	cout<<"Out liners count : "<<OutLinerCount<<endl;
	//TODO  model 3d move
}

//=============================================================}
void CPointTracker::EyeOpticalFlow()
{
	int i = 0;
	int PointCount;
	/////----------------------pupil center-------------------------------


	//if (m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError < 1500)
	//	m_pLeftPupilCenter->m_CurrentLocation = m_pLeftPupilCenter->m_bSecondOFEstymationLocation;

	//if (m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError < 1500)
	//	m_pRightPupilCenter->m_CurrentLocation = m_pRightPupilCenter->m_bSecondOFEstymationLocation;


	//if ( PointDistans(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation) < g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 10.0 )
	//	m_pLeftPupilCenter->m_Model3DState =InModelNotUseState;

	//if ( PointDistans(m_pRightPupilCenter->m_CurrentLocation,m_pRightPupilCenter->m_PrevLocation) < g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 10.0 )
	//	m_pRightPupilCenter->m_Model3DState =InModelNotUseState;

	
		if ( PointDistans(m_pLeftPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter)> g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/4.0 )
			m_pLeftPupilCenter->m_Model3DState =OutLintersState;

		if ( PointDistans(m_pRightPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter) >  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 4.0 )
		m_pRightPupilCenter->m_Model3DState =OutLintersState;


	//m_pRightPupilCenter->m_Model3DState =InModelNotUseState;

	m_CurrentModelFutures[i++] = m_pLeftPupilCenter;
	m_CurrentModelFutures[i++] = m_pRightPupilCenter;

	///----------------------eye corner-------------------------------

	m_CurrentModelFutures[i++] = m_pLeftEyeCornerL;
	m_CurrentModelFutures[i++] = m_pLeftEyeCornerR;
	m_CurrentModelFutures[i++] = m_pRightEyeCornerL;
	m_CurrentModelFutures[i++] = m_pRightEyeCornerR;

	PointCount = i;

	float dl =  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans();



	int WinS = 5;



	for(  i = 0; i < PointCount; i++ )
	{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_PrevLocation;

		//m_Model3dProjectLocation
	};

	//int a = cvRound( dl * 0.05);
	//if (a < 5)
		/// a = cvRound( dl * 0.09);
	WinS = cvRound( dl * 0.09);
	//cout<<"win size eye"<<WinS<<endl;
	

	cvCalcOpticalFlowPyrLK( m_PrevGreyImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,3, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,1000,0.0001), CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY );


//
//for(  i = 0; i < PointCount; i++ )
//	{
//		if( (m_FindStatus[i] == 1)&&( m_FitErrors[i] <300))
//		{
//			CvPoint2D32f PrevLocation = m_CurrentModelFutures[i]->m_CurrentLocation;
//			double dist;
//			dist = sqrt((m_NewPoints[i].x - PrevLocation.x)*(m_NewPoints[i].x - PrevLocation.x) + 
//				(m_NewPoints[i].y - PrevLocation.y)*(m_NewPoints[i].y - PrevLocation.y));
//
//			if (dist > dl * 0.1)
//			{
//				continue;
//				cout<<"Big pupil move detect"<<endl;
//			}
//
//			if (( m_FitErrors[i] <500))
//			{
//				m_CurrentModelFutures[i]->m_CurrentLocation = m_NewPoints[i] ;
//				m_CurrentModelFutures[i]->m_bBadFit = false;
//				m_CurrentModelFutures[i]->m_FuterLocationSource = EyeCorrectState;
//				m_CurrentModelFutures[i]->m_Model3DState =InModelNotUseState;
//			}
//		}
//}
//





	////---------------prev frame----------------------------------
	for(  i = 0; i < PointCount; i++ )
	{
if ((	m_CurrentModelFutures[i]->m_Model3DState !=  InModelState )&&(m_CurrentModelFutures[i]->m_Model3DState !=InModelNotUseState))
		
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
else
{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_PrevLocation;
}
m_NewPoints[i] = m_CurrentModelFutures[i]->m_Model3dProjectLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;



	};



	cvCalcOpticalFlowPyrLK( m_TransformKeyFrameImg, m_GreyImg, m_PrevPyramidImgKeyFrame, m_PyramidImgKeyFrame,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,3, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,1000,0.0001), CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY || CV_LKFLOW_INITIAL_GUESSES);


		//---------------prev frame----------------------------------
	//for(  i = 0; i < PointCount; i++ )
	//{
	//	m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
	//	m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_PrevLocation;
	//};
	// WinS = dl * 0.1;

	//cvCalcOpticalFlowPyrLK( m_PrevGreyImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
	//	m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,3, m_FindStatus, m_FitErrors,
	//	cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,1000,0.0001), CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY );



	int BadFitCount = 0;
	for(  i = 0; i < PointCount; i++ )
	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/)
		{
			CvPoint2D32f PrevLocation = m_CurrentModelFutures[i]->m_CurrentLocation;
			double dist;
			dist = sqrt((m_NewPoints[i].x - PrevLocation.x)*(m_NewPoints[i].x - PrevLocation.x) + 
				(m_NewPoints[i].y - PrevLocation.y)*(m_NewPoints[i].y - PrevLocation.y));

			if (dist > dl * 0.1)
			{
				continue;
				cout<<"Big pupil move detect"<<endl;
			}

		if (( m_FitErrors[i] <500))
			{
				m_CurrentModelFutures[i]->m_CurrentLocation = m_NewPoints[i] ;
				m_CurrentModelFutures[i]->m_bBadFit = false;
				m_CurrentModelFutures[i]->m_FuterLocationSource = EyeCorrectState;
				m_CurrentModelFutures[i]->m_Model3DState =InModelNotUseState;
			}
			//else
			//{

			//	//m_CurrentModelFutures[i]->m_bBadFit = true;
			//	//m_CurrentModelFutures[i]->UpdatePosition(g_TrackinkSystem->m_HeadPosition.ProjectTo2D(m_CurrentModelFutures[i]->m_Model3DCoordinate));

		
			//}
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
		if (m_CurrentModelFutures[i]->m_Model3DState == InModelState)
			m_CurrentModelFutures[i]->m_Model3DState =InModelNotUseState;


	}

		
		if ( PointDistans(m_pLeftPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_GazeDetector.m_LeftEyeCenter)> g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/4.0 )
			m_pLeftPupilCenter->m_Model3DState =OutLintersState;

		if ( PointDistans(m_pRightPupilCenter->m_CurrentLocation,g_TrackinkSystem->m_GazeDetector.m_RightEyeCenter) >  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/ 4.0 )
			m_pRightPupilCenter->m_Model3DState =OutLintersState;


//----------------------------------------------------------------

		float  LeftMove;
		float RightMove;

		if (m_pRightPupilCenter->m_Model3DState ==OutLintersState)
		{
				CvPoint2D32f shift = PointSub(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation);
			m_pRightPupilCenter->m_CurrentLocation = PointAdd(m_pRightPupilCenter->m_PrevLocation,shift);
			cout<<"Right Pupil center corected"<<endl;
		cout<<"Right outliner"<<endl;
		}

			if (m_pLeftPupilCenter->m_Model3DState ==OutLintersState)
			{
						CvPoint2D32f shift = PointSub(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation);
			m_pLeftPupilCenter->m_CurrentLocation = PointAdd(m_pLeftPupilCenter->m_PrevLocation,shift);
			cout<<"Left Pupil center corected"<<endl;
		cout<<"Left outliner"<<endl;
			}


		LeftMove = PointDistans(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation);
		RightMove = PointDistans(m_pRightPupilCenter->m_CurrentLocation,m_pRightPupilCenter->m_PrevLocation);

		if ( (LeftMove >2 *RightMove) && (m_pRightPupilCenter->m_FuterLocationSource == EyeCorrectState) && (LeftMove >  dl * 0.1))
		{
			CvPoint2D32f shift = PointSub(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation);
			m_pLeftPupilCenter->m_CurrentLocation = PointAdd(m_pLeftPupilCenter->m_PrevLocation,shift);
			cout<<"Left Pupil center corected"<<endl;
		}
			


		if ( (RightMove > 2 *LeftMove) && (m_pLeftPupilCenter->m_FuterLocationSource == EyeCorrectState) && (RightMove >  dl * 0.1))
		{
			CvPoint2D32f shift = PointSub(m_pLeftPupilCenter->m_CurrentLocation,m_pLeftPupilCenter->m_PrevLocation);
			m_pRightPupilCenter->m_CurrentLocation = PointAdd(m_pRightPupilCenter->m_PrevLocation,shift);
			cout<<"Right Pupil center corected"<<endl;
		}


	m_LeftEyeCenterEstymation = GetEyeCenter(m_vLeftEyeFutures);
	m_RightEyeCenterEstymation = 	GetEyeCenter(m_vRightEyeFutures);

}


//=============================================================}
void CPointTracker::OpticalFlow()
{

	if (m_vTrackingFutures.size() < 10)
		throw ("to less tracking tuter");
	int PointCount;
	cvResetImageROI(m_GreyImg);

	int WinS = 11;//m_FutureBlockSize;
 WinS = 42;
  WinS = g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans() / 3.5;
  cout<<"Win size:"<<WinS<<endl;

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
			//(*it)->m_Model3DState= NotLocalizedState;
			m_CurrentModelFutures[i]->m_bMoved  = false;
			i++;
		}
	}

	PointCount = i;


	for(  i = 0; i < PointCount; i++ )
	{
		m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;

		
	//m_NewPoints[i] =m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;;
	};

	// first calculate the new position of the features based
	// on the (pyramidal) last frame and position estimations



	m_GreyImg = g_TrackinkSystem->m_Graber.m_pGrayFrame;
	m_PrevGreyImg = g_TrackinkSystem->m_Graber.m_pPrevGrayFrame;

	cvCalcOpticalFlowPyrLK( m_PrevGreyImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS/2.0,WinS/2.0) /*m_WinSize*/,0, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,400,0.0001),/*CV_LKFLOW_INITIAL_GUESSES ||*/ m_LKFlag );



	int SecondFitTreshold = 300;

	for(  i = 0; i < PointCount; i++ )
	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/ )
		{
			// to do uwzglednic fiterror jak za duzy to nie brac
			m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError =  m_FitErrors[i];;

		//	if (m_FitErrors[i] <1000)
			if (! m_CurrentModelFutures[i]->IsOutLiter())
			{
				m_CurrentModelFutures[i]->m_CurrentLocation = m_NewPoints[i] ;
				m_CurrentModelFutures[i]->m_Model3DState= InModelState;
			}
			m_CurrentModelFutures[i]->m_FitError += m_FitErrors[i];
			m_CurrentModelFutures[i]->m_bFirstOFEstymationOk = true;
			m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation =m_NewPoints[i] ;
			m_CurrentModelFutures[i]->m_FuterLocationSource = FirstEstymationOFState;


		}
		else
		{
			m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError =  200000;
			m_CurrentModelFutures[i]->m_bFirstOFEstymationOk = false;
			m_CurrentModelFutures[i]->m_Model3DState= NotLocalizedState;

			//m_CurrentModelFutures[i]->m_Model3DState= NotLocalizedState;
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

		if ( !m_CurrentModelFutures[i]->IsOutLiter())
			m_NewPoints[i] = m_CurrentModelFutures[i]->m_CurrentLocation;
		else
			m_NewPoints[i] = m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;
			//m_NewPoints[i] = m_CurrentModelFutures[i]->m_PrevLocation;

//	m_NewPoints[i] =m_CurrentModelFutures[i]->m_Model3dProjectLocation;;
		m_CurrentPoints[i] = m_CurrentModelFutures[i]->m_TransformKeyFrameLocation;
	};

	//WinS = 20;
	//WinS = 11;
	cvCalcOpticalFlowPyrLK( m_TransformKeyFrameImg, m_GreyImg,m_PrevPyramidImgKeyFrame, m_PyramidImgKeyFrame,
		m_CurrentPoints, m_NewPoints, PointCount,cvSize(WinS,WinS) /*m_WinSize*/,3, m_FindStatus, m_FitErrors,
		cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,400,0.001), CV_LKFLOW_INITIAL_GUESSES || m_LKFlagKeyFrame/*CV_LKFLOW_INITIAL_GUESSES || CV_LKFLOW_PYR_A_READY || CV_LKFLOW_PYR_B_READY */  );




	//cvCalcOpticalFlowPyrLK( m_KeyFrameImg, m_GreyImg, m_PrevPyramidImg, m_PyramidImg,
	//	m_NewPoints, m_NewPoints, PointCount,oucvSize(5,5) /*m_WinSize*/,0, m_FindStatus, m_FitErrors,
	//	cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.01), 0 );




	int BadFitCount = 0;
	int SecondTreshold =  g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()*g_TrackinkSystem->m_HeadPosition.GetReferenceDistans()/12;

		cout<<"Second treshold value :"<<SecondTreshold<<endl;
	it = m_vTrackingFutures.begin();
	for(  i = 0; i < PointCount; i++ )

	{
		if( (m_FindStatus[i] == 1)/*&&( m_FitErrors[i] <500)*/)
		{
			m_CurrentModelFutures[i]->m_bLocationCorected = false;

		if (	m_CurrentModelFutures[i]->IsOutLiter())
		{
			
			if (( m_FitErrors[i] <SecondTreshold) &&
				(/*(Counter < 10)||*/(PointDistans((*it)->m_CurrentLocation, (*it)->m_TransformKeyFrameLocation) < g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans() * 0.3)
				&& (PointDistans((*it)->m_CurrentLocation, (*it)->m_Model3dProjectLocation) < g_pTrackinkSystem->m_HeadPosition.GetReferenceDistans() * 0.3))
				)
			{
				m_CurrentModelFutures[i]->m_FuterLocationSource = SecondEstymationOFState;
				m_CurrentModelFutures[i]->m_bBadFit = false;
				//m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
				m_CurrentModelFutures[i]->m_CurrentLocation = m_NewPoints[i] ;
				m_CurrentModelFutures[i]->m_Model3DState= InModelState;
			};
		}

		else
			// TODO nowa lista 
			//m_CurrentModelFutures[i]
			//while ((*it)->m_FutureState != TrackigState ) it++;
			//if (( m_FitErrors[i] <800) || (m_CurrentModelFutures[i]->m_Model3DState== NotLocalizedState)
			//	||( 10* m_FitErrors[i] < m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError)  )
		if (( m_FitErrors[i] <SecondTreshold) /*|| (m_CurrentModelFutures[i]->m_Model3DState== NotLocalizedState)*/
			/*	||( 10*m_FitErrors[i] < m_CurrentModelFutures[i]->m_bFirstOFEstymationFitError) */ )
			{


		if (!
			
			(
			( 
			
					(m_FitErrors[i] > SecondTreshold * 0.75) &&
				(PointDistans(	m_CurrentModelFutures[i]->m_PrevLocation,m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation )
				> 3* PointDistans(	m_CurrentModelFutures[i]->m_PrevLocation,m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation )) &&
				(PointDistans(	m_CurrentModelFutures[i]->m_PrevLocation,m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation) > g_TrackinkSystem->m_HeadPosition.GetReferenceDistans() *0.2)&&
				(m_CurrentModelFutures[i]->m_bFirstOFEstymationOk) &&

(PointDistans(	m_CurrentModelFutures[i]->m_Model3dProjectLocation,m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation )
				>3*  PointDistans(	m_CurrentModelFutures[i]->m_Model3dProjectLocation,m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation )) 
				
				)||
	//------------0r----------------------
		(
		0
		
//					(m_FitErrors[i] >200) &&
//					(PointDistans(	m_CurrentModelFutures[i]->m_Model3dProjectLocation,m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation ) > g_TrackinkSystem->m_HeadPosition.GetReferenceDistans() *0.3)&&
//				(m_CurrentModelFutures[i]->m_bFirstOFEstymationOk) &&
//
//(PointDistans(	m_CurrentModelFutures[i]->m_Model3dProjectLocation,m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation )
//				> 10* PointDistans(	m_CurrentModelFutures[i]->m_Model3dProjectLocation,m_CurrentModelFutures[i]->m_bFirstOFEstymationLocation )) 
//				
////		
				)
//------------0r----------------------
))
		{
	

				m_CurrentModelFutures[i]->m_FuterLocationSource = SecondEstymationOFState;
				m_CurrentModelFutures[i]->m_bBadFit = false;
				//m_CurrentModelFutures[i]->UpdatePosition( m_NewPoints[i]);
				m_CurrentModelFutures[i]->m_CurrentLocation = m_NewPoints[i] ;
				m_CurrentModelFutures[i]->m_Model3DState= InModelState;

		}
		else
		{
		// jum use first estymation
				m_CurrentModelFutures[i]->m_bMoved  = true;
					m_CurrentModelFutures[i]->m_Model3DState= InModelState;
				cout<<"!!!!!!!!"<<endl;
		};
		
		

		}
					


		
			else
			{
				m_CurrentModelFutures[i]->m_bBadFit = true;
				BadFitCount++;
			}
			//m_CurrentModelFutures[i]->m_Model3DState= InModelState;

			m_CurrentModelFutures[i]->m_bSecondOFEstymationLocation = m_NewPoints[i];
			m_CurrentModelFutures[i]->m_bSecondOFEstymationOk = true;
			m_CurrentModelFutures[i]->m_IsLocated = true;
			m_CurrentModelFutures[i]->m_bSecondOFEstymationFitError = m_FitErrors[i];;
			m_CurrentModelFutures[i]->m_FitError += m_FitErrors[i];
		}
		else
		{
	m_CurrentModelFutures[i]->m_Model3DState= NotLocalizedState;
			m_CurrentModelFutures[i]->m_FuterLocationSource = Model3dState;
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

	CheckFutersMove();
	
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




















