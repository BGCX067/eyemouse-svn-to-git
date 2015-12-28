#pragma once
#include "GUI.h"
#include "cv.h"
#include "highgui.h"
#include <vector>
#include <list>
#include <iostream>
#include "params.h"
#include "ImageRgion.h"

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
using namespace std;
using namespace boost;
#define SAFE_DELETE(prt) if(prt) delete prt; prt = NULL;class

class CTrackinkSystem;
extern CTrackinkSystem* g_pTrackinkSystem;
//==========================================================================
CvPoint2D32f PointMul(CvPoint2D32f a, CvPoint2D32f b );
CvPoint2D32f PointMul(CvPoint2D32f a, float b );
CvPoint2D32f PointAdd(CvPoint2D32f a, CvPoint2D32f b );
CvPoint2D32f PointSub(CvPoint2D32f a, CvPoint2D32f b );

CvPoint3D32f PointSub(CvPoint3D32f a, CvPoint3D32f b );

//==========================================================================

double PointDistans(CvPoint2D32f a, CvPoint2D32f b );
bool PointInRect(CvPoint2D32f a, CvRect rect );
bool PointInElipse(CvPoint2D32f a,CvBox2D32f Elipse, float margin = 0);
