#pragma once
#include "Utilitis.h"

class CRejestrator
{
	
public:
	CvVideoWriter *m_pVidoWriter;
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
	//==================================================================
};
