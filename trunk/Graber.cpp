///////////////////////////////////////////////////////////
//  CGraber.cpp
//  Implementation of the Class CGraber
//  Created on:      10-lut-2009 15:39:54
///////////////////////////////////////////////////////////

#include "Graber.h"
#include <string>
using namespace std;
#include "Utilitis.h"
#include "cv.h"
#include "highgui.h"
#define USE_ORGINAL_FRAME_SIZE 0
//============================================================================

CGraber g_Graber;

CGraber::CGraber()
{
	m_pFrameCopy =NULL;
	m_ImageSourceType = GS_CAM;
	m_pCapture = 0;
	m_szFileName = "";
	m_iWith = 320;
	m_iHeight = 240;

	m_iWith = 640;
	m_iHeight = 480;
	m_pGrayFrame = NULL;

	m_bReziseFrame = false;
}

//============================================================================

CGraber::~CGraber()
{
	cvReleaseCapture( &m_pCapture );
}

//===========================================================================


void CGraber::Init()
{

	//	m_pGrayFrame =  cvCreateImage(cvSize(m_iWith,m_iHeight), 8,1);
	switch(m_ImageSourceType)
	{
	case GS_CAM : 
		{
			cout<<"source : camera"<<endl;
			m_pCapture = cvCaptureFromCAM( 0);
			cvSetCaptureProperty(m_pCapture,CV_CAP_PROP_FRAME_WIDTH,m_iWith) ;
			cvSetCaptureProperty(m_pCapture,CV_CAP_PROP_FRAME_HEIGHT,m_iHeight) ;
			cvSetCaptureProperty(m_pCapture,CV_CAP_PROP_FPS,30) ;

			if( !m_pCapture )
				throw "camera inicalization fail!";
			break;
		}
	case GS_AVI:
		{
			cout<<"source  file:"<< m_szFileName<<endl;
			//	m_pCapture = cvCaptureFromAVI( m_szFileName.c_str() );
			m_pCapture = cvCaptureFromAVI( m_szFileName);
			if( !m_pCapture )
				throw "loading avi file fail!";
			break;
		}
	case GS_IMG:
		{
			cout<<"source  file:"<< m_szFileName<<endl;
			m_pFrame = cvLoadImage( m_szFileName, 1 );
			if( !m_pFrame )
				throw "loading image file fail!";
			break;
		}
	}

	getNextFrame(&m_pFrame); // get first frmae, nesserre for proper bufer size inicialization
	double fps = cvGetCaptureProperty( m_pCapture, CV_CAP_PROP_FPS);
		cout<<"FPS:"<<(int)fps<<endl;
}
//============================================================================


void  CGraber::UpdateBuffers()
{
	 m_bFrameEqualizeHistComputed = false;
	 m_bFrameCopyComputed = false;

	if (m_pGrayFrame ==NULL)
	{

		if (USE_ORGINAL_FRAME_SIZE)
		{
		m_iWith = m_pOrginalFrame->width;
		(m_iHeight = m_pOrginalFrame->height);
		};

		if ((m_iWith == m_pOrginalFrame->width) &&  (m_iHeight == m_pOrginalFrame->height) )
		{
			m_bReziseFrame = false;
		}
		else
		{

			m_bReziseFrame = true;
			m_pFrame =  cvCreateImage(cvSize(m_iWith,m_iHeight), 8,3);
		}
		//m_iWith = 	m_pFrame->width;
		//m_iHeight = m_pFrame->height;

	
		m_pGrayFrame			=  cvCreateImage(cvSize(m_iWith,m_iHeight), 8,1);
		m_pGrayFrameEqualizeHist =   cvCreateImage(cvSize(m_iWith,m_iHeight), 8,1);
		m_pPrevGrayFrame =  cvCreateImage(cvSize(m_iWith,m_iHeight), 8,1);
	};


	if (m_bReziseFrame)
		cvResize( m_pOrginalFrame, m_pFrame, CV_INTER_LINEAR );
	else
		m_pFrame = m_pOrginalFrame;

if (m_pFrameCopy ==NULL)
{
		m_pFrameCopy = cvCreateImage(cvSize(m_iWith,m_iHeight), m_pFrame->depth,m_pFrame->nChannels);
		m_pTmpDebug = cvCreateImage(cvSize(m_iWith,m_iHeight),8, 1);
		m_pTmpDebug2 = cvCreateImage(cvSize(m_iWith,m_iHeight),8, 1);
};

	
cvResetImageROI(m_pFrame);
cvResetImageROI(m_pGrayFrame);
cvResetImageROI(m_pGrayFrameEqualizeHist);
cvResetImageROI(m_pFrameCopy);

IplImage *tmp;
	tmp = m_pPrevGrayFrame;
	m_pPrevGrayFrame = m_pGrayFrame;
	m_pGrayFrame = tmp;
	//cvCopyImage(m_pFrame,m_pFrameCopy);
	cvCvtColor( m_pFrame, m_pGrayFrame, CV_BGR2GRAY );
	//cvEqualizeHist( m_pGrayFrame, m_pGrayFrameEqualizeHist );

	//-------------------------debug------------------------------------
	//cvSub( m_pPrevGrayFrame,m_pGrayFrame,m_pTmpDebug );

	//cvNormalize(m_pTmpDebug,m_pTmpDebug,250,0,CV_MINMAX);
	//g_GUI.ShowImage(m_pTmpDebug,"dif");

	//cvSmooth(m_pPrevGrayFrame, m_pTmpDebug, CV_BILATERAL,5);


	////cvNormalize(m_pTmpDebug2,m_pTmpDebug2,250,0,CV_MINMAX);
	//g_GUI.ShowImage(m_pTmpDebug,"smoof dif");


//	cvSub( m_pTmpDebug,m_pTmpDebug2,m_pTmpDebug );
//cvNormalize(m_pTmpDebug,m_pTmpDebug,250,0,CV_MINMAX);
//	g_GUI.ShowImage(m_pTmpDebug,"dif difu dif");



	//-------------------------------------------------------------


}
//============================================================================
IplImage * CGraber::getFrameEqualizeHist()
{
	if (! m_bFrameEqualizeHistComputed)
	{
		m_bFrameEqualizeHistComputed = true;
			cvEqualizeHist( m_pGrayFrame, m_pGrayFrameEqualizeHist );
	}
return m_pGrayFrameEqualizeHist;
}
//============================================================================
IplImage * CGraber::getFrameCopy()
{
	if (! m_bFrameCopyComputed)
	{
		m_bFrameCopyComputed = true;
			cvCopyImage(m_pFrame,m_pFrameCopy);
	}
return m_pFrameCopy;
}
//============================================================================


bool CGraber::getNextFrame(IplImage **outFrame)
{

	switch(m_ImageSourceType)
	{

	case GS_IMG:
		{
			*outFrame = m_pFrame;
			return false;
			if( !m_pFrame )
				throw "loading image file fail!";
			break;
		}
	default : 
		{
			if( m_pCapture )
			{
				if( cvGrabFrame( m_pCapture ))	
				{
					m_pOrginalFrame = cvRetrieveFrame( m_pCapture );
					//m_pFrame  = cvRetrieveFrame( m_pCapture );
					UpdateBuffers();
					outFrame = &m_pFrame;
					return true;
				}
				else
					return false;
			}
			else
			{
				cout<<"twhro"<<endl;
				throw "capture not initialized!";

			};
			break;
		}
	}
	return false;
}
//============================================================================
//
//void CGraber::setSorceFileName(string & fileName){
//
//}
//============================================================================

bool CGraber::setFrameSize(int x, int y ){

	return  0;
}

//============================================================================
bool CGraber::setFrameSoruce(int SourceType){

	return  0;
}
//============================================================================
void CGraber::Stop(){
	cvReleaseCapture( &m_pCapture );

}
//============================================================================


