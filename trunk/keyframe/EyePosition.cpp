#include "EyePosition.h"


#include "CFaceExtractor.h"
#include "assert.h"
#include "cv.h"
#include "highgui.h"
#include "GUI.h"
#include "Graber.h"
//================================================
CEyePosition::CEyePosition(void)
{
}
//================================================
CEyePosition::~CEyePosition(void)
{
}


//================================================
void CEyePosition::Update(CImageRgion *  EyeImg)
{


	IplImage *gray = cvCreateImage(cvSize( EyeImg->m_ROI.width, EyeImg->m_ROI.height), 8, 1 );

	cvCvtColor( EyeImg->getImg(), gray, CV_BGR2GRAY );

	DrawSurf(gray,EyeImg->getImg());

	//EyeImg->Unlock();


}
//================================================

void CEyePosition::DrawSurf(IplImage *ImgIn, IplImage *ImgOut)
{
	assert(ImgIn) ;


	//	g_Graber.m_pGrayFrame;

	CvSeq *objectKeypoints = 0, *objectDescriptors = 0;



	//cvEqualizeHist( gray,gray );

	//g_GUI.ShowImage(gray, "LeftEyegray");
	CvMemStorage* storage = cvCreateMemStorage(0);
	//	cvClearMemStorage( storage );

	CvSURFParams aa = cvSURFParams(500, 1);



	cvExtractSURF( ImgIn, 0, &objectKeypoints, &objectDescriptors, storage, aa );



	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq( objectKeypoints, &kreader );
	cvStartReadSeq( objectDescriptors, &reader );
	std::cout<<"point count:"<<objectDescriptors->total<<std::endl;
	for( i = 0; i < objectDescriptors->total; i++ )
	{
		const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
		kp->pt;
		const float* descriptor = (const float*)reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );

		cvCircle( ImgOut,cvPointFrom32f( kp->pt), 2, CV_RGB(250,0,0), -1, 8,0);

	}

	cvClearMemStorage( storage );
}

//===========================================================================
