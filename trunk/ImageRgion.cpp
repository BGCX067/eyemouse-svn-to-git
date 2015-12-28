#include "ImageRgion.h"

//============================================================================
CImageRgion::CImageRgion(IplImage ** ImgParent, float Scale ,bool CopyImage)
{
	m_pImageRgionParent = NULL;
	m_bParrentHaveROI =false;
	m_pImgParent = ImgParent;
	m_bIsLock = false;
	m_fScale = Scale;
	m_bOwnsBuffer = false;
	//m_pImgParent = NULL;
	ResetROI();

	//m_bOwnsBuffer =  Scale != 1 ;
	m_bOwnsBuffer = m_bOwnsBuffer ||CopyImage;

	if (m_bOwnsBuffer)
		AllocateBuffer();
}

//============================================================================
CImageRgion::CImageRgion(IplImage ** ImgParent, CImageRgion * ImageRgionParent, float Scale )

{
	m_bParrentHaveROI =true;
	m_pImageRgionParent = ImageRgionParent;
	m_pImgParent = ImgParent;
	m_bIsLock = false;
	m_fScale = Scale;
	m_bOwnsBuffer = false;
	//m_pImgParent = NULL;
	ResetROI();

	////m_bOwnsBuffer =  Scale != 1 ;
	//m_bOwnsBuffer = m_bOwnsBuffer ||CopyImage;

	//if (m_bOwnsBuffer)
	//	AllocateBuffer();
}



//============================================================================
CImageRgion::~CImageRgion(void)
{
	if (m_bOwnsBuffer)
		cvReleaseImage(&m_pImgCopy );

}


//============================================================================
void CImageRgion::Lock()
{
	//assert(!m_bIsLock);
	m_bIsLock = true;
	if (!m_bOwnsBuffer)
	{
		cvResetImageROI(*m_pImgParent);
		cvSetImageROI(*m_pImgParent, m_ROI );
	}
	else
		Update();

};

//============================================================================
void CImageRgion::Unlock()
{
	m_bIsLock = true;
	if (!m_bOwnsBuffer)
		cvResetImageROI(*m_pImgParent);
}


//============================================================================
void CImageRgion::Update()
{
	if (!m_bOwnsBuffer)


		if (m_fScale  = 1)
		{
			cvCopy(*m_pImgParent,m_pImgCopy );
		}
		else
		{
			cvSetImageROI(*m_pImgParent, m_ROI );
			if ((m_pImgCopy->width !=m_ROI.width )||(m_pImgCopy->height !=m_ROI.height ))
			{
				cvReleaseImage(&m_pImgCopy );
				AllocateBuffer();
			};
			cvResize( *m_pImgParent, m_pImgCopy, CV_INTER_LINEAR );
			cvResetImageROI(*m_pImgParent );
		};
}
//============================================================================
CvRect CImageRgion::GetParentRect( CvRect & rect)
{

		return cvRect(    m_ROI.x* m_fScale +  rect.x * m_fScale ,    m_ROI.y * m_fScale+  rect.y * m_fScale ,  rect.width * m_fScale  , rect.height * m_fScale   );

}

//============================================================================
CvRect CImageRgion::GetParentRect( CvRect & rect , CvRect & ParrentROI)
{

	if (m_bParrentHaveROI)
		return cvRect(   ParrentROI.x +  m_ROI.x +  rect.x * m_fScale ,
		 ParrentROI.y + m_ROI.y +  rect.y * m_fScale
		,  rect.width * m_fScale  , rect.height * m_fScale   );

}

//============================================================================
CvPoint CImageRgion::GetParentPoint( CvPoint & point)
{
	
	return cvPoint(    m_ROI.x +  point.x / m_fScale ,    m_ROI.y +  point.y / m_fScale );
}
//============================================================================
CvPoint2D32f CImageRgion::GetParentPointFloat( CvPoint & point)
{
	CvPoint2D32f result;
	result.x = float(m_ROI.x +  point.x) / float(m_fScale);
	result.y = float(m_ROI.y +  point.y) / float(m_fScale);
	return result;
}

//============================================================================
CvPoint2D32f CImageRgion::GetParentPointFloat( CvPoint2D32f & point)
{
	CvPoint2D32f result;
	result.x = float(m_ROI.x +  point.x) / float(m_fScale);
	result.y = float(m_ROI.y +  point.y) / float(m_fScale);
	return result;
}


//============================================================================
//CvRect CImageRgion::GetOrginalRect( CvRect & rect, float scale)
//{
//	return cvRect(    m_ROI.x +  rect.x / m_fScale ,    m_ROI.y +  rect.y / m_fScale ,  rect.width / m_fScale  , rect.height / m_fScale   );
//}
//============================================================================
void CImageRgion::SetROI( CvRect & roi)
{
	m_ROI = roi	;
}
//============================================================================
void CImageRgion::ResetROI( )
{
	assert(*m_pImgParent);
	m_ROI = cvRect(0,0,(*m_pImgParent)->width,(*m_pImgParent)->height )	;
}
//============================================================================

IplImage * CImageRgion::getImg()
{
	//assert(m_bIsLock);
	if (!m_bOwnsBuffer)
	{
		return (*m_pImgParent);


	}
	else
	{
		return m_pImgCopy;
	}

}
//============================================================================
void CImageRgion::AllocateBuffer()
{
	/*	int ChannelCount;
	isGray? ChannelCount = (*m_pImgParent)->nChannels : ChannelCount = 1;*/

	m_pImgCopy = cvCreateImage(cvSize((*m_pImgParent)->width,(*m_pImgParent)->height),8,(*m_pImgParent)->nChannels );

};
//============================================================================