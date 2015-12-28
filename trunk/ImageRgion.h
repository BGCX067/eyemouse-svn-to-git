#pragma once
#include "cv.h"
#include "highgui.h"

class CImageRgion;
class CImageRgion
{
	//TODO scale
	
	CImageRgion *m_pImageRgionParent;

	IplImage *m_pImgCopy;
	

	bool m_bOwnsBuffer; // czy obraz jest w wewnetrznym buforze;
	bool m_bIsLock;

	bool m_bParrentHaveROI;
public:
	IplImage **m_pImgParent;
	float m_fScale;
	CImageRgion(IplImage **ImgParent,float Scale = 1,bool CopyImage = false );

	CImageRgion::CImageRgion(IplImage ** ImgParent, CImageRgion * ImageRgionParent, float  Scale= 1);

	/*CImageRgion(IplImage * img);*/
	~CImageRgion(void);

	CvRect m_ROI;


	CvPoint getRoiCenter()
	{
		CvPoint result;
		result.x = m_ROI.x + m_ROI.width/2;
		result.y = m_ROI.y + m_ROI.height/2;
	return result;
	}
	
	IplImage *getImg();
	void Lock();

	void Unlock();

	void Update();

	CvRect GetParentRect( CvRect & rect);
	CvRect CImageRgion::GetParentRect( CvRect & rect , CvRect & ParrentROI);
	CvPoint CImageRgion::GetParentPoint( CvPoint & point);
	CvPoint2D32f CImageRgion::GetParentPointFloat( CvPoint & point);
	CvPoint2D32f CImageRgion::GetParentPointFloat( CvPoint2D32f & point);

	void SetROI( CvRect & roi);

	void ResetROI( );

	virtual void AllocateBuffer();


};
