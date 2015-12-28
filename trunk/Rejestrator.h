#pragma once
#include "Utilitis.h"

class CRejestrator
{
	
public:
	CvVideoWriter *m_pVidoWriter;

		CvVideoWriter *m_pDebugVidoWriter;
	//==================================================================
	CRejestrator(void);
public:
	//==================================================================
	~CRejestrator(void)
	{
		cout<<"relase save file"<<endl;
	cvReleaseVideoWriter(&m_pVidoWriter);
	};
	//==================================================================

	void SaveFrame(	IplImage *Frame);
		void CRejestrator::SaveDebugFrame(	IplImage *Frame);

	//==================================================================
};
