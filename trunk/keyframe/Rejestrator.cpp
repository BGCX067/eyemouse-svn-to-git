#include "Rejestrator.h"
#include "CTrackinkSystem.h"
//
CRejestrator::CRejestrator(void)
{
	
	m_pVidoWriter = NULL;
}
//
//CRejestrator::~CRejestrator(void)
//{
//}

	//==================================================================

	void CRejestrator::SaveFrame(	IplImage *Frame)
	{
	g_TrackinkSystem->SaveKeyPresed();
	Frame = 	g_TrackinkSystem->m_Graber.getFrameCopy();
	if(m_pVidoWriter == NULL)
		m_pVidoWriter = cvCreateVideoWriter( "history.avi",  0/*CV_FOURCC('P','I','M','1')*/   ,30, cvSize(Frame->width,Frame->height) );

	cvWriteFrame(m_pVidoWriter, Frame );
	};
