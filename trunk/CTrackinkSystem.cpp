///////////////////////////////////////////////////////////
//  CTrackinkSystem.cpp
//  Implementation of the Class CTrackinkSystem
//  Created on:      10-lut-2009 15:40:48
//  Original author: Szymon
///////////////////////////////////////////////////////////

#include "CTrackinkSystem.h"
#include "Utilitis.h"
#include <iostream>
using namespace std;
const int InitIteration = 5;
bool g_PlotAllDebug = false;

CTrackinkSystem* g_TrackinkSystem;

CTrackinkSystem::CTrackinkSystem(){
	m_CurentFaze = InitFaze;
	m_IterationCount = 0;
	m_bCalibrationFaze = true;

	//TODO uncoment
	//m_bCalibrationFaze = false;
	m_pCalibrationScreen = cvCreateImage( cvSize(800,600), 8, 3);
	g_TrackinkSystem = this;
	m_bKeyInitModel3D = false;
	m_bKeyStartCalibration = false;
	m_bKeyChangeRef3DPoint = false;
	m_bKeyPause = false;
	m_bKeyStartCalibrationHeadAngle = false;
	m_bKeyStartCalibrationHeadPosition = false;
	m_bKeyClearPlot = false;
	m_bKeyDebugPlot = false;
	m_bKeyPaint = true;
}

//===========================================================================
CTrackinkSystem::~CTrackinkSystem(){

}



//===========================================================================
void CTrackinkSystem::ShowCalibrationPoint()
{
	
}


//===========================================================================
bool  CTrackinkSystem::Process()
{


	CvPoint2D32f EyeCornerPoint;
	CvPoint2D32f EyesCenter;
	CvScalar White, Red, Green, Blue, Yellow;
	White = CV_RGB(255,255,255);
	Red = CV_RGB(255,0,0);
	Green = CV_RGB(0,255,0);
	Blue = CV_RGB(0,0,255);
	Yellow = CV_RGB(255,255,0);

	IplImage **TmpFrame;


	
	if (m_bKeyInitModel3D) 
	{
		m_IterationCount = 0;
		m_CurentFaze = InitFaze;
	};

	if (m_bKeyStartCalibration)
	{
		m_CurentFaze = CalibrationFaze;
		m_CalibrationPlot.Init(CalibrationMesureEyeAngle);
		m_GazeMaping.m_Caliblator.Init();	
	};

	if ( m_bKeyStartCalibrationHeadAngle  ) 
	{
		m_CurentFaze = CalibrationFaze;
		m_CalibrationPlot.Init(CalibrationMesureHeadAngle);
		m_GazeMaping.m_Caliblator.Init();	
	};


	if ( m_bKeyStartCalibrationHeadPosition  ) 
	{
		m_CurentFaze = CalibrationFaze;
		m_CalibrationPlot.Init(CalibrationMesureHeadPosition);
		m_GazeMaping.m_Caliblator.Init();	
	};


	if (m_CurentFaze == CalibrationFaze)
	{

		if ( !m_CalibrationPlot.PlotNextPoint())
		{

			m_CurentFaze  = TrackingFaze;
			m_GazeMaping.m_Caliblator.ComputeCalibration();

			//if (m_CalibrationPlot.m_CalibrationType == CalibrationMesureEyeAngle)
			//{
			//m_GazeMaping.m_Caliblator.ComputeCalibration();
			//m_CurentFaze = CalibrationFaze;
			//m_CalibrationPlot.Init(CalibrationMesureHeadAngle);
			//
			////m_GazeMaping.m_Caliblator.Init();	
			//}
			//else
			//{
			//m_CurentFaze  = TrackingFaze;
			//m_GazeMaping.m_Caliblator.ComputeCalibration();
			//};
			//m_GazeMaping.m_Caliblator.PlotCalibrationResult();
		}

	};

	//----------------------------------------

	if (m_Graber.getNextFrame(TmpFrame))
	{
		m_Timer.UpdateFps();
		M_FaceExtractor.Update();
		TmpFrame = &m_Graber.m_pFrame;
		m_VideoRejestrator.SaveFrame(m_Graber.m_pFrame);
		//return true;
		//if (m_IterationCount > 100000)
		//	return false;
		//g_GUI.ShowImage(m_Graber.m_pFrame, "frame");


if (	m_CurentFaze == InitFaze)
{
	bool InitOk = true;
	M_FaceExtractor.FindFactialFuters();
	if (!M_FaceExtractor.m_bFaceIsDetected || !M_FaceExtractor.m_bLeftEyeIsDetected || !M_FaceExtractor.m_bRightEyeIsDetected )
	{
		cout<<"face init fail"<<endl;
		return true;
	}
	if (!m_GazeDetector.InitPupilsCenters())
	{
		cout<<" gaze detector init fail"<<endl;
		return true;
	}
	if (!m_PointTracker.Initialize())
	{
		cout<<"point tracker init fail"<<endl;
		return true;
	}
	if (!m_HeadPosition.InitModel())
	{
		cout<<" 3d model init fail"<<endl;
		return true;
	}
		m_CurentFaze = TrackingFaze;
		return true;
};

//if (m_CurentFaze == TrackingFaze)
{

		m_PointTracker.Update(M_FaceExtractor.m_pFaceImg->getImg(), M_FaceExtractor.m_FaceRect);	

		if (m_PointTracker.m_vTrackingFutures.size() < 20) 
			throw("LLL"); 


		


		m_HeadPosition.Update();
		m_GazeMaping.Update();

		m_GazeDetector.UpdatePupilsCenters();
		if (g_TrackinkSystem->m_bKeyDebugPlot)
		{
			m_GazeDetector.PlotLeftEye();
			m_GazeDetector.PlotRightEye();
		};



	
};

		if (m_CurentFaze == CalibrationFaze)
			m_GazeMaping.UpdateCalibration();

		m_IterationCount++;
		cvResetImageROI(m_Graber.m_pFrame);

	};
	return true;
}

//===========================================================================


void CTrackinkSystem::Start(void)
{
	//	m_Graber.m_iWith = 960;
	//	m_Graber.m_iHeight = 720;
	//	m_Graber.m_iWith = 1024;
	//	m_Graber.m_iHeight = 768;

		m_IterationCount = 0; 
	string Files[] = { "HeadMove.avi", "HeadMove2.avi", "video 7.avi", "video 10.avi", "video 11.avi","video 12.avi","video 13.avi","video 14.avi" };
	m_Graber.m_iWith = 1600;
	m_Graber.m_iHeight = 1200;

	m_Graber.m_iWith = 800;
	m_Graber.m_iHeight = 600;


	m_Graber.m_iWith = 320;
	m_Graber.m_iHeight = 240;

	m_Graber.m_iWith = 640;
	m_Graber.m_iHeight = 480;
	//m_Graber.m_iWith = 800;
	//m_Graber.m_iHeight = 600;

	//m_Graber.m_iWith = 1200;
	//m_Graber.m_iHeight = 900;
	//m_Graber.m_ImageSourceType = GS_AVI;
	//m_Graber.m_szFileName = "0.avi";

	string InputName;
	cout<<"Input :";
	cin>>InputName;

	if ((InputName == "cam") || (InputName == ""))
		m_Graber.m_ImageSourceType = GS_CAM;
	else
	{
		m_Graber.m_ImageSourceType = GS_AVI;
		if ((InputName == "sample" )||(InputName == "s" ))
		{
			int SampleNr;
			cout<<"Sample Nr :";
			cin>>SampleNr;
			m_Graber.m_szFileName = (char*)Files[SampleNr].c_str();
		}
		else
		{
			m_Graber.m_szFileName = (char*) InputName.c_str();

		};
	};

	//m_Graber.m_szFileName = "sample.avi";
	//m_Graber.m_szFileName = "video 13.avi";//	m_Graber.m_szFileName = "test.avi";
	//m_Graber.m_szFileName = (char*) InputName.c_str();


	m_Graber.Init();


	M_FaceExtractor.m_pFrame = &m_Graber.m_pFrame;
	M_FaceExtractor.Init(m_Graber.m_pFrame->width, m_Graber.m_pFrame->height);
	m_PointTracker.Init( cvSize(m_Graber.m_iWith,m_Graber.m_iHeight),cvSize(200,200));
	m_PointTracker.m_fScale =M_FaceExtractor.m_fScale;

	//m_LeftEyeCorner.Init(&m_PointTracker);
	//	m_HeadCenter.Init(&m_PointTracker);

	m_HeadPosition.Init(&m_PointTracker);

	g_GUI.ShowParamWindows();
	int key;
	while(1)
	{
		if (!m_bKeyPause || m_bKeyNextFrame)
			Process();
		m_bKeyInitModel3D = false;
		m_bKeyStartCalibration = false;
		m_bKeyStartCalibrationHeadAngle = false;
		m_bKeyStartCalibrationHeadPosition = false;
		m_bKeyChangeRef3DPoint = false;
		m_bKeyNextFrame = false;
		m_bKeyClearPlot = false;

		if(( key = cvWaitKey( 10 )) >= 0 )
		{

			char b = 'i';
			if ((char)key == 'a')
				m_bKeyClearPlot = true;


			if ((char)key == 'i')
				m_bKeyInitModel3D = true;

			if ((char)key == 'z')
				m_bKeyChangeRef3DPoint = true;

			if ((char)key == 'p')
				m_bKeyPause = !m_bKeyPause;

			if ((char)key == 'n')
				m_bKeyNextFrame = true;


			if ((char)key == 'c')
				m_bKeyStartCalibration = true;


			if ((char)key == ' ')
				m_bKeyPaint = !m_bKeyPaint;

			if ((char)key == 'v')
				m_bKeyStartCalibrationHeadAngle = true;

			if ((char)key == 'b')
				m_bKeyStartCalibrationHeadPosition = true;

				if ((char)key == 'd')
						m_bKeyDebugPlot = !m_bKeyDebugPlot; 




			if ((char)key == 'x')
			{
				m_Graber.Stop();
				return;
			}


		



		}
		LoadKeyPresed();


	};
}
//===========================================================================

//write kode presed to saved freame

void CTrackinkSystem::SaveKeyPresed(void)
{

	IplImage *Img = 	m_Graber.getFrameCopy();
	uchar* ptr = (uchar*) (Img->imageData );
	//);
	//for( int x=0; x<img->width; x++ ) {
	//ptr[3*x+1] = 255;
	//ptr[3*x+2] = 255;
	//}
	//}


	//float* ptr = (float*) (EigImg->imageData + y * EigImg->widthStep);
	////	// cv::minMaxLoc(tmp, 0, &MaxVal, 0, 0 ,mask);
	//for( int y=0; y<EigImg->height; y++ ) {
	//float* ptr = (float*) (EigImg->imageData + y * EigImg->widthStep);
	//float* ptr2 = (float*) (TmpImg->imageData + y * TmpImg->widthStep);
	//for( int x=0; x<EigImg->width; x++ )
	//{
	//	double distans = sqrt(4*(InitGuas.x - x)*(InitGuas.x - x) + (InitGuas.y - y)*(InitGuas.y - y));
	//			distans = distans/(TresholdDistans);
	//
	//			//if (x > 100)
	//			ptr[x] = ptr[x] - MaxVal* distans* distans*distans* distans /* +  MaxVal/2 */ ;
	//		ptr2[x] =	ptr[x]  ;
	//}
	//}
	//g_GUI.ShowImage(TmpImg,"TmpImg TmpImg TmpImg");
	//

	//	cvCircle( m_pCalibrationScreen,m_CurrentCalibrationScreenPoint, 5, CV_RGB(200,0,0), -1, 8,0);	


	//cvCircle( Img,cvPoint(5,5), 15, CV_RGB(251,251,251), -1, 8,0);	

	if (m_bKeyInitModel3D)
	{
		cvCircle(Img,cvPoint(1,1), 3, CV_RGB(255,255,255), -1, 8,0);	
		ptr[0] = 255;
		ptr[1] = 255;
		ptr[2] = 255;
	};

	if (m_bKeyStartCalibration)
	{
		cvCircle( Img,cvPoint(1,1), 3, CV_RGB(0,255,0), -1, 8,0);	
		ptr[0] = 0;
		ptr[1] = 255;
		ptr[2] = 0;
	};


}
//===========================================================================

//write kode presed to saved freame

void CTrackinkSystem::LoadKeyPresed(void)
{
	IplImage *Img = 	m_Graber.m_pFrame;
	int offset =3;
	uchar* ptr = (uchar*) (Img->imageData );

	if (
		ptr[0 + offset] > 240 &&
		ptr[1+ offset] > 240 &&
		ptr[2+ offset] > 240 )
		m_bKeyInitModel3D = true;

	if (
		ptr[0+ offset] < 10 &&
		ptr[1+ offset] >  240 &&
		ptr[2+ offset] < 10 )
		m_bKeyStartCalibration = true;

	if (
		ptr[0+ offset] >  240 &&
		ptr[1+ offset] >  240 &&
		ptr[2+ offset] >  240 )
		cout<<"dsf"<<endl;




}