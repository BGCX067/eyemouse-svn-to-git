#pragma once
#include "cv.h"
#include "highgui.h"

enum GraberSouceType { GS_CAM, GS_AVI, GS_IMG };

//class CvCapture;
//struct IplImage;


class CFrame
{
IplImage *m_pFrame;
IplImage *m_pGrayFrame;

//IplImage *m_pGrayFrame;
//IplImage *m_pGrayFrame;


};


class CGraber
{
		IplImage *m_pFrameCopy; // tmp to debug plot
			IplImage *m_pGrayFrameEqualizeHist;
	//TODO acess
	public:
	CvCapture* m_pCapture ;
	char* m_szFileName;	
	IplImage *m_pOrginalFrame;
	IplImage *m_pFrame;

	IplImage *m_pGrayFrame;


	IplImage *m_pTmpDebug;
		IplImage *m_pTmpDebug2;

	IplImage *m_pPrevGrayFrame;



bool m_bFrameEqualizeHistComputed;
bool m_bFrameCopyComputed;

	int m_iWith, m_iHeight;
	bool m_bReziseFrame;
public:


	GraberSouceType m_ImageSourceType;
	CGraber();
	virtual ~CGraber();

	void Init();
	void setSorceFileName(char* fileName);
	bool setFrameSize(int x, int y );
	bool setFrameSoruce(int SourceType);
	bool getNextFrame(IplImage **outFrame);
	IplImage * CGraber::getFrameEqualizeHist();
	IplImage * CGraber::getFrameCopy();

	void UpdateBuffers();

	void Stop();

};

extern CGraber g_Graber;