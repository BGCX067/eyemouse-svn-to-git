#pragma once

#include "cv.h"
#include "highgui.h"
#include "ImageRgion.h"

class CEyePosition
{
public:
	CEyePosition(void);
public:
	~CEyePosition(void);

	CvPoint m_IrisCenter;
	CvPoint m_LeftCorner;
    CvPoint m_RightCorner;

	float m_fVerticalAngle;
	float m_fHorizonalAngle;

	void Update(CImageRgion * EyeImg);

	void CEyePosition::DrawSurf(IplImage *ImgIn, IplImage *ImgOut);


};
