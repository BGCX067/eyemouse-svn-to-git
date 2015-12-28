#include "Rejestrator.h"
#include "CTrackinkSystem.h"
//
CRejestrator::CRejestrator(void)
{
	
	m_pVidoWriter = NULL;
	m_pDebugVidoWriter = NULL;
}
//
//CRejestrator::~CRejestrator(void)
//{
//}

	//==================================================================

	void CRejestrator::SaveFrame(	IplImage *Frame)
	{
return;
	g_TrackinkSystem->SaveKeyPresed();
	Frame = 	g_TrackinkSystem->m_Graber.getFrameCopy();
	if(m_pVidoWriter == NULL)
		m_pVidoWriter = cvCreateVideoWriter( "history.avi",  CV_FOURCC('D', 'I', 'V', 'X')    ,12, cvSize(Frame->width,Frame->height) );

	cvWriteFrame(m_pVidoWriter, Frame );
	};

	
	//==================================================================

	void CRejestrator::SaveDebugFrame(	IplImage *Frame)
	{
	//Frame = 	g_TrackinkSystem->m_Graber.getFrameCopy();
	if(m_pDebugVidoWriter == NULL)
		m_pDebugVidoWriter = cvCreateVideoWriter( "Debug.avi",CV_FOURCC('D', 'I', 'V', 'X')    ,20, cvSize(Frame->width,Frame->height) );

	cvWriteFrame(m_pDebugVidoWriter, Frame );
	};


		
